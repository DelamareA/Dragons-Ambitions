#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

void Fenetre::sauvegarde(QString s)
{
    QString chemin = "data/mission/test1.damission";
    QString tampon;

    ofstream flux(chemin.toStdString().c_str());

    if (flux)
    {
        flux << liste_unite.size() << " ";

        for (unsigned int i=0; i<liste_unite.size(); i++)
        {
            flux << liste_unite[i].type << " ";
            flux << liste_unite[i].angle << " ";
            flux << liste_unite[i].position.x << " ";
            flux << liste_unite[i].position.y << " ";
            flux << liste_unite[i].position.z << " ";

            tampon=liste_unite[i].nom;

            tampon.replace(' ', '*');

            flux << tampon.toStdString() << " ";

            int nb=0;
            for (unsigned int j=0; j<NOMBRE_OBJET; j++)
            {
                if (liste_unite[i].nombre_objet[j]>0)
                {
                    nb++;
                }
            }
            flux << nb << " ";

            for (unsigned int j=0; j<NOMBRE_OBJET; j++)
            {
                if (liste_unite[i].nombre_objet[j]>0)
                {
                    flux << j << " " << liste_unite[i].nombre_objet[j] << " ";
                }
            }

        }

        int nb=0;
        for (unsigned int i=0; i<NOMBRE_PAYS; i++)
        {
            for (unsigned int j=0; j<i; j++)
            {
                if (liste_pays[i].relation[j]!=0)
                {
                    nb++;
                }
            }
        }
        flux << nb << " ";
        for (unsigned int i=0; i<NOMBRE_PAYS; i++)
        {
            for (unsigned int j=0; j<i; j++)
            {
                if (liste_pays[i].relation[j]!=0)
                {
                    flux << i << " " << j << " " << liste_pays[i].relation[j] << " ";
                }
            }
        }

        for (unsigned int i=0; i<NOMBRE_PAYS; i++){
            flux << liste_pays[i].data_ia.nombre_missile << " " << liste_pays[i].data_ia.nombre_missile_variable << " " << liste_pays[i].data_ia.distance_missile << " " << liste_pays[i].data_ia.pourcentage_carbu << " ";
        }

        flux << joueur << " ";

        flux << liste_script.size() << " ";

        for (unsigned int i=0; i<liste_script.size(); i++)
        {
            tampon=liste_script[i].nom;
            tampon.replace(' ', '*');
            flux << tampon.toStdString() << " ";

            flux << liste_script[i].liste_cause.size() << " ";

            for (unsigned int j=0; j<liste_script[i].liste_cause.size(); j++)
            {
                flux << liste_script[i].liste_cause[j].type << " ";
                flux << liste_script[i].liste_cause[j].temps << " ";
            }

            flux << liste_script[i].liste_consequence.size() << " ";

            for (unsigned int j=0; j<liste_script[i].liste_consequence.size(); j++)
            {
                flux << liste_script[i].liste_consequence[j].type << " ";
            }
        }

        flux << liste_sat.size() << " ";

        for (unsigned int i=0; i<liste_sat.size(); i++)
        {
            tampon=liste_sat[i].nom;
            tampon.replace(' ', '*');
            flux << tampon.toStdString() << " ";

            if (liste_sat[i].description.size() == 0){
               flux << "_ ";
            }
            else {
                tampon=liste_sat[i].description;
                tampon.replace(' ', '*');
                flux << tampon.toStdString() << " ";
            }

            flux << liste_sat[i].type << " ";
            flux << liste_sat[i].pays << " ";
            flux << liste_sat[i].precision << " ";
            flux << liste_sat[i].temps_dessus << " ";
            flux << liste_sat[i].temps_ailleurs << " ";
        }
    }
}

