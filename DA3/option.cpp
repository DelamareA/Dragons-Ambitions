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

void Fenetre::lancer_option()
{
    pixmap_menu->setVisible(acces_option == 0);
    bouton_jeu->setVisible(false);
    bouton_editeur->setVisible(false);
    bouton_generateur->setVisible(false);
    bouton_option->setVisible(false);
    bouton_quit->setVisible(false);

    pixmap_menu->setZValue(100);


    mode=4;

    //chargement_option();

    if (fenetre_menu_option != 0){
        delete fenetre_menu_option;
    }

    fenetre_menu_option = new QWidget(this);
    fenetre_menu_option->setObjectName("merde");
    QVBoxLayout *layout = new QVBoxLayout;

    QGroupBox *groupe_option_hud = new QGroupBox("Displayed informations", fenetre_menu_option);
    QVBoxLayout *layout_option_hud = new QVBoxLayout;

    QCheckBox *checkbox_option_fuel = new QCheckBox("Display fuel left", fenetre_menu_option);
    QCheckBox *checkbox_option_weapon = new QCheckBox("Display weaponry", fenetre_menu_option);
    QCheckBox *checkbox_option_vitesse = new QCheckBox("Display current speed", fenetre_menu_option);

    if (option.affichage_fuel)
    {
        checkbox_option_fuel->setChecked(true);
    }
    if (option.affichage_arme)
    {
        checkbox_option_weapon->setChecked(true);
    }
    if (option.affichage_vitesse)
    {
        checkbox_option_vitesse->setChecked(true);
    }

    QCheckBox *checkbox_option_groupe = new QCheckBox("Display groups buttons", fenetre_menu_option);
    checkbox_option_groupe->setChecked(option.affichage_groupe);

    QWidget *widg_affichage_stack = new QWidget(fenetre_menu_option);
    QHBoxLayout *layout_affichage_stack = new QHBoxLayout;

    QLabel * label_affichage_stack = new QLabel("Stacked units display : ", widg_affichage_stack);
    QComboBox *combo_affichage_stack = new QComboBox(widg_affichage_stack);

    combo_affichage_stack->addItem("No special display");
    combo_affichage_stack->addItem("Group units when close");
    combo_affichage_stack->addItem("Make smaller icons when units are close");
    combo_affichage_stack->setCurrentIndex(option.affichage_stack);

    layout_affichage_stack->addWidget(label_affichage_stack);
    layout_affichage_stack->addWidget(combo_affichage_stack);
    widg_affichage_stack->setLayout(layout_affichage_stack);

    QLabel * label_affichage_warn = new QLabel("The following options need a restart to be applied : ", fenetre_menu_option);

    QRect rec = QApplication::desktop()->screenGeometry();
    int h = rec.height();
    int w = rec.width();

    QWidget *widg_affichage_res = new QWidget(fenetre_menu_option);
    QHBoxLayout *layout_affichage_res = new QHBoxLayout;

    QLabel * label_affichage_res = new QLabel("Resolution : ", widg_affichage_res);
    QComboBox *combo_affichage_res = new QComboBox(widg_affichage_res);

    combo_affichage_res->addItem("1280 x 720");
    if (h >= 768 && w >= 1366){
        combo_affichage_res->addItem("1366 x 768");
    }
    if (h >= 900 && w >= 1600){
        combo_affichage_res->addItem("1600 x 900");
    }
    if (h >= 1080 && w >= 1920){
        combo_affichage_res->addItem("1920 x 1080");
    }

    combo_affichage_res->setCurrentIndex(option.nb_res);

    layout_affichage_res->addWidget(label_affichage_res);
    layout_affichage_res->addWidget(combo_affichage_res);
    widg_affichage_res->setLayout(layout_affichage_res);

    QCheckBox *checkbox_option_fullscreen = new QCheckBox("Fullscreen", fenetre_menu_option);
    checkbox_option_fullscreen->setChecked(option.fullscreen);
    //checkbox_option_fullscreen->setCheckable(h <= 1080 && w <=1920);

    QPushButton* bouton_retour = new QPushButton(fenetre_menu_option);

    if (acces_option == 0){
        bouton_retour->setText("Back to main menu");
        bouton_retour->setFixedWidth(150);
    }
    else if (acces_option == 1){
        bouton_retour->setText("Back to game");
        bouton_retour->setFixedWidth(150);
    }
    else if (acces_option == 2){
        bouton_retour->setText("Back to editor");
        bouton_retour->setFixedWidth(150);
    }

    layout_option_hud->addWidget(checkbox_option_fuel);
    layout_option_hud->addWidget(checkbox_option_vitesse);
    layout_option_hud->addWidget(checkbox_option_weapon);
    layout_option_hud->addWidget(checkbox_option_groupe);
    layout_option_hud->addWidget(widg_affichage_stack);
    layout_option_hud->addWidget(label_affichage_warn);
    layout_option_hud->addWidget(widg_affichage_res);
    layout_option_hud->addWidget(checkbox_option_fullscreen);



    groupe_option_hud->setLayout(layout_option_hud);



    QGroupBox *groupe_option_completion = new QGroupBox("Autocomplete", fenetre_menu_option);
    QVBoxLayout *layout_option_completion = new QVBoxLayout;

    QWidget *widg_completion_commande = new QWidget(fenetre_menu_option);
    QHBoxLayout *layout_completion_commande = new QHBoxLayout;

    QLabel * label_completion_commande = new QLabel("Commands autocomplete : ", widg_completion_commande);
    QComboBox *combo_completion_commande = new QComboBox(widg_completion_commande);

    combo_completion_commande->addItem("No autocomplete");
    combo_completion_commande->addItem("Only autocomplete when one command is possible");
    combo_completion_commande->addItem("Full autocomplete");
    combo_completion_commande->setCurrentIndex(option.completion_commande);

    layout_completion_commande->addWidget(label_completion_commande);
    layout_completion_commande->addWidget(combo_completion_commande);
    widg_completion_commande->setLayout(layout_completion_commande);

    groupe_option_completion->setLayout(layout_option_completion);

    QWidget *widg_completion_objet = new QWidget(fenetre_menu_option);
    QHBoxLayout *layout_completion_objet = new QHBoxLayout;

    QLabel * label_completion_objet = new QLabel("Objects autocomplete : ", widg_completion_objet);
    QComboBox *combo_completion_objet = new QComboBox(widg_completion_objet);

    combo_completion_objet->addItem("No autocomplete");
    combo_completion_objet->addItem("Only autocomplete when one object is possible");
    combo_completion_objet->addItem("Full autocomplete");
    combo_completion_objet->setCurrentIndex(option.completion_objet);

    layout_completion_objet->addWidget(label_completion_objet);
    layout_completion_objet->addWidget(combo_completion_objet);
    widg_completion_objet->setLayout(layout_completion_objet);

    QCheckBox *checkbox_completion_objet_smart = new QCheckBox("Only autocomplete objects which can be fired from unit", fenetre_menu_option);
    checkbox_completion_objet_smart->setChecked(option.completion_objet_smart);


    layout_option_completion->addWidget(widg_completion_commande);
    layout_option_completion->addWidget(widg_completion_objet);
    layout_option_completion->addWidget(checkbox_completion_objet_smart);

    groupe_option_completion->setLayout(layout_option_completion);

    layout->addWidget(groupe_option_hud);
    layout->addWidget(groupe_option_completion);
    layout->addWidget(bouton_retour);
    layout->setAlignment(bouton_retour, Qt::AlignHCenter);

    fenetre_menu_option->setLayout(layout);

    fenetre_menu_option->setVisible(true);
    fenetre_menu_option->setFixedHeight(500);
    fenetre_menu_option->setFixedWidth(500);

    fenetre_menu_option->move(LARGEUR_FENETRE/2-fenetre_menu_option->width()/2, HAUTEUR_FENETRE/2-fenetre_menu_option->height()/2);

    QObject::connect(checkbox_option_fuel, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_option_fuel(int)));
    QObject::connect(checkbox_option_weapon, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_option_arme(int)));
    QObject::connect(checkbox_option_vitesse, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_option_vitesse(int)));
    QObject::connect(checkbox_option_groupe, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_option_groupe(int)));
    QObject::connect(combo_affichage_stack, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_affichage_stack(const QString &)));
    QObject::connect(checkbox_option_fullscreen, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_option_fullscreen(int)));
    QObject::connect(combo_affichage_res, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_affichage_res(const QString &)));
    QObject::connect(combo_completion_commande, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_completion_commande(const QString &)));
    QObject::connect(combo_completion_objet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_completion_objet(const QString &)));
    QObject::connect(checkbox_completion_objet_smart, SIGNAL(stateChanged(int)), this, SLOT(chg_checkbox_completion_objet_smart(int)));
    QObject::connect(bouton_retour, SIGNAL(clicked()), this, SLOT(chg_bouton_retour()));
}

