#include "scene.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QGraphicsScene>
#include <math.h>
#include <qDebug.h>


Scene::Scene(int a, int b, int c, int d) : QGraphicsScene(a,b,c,d)
{
    x=0;
    y=0;
    release=0;
    release_droite=0;
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button()==Qt::LeftButton)
    {
        release=1;
    }
    if (event->button()==Qt::RightButton)
    {
        release_droite=1;
    }

    event->accept();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    x=event->scenePos().x();
    y=event->scenePos().y();

    event->accept();
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button()==Qt::LeftButton)
    {
        dbl=1;
    }


    event->accept();
}


