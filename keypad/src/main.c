#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include "usb_descriptors.h"

// CHIP-8 keypad buttons: { GPIO pin, HID keycode }
typedef struct {
  uint8_t pin;
  uint8_t keycode;
} key_map_t;

static const key_map_t keys[] = {
  { 28, HID_KEY_1 },
  { 27, HID_KEY_2 },
  { 26, HID_KEY_3 },
  { 22, HID_KEY_C },
  { 21, HID_KEY_4 },
  { 20, HID_KEY_5 },
  { 19, HID_KEY_6 },
  { 18, HID_KEY_D },
};
#define NUM_KEYS (sizeof(keys) / sizeof(keys[0]))

enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

int main(void)
{
    board_init();

    // init all keypad GPIOs as pulled-up inputs (active-low)
    for (uint8_t i = 0; i < NUM_KEYS; i++)
    {
      gpio_init(keys[i].pin);
      gpio_set_dir(keys[i].pin, GPIO_IN);
      gpio_pull_up(keys[i].pin);
    }

    // init device stack on configured roothub port
    const tusb_rhport_init_t rh_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUD_OPT_HIGH_SPEED ? TUSB_SPEED_HIGH : TUSB_SPEED_FULL
    };
    TU_ASSERT(tud_rhport_init(BOARD_TUD_RHPORT, &rh_init));
    board_init_after_tusb();

    while (1)
    {
        tud_task();
        led_blinking_task();
        hid_task();
    }
}

// Device callbacks
void tud_mount_cb(void)   { blink_interval_ms = BLINK_MOUNTED; }
void tud_umount_cb(void)  { blink_interval_ms = BLINK_NOT_MOUNTED; }

void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

// USB HID
#if TUSB_VERSION_NUMBER > 1800
// board_millis has been removed from tinyusb. Use tusb_time_millis_api instead
#define board_millis tusb_time_millis_api
#endif

// Every 10ms, scan the keypad and send one keyboard report
void hid_task(void)
{
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  if ( !tud_hid_ready() ) return;

  // Build the pressed-key list (up to 6 simultaneous, per HID boot keyboard)
  uint8_t keycode[6] = { 0 };
  uint8_t count = 0;
  bool any_pressed = false;

  for (uint8_t i = 0; i < NUM_KEYS && count < 6; i++)
  {
    if (gpio_get(keys[i].pin) == 0)   // active-low: pressed = 0
    {
      keycode[count++] = keys[i].keycode;
      any_pressed = true;
    }
  }

  static bool had_key = false;

  if (any_pressed)
  {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
    had_key = true;
  }
  else
  {
    // send one empty report on release
    if (had_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
    had_key = false;
  }
}

// GET_REPORT — not implemented
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance; (void) report_id; (void) report_type; (void) buffer; (void) reqlen;
  return 0;
}

// SET_REPORT — handle keyboard LED state (capslock etc.)
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT && report_id == REPORT_ID_KEYBOARD)
  {
    if ( bufsize < 1 ) return;
    uint8_t const kbd_leds = buffer[0];

    if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
    {
      blink_interval_ms = 0;
      board_led_write(true);
    }
    else
    {
      board_led_write(false);
      blink_interval_ms = BLINK_MOUNTED;
    }
  }
}

// BLINKING TASK
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  if (!blink_interval_ms) return;

  if ( board_millis() - start_ms < blink_interval_ms) return;
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;
}