#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <QTextStream>

QString Fenetre::load_file(QString filename){
    QString result = "";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        erreur_log("Could not open file");
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        result += in.readLine();
    }

    file.close();

    return result;
}

void Fenetre::set_numbers(){
    QFile file("data/global_index");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open global_index ! exiting");
        exit(10101);
    }

    QTextStream stream(&file);

    QString line;
    QStringList liste;

    NOMBRE_OBJET = 0;
    NOMBRE_MODULE = 0;
    NOMBRE_CLASSE = 0;


    do {
        line = stream.readLine();

        if (!line.isNull()){
            line.simplified();

            liste = line.split(' ');

            if (liste.size() > 0){
                liste[0].toLower();

                if (liste[0] == "number_object" && liste.size() == 2){
                    int nb = liste[1].toInt();

                    if (nb < 0){
                        erreur_log("Error in global_index : number of objects cant be negative, exiting");
                        exit(10101);
                    }
                    else {
                        NOMBRE_OBJET = nb;
                    }
                }
                else if (liste[0] == "number_module" && liste.size() == 2){
                    int nb = liste[1].toInt();

                    if (nb < 0){
                        erreur_log("Error in global_index : number of modules cant be negative, exiting");
                        exit(10101);
                    }
                    else {
                        NOMBRE_MODULE = nb;
                    }
                }
                else if (liste[0] == "number_module" && liste.size() == 2){
                    int nb = liste[1].toInt();

                    if (nb < 0){
                        erreur_log("Error in global_index : numbre of modules cant be negative, exiting");
                        exit(10101);
                    }
                    else {
                        NOMBRE_MODULE = nb;
                    }
                }
                else if (liste[0] == "number_class" && liste.size() == 2){
                    int nb = liste[1].toInt();

                    if (nb < 0){
                        erreur_log("Error in global_index : number of classes cant be negative, exiting");
                        exit(10101);
                    }
                    else {
                        NOMBRE_CLASSE = nb;
                    }
                }
            }
        }
    } while (!line.isNull());

    if (NOMBRE_CLASSE == 0 || NOMBRE_MODULE == 0 || NOMBRE_OBJET == 0){
        erreur_log("Error in global_index : some numbers are not set, exiting");
        exit(10101);
    }

    liste_classe = new classe[NOMBRE_CLASSE];
    liste_module = new module[NOMBRE_MODULE];
    liste_objet = new objet[NOMBRE_OBJET];
}

void Fenetre::clear_log(){
    QFile::remove("errors.dalog");

    QFile file("errors.dalog");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
}

void erreur_log(QString erreur){
    qDebug() << erreur;

    QFile file("errors.dalog");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << erreur << "\n";

}

