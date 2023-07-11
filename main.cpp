#include <QtWidgets>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    //GameMap gm;
    //for(int i =0;i<100;i++)gm.generate();
    //return 0;
    QApplication app(argc, argv);
    MainWindow *mw = new MainWindow();
    mw->show();
    return app.exec();
}
