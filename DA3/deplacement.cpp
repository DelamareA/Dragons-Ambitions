#include "header.h"
#include "constante.h"

#include <limits>

/*function A*(start,goal)
    closedset := the empty set    // The set of nodes already evaluated.
    openset := {start}    // The set of tentative nodes to be evaluated, initially containing the start node
    came_from := the empty map    // The map of navigated nodes.

    g_score[start] := 0    // Cost from start along best known path.
    // Estimated total cost from start to goal through y.
    f_score[start] := g_score[start] + heuristic_cost_estimate(start, goal)

    while openset is not empty
        current := the node in openset having the lowest f_score[] value
        if current = goal
            return reconstruct_path(came_from, goal)

        remove current from openset
        add current to closedset
        for each neighbor in neighbor_nodes(current)
            if neighbor in closedset
                continue
            tentative_g_score := g_score[current] + dist_between(current,neighbor)

            if neighbor not in openset or tentative_g_score < g_score[neighbor]
                came_from[neighbor] := current
                g_score[neighbor] := tentative_g_score
                f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)
                if neighbor not in openset
                    add neighbor to openset

    return failure

function reconstruct_path(came_from, current_node)
    if current_node in came_from
        p := reconstruct_path(came_from, came_from[current_node])
        return (p + current_node)
    else
        return current_node

        */


/*
QVector <coo> mouvement(coo depart, coo arrivee, int ** map2, int largeur_map, int hauteur_map, int type) //  A CHANGER, METTRE OK
{

    QVector <coo4> open_set;
    QVector <coo4> closed_set;

    coo4 copie;
    coo4 copie2;
    copie.p.x=depart.x;
    copie.p.y=depart.y;
    copie.precedent.x=-1;
    copie.precedent.y=-1;
    copie.valeur=0;
    copie.valeur2=distance(depart.x, depart.y, arrivee.x, arrivee.y);

    open_set.push_back(copie);

    bool ok=0;
    int ind;
    int compteur=0;
    double tentative_valeur;

    while (open_set.size()>0 && !ok && compteur<10000)
    {
        ind = meilleur(open_set);
        if (ind ==-1)
        {
            coo copieq;
            copieq.x=666;
            copieq.y=666;

            QVector <coo> final;
            final.push_back(copieq);
            return final;
        }
        copie=open_set[ind];

        if (copie.p.x==arrivee.x && copie.p.y==arrivee.y)
        {
            ok=1;
            closed_set.push_back(copie);
        }
        else
        {
            suppr(ind, open_set);
            closed_set.push_back(copie);

            for (unsigned int i=0; i<8; i++)
            {
                switch(i)
                {
                    default:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y;
                    break;
                    case 1:
                        copie2.p.x=copie.p.x;
                        copie2.p.y=copie.p.y-1;
                    break;
                    case 2:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y;
                    break;
                    case 3:
                        copie2.p.x=copie.p.x;
                        copie2.p.y=copie.p.y+1;
                    break;
                    case 4:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y+1;
                    break;
                    case 5:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y-1;
                    break;
                    case 6:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y+1;
                    break;
                    case 7:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y-1;
                    break;
                }

                if (!(copie2.p.x<0 || copie2.p.y<0 || copie2.p.x>=largeur_map || copie2.p.y>=hauteur_map) && ((map2[copie2.p.x][copie2.p.y]==0 && type != 5) || (map2[copie2.p.x][copie2.p.y]==1 && type == 5)) && !in_closed_set(copie2.p.x, copie2.p.y, closed_set))
                {

                    tentative_valeur=distance(copie2.p.x, copie2.p.y, depart.x, depart.y)+distance(copie2.p.x, copie2.p.y, arrivee.x, arrivee.y);

                    if (!in_open_set(copie2.p.x, copie2.p.y, open_set) || tentative_valeur<valeur_noeud(copie2.p.x, copie2.p.y, open_set))
                    {
                        copie2.precedent.x=copie.p.x;
                        copie2.precedent.y=copie.p.y;
                        copie2.valeur=tentative_valeur;
                        copie2.valeur2=tentative_valeur+distance(copie2.p.x, copie2.p.y, arrivee.x, arrivee.y);
                        if (!in_open_set(copie2.p.x, copie2.p.y, open_set))
                        {
                            open_set.push_back(copie2);
                        }
                    }
                }
            }
        }

        compteur++;
    }

    if (ok)
    {
        QVector <coo> final;

        coo4 copie3;
        copie3=copie;

        bool ok2=0;
        int count=0;

        // final.push_back(arrivee);

        do
        {
            ok2=0;
            final.push_back(copie3.p);
            for (unsigned int i=final.size()-1; i>0; i--)
            {
                final[i]=final[i-1];
            }
            final[0]=copie3.p;

            for (int i=0; i<closed_set.size() && !ok2; i++)
            {
                if (copie3.precedent.x==closed_set[i].p.x && copie3.precedent.y==closed_set[i].p.y)
                {
                    copie3=closed_set[i];
                    ok2=1;
                }
            }

            count++;
        }while(copie3.precedent.x!=-1 && copie3.precedent.y!=-1 && count<10000);


        return final;
    }
    else
    {
        if (open_set.size()==0)
        {
            coo copieq;
            copieq.x=closed_set[33].p.x;
            copieq.y=123;

            QVector <coo> final;
            final.push_back(copieq);
            return final;
        }
        else
        {
            coo copieq;
            copieq.x=1111;
            copieq.y=1111;

            QVector <coo> final;
            final.push_back(copieq);
            return final;
        }
    }
}
*/

