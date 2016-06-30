#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>


void Fenetre::check_autodef()
{
    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (liste_classe[liste_unite[i].type].type==0){ // bato
            for (unsigned int j=0; j<liste_unite.size(); j++){

                int pays1=liste_classe[liste_unite[i].type].pays;
                int pays2=liste_classe[liste_unite[j].type].pays;
                int type=liste_classe[liste_unite[j].type].type;
                /*if (pays1!=pays2 && liste_pays[pays1].relation[pays2]==2 && pays1!=joueur && liste_unite[j].est_detecte[pays1]) {

                    int portee=0;
                    int index=-1;
                    for (unsigned int k=0; k<NOMBRE_OBJET; k++){
                        if (liste_unite[i].nombre_objet[k]>0 && liste_unite[i].peut_lancer[k] && liste_objet[k].type == 0 && liste_objet[k].peut_toucher[type] && liste_classe[liste_objet[k].future_unite].fuel_base>portee)
                        {
                            index=k;
                            portee=liste_classe[liste_objet[k].future_unite].fuel_base;


                        }
                    }

                    if (index!=-1)
                    {

                        int conso=1;
                        double vitesse_max=0;
                        for (unsigned int k=0; k<NOMBRE_MODULE; k++)
                        {
                            if (liste_classe[liste_objet[index].future_unite].module_present[k] && liste_module[k].type==1)
                            {
                                conso=liste_module[k].conso;
                                vitesse_max=liste_module[k].vitesse_max;
                            }
                        }
                        if (distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z) < 0.8*liste_classe[liste_objet[index].future_unite].fuel_base*vitesse_max/conso)
                        {
                            preparation_lancer();
                            //tirer_missile(liste_unite[i].id, index, liste_unite[j].id);

                        }
                    }
                }*/

                if (!liste_unite[i].engaging && pays1!=pays2 && liste_pays[pays1].relation[pays2]==2 && liste_unite[j].est_detecte[pays1] && (type == 1 || type == 2 || type == 4)) { // ciws

                    for (unsigned int k=0; k<NOMBRE_OBJET; k++){
                        if (liste_unite[i].nombre_objet[k]>0 && liste_unite[i].peut_lancer[k] && liste_objet[k].peut_toucher[type] && liste_objet[k].type == 2 && distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z) < liste_objet[k].data.portee){
                            liste_unite[i].cible =  liste_unite[j].id;
                            liste_unite[i].engaging = 1;
                        }
                    }
                }

                if (pays1!=pays2 && liste_pays[pays1].relation[pays2]==2 && liste_unite[j].est_detecte[pays1] && type == 1 && distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z) < liste_pays[pays1].data_ia.distance_missile) { // ciws missile
                    DAVector<coo> liste_possible;
                    int nb_possible = 0;

                    for (unsigned int k=0; k<NOMBRE_OBJET; k++){
                        if (liste_unite[i].nombre_objet[k]>0 && liste_unite[i].peut_lancer[k] && liste_objet[k].peut_toucher[type] && liste_objet[k].type == 0){
                            coo c;
                            c.x = k;
                            c.y = liste_unite[i].nombre_objet[k];
                            liste_possible.push_back(c);
                            nb_possible += liste_unite[i].nombre_objet[k];
                        }
                    }

                    if (liste_possible.size() > 0){
                        int nb_tire = 0;
                        for (int k = 0; k < liste_pays[pays1].data_ia.liste_missile_tire.size(); k++){
                            if (liste_pays[pays1].data_ia.liste_missile_tire[k].x == liste_unite[j].id){
                                nb_tire = liste_pays[pays1].data_ia.liste_missile_tire[k].y;
                            }
                        }

                        int nb_a_tirer = liste_pays[pays1].data_ia.nombre_missile;

                        if (liste_pays[pays1].data_ia.nombre_missile_variable){
                            for (unsigned int k = 0; k < NOMBRE_MODULE; k++){
                                if (liste_unite[j].vie_module[k] > 0 && liste_module[k].type == 0){
                                    nb_a_tirer = (liste_module[k].pv / CONSTANTE_WARHEAD_MISSILE) + 1;
                                }
                            }
                        }

                        if (nb_tire < nb_a_tirer){
                            int nb_fired = 0;
                            for (int k = 0; k < nb_a_tirer - nb_tire && k < nb_possible; k++){
                                bool fired = false;
                                for (int l = 0; l < liste_possible.size() && !fired; l++){
                                    if (liste_possible[l].y > 0){
                                        fired = true;

                                        preparation_lancer(liste_unite[i].id, liste_possible[l].x, liste_unite[j].id, 0, -1);
                                        //tirer_missile(liste_unite[i].id, liste_possible[l].x, liste_unite[j].id);
                                        nb_fired ++;

                                        liste_possible[l].y--;
                                    }
                                }
                            }
                            bool found = false;
                            for (int k = 0; k < liste_pays[pays1].data_ia.liste_missile_tire.size() && !found; k++){
                                if (liste_pays[pays1].data_ia.liste_missile_tire[k].x == liste_unite[j].id){
                                    liste_pays[pays1].data_ia.liste_missile_tire[k].y += nb_fired;
                                    found = true;
                                }
                            }

                            if (!found){
                                coo c;
                                c.x = liste_unite[j].id;
                                c.y = nb_fired;
                                liste_pays[pays1].data_ia.liste_missile_tire.push_back(c);
                            }
                        }
                    }
                }
            }
        }
        else if (liste_classe[liste_unite[i].type].type==2 || liste_classe[liste_unite[i].type].type==4){
            int pays1=liste_classe[liste_unite[i].type].pays;
            if (liste_unite[i].fuel < liste_classe[liste_unite[i].type].fuel_base * liste_pays[pays1].data_ia.pourcentage_carbu / 100 && !liste_unite[i].atterrissage){

                int id_objet=-1;
                for (unsigned int j=0; j<NOMBRE_OBJET; j++){
                    if (liste_objet[j].future_unite==liste_unite[i].type){
                        id_objet=j;
                    }
                }
                double min_dist = std::numeric_limits<int>::max()/2;
                int index_min = -1;
                for (unsigned int j=0; j<liste_unite.size(); j++){
                    int pays2=liste_classe[liste_unite[j].type].pays;


                    if (pays2 == pays1 && id_objet!=-1 && liste_unite[j].peut_lancer[id_objet]){
                        double d = distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z);

                        if (d < min_dist){
                            min_dist = d;
                            index_min = j;
                        }
                    }
                }

                if (index_min != -1){
                    coo pos;
                    pos.x = liste_unite[index_min].position.x-cos(liste_unite[index_min].angle)*10000;
                    pos.y = liste_unite[index_min].position.y-sin(liste_unite[index_min].angle)*10000;

                    if (deplacement_unite(i, pos, false, false)){
                        liste_unite[i].altitude_voulue=100;
                        liste_unite[i].montee=1;
                        liste_unite[i].en_deplacement=1;
                        liste_unite[i].atterrissage=1;
                        liste_unite[i].cible_ami = liste_unite[index_min].id;
                    }
                    else {
                        liste_unite[i].atterrissage=0;
                    }

                }
            }
        }

    }
}

