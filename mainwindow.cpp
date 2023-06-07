#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    SetupTable();

    //Connect Signals and Slots
    connect(ui->buttonNew, &QToolButton::clicked, this, On_ButtonNew_Clicked);
    connect(ui->buttonOpen, &QToolButton::clicked, this, On_ButtonOpen_Clicked);

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

    ui->tableWidget->setColumnCount(29);

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
        connect(this, ImportXMLs, worker, &WorkerImportXML::ImportXMLs);
        connect(threadWork, &QThread::finished, worker, &WorkerImportXML::deleteLater);

        worker->moveToThread(threadWork);
        threadWork->start();

        ui->progressBar->setValue(0);
        ui->progressBar->show();
        DisplayInfo("Importing...");

        emit ImportXMLs(filepath, ui->tableWidget->model());
    }
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
        return "ICMSSN500/orig";
    case 17:
        return "ICMSSN500/CSOSN";
    case 18:
        return "ICMSSN500/vBCSTRet";
    case 19:
        return "ICMSSN500/pST";
    case 20:
        return "ICMSSN500/vICMSSTRet";
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
    default:
        return QString();
    }
}

void MainWindow::On_ButtonNew_Clicked(){
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    SetupTable();
}

void MainWindow::On_ButtonOpen_Clicked(){
    OpenFiles();
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
}
