#include "parserqt.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParserQt w;
    w.show();
    return a.exec();
}
