#include "mainwindow.h"
#include "newdb.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModel()
{
    mapper = new QDataWidgetMapper(this);
    model = new QSqlRelationalTableModel(this);
    model->setTable("receipts");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    typeIndexRepo = model->fieldIndex("repository");
    typeIndexDocType = model->fieldIndex("docType");
    typeIndexDocDesc = model->fieldIndex("docDesc");
    typeIndexMonth = model->fieldIndex("month");

    model->setRelation(typeIndexRepo, QSqlRelation("reposID", "id", "repo"));
    model->setRelation(typeIndexDocType, QSqlRelation("docTypeID", "id", "doc"));
    model->setRelation(typeIndexDocDesc, QSqlRelation("docDescID", "id", "desc"));
    model->setRelation(typeIndexMonth, QSqlRelation("monthID", "id", "mon"));

    model->select();
    ui->entriesTable->setModel(model);
    ui->entriesTable->show();
    mapper->setModel(model);

    // Set the repoComboBox
    QSqlTableModel *relModelRepo = model->relationModel(typeIndexRepo);
    ui->repoComboBox->setModel(relModelRepo);
    ui->repoComboBox->setModelColumn(relModelRepo->fieldIndex("repo"));

    // Set the docTypeComboBox
    QSqlTableModel *relModelDocType = model->relationModel(typeIndexDocType);
    ui->docTypeComboBox->setModel(relModelDocType);
    ui->docTypeComboBox->setModelColumn(relModelDocType->fieldIndex("doc"));

    // Set the docDescBox
    QSqlTableModel *relModelDocDesc = model->relationModel(typeIndexDocDesc);
    ui->docDescBox->setModel(relModelDocDesc);
    ui->docDescBox->setModelColumn(relModelDocDesc->fieldIndex("desc"));

    // Set the monthBox
    QSqlTableModel *relModelMonth = model->relationModel(typeIndexMonth);
    ui->monthComboBox->setModel(relModelMonth);
    ui->monthComboBox->setModelColumn(relModelMonth->fieldIndex("mon"));

    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->userEntryEdit, model->fieldIndex("enteredBy"));
    mapper->addMapping(ui->bundleRefEntryEdit, model->fieldIndex("bundleRef"));
    mapper->addMapping(ui->dateEntryEdit, model->fieldIndex("entryDate"));
    mapper->addMapping(ui->itemNumEdit, model->fieldIndex("itemNum"));
    mapper->addMapping(ui->entryNumEdit, model->fieldIndex("entryNum"));
    mapper->addMapping(ui->repoComboBox, typeIndexRepo);
    mapper->addMapping(ui->docTypeComboBox, typeIndexDocType);
    mapper->addMapping(ui->docDescBox, typeIndexDocDesc);
    mapper->addMapping(ui->consReqBox, model->fieldIndex("consRequired"));
    mapper->addMapping(ui->consNotesEdit, model->fieldIndex("consNotes"));
    mapper->addMapping(ui->manuBox, model->fieldIndex("allManuscript"));
    mapper->addMapping(ui->headerBox, model->fieldIndex("printedHeader"));
    mapper->addMapping(ui->imageBox, model->fieldIndex("containsImage"));
    mapper->addMapping(ui->yearEdit, model->fieldIndex("year"));
    mapper->addMapping(ui->monthComboBox, typeIndexMonth);
    mapper->addMapping(ui->dayEdit, model->fieldIndex("day"));
    mapper->addMapping(ui->poundsEdit, model->fieldIndex("pounds"));
    mapper->addMapping(ui->shillingsEdit, model->fieldIndex("shillings"));
    mapper->addMapping(ui->penceEdit, model->fieldIndex("pence"));
    mapper->addMapping(ui->descriptionTextEdit, model->fieldIndex("description"));
    mapper->addMapping(ui->commodCatAEdit, model->fieldIndex("commodityCatA"));
    mapper->addMapping(ui->commodCatBEdit, model->fieldIndex("commodityCatB"));
    mapper->addMapping(ui->toForenameEdit, model->fieldIndex("paidToForename"));
    mapper->addMapping(ui->toSurnameEdit, model->fieldIndex("paidToSurname"));
    mapper->addMapping(ui->toJobCatAEdit, model->fieldIndex("paidToJobA"));
    mapper->addMapping(ui->toJobCatBEdit, model->fieldIndex("paidToJobB"));
    mapper->addMapping(ui->fromForenameEdit, model->fieldIndex("paidByForename"));
    mapper->addMapping(ui->fromSurnameEdit, model->fieldIndex("paidBySurname"));
    mapper->addMapping(ui->fromJobCatAEdit, model->fieldIndex("paidByJobA"));
    mapper->addMapping(ui->fromJobCatBEdit, model->fieldIndex("paidByJobB"));
    mapper->addMapping(ui->benifPersonBox, model->fieldIndex("isBenificiaryPerson"));
    mapper->addMapping(ui->benifForenameEdit, model->fieldIndex("benificiaryForename"));
    mapper->addMapping(ui->benifSurnameEdit, model->fieldIndex("benificiarySurname"));
    mapper->addMapping(ui->benifNotesEdit, model->fieldIndex("benificaryNotes"));
    mapper->addMapping(ui->flagAttentionCheck, model->fieldIndex("attentionFlag"));
    mapper->addMapping(ui->attentionNotesEdit, model->fieldIndex("attentionNote"));

    mapper->toFirst();
    model->select();

}

