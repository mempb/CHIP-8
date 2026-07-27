#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include <QWidget>
#include <QTimer>
#include "chip8.h"

class Chip8Screen : public QWidget {
    Q_OBJECT                        // required macro for any QObject with signals/slots

public:
    Chip8Screen(Chip8 *chip, QWidget *parent = nullptr);
    void loadRom(const QString &path);   // reset + load, called by the menu

protected:
    void paintEvent(QPaintEvent *event) override;   // Qt calls this to redraw

private slots:
    void tick();                    // runs one batch of cycles, called by the timer

private:
    Chip8 *c;                       // pointer to the emulator state (owned by main)
    QTimer *timer;                  // drives the emulation loop
};

#endif