#include "workerimportxml.h"

WorkerImportXML::WorkerImportXML(QObject *parent) : QObject{parent}{

}

QString WorkerImportXML::FindAttribute(QDomDocument &xmlDoc, QString tokenname, QString attribute){
    return xmlDoc.elementsByTagName(tokenname).at(0).attributes().namedItem(attribute).nodeValue();
}

QString WorkerImportXML::FindNextValue(QString tokenname, QDomElement node){

    QDomElement childtoken = FindToken(tokenname, node);

    if(childtoken.hasChildNodes()){
        return childtoken.childNodes().at(0).nodeValue();
    }

    return QString();
}

QDomElement WorkerImportXML::FindToken(QString tokenname, QDomElement node){
    return node.elementsByTagName(tokenname).at(0).toElement();
}

void WorkerImportXML::ImportXMLs(QStringList filelist, QAbstractItemModel *tablemodel){

    int rowcount = tablemodel->rowCount(), columncount=1;
    int filecount = 0;
    QString nfeID;

    for(filecount = 0; filecount < filelist.size() ;filecount++){

        QFile fp(filelist[filecount]);
        QDomDocument xmlfile("XMLfile");
        QDomNodeList items;
        QDomElement element, subelement;

        if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
            emit DisplayInfo("Failed to open file!!!");
            emit WorkerFinished(0);
            return;
        }

        xmlfile.setContent(&fp);
        fp.close();

        nfeID = FindAttribute(xmlfile, "infNFe", "Id");

        items = xmlfile.elementsByTagName("det");

        for(int i=0; i<items.size() ;i++){

            tablemodel->insertRow(rowcount++);
            columncount = 0;

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), nfeID);

            element = FindToken("prod", items.at(i).toElement());

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cProd", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cEAN", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("xProd", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("NCM", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CFOP", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("uCom", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("qCom", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vUnCom", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vProd", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cEANTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("uTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("qTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vUnTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("indTot", element));

            element = FindToken("imposto", items.at(i).toElement());
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vTotTrib", element));

            subelement = FindToken("ICMS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("orig", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CSOSN", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBCSTRet", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pST", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vICMSSTRet", subelement));

            subelement = FindToken("PIS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CST", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBC", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pPIS", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vPIS", subelement));

            subelement = FindToken("COFINS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CST", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBC", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pCOFINS", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vCOFINS", subelement));
        }
        emit UpdateProgressBar(((filecount+1)*100.0)/filelist.size());
    }

    emit DisplayInfo("Import Complete! " + QString::number(filecount) + " Files");
    emit WorkerFinished(0);
}