void MainWindow::on_newDatabaseButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("New Database"), QDir::currentPath(), tr("SQLite Database Files (*.db)"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if (fileName != NULL)
    {
        if (!db.open()) {
            QMessageBox::critical(0, tr("Cannot open database"),
                tr("Unable to establish a database connection.\n"
                   "This example needs SQLite support. Please read "
                   "the Qt SQL driver documentation for information how "
                   "to build it."), QMessageBox::Cancel);
            return;
        }

        setWindowTitle("Small Bills and Petty Finance DB Editor: "+fileName);
        toggleVisible(true);

        QSqlQuery query(db);

        query.exec("DROP TABLE IF EXISTS receipts;");
        query.exec("CREATE table receipts "
                   "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "repository INT, "
                   "enteredBy TEXT, "
                   "entryDate INT, "
                   "bundleRef TEXT, "
                   "itemNum INT, "
                   "entryNum INT, "
                   "docType INT, "
                   "docDesc INT, "
                   "consRequired INT, "
                   "consNotes TEXT, "
                   "allManuscript INT, "
                   "prINTedHeader INT, "
                   "containsImage INT, "
                   "year INT, "
                   "month INT, "
                   "day INT, "
                   "pounds INT, "
                   "shillings INT, "
                   "pence INT, "
                   "description TEXT, "
                   "commodityCatA INT, "
                   "commodityCatB INT, "
                   "paidToForename TEXT, "
                   "paidToSurname TEXT, "
                   "paidToJobA INT, "
                   "paidToJobB INT, "
                   "paidByForename TEXT, "
                   "paidBySurname TEXT, "
                   "paidByJobA INT, "
                   "paidByJobB INT, "
                   "isBenificiaryPerson INT, "
                   "benificiaryForename TEXT, "
                   "benicifiarySurname TEXT, "
                   "benificaryNotes TEXT,"
                   "attentionFlag INT,"
                   "attentionNote TEXT);"
                   );
        // Create repos table
        query.exec("DROP TABLE IF EXISTS reposID;");
        query.exec("CREATE TABLE reposID (id INT, repo TEXT);");
        query.exec("INSERT INTO reposID values(101, 'Cumberland');");
        query.exec("INSERT INTO reposID values(102, 'East Sussex');");
        query.exec("INSERT INTO reposID values(103, 'Staffordshire');");

        // Create docType table
        query.exec("DROP TABLE IF EXISTS docTypeID;");
        query.exec("CREATE table docTypeID (id int, doc text);");
        query.exec("INSERT into docTypeID values(100, 'Other');");
        query.exec("INSERT into docTypeID values(101, 'Overseer voucher');");
        query.exec("INSERT into docTypeID values(102, 'Churchwarden voucher');");
        query.exec("INSERT into docTypeID values(103, 'Settlement certificate');");

        // CREATE docDesc table
        query.exec("DROP TABLE IF EXISTS docDescID;");
        query.exec("CREATE table docDescID (id int, desc text);");
        query.exec("INSERT into docDescID values(100, 'Letter');");
        query.exec("INSERT into docDescID values(102, 'Receipt');");
        query.exec("INSERT into docDescID values(103, 'Poster');");
        query.exec("INSERT into docDescID values(104, 'Printed bill);");
        query.exec("INSERT into docDescID values(105, 'Other');");

        // CREATE month table
        query.exec("DROP TABLE IF EXISTS monthID;");
        query.exec("CREATE table monthID (id int, mon text);");
        query.exec("INSERT into monthID values(100, 'Unknown');");
        query.exec("INSERT into monthID values(101, 'January');");
        query.exec("INSERT into monthID values(102, 'February');");
        query.exec("INSERT into monthID values(103, 'March');");
        query.exec("INSERT into monthID values(104, 'April');");
        query.exec("INSERT into monthID values(105, 'May');");
        query.exec("INSERT into monthID values(106, 'June');");
        query.exec("INSERT into monthID values(107, 'July');");
        query.exec("INSERT into monthID values(108, 'August');");
        query.exec("INSERT into monthIDsetupModel values(109, 'September');");
        query.exec("INSERT into monthID values(110, 'October');");
        query.exec("INSERT into monthID values(111, 'November');");
        query.exec("INSERT into monthID values(112, 'December');");

        setupModel();
    }
}