void Fenetre::chg_checkbox_option_fuel(int a)
{
    option.affichage_fuel=a;
    sauvegarde_option();
}

void Fenetre::chg_checkbox_option_arme(int a)
{
    option.affichage_arme=a;
    sauvegarde_option();
}

void Fenetre::chg_checkbox_option_vitesse(int a)
{
    option.affichage_vitesse=a;
    sauvegarde_option();
}

void Fenetre::chg_checkbox_option_groupe(int a){
    option.affichage_groupe=a;
    sauvegarde_option();
}

void Fenetre::chg_combo_affichage_stack(const QString & a){
    if (a == "No special display"){
        option.affichage_stack = 0;
    }
    else if (a == "Group units when close"){
        option.affichage_stack = 1;
    }
    else {
        option.affichage_stack = 2;
    }
    sauvegarde_option();
}

void Fenetre::chg_checkbox_option_fullscreen(int a){
    opt old = option;
    option.fullscreen=a;
    sauvegarde_option();
    option = old;
}

void Fenetre::chg_combo_affichage_res(const QString & a){
    opt old = option;
    if (a == "1280 x 720"){
        option.nb_res = 0;
        option.res_x = 1280;
        option.res_y = 720;
    }
    else if (a == "1366 x 768"){
        option.nb_res = 1;
        option.res_x = 1366;
        option.res_y = 768;
    }
    else if (a == "1600 x 900"){
        option.nb_res = 2;
        option.res_x = 1600;
        option.res_y = 900;
    }
    else if (a == "1920 x 1080"){
        option.nb_res = 3;
        option.res_x = 1920;
        option.res_y = 1080;
    }
    sauvegarde_option();

    option = old;
}

