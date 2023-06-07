#include "workerimportxml.h"

WorkerImportXML::WorkerImportXML(QObject *parent) : QObject{parent}{

}

QString WorkerImportXML::FindAttribute(QXmlStreamReader &reader, QString tokenname, int attributeid){

    while(!reader.atEnd()){

        if(reader.readNext() == QXmlStreamReader::TokenType::StartElement){

            if(!QString::compare(reader.name(), tokenname)){

                if(reader.attributes().size()){
                    return reader.attributes().at(attributeid).value().toString();
                }else{
                    return QString();
                }
            }
        }
    }

    return QString();
}

QString WorkerImportXML::FindNextValue(QXmlStreamReader &reader, QString tokenname){

    QString value;

    while(!reader.atEnd()){

        if(reader.readNext() == QXmlStreamReader::TokenType::StartElement){
            if(!QString::compare(reader.name(), tokenname)){
                reader.readNext();
                value = reader.text().toString();
                reader.readNext();
                return value;
            }
        }
    }

    return value;
}

QString WorkerImportXML::NextValue(QXmlStreamReader &reader){

    QString value;

    if(!reader.atEnd()){
        reader.readNext();
        reader.readNext();
        value = reader.text().toString();
        reader.readNext();
    }

    return value;
}

bool WorkerImportXML::FindToken(QXmlStreamReader &reader, QString tokenname){


    while(!reader.atEnd()){

        if(reader.readNext() == QXmlStreamReader::TokenType::StartElement){
            if(!QString::compare(reader.name(), tokenname))
                return true;
        }
    }

    return false;
}

void WorkerImportXML::ImportXMLs(QStringList filelist, QAbstractItemModel *tablemodel){

    int rowcount = tablemodel->rowCount(), columncount=1;
    QString nfeID;

    for(int filecount = 0; filecount < filelist.size() ;filecount++){

        QFile fp(filelist[filecount]);
        QXmlStreamReader reader(&fp);

        if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
            emit DisplayInfo("Failed to open file!!!");
            emit WorkerFinished(0);
            return;
        }

        nfeID = FindAttribute(reader, "infNFe", 0);

        while(FindToken(reader, "prod")){

            tablemodel->insertRow(rowcount++);
            columncount = 0;

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), nfeID);

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "cProd"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "cEAN"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "xProd"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "NCM"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "CFOP"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "uCom"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "qCom"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vUnCom"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vProd"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "cEANTrib"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "uTrib"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "qTrib"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vUnTrib"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "indTot"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vTotTrib"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "orig"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "CSOSN"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vBCSTRet"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "pST"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vICMSSTRet"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "CST"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vBC"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "pPIS"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vPIS"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "CST"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vBC"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "pCOFINS"));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue(reader, "vCOFINS"));
        }
        fp.close();

        emit UpdateProgressBar(((filecount+1)*100.0)/filelist.size());
    }

    emit DisplayInfo("Import Complete!");
    emit WorkerFinished(0);
}
