#include "Viewer.h"
#include <QApplication>
#include <QDesktopWidget>
#include <argstream.h>

int main(int argc, char** argv) {
    std::string filename;
    argstream as(argc, argv);
    as >> values<std::string>(&filename, "nom du fichier .obj a charger", 1)
       >> help();
    as.defaultErrorHandling();

    QApplication application(argc,argv);
    QString fileName(filename.c_str());
    Viewer viewer(fileName);

    // centrage de la fenetre :
    QDesktopWidget *desktop = QApplication::desktop();
    int ws = desktop->width(),
        hs = desktop->height();
    viewer.move((ws - viewer.width())/2, (hs - viewer.height())/2);
    
    viewer.show();
    return application.exec();
    }
