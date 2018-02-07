#ifndef NEWDB_H
#define NEWDB_H

#include "mainwindow.h"
#include <QDialog>

namespace Ui {
class NewDB;
}

class NewDB : public QDialog
{
    Q_OBJECT

public:
    explicit NewDB(QWidget *parent = 0);
    ~NewDB();


private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::NewDB *ui;
    MainWindow w;
};

#endif // NEWDB_H