long meilleur (QMap<long, coo4> &open_set){
  long index=-1;
  int valeur2=std::numeric_limits<int>::max();
  int valeur2_bis = std::numeric_limits<int>::max();
  QList<coo4> liste = open_set.values();

  for (int i=0; i<liste.size(); i++){
      if (liste[i].valeur2<valeur2){
          valeur2=liste[i].valeur2;
          valeur2_bis = liste[i].valeur2_bis;
          index=liste[i].p.x + liste[i].p.y * CONSTANTE_LONG;
      }
      else if (liste[i].valeur2 == valeur2 && liste[i].valeur2_bis == valeur2_bis){
          valeur2=liste[i].valeur2;
          valeur2_bis = liste[i].valeur2_bis;
          index=liste[i].p.x + liste[i].p.y * CONSTANTE_LONG;
      }
  }

  return index;
}

/*
void suppr (long pos, QMap<long, coo5> &open_set){
    open_set.remove(pos);
}

bool in_closed_set(int x, int y, QSet <coo4> closed_set)
{
    for (int i=0; i<closed_set.size(); i++)
    {
        if (closed_set[i].p.x==x && closed_set[i].p.y==y)
        {
            return true;
        }
    }

    return false;
}

bool in_open_set(int x, int y, DAVector <coo4> open_set)
{
    for (int i=0; i<open_set.size(); i++)
    {
        if (open_set[i].p.x==x && open_set[i].p.y==y)
        {
            return true;
        }
    }

    return false;
}

double valeur_noeud (int x, int y, DAVector <coo4> open_set)
{
    for (int i=0; i<open_set.size(); i++)
    {
        if (open_set[i].p.x==x && open_set[i].p.y==y)
        {
            return open_set[i].valeur;
        }
    }

    return 0;
}
*/

