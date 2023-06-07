
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QFile>

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
    Ui::MainWindow *ui;

    //Private Functions
    void SetupTable();
    void OpenFiles();
    QString HeaderText(int index);

private slots:
    void On_ButtonOpen_Clicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
