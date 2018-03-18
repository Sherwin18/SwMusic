#include "dialog.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.resize(900,600);
    w.show();
    return a.exec();
}
