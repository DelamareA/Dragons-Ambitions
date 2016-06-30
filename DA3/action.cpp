#include "Fenetre.h"
#include "constante.h"
#include "header.h"

QVector<int> Fenetre::unite_pays_position(coo pos, int pays, QSet<int>* type = 0){
    QVector<int> liste;

    for (unsigned int i = 0; i < liste_unite.size(); i++){
        if ((int)(liste_unite[i].position.x/echelle)/TAILLE_CASE_IA == pos.x && (int)(liste_unite[i].position.y/echelle)/TAILLE_CASE_IA == pos.y && liste_classe[liste_unite[i].type].pays == pays && (type == 0 || type->contains(liste_classe[liste_unite[i].type].type))){
            liste.push_back(i);
        }
    }
    return liste;
}

bool Fenetre::deplacement_unite(int index, coo pos, bool shift = false, bool message = false){
    coo depart;
    coo arrivee;

    depart.x=(liste_unite[index].position.x/echelle)/TAILLE_MAP2;
    depart.y=(liste_unite[index].position.y/echelle)/TAILLE_MAP2;
    arrivee.x=(pos.x/echelle)/TAILLE_MAP2;
    arrivee.y=(pos.y/echelle)/TAILLE_MAP2;

    if (shift){
        if (liste_unite[index].en_deplacement){
            if (liste_unite[index].liste_deplacement.size()>0){
                depart.x=(liste_unite[index].liste_deplacement[liste_unite[index].liste_deplacement.size()-1].x/echelle)/TAILLE_MAP2;
                depart.y=(liste_unite[index].liste_deplacement[liste_unite[index].liste_deplacement.size()-1].y/echelle)/TAILLE_MAP2;
            }
            else{
                depart.x=(liste_unite[index].destination.x/echelle)/TAILLE_MAP2;
                depart.y=(liste_unite[index].destination.y/echelle)/TAILLE_MAP2;
            }
        }
        else{
            depart.x=(liste_unite[index].position.x/echelle)/TAILLE_MAP2;
            depart.y=(liste_unite[index].position.y/echelle)/TAILLE_MAP2;
        }
    }
    else {
        depart.x=(liste_unite[index].position.x/echelle)/TAILLE_MAP2;
        depart.y=(liste_unite[index].position.y/echelle)/TAILLE_MAP2;
    }

    arrivee.x=(pos.x/echelle)/TAILLE_MAP2;
    arrivee.y=(pos.y/echelle)/TAILLE_MAP2;

    if (message && ((map2[arrivee.x][arrivee.y]==1 && (liste_classe[liste_unite[index].type].type == 0 || liste_classe[liste_unite[index].type].type == 3)) || (map2[arrivee.x][arrivee.y]==0 && (liste_classe[liste_unite[index].type].type == 5 || liste_classe[liste_unite[index].type].type == 6)))) // A CHANGER
    {
        int xx=pos.x/echelle;
        int yy=pos.y/echelle;
        if (map[xx][yy].mer==1 && (liste_classe[liste_unite[index].type].type == 0 || liste_classe[liste_unite[index].type].type == 3)){
            ajout_info_texte("There are too many shallows there");
        }
        else if (liste_classe[liste_unite[index].type].type == 5 || liste_classe[liste_unite[index].type].type == 6){
            ajout_info_texte("This unit can't swim too far");
        }
        else{
            ajout_info_texte("This unit can't go there");
        }

        return false;
    }

    bool ok;
    int hauteur_max;
    int hauteur_min;

    QVector<coo> liste = mouvement_alt(depart, arrivee, map_altitude, largeur_map/TAILLE_MAP2, hauteur_map/TAILLE_MAP2, liste_unite[index].altitude_min, liste_unite[index].altitude_max, &ok, &hauteur_min, &hauteur_max, liste_classe[liste_unite[index].type].type);

    if (ok){
        coo3 copie;

        if (liste_unite[index].remontee){
            liste_unite[index].remontee = false;
        }

        if (shift){
            liste_unite[index].altitude_max_trajet = max(liste_unite[index].altitude_max_trajet, hauteur_max + 10);
        }
        else {
            liste_unite[index].altitude_max_trajet = hauteur_max + 10;
        }

        if (hauteur_max + 10 > liste_unite[index].altitude_voulue){
            liste_unite[index].montee = true;
            liste_unite[index].altitude_voulue = hauteur_max + 10;
        }

        if (shift){
            if (liste_unite[index].liste_deplacement.size()>0){
                liste_unite[index].liste_deplacement.pop_back();
            }
            else {
                liste_unite[index].liste_deplacement.clear(); // useless
            }
        }
        else {
            liste_unite[index].liste_deplacement.clear();
        }

        for (int k=0; k<liste.size(); k++){

            copie.x=(liste[k].x*TAILLE_MAP2+TAILLE_MAP2/2)*echelle;
            copie.y=(liste[k].y*TAILLE_MAP2+TAILLE_MAP2/2)*echelle;
            if (k==0 && liste_unite[index].liste_deplacement.size()==0){
                liste_unite[index].destination.x=copie.x;
                liste_unite[index].destination.y=copie.y;
            }
            else{
                liste_unite[index].liste_deplacement.push_back(copie);
            }

        }

        copie.x=pos.x;
        copie.y=pos.y;

        if (liste_unite[index].liste_deplacement.size()==0){
            liste_unite[index].destination.x=copie.x;
            liste_unite[index].destination.y=copie.y;
        }
        else{

            liste_unite[index].liste_deplacement.push_back(copie);
        }
        liste_unite[index].patrouille=0;
        liste_unite[index].en_deplacement=1;
        liste_unite[index].chg=1;
    }
    else if (message){
        ajout_info_texte("This unit can't go there");
    }
    return ok;
}