void Fenetre::chargement_liste_classe ()
{
    for (unsigned int i=0; i<NOMBRE_CLASSE; i++)
    {
        liste_classe[i].nom="";
        liste_classe[i].pays=0;
        liste_classe[i].fuel_base=10000;
        liste_classe[i].stealth=1;
        liste_classe[i].taille=100;
        liste_classe[i].type=0;
        liste_classe[i].warhead=0;
        liste_classe[i].frottement=10.0;
        liste_classe[i].bruit = 0.3;


        for (unsigned int j=0; j<5; j++)
        {
            liste_classe[i].image[j]= new QPixmap();
        }
        liste_classe[i].module_present = new bool[NOMBRE_MODULE];
        if (liste_classe[i].module_present == NULL){
            exit(666);
        }
        for (unsigned int j=0; j<NOMBRE_MODULE; j++)
        {
            liste_classe[i].module_present[j]= false;
        }
        for (unsigned int j=0; j<4; j++)
        {
            liste_classe[i].hangar[j]= 100;
        }
    }

    QFile file("data/class/index");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open class's index");
        return;
    }
    QTextStream stream(&file);

    QString line;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            chargement_classe(line);
        }
    } while(!line.isNull());

    liste_classe[0].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[0].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[0].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[0].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[0].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[0].nom="Arleigh Burke";
    liste_classe[0].type=0;
    liste_classe[0].pays=USA;
    liste_classe[0].module_present[0]=1;
    liste_classe[0].module_present[1]=1;
    liste_classe[0].module_present[4]=1;
    liste_classe[0].module_present[6]=1;
    liste_classe[0].module_present[8]=1;
    liste_classe[0].module_present[15]=1;


    liste_classe[1].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[1].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[1].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[1].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[1].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[1].nom="Luyang II";
    liste_classe[1].type=0;
    liste_classe[1].pays=CHINA;
    liste_classe[1].module_present[0]=1;
    liste_classe[1].module_present[1]=1;
    liste_classe[1].module_present[4]=1;
    liste_classe[1].module_present[6]=1;
    //liste_classe[1].module_present[7]=1;
    liste_classe[1].module_present[13]=1;

    liste_classe[2].image[0]->load("ressource/unit/missile/0.png");
    liste_classe[2].image[1]->load("ressource/unit/missile/1.png");
    liste_classe[2].image[2]->load("ressource/unit/missile/2.png");
    liste_classe[2].image[3]->load("ressource/unit/missile/3.png");
    liste_classe[2].image[4]->load("ressource/unit/missile/4.png");

    liste_classe[2].nom="Harpoon";
    liste_classe[2].type=1;
    liste_classe[2].pays=USA;
    liste_classe[2].module_present[2]=1;
    liste_classe[2].module_present[3]=1;
    liste_classe[2].module_present[6]=1;
    liste_classe[2].module_present[14]=1;
    liste_classe[2].warhead=1000;

    liste_classe[3].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[3].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[3].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[3].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[3].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[3].nom="Gerald R. Ford";
    liste_classe[3].type=0;
    liste_classe[3].pays=USA;
    liste_classe[3].frottement=1.0;
    liste_classe[3].module_present[0]=1;
    liste_classe[3].module_present[1]=1;
    liste_classe[3].module_present[5]=1;
    liste_classe[3].module_present[6]=1;
    liste_classe[3].module_present[12]=1;

    liste_classe[4].image[0]->load("ressource/unit/missile/0.png");
    liste_classe[4].image[1]->load("ressource/unit/missile/1.png");
    liste_classe[4].image[2]->load("ressource/unit/missile/2.png");
    liste_classe[4].image[3]->load("ressource/unit/missile/3.png");
    liste_classe[4].image[4]->load("ressource/unit/missile/4.png");

    liste_classe[4].nom="F-35C";
    liste_classe[4].type=2;
    liste_classe[4].pays=USA;
    liste_classe[4].frottement=100.0;
    liste_classe[4].module_present[2]=1;
    liste_classe[4].module_present[3]=1;
    liste_classe[4].module_present[4]=1;
    liste_classe[4].module_present[6]=1;
    liste_classe[4].module_present[7]=1;

    liste_classe[5].image[0]->load("ressource/unit/missile/0.png");
    liste_classe[5].image[1]->load("ressource/unit/missile/1.png");
    liste_classe[5].image[2]->load("ressource/unit/missile/2.png");
    liste_classe[5].image[3]->load("ressource/unit/missile/3.png");
    liste_classe[5].image[4]->load("ressource/unit/missile/4.png");

    liste_classe[5].nom="HHQ-9";
    liste_classe[5].type=1;
    liste_classe[5].pays=CHINA;
    liste_classe[5].module_present[2]=1;
    liste_classe[5].module_present[3]=1;
    liste_classe[5].module_present[6]=1;
    liste_classe[5].module_present[14]=1;
    liste_classe[5].warhead=1000;

    liste_classe[6].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[6].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[6].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[6].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[6].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[6].nom="Virginia";
    liste_classe[6].type=3;
    liste_classe[6].pays=USA;
    liste_classe[6].frottement=1.0;
    liste_classe[6].module_present[0]=1;
    liste_classe[6].module_present[16]=1; // A CHANGER
    liste_classe[6].module_present[8]=1;
    liste_classe[6].module_present[6]=1;
    liste_classe[6].module_present[17]=1;

    liste_classe[7].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[7].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[7].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[7].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[7].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[7].nom="Type 094";
    liste_classe[7].type=3;
    liste_classe[7].pays=CHINA;
    liste_classe[7].frottement=1.0;
    liste_classe[7].module_present[0]=1;
    liste_classe[7].module_present[1]=1;
    liste_classe[7].module_present[5]=1;
    liste_classe[7].module_present[6]=1;

    liste_classe[8].image[0]->load("ressource/unit/marine/0.png");
    liste_classe[8].image[1]->load("ressource/unit/marine/1.png");
    liste_classe[8].image[2]->load("ressource/unit/marine/2.png");
    liste_classe[8].image[3]->load("ressource/unit/marine/3.png");
    liste_classe[8].image[4]->load("ressource/unit/marine/4.png");

    liste_classe[8].nom="USMC";
    liste_classe[8].type=5;
    liste_classe[8].pays=USA;
    liste_classe[8].frottement=1.0;
    liste_classe[8].module_present[9]=1;
    liste_classe[8].module_present[10]=1;

    liste_classe[9].image[0]->load("ressource/unit/marine/0.png");
    liste_classe[9].image[1]->load("ressource/unit/marine/1.png");
    liste_classe[9].image[2]->load("ressource/unit/marine/2.png");
    liste_classe[9].image[3]->load("ressource/unit/marine/3.png");
    liste_classe[9].image[4]->load("ressource/unit/marine/4.png");

    liste_classe[9].nom="Marines";
    liste_classe[9].type=5;
    liste_classe[9].taille=5;
    liste_classe[9].pays=CHINA;
    liste_classe[9].frottement=1.0;
    liste_classe[9].module_present[9]=1;

    liste_classe[10].image[0]->load("ressource/unit/arleigh_burke/0.png");
    liste_classe[10].image[1]->load("ressource/unit/arleigh_burke/1.png");
    liste_classe[10].image[2]->load("ressource/unit/arleigh_burke/2.png");
    liste_classe[10].image[3]->load("ressource/unit/arleigh_burke/3.png");
    liste_classe[10].image[4]->load("ressource/unit/arleigh_burke/4.png");

    liste_classe[10].nom="SSC";
    liste_classe[10].type=0;
    liste_classe[10].pays=USA;
    liste_classe[10].frottement=1.0;
    liste_classe[10].module_present[0]=1;
    liste_classe[10].module_present[1]=1;
    liste_classe[10].module_present[5]=1;
    liste_classe[10].module_present[6]=1;
    liste_classe[10].module_present[11]=1;

    liste_classe[11].image[0]->load("ressource/unit/lav-25/0.png");
    liste_classe[11].image[1]->load("ressource/unit/lav-25/1.png");
    liste_classe[11].image[2]->load("ressource/unit/lav-25/2.png");
    liste_classe[11].image[3]->load("ressource/unit/lav-25/3.png");
    liste_classe[11].image[4]->load("ressource/unit/lav-25/4.png");

    liste_classe[11].nom="LAV-25";
    liste_classe[11].type=6;
    liste_classe[11].taille=5;
    liste_classe[11].pays=USA;
    liste_classe[11].frottement=1.0;
    liste_classe[11].module_present[2]=1;
    //liste_classe[11].module_present[1]=1;
    liste_classe[11].module_present[6]=1;
    liste_classe[11].module_present[18]=1;

    for (int i = 0; i < NOMBRE_CLASSE; i++){
        for (int k = 0; k <= 4; k++){
            liste_classe[i].small_image[k] = liste_classe[i].image[k]->scaled(liste_classe[i].image[k]->width()/2 , liste_classe[i].image[k]->height()/2);
        }
    }
}

