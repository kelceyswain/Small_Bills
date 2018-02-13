#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString bundleref;
    QString username;
    int repository;
    int pencetally;
    ~MainWindow();

private slots:
    void clearEntries();
    void toggleVisible(bool toggle);
    void on_newDatabaseButton_clicked();
    void on_newEntryButton_clicked();
    void on_entriesTable_clicked(const QModelIndex &index);
    void on_openDatabaseButton_clicked();
    void addPence();

    void on_saveEntryButton_clicked();

    void on_deleteEntryButton_clicked();

    void on_poundsEdit_editingFinished();

    void on_shillingsEdit_editingFinished();

    void on_penceEdit_editingFinished();

private:
    void setupModel();
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *model;
    QItemSelectionModel *selectionModel;
    QDataWidgetMapper *mapper;
    int typeIndexRepo;
    int typeIndexDocType;
    int typeIndexDocDesc;
    int typeIndexMonth;
};

#endif // MAINWINDOW_H