bool Fenetre::fonction_rien_faire(coo pos, int pays, int id_element_ia){
    pos.x+=0;
    pays+=0;
    id_element_ia+=0;
    return true;
}

bool Fenetre::fonction_deplacement(coo pos, int pays, int id_element_ia, int cible, int dista){
    coo direction;
    int found = 0;

    direction.x = 0;
    direction.y = 0;

    for (unsigned int i = 0; i < liste_unite.size(); i++){
        double dist = distance((pos.x + 0.5)* TAILLE_CASE_IA * echelle, (pos.y + 0.5)* TAILLE_CASE_IA * echelle, liste_unite[i].position.x, liste_unite[i].position.y);

        bool bonne_distance = false;
        bool bonne_distance2 = false;
        bool bonne_cible = false;

        switch(dista){
            default:
            bonne_distance = dist < 100000;
            break;
            case PROCHE:
            bonne_distance = dist < 100000;
            break;
            case ELOIGNE:
            bonne_distance = dist >= 100000;
            break;
        }

        switch(dista){
            default:
            bonne_distance2 = dist < 50000;
            break;
            case PROCHE:
            bonne_distance2 = dist < 50000;
            break;
            case ELOIGNE:
            bonne_distance2 = dist >= 500000;
            break;
        }

        switch(cible){
            default:
            bonne_cible = liste_classe[liste_unite[i].type].pays != pays && liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2 && liste_unite[i].est_detecte[pays] ;
            break;
            case ENNEMI:
            bonne_cible = liste_classe[liste_unite[i].type].pays != pays && liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2 && liste_unite[i].est_detecte[pays] ;
            break;
            case ALLIE:
            bonne_cible = liste_classe[liste_unite[i].type].pays == pays;
            break;
        }

        if (liste_classe[liste_unite[i].type].type != 1 && bonne_cible && bonne_distance){
            found++;

            direction.x += liste_unite[i].position.x;
            direction.y += liste_unite[i].position.y;

            if (bonne_distance2){
                direction.x += liste_unite[i].position.x;
                direction.y += liste_unite[i].position.y;

                found++;
            }

            //qDebug() << liste_classe[liste_unite[i].type].nom << " , cible : " << cible << " , distance : " << dista;


        }
    }

    if (found == 0){
        return false;
    }

    direction.x /= found;
    direction.y /= found;

    QVector<int> liste = unite_pays_position(pos, pays);

    for (int i = 0; i < liste.size(); i++){
        if (!liste_unite[liste[i]].en_deplacement){

            //if (liste_classe[liste_unite[liste[i]].type].type == 0)
                //qDebug() << "Movement ! , cible : " << cible << " , distance : " << dista;

            coo depl;
            depl.x = - liste_unite[liste[i]].position.x + direction.x;
            depl.y = - liste_unite[liste[i]].position.y + direction.y;

            depl.x /= 1.5;
            depl.y /= 1.5;

            depl.x += liste_unite[liste[i]].position.x;
            depl.y += liste_unite[liste[i]].position.y;

            //qDebug() << depl.x << " , " << depl.y;

            deplacement_unite(liste[i], depl);
            liste_unite[liste[i]].liste_id_element_ia.push_back(id_element_ia);
        }
    }

    return true;
}