int hauteur_max(coo depart, coo arrivee, int ** map_altitude, int largeur_map, int hauteur_map, int type, int *hau_min){
    double r = 0.0;
    double phi = 0.0;
    bool found = false;

    type++;
    type--;


    for (double phit = 0; phit < PI; phit+=0.01){
        double r1 = depart.x * cos(phit) + depart.y * sin(phit);
        double r2 = arrivee.x * cos(phit) + arrivee.y * sin(phit);

        if (abs(r1 - r2) < 0.5){
            found = true;
            r = r1;
            phi = phit;
        }
    }

    if (!found){
        erreur_log("Coordinates not found : " + QString::number(depart.x * TAILLE_MAP2) + " , " +QString::number(depart.y * TAILLE_MAP2) + " and " + QString::number(arrivee.x * TAILLE_MAP2) + " , " +QString::number(arrivee.y * TAILLE_MAP2));
        return -1;
    }

    double cphi = cos(phi);
    double sphi = sin(phi);
    int hauteur = -10000; // A CHANGER
    *hau_min = 10000;

    for (int i = 0; i < largeur_map; i++){
        for (int j = 0; j < hauteur_map; j++){
            if (abs((i * cphi + j * sphi) - r) < 2 && ((i >= depart.x && i <= arrivee.x && j >= depart.y && j <= arrivee.y) || (i >= arrivee.x && i <= depart.x && j >= arrivee.y && j <= depart.y) || (i >= depart.x && i <= arrivee.x && j >= arrivee.y && j <= depart.y) || (i >= arrivee.x && i <= depart.x && j >= depart.y && j <= arrivee.y))){
                //erreur_log(QString::number(i * TAILLE_MAP2) + " , " +QString::number(j * TAILLE_MAP2));
                coo po;
                po.x = i;
                po.y = j;
                int al = alt(map_altitude, po, type);
                if (al > hauteur){
                    hauteur = al;
                }
                if (al < *hau_min){
                    *hau_min = al;

                }

            }
        }
    }

    return hauteur;
}