situation Fenetre::current_situation(coo pos, int pays){
    situation si;

    si.ennemi_proche[BATEAU] = 0.01;
    si.ennemi_proche[TERRESTRE] = 0.01;
    si.ennemi_proche[AVION] = 0.01;
    si.ennemi_proche[SUB] = 0.01;
    si.allie_proche = 0.01;
    si.munition_projectile = 0.01;
    si.munition[BATEAU] = 0.01;
    si.munition[TERRESTRE] = 0.01;
    si.munition[AVION] = 0.01;
    si.munition[SUB] = 0.01;
    si.global_situation = 0;
    int total_unit = 0;
    int total_ennemi = 0;
    si.vie_unite_allie = 0.01;
    si.vie_unite_ennemi = 0.01;
    si.niveau_detection = 0;
    int detection[NOMBRE_TYPE];
    for (int i = 0; i < NOMBRE_TYPE; i++){
        detection[i] = 0;
    }
    si.fuel = 0.01;
    si.nombre_objet[BATEAU] = 0.01;
    si.nombre_objet[TERRESTRE] = 0.01;
    si.nombre_objet[AVION] = 0.01;
    si.nombre_objet[SUB] = 0.01;
    si.proche_terre = 0.01;

    for (unsigned int i = 0; i < liste_unite.size(); i++){
        int temp = 1;
        if (liste_classe[liste_unite[i].type].type == 0){
            temp = 3;
        }
        else if (liste_classe[liste_unite[i].type].type == 1){
            temp = 0;
        }
        total_unit += temp;

        if (liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2){
            total_ennemi += temp;
        }

        if ((int)(liste_unite[i].position.x/echelle/TAILLE_CASE_IA) == pos.x && (int)(liste_unite[i].position.y/echelle/TAILLE_CASE_IA) == pos.y){
            if (liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2){
                if (liste_classe[liste_unite[i].type].type == 0){
                    si.ennemi_proche[BATEAU] = sqrt(si.ennemi_proche[BATEAU]);
                }
                else if (liste_classe[liste_unite[i].type].type == 5 || liste_classe[liste_unite[i].type].type == 6 || liste_classe[liste_unite[i].type].type == 7){
                    si.ennemi_proche[TERRESTRE] = sqrt(si.ennemi_proche[TERRESTRE]);
                }
                else if (liste_classe[liste_unite[i].type].type == 2 || liste_classe[liste_unite[i].type].type == 4){
                    si.ennemi_proche[AVION] = sqrt(si.ennemi_proche[AVION]);
                }
                else if (liste_classe[liste_unite[i].type].type == 3){
                    si.ennemi_proche[SUB] = sqrt(si.ennemi_proche[SUB]);
                }
            }
            else if (liste_classe[liste_unite[i].type].pays == pays || liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 1){
                si.allie_proche = sqrt(si.allie_proche);

                if (liste_classe[liste_unite[i].type].pays == pays){

                    for (unsigned int j = 0; j < NOMBRE_OBJET; j++){

                        for (int k = 0; k < liste_unite[i].nombre_objet[j] && liste_unite[i].peut_lancer[j]; k++){
                            if (liste_objet[j].type == 2){
                                si.munition_projectile = pow(si.munition_projectile, 0.875);
                            }
                            else if (liste_objet[j].type == 0){
                                if (liste_objet[j].peut_toucher[0]){
                                    si.munition[BATEAU] = pow(si.munition[BATEAU], 0.875);
                                }
                                else if (liste_objet[j].peut_toucher[5] || liste_objet[j].peut_toucher[6] || liste_objet[j].peut_toucher[7]){
                                    si.munition[TERRESTRE] = pow(si.munition[TERRESTRE], 0.875);
                                }
                                else if (liste_objet[j].peut_toucher[2] || liste_objet[j].peut_toucher[4]){
                                    si.munition[AVION] = pow(si.munition[AVION], 0.875);
                                }
                                else if (liste_objet[j].peut_toucher[3]){
                                    si.munition[SUB] = pow(si.munition[SUB], 0.875);
                                }
                            }
                            else if (liste_objet[j].type == 6){
                                si.nombre_objet[BATEAU] = pow(si.nombre_objet[BATEAU], 0.875);
                            }
                            else if (liste_objet[j].type == 3 || liste_objet[j].type == 4 || liste_objet[j].type == 5){
                                si.nombre_objet[TERRESTRE] = pow(si.nombre_objet[TERRESTRE], 0.875);
                            }
                            else if (liste_objet[j].type == 1 || liste_objet[j].type == 7){
                                si.nombre_objet[AVION] = pow(si.nombre_objet[AVION], 0.875);
                            }
                            else if (liste_objet[j].type == 8){
                                si.nombre_objet[SUB] = pow(si.nombre_objet[SUB], 0.875);
                            }
                        }
                    }

                    double ratio = liste_unite[i].vie / liste_unite[i].vie_max;

                    ratio = 1.5 - ratio;

                    si.vie_unite_allie = pow(si.vie_unite_allie, ratio);


                    ratio = liste_unite[i].fuel / liste_classe[liste_unite[i].type].fuel_base;

                    ratio = 1.5 - ratio;

                    si.fuel = pow(si.fuel, ratio);

                    for (int j = 0; j < NOMBRE_TYPE; j++){
                        int val = liste_unite[i].portee_radar[j]/1000000 * liste_unite[i].puissance_radar[j];
                        if (val > detection[j]){
                            detection[j] = val;
                        }

                    }
                }
            }
        }
        else if (distance(liste_unite[i].position.x, liste_unite[i].position.y, (pos.x + 0.5)* TAILLE_CASE_IA * echelle, (pos.y + 0.5)* TAILLE_CASE_IA * echelle)){
            if (liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2){
                if (liste_classe[liste_unite[i].type].type == 0){
                    si.ennemi_proche[BATEAU] = pow(si.ennemi_proche[BATEAU], 0.75);
                }
                else if (liste_classe[liste_unite[i].type].type == 5 || liste_classe[liste_unite[i].type].type == 6 || liste_classe[liste_unite[i].type].type == 7){
                    si.ennemi_proche[TERRESTRE] = pow(si.ennemi_proche[TERRESTRE], 0.75);
                }
                else if (liste_classe[liste_unite[i].type].type == 2 || liste_classe[liste_unite[i].type].type == 4){
                    si.ennemi_proche[AVION] = pow(si.ennemi_proche[AVION], 0.75);
                }
                else if (liste_classe[liste_unite[i].type].type == 3){
                    si.ennemi_proche[SUB] = pow(si.ennemi_proche[SUB], 0.75);
                }

                double ratio = liste_unite[i].vie / liste_unite[i].vie_max;

                ratio = 1.5 - ratio;

                si.vie_unite_ennemi = pow(si.vie_unite_ennemi, ratio);
            }
            else if (liste_classe[liste_unite[i].type].pays == pays || liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 1){
                si.allie_proche = pow(si.allie_proche, 0.75);
            }
        }
    }

    coo pos_map;
    pos_map.x = (pos.x * TAILLE_CASE_IA) / TAILLE_MAP2;
    pos_map.y = (pos.y * TAILLE_CASE_IA) / TAILLE_MAP2;

    for (int a = -2; a <=2; a++){
        for (int b = -2; b <=2; b++){
            if (!(a + pos_map.x < 0 || a + pos_map.x >= largeur_map/TAILLE_MAP2 || b + pos_map.y < 0 || b + pos_map.y >= hauteur_map/TAILLE_MAP2)){
                if (map2[a + pos_map.x][b + pos_map.y] == 1){
                    si.proche_terre = pow(si.proche_terre, 0.40);
                }
            }
        }
    }

    if (total_unit > 0){
        si.global_situation = 1 - total_ennemi / (double)total_unit;
    }

    for (int i = 0; i < NOMBRE_TYPE; i++){
        si.niveau_detection += detection[i]/120000.0;
    }

    if (si.niveau_detection > 1.0){
        si.niveau_detection = 1.0;
    }

    if (si.allie_proche < 0 || si.allie_proche > 1){
        erreur_log("erreur1");
    }
    for (int i = 0; i < 4; i++){
        if (si.ennemi_proche[i] < 0 || si.ennemi_proche[i] > 1){
            erreur_log("erreur2");
        }
        if (si.munition[i] < 0 || si.munition[i] > 1){
            erreur_log("erreur3");
        }
        if (si.nombre_objet[i] < 0 || si.nombre_objet[i] > 1){
            erreur_log("erreur4");
        }
    }
    if (si.fuel < 0 || si.fuel > 1){
        erreur_log("erreur5");
    }
    if (si.proche_terre < 0 || si.proche_terre > 1){
        erreur_log("erreur6");
    }
    if (si.munition_projectile < 0 || si.munition_projectile > 1){
        erreur_log("erreur7");
    }
    if (si.global_situation < 0 || si.global_situation > 1){
        erreur_log("erreur8");
    }
    if (si.vie_unite_allie < 0 || si.vie_unite_allie > 1){
        erreur_log("erreur9");
    }
    if (si.vie_unite_ennemi < 0 || si.vie_unite_ennemi > 1){
        erreur_log("erreur10");
    }
    if (si.niveau_detection < 0 || si.niveau_detection > 1){
        erreur_log("erreur11");
    }


    return si;
}

