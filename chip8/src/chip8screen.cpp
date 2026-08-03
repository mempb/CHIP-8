#include "chip8screen.h"
#include <QPainter>
#include <QString>

// how big each CHIP-8 pixel is drawn, in real screen pixels
static const int SCALE = 10;

Chip8Screen::Chip8Screen(Chip8 *chip, QWidget *parent) : QWidget(parent), c(chip)
{
    setFixedSize(64 * SCALE, 32 * SCALE);   // 640x320 window
    setWindowTitle("CHIP-8");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Chip8Screen::tick);    
    timer->start(2);                        // fire ~every 2ms → ~500 cycles/sec
}

void Chip8Screen::tick()
{
    chip8_cycle(c);                         // run one instruction

    if (c->draw_flag) {
        c->draw_flag = 0;
        update();                           // ask Qt to repaint (calls paintEvent)
    }
}

void Chip8Screen::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);    // clear background

    painter.setBrush(Qt::white);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (c->display[y * 64 + x]) {
                painter.fillRect(x * SCALE, y * SCALE, SCALE, SCALE, Qt::white);
            }
        }
    }
}

void Chip8Screen::loadRom(const QString &path)
{
    timer->stop();                          // pause while we swap
    chip8_init(c);                          // reset all state
    chip8_load_rom(c, path.toUtf8().constData());  // QString → C string
    update();                               // clear the old screen
    timer->start(2);                        // resume
}