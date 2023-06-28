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
    QStringList nfedata;

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

        //Get NFE data
        nfedata.clear();
        nfedata.append(FindAttribute(xmlfile, "infNFe", "Id"));
        element = xmlfile.elementsByTagName("ICMSTot").at(0).toElement();
        nfedata.append(FindNextValue("vBC", element));
        nfedata.append(FindNextValue("vICMS", element));
        nfedata.append(FindNextValue("vICMSDeson", element));
        nfedata.append(FindNextValue("vFCPUFDest", element));
        nfedata.append(FindNextValue("vICMSUFDest", element));
        nfedata.append(FindNextValue("vICMSUFRemet", element));
        nfedata.append(FindNextValue("vFCP", element));
        nfedata.append(FindNextValue("vBCST", element));
        nfedata.append(FindNextValue("vST", element));
        nfedata.append(FindNextValue("vFCPST", element));
        nfedata.append(FindNextValue("vFCPSTRet", element));
        nfedata.append(FindNextValue("vProd", element));
        nfedata.append(FindNextValue("vFrete", element));
        nfedata.append(FindNextValue("vSeg", element));
        nfedata.append(FindNextValue("vDesc", element));
        nfedata.append(FindNextValue("vII", element));
        nfedata.append(FindNextValue("vIPI", element));
        nfedata.append(FindNextValue("vIPIDevol", element));
        nfedata.append(FindNextValue("vPIS", element));
        nfedata.append(FindNextValue("vCOFINS", element));
        nfedata.append(FindNextValue("vOutro", element));
        nfedata.append(FindNextValue("vNF", element));
        nfedata.append(FindNextValue("vTotTrib", element));

        items = xmlfile.elementsByTagName("det");

        for(int i=0; i<items.size() ;i++){

            tablemodel->insertRow(rowcount++);
            columncount = 0;

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), nfedata.at(0));

            element = FindToken("prod", items.at(i).toElement());

            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cProd", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cEAN", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("xProd", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("NCM", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CFOP", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("uCom", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("qCom", element).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vUnCom", element).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vProd", element).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("cEANTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("uTrib", element));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("qTrib", element).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vUnTrib", element).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("indTot", element));

            element = FindToken("imposto", items.at(i).toElement());
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vTotTrib", element).replace(".",","));

            subelement = FindToken("ICMS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("orig", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CSOSN", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBCSTRet", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pST", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vICMSSTRet", subelement).replace(".",","));

            subelement = FindToken("PIS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CST", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBC", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pPIS", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vPIS", subelement).replace(".",","));

            subelement = FindToken("COFINS", element);
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("CST", subelement));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vBC", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("pCOFINS", subelement).replace(".",","));
            tablemodel->setData(tablemodel->index(rowcount-1, columncount++), FindNextValue("vCOFINS", subelement).replace(".",","));

            //NFE Data
            for(int i=1; i<nfedata.size() ;i++)
                tablemodel->setData(tablemodel->index(rowcount-1, columncount++), nfedata[i].replace(".",","));
        }
        emit UpdateProgressBar(((filecount+1)*100.0)/filelist.size());
    }

    emit DisplayInfo("Import Complete! " + QString::number(filecount) + " Files");
    emit WorkerFinished(0);
}