double Fenetre::compare_situation(situation s1, situation s2){


    double val = 0;
    val += 80 * abs(s1.ennemi_proche[BATEAU] - s2.ennemi_proche[BATEAU]);
    val += 20 * abs(s1.ennemi_proche[TERRESTRE] - s2.ennemi_proche[TERRESTRE]);
    val += 60 * abs(s1.ennemi_proche[AVION] - s2.ennemi_proche[AVION]);
    val += 40 * abs(s1.ennemi_proche[SUB] - s2.ennemi_proche[SUB]);
    val += 70 * abs(s1.allie_proche - s2.allie_proche);
    val += 20 * abs(s1.munition_projectile - s2.munition_projectile);
    val += 90 * abs(s1.munition[BATEAU] - s2.munition[BATEAU]);
    val += 20 * abs(s1.munition[TERRESTRE] - s2.munition[TERRESTRE]);
    val += 70 * abs(s1.munition[AVION] - s2.munition[AVION]);
    val += 50 * abs(s1.munition[SUB] - s2.munition[SUB]);
    val += 40 * abs(s1.global_situation - s2.global_situation);
    val += 30 * abs(s1.vie_unite_allie - s2.vie_unite_allie);
    val += 60 * abs(s1.vie_unite_ennemi - s2.vie_unite_ennemi);
    val += 80 * abs(s1.niveau_detection - s2.niveau_detection);
    val += 50 * abs(s1.fuel - s2.fuel);
    val += 15 * abs(s1.nombre_objet[BATEAU] - s2.nombre_objet[BATEAU]);
    val += 30 * abs(s1.nombre_objet[TERRESTRE] - s2.nombre_objet[TERRESTRE]);
    val += 80 * abs(s1.nombre_objet[AVION] - s2.nombre_objet[AVION]);
    val += 10 * abs(s1.nombre_objet[SUB] - s2.nombre_objet[SUB]);
    val += 80 * abs(s1.proche_terre - s2.proche_terre);

    val /= (80 + 20 + 60 + 40 + 70 + 20 + 90 + 20 + 70 + 50 + 40 + 30 + 60 + 80 + 50 + 15 + 30 + 80 + 10 + 80);

    if (val < 0 || val > 1){
        erreur_log("val ai out of range : " + QString::number(val));
    }

    return 1.0 - val*val;
}

