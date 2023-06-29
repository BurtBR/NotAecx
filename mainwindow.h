
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QThread>

#include "workerimportxml.h"
#include "workerexportcsv.h"

//------TEMPORARIO------
#include <QDebug>
//------TEMPORARIO------


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    //Provate Attributes
    bool isBusy = false;
    Ui::MainWindow *ui;
    QThread *threadWork = nullptr;
    QString lastsavedfile;

    //Private Functions
    void KillThreads();
    void SetupTable();
    void OpenFiles();
    void Save();
    void SaveAs();
    bool StartExportWorker();
    QString HeaderText(int index);

private slots:
    void On_ButtonNew_Clicked();
    void On_ButtonOpen_Clicked();
    void On_ButtonSave_Clicked();
    void On_ButtonSaveAs_Clicked();
    void UpdateProgressBar(uint8_t value);
    void DisplayInfo(QString text);
    void WorkerFinished(uint8_t id);
    void InsertData(int row, int column, QString data);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void ImportXMLs(QStringList, int);
    void ExportCSV(QString, QAbstractItemModel*);
};

#endif // MAINWINDOW_H
