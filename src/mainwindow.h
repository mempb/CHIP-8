#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chip8screen.h"
#include "chip8.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Chip8 *chip, QWidget *parent = nullptr);

private slots:
    void openRom();                 // File → Open handler

private:
    Chip8Screen *screen;                 // the emulator display (central widget)
    Chip8 *c;
};

#endif // MAINWINDOW_H