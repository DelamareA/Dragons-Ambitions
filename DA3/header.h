#include <QVector>
#include "constante.h"

#ifndef HEADER_H
#define HEADER_H


    double distance (double x1, double y1, double x2, double y2);
    double distance (double x1, double y1, double z1, double x2, double y2, double z2);
    double angleFromPosAndDes(double x1, double y1, double x2, double y2);
    int max(int x1, int x2);
    int min(int x1, int x2);
    double degtorad (double valeur);
    double radtodeg (double valeur);

    /*bool in_closed_set(int x, int y, QVector <coo4> closed_set);
    bool in_open_set(int x, int y, QVector <coo4> open_set);
    double valeur_noeud (int x, int y, QVector <coo4> open_set);
    void suppr (int index, QVector <coo4> &open_set);*/
    //int meilleur (QVector <coo4> &a_traiter);
    long meilleur (QMap<long, coo4> &open_set);
    //QVector<coo> mouvement(coo depart, coo arrivee, int ** map2, int largeur_map, int hauteur_map, int type);
    QVector<coo> mouvement_alt(coo depart, coo arrivee, int ** map_alt, int largeur_map, int hauteur_map, int alt_min, int alt_max, bool*ok, int*trajet_hauteur_min, int* trajet_hauteur_max, int type);
    int hauteur_max(coo depart, coo arrivee, int ** map_altitude, int largeur_map, int hauteur_map, int type, int * hau_min);
    int alt(int** map_altitude, coo pos, int type);

#endif
