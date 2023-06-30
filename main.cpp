
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[]){
    QApplication application(argc, argv);

    MainWindow *window = nullptr;
    try{
        window = new MainWindow();
    }catch(...){
        QMessageBox::critical(nullptr,"Error","Fail to open program");
        return 0;
    }
    window->setWindowTitle("NotAecx v1.6.2");
    window->showMaximized();
    application.exec();

    delete window;
    return 0;
}