action Fenetre::action_selon_situation(situation si, QVector<element_ia> liste_elem){
    action a;
    double tem[NOMBRE_ACTION];
    double max = - std::numeric_limits<double>::max();
    double min = std::numeric_limits<double>::max();
    //qDebug() << "--------------------------------------------";

    for (int i = 0; i < NOMBRE_ACTION; i++){
        tem[i] = 0;
    }

    for (int i = 0; i < liste_elem.size(); i++){
        double val = compare_situation(si, liste_elem[i].situ);
        double res = liste_elem[i].res.result;

        for (int j = 0; j < NOMBRE_ACTION; j++){
            if (liste_elem[i].act.action_effectue[j]){
                tem[j] += val * (res + 0.5) * liste_elem[i].act.probo[j];
            }
        }
    }

    for (int j = 0; j < NOMBRE_ACTION; j++){
        if (tem[j] > max){
            max = tem[j];
        }
        if (tem[j] < min){
            min = tem[j];
        }
    }

    for (int i = 0; i < NOMBRE_ACTION; i++){
        a.action_effectue[i] = false;
        if (max == min){
            a.probo[i] = 0.5;
            //qDebug() << "0.5";
        }
        else {
            a.probo[i] = (tem[i] - min) / (max-min);
            //qDebug() << a.probo[i] << " , i: " << i << " , max : " << max << " , min : " << min;
        }

        if (a.probo[i] < 0 || a.probo[i] > 1){
            erreur_log("proba < 0 or > 1");
        }
    }

    return a;
}

