#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    SetupTable();

    //Connect Signals and Slots
    connect(ui->buttonNew, &QToolButton::clicked, this, On_ButtonNew_Clicked);
    connect(ui->buttonOpen, &QToolButton::clicked, this, On_ButtonOpen_Clicked);
    connect(ui->buttonSave, &QToolButton::clicked, this, On_ButtonSave_Clicked);
    connect(ui->buttonSaveAs, &QToolButton::clicked, this, On_ButtonSaveAs_Clicked);

    ui->progressBar->hide();
    ui->lineInfo->setText("");
}

MainWindow::~MainWindow(){

    KillThreads();
    delete ui;
}

void MainWindow::KillThreads(){
    if(threadWork){
        threadWork->quit();
        while(!threadWork->wait());
        delete threadWork;
        threadWork = nullptr;
    }
}

void MainWindow::SetupTable(){

    QStringList headerlist;

    ui->tableWidget->setColumnCount(52);

    for(int i=0; i<ui->tableWidget->columnCount() ;i++){
        headerlist.append(HeaderText(i));
    }

    ui->tableWidget->setHorizontalHeaderLabels(headerlist);

    ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->setColumnWidth(0, 440);
    ui->tableWidget->setColumnWidth(1, 130);
    ui->tableWidget->setColumnWidth(2, 130);
    ui->tableWidget->setColumnWidth(3, 450);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(7, 100);
    ui->tableWidget->setColumnWidth(8, 100);
}

void MainWindow::OpenFiles(){

    if(isBusy){
        DisplayInfo("Execution in progress! Wait for it to finish...");
        return;
    }

    QStringList filepath = QFileDialog::getOpenFileNames(this, "Open File", "", "XML Files (*.xml)");
    WorkerImportXML *worker;

    if(filepath.size()){

        try{
            worker = new WorkerImportXML;
        }catch(...){
            DisplayInfo("Failed to allocate memory for thread worker");
            return;
        }

        try{
            threadWork = new QThread(this);
        }catch(...){
            DisplayInfo("Failed to allocate memory for thread");
            delete worker;
            worker = nullptr;
            return;
        }

        isBusy = true;
        this->setDisabled(true);

        connect(worker, &WorkerImportXML::WorkerFinished, this, WorkerFinished);
        connect(worker, &WorkerImportXML::UpdateProgressBar, this, UpdateProgressBar);
        connect(worker, &WorkerImportXML::DisplayInfo, this, DisplayInfo);
        connect(worker, &WorkerImportXML::InsertData, this, InsertData);
        connect(this, ImportXMLs, worker, &WorkerImportXML::ImportXMLs);
        connect(threadWork, &QThread::finished, worker, &WorkerImportXML::deleteLater);

        worker->moveToThread(threadWork);
        threadWork->start();

        ui->progressBar->setValue(0);
        ui->progressBar->show();
        DisplayInfo("Importing...");

        ui->tableWidget->setSortingEnabled(false);

        emit ImportXMLs(filepath, ui->tableWidget->rowCount());
    }
}

void MainWindow::Save(){

    if(!lastsavedfile.size())
        return SaveAs();

    if(!StartExportWorker())
        return;

    ui->tableWidget->setSortingEnabled(false);
    emit ExportCSV(lastsavedfile, ui->tableWidget->model());
}

void MainWindow::SaveAs(){

    QString filepath = QFileDialog::getSaveFileName(this, "Save File", "", "CSV Files (*.csv)");

    if(filepath.size()){
        if(!StartExportWorker())
            return;

        ui->tableWidget->setSortingEnabled(false);
        lastsavedfile = filepath;
        emit ExportCSV(filepath, ui->tableWidget->model());
    }
}

bool MainWindow::StartExportWorker(){

    if(isBusy){
        DisplayInfo("Execution in progress! Wait for it to finish...");
        return false;
    }

    WorkerExportCSV *worker;

    try{
        worker = new WorkerExportCSV;
    }catch(...){
        DisplayInfo("Failed to allocate memory for thread worker");
        return false;
    }

    try{
        threadWork = new QThread(this);
    }catch(...){
        DisplayInfo("Failed to allocate memory for thread");
        delete worker;
        worker = nullptr;
        return false;
    }

    isBusy = true;
    this->setDisabled(true);

    connect(worker, &WorkerExportCSV::WorkerFinished, this, WorkerFinished);
    connect(worker, &WorkerExportCSV::UpdateProgressBar, this, UpdateProgressBar);
    connect(worker, &WorkerExportCSV::DisplayInfo, this, DisplayInfo);
    connect(this, ExportCSV, worker, &WorkerExportCSV::ExportCSV);
    connect(threadWork, &QThread::finished, worker, &WorkerExportCSV::deleteLater);

    ui->progressBar->setValue(0);
    ui->progressBar->show();
    DisplayInfo("Exporting...");

    return true;
}

