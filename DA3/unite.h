#ifndef UNITE_H
#define UNITE_H

#include <QtGui>
#include <QFrame>
#include <QGraphicsScene>
#include <QTimer>
#include <QComboBox>
#include "constante.h"
#include "scene.h"

class Unite
{
    public:
    Unite();

    protected :
    static int vitesse_classe;
    static double rotation_classe;
    static QPixmap *image_classe;



    int vitesse;
    double angle;
    QGraphicsPixmapItem * image;
    QCheckBox * cadre;
};

#endif // UNITE_H
