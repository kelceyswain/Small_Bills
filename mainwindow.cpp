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

    mapper->toFirst();
    model->select();

}

void MainWindow::on_newDatabaseButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open Database"), QDir::currentPath(), tr("SQLite Database Files (*.db)"));
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
        ui->newEntryButton->setEnabled(true);
        ui->saveEntryButton->setEnabled(true);
        ui->deleteEntryButton->setEnabled(true);

        QSqlQuery query(db);

        /*****************************************
         * table recipts
         * id int primary key
         * repository (subtable)
         * enteredBy varchar(32)
         * entryDate
         * bundleRef varchar(8)
         * itemNum unsigned int
         * entryNum unsigned int
         * docType (subtable)
         * docDesc (subtable)
         * consRequired tinyint(1)
         * consNotes varchar(256)
         * allManuscript tinyint(1)
         * printedHeader tinyint(1)
         * containsImage tinyint(1)
         * year unsigned int
         * month unsigned int
         * day unsigned int
         * pounds unsigned int
         * shillings unsigned int
         * pence unsigned int
         * description varchar(1000)
         * commodityCatA (subtable)
         * commodityCatB (subtable)
         * paidToForename varchar(32)
         * paidToSurname varchar(32)
         * paidToJobA (subtable)
         * paidToJobB (subtable)
         * paidByForename varchar(32)
         * paidBySurname varchar(32)
         * paidByJobA (subtable)
         * paidByJobB (subtable)
         * isBenificiaryPerson tinyint(1)
         * benificiaryForname varchar(32)
         * benicifiarySurname varchar(32)
         * benificaryNotes varchar(256)
         ****************************************/

        query.exec("create table receipts "
                   "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "repository int, "
                   "enteredBy text, "
                   "entryDate int, "
                   "bundleRef text, "
                   "itemNum int, "
                   "entryNum int, "
                   "docType int, "
                   "docDesc int, "
                   "consRequired int, "
                   "consNotes text, "
                   "allManuscript int, "
                   "printedHeader int, "
                   "containsImage int, "
                   "year int, "
                   "month int, "
                   "day int, "
                   "pounds int, "
                   "shillings int, "
                   "pence int, "
                   "description text, "
                   "commodityCatA int, "
                   "commodityCatB int, "
                   "paidToForename text, "
                   "paidToSurname text, "
                   "paidToJobA int, "
                   "paidToJobB int, "
                   "paidByForename text, "
                   "paidBySurname text, "
                   "paidByJobA int, "
                   "paidByJobB int, "
                   "isBenificiaryPerson int, "
                   "benificiaryForename text, "
                   "benicifiarySurname text, "
                   "benificaryNotes text)"
                   );
        // Create repos table
        query.exec("create table reposID (id int, repo text)");
        query.exec("insert into reposID values(101, 'Cumberland')");
        query.exec("insert into reposID values(102, 'East Sussex')");
        query.exec("insert into reposID values(103, 'Staffordshire')");

        // Create docType table
        query.exec("create table docTypeID (id int, doc text)");
        query.exec("insert into docTypeID values(101, 'Receipt')");
        query.exec("insert into docTypeID values(102, 'Booklet')");
        query.exec("insert into docTypeID values(103, 'Other')");

        // Create docDesc table
        query.exec("create table docDescID (id int, desc text)");
        query.exec("insert into docDescID values(101, 'Paper')");
        query.exec("insert into docDescID values(102, 'Card')");
        query.exec("insert into docDescID values(103, 'Whatever')");

        // Create month table
        query.exec("create table monthID (id int, mon text)");
        query.exec("insert into monthID values(100, 'Unknown')");
        query.exec("insert into monthID values(101, 'January')");
        query.exec("insert into monthID values(102, 'February')");
        query.exec("insert into monthID values(103, 'March')");
        query.exec("insert into monthID values(104, 'April')");
        query.exec("insert into monthID values(105, 'May')");
        query.exec("insert into monthID values(106, 'June')");
        query.exec("insert into monthID values(107, 'July')");
        query.exec("insert into monthID values(108, 'August')");
        query.exec("insert into monthID values(109, 'September')");
        query.exec("insert into monthID values(110, 'October')");
        query.exec("insert into monthID values(111, 'November')");
        query.exec("insert into monthID values(112, 'December')");

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
    ui->entryNumEdit->clear();
    ui->fromForenameEdit->clear();
    ui->fromSurnameEdit->clear();
    ui->fromJobCatAEdit->clear();
    ui->fromJobCatBEdit->clear();
    ui->headerBox->setChecked(false);
    ui->imageBox->setChecked(false);
    ui->itemNumEdit->clear();
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
        ui->newEntryButton->setEnabled(true);
        ui->saveEntryButton->setEnabled(true);
        ui->deleteEntryButton->setEnabled(true);
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

