#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <QGraphicsScene>
#include <math.h>
#include <qDebug.h>

unite Fenetre::preparation_unite(int classe)
{
    if (classe < 0 || classe >= NOMBRE_CLASSE){
        erreur_log("Class out of range");
    }

    unite copie_unite;

    copie_unite.peut_lancer = new bool[NOMBRE_OBJET];
    copie_unite.vie_module = new int[NOMBRE_MODULE];
    copie_unite.nombre_objet = new int[NOMBRE_OBJET];

    if (copie_unite.peut_lancer == NULL || copie_unite.vie_module == NULL || copie_unite.nombre_objet == NULL){
        exit(666);
    }

    copie_unite.place=0;
    copie_unite.destination_place=0;
    copie_unite.chg=true;
    copie_unite.chg_pixmap=true;
    copie_unite.chg_module=1;
    copie_unite.chg_objet=0;
    copie_unite.immobilise=0;
    copie_unite.detruite=0;
    copie_unite.en_deplacement=0;
    copie_unite.arret_en_cours=0;
    copie_unite.montee=0;
    copie_unite.patrouille=0;
    copie_unite.id=attribution_id();
    copie_unite.cible=-1;
    copie_unite.cible_ami=-1;
    copie_unite.type=classe;
    copie_unite.altitude_croisiere=0;
    copie_unite.altitude_voulue=0;
    copie_unite.altitude_max_trajet=0;
    copie_unite.repair_object = 0;
    copie_unite.atterrissage=0;
    copie_unite.engaging=0;
    copie_unite.widget_construit = false;
    copie_unite.crazee = false;
    copie_unite.radar_off = false;
    copie_unite.remontee = false;
    copie_unite.vie = 1;
    copie_unite.vie_max = 1;

    for (unsigned int i=0; i<NOMBRE_MODULE; i++)
    {
        if (liste_classe[classe].module_present[i])
        {
            copie_unite.vie_module[i]=liste_module[i].pv;

            if (liste_module[i].type == 1){
                copie_unite.altitude_croisiere = liste_module[i].altitude_croisiere;
                copie_unite.altitude_voulue = liste_module[i].altitude_croisiere;
            }
        }
        else
        {
            copie_unite.vie_module[i]=0;
        }
    }
    for (unsigned int i=0; i<NOMBRE_OBJET; i++)
    {
        copie_unite.nombre_objet[i]=0;
    }
    copie_unite.angle=0;
    copie_unite.angle_voulu=0;
    copie_unite.vitesse=0;
    copie_unite.vitesse_voulue=0;
    copie_unite.vitesse_chute=0;
    copie_unite.fuel=liste_classe[classe].fuel_base;
    copie_unite.position.x=0;
    copie_unite.position.y=0;
    copie_unite.position.z=0;
    copie_unite.destination.x=0;
    copie_unite.destination.y=0;
    copie_unite.destination.z=0;
    copie_unite.nom="";
    copie_unite.pixmap = new QGraphicsPixmapItem();
    copie_unite.pixmap_destination = new QGraphicsPixmapItem();
    copie_unite.widg_selection = new QWidget(onglet_selection);

    return copie_unite;
}