bool Fenetre::fonction_deplacement_vers_ennemi_proche(coo pos, int pays, int id_element_ia){
    return fonction_deplacement(pos, pays, id_element_ia, ENNEMI, PROCHE);
}

bool Fenetre::fonction_deplacement_vers_ennemi_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_deplacement(pos, pays, id_element_ia, ENNEMI, ELOIGNE);
}

bool Fenetre::fonction_deplacement_vers_allie_proche(coo pos, int pays, int id_element_ia){
    return fonction_deplacement(pos, pays, id_element_ia, ALLIE, PROCHE);
}

bool Fenetre::fonction_deplacement_vers_allie_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_deplacement(pos, pays, id_element_ia, ALLIE, ELOIGNE);
}

bool Fenetre::fonction_tirer_missile(coo pos, int pays, int id_element_ia, int nombre, int cible, int dist){
    int best = -1;
    int best_val = -1;
    for (unsigned int i = 0; i < liste_unite.size(); i++){
        coo posm;
        posm.x = (pos.x + 0.5)* TAILLE_CASE_IA * echelle;
        posm.y = (pos.y + 0.5)* TAILLE_CASE_IA * echelle;
        bool bon_type = false;
        bool bonne_distance = false;

        switch(cible){
            default:
            bon_type = liste_classe[liste_unite[i].type].type == 0;
            break;
            case BATEAU:
            bon_type = liste_classe[liste_unite[i].type].type == 0;
            break;
            case TERRESTRE:
            bon_type = liste_classe[liste_unite[i].type].type == 5 || liste_classe[liste_unite[i].type].type == 6 || liste_classe[liste_unite[i].type].type == 7;
            break;
            case AVION:
            bon_type = liste_classe[liste_unite[i].type].type == 2 || liste_classe[liste_unite[i].type].type == 4;
            break;
            case SUB:
            bon_type = liste_classe[liste_unite[i].type].type == 3;
            break;
        }

        switch(dist){
            default:
            bonne_distance = distance(liste_unite[i].position.x, liste_unite[i].position.y, posm.x, posm.y) < 200000;
            break;
            case PROCHE:
            bonne_distance = distance(liste_unite[i].position.x, liste_unite[i].position.y, posm.x, posm.y) < 200000;
            break;
            case ELOIGNE:
            bonne_distance = distance(liste_unite[i].position.x, liste_unite[i].position.y, posm.x, posm.y) >= 200000;
            break;
        }


        if (bon_type && liste_classe[liste_unite[i].type].pays != pays && liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2 && liste_unite[i].est_detecte[pays] && bonne_distance){
            int val = 0;
            for (int j = 0; j < NOMBRE_TYPE; j++){
                val += liste_unite[i].portee_radar[j]/ 1000000 * liste_unite[i].puissance_radar[j] ;
            }
            for (unsigned int j = 0; j < NOMBRE_OBJET; j++){
                val += liste_unite[i].peut_lancer[j];
            }
            val+= 10 - distance(liste_unite[i].position.x, liste_unite[i].position.y, posm.x, posm.y)/20000;
            if (val > best_val){
                best = i;
                best_val = val;
            }


        }
    }

    if (best == -1){
        return false;
    }

    QVector<int> liste = unite_pays_position(pos, pays);

    int nb_a_tirer = 20 + rand()%20;

    switch(nombre){
        default:
        nb_a_tirer = 20 + rand()%20;
        break;
        case PEU:
        nb_a_tirer = 5 + rand()%5;
        break;
        case MOYEN:
        nb_a_tirer = 10 + rand()%10;
        break;
        case BCP:
        nb_a_tirer = 20 + rand()%20;
        break;
    }

    bool has_left = false;

    for (int i = 0; i < liste_pays[pays].data_ia.liste_missile_tire.size(); i++){
        if (liste_pays[pays].data_ia.liste_missile_tire[i].x == liste_unite[best].id){
            nb_a_tirer -= liste_pays[pays].data_ia.liste_missile_tire[i].y;
        }
    }

    int nb_fired = 0;


    if (nb_a_tirer <= 0)
        return true;

    do {
        has_left = false;
        bool has_fired = false;
        for (int i = 0; i < liste.size() && !has_fired; i++){
            for (unsigned int j = 0; j < NOMBRE_OBJET; j++){
                if (liste_objet[j].type == 1 && liste_unite[liste[i]].nombre_objet[j] > 0 && liste_objet[j].peut_toucher[liste_classe[liste_unite[best].type].type] && liste_unite[liste[i]].peut_lancer[j]){
                    has_fired = true;
                    has_left = true;
                    nb_a_tirer --;
                    nb_fired ++;

                    preparation_lancer(liste_unite[liste[i]].id, j, liste_unite[best].id, 0, id_element_ia);
                }
            }
        }

    } while(nb_a_tirer > 0 && has_left);

    bool found = false;

    for (int i = 0; i < liste_pays[pays].data_ia.liste_missile_tire.size(); i++){
        if (liste_pays[pays].data_ia.liste_missile_tire[i].x == liste_unite[best].id){
            liste_pays[pays].data_ia.liste_missile_tire[i].y += nb_fired;
        }
    }

    if (!found){
        coo co;
        co.x = liste_unite[best].id;
        co.y = nb_fired;

        liste_pays[pays].data_ia.liste_missile_tire.push_back(co);
    }

    return has_left;
}

