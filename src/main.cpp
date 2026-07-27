#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <QApplication>
#include "chip8.h"
#include "mainwindow.h"

#define ROM_DIR "../ROMs"
#define MAX_ROMS 32     // Max roms read, used for array size
#define MAX_PATH 300    // Max rom path, used for array size

int main(int argc, char *argv[])
{
    // Chip-8
    Chip8 c;
    chip8_init(&c);

    // Qt
    QApplication app(argc, argv);
    MainWindow Chip8Screen(&c);       // widget owns the display + cycle timer
    Chip8Screen.show();
    return app.exec();
}