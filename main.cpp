
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[]){
    QApplication application(argc, argv);

    MainWindow *window = nullptr;
    try{
        window = new MainWindow();
    }catch(const char *error){
        //DialogPopup(0, "Erro catastrófico, assim como em Chernobyl! " + QString(error)).exec();
        return 0;
    }catch(...){
        //DialogPopup(0, "Erro catastrófico, assim como em Chernobyl!").exec();
        return 0;
    }
    window->showMaximized();
    application.exec();

    delete window;
    return 0;
}
