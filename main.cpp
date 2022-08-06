#include <QApplication>

#include "glwidget.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // GLWidget* window = new GLWidget();
    MainWindow* main_window = new MainWindow();
    main_window->show();

    return a.exec();
}