void Fenetre::chargement_liste_module ()
{
    for (unsigned int i=0; i<NOMBRE_MODULE; i++)
    {
        liste_module[i].type=0;
        liste_module[i].altitude_max=0;
        liste_module[i].altitude_min=0;
        liste_module[i].altitude_croisiere=0;
        liste_module[i].pv=1;
        liste_module[i].fuel=0;
        liste_module[i].repair_object = 0;
        liste_module[i].vision=1;
        liste_module[i].conso=1.0;
        liste_module[i].vitesse_max=0.0;
        liste_module[i].vitesse_min=0.0;
        liste_module[i].angle_montee=0.0;
        liste_module[i].vitesse_rotation=0.0;
        liste_module[i].acceleration=0.0;
        liste_module[i].decceleration=0.0;
        liste_module[i].resistance=1.0;
        liste_module[i].proba_fail=1.0;
        liste_module[i].proba_fail_fin=1.0;
        liste_module[i].nom="";

        liste_module[i].peut_lancer = new bool[NOMBRE_OBJET];
        if (liste_module[i].peut_lancer == NULL){
            exit(666);
        }
        for (unsigned int j=0; j<NOMBRE_OBJET; j++)
        {
            liste_module[i].peut_lancer[j]=0;
        }
        liste_module[i].puissance_radar_passif = 1;
        for (unsigned int j=0; j<NOMBRE_TYPE; j++)
        {
            liste_module[i].peut_detecter[j] = 0;
            liste_module[i].puissance_radar[j] = 1;
            liste_module[i].processing_radar[j] = 1.0;
            liste_module[i].portee_radar[j]=0;
        }
    }

    QFile file("data/module/index");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open module's index");
        return;
    }
    QTextStream stream(&file);

    QString line;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            chargement_module(line);
        }
    } while(!line.isNull());

    liste_module[0].nom="Hull";   // DDG
    liste_module[0].pv=1000;
    liste_module[0].resistance=0.33;

    /*liste_module[1].nom="Motor";   // moteur DDG
    liste_module[1].type=1;
    liste_module[1].altitude_max=0;
    liste_module[1].altitude_min=0;
    liste_module[1].altitude_croisiere=0;
    liste_module[1].pv=100;
    liste_module[1].conso=10.0;
    liste_module[1].resistance=0.5;
    liste_module[1].vitesse_max=60.0;
    liste_module[1].vitesse_min=0.0;
    liste_module[1].vitesse_rotation=0.1;
    liste_module[1].acceleration=10.0;
    liste_module[1].decceleration=15.0;*/

    /*liste_module[4].type=2;
    liste_module[4].nom="MK-41";   // VLS DDG
    liste_module[4].pv=100;
    liste_module[4].resistance=0.33;
    liste_module[4].peut_lancer[0]=1;
    liste_module[4].peut_lancer[2]=1;*/

    liste_module[5].type=2;
    liste_module[5].nom="Flight Deck";   // Flight Deck
    liste_module[5].pv=100;
    liste_module[5].resistance=0.33;
    liste_module[5].peut_lancer[1]=1;
    liste_module[5].peut_lancer[9]=1;


    liste_module[2].nom="Hull";   // coque missile
    liste_module[2].pv=10;
    liste_module[2].resistance=1.0;

    liste_module[3].nom="Motor";   // moteur missile
    liste_module[3].type=1;
    liste_module[3].altitude_max=10000;
    liste_module[3].altitude_min=0;
    liste_module[3].altitude_croisiere=10;
    liste_module[3].pv=1;
    liste_module[3].resistance=1;
    liste_module[3].conso=10.0;
    liste_module[3].vitesse_max=3600.0;
    liste_module[3].vitesse_min=1000.0;
    liste_module[3].vitesse_rotation=1.0;
    liste_module[3].acceleration=6000.0;
    liste_module[3].decceleration=1000.0;
    liste_module[3].angle_montee=1.0;

    liste_module[6].nom="Fuel Tank";   // reservoir missile
    liste_module[6].type=3;
    liste_module[6].pv=1;
    liste_module[6].resistance=1;
    liste_module[6].fuel=10000;

    liste_module[7].nom="Radar F-35";   // A CHANGER
    liste_module[7].type=4;
    liste_module[7].pv=1;
    liste_module[7].resistance=1;
    liste_module[7].puissance_radar[0]=20000;
    liste_module[7].portee_radar[0]=10000000;
    liste_module[7].peut_detecter[0]=1;
    liste_module[7].peut_detecter[1]=1;
    liste_module[7].puissance_radar[1]=20000;
    liste_module[7].portee_radar[1]=10000000;
    liste_module[7].peut_detecter[2]=1;
    liste_module[7].puissance_radar[2]=20000;
    liste_module[7].portee_radar[2]=10000000;

    liste_module[8].nom="Sonar DDG";   // A CHANGER
    liste_module[8].type=4;
    liste_module[8].pv=1;
    liste_module[8].resistance=1;
    liste_module[8].puissance_radar[3]=100;
    liste_module[8].portee_radar[3]=10000000;
    liste_module[8].peut_detecter[3]=1;

    liste_module[9].nom="Inf20";
    liste_module[9].type=5;
    liste_module[9].pv=20;
    liste_module[9].resistance=1.0;
    liste_module[9].vision=100;
    liste_module[9].vitesse_max=10;
    liste_module[9].acceleration=10;
    liste_module[9].vitesse_rotation=1.0;
    liste_module[9].decceleration=10;
    liste_module[9].altitude_max = 10000;
    liste_module[9].altitude_min = 1;

    liste_module[10].nom="Rifle";
    liste_module[10].type=2;
    liste_module[10].pv=20;
    liste_module[10].resistance=0.2;
    liste_module[10].peut_lancer[3] = 1;

    liste_module[11].nom="Landing pad"; // changer nom
    liste_module[11].type=2;
    liste_module[11].pv=20;
    liste_module[11].resistance=0.2;
    liste_module[11].peut_lancer[5] = 1;
    liste_module[11].peut_lancer[6] = 1;

    liste_module[12].nom="Well deck";
    liste_module[12].type=2;
    liste_module[12].pv=20;
    liste_module[12].resistance=0.2;
    liste_module[12].peut_lancer[4] = 1;
    liste_module[12].peut_lancer[5] = 1;

    liste_module[13].nom="Type 1130 CIWS";
    liste_module[13].type=2;
    liste_module[13].pv=20;
    liste_module[13].resistance=0.2;
    liste_module[13].peut_lancer[7] = 1;
}

void Fenetre::chargement_liste_objet ()
{
    for (unsigned int i=0; i<NOMBRE_OBJET; i++){
        liste_objet[i].type=0;
        liste_objet[i].future_unite=2;
        liste_objet[i].taille=1;
        liste_objet[i].temps_recharge=1;
        liste_objet[i].nom="NO_OBJECT" + QString::number(i);
        liste_objet[i].image= new QPixmap();
        //liste_objet[i].image->load("ressource/object/harpoon.png");

        liste_objet[i].data.precision = 1.0;
        liste_objet[i].data.portee = 0.0;
        liste_objet[i].data.vitesse = 0.0;
        liste_objet[i].data.warhead = 0.0;
        liste_objet[i].data.zone_degat = 0.0;
        liste_objet[i].data.explosion = false;
        liste_objet[i].data.nb_tir = 1;

        liste_objet[i].asat = false;
        for (unsigned int j=0; j<NOMBRE_TYPE; j++)
        {
            liste_objet[i].peut_toucher[j]=0;
        }
    }

    QFile file("data/object/index");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open object's index");
        return;
    }
    QTextStream stream(&file);

    QString line;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            chargement_objet(line);
        }
    } while(!line.isNull());

    liste_objet[2].type=0;
    liste_objet[2].future_unite=5;  // HHQ-9
    liste_objet[2].taille=1;
    liste_objet[2].temps_recharge=0.1;
    liste_objet[2].nom="HHQ-9";
    liste_objet[2].peut_toucher[1]=1;
    liste_objet[2].peut_toucher[2]=1;
    liste_objet[2].image->load("ressource/object/F-35.png");

    liste_objet[3].type=2;
    liste_objet[3].taille=0.01;
    liste_objet[3].nom="Rifle ammo";
    liste_objet[3].peut_toucher[5]=1;
    liste_objet[3].data.portee = 3000;
    liste_objet[3].data.vitesse = 1000;
    liste_objet[3].data.warhead = 0.05;
    liste_objet[3].data.nb_tir = 10;
    liste_objet[3].image->load("ressource/object/harpoon.png");

    /*liste_objet[4].type=6;
    liste_objet[4].future_unite=10;  //SSC
    liste_objet[4].taille=10;
    liste_objet[4].nom="SSC";
    liste_objet[4].image->load("ressource/object/F-35.png");
    config con;
    coo co;
    con.nom = "config0";
    co.x = 5; co.y = 1;
    con.liste.push_back(co);
    co.x = 6; co.y = 2;
    con.liste.push_back(co);
    liste_objet[4].liste_config.push_back(con);

    con.liste.clear();
    con.nom = "config1";
    co.x = 6; co.y = 1;
    con.liste.push_back(co);
    liste_objet[4].liste_config.push_back(con);*/


    liste_objet[5].type=3;
    liste_objet[5].future_unite=11;  //LAV-25
    liste_objet[5].taille=10;
    liste_objet[5].nom="LAV-25";
    liste_objet[5].image->load("ressource/object/F-35.png");

    liste_objet[6].type=5;
    liste_objet[6].future_unite=8;  //USMC
    liste_objet[6].taille=10;
    liste_objet[6].nom="USMC";
    liste_objet[6].image->load("ressource/object/F-35.png");

}

