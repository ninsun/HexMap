#include "HexMap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HexMap w;
    w.show();
    return a.exec();
}
