#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(Chip8 *chip, QWidget *parent)
    : QMainWindow(parent), c(chip)
{
    screen = new Chip8Screen(c, this);
    setCentralWidget(screen);           // emulator fills the window body

    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *openAction = fileMenu->addAction("Open ROM...");
    connect(openAction, &QAction::triggered, this, &MainWindow::openRom);

    setWindowTitle("CHIP-8");
}

void MainWindow::openRom()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open ROM", "../ROMs", "CHIP-8 ROMs (*.ch8);;All Files (*)");

    if (!path.isEmpty()) {
        screen->loadRom(path);          // reset + load the picked ROM
    }
}