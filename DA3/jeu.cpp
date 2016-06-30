#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>

void Fenetre::lancer_jeu()
{
    setCursor(Qt::WaitCursor);



    pixmap_menu->setVisible(false);
    bouton_jeu->setVisible(false);
    bouton_editeur->setVisible(false);
    bouton_generateur->setVisible(false);
    bouton_option->setVisible(false);
    bouton_quit->setVisible(false);

    mode = 1;
    pause = 0;
    vitesse_jeu=1;
    joueur=0;

    largeur_map=2000;
    hauteur_map=2000;
    echelle=500;  // nb m pour 1 pixel
    oldzoom = -1;
    zoom=0;
    camera.x=5;
    camera.y=5;

    id=1;

    chargement_map ("Taiwan");

    chargement("kapoue");

    chargement_ia();

    select_rect=0;

    temps=0;
    fps_precedent=30.0;

    pixmap_map.chg=1;

    /*bouton_pause = new QPushButton("Kapoue23", this);
    bouton_pause->setVisible(true);
    bouton_pause->setObjectName("jeu");
    //bouton_pause->setStyleSheet("QPushButton {background-color: rgb(15, 0, 50); border-width: 2px; border-color: rgb(0, 0, 0); border-style: solid;font: bold 25px; font-family: Rockwell; min-width: 10em; min-height: 1em; padding: 6px;} QPushButton:pressed {  background-color: rgb(0, 30, 100); } QPushButton:hover {  background-color:  rgb(0, 30, 100);}");

    QObject::connect(bouton_pause, SIGNAL(clicked()), this, SLOT(chg_bouton_pause()));*/

    /*bouton_pause = new QPushButton("Campaign", this);
    bouton_pause->move(5, 5);
    //bouton_pause->setVisible(true);
    bouton_pause->setObjectName("validation_action");*/

    barre_principale_jeu = new QWidget(this);

    QHBoxLayout *layout_barre = new QHBoxLayout;

    bouton_pause = new QPushButton("1 X", barre_principale_jeu);
    bouton_pause->setObjectName("s1");
    bouton_pause->setFixedWidth(120);
    layout_barre->addWidget(bouton_pause);
    QObject::connect(bouton_pause, SIGNAL(clicked()), this, SLOT(chg_bouton_pause()));

    bouton_sat_jeu = new QPushButton("Satellites", barre_principale_jeu);
    bouton_sat_jeu->setObjectName("s1");
    bouton_sat_jeu->setFixedWidth(120);
    layout_barre->addWidget(bouton_sat_jeu);
    QObject::connect(bouton_sat_jeu, SIGNAL(clicked()), this, SLOT(chg_bouton_sat_jeu()));

    bouton_log = new QPushButton("Log", barre_principale_jeu);
    bouton_log->setObjectName("s1");
    bouton_log->setFixedWidth(120);
    layout_barre->addWidget(bouton_log);
    QObject::connect(bouton_log, SIGNAL(clicked()), this, SLOT(chg_bouton_log()));

    barre_principale_jeu->setLayout(layout_barre);
    barre_principale_jeu->move(-2,-10);
    barre_principale_jeu->setVisible(true);
    barre_principale_jeu->setFixedHeight(50);
    barre_principale_jeu->setFixedWidth(380);

    QSignalMapper *signalMapper = new QSignalMapper();

    barre_bouton = new QWidget(this);
    QHBoxLayout *layout_barre_bouton = new QHBoxLayout;
    int n = 0;

    for (int i = 0; i < 10; i++){
        bouton_groupe[i] = new QPushButton("Group " + QString::number(i), barre_bouton);
        bouton_groupe[i]->setObjectName("s1");
        bouton_groupe[i]->setFixedWidth(80);
        layout_barre_bouton->addWidget(bouton_groupe[i]);
        layout_barre_bouton->setAlignment(bouton_groupe[i], Qt::AlignLeft);
        bouton_groupe[i]->setVisible(false);

        QObject::connect(bouton_groupe[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(bouton_groupe[i], i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_groupe(int)));

    barre_bouton->setLayout(layout_barre_bouton);
    barre_bouton->move(-2,20);
    barre_bouton->setVisible(true);
    barre_bouton->setFixedHeight(50);
    barre_bouton->setFixedWidth(90 * n);

    setCursor(Qt::CrossCursor);

}

void Fenetre::chg_bouton_groupe(int i){
    onglet_selection->clear();

    for (int j=0; j<liste_selection.size(); j++){
        int k=index_unite(liste_selection[j]);
        if (k!=-1){
            liste_unite[k].chg=1;
        }
    }

    liste_selection.clear();

    for (int j=0; j<liste_groupe[i].size(); j++){
        liste_selection.push_back(liste_groupe[i][j]);
        int in = index_unite(liste_groupe[i][j]);
        if (in != -1){
            onglet_selection->addTab(liste_unite[in].widg_selection, liste_unite[in].nom + " / " + liste_classe[liste_unite[in].type].nom);
        }
        else {
            erreur_log("in2 == -1");
        }

    }

    int current = onglet_selection->currentIndex();
    changement_tab(current);

    onglet_selection->setVisible(onglet_selection->count() > 0);
}

void Fenetre::chg_bouton_pause(){
    if (!pause){
        pause=true;
    }
    else{
        pause=false;
    }
}

void Fenetre::chg_bouton_sat_jeu(){
    onglet_selection->setVisible(false);
    if (fenetre_sat_jeu == 0){
        construction_fenetre_sat_jeu();
    }
    else {
        fenetre_sat_jeu->setVisible(!fenetre_sat_jeu->isVisible());
    }
}

void Fenetre::construction_fenetre_sat_jeu(){
    if (fenetre_sat_jeu!=0){
        delete fenetre_sat_jeu;
    }

    fenetre_sat_jeu = new QGroupBox("Satellites", this);
    QHBoxLayout* layout_fenetre_sat= new QHBoxLayout(fenetre_sat_jeu);
    fenetre_sat_jeu->setObjectName("s2");
    QScrollArea *scroll_sat = new QScrollArea(fenetre_sat_jeu);
    scroll_sat->setObjectName("s3");
    //scroll_sat->verticalScrollBar()->setStyleSheet(" QScrollBar:vertical {border: 2px solid grey;background: #32CC99;width: 15px;margin: 22px 0 22px 0;} QScrollBar::handle:vertical {background: white;min-height: 20px; }QScrollBar::add-line:vertical { border: 2px solid grey;background: #32CC99;height: 20px;subcontrol-position: bottom;subcontrol-origin: margin;}QScrollBar::sub-line:vertical {border: 2px solid grey;background: #32CC99;height: 20px;subcontrol-position: top;subcontrol-origin: margin;}QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {border: 2px solid grey;width: 3px;height: 3px;background: white;}QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
    QVBoxLayout* layout_scroll_sat = new QVBoxLayout(scroll_sat);
    QWidget *fenetre_sat_interieur = new QWidget(scroll_sat);
    QGridLayout* layout_fenetre_sat_interieur = new QGridLayout(fenetre_sat_interieur);
    layout_fenetre_sat_interieur->setVerticalSpacing(20);

    //fenetre_sat_jeu->setContentsMargins(20, 20, 20, 20);
    //layout_fenetre_sat->setContentsMargins(20, 20, 20, 20);

    id_unite_asat = -1;

    for (int i = 0; i < liste_unite.size(); i++){
        if (liste_classe[liste_unite[i].type].pays == joueur){
            for (int j = 0; j < NOMBRE_OBJET; j++){
                if (liste_objet[j].asat && liste_unite[i].nombre_objet[j] > 0 && liste_unite[i].peut_lancer[j]){
                    id_unite_asat = liste_unite[i].id;
                }
            }
        }
    }

    QSignalMapper *signalMapper = new QSignalMapper();

    for (int i = 0; i < liste_sat.size(); i++){
        QLabel* label_nom = new QLabel(liste_sat[i].nom, fenetre_sat_interieur);
        QLabel* label_des = new QLabel(fenetre_sat_interieur);

        QString desc = "";

        desc += liste_pays[liste_sat[i].pays].demonym + " ";

        if (liste_sat[i].type == 0){
            desc += "geostationnary satellite";
        }
        else {
            desc += "non-geostationnary satellite";
            if (liste_sat[i].temps_propre > liste_sat[i].temps_ailleurs){
                desc += "\nOn zone, " + QString::number(liste_sat[i].temps_ailleurs + liste_sat[i].temps_dessus - liste_sat[i].temps_propre) + " s left";
            }
            else {
                desc += "\nNot on zone, " + QString::number(liste_sat[i].temps_ailleurs - liste_sat[i].temps_propre) + " s until arrival";
            }
        }

        if (liste_sat[i].description != ""){
            desc += "\n" + liste_sat[i].description;
        }

        label_des->setText(desc);

        layout_fenetre_sat_interieur->addWidget(label_nom, i, 0);
        layout_fenetre_sat_interieur->addWidget(label_des, i, 1);

        if (liste_pays[liste_sat[i].pays].relation[joueur] == 2 && id_unite_asat != -1 && (liste_sat[i].type == 0 || (liste_sat[i].type == 1 && liste_sat[i].temps_propre > liste_sat[i].temps_ailleurs))){
            QPushButton* bouton_action = new QPushButton("Go to a ASAT unit", fenetre_sat_interieur);
            layout_fenetre_sat_interieur->addWidget(bouton_action, i, 2);

            QObject::connect(bouton_action, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(bouton_action, i);
        }
        else if (liste_pays[liste_sat[i].pays].relation[joueur] == 2 && id_unite_asat != -1 && liste_sat[i].type == 1 && liste_sat[i].temps_propre <= liste_sat[i].temps_ailleurs){
            QPushButton* bouton_action = new QPushButton("Cannot destroy a\nsatellite out of zone", fenetre_sat_interieur);
            layout_fenetre_sat_interieur->addWidget(bouton_action, i, 2);
            bouton_action->setEnabled(false);
        }
        else if (liste_pays[liste_sat[i].pays].relation[joueur] == 2 && id_unite_asat == -1){
            QPushButton* bouton_action = new QPushButton("No ASAT missile available", fenetre_sat_interieur);
            layout_fenetre_sat_interieur->addWidget(bouton_action, i, 2);
            bouton_action->setEnabled(false);
        }
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_action_sat(int)));

    fenetre_sat_interieur->setLayout(layout_fenetre_sat_interieur);

    layout_scroll_sat->addWidget(fenetre_sat_interieur);
    scroll_sat->setWidget(fenetre_sat_interieur);
    scroll_sat->setWidgetResizable(true);
    scroll_sat->setLayout(layout_scroll_sat);

    layout_fenetre_sat->addWidget(scroll_sat, Qt::AlignCenter);
    fenetre_sat_jeu->setLayout(layout_fenetre_sat);


    fenetre_sat_jeu->setVisible(true);
    fenetre_sat_jeu->setFixedHeight(600);
    fenetre_sat_jeu->setFixedWidth(400);

    fenetre_sat_jeu->move(LARGEUR_FENETRE/2-fenetre_sat_jeu->width()/2, HAUTEUR_FENETRE/2-fenetre_sat_jeu->height()/2);
}

void Fenetre::chg_bouton_action_sat(int a){
    int i = index_unite(id_unite_asat);
    if (i == -1){
        return;
    }

    for (int j=0; j<liste_selection.size(); j++){
        int k=index_unite(liste_selection[j]);
        if (k!=-1){
            liste_unite[k].chg=1;
        }
    }

    fenetre_sat_jeu->setVisible(false);
    liste_selection.clear();
    onglet_selection->clear();

    liste_selection.push_back(id_unite_asat);
    onglet_selection->addTab(liste_unite[i].widg_selection, liste_unite[i].nom + " / " + liste_classe[liste_unite[i].type].nom);
    changement_tab(0);
    onglet_selection->setVisible(true);

    QGroupBox *groupe = liste_unite[i].widg_selection->findChild<QGroupBox *>("groupe");
    QWidget *sous1 = 0;
    QWidget *sous2 = 0;
    QLineEdit *champ_actioni = 0;

    if (groupe == 0){
        erreur_log("Groupe is NULL");
        return;
    }

    sous1 = groupe->findChild<QWidget *>("sous1");
    sous2 = groupe->findChild<QWidget *>("sous2");

    if (sous1 == 0 || sous2 == 0){
        erreur_log("sous? is NULL");
        return;
    }

    champ_actioni = groupe->findChild<QLineEdit *>("champ_action");

    if (champ_actioni == 0){
        erreur_log("????_action is NULL");
        return;
    }

    champ_actioni->setText("destroy sat " + liste_sat[a].nom);

}

void Fenetre::construction_fenetre_menu_jeu(){
    if (fenetre_menu_jeu!=0){
        delete fenetre_menu_jeu;
    }

    fenetre_menu_jeu = new QGroupBox("Menu", this);
    QVBoxLayout* layout_fenetre_menu_jeu= new QVBoxLayout(fenetre_menu_jeu);
    fenetre_menu_jeu->setObjectName("s2");

    QPushButton* bouton_menu_jeu_retour = new QPushButton(fenetre_menu_jeu);
    layout_fenetre_menu_jeu->addWidget(bouton_menu_jeu_retour);
    bouton_menu_jeu_retour->setObjectName("s1");
    bouton_menu_jeu_retour->setFixedHeight(30);
    bouton_menu_jeu_retour->setFixedWidth(150);
    layout_fenetre_menu_jeu->setAlignment(bouton_menu_jeu_retour, Qt::AlignHCenter);

    if (mode == 1){
        bouton_menu_jeu_retour->setText("Back to game");
    }
    else {
        bouton_menu_jeu_retour->setText("Back to editor");
    }

    if (mode == 1){
        QPushButton* bouton_menu_jeu_save = new QPushButton("Save game", fenetre_menu_jeu);
        bouton_menu_jeu_save->setObjectName("s1");
        bouton_menu_jeu_save->setFixedHeight(30);
        bouton_menu_jeu_save->setFixedWidth(150);
        layout_fenetre_menu_jeu->addWidget(bouton_menu_jeu_save);
        layout_fenetre_menu_jeu->setAlignment(bouton_menu_jeu_save, Qt::AlignHCenter);
        QObject::connect(bouton_menu_jeu_save, SIGNAL(clicked()), this, SLOT(chg_bouton_menu_jeu_save()));
    }

    QPushButton* bouton_menu_jeu_option = new QPushButton("Options", fenetre_menu_jeu);
    bouton_menu_jeu_option->setObjectName("s1");
    bouton_menu_jeu_option->setFixedHeight(30);
    bouton_menu_jeu_option->setFixedWidth(150);
    layout_fenetre_menu_jeu->addWidget(bouton_menu_jeu_option);
    layout_fenetre_menu_jeu->setAlignment(bouton_menu_jeu_option, Qt::AlignHCenter);

    QPushButton* bouton_menu_jeu_menu = new QPushButton("Exit to main menu", fenetre_menu_jeu);
    bouton_menu_jeu_menu->setObjectName("s1");
    bouton_menu_jeu_menu->setFixedHeight(30);
    bouton_menu_jeu_menu->setFixedWidth(150);
    layout_fenetre_menu_jeu->addWidget(bouton_menu_jeu_menu);
    layout_fenetre_menu_jeu->setAlignment(bouton_menu_jeu_menu, Qt::AlignHCenter);

    QPushButton* bouton_menu_jeu_exit = new QPushButton("Exit to Windows", fenetre_menu_jeu);
    bouton_menu_jeu_exit->setObjectName("s1");
    bouton_menu_jeu_exit->setFixedHeight(30);
    bouton_menu_jeu_exit->setFixedWidth(150);
    layout_fenetre_menu_jeu->addWidget(bouton_menu_jeu_exit);
    layout_fenetre_menu_jeu->setAlignment(bouton_menu_jeu_exit, Qt::AlignHCenter);

    fenetre_menu_jeu->setLayout(layout_fenetre_menu_jeu);

    fenetre_menu_jeu->setVisible(true);
    fenetre_menu_jeu->setFixedHeight(300);
    fenetre_menu_jeu->setFixedWidth(200);

    fenetre_menu_jeu->move(LARGEUR_FENETRE/2-fenetre_menu_jeu->width()/2, HAUTEUR_FENETRE/2-fenetre_menu_jeu->height()/2);

    QObject::connect(bouton_menu_jeu_retour, SIGNAL(clicked()), this, SLOT(chg_bouton_menu_jeu_retour()));
    QObject::connect(bouton_menu_jeu_option, SIGNAL(clicked()), this, SLOT(chg_bouton_menu_jeu_option()));
    QObject::connect(bouton_menu_jeu_menu, SIGNAL(clicked()), this, SLOT(chg_bouton_menu_jeu_menu()));
    QObject::connect(bouton_menu_jeu_exit, SIGNAL(clicked()), this, SLOT(chg_bouton_menu_jeu_exit()));
}

void Fenetre::chg_bouton_menu_jeu_retour(){
    fenetre_menu_jeu->setVisible(false);
    if (mode == 1){

        bouton_pause->setEnabled(true);
        bouton_sat_jeu->setEnabled(true);
        bouton_log->setEnabled(true);
    }
    else {
        bouton_script->setEnabled(true);
        bouton_pays->setEnabled(true);
        bouton_map->setEnabled(true);
        bouton_ia->setEnabled(true);
        bouton_sat->setEnabled(true);
        bouton_sauvegarde->setEnabled(true);
        bouton_sauvegarde_sous->setEnabled(true);
    }

}

void Fenetre::chg_bouton_menu_jeu_save(){

}

void Fenetre::chg_bouton_menu_jeu_option(){
    fenetre_menu_jeu->setVisible(false);

    if (mode == 1){
        bouton_pause->setVisible(false);
        bouton_sat_jeu->setVisible(false);
        bouton_log->setVisible(false);
        barre_bouton->setVisible(false);
    }
    else {
        bouton_script->setVisible(false);
        bouton_pays->setVisible(false);
        bouton_map->setVisible(false);
        bouton_ia->setVisible(false);
        bouton_sat->setVisible(false);
        bouton_sauvegarde->setVisible(false);
        bouton_sauvegarde_sous->setVisible(false);
    }

    acces_option = mode;

    lancer_option();
}


void Fenetre::chg_bouton_menu_jeu_menu(){
    fenetre_menu_jeu->setVisible(false);

    if (mode == 1){
        bouton_pause->setVisible(false);
        bouton_sat_jeu->setVisible(false);
        bouton_log->setVisible(false);
    }
    else {
        bouton_script->setVisible(false);
        bouton_pays->setVisible(false);
        bouton_map->setVisible(false);
        bouton_ia->setVisible(false);
        bouton_sat->setVisible(false);
        bouton_sauvegarde->setVisible(false);
        bouton_sauvegarde_sous->setVisible(false);
    }
    initialisation();
    lancer_menu();
}

void Fenetre::chg_bouton_menu_jeu_exit(){
    exit(0);
}

void Fenetre::chg_bouton_log(){
    pause = true;
    onglet_selection->setVisible(false);
    if (fenetre_log_jeu == 0){
        construction_fenetre_log();
    }
    else {
        if (!fenetre_log_jeu->isVisible()){
            construction_fenetre_log();
        }
        else {
            fenetre_log_jeu->setVisible(false);
        }
    }
}

void Fenetre::construction_fenetre_log(){
    if (fenetre_log_jeu!=0){
        delete fenetre_log_jeu;
    }

    fenetre_log_jeu = new QGroupBox("Log", this);
    QHBoxLayout* layout_fenetre_log= new QHBoxLayout(fenetre_log_jeu);
    fenetre_log_jeu->setObjectName("s2");
    QScrollArea *scroll_log = new QScrollArea(fenetre_log_jeu);
    scroll_log->setObjectName("s3");
    //scroll_log->verticalScrollBar()->setStyleSheet(" QScrollBar:vertical {border: 2px solid grey;background: #32CC99;width: 15px;margin: 22px 0 22px 0;} QScrollBar::handle:vertical {background: white;min-height: 20px; }QScrollBar::add-line:vertical { border: 2px solid grey;background: #32CC99;height: 20px;subcontrol-position: bottom;subcontrol-origin: margin;}QScrollBar::sub-line:vertical {border: 2px solid grey;background: #32CC99;height: 20px;subcontrol-position: top;subcontrol-origin: margin;}QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {border: 2px solid grey;width: 3px;height: 3px;background: white;}QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
    QVBoxLayout* layout_scroll_log = new QVBoxLayout(scroll_log);
    QWidget *fenetre_log_interieur = new QWidget(scroll_log);
    QVBoxLayout* layout_fenetre_log_interieur = new QVBoxLayout(fenetre_log_interieur);
    layout_fenetre_log_interieur->setSpacing(20);

    for (int i = 0; i < liste_log.size(); i++){
        QLabel* label = new QLabel(liste_log[i], fenetre_log_interieur);

        layout_fenetre_log_interieur->addWidget(label);
        layout_fenetre_log_interieur->setAlignment(label, Qt::AlignHCenter);
    }
    fenetre_log_interieur->setLayout(layout_fenetre_log_interieur);

    layout_scroll_log->addWidget(fenetre_log_interieur);
    scroll_log->setWidget(fenetre_log_interieur);
    scroll_log->setWidgetResizable(true);
    scroll_log->setLayout(layout_scroll_log);

    layout_fenetre_log->addWidget(scroll_log, Qt::AlignCenter);
    fenetre_log_jeu->setLayout(layout_fenetre_log);


    fenetre_log_jeu->setVisible(true);
    fenetre_log_jeu->setFixedHeight(600);
    fenetre_log_jeu->setFixedWidth(400);

    fenetre_log_jeu->move(LARGEUR_FENETRE/2-fenetre_log_jeu->width()/2, HAUTEUR_FENETRE/2-fenetre_log_jeu->height()/2);
}


void Fenetre::initialisation(){
    for (int i = 0; i < largeur_map; i++){
        delete map[i];
    }

    for (int i = 0; i < largeur_map/TAILLE_MAP2; i++){
        delete map2[i];
        delete map_altitude[i];
        delete map_transition[i];
    }

    delete map;
    delete map2;
    delete map_altitude;
    delete map_transition;

    for (unsigned int i = 0; i < liste_ligne.size(); i++){
        scene->removeItem(liste_ligne[i]);
        delete liste_ligne[i];
    }
    liste_ligne.clear();

    for (unsigned int i = 0; i < liste_info_texte.size(); i++){
        scene->removeItem(liste_info_texte[i].texte);
        delete liste_info_texte[i].texte;
    }
    liste_info_texte.clear();

    for (unsigned int i = 0; i < liste_unite.size(); i++){
        delete liste_unite[i].peut_lancer;
        delete liste_unite[i].vie_module;
        delete liste_unite[i].nombre_objet;
        delete liste_unite[i].pixmap;
        delete liste_unite[i].pixmap_destination;
        delete liste_unite[i].widg_selection;

        liste_unite[i].liste_temps_recharge.clear();
        liste_unite[i].liste_deplacement.clear();
        liste_unite[i].liste_ancienne_vie_module.clear();
        liste_unite[i].liste_id_element_ia.clear();

        for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
            scene->removeItem(liste_unite[i].liste_graphical_item[j].item);
            delete liste_unite[i].liste_graphical_item[j].item;
        }

        liste_unite[i].liste_graphical_item.clear();
    }
    liste_unite.clear();

    for (int i = 0; i < liste_unite_attente.size(); i++){
        delete liste_unite_attente[i].nb_objet;
    }
    liste_unite_attente.clear();

    liste_script.clear();
    liste_sat.clear();
    liste_projectile.clear();

    liste_selection.clear();

    if (barre_bouton != 0){
        delete barre_bouton;
        barre_bouton = 0;
    }

    if (barre_principale_jeu != 0){
        delete barre_principale_jeu;
        barre_principale_jeu = 0;
    }

    for (int i = 0; i < 10; i++){
        liste_groupe[i].clear();
    }

    for (int i = 0; i < NOMBRE_ZOOM; i++){
        foreach (tile* value, image_map[i]){
            delete value->image;
            delete value->pixmap;
            delete value;
        }
        image_map[i].clear();
    }

    foreach (QGraphicsPixmapItem* value, image_croix){
        delete value;
    }
    image_croix.clear();

    delete image_minimap;
    delete pixmap_minimap;
    delete rectangle_minimap;
}

double Fenetre::place_restante(int id, int hangar){
    int j=index_unite(id);
    if (j==-1) {
        return 0;
    }
    double place=liste_classe[liste_unite[j].type].hangar[hangar];

    for (unsigned int i=0; i<NOMBRE_OBJET; i++){
        bool ok = false;
        if (liste_objet[i].type == 0 && hangar == 0){     // missile dans missile
            ok = true;
        }
        else if (liste_objet[i].type == 1 && hangar == 1){ // avions dans avion
            ok = true;
        }
        else if (liste_objet[i].type == 2 && hangar == 0){ // projectile dans missile
            ok = true;
        }
        else if (liste_objet[i].type == 3 && hangar == 3){ // amphi dans amphi
            ok = true;
        }
        else if (liste_objet[i].type == 4 && hangar == 3){ // vehicle dans amphi
            ok = true;
        }
        else if (liste_objet[i].type == 5 && hangar == 3){ // inf dans amphi
            ok = true;
        }
        else if (liste_objet[i].type == 6 && hangar == 3){ // bato dans amphi
            ok = true;
        }
        else if (liste_objet[i].type == 7 && hangar == 2){ // helo dans helo
            ok = true;
        }
        else if (liste_objet[i].type == 8 && hangar == 0){ // boue dans missile
            ok = true;
        }

        if (ok){
            place-=liste_unite[j].nombre_objet[i]*liste_objet[i].taille;
        }
    }
    return place;
}

QString Fenetre::time_to_string(int a){
    QString ret;
    if (a >= 86400){
        ret+=QString::number(a/86400) + "d";
        a = a%86400;
    }

    if (a >= 3600){
        ret+=QString::number(a/3600) + "h";
        a = a%3600;
    }

    if (a >= 60){
        ret+=QString::number(a/60) + "m";
        a = a%60;
    }

    if (a > 0){
        ret+=QString::number(a) + "s";
    }

    return ret;
}

void Fenetre::devin(const QString & text){

    int index=onglet_selection->currentIndex();

    int j = index_unite(liste_selection[index]);

    if (j < 0 || j >= liste_unite.size()){
        erreur_log("Sending order to a non existing unit");
        return;
    }

    QGroupBox *groupe = liste_unite[j].widg_selection->findChild<QGroupBox *>("groupe");
    QWidget *sous1 = 0;
    QWidget *sous2 = 0;
    QLineEdit *champ_actioni = 0;
    QLabel* label_actioni = 0;

    if (groupe == 0){
        erreur_log("Groupe is NULL");
        return;
    }

    sous1 = groupe->findChild<QWidget *>("sous1");
    sous2 = groupe->findChild<QWidget *>("sous2");

    if (sous1 == 0 || sous2 == 0){
        erreur_log("sous? is NULL");
        return;
    }

    champ_actioni = groupe->findChild<QLineEdit *>("champ_action");
    label_actioni = sous2->findChild<QLabel *>("label_action");

    if (champ_actioni == 0 || label_actioni == 0){
        erreur_log("????_action is NULL");
        return;
    }

    QString final;

    int nb = 0;

    QVector<QString> liste1;

    for (int i = 0; i < liste_commande.size(); i++){
        if (liste_commande[i].startsWith(text,Qt::CaseInsensitive)){
            nb++;
            final = liste_commande[i];
            liste1.push_back(liste_commande[i].repeated(1));
        }
    }

    if (nb > 1){
        if (option.completion_commande == 2){
            QString lcp = longest_common_prefix(liste1);
            champ_actioni->setText(lcp);
            champ_actioni->setSelection(text.length(), lcp.length() - text.length() + 1);
        }
    }
    else if (nb == 0){
        if (text.simplified().split(' ').size() == 2 && text.simplified().split(' ')[0] == "fire" && text.simplified().split(' ')[1].toInt() == 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[1];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (liste_objet[i].type == 0 || liste_objet[i].type == 2) && (!option.completion_objet_smart || (liste_unite[j].peut_lancer[i] && liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("fire " + longest_common_prefix(liste2));
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("fire " + longest_common_prefix(liste2));
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We can't fire any missile with that name Sir !");
                }
                else {
                    label_actioni->setText("No missiles with that name exist Sir !");
                }

            }
        }
        if (text.simplified().split(' ').size() == 2 && text.simplified().split(' ')[0] == "drop" && text.simplified().split(' ')[1].toInt() == 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[1];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (liste_objet[i].type == 2) && (!option.completion_objet_smart || (liste_unite[j].peut_lancer[i] && liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("drop " + longest_common_prefix(liste2));
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("drop " + longest_common_prefix(liste2));
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We can't drop any bomb with that name Sir !");
                }
                else {
                    label_actioni->setText("No bombs with that name exist Sir !");
                }

            }
        }
        else if (text.simplified().split(' ').size() == 3 && text.simplified().split(' ')[0] == "fire" && text.simplified().split(' ')[1].toInt() > 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[2];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (liste_objet[i].type == 0 || liste_objet[i].type == 2) && (!option.completion_objet_smart || (liste_unite[j].peut_lancer[i] && liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("fire " + text.simplified().split(' ')[1] + " " + longest_common_prefix(liste2));
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("fire " + text.simplified().split(' ')[1] + " " + longest_common_prefix(liste2));
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We can't fire any missile with that name Sir !");
                }
                else {
                    label_actioni->setText("No missiles with that name exist Sir !");
                }
            }
        }
        else if (text.simplified().split(' ').size() == 2 && text.simplified().split(' ')[0] == "launch" && text.simplified().split(' ')[1].toInt() == 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[1];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (liste_objet[i].type != 0 && liste_objet[i].type != 2) && (!option.completion_objet_smart || (liste_unite[j].peut_lancer[i] && liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("launch " + longest_common_prefix(liste2));
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("launch " + longest_common_prefix(liste2));
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We can't launch any vehicles with that name Sir !");
                }
                else {
                    label_actioni->setText("No vehicles with that name exist Sir !");
                }
            }
        }
        else if (text.simplified().split(' ').size() == 3 && text.simplified().split(' ')[0] == "launch" && text.simplified().split(' ')[1].toInt() > 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[2];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (liste_objet[i].type != 0 && liste_objet[i].type != 2) && (!option.completion_objet_smart || (liste_unite[j].peut_lancer[i] && liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("launch " + text.simplified().split(' ')[1] + " " + longest_common_prefix(liste2));
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("launch " + text.simplified().split(' ')[1] + " " + longest_common_prefix(liste2));
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We can't launch any vehicles with that name Sir !");
                }
                else {
                    label_actioni->setText("No vehicles with that name exist Sir !");
                }
            }
        }
        else if (text.simplified().split(' ').size() == 4 && text.simplified().split(' ')[0] == "launch" && text.simplified().split(' ')[1].toInt() > 0 && text.simplified().split(' ')[3].toInt() == 0 && option.completion_commande != 0){
            label_actioni->setText("I'm waiting for your order Admiral !");
            champ_actioni->setText("launch " + text.simplified().split(' ')[1] + " " + text.simplified().split(' ')[2] + " with ");
        }
        else if (text.simplified().split(' ').size() == 5 && text.simplified().split(' ')[0] == "launch" && text.simplified().split(' ')[1].toInt() > 0 &&  text.simplified().split(' ')[4].toInt() == 0 && option.completion_objet != 0){
            int nb2 = 0;
            bool no_default = false;
            QString debut = text.simplified().split(' ')[4];
            QVector<QString> liste2;
            QString def = "default";

            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom == text.simplified().split(' ')[2]){
                    if (liste_objet[i].liste_config.size() > 0 && def.startsWith(debut,Qt::CaseInsensitive)){
                        liste2.push_back("default");
                        nb2 ++;
                    }
                    else if (liste_objet[i].liste_config.size() == 0 && def.startsWith(debut,Qt::CaseInsensitive)){
                        no_default = true;
                    }

                    for (int j = 0; j < liste_objet[i].liste_config.size(); j++){
                        if (liste_objet[i].liste_config[j].nom.startsWith(debut,Qt::CaseInsensitive)){
                            liste2.push_back(liste_objet[i].liste_config[j].nom.repeated(1)); // pas sur si utile
                            nb2 ++;
                        }
                    }
                }
            }


            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                champ_actioni->setText("launch " + text.simplified().split(' ')[1] + " " + text.simplified().split(' ')[2] + " with " + longest_common_prefix(liste2) + " config");
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 7);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    champ_actioni->setText("launch " + text.simplified().split(' ')[1] + " " + text.simplified().split(' ')[2] + " with " + longest_common_prefix(liste2) + " config");
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 7);
                }
            }
            else if (nb2 == 0){
                if (no_default){
                    label_actioni->setText("No default config exist with that vehicle Sir !");
                }
                else {
                    label_actioni->setText("No config with that name exist with that vehicle Sir !");
                }

            }
        }
        else if (text.simplified().split(' ').size() >= 6 && text.simplified().split(' ').size()%2==0 && text.simplified().split(' ')[0] == "launch" && text.simplified().split(' ')[1].toInt() > 0 && text.simplified().split(' ')[4].toInt() > 0 && option.completion_objet != 0){
            int nb2 = 0;
            QString nom = "";
            QString debut = text.simplified().split(' ')[text.simplified().split(' ').size()-1];
            QVector<QString> liste2;
            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].nom.startsWith(debut,Qt::CaseInsensitive) && (!option.completion_objet_smart || (liste_unite[j].nombre_objet[i] > 0))){
                    nom = liste_objet[i].nom;
                    nb2++;
                    liste2.push_back(liste_objet[i].nom.repeated(1)); // pas sur si utile
                }
            }

            if (nb2 == 1){
                label_actioni->setText("I'm waiting for your order Admiral !");
                QString fin = "";
                for (int i = 0; i < text.simplified().split(' ').size()-1; i++){
                    fin += text.simplified().split(' ')[i] + " ";
                }
                fin+=longest_common_prefix(liste2);
                champ_actioni->setText(fin);
                champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
            }
            else if (nb2 > 1){
                if (option.completion_objet == 2){
                    label_actioni->setText("I'm waiting for your order Admiral !");
                    QString fin = "";
                    for (int i = 0; i < text.simplified().split(' ').size()-1; i++){
                        fin += text.simplified().split(' ')[i] + " ";
                    }
                    fin+=longest_common_prefix(liste2);
                    champ_actioni->setText(fin);
                    champ_actioni->setSelection(text.length(), longest_common_prefix(liste2).length() + 1);
                }
            }
            else if (nb2 == 0){
                if (option.completion_objet_smart){
                    label_actioni->setText("We don't have any vehicles or ammo with that name Sir !");
                }
                else {
                    label_actioni->setText("No vehicles or ammo with that name exist Sir !");
                }

            }
        }
    }
    else if (nb == 1){
        if (option.completion_commande != 0){
            label_actioni->setText("I'm waiting for your order Admiral !");
            champ_actioni->setText(final + " ");
            champ_actioni->setSelection(text.length(), final.length() - text.length() + 1);
        }
    }
}
QString Fenetre::longest_common_prefix(QVector<QString> liste){
    bool word_set = false;
    int len = 0;
    bool over = false;
    bool need_space = false;
    bool last_need_space = false;
    QString word = "";
    QString lastWord = "";

    do {
        word_set = false;
        need_space = true;

        for (int i = 0; i < liste.size() && !over; i++){
            if (liste[i].size() < len){
                over = true;
            }
            else {
                if (!word_set){
                    word_set = true;
                    word = liste[i].left(len);
                }
                else if (!liste[i].startsWith(word,Qt::CaseInsensitive)){
                    over = true;
                }
            }

            if (liste[i].size() > len && liste[i].at(len) != ' '){
                need_space = false;
                //qDebug() << len << " : " << liste_commande[i].at(len) << " in " << liste_commande[i];
            }
        }

        if (over){
            if (last_need_space){
                return lastWord + " ";
            }
            else {
                return lastWord;
            }

        }
        else {
            len++;
            lastWord = word;
            last_need_space = need_space;
        }
    } while (!over);

    return lastWord;
}

