#include "workerexportcsv.h"

WorkerExportCSV::WorkerExportCSV(QObject *parent) : QObject{parent}{

}

void WorkerExportCSV::ExportCSV(QString filename, QAbstractItemModel *tablemodel){

    if(!tablemodel){
        emit DisplayInfo("Table pointer null");
        emit WorkerFinished(1);
        return;
    }

    QFile fp(filename);
    QTextStream out(&fp);

    if(!fp.open(QIODevice::WriteOnly | QIODevice::Text)){
        emit DisplayInfo("Failed to open file!!!");
        emit WorkerFinished(1);
        return;
    }

    //Header Data
    out << tablemodel->headerData(0, Qt::Horizontal).toString();
    for(int i=1; i<tablemodel->columnCount() ;i++)
        out << "," << tablemodel->headerData(i, Qt::Horizontal).toString();
    out << "\n";

    for(int i=0; i<tablemodel->rowCount() ;i++){

        out << tablemodel->data(tablemodel->index(i,0)).toString();
        for(int j=1; j<tablemodel->columnCount() ;j++)
            out << "," << tablemodel->data(tablemodel->index(i,j)).toString().remove(',').remove(';');
        out << "\n";
    }

    fp.close();
    emit DisplayInfo("Export Complete!");
    emit WorkerFinished(1);
}
