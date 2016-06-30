#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "constante.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

    public:
        Scene(int a, int b, int c, int d);
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

        int x;
        int y;
        bool release;
        bool release_droite;
        bool dbl;

    private:

    public slots:

    private:

};

#endif // SCENE_H