void Fenetre::chargement_objet(QString file_name){

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open object file : " + file_name);
        return;
    }
    QTextStream stream(&file);

    QString line;
    QStringList liste;
    int index = -1;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            line.simplified();

            liste = line.split(' ');

            if (liste.size() > 0){
                liste[0].toLower();

                if (liste[0] == "id" && liste.size() == 2){
                    index = liste[1].toInt();

                    if (index < 0 || index >= NOMBRE_OBJET){
                        erreur_log("Error in " + file_name + " : id not in range");
                    }
                }
                else if (index == -1){
                    erreur_log("Error in " + file_name + " : id not set");
                }
                else if (liste[0] == "type" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type > 8){
                        erreur_log("Error in " + file_name + " : type not in range");
                    }
                    else {
                        liste_objet[index].type = type;
                    }
                }
                else if (liste[0] == "future_unit" && liste.size() == 2){
                    int unit = liste[1].toInt();

                    if (unit < 0 || unit > NOMBRE_CLASSE){
                        erreur_log("Error in " + file_name + " : future unit not in range");
                    }
                    else {
                        liste_objet[index].future_unite = unit;
                    }
                }
                else if (liste[0] == "country" && liste.size() == 2){
                    int country = liste[1].toInt();

                    if (country < 0 || country > 1){
                        erreur_log("Error in " + file_name + " : country not in range");
                    }
                    else {
                        liste_objet[index].pays = country;
                    }
                }
                else if (liste[0] == "time_reload" && liste.size() == 2){
                    double ti = liste[1].toDouble();

                    if (ti <= 0){
                        erreur_log("Error in " + file_name + " : time reload can't be negative not null");
                    }
                    else {
                        liste_objet[index].temps_recharge = ti;
                    }
                }
                else if (liste[0] == "size" && liste.size() == 2){
                    double size = liste[1].toDouble();

                    if (size <= 0){
                        erreur_log("Error in " + file_name + " : size can't be negative nor null");
                    }
                    else {
                        liste_objet[index].taille = size;
                    }
                }
                else if (liste[0] == "name" && liste.size() == 2){
                    QString nom = liste[1].replace('*', ' ');

                    if (nom == "config"){
                        erreur_log("Error in " + file_name + " : object can't be named 'config'");
                    }
                    else {
                        liste_objet[index].nom = nom;
                    }
                }
                else if (liste[0] == "image" && liste.size() == 2){
                    QString im = liste[1].replace('*', ' ');;

                    liste_objet[index].image->load(im);

                    if (liste_objet[index].image->isNull()){
                        erreur_log("Error in " + file_name + " : image not found");
                    }
                }
                else if (liste[0] == "asat" && liste.size() == 1){
                    liste_objet[index].asat = true;
                }
                else if (liste[0] == "can_hit" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type >= NOMBRE_TYPE){
                        erreur_log("Error in " + file_name + " : can hit type not in range");
                    }
                    else {
                        liste_objet[index].peut_toucher[type] = true;
                    }
                }
                else if (liste[0] == "new_config" && liste.size() == 2){
                    QString nom = liste[1];
                    bool valid = true;

                    for (unsigned int i = 0; i < liste_objet[index].liste_config.size(); i++){
                        if (nom == liste_objet[index].liste_config[i].nom){
                            valid = false;
                        }
                    }

                    if (!valid){
                        erreur_log("Error in " + file_name + " : config name already exists");
                    }
                    else {
                        config con;
                        con.nom = nom;

                        liste_objet[index].liste_config.push_back(con);
                    }
                }
                else if (liste[0] == "add_config" && liste.size() == 4){
                    QString nom = liste[1];
                    int index_objet = liste[2].toInt();
                    int nombre_objet = liste[3].toInt();
                    int index_config = -1;
                    bool valid = false;

                    for (unsigned int i = 0; i < liste_objet[index].liste_config.size(); i++){
                        if (nom == liste_objet[index].liste_config[i].nom){
                            valid = true;
                            index_config = i;
                        }
                    }

                    if (!valid){
                        erreur_log("Error in " + file_name + " : config name does not exist");
                    }
                    else if (nombre_objet <= 0){
                        erreur_log("Error in " + file_name + " : number of objects can't be negative nor null");
                    }
                    else if (index_objet < 0 || index_objet > NOMBRE_OBJET){
                        erreur_log("Error in " + file_name + " : id of config object not in range");
                    }
                    else {
                        coo co;
                        co.x = index_objet;
                        co.y = nombre_objet;

                        liste_objet[index].liste_config[index_config].liste.push_back(co);
                    }
                }
                else if (liste[0] == "explosion" && liste.size() == 2 && liste_objet[index].type == 2){
                    int expl = liste[1].toInt();

                    liste_objet[index].data.explosion = (expl != 0);
                }
                else if (liste[0] == "number_fired" && liste.size() == 2 && liste_objet[index].type == 2){
                    int nb = liste[1].toInt();

                    if (nb <= 0){
                        erreur_log("Error in " + file_name + " : number fired not in range");
                    }
                    else {
                        liste_objet[index].data.nb_tir = nb;
                    }
                }
                else if (liste[0] == "precision" && liste.size() == 2 && liste_objet[index].type == 2){
                    double nb = liste[1].toDouble();

                    if (nb < 0 || nb > 1){
                        erreur_log("Error in " + file_name + " : precision not in range");
                    }
                    else {
                        liste_objet[index].data.precision = nb;
                    }
                }
                else if (liste[0] == "damage_zone" && liste.size() == 2 && liste_objet[index].type == 2){
                    double nb = liste[1].toDouble();

                    if (nb <= 0){
                        erreur_log("Error in " + file_name + " : damage zone not in range");
                    }
                    else {
                        liste_objet[index].data.zone_degat = nb;
                    }
                }
                else if (liste[0] == "speed" && liste.size() == 2 && liste_objet[index].type == 2){
                    double nb = liste[1].toDouble();

                    if (nb <= 0){
                        erreur_log("Error in " + file_name + " : speed not in range");
                    }
                    else {
                        liste_objet[index].data.vitesse = nb;
                    }
                }
                else if (liste[0] == "range" && liste.size() == 2 && liste_objet[index].type == 2){
                    double nb = liste[1].toDouble();

                    if (nb <= 0){
                        erreur_log("Error in " + file_name + " : range not in range");
                    }
                    else {
                        liste_objet[index].data.portee = nb;
                    }
                }
                else if (liste[0] == "warhead" && liste.size() == 2 && liste_objet[index].type == 2){
                    double nb = liste[1].toDouble();

                    if (nb <= 0){
                        erreur_log("Error in " + file_name + " : warhead type not in range");
                    }
                    else {
                        liste_objet[index].data.warhead = nb;
                    }
                }
                else {
                    erreur_log("Error in " + file_name + " : unrecognized command : " + line);
                }
            }
        }

    } while (!line.isNull());
}