QVector<coo> mouvement_alt(coo depart, coo arrivee, int ** map_alt, int largeur_map, int hauteur_map, int alt_min, int alt_max, bool*ok, int*trajet_hauteur_min, int* trajet_hauteur_max, int type) {
    *ok = false;

    if (!(alt(map_alt, arrivee, type) >= alt_min && alt(map_alt, arrivee, type) <= alt_max)){
        QVector <coo> final;
        return final;
    }


    QMap<long, coo4> open_set;
    QMap<long, coo4> closed_set;

    coo4 copie;
    coo4 copie2;
    copie.p.x=depart.x;
    copie.p.y=depart.y;
    copie.precedent.x=-1;
    copie.precedent.y=-1;
    copie.valeur=0;
    copie.valeur2=distance(depart.x, depart.y, arrivee.x, arrivee.y);
    copie.valeur2_bis = distance(depart.x, depart.y, arrivee.x, arrivee.y);
    long key1 = depart.x + depart.y * CONSTANTE_LONG;
    open_set.insert(key1, copie);


    int compteur=0;
    double tentative_valeur;

    *trajet_hauteur_max = -std::numeric_limits<int>::max()/2;
    *trajet_hauteur_min = std::numeric_limits<int>::max()/2;

    while (open_set.size()>0 && !*ok && compteur<5000){

        long ind = meilleur(open_set);

        copie=open_set.value(ind);

        if (copie.p.x==arrivee.x && copie.p.y==arrivee.y){
            *ok=true;
            long key = copie.p.x + CONSTANTE_LONG * copie.p.y;
            closed_set.insert(key, copie);
        }
        else{
            open_set.remove(ind);
            long key = copie.p.x + CONSTANTE_LONG * copie.p.y;
            closed_set.insert(key, copie);

            for (unsigned int i=0; i<8; i++){
                double dist = 1;
                switch(i){
                    default:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y;
                    break;
                    case 1:
                        copie2.p.x=copie.p.x;
                        copie2.p.y=copie.p.y-1;
                    break;
                    case 2:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y;
                    break;
                    case 3:
                        copie2.p.x=copie.p.x;
                        copie2.p.y=copie.p.y+1;
                    break;
                    case 4:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y+1;
                        dist = sqrt(2);
                    break;
                    case 5:
                        copie2.p.x=copie.p.x+1;
                        copie2.p.y=copie.p.y-1;
                        dist = sqrt(2);
                    break;
                    case 6:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y+1;
                        dist = sqrt(2);
                    break;
                    case 7:
                        copie2.p.x=copie.p.x-1;
                        copie2.p.y=copie.p.y-1;
                        dist = sqrt(2);
                    break;
                }
                long key2 = copie2.p.x + CONSTANTE_LONG * copie2.p.y;
                if (!(copie2.p.x<0 || copie2.p.y<0 || copie2.p.x>=largeur_map || copie2.p.y>=hauteur_map) && alt(map_alt, copie2.p, type) >= alt_min && alt(map_alt, copie2.p, type) <= alt_max && !closed_set.contains(key2)){

                    //tentative_valeur=distance(copie2.p.x, copie2.p.y, depart.x, depart.y)+distance(copie2.p.x, copie2.p.y, arrivee.x, arrivee.y);
                    tentative_valeur = copie.valeur + dist;

                    if (!open_set.contains(key2) || tentative_valeur<open_set.value(key2).valeur){
                        copie2.precedent.x=copie.p.x;
                        copie2.precedent.y=copie.p.y;
                        copie2.valeur=tentative_valeur;
                        copie2.valeur2=distance(copie2.p.x, copie2.p.y, arrivee.x, arrivee.y);
                        copie2.valeur2_bis = copie2.valeur + distance(copie2.p.x, copie2.p.y, arrivee.x, arrivee.y);
                        if (!open_set.contains(key2)){
                            open_set.insert(key2, copie2);
                        }
                    }
                }
            }
        }

        compteur++;
    }

    if (*ok){
        QVector <coo> final;

        coo4 copie3=copie;;

        bool ok2=0;
        int count=0;

        // final.push_back(arrivee);

        do {
            ok2 = false;

            if (map_alt[copie3.p.x][copie3.p.y] > *trajet_hauteur_max){
                *trajet_hauteur_max = map_alt[copie3.p.x][copie3.p.y];
            }
            if (map_alt[copie3.p.x][copie3.p.y] < *trajet_hauteur_min){
                *trajet_hauteur_min = map_alt[copie3.p.x][copie3.p.y];
            }

            final.push_back(copie3.p);
            for (unsigned int i=final.size()-1; i>0; i--){
                final[i]=final[i-1];
            }
            final[0]=copie3.p;

            QList<coo4> liste_closed_set = closed_set.values();
            for (int i=0; i<liste_closed_set.size() && !ok2; i++){
                if (copie3.precedent.x==liste_closed_set[i].p.x && copie3.precedent.y==liste_closed_set[i].p.y){
                    copie3=liste_closed_set[i];
                    ok2=true;
                }
            }

            count++;
        }while(copie3.precedent.x!=-1 && copie3.precedent.y!=-1 && count<5000);


        QVector <coo> final2;

        coo current = depart;
        int longest = 0;
        for (int k = 0; k < final.size(); k++){
            int hau_min;
            int hau = hauteur_max(current, final[k], map_alt, largeur_map, hauteur_map, type, &hau_min);
            if (hau_min >= alt_min && hau <= alt_max){
                longest = k;

                if (k == final.size()-1 && k != 0){
                    final2.push_back(final[k]);
                }
            }
            else {
                final2.push_back(final[longest]);
                current = final[longest];
                longest = k;
            }
        }

        return final2;

        //return final;
    }
    else{
        if (open_set.size()==0){

            QVector <coo> final;
            return final;
        }
        else{

            QVector <coo> final;
            return final;
        }
    }
}

int alt(int** map_altitude, coo pos, int type){
    int al = map_altitude[pos.x][pos.y];

    if (type != 3 && al < 0){
        al = 0;
    }
    else if (type == 3 && al < 0){
        al = -1;
    }

    return al;
}