QString MainWindow::HeaderText(int index){

    switch(index){
    case 0:
        return "NFe";
    case 1:
        return "cProd";
    case 2:
        return "cEAN";
    case 3:
        return "xProd";
    case 4:
        return "NCM";
    case 5:
        return "CFOP";
    case 6:
        return "uCom";
    case 7:
        return "qCom";
    case 8:
        return "unCom";
    case 9:
        return "vProd";
    case 10:
        return "cEANTrib";
    case 11:
        return "uTrib";
    case 12:
        return "qTrib";
    case 13:
        return "vUnTrib";
    case 14:
        return "indTot";
    case 15:
        return "vTotTrib";
    case 16:
        return "ICMSSN/orig";
    case 17:
        return "ICMSSN/CSOSN";
    case 18:
        return "ICMSSN/vBCSTRet";
    case 19:
        return "ICMSSN/pST";
    case 20:
        return "ICMSSN/vICMSSTRet";
    case 21:
        return "PISOutr/CST";
    case 22:
        return "PISOutr/vBC";
    case 23:
        return "PISOutr/pPIS";
    case 24:
        return "PISOutr/vPIS";
    case 25:
        return "COFINSOutr/CST";
    case 26:
        return "COFINSOutr/vBC";
    case 27:
        return "COFINSOutr/pCOFINS";
    case 28:
        return "COFINSOutr/vCOFINS";
    case 29:
        return "ICMSTot/vBC";
    case 30:
        return "ICMSTot/vICMS";
    case 31:
        return "ICMSTot/vICMSDeson";
    case 32:
        return "ICMSTot/vFCPUFDest";
    case 33:
        return "ICMSTot/vICMSUFDest";
    case 34:
        return "ICMSTot/vICMSUFRemet";
    case 35:
        return "ICMSTot/vFCP";
    case 36:
        return "ICMSTot/vBCST";
    case 37:
        return "ICMSTot/vST";
    case 38:
        return "ICMSTot/vFCPST";
    case 39:
        return "ICMSTot/vFCPSTRet";
    case 40:
        return "ICMSTot/vProd";
    case 41:
        return "ICMSTot/vFrete";
    case 42:
        return "ICMSTot/vSeg";
    case 43:
        return "ICMSTot/vDesc";
    case 44:
        return "ICMSTot/vII";
    case 45:
        return "ICMSTot/vIPI";
    case 46:
        return "ICMSTot/vIPIDevol";
    case 47:
        return "ICMSTot/vPIS";
    case 48:
        return "ICMSTot/vCOFINS";
    case 49:
        return "ICMSTot/vOutro";
    case 50:
        return "ICMSTot/vNF";
    case 51:
        return "ICMSTot/vTotTrib";
    default:
        return QString();
    }
}

void MainWindow::On_ButtonNew_Clicked(){
    lastsavedfile.clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    SetupTable();
}

void MainWindow::On_ButtonOpen_Clicked(){
    OpenFiles();
}

void MainWindow::On_ButtonSave_Clicked(){
    Save();
}

void MainWindow::On_ButtonSaveAs_Clicked(){
    SaveAs();
}

void MainWindow::UpdateProgressBar(uint8_t value){
    if(value <= 100)
        ui->progressBar->setValue(value);
}

void MainWindow::DisplayInfo(QString text){
    ui->lineInfo->setText(text);
    ui->lineInfo->show();
    return;
}

void MainWindow::WorkerFinished(uint8_t){

    KillThreads();
    isBusy = false;
    this->setEnabled(true);
    ui->progressBar->hide();
    ui->tableWidget->setSortingEnabled(true);
}

void MainWindow::InsertData(int row, int column, QString data){

    if(column >= ui->tableWidget->columnCount())
        return;

    if(row >= ui->tableWidget->rowCount()){
        ui->tableWidget->insertRow(row);
    }

    ui->tableWidget->model()->setData(ui->tableWidget->model()->index(row, column), data);
}