void Fenetre::chargement_module(QString file_name){

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open module file : " + file_name);
        return;
    }
    QTextStream stream(&file);

    QString line;
    QStringList liste;
    int index = -1;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            line.simplified();

            liste = line.split(' ');

            if (liste.size() > 0){
                liste[0].toLower();

                if (liste[0] == "id" && liste.size() == 2){
                    index = liste[1].toInt();

                    if (index < 0 || index >= NOMBRE_MODULE){
                        erreur_log("Error in " + file_name + " : id not in range");
                    }
                }
                else if (index == -1){
                    erreur_log("Error in " + file_name + " : id not set");
                }
                else if (liste[0] == "type" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type > 6){
                        erreur_log("Error in " + file_name + " : type not in range");
                    }
                    else {
                        liste_module[index].type = type;
                    }
                }
                else if (liste[0] == "hitpoint" && liste.size() == 2){
                    int hit = liste[1].toInt();

                    if (hit < 0){
                        erreur_log("Error in " + file_name + " : hitpoint not in range");
                    }
                    else {
                        liste_module[index].pv = hit;
                    }
                }
                else if (liste[0] == "fuel" && liste.size() == 2){
                    int fuel = liste[1].toInt();

                    if (fuel < 0){
                        erreur_log("Error in " + file_name + " : fuel not in range");
                    }
                    else {
                        liste_module[index].fuel = fuel;
                    }
                }
                else if (liste[0] == "sight" && liste.size() == 2){
                    double si = liste[1].toDouble();

                    if (si < 1 || si > 100){
                        erreur_log("Error in " + file_name + " : sight not in range");
                    }
                    else {
                        liste_module[index].vision = si;
                    }
                }
                else if (liste[0] == "vulnerability" && liste.size() == 2){
                    double vul = liste[1].toDouble();

                    if (vul < 0 || vul > 1){
                        erreur_log("Error in " + file_name + " : vulnerability not in range");
                    }
                    else {
                        liste_module[index].resistance = vul;
                    }
                }
                else if (liste[0] == "repair_object" && liste.size() == 2){
                    int vul = liste[1].toInt();

                    if (vul < 0){
                        erreur_log("Error in " + file_name + " : repair_object not in range");
                    }
                    else {
                        liste_module[index].repair_object = vul;
                    }
                }
                else if (liste[0] == "name" && liste.size() == 2){
                    QString nom = liste[1].replace('*', ' ');

                    liste_module[index].nom = nom;
                }
                else if (liste[0] == "can_detect" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type >= NOMBRE_TYPE){
                        erreur_log("Error in " + file_name + " : can detect type not in range");
                    }
                    else {
                        liste_module[index].peut_detecter[type] = true;
                    }
                }
                else if (liste[0] == "can_launch" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type >= NOMBRE_OBJET){
                        erreur_log("Error in " + file_name + " : can launch type not in range");
                    }
                    else {
                        liste_module[index].peut_lancer[type] = true;
                    }
                }
                else if (liste[0] == "radar_range" && liste.size() == 3){
                    int type = liste[1].toInt();
                    int range = liste[2].toInt();

                    if (type < 0 || type >= NOMBRE_TYPE){
                        erreur_log("Error in " + file_name + " : radar range type not in range");
                    }
                    else if (range < 0){
                        erreur_log("Error in " + file_name + " : range can't be negative");
                    }
                    else {
                        liste_module[index].portee_radar[type] = range;
                    }
                }
                else if (liste[0] == "radar_power" && liste.size() == 3){
                    int type = liste[1].toInt();
                    int pow = liste[2].toInt();

                    if (type < 0 || type >= NOMBRE_TYPE){
                        erreur_log("Error in " + file_name + " : radar power type not in range");
                    }
                    else if (pow < 0 || pow > 100){
                        erreur_log("Error in " + file_name + " : radar power not in range");
                    }
                    else {
                        liste_module[index].puissance_radar[type] = pow;
                        liste_module[index].peut_detecter[type] = true;
                    }
                }
                else if (liste[0] == "radar_processing" && liste.size() == 3){
                    int type = liste[1].toInt();
                    double pow = liste[2].toDouble();

                    if (type < 0 || type >= NOMBRE_TYPE){
                        erreur_log("Error in " + file_name + " : radar processing type not in range");
                    }
                    else if (pow < 0){
                        erreur_log("Error in " + file_name + " : radar processing not in range");
                    }
                    else {
                        liste_module[index].processing_radar[type] = pow;
                    }
                }
                else if (liste[0] == "radar_passive" && liste.size() == 2){
                    int pow = liste[1].toInt();

                    if (pow < 0 || pow > 100){
                        erreur_log("Error in " + file_name + " : radar passive not in range");
                    }
                    else {
                        liste_module[index].puissance_radar_passif = pow;
                    }
                }
                else if (liste[0] == "consumption" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : consumption not in range");
                    }
                    else {
                        liste_module[index].conso = conso;
                    }
                }
                else if (liste[0] == "max_speed" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : max speed not in range");
                    }
                    else {
                        liste_module[index].vitesse_max = conso;
                    }
                }
                else if (liste[0] == "min_speed" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : min speed not in range");
                    }
                    else {
                        liste_module[index].vitesse_min = conso;
                    }
                }
                else if (liste[0] == "max_altitude" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    liste_module[index].altitude_max = conso;
                }
                else if (liste[0] == "min_altitude" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    liste_module[index].altitude_min = conso;
                }
                else if (liste[0] == "cruise_altitude" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    liste_module[index].altitude_croisiere = conso;
                }
                else if (liste[0] == "ascendant_angle" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0 || conso > PI){
                        erreur_log("Error in " + file_name + " : ascendant angle not in range");
                    }
                    else {
                        liste_module[index].angle_montee = conso;
                    }
                }
                else if (liste[0] == "acceleration" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : acceleration not in range");
                    }
                    else {
                        liste_module[index].acceleration = conso;
                    }
                }
                else if (liste[0] == "deceleration" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : deceleration not in range");
                    }
                    else {
                        liste_module[index].decceleration = conso;
                    }
                }
                else if (liste[0] == "rotation_speed" && liste.size() == 2 && liste_module[index].type == 1){
                    double conso = liste[1].toDouble();

                    if (conso < 0){
                        erreur_log("Error in " + file_name + " : rotation speed not in range");
                    }
                    else {
                        liste_module[index].vitesse_rotation = conso;
                    }
                }
                else if (liste[0] == "fail_probability" && liste.size() == 2 && liste_module[index].type == 6){
                    double probo = liste[1].toDouble();

                    if (probo < 0 || probo > 1){
                        erreur_log("Error in " + file_name + " : fail probability not in range");
                    }
                    else {
                        liste_module[index].proba_fail = probo;
                    }
                }
                else if (liste[0] == "miss_probability" && liste.size() == 2 && liste_module[index].type == 6){
                    double probo = liste[1].toDouble();

                    if (probo < 0 || probo > 1){
                        erreur_log("Error in " + file_name + " : miss probability not in range");
                    }
                    else {
                        liste_module[index].proba_fail_fin = probo;
                    }
                }
                else {
                    erreur_log("Error in " + file_name + " : unrecognized command : " + line);
                }
            }
        }

    } while (!line.isNull());
}