void Fenetre::chargement(QString s)
{
    QString chemin = "data/mission/test1.damission";
    string tampon;
    QString tampon2;

    ifstream flux(chemin.toStdString().c_str());

    for (unsigned int i = 0; i < NOMBRE_PAYS; i++){
        liste_pays[i].data_ia.nombre_missile = 3;
        liste_pays[i].data_ia.distance_missile = 20000;
        liste_pays[i].data_ia.nombre_missile_variable = false;

        for (unsigned int j = 0; j < NOMBRE_PAYS; j++){
            if (i!=j){
                liste_pays[i].relation[j] = 0;
                liste_pays[j].relation[i] = 0;
            }
        }
    }

    if (flux)
    {
        liste_unite.clear();
        int taille_liste_unite, type;

        flux >> taille_liste_unite;

        unite copie_unite;
        for (int i=0; i<taille_liste_unite; i++)
        {
            flux >> type;

            copie_unite=preparation_unite(type);

            flux >> copie_unite.angle;
            flux >> copie_unite.position.x;
            flux >> copie_unite.position.y;
            flux >> copie_unite.position.z;
            flux >> tampon;

            copie_unite.altitude_voulue = copie_unite.position.z;

            copie_unite.nom=QString::fromStdString(tampon);
            copie_unite.nom.replace('*', ' ');

            int nb=0;
            flux >> nb;

            int index, valeur;

            for (int j=0; j<nb; j++)
            {
                flux >> index;
                flux >> valeur;

                copie_unite.nombre_objet[index]=valeur;
            }

            liste_unite.push_back(copie_unite);

        }

        int nb=0;
        flux >> nb;

        int index1, index2, valeur;

        for (int i=0; i<nb; i++)
        {
            flux >> index1;
            flux >> index2;
            flux >> valeur;

            liste_pays[index1].relation[index2]=valeur;
            liste_pays[index2].relation[index1]=valeur;
        }

        for (unsigned int i = 0; i < NOMBRE_PAYS; i++){
            flux >> liste_pays[i].data_ia.nombre_missile;
            flux >> liste_pays[i].data_ia.nombre_missile_variable;
            flux >> liste_pays[i].data_ia.distance_missile;
            flux >> liste_pays[i].data_ia.pourcentage_carbu;
        }

        flux >> joueur;

        script copie_script;
        cause copie_cause;
        consequence copie_consequence;

        int nb2;

        flux >> nb;
        for (int i=0; i<nb; i++)
        {
            copie_script.actif=1;
            copie_script.liste_cause.clear();
            copie_script.liste_consequence.clear();

            flux >> tampon;
            copie_script.nom=QString::fromStdString(tampon);
            copie_script.nom.replace('*', ' ');


            flux >> nb2;

            for (int j=0; j<nb2; j++)
            {
                copie_cause.valide=0;
                flux >> copie_cause.type;
                flux >> copie_cause.temps;

                copie_script.liste_cause.push_back(copie_cause);
            }

            flux >> nb2;

            for (unsigned int j=0; j<nb2; j++)
            {
                flux >> copie_consequence.type;

                copie_script.liste_consequence.push_back(copie_consequence);
            }

            liste_script.push_back(copie_script);

        }

        sat copie_sat;

        flux >> nb;
        for (int i=0; i<nb; i++){

            flux >> tampon;
            copie_sat.nom=QString::fromStdString(tampon);
            copie_sat.nom.replace('*', ' ');

            flux >> tampon;
            copie_sat.description=QString::fromStdString(tampon);
            copie_sat.description.replace('*', ' ');

            if (copie_sat.description == "_"){
                copie_sat.description = "";
            }

            flux >> copie_sat.type;
            flux >> copie_sat.pays;
            flux >> copie_sat.precision;
            flux >> copie_sat.temps_dessus;
            flux >> copie_sat.temps_ailleurs;

            if (copie_sat.type != 0){
                copie_sat.temps_propre = rand() % (copie_sat.temps_dessus + copie_sat.temps_ailleurs);
            }

            copie_sat.id = attribution_id();

            liste_sat.push_back(copie_sat);

        }
    }
}
