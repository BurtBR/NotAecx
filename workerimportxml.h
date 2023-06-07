#ifndef WORKERIMPORTXML_H
#define WORKERIMPORTXML_H

#include <QObject>
#include <QXmlStreamReader>
#include <QAbstractItemModel>
#include <QFile>

//TEMPORARIO
#include <QThread>
//TEMPORARIO

class WorkerImportXML : public QObject{
    Q_OBJECT

private:

    //Private Methods
    QString FindAttribute(QXmlStreamReader &reader, QString tokenname, int attributeid);
    QString FindNextValue(QXmlStreamReader &reader, QString tokenname);
    QString NextValue(QXmlStreamReader &reader);
    bool FindToken(QXmlStreamReader &reader, QString tokenname);

public:
    WorkerImportXML(QObject *parent = nullptr);

public slots:
    void ImportXMLs(QStringList filelist, QAbstractItemModel *tablemodel);

signals:
    void UpdateProgressBar(uint8_t value);
    void DisplayInfo(QString text);
    void WorkerFinished(uint8_t id);

};

#endif // WORKERIMPORTXML_H
