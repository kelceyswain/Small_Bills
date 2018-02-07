#include "mainwindow.h"
#include "newdb.h"
#include "ui_newdb.h"

NewDB::NewDB(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDB)
{

    MainWindow w;
    w.username = "";
    w.bundleref = "";
    QStringList repoList;
    repoList << "Cumberland" << "East Sussex" << "Staffordshire";
    ui->setupUi(this);
    ui->repositoryCombo->addItems(repoList);

}

NewDB::~NewDB()
{
    delete ui;
}

void NewDB::on_buttonBox_accepted()
{
    w.bundleref = ui->bundleRefEdit->text();
    w.username = ui->usernameEdit->text();
    w.repository = ui->repositoryCombo->currentIndex();
    w.show();
}

void NewDB::on_buttonBox_rejected()
{
    QApplication::quit();
}