bool Fenetre::fonction_tirer_bcp_missile_bateau_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, BATEAU, PROCHE);
}

bool Fenetre::fonction_tirer_bcp_missile_bateau_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, BATEAU, ELOIGNE);
}

bool Fenetre::fonction_tirer_bcp_missile_avion_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, AVION, PROCHE);
}

bool Fenetre::fonction_tirer_bcp_missile_avion_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, AVION, ELOIGNE);
}

bool Fenetre::fonction_tirer_bcp_missile_terrestre_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, TERRESTRE, PROCHE);
}

bool Fenetre::fonction_tirer_bcp_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, TERRESTRE, ELOIGNE);
}

bool Fenetre::fonction_tirer_bcp_missile_sub_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, SUB, PROCHE);
}

bool Fenetre::fonction_tirer_bcp_missile_sub_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, BCP, SUB, ELOIGNE);
}

bool Fenetre::fonction_tirer_moyen_missile_bateau_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, BATEAU, PROCHE);
}

bool Fenetre::fonction_tirer_moyen_missile_bateau_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, BATEAU, ELOIGNE);
}

bool Fenetre::fonction_tirer_moyen_missile_avion_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, AVION, PROCHE);
}

bool Fenetre::fonction_tirer_moyen_missile_avion_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, AVION, ELOIGNE);
}

bool Fenetre::fonction_tirer_moyen_missile_terrestre_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, TERRESTRE, PROCHE);
}

bool Fenetre::fonction_tirer_moyen_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, TERRESTRE, ELOIGNE);
}

bool Fenetre::fonction_tirer_moyen_missile_sub_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, SUB, PROCHE);
}

bool Fenetre::fonction_tirer_moyen_missile_sub_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, MOYEN, SUB, ELOIGNE);
}

bool Fenetre::fonction_tirer_peu_missile_bateau_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, BATEAU, PROCHE);
}

bool Fenetre::fonction_tirer_peu_missile_bateau_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, BATEAU, ELOIGNE);
}

bool Fenetre::fonction_tirer_peu_missile_avion_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, AVION, PROCHE);
}

bool Fenetre::fonction_tirer_peu_missile_avion_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, AVION, ELOIGNE);
}

bool Fenetre::fonction_tirer_peu_missile_terrestre_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, TERRESTRE, PROCHE);
}

bool Fenetre::fonction_tirer_peu_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, TERRESTRE, ELOIGNE);
}

bool Fenetre::fonction_tirer_peu_missile_sub_proche(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, SUB, PROCHE);
}

bool Fenetre::fonction_tirer_peu_missile_sub_eloigne(coo pos, int pays, int id_element_ia){
    return fonction_tirer_missile(pos, pays, id_element_ia, PEU, SUB, ELOIGNE);
}

bool Fenetre::fonction_engagement(coo pos, int pays, int id_element_ia){
    double min_dist = 20000;
    int best = -1;
    for (unsigned int i = 0; i < liste_unite.size(); i++){
        double dist = distance((pos.x + 0.5)* TAILLE_CASE_IA * echelle, (pos.y + 0.5)* TAILLE_CASE_IA * echelle, liste_unite[i].position.x, liste_unite[i].position.y);

        if (liste_classe[liste_unite[i].type].pays != pays && liste_pays[liste_classe[liste_unite[i].type].pays].relation[pays] == 2 && liste_unite[i].est_detecte[pays] && dist < 20000){

            if (dist < min_dist){
                min_dist = dist;
                best = i;
            }

        }
    }

    if (best == -1){
        return false;
    }

    QVector<int> liste = unite_pays_position(pos, pays);

    for (int i = 0; i < liste.size(); i++){
        for (unsigned int j = 0; j < NOMBRE_OBJET; j++){
            if (liste_objet[j].type == 2 && liste_objet[j].data.portee >= min_dist && liste_unite[liste[i]].nombre_objet[j] > 0 && liste_objet[j].peut_toucher[liste_classe[liste_unite[best].type].type] && liste_unite[liste[i]].peut_lancer[j]){
                liste_unite[liste[i]].liste_id_element_ia.push_back(id_element_ia);
                liste_unite[liste[i]].cible = liste_unite[best].id;
                liste_unite[liste[i]].engaging = 1;
            }
        }
    }

    return true;
}


