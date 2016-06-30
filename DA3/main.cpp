#include <QApplication>
#include "Fenetre.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Fenetre fenetre;
    fenetre.show();

    return app.exec();
}