void Fenetre::chg_combo_completion_commande(const QString & a){
    if (a == "No autocomplete"){
        option.completion_commande = 0;
    }
    else if (a == "Only autocomplete when one command is possible"){
        option.completion_commande = 1;
    }
    else {
        option.completion_commande = 2;
    }
    sauvegarde_option();
}

void Fenetre::chg_combo_completion_objet(const QString & a){
    if (a == "No autocomplete"){
        option.completion_objet = 0;
    }
    else if (a == "Only autocomplete when one object is possible"){
        option.completion_objet = 1;
    }
    else {
        option.completion_objet = 2;
    }
    sauvegarde_option();
}

void Fenetre::chg_checkbox_completion_objet_smart(int a){
    option.completion_objet_smart = a;
    sauvegarde_option();
}

void Fenetre::chg_bouton_retour(){
    fenetre_menu_option->setVisible(false);

    if (acces_option == 0){
        lancer_menu();
    }
    else if (acces_option == 1){
        pixmap_menu->setVisible(false);
        bouton_pause->setVisible(true);
        bouton_sat_jeu->setVisible(true);
        bouton_log->setVisible(true);
        fenetre_menu_jeu->setVisible(true);
        mode = 1;
    }
    else if (acces_option == 2){
        bouton_script->setVisible(true);
        bouton_pays->setVisible(true);
        bouton_map->setVisible(true);
        bouton_ia->setVisible(true);
        bouton_sat->setVisible(true);
        bouton_sauvegarde->setVisible(true);
        bouton_sauvegarde_sous->setVisible(true);
        fenetre_menu_jeu->setVisible(true);
        mode = 2;
    }
}

void Fenetre::sauvegarde_option()
{
    ofstream flux("data/option.daopt");

    if (flux)
    {
        flux << option.affichage_fuel << " ";
        flux << option.affichage_arme << " ";
        flux << option.affichage_vitesse << " ";
        flux << option.affichage_stack << " ";
        flux << option.fullscreen << " ";
        flux << option.nb_res << " ";
        flux << option.res_x << " ";
        flux << option.res_y << " ";
        flux << option.completion_objet_smart << " ";
        flux << option.completion_commande << " ";
        flux << option.completion_objet << " ";
        flux << option.affichage_groupe << " ";
    }
}

void Fenetre::chargement_option()
{
    ifstream flux("data/option.daopt");

    if (flux)
    {
        flux >> option.affichage_fuel;
        flux >> option.affichage_arme;
        flux >> option.affichage_vitesse;
        flux >> option.affichage_stack;
        flux >> option.fullscreen;
        flux >> option.nb_res;
        flux >> option.res_x;
        flux >> option.res_y;
        flux >> option.completion_objet_smart;
        flux >> option.completion_commande;
        flux >> option.completion_objet;
        flux >> option.affichage_groupe;
    }
}
