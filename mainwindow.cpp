#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    SetupTable();

    //Connect Signals and Slots
    connect(ui->buttonOpen, &QToolButton::clicked, this, On_ButtonOpen_Clicked);

    ui->progressBar->hide();
    ui->lineInfo->hide();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::SetupTable(){

    QAbstractItemModel *modelaux = ui->tableWidget->model();

    ui->tableWidget->setColumnCount(29);
    ui->tableWidget->setRowCount(1);

    for(int i=0; i<ui->tableWidget->columnCount() ;i++){
        modelaux->setData(modelaux->index(0, i), HeaderText(i));
        modelaux->setData(modelaux->index(0, i), Qt::AlignCenter, Qt::TextAlignmentRole);
        modelaux->setData(modelaux->index(0, i), QFont("Source Code Pro", 15, QFont::Bold), Qt::FontRole);
    }

    ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->setColumnWidth(0, 440);
    ui->tableWidget->setColumnWidth(3, 500);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(7, 100);
    ui->tableWidget->setColumnWidth(8, 100);

    //ui->tableWidget->setWordWrap(true);
}

void MainWindow::OpenFiles(){

    QStringList filepath = QFileDialog::getOpenFileNames(this, "Abrir Arquivo", "", "XML Files (*.xml)");
    QAbstractItemModel *modelaux = ui->tableWidget->model();
    int rowcount = ui->tableWidget->rowCount(), columncount=1;
    QString nfeID;

    ui->progressBar->show();
    ui->lineInfo->show();

    for(int filecnt = 0; filecnt<filepath.size() ;filecnt++){

        QFile fp(filepath[filecnt]);

        if(!fp.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QXmlStreamReader reader(&fp);

        nfeID.clear();

        while(!nfeID.size()){
            reader.readNext();
            if(reader.attributes().size()){
                if(!QString::compare(reader.attributes().at(0).name().toString(), "Id"))
                    nfeID = reader.attributes().at(0).value().toString();
            }
        }

        while(!reader.atEnd()){

            reader.readNext();

            if(!QString::compare(reader.name(), "det")){

                ui->tableWidget->insertRow(rowcount++);
                columncount = 1;

                modelaux->setData(modelaux->index(rowcount-1, 0), nfeID);
                reader.readNext();
                for(int i=0; i<14; columncount++, i++){
                    reader.readNext();
                    reader.readNext();
                    modelaux->setData(modelaux->index(rowcount-1, i+1), reader.text().toString());
                    reader.readNext();
                }

                reader.readNext();
                reader.readNext();
                reader.readNext();
                reader.readNext();
                modelaux->setData(modelaux->index(rowcount-1, columncount++), reader.text().toString());
                reader.readNext();

                reader.readNext();
                reader.readNext();

                for(int i=0; i<5 ;columncount++, i++){
                    reader.readNext();
                    reader.readNext();
                    modelaux->setData(modelaux->index(rowcount-1, columncount), reader.text().toString());
                    reader.readNext();
                }

                reader.readNext();
                reader.readNext();
                reader.readNext();
                reader.readNext();

                for(int i=0; i<4 ;columncount++, i++){
                    reader.readNext();
                    reader.readNext();
                    modelaux->setData(modelaux->index(rowcount-1, columncount), reader.text().toString());
                    reader.readNext();
                }

                reader.readNext();
                reader.readNext();
                reader.readNext();
                reader.readNext();

                for(int i=0; i<4 ;columncount++, i++){
                    reader.readNext();
                    reader.readNext();
                    modelaux->setData(modelaux->index(rowcount-1, columncount), reader.text().toString());
                    reader.readNext();
                }

                while(QString::compare(reader.name(), "det"))
                    reader.readNext();
            }
        }

        fp.close();
    }
}

QString MainWindow::HeaderText(int index){

    switch(index){
    case 0:
        return "NFe";
    case 1:
        return "Código do Prod.";
    case 2:
        return "Código EAN";
    case 3:
        return "Descrição";
    case 4:
        return "NCM";
    case 5:
        return "CFOP";
    case 6:
        return "Unidade";
    case 7:
        return "qCom";
    case 8:
        return "unCom";
    case 9:
        return "vProd";
    case 10:
        return "EAN Trib.";
    case 11:
        return "Un. Trib.";
    case 12:
        return "Qtd. Trib.";
    case 13:
        return "Valor Un. do Trib.";
    case 14:
        return "indTot";
    case 15:
        return "Total de Trib.";
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

void MainWindow::On_ButtonOpen_Clicked(){
    OpenFiles();
}