void Fenetre::gestion_situation(coo pos, int pays){
    element_ia_current elem;
    elem.id = attribution_id(); // peut etre changer
    elem.pos = pos;
    elem.temps = temps;

    situation si = current_situation(pos, pays);

    action a = action_selon_situation(si, liste_element_ia);

    resultat res;
    res.result = 0.0;

    elem.elem.act = a;
    elem.elem.situ = si;
    elem.elem.res = res;

    for (int i = 0; i < NOMBRE_ACTION; i++){
        //qDebug() << "Beg : " << i;
        if (rand()%10000 <= elem.elem.act.probo[i] * 10000){
            //qDebug() << "Doing : " << i;
            bool ok;
            switch(i){
                case RIEN_FAIRE:
                    ok = fonction_rien_faire(pos,pays,elem.id);
                break;
                case DEPLACEMENT_VERS_ENNEMI_PROCHE:
                    ok = fonction_deplacement_vers_ennemi_proche(pos,pays,elem.id);
                break;
                case DEPLACEMENT_VERS_ENNEMI_ELOIGNE:
                    ok = fonction_deplacement_vers_ennemi_eloigne(pos,pays,elem.id);
                break;
                case DEPLACEMENT_VERS_ALLIE_PROCHE:
                    ok = fonction_deplacement_vers_allie_proche(pos,pays,elem.id);
                break;
                case DEPLACEMENT_VERS_ALLIE_ELOIGNE:
                    ok = fonction_deplacement_vers_allie_eloigne(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_BATEAU_PROCHE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_BATEAU_PROCHE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_BATEAU_PROCHE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_BATEAU_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_BATEAU_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_BATEAU_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_bateau_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_AVION_PROCHE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_AVION_PROCHE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_AVION_PROCHE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_AVION_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_AVION_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_AVION_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_avion_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_TERRESTRE_PROCHE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_TERRESTRE_PROCHE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_TERRESTRE_PROCHE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_TERRESTRE_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_TERRESTRE_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_TERRESTRE_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_terrestre_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_SUB_PROCHE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_SUB_PROCHE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_SUB_PROCHE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case TIR_PEU_MISSILE_SUB_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case TIR_MOYEN_MISSILE_SUB_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case TIR_BCP_MISSILE_SUB_ELOIGNE:
                    ok = fonction_tirer_bcp_missile_sub_proche(pos,pays,elem.id);
                break;
                case ENGAGEMENT:
                    ok = fonction_engagement(pos,pays,elem.id);
                break;
                case LANCEMENT_AVION_RADAR:
                    ok = fonction_lancer_avion_radar(pos,pays,elem.id);
                break;
                case LANCEMENT_AVION_ANTI_AIR:
                    ok = fonction_lancer_avion_anti_air(pos,pays,elem.id);
                break;
                case LANCEMENT_AVION_ATTAQUE:
                    ok = fonction_lancer_avion_attaque(pos,pays,elem.id);
                break;
                default :
                    erreur_log("No found action");
                    ok = fonction_rien_faire(pos,pays,elem.id);
                break;
            }

            if (!ok){ // si action ne peut pas etre effectuee
                //elem.elem.res.result -= 0.5;

                elem.elem.act.probo[i] /= 2.0;
            }
            elem.elem.act.action_effectue[i] = true;

        }

        //qDebug() << "End : " << i;
    }

    liste_pays[pays].liste_element.push_back(elem);
}

void Fenetre::check_ia(){
    for (int i = 0; i < NOMBRE_PAYS; i++){
        if (liste_pays[i].present){
            QSet<long> set_unite;

            for (unsigned int j = 0; j < liste_unite.size(); j++){
                if (liste_classe[liste_unite[j].type].pays == i){
                    coo co;
                    co.x = liste_unite[j].position.x/echelle/TAILLE_CASE_IA;
                    co.y = liste_unite[j].position.y/echelle/TAILLE_CASE_IA;

                    long lo = co.x + CONSTANTE_LONG * co.y;

                    set_unite.insert(lo);
                }
            }
            QList<long> liste = set_unite.toList();
            for (int j = 0; j < liste.size(); j++){
                coo pos;
                pos.x = liste[j] % CONSTANTE_LONG;
                pos.y = liste[j] / CONSTANTE_LONG;

                gestion_situation(pos, i);

            }
        }
    }
}

int Fenetre::index_element_ia(int id, int pays){
    if (liste_pays[pays].present){
        for (int j = 0; j < liste_pays[pays].liste_element.size(); j++){
            if (id == liste_pays[pays].liste_element[j].id){
                return j;
            }

        }
    }

    return -1;
}

void Fenetre::chargement_ia(){
    QFile file("data/ai/data.daai");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open data.daai ! exiting");
        exit(10101);
    }

    QTextStream stream(&file);

    QString line;
    QStringList liste;


    do {
        line = stream.readLine();

        if (!line.isNull()){
            line.simplified();

            liste = line.split(' ');

            if (liste.size() > 0){
                liste[0].toLower();



                if (liste[0] == "a"){

                    element_ia ia;

                    ia.situ.ennemi_proche[BATEAU] = liste[1].toDouble();
                    ia.situ.ennemi_proche[TERRESTRE] = liste[2].toDouble();
                    ia.situ.ennemi_proche[AVION] = liste[3].toDouble();
                    ia.situ.ennemi_proche[SUB] = liste[4].toDouble();
                    ia.situ.allie_proche = liste[5].toDouble();
                    ia.situ.munition_projectile = liste[6].toDouble();
                    ia.situ.munition[BATEAU] = liste[7].toDouble();
                    ia.situ.munition[TERRESTRE] = liste[8].toDouble();
                    ia.situ.munition[AVION] = liste[9].toDouble();
                    ia.situ.munition[SUB] = liste[10].toDouble();
                    ia.situ.global_situation = liste[11].toDouble();
                    ia.situ.vie_unite_allie = liste[12].toDouble();
                    ia.situ.vie_unite_ennemi = liste[13].toDouble();
                    ia.situ.niveau_detection = liste[14].toDouble();
                    ia.situ.fuel = liste[15].toDouble();
                    ia.situ.nombre_objet[BATEAU] = liste[16].toDouble();
                    ia.situ.nombre_objet[TERRESTRE] = liste[17].toDouble();
                    ia.situ.nombre_objet[AVION] = liste[18].toDouble();
                    ia.situ.nombre_objet[SUB] = liste[19].toDouble();
                    ia.situ.proche_terre = liste[20].toDouble();

                    ia.res.result = liste[21].toDouble();

                    int nb_action = liste[22].toInt();

                    for (int i = 0; i < NOMBRE_ACTION; i++){
                        ia.act.probo[i] = 0.0;
                        ia.act.action_effectue[i] = false;
                    }

                    for (int i = 0; i < nb_action; i++){
                        ia.act.probo[i] = liste[23 + 2*i].toDouble();
                        ia.act.action_effectue[i] = liste[24 + 2*i].toInt();
                    }

                    liste_element_ia.push_back(ia);
                }


            }
        }
    } while (!line.isNull());
}