void Fenetre::check_script()
{

    int nb;
    for (int i=0; i<liste_script.size(); i++)
    {
        if (liste_script[i].actif)
        {
            nb=0;
            for (int j=0; j<liste_script[i].liste_cause.size(); j++)
            {
                if (!liste_script[i].liste_cause[j].valide)
                {
                    if (liste_script[i].liste_cause[j].type==0)  // temps
                    {
                        if (liste_script[i].liste_cause[j].temps<=temps)
                        {
                            liste_script[i].liste_cause[j].valide=1;
                        }
                    }


                }
                if (!liste_script[i].liste_cause[j].valide)
                {
                    nb++;
                }
            }

            if (nb==0)
            {
                liste_script[i].actif=0;
                for (int j=0; j<liste_script[i].liste_consequence.size(); j++)
                {
                    if (liste_script[i].liste_consequence[j].type==0)  // victoire
                    {
                        ajout_pop_up("VICTORY");
                    }
                    if (liste_script[i].liste_consequence[j].type==1)  // defeat
                    {
                        ajout_pop_up("DEFEAT");
                    }
                }
            }
        }
    }
}

void Fenetre::ajout_pop_up(QString text)
{
    groupe_pop_up = new QTabWidget(this);
    QWidget *groupe = new QWidget(groupe_pop_up);
    QVBoxLayout *layout = new QVBoxLayout;
    QVBoxLayout *layout2 = new QVBoxLayout;


    QLabel *lab = new QLabel(text, groupe);
    QPushButton *but = new QPushButton("Ok", groupe);
    but->setFixedHeight(20);
    but->setFixedWidth(80);

    layout2->addWidget(lab,Qt::AlignHCenter);
    layout2->addWidget(but,Qt::AlignHCenter);
    layout2->setAlignment(lab,Qt::AlignHCenter);
    layout2->setAlignment(but,Qt::AlignHCenter);

    groupe->setLayout(layout2);

    layout->addWidget(groupe,Qt::AlignHCenter);

    groupe_pop_up->setLayout(layout);

    groupe_pop_up->setFixedHeight(150);
    groupe_pop_up->setFixedWidth(lab->width()+80);

    groupe_pop_up->setVisible(true);

    groupe_pop_up->move(1280/2-groupe_pop_up->width()/2, 720/2-groupe_pop_up->height()/2);

    QObject::connect(but, SIGNAL(clicked()), this, SLOT(destruction_pop_up()));
}

void Fenetre::destruction_pop_up()
{
    delete groupe_pop_up;
}