bool Fenetre::fonction_lancer_avion(coo pos, int pays, int id_element_ia, int type){

    QSet<int>* set = new QSet<int>();
    set->insert(0);
    set->insert(7);
    QVector<int> liste = unite_pays_position(pos, pays, set);

    int a_lancer = 1 + rand()%3;

    for (int i = 0; i < liste.size(); i++){

        for (unsigned int j = 0; j < NOMBRE_OBJET; j++){
            if ((liste_objet[j].type == 1 || liste_objet[j].type == 7) && liste_unite[liste[i]].nombre_objet[j] > 0 && liste_unite[liste[i]].peut_lancer[j]){
                int val_radar = 0;
                int val_anti_air = 0;
                int val_attaque = 0;

                int bonne_config = -1;

                if (type == RADAR){
                    for (unsigned int k = 0; k < NOMBRE_MODULE; k++){
                        if (liste_classe[liste_objet[j].future_unite].module_present[k] && liste_module[k].type == 4 && (liste_module[k].peut_detecter[0] || liste_module[k].peut_detecter[3])){
                            val_radar ++;
                        }
                    }
                }
                else if (type == ANTI_AIR || type == ATTAQUE){
                    for (int k = 0; k < liste_objet[j].liste_config.size(); k++){
                        for (int l = 0; l < liste_objet[j].liste_config[k].liste.size(); l++){
                            if (type == ANTI_AIR && (liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[1] || liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[2] || liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[4])){
                                val_anti_air++;
                                bonne_config = k;
                            }
                            else if (type == ATTAQUE && (liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[0] || liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[3] || liste_objet[liste_objet[j].liste_config[k].liste[l].x].peut_toucher[7])){
                                val_attaque++;
                                bonne_config = k;
                            }
                        }
                    }
                }

                // si lent, faire une fois au début

                int* nb_objet = new int[NOMBRE_OBJET];
                for (unsigned int k = 0; k < NOMBRE_OBJET; k++){
                    nb_objet[k] = 0;
                }

                if (type == RADAR && val_radar > 0){
                    // no objects
                }
                else if (type == ANTI_AIR && val_anti_air > 0){
                    for (int k = 0; k < liste_objet[j].liste_config[bonne_config].liste.size(); k++){
                        nb_objet[liste_objet[j].liste_config[bonne_config].liste[k].x] += liste_objet[j].liste_config[bonne_config].liste[k].y;
                    }
                }
                else if (type == ATTAQUE && val_attaque > 0){
                    for (int k = 0; k < liste_objet[j].liste_config[bonne_config].liste.size(); k++){
                        nb_objet[liste_objet[j].liste_config[bonne_config].liste[k].x] += liste_objet[j].liste_config[bonne_config].liste[k].y;
                    }
                }

                bool ok = true;

                for (unsigned int k = 0; k < NOMBRE_OBJET; k++){
                    if (liste_unite[liste[i]].nombre_objet[k] < nb_objet[k] * a_lancer){
                        ok = false;
                    }
                }

                for (int k = 0; k < a_lancer && k < liste_unite[liste[i]].nombre_objet[j]; k++){
                    a_lancer --;
                    preparation_lancer(liste_unite[liste[i]].id, j, -1, nb_objet, id_element_ia);
                }
            }
        }
    }

    return true;
}

bool Fenetre::fonction_lancer_avion_radar(coo pos, int pays, int id_element_ia){
    return fonction_lancer_avion(pos, pays, id_element_ia, RADAR);
}

bool Fenetre::fonction_lancer_avion_anti_air(coo pos, int pays, int id_element_ia){
    return fonction_lancer_avion(pos, pays, id_element_ia, ANTI_AIR);
}

bool Fenetre::fonction_lancer_avion_attaque(coo pos, int pays, int id_element_ia){
    return fonction_lancer_avion(pos, pays, id_element_ia, ATTAQUE);
}
