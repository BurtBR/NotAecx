#ifndef WORKERIMPORTXML_H
#define WORKERIMPORTXML_H

#include <QObject>
#include <QDomDocument>
#include <QAbstractItemModel>
#include <QFile>

//TEMPORARIO
#include <QThread>
#include <QDebug>
#include <QTableWidget>
//TEMPORARIO

class WorkerImportXML : public QObject{
    Q_OBJECT

private:

    //Private Methods
    QString FindAttribute(QDomDocument &xmlDoc, QString tokenname, QString attribute);
    QString FindNextValue(QString tokenname, QDomElement node);
    QDomElement FindToken(QString tokenname, QDomElement node);

public:
    WorkerImportXML(QObject *parent = nullptr);

public slots:
    void ImportXMLs(QStringList filelist, int rowcount);

signals:
    void UpdateProgressBar(uint8_t value);
    void DisplayInfo(QString text);
    void WorkerFinished(uint8_t id);
    void InsertData(int row, int column, QString data);

};

#endif // WORKERIMPORTXML_H
