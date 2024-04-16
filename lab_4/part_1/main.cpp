#include "lab_4_qt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lab_4_qt w;
    w.show();
    return a.exec();
}