void MainWindow::clearEntries()
{
    pencetally = 0;
    ui->benifForenameEdit->clear();
    ui->benifNotesEdit->clear();
    ui->benifPersonBox->setChecked(false);
    ui->benifSurnameEdit->clear();
    ui->bundleRefEntryEdit->clear();
    ui->commodCatAEdit->clear();
    ui->commodCatBEdit->clear();
    ui->consNotesEdit->clear();
    ui->consReqBox->setChecked(false);
    ui->dayEdit->clear();
    ui->descriptionTextEdit->clear();
    ui->docDescBox->clear();
    ui->docTypeComboBox->clear();

    //Don't remove the entry, but increment
    QString entryText = ui->entryNumEdit->text();
    int entryNum = entryText.toInt();
    entryNum++;
    entryText = QString::number(entryNum);
    ui->entryNumEdit->clear();
    ui->entryNumEdit->setText(entryText);

    ui->fromForenameEdit->clear();
    ui->fromSurnameEdit->clear();
    ui->fromJobCatAEdit->clear();
    ui->fromJobCatBEdit->clear();
    ui->headerBox->setChecked(false);
    ui->imageBox->setChecked(false);

    // Don't remove the item
    QString itemText = ui->itemNumEdit->text();
    ui->itemNumEdit->clear();
    ui->itemNumEdit->setText(itemText);

    ui->manuBox->setChecked(false);
    ui->monthComboBox->clear();
    ui->penceEdit->clear();
    ui->penceTotalEdit->clear();
    ui->poundsEdit->clear();
    ui->repoComboBox->setCurrentIndex(repository);
    ui->shillingsEdit->clear();
    ui->toForenameEdit->clear();
    ui->toJobCatAEdit->clear();
    ui->toJobCatBEdit->clear();
    ui->toSurnameEdit->clear();
    ui->userEntryEdit->clear();
    ui->yearEdit->clear();
    ui->flagAttentionCheck->setChecked(false);
    ui->attentionNotesEdit->clear();
}

void MainWindow::toggleVisible(bool toggle)
{
    ui->newEntryButton->setEnabled(toggle);
    ui->saveEntryButton->setEnabled(toggle);
    ui->deleteEntryButton->setEnabled(toggle);
    ui->scrollArea->setEnabled(toggle);
}

void MainWindow::on_newEntryButton_clicked()
{
    clearEntries();
    QDate date = QDate::currentDate();
    QSqlRecord r = model->record();
    r.setValue("enteredBy", username);
    r.setValue("entryDate", date.toString(Qt::ISODate));
    r.setValue("bundleRef", bundleref);
    //r.setValue("repository", repository);
    addPence();
    model->insertRecord(model->rowCount(), r);
    mapper->toLast();

    mapper->submit();
}

void MainWindow::on_entriesTable_clicked(const QModelIndex &index)
{
    clearEntries();
    mapper->setCurrentModelIndex(index);
}

void MainWindow::on_openDatabaseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), QDir::currentPath(), tr("SQLite Database Files (*.db)"));
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (fileName != NULL)
    {
        db.setDatabaseName(fileName);
        if (!db.open()) {
            QMessageBox::critical(0, tr("Cannot open database"),
                tr("Unable to establish a database connection.\n"
                   "This example needs SQLite support. Please read "
                   "the Qt SQL driver documentation for information how "
                   "to build it."), QMessageBox::Cancel);
            return;
        }

        QSqlQuery query(db);
        setWindowTitle("Small Bills and Petty Finance DB Editor: "+fileName);
        toggleVisible(true);
        setupModel();
    }
    else
    {
        return;
    }
}

void MainWindow::on_saveEntryButton_clicked()
{
    model->submitAll();
    model->select();
}

void MainWindow::on_deleteEntryButton_clicked()
{
    model->removeRow(mapper->currentIndex());
}

void MainWindow::addPence()
{
    pencetally = ((ui->poundsEdit->text()).toInt()*240) + ((ui->shillingsEdit->text()).toInt()*12) + (ui->penceEdit->text()).toInt();
    ui->penceTotalEdit->setText(QString::number(pencetally) + " pence.");
}


void MainWindow::on_poundsEdit_editingFinished()
{
    addPence();
}

void MainWindow::on_shillingsEdit_editingFinished()
{
    addPence();
}

void MainWindow::on_penceEdit_editingFinished()
{
    addPence();
}