void Fenetre::chargement_classe(QString file_name){
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        erreur_log("Could not open class file : " + file_name);
        return;
    }
    QTextStream stream(&file);

    QString line;
    QStringList liste;
    int index = -1;
    do {
        line = stream.readLine();

        if (!line.isNull()){
            line.simplified();

            liste = line.split(' ');

            if (liste.size() > 0){
                liste[0].toLower();

                if (liste[0] == "id" && liste.size() == 2){
                    index = liste[1].toInt();

                    if (index < 0 || index >= NOMBRE_CLASSE){
                        erreur_log("Error in " + file_name + " : id not in range");
                    }
                }
                else if (index == -1){
                    erreur_log("Error in " + file_name + " : id not set");
                }
                else if (liste[0] == "type" && liste.size() == 2){
                    int type = liste[1].toInt();

                    if (type < 0 || type > 8){
                        erreur_log("Error in " + file_name + " : type not in range");
                    }
                    else {
                        liste_classe[index].type = type;
                    }
                }
                else if (liste[0] == "country" && liste.size() == 2){
                    int country = liste[1].toInt();

                    if (country < 0 || country >= NOMBRE_PAYS){
                        erreur_log("Error in " + file_name + " : country not in range");
                    }
                    else {
                        liste_classe[index].pays = country;
                    }
                }
                else if (liste[0] == "size" && liste.size() == 2){
                    int size = liste[1].toInt();

                    if (size <= 0 || size > 100){
                        erreur_log("Error in " + file_name + " : size can't be negative nor null nor bigger than 100");
                    }
                    else {
                        liste_classe[index].taille = size;
                    }
                }
                else if (liste[0] == "name" && liste.size() == 2){
                    QString nom = liste[1].replace('*', ' ');

                    liste_classe[index].nom = nom;
                }
                else if (liste[0] == "image" && liste.size() == 2){
                    QString im = liste[1].replace('*', ' ');

                    for (int k = 0; k <= 4; k++){
                        if (!liste_classe[index].image[k]->load(im + "/" + QString::number(k) + ".png")){
                            erreur_log("Error in " + file_name + " : image not found");
                        }
                        //liste_classe[index].small_image[k] = liste_classe[index].image[k]->scaled(liste_classe[index].image[k]->width()/2 , liste_classe[index].image[k]->height()/2);
                    }


                }
                else if (liste[0] == "fuel" && liste.size() == 2){
                    int size = liste[1].toInt();

                    if (size < 0){
                        erreur_log("Error in " + file_name + " : fuel can't be negative");
                    }
                    else {
                        liste_classe[index].fuel_base = size;
                    }
                }
                else if (liste[0] == "stealth" && liste.size() == 2){
                    int size = liste[1].toInt();

                    if (size < 0 || size > 100){
                        erreur_log("Error in " + file_name + " : stealth can't be negative nor bigger than 100");
                    }
                    else {
                        liste_classe[index].stealth = size;
                    }
                }
                else if (liste[0] == "module" && liste.size() == 2){
                    int size = liste[1].toInt();

                    if (size < 0 || size >= NOMBRE_MODULE){
                        erreur_log("Error in " + file_name + " : module id not in range");
                    }
                    else {
                        liste_classe[index].module_present[size] = true;
                    }
                }
                else if (liste[0] == "warhead" && liste.size() == 2 && liste_classe[index].type == 1){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  warhead can't be negative");
                    }
                    else {
                        liste_classe[index].warhead = war;
                    }
                }
                else if (liste[0] == "friction" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  friction can't be negative");
                    }
                    else {
                        liste_classe[index].frottement = war;
                    }
                }
                else if (liste[0] == "noise" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0 || war > 1){
                        erreur_log("Error in " + file_name + " :  noise not in range");
                    }
                    else {
                        liste_classe[index].frottement = war;
                    }
                }
                else if (liste[0] == "cargo_ammo" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  cargo not in range");
                    }
                    else {
                        liste_classe[index].hangar[0] = war;
                    }
                }
                else if (liste[0] == "plane_hangar" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  cargo not in range");
                    }
                    else {
                        liste_classe[index].hangar[1] = war;
                    }
                }
                else if (liste[0] == "helo_hangar" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  cargo not in range");
                    }
                    else {
                        liste_classe[index].hangar[2] = war;
                    }
                }
                else if (liste[0] == "well_dock" && liste.size() == 2){
                    double war = liste[1].toDouble();

                    if (war < 0){
                        erreur_log("Error in " + file_name + " :  cargo not in range");
                    }
                    else {
                        liste_classe[index].hangar[3] = war;
                    }
                }
                else {
                    erreur_log("Error in " + file_name + " : unrecognized command : " + line);
                }
            }
        }

    } while (!line.isNull());
}