void Fenetre::sauvegarde_ia(){
    qDebug() << "ok";
    QFile file("data/ai/data.daai");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open data.daai ! exiting");
        exit(10101);
    }

    QTextStream stream(&file);
    QString line;
    QVector<QString> listeOriginale;

    do {
        line = stream.readLine();

        if (!line.isNull()){
            listeOriginale.push_back(line + '\n');
        }
    } while (!line.isNull());

    file.close();

    QVector<QString> listeNouvelle;

    for (int i = 0; i < NOMBRE_PAYS; i++){
        if (liste_pays[i].present){

            for (int j = 0; j < liste_pays[i].liste_element.size(); j++){
                if (temps - liste_pays[i].liste_element[j].temps > 0){ //  A CHANGER
                    element_ia elem = liste_pays[i].liste_element[j].elem;

                    QString ac;

                    for (int k = 0; k < NOMBRE_ACTION; k++){
                        ac+= QString::number(elem.act.probo[k]) + " " + QString::number(elem.act.action_effectue[k]) + " ";
                    }


                    QString ad("a " + QString::number(elem.situ.ennemi_proche[BATEAU]) + " " + QString::number(elem.situ.ennemi_proche[TERRESTRE]) + " " + QString::number(elem.situ.ennemi_proche[AVION]) + " " + QString::number(elem.situ.ennemi_proche[SUB]) + " " + QString::number(elem.situ.allie_proche) + " " + QString::number(elem.situ.munition_projectile) + " " + QString::number(elem.situ.munition[BATEAU]) + " " + QString::number(elem.situ.munition[TERRESTRE]) + " " + QString::number(elem.situ.munition[AVION]) + " " + QString::number(elem.situ.munition[SUB]) + " " + QString::number(elem.situ.global_situation) + " " + QString::number(elem.situ.vie_unite_allie) + " " + QString::number(elem.situ.vie_unite_ennemi) + " " + QString::number(elem.situ.niveau_detection) + " " + QString::number(elem.situ.fuel) + " " + QString::number(elem.situ.nombre_objet[BATEAU]) + " " + QString::number(elem.situ.nombre_objet[TERRESTRE]) + " " + QString::number(elem.situ.nombre_objet[AVION]) + " " + QString::number(elem.situ.nombre_objet[SUB]) + " " + QString::number(elem.situ.proche_terre) + " " + QString::number(elem.res.result) + " " + QString::number(NOMBRE_ACTION) + " " +  ac + '\n');

                    listeNouvelle.push_back(ad);
                }
            }
        }
    }



    QFile::remove("data/ai/data.daai");

    QFile file2("data/ai/data.daai");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)){
        erreur_log("Could not open data.daai ! exiting");
        exit(10101);
    }

    QTextStream stream2(&file2);


    if (listeOriginale.size() < 500){
        qDebug() << listeOriginale.size();
        for (int i = 0; i < listeOriginale.size(); i++){
            qDebug() << listeOriginale[i];
            stream2 << listeOriginale[i];
        }
        for (int i = 0; i < listeNouvelle.size(); i++){
            stream2 << listeNouvelle[i];
        }
    }
    else {
        QVector<QString> listeFuture;

        int nbO = 0;
        bool takenO[listeOriginale.size()];

        for (int i = 0; i < listeOriginale.size(); i++){
            takenO[i] = false;
        }

        do {
            int in = rand()% listeOriginale.size();

            if (!takenO[in]){
                nbO ++;
                takenO[in] = true;
                listeFuture.push_back(listeOriginale[in]);
            }
        } while(nbO < 400);

        int nbN = 0;
        bool takenN[listeNouvelle.size()];

        for (int i = 0; i < listeNouvelle.size(); i++){
            takenN[i] = false;
        }

        do {
            int in = rand()% listeNouvelle.size();

            if (!takenN[in]){
                nbN ++;
                takenN[in] = true;
                listeFuture.push_back(listeNouvelle[in]);
            }
        } while(nbN < 100);

        for (int i = 0; i < listeFuture.size(); i++){
            stream2 << listeFuture[i];
        }
    }

}
