#ifndef WORKEREXPORTCSV_H
#define WORKEREXPORTCSV_H

#include <QObject>
#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>

//TEMPORARIO
#include <QThread>
//TEMPORARIO

class WorkerExportCSV : public QObject{
    Q_OBJECT

private:


public:
    WorkerExportCSV(QObject *parent = nullptr);

public slots:
    void ExportCSV(QString filename, QAbstractItemModel *tablemodel);

signals:
    void UpdateProgressBar(uint8_t value);
    void DisplayInfo(QString text);
    void WorkerFinished(uint8_t id);

};

#endif // WORKEREXPORTCSV_H
