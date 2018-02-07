#include "mainwindow.h"
#include "newdb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NewDB *newdb = new NewDB;
    newdb->show();

    return a.exec();
}
