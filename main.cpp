#include <QCoreApplication>
#include <QApplication>

#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GLWidget* window = new GLWidget();
    window->show();

    return a.exec();
}
