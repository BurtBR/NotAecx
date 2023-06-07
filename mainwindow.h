
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QThread>

#include "workerimportxml.h"

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

    //Private Functions
    void KillThreads();
    void SetupTable();
    void OpenFiles();
    QString HeaderText(int index);

private slots:
    void On_ButtonOpen_Clicked();
    void UpdateProgressBar(uint8_t value);
    void DisplayInfo(QString text);
    void WorkerFinished(uint8_t id);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void ImportXMLs(QStringList, QAbstractItemModel*);
};

#endif // MAINWINDOW_H
