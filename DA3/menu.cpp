#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>

void Fenetre::lancer_menu()
{
    setCursor(Qt::ArrowCursor);

    mode = 0;
    acces_option = 0;

    /*scene->setSceneRect(0, 0, 1280-10, 720-10);
    setFixedWidth(1280);
    setFixedHeight(720);

    showNormal();*/

    if (image_menu == 0){
        image_menu = new QPixmap("ressource/other/DA3art (2).png");
        QPixmap image_menu2 = image_menu->scaled(LARGEUR_FENETRE, HAUTEUR_FENETRE);

        pixmap_menu = new QGraphicsPixmapItem(image_menu2);
        pixmap_menu->setPos(-5,-5);
        scene->addItem(pixmap_menu);

        bouton_jeu = new QPushButton("Campaign", this);
        //bouton_jeu->move(LARGEUR_FENETRE/2 - 2 * bouton_jeu->width(), HAUTEUR_FENETRE - 6 * bouton_jeu->height());
        bouton_jeu->setObjectName("jeu");
        QObject::connect(bouton_jeu, SIGNAL(clicked ()), this, SLOT(lancer_jeu()));

        bouton_editeur = new QPushButton("Editor", this);
        //bouton_editeur->move(LARGEUR_FENETRE/2 + 1 * bouton_editeur->width(), HAUTEUR_FENETRE - 6 * bouton_editeur->height());
        bouton_editeur->setObjectName("jeu");
        QObject::connect(bouton_editeur, SIGNAL(clicked ()), this, SLOT(lancer_editeur()));

        bouton_generateur = new QPushButton("Map Generator", this);
        //bouton_generateur->move(LARGEUR_FENETRE/2 - 2 * bouton_generateur->width(), HAUTEUR_FENETRE - 4 * bouton_generateur->height());
        bouton_generateur->setObjectName("jeu");
        QObject::connect(bouton_generateur, SIGNAL(clicked ()), this, SLOT(lancer_generateur()));

        bouton_option = new QPushButton("Options", this);
        //bouton_option->move(LARGEUR_FENETRE/2 + 1 * bouton_option->width(), HAUTEUR_FENETRE - 4 * bouton_option->height());
        bouton_option->setObjectName("jeu");
        QObject::connect(bouton_option, SIGNAL(clicked ()), this, SLOT(lancer_option()));

        bouton_quit = new QPushButton("Exit", this);
        //bouton_quit->move(LARGEUR_FENETRE/2 - bouton_quit->width()/2, HAUTEUR_FENETRE - 2 * bouton_quit->height());
        bouton_quit->setObjectName("jeu");
        QObject::connect(bouton_quit, SIGNAL(clicked ()), this, SLOT(chg_bouton_menu_jeu_exit()));
    }

    pixmap_menu->setVisible(true);
    bouton_jeu->setVisible(true);
    bouton_editeur->setVisible(true);
    bouton_generateur->setVisible(true);
    bouton_option->setVisible(true);
    bouton_quit->setVisible(true);



    /*bouton_pause = new QPushButton("Kapoue23", this);
    bouton_pause->setVisible(false);
    bouton_pause->setObjectName("s1");
    //bouton_pause->setStyleSheet("QPushButton {background-color: rgb(15, 0, 50); border-width: 2px; border-color: rgb(0, 0, 0); border-style: solid;font: bold 25px; font-family: Rockwell; min-width: 10em; min-height: 1em; padding: 6px;} QPushButton:pressed {  background-color: rgb(0, 30, 100); } QPushButton:hover {  background-color:  rgb(0, 30, 100);}");

    QObject::connect(bouton_pause, SIGNAL(clicked()), this, SLOT(chg_bouton_pause()));*/

}
