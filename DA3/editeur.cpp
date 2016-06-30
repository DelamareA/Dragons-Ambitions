#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>


void Fenetre::lancer_editeur()
{

    setCursor(Qt::WaitCursor);

    mode=2;

    pixmap_menu->setVisible(false);
    bouton_jeu->setVisible(false);
    bouton_editeur->setVisible(false);
    bouton_generateur->setVisible(false);
    bouton_option->setVisible(false);
    bouton_quit->setVisible(false);

    barre_principale = new QWidget(this);
    barre_principale->setVisible(true);

    QHBoxLayout *layout_barre = new QHBoxLayout;

    bouton_script = new QPushButton("Scripts", barre_principale);
    layout_barre->addWidget(bouton_script);

    bouton_pays = new QPushButton("Country", barre_principale);
    layout_barre->addWidget(bouton_pays);

    bouton_map = new QPushButton("Map", barre_principale);
    layout_barre->addWidget(bouton_map);

    bouton_ia = new QPushButton("AI", barre_principale);
    layout_barre->addWidget(bouton_ia);

    bouton_sat = new QPushButton("Satellites", barre_principale);
    layout_barre->addWidget(bouton_sat);

    bouton_sauvegarde = new QPushButton("Save", barre_principale);
    layout_barre->addWidget(bouton_sauvegarde);

    bouton_sauvegarde_sous = new QPushButton("Save as", barre_principale);
    layout_barre->addWidget(bouton_sauvegarde_sous);

    barre_principale->setLayout(layout_barre);
    barre_principale->move(0,0);
    barre_principale->setVisible(true);
    barre_principale->setFixedWidth(LARGEUR_FENETRE);
    barre_principale->setFixedHeight(50);


    QObject::connect(bouton_script, SIGNAL(clicked()), this, SLOT(construction_fenetre_script()));
    QObject::connect(bouton_pays, SIGNAL(clicked()), this, SLOT(construction_fenetre_pays()));
    QObject::connect(bouton_sat, SIGNAL(clicked()), this, SLOT(construction_fenetre_sat()));
    QObject::connect(bouton_map, SIGNAL(clicked()), this, SLOT(construction_fenetre_map()));
    QObject::connect(bouton_ia, SIGNAL(clicked()), this, SLOT(construction_fenetre_ia()));
    QObject::connect(bouton_sauvegarde_sous, SIGNAL(clicked()), this, SLOT(slot_sauvegarde_sous()));

    largeur_map=2000;
    hauteur_map=2000;
    echelle=500;  // nb m pour 1 pixel
    oldzoom = -1;
    zoom=0;
    camera.x=5;
    camera.y=5;

    chargement_map ("Taiwan");
    chargement("kapoue");

    fenetre_unite_cree=0;
    groupe_unite_nombre_objet_cree=0;

    ia_actuelle = USA;

    setCursor(Qt::CrossCursor);
}

void Fenetre::construction_fenetre_unite(int id)
{
    check_module_unite();

    int j=index_unite(id);

    if (j!=-1)
    {
        if (fenetre_unite_cree)
        {
            delete fenetre_unite;
        }

        fenetre_unite_cree=1;

        int pays=liste_classe[liste_unite[j].type].pays;

        fenetre_unite = new QWidget();
        QVBoxLayout *layout_unite = new QVBoxLayout;

        QWidget *groupe_unite_pays = new QWidget(fenetre_unite);   // pays
        QLabel *label_unite_pays = new QLabel("Country : ", groupe_unite_pays);
        QHBoxLayout *layout_unite_pays = new QHBoxLayout;

        combo_unite_pays = new QComboBox(groupe_unite_pays);
        for (unsigned int i=0; i<NOMBRE_PAYS; i++)
        {
            combo_unite_pays->addItem(liste_pays[i].nom);

            if (pays==i)
            {
                combo_unite_pays->setCurrentIndex(i);
            }
        }
        layout_unite_pays->addWidget(label_unite_pays);
        layout_unite_pays->addWidget(combo_unite_pays);
        groupe_unite_pays->setLayout(layout_unite_pays);

        layout_unite->addWidget(groupe_unite_pays);


        QWidget *groupe_unite_type = new QWidget(fenetre_unite);   // classe
        QLabel *label_unite_type = new QLabel("Class : ", groupe_unite_type);
        QHBoxLayout *layout_unite_type = new QHBoxLayout;

        int nb=0;

        combo_unite_type = new QComboBox(groupe_unite_type);
        for (unsigned int i=0; i<NOMBRE_CLASSE; i++)
        {
            if (liste_classe[i].pays==pays)
            {
                combo_unite_type->addItem(liste_classe[i].nom);
                nb++;
            }

            if (liste_unite[j].type==i)
            {
                combo_unite_type->setCurrentIndex(nb-1);
            }
        }
        layout_unite_type->addWidget(label_unite_type);
        layout_unite_type->addWidget(combo_unite_type);
        groupe_unite_type->setLayout(layout_unite_type);

        layout_unite->addWidget(groupe_unite_type);


        QWidget *groupe_unite_nom = new QWidget(fenetre_unite);   // nom
        QLabel *label_unite_nom = new QLabel("Name : ", groupe_unite_nom);
        QHBoxLayout *layout_unite_nom = new QHBoxLayout;

        champ_unite_nom = new QLineEdit(groupe_unite_nom);
        champ_unite_nom->setText(liste_unite[j].nom);
        layout_unite_nom->addWidget(label_unite_nom);
        layout_unite_nom->addWidget(champ_unite_nom);
        groupe_unite_nom->setLayout(layout_unite_nom);

        layout_unite->addWidget(groupe_unite_nom);



        QWidget *groupe_unite_altitude = new QWidget(fenetre_unite);   // altitude
        QLabel *label_unite_altitude = new QLabel("Altitude (m): ", groupe_unite_altitude);
        QHBoxLayout *layout_unite_altitude = new QHBoxLayout;

        champ_unite_altitude = new QSpinBox(groupe_unite_nom);
        champ_unite_altitude->setMinimum(liste_unite[j].altitude_min);
        champ_unite_altitude->setMaximum(liste_unite[j].altitude_max);
        champ_unite_altitude->setValue(liste_unite[j].position.z);
        layout_unite_altitude->addWidget(label_unite_altitude);
        layout_unite_altitude->addWidget(champ_unite_altitude);
        groupe_unite_altitude->setLayout(layout_unite_altitude);

        layout_unite->addWidget(groupe_unite_altitude);


        QWidget *groupe_unite_angle = new QWidget(fenetre_unite);   // angle
        QLabel *label_unite_angle = new QLabel("Angle : ", groupe_unite_angle);
        QHBoxLayout *layout_unite_angle = new QHBoxLayout;

        champ_unite_angle = new QSpinBox(groupe_unite_angle);
        champ_unite_angle->setMinimum(0);
        champ_unite_angle->setMaximum(359);
        champ_unite_angle->setValue(radtodeg(liste_unite[j].angle));
        layout_unite_angle->addWidget(label_unite_angle);
        layout_unite_angle->addWidget(champ_unite_angle);
        groupe_unite_angle->setLayout(layout_unite_angle);

        layout_unite->addWidget(groupe_unite_angle);




        groupe_unite_objet = new QGroupBox("Objects", fenetre_unite);
        layout_unite_objet = new QVBoxLayout;

        QWidget *groupe_unite_objet2 = new QWidget(groupe_unite_objet);   // objet
        QHBoxLayout *layout_unite_objet2 = new QHBoxLayout;
        champ_unite_nombre_objet = new QSpinBox(groupe_unite_objet2);
        combo_unite_objet = new QComboBox(groupe_unite_objet2);

        int index = -1;
        for (unsigned int k=0; k<9; k++){
            for (unsigned int i=0; i<NOMBRE_OBJET; i++){
                if (liste_objet[i].type==k){
                    combo_unite_objet->addItem(liste_objet[i].nom);
                    if (index == -1){
                        index = i;
                    }
                }
            }
        }

        label_unite_objet = new QLabel("Max : " + QString::number((int)(place_restante(liste_unite[j].id, 0) / liste_objet[index].taille)), groupe_unite_objet2); // assume first is always a missile
        bouton_unite_ajout_objet = new QPushButton("Add", groupe_unite_objet2);

        layout_unite_objet2->addWidget(champ_unite_nombre_objet);
        layout_unite_objet2->addWidget(combo_unite_objet);
        layout_unite_objet2->addWidget(label_unite_objet);
        layout_unite_objet2->addWidget(bouton_unite_ajout_objet);
        groupe_unite_objet2->setLayout(layout_unite_objet2);

        layout_unite_objet->addWidget(groupe_unite_objet2);

        bouton_unite_reset_objet = new QPushButton("Reset", groupe_unite_objet);

        layout_unite_objet->addWidget(bouton_unite_reset_objet);
        construction_groupe_objet(liste_unite[j].id);
        layout_unite_objet->addWidget(groupe_unite_nombre_objet);

        groupe_unite_objet->setLayout(layout_unite_objet);



        layout_unite->addWidget(groupe_unite_objet);

        bouton_unite_delete = new QPushButton("Delete unit");  // destruction
        layout_unite->addWidget(bouton_unite_delete);

        fenetre_unite->setLayout(layout_unite);

        fenetre_unite->setVisible(true);
        fenetre_unite->setWindowTitle("Unit management");


        QObject::connect(combo_unite_type, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_unite_type(const QString &)));
        QObject::connect(combo_unite_pays, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_unite_pays(const QString &)));
        QObject::connect(champ_unite_nom, SIGNAL(textChanged(QString)), this, SLOT(chg_champ_unite_nom(QString)));
        QObject::connect(champ_unite_altitude, SIGNAL(valueChanged(int)), this, SLOT(chg_champ_unite_altitude(int)));
        QObject::connect(champ_unite_angle, SIGNAL(valueChanged(int)), this, SLOT(chg_champ_unite_angle(int)));
        QObject::connect(combo_unite_objet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_unite_objet(const QString &)));
        QObject::connect(bouton_unite_ajout_objet, SIGNAL(clicked()), this, SLOT(chg_bouton_unite_ajout_objet()));
        QObject::connect(bouton_unite_reset_objet, SIGNAL(clicked()), this, SLOT(chg_bouton_unite_reset_objet()));
        QObject::connect(bouton_unite_delete, SIGNAL(clicked()), this, SLOT(chg_bouton_unite_delete()));
    }
}

void Fenetre::construction_groupe_objet(int id)
{
    int i=index_unite(id);

    if (i==-1){
        return;
    }

    groupe_unite_nombre_objet = new QWidget(groupe_unite_objet);
    QHBoxLayout *layout_unite_nombre_objet = new QHBoxLayout;

    QGroupBox *groupe_missile = new QGroupBox("Missile/Ammo");
    QGroupBox *groupe_avion = new QGroupBox("Plane");
    QGroupBox *groupe_helo = new QGroupBox("Helo");
    QGroupBox *groupe_well_dock = new QGroupBox("Vehicules/Inf");
    QHBoxLayout *layout_missile = new QHBoxLayout;
    QHBoxLayout *layout_avion = new QHBoxLayout;
    QHBoxLayout *layout_helo = new QHBoxLayout;
    QHBoxLayout *layout_well_dock = new QHBoxLayout;

    for (unsigned int j=0; j<NOMBRE_OBJET; j++)
    {
        if (liste_unite[i].nombre_objet[j]>0)
        {
            QWidget *widg;
            if (liste_objet[j].type==0 || liste_objet[j].type==2 || liste_objet[j].type==8){
                widg = new QWidget(groupe_missile);
            }
            else if (liste_objet[j].type==1){
                widg = new QWidget(groupe_avion);
            }
            else if (liste_objet[j].type==3 || liste_objet[j].type==4 || liste_objet[j].type==5 || liste_objet[j].type==6){
                widg = new QWidget(groupe_well_dock);
            }
            else {
                widg = new QWidget(groupe_helo);
            }

            QVBoxLayout *layoutbla = new QVBoxLayout;

            QLabel *label_nomi = new QLabel(liste_objet[j].nom, widg);
            QLabel *label_nombrei = new QLabel(QString::number(liste_unite[i].nombre_objet[j]), widg);
            QLabel *label_imagei = new QLabel(widg);
            label_imagei->setPixmap(*liste_objet[j].image);

            layoutbla->addWidget(label_nomi);
            layoutbla->addWidget(label_imagei);
            layoutbla->addWidget(label_nombrei);
            widg->setLayout(layoutbla);

            if (liste_objet[j].type==0 || liste_objet[j].type==2){
                layout_missile->addWidget(widg);
            }
            else if (liste_objet[j].type==1){
                layout_avion->addWidget(widg);
            }
            else if (liste_objet[j].type==3 || liste_objet[j].type==4 || liste_objet[j].type==5 || liste_objet[j].type==6){
                layout_well_dock->addWidget(widg);
            }
            else {
                layout_helo->addWidget(widg);
            }
        }
    }

    groupe_missile->setLayout(layout_missile);
    groupe_avion->setLayout(layout_avion);
    groupe_helo->setLayout(layout_helo);
    groupe_well_dock->setLayout(layout_well_dock);

    if (layout_missile->isEmpty()){
        groupe_missile->setVisible(false);
    }
    if (layout_avion->isEmpty()){
        groupe_avion->setVisible(false);
    }
    if (layout_helo->isEmpty()){
        groupe_helo->setVisible(false);
    }
    if (layout_well_dock->isEmpty()){
         groupe_well_dock->setVisible(false);
    }

    layout_unite_nombre_objet->addWidget(groupe_missile);
    layout_unite_nombre_objet->addWidget(groupe_avion);
    layout_unite_nombre_objet->addWidget(groupe_helo);
    layout_unite_nombre_objet->addWidget(groupe_well_dock);

    groupe_unite_nombre_objet->setLayout(layout_unite_nombre_objet);
}

void Fenetre::chg_combo_unite_pays(const QString & text)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }
    }
    liste_unite[j].chg_pixmap = true;

    combo_unite_type->clear();

    int pays=USA;

    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        if (text.compare(liste_pays[i].nom)==0)
        {
            pays=i;
        }
    }

    for (unsigned int i=0; i<NOMBRE_CLASSE; i++)
    {
        if (liste_classe[i].pays==pays)
        {
            combo_unite_type->addItem(liste_classe[i].nom);
        }
    }
}

void Fenetre::chg_combo_unite_type(const QString & text)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }

        for (unsigned int i=0; i<NOMBRE_CLASSE; i++)
        {
            if (text.compare(liste_classe[i].nom)==0){
                liste_unite[j].type=i;
                liste_unite[j].chg_pixmap = true;
                liste_unite[j].chg_module = true;
            }
        }
    }

    for (unsigned int i=0; i<NOMBRE_MODULE; i++){
        if (liste_classe[liste_unite[j].type].module_present[i]){
            liste_unite[j].vie_module[i]=liste_module[i].pv;
        }
        else{
            liste_unite[j].vie_module[i]=0;
        }
    }

    check_module_unite();

    champ_unite_altitude->setMinimum(liste_unite[j].altitude_min);
    champ_unite_altitude->setMaximum(liste_unite[j].altitude_max);

    // mettre min et max altitude
}

void Fenetre::chg_champ_unite_nom(QString s)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }

        liste_unite[j].nom=s;
    }

}


void Fenetre::chg_champ_unite_altitude(int v)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }

        liste_unite[j].position.z=v;
    }

}

void Fenetre::chg_champ_unite_angle(int v)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }

        liste_unite[j].angle=degtorad(v);
    }

}

void Fenetre::chg_combo_unite_objet(const QString & text)
{
    int j;
    if (liste_selection.size()>0)
    {
        j=index_unite(liste_selection[0]);
        if (j==-1){
            return;
        }

        for (unsigned int i=0; i<NOMBRE_OBJET; i++){
            if (text.compare(liste_objet[i].nom)==0){
                int type=liste_objet[i].type;

                if (type == 2){ // ammo goes to hangar missile
                    type = 0;
                }
                else if (type == 4){ // vehicle goes to amphi hangar
                    type = 3;
                }
                else if (type == 5){ // inf goes to amphi hangar
                    type = 3;
                }
                else if (type == 6){ // boat goes to amphi hangar
                    type = 3;
                }
                else if (type == 7){ // helo goes to helo hangar
                    type = 2;
                }
                else if (type == 8){ // bouy goes to ammo
                    type = 0;
                }


                label_unite_objet->setText("Max : " + QString::number((int)(place_restante(liste_unite[j].id, type)/liste_objet[i].taille)));
            }
        }
    }
}

void Fenetre::chg_bouton_unite_ajout_objet()
{

    if (liste_selection.size()>0)
    {
        int j=index_unite(liste_selection[0]);
        if (j==-1){

            return;
        }
        int index=0;
        int type;
        for (unsigned int i=0; i<NOMBRE_OBJET; i++)
        {
            if (combo_unite_objet->currentText().compare(liste_objet[i].nom)==0)
            {
                index=i;
                type=liste_objet[i].type;

                if (type == 2){ // ammo goes to hangar missile
                    type = 0;
                }
                else if (type == 4){ // vehicle goes to amphi hangar
                    type = 3;
                }
                else if (type == 5){ // inf goes to amphi hangar
                    type = 3;
                }
                else if (type == 6){ // boat goes to amphi hangar
                    type = 3;
                }
                else if (type == 7){ // helo goes to helo hangar
                    type = 2;
                }
                else if (type == 8){ // bouy goes to ammo
                    type = 0;
                }
            }
        }

        if (place_restante(liste_selection[0], type)-champ_unite_nombre_objet->value()*liste_objet[index].taille>=0){
            liste_unite[j].nombre_objet[index]+=champ_unite_nombre_objet->value();
        }
        label_unite_objet->setText("Max : " + QString::number((int)(place_restante(liste_unite[j].id, type)/liste_objet[index].taille)));

        delete groupe_unite_nombre_objet;
        construction_groupe_objet(liste_unite[j].id);
        layout_unite_objet->addWidget(groupe_unite_nombre_objet);
    }
}

void Fenetre::chg_bouton_unite_reset_objet()
{
    if (liste_selection.size()>0)
    {
        int j=index_unite(liste_selection[0]);
        if (j==-1)
        {
            return;
        }

        for (unsigned int i=0; i<NOMBRE_OBJET; i++)
        {
            liste_unite[j].nombre_objet[i]=0;
        }

        delete groupe_unite_nombre_objet;
        construction_groupe_objet(liste_unite[j].id);
        layout_unite_objet->addWidget(groupe_unite_nombre_objet);
    }
}

void Fenetre::chg_bouton_unite_delete()
{
    if (liste_selection.size()>0)
    {
        fenetre_unite->setVisible(false);

        int k = index_unite(liste_selection[0]);

        if (k != -1){
            liste_unite[k].detruite = true;
            check_dead();
        }
    }
}

void Fenetre::construction_fenetre_pays()
{
    fenetre_pays = new QWidget();
    QVBoxLayout *layout_pays = new QVBoxLayout;

    QWidget *groupe_pays_joueur = new QWidget(fenetre_pays);   // pays joueur
    QLabel *label_pays_joueur = new QLabel("Player's country : ", groupe_pays_joueur);
    QHBoxLayout *layout_pays_joueur = new QHBoxLayout;

    combo_pays_joueur = new QComboBox(groupe_pays_joueur);

    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        combo_pays_joueur->addItem(liste_pays[i].nom);
        if (joueur==i)
        {
            combo_pays_joueur->setCurrentIndex(i);
        }
    }

    layout_pays_joueur->addWidget(label_pays_joueur);
    layout_pays_joueur->addWidget(combo_pays_joueur);
    groupe_pays_joueur->setLayout(layout_pays_joueur);

    layout_pays->addWidget(groupe_pays_joueur);

    groupe_pays_relation = new QGroupBox("Diplomacy", groupe_pays_joueur);
    layout_pays_relation = new QVBoxLayout;

    QWidget *groupe_pays_pays1 = new QWidget(groupe_pays_relation);   // pays 1
    QLabel *label_pays_pays1 = new QLabel("Country 1 : ", groupe_pays_pays1);
    QHBoxLayout *layout_pays_pays1 = new QHBoxLayout;

    combo_pays_pays1 = new QComboBox(groupe_pays_pays1);

    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        combo_pays_pays1->addItem(liste_pays[i].nom);
    }

    layout_pays_pays1->addWidget(label_pays_pays1);
    layout_pays_pays1->addWidget(combo_pays_pays1);
    groupe_pays_pays1->setLayout(layout_pays_pays1);

    layout_pays_relation->addWidget(groupe_pays_pays1);


    QWidget *groupe_pays_pays2 = new QWidget(groupe_pays_relation);   // pays 2
    QLabel *label_pays_pays2 = new QLabel("Country 2 : ", groupe_pays_pays2);
    QHBoxLayout *layout_pays_pays2 = new QHBoxLayout;

    combo_pays_pays2 = new QComboBox(groupe_pays_pays2);

    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        combo_pays_pays2->addItem(liste_pays[i].nom);
    }
    combo_pays_pays2->setCurrentIndex(1);

    layout_pays_pays2->addWidget(label_pays_pays2);
    layout_pays_pays2->addWidget(combo_pays_pays2);
    groupe_pays_pays2->setLayout(layout_pays_pays2);

    layout_pays_relation->addWidget(groupe_pays_pays2);

    QWidget *groupe_pays_bouton= new QWidget(groupe_pays_relation);   // boutons
    QHBoxLayout *layout_pays_bouton = new QHBoxLayout;

    bouton_pays_alliance = new QPushButton("Create alliance", groupe_pays_bouton);
    bouton_pays_guerre = new QPushButton("Create war", groupe_pays_bouton);
    bouton_pays_reset = new QPushButton("Reset", groupe_pays_bouton);

    layout_pays_bouton->addWidget(bouton_pays_alliance);
    layout_pays_bouton->addWidget(bouton_pays_guerre);
    layout_pays_bouton->addWidget(bouton_pays_reset);
    groupe_pays_bouton->setLayout(layout_pays_bouton);

    layout_pays_relation->addWidget(groupe_pays_bouton);

    groupe_pays_relation->setLayout(layout_pays_relation);

    construction_groupe_diplomatie();
    layout_pays_relation->addWidget(groupe_pays_diplomatie);

    layout_pays->addWidget(groupe_pays_relation);

    fenetre_pays->setLayout(layout_pays);

    fenetre_pays->setVisible(true);
    fenetre_pays->setWindowTitle("Country management");

    QObject::connect(combo_pays_joueur, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(chg_combo_pays_joueur(const QString &)));
    QObject::connect(bouton_pays_alliance, SIGNAL(clicked()), this, SLOT(chg_bouton_pays_alliance()));
    QObject::connect(bouton_pays_guerre, SIGNAL(clicked()), this, SLOT(chg_bouton_pays_guerre()));
    QObject::connect(bouton_pays_reset, SIGNAL(clicked()), this, SLOT(chg_bouton_pays_reset()));
}

void Fenetre::construction_groupe_diplomatie()
{
    groupe_pays_diplomatie = new QWidget(groupe_pays_relation);
    QVBoxLayout *layout_pays_diplomatie = new QVBoxLayout;

    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        for (unsigned int j=0; j<i; j++)
        {
            if (liste_pays[i].relation[j]==1)
            {
                QLabel *lab = new QLabel(liste_pays[i].nom + " is allied with " + liste_pays[j].nom, groupe_pays_diplomatie);
                layout_pays_diplomatie->addWidget(lab);
            }
            if (liste_pays[i].relation[j]==2)
            {
                QLabel *lab = new QLabel(liste_pays[i].nom + " is at war with " + liste_pays[j].nom, groupe_pays_diplomatie);
                layout_pays_diplomatie->addWidget(lab);
            }
        }
    }

    groupe_pays_diplomatie->setLayout(layout_pays_diplomatie);

}

void Fenetre::chg_combo_pays_joueur(const QString & text)
{
    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        if(text.compare(liste_pays[i].nom)==0)
        {
            joueur=i;
        }
    }

}

void Fenetre::chg_bouton_pays_alliance()
{
    int pays1=combo_pays_pays1->currentIndex();
    int pays2=combo_pays_pays2->currentIndex();

    if (pays1!=pays2)
    {
        liste_pays[pays1].relation[pays2]=1;
        liste_pays[pays2].relation[pays1]=1;

        delete groupe_pays_diplomatie;
        construction_groupe_diplomatie();
        layout_pays_relation->addWidget(groupe_pays_diplomatie);
    }
}

void Fenetre::chg_bouton_pays_guerre()
{
    int pays1=combo_pays_pays1->currentIndex();
    int pays2=combo_pays_pays2->currentIndex();

    if (pays1!=pays2)
    {
        liste_pays[pays1].relation[pays2]=2;
        liste_pays[pays2].relation[pays1]=2;

        delete groupe_pays_diplomatie;
        construction_groupe_diplomatie();
        layout_pays_relation->addWidget(groupe_pays_diplomatie);
    }
}

void Fenetre::chg_bouton_pays_reset()
{
    for (unsigned int i=0; i<NOMBRE_PAYS; i++)
    {
        for (unsigned int j=0; j<NOMBRE_PAYS; j++)
        {
            liste_pays[i].relation[j]=0;
        }
    }

    delete groupe_pays_diplomatie;
    construction_groupe_diplomatie();
    layout_pays_relation->addWidget(groupe_pays_diplomatie);
}

void Fenetre::construction_fenetre_map()
{
    string copie;
    QString copie2;

    fenetre_map = new QWidget();
    QVBoxLayout *layout_map = new QVBoxLayout;

    QLabel *lab = new QLabel("Choose the mission's map. You can create new maps with the map generator", fenetre_map);
    combo_map = new QComboBox(fenetre_map);


    DIR *rep;

    rep = opendir("data/map");
    struct dirent *lecture;

    while ((lecture = readdir(rep)))
    {
        copie=lecture->d_name;
        if (copie.size()>5 && copie.substr(copie.size()-6)==".damap")
        {
            copie2= QString::fromStdString(copie.substr(0,copie.size()-6));
            combo_map->addItem(copie2);
        }
    }

    QPushButton *but = new QPushButton("Change map", fenetre_map);

    layout_map->addWidget(lab);
    layout_map->addWidget(combo_map);
    layout_map->addWidget(but);

    fenetre_map->setLayout(layout_map);

    fenetre_map->setVisible(true);
    fenetre_map->setWindowTitle("Map management");

    if (combo_map->count()==0)
    {
        but->setEnabled(false);
    }

    QObject::connect(but, SIGNAL(clicked()), this, SLOT(chg_bouton_map()));
}

void Fenetre::chg_bouton_map()
{
    int reponse = QMessageBox::warning(this, "Warning", "Do you really want to change the map ? This will delete all the current units !", QMessageBox ::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes)
    {
        for (unsigned int i=0; i<liste_unite.size(); i++)
        {
            if (liste_unite[i].place)
            {
                scene->removeItem(liste_unite[i].pixmap);
            }
            if (liste_unite[i].destination_place)
            {
                scene->removeItem(liste_unite[i].pixmap_destination);
            }
        }

        liste_unite.clear();
        chargement_map(combo_map->currentText());
        pixmap_map.chg=1;
    }
}

void Fenetre::construction_fenetre_sat(){
    fenetre_sat = new QWidget();
    QVBoxLayout *layout_sat = new QVBoxLayout;

    bouton_sat_ajout = new QPushButton("Add new satellite", fenetre_sat);
    layout_sat->addWidget(bouton_sat_ajout);

    groupe_sat_sat = new QGroupBox("Current sat(s)", fenetre_sat);
    layout_sat_sat = new QVBoxLayout;

    construction_groupe_sat();

    layout_sat->addWidget(groupe_sat_sat);

    fenetre_sat->setFixedWidth(210);
    fenetre_sat->setFixedHeight(90 + liste_sat.size()*57);

    fenetre_sat->setLayout(layout_sat);
    fenetre_sat->setVisible(true);
    fenetre_sat->setWindowTitle("Satellites management");

    QObject::connect(bouton_sat_ajout, SIGNAL(clicked()), this, SLOT(chg_bouton_sat_ajout()));
}

void Fenetre::construction_groupe_sat()
{
    liste_bouton_sat.clear();

    QSignalMapper *signalMapper = new QSignalMapper();

    groupe_sat_sat2 = new QWidget(groupe_sat_sat);
    QVBoxLayout *layout_sat_sat2 = new QVBoxLayout;

    for (int i=0; i<liste_sat.size(); i++)
    {

        QWidget *groupe = new QWidget(groupe_sat_sat2);
        QHBoxLayout *layout = new QHBoxLayout;

        QLabel *lab = new QLabel(liste_sat[i].nom, groupe_sat_sat2);
        QPushButton *bouton = new QPushButton("Modify", groupe_sat_sat2);

        layout->addWidget(lab);
        layout->addWidget(bouton);
        groupe->setLayout(layout);

        layout_sat_sat2->addWidget(groupe);

        liste_bouton_sat.push_back(bouton);

        QObject::connect(liste_bouton_sat[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(liste_bouton_sat[i], i);
    }

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_sat(int)));

    groupe_sat_sat2->setLayout(layout_sat_sat2);

    layout_sat_sat->addWidget(groupe_sat_sat2);
    groupe_sat_sat2->setVisible(true);
}

void Fenetre::chg_bouton_sat_ajout()
{
    sat copie;

    copie.nom="Satellite " + QString::number(liste_sat.size()+1);
    copie.pays = USA;
    copie.type = 0;
    copie.precision = 50;
    copie.temps_ailleurs = 10;
    copie.temps_dessus = 1;
    liste_sat.push_back(copie);

    construction_groupe_sat();
    groupe_sat_sat2->setVisible(true);
    layout_sat_sat->addWidget(groupe_sat_sat2);

    fenetre_sat->setFixedHeight(90 + liste_sat.size()*57);

}

void Fenetre::chg_bouton_sat(int a)
{
    sat_actuel=a;

    fenetre_gestion_sat = new QWidget();
    layout_gestion_sat = new QVBoxLayout;

    QWidget *groupe_gestion_nom = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_nom = new QLabel("Satellite's name : ", groupe_gestion_nom);
    QHBoxLayout *layout_gestion_nom = new QHBoxLayout;

    champ_gestion_nom_sat = new QLineEdit(liste_sat[a].nom, groupe_gestion_nom);
    layout_gestion_nom->addWidget(label_gestion_nom);
    layout_gestion_nom->addWidget(champ_gestion_nom_sat);
    groupe_gestion_nom->setLayout(layout_gestion_nom);

    layout_gestion_sat->addWidget(groupe_gestion_nom);

    QWidget *groupe_gestion_pays = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_pays = new QLabel("Country : ", groupe_gestion_pays);
    QHBoxLayout *layout_gestion_pays = new QHBoxLayout;

    combo_pays_sat = new QComboBox(groupe_gestion_pays);

    for (int i = 0; i < NOMBRE_PAYS; i++){
        combo_pays_sat->addItem(liste_pays[i].nom);
    }
    combo_pays_sat->setCurrentIndex(liste_sat[sat_actuel].pays);
    layout_gestion_pays->addWidget(label_gestion_pays);
    layout_gestion_pays->addWidget(combo_pays_sat);
    groupe_gestion_pays->setLayout(layout_gestion_pays);

    layout_gestion_sat->addWidget(groupe_gestion_pays);

    QWidget *groupe_gestion_geo = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_geo = new QLabel("Is satellite geostationary : ", groupe_gestion_geo);
    QHBoxLayout *layout_gestion_geo = new QHBoxLayout;

    champ_geo_sat = new QCheckBox(fenetre_gestion_sat);
    champ_geo_sat->setChecked(liste_sat[sat_actuel].type == 0);
    layout_gestion_geo->addWidget(label_gestion_geo);
    layout_gestion_geo->addWidget(champ_geo_sat);
    groupe_gestion_geo->setLayout(layout_gestion_geo);

    layout_gestion_sat->addWidget(groupe_gestion_geo);


    QWidget *groupe_gestion_temps_dessus = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_temps_dessus = new QLabel("Time on zone : ", groupe_gestion_temps_dessus);
    QHBoxLayout *layout_gestion_temps_dessus = new QHBoxLayout;

    champ_temps_dessus_sat = new QSpinBox(groupe_gestion_temps_dessus);
    champ_temps_dessus_sat->setValue(liste_sat[sat_actuel].temps_dessus);
    champ_temps_dessus_sat->setEnabled(liste_sat[sat_actuel].type == 1);
    champ_temps_dessus_sat->setMaximum(310490251);
    layout_gestion_temps_dessus->addWidget(label_gestion_temps_dessus);
    layout_gestion_temps_dessus->addWidget(champ_temps_dessus_sat);
    groupe_gestion_temps_dessus->setLayout(layout_gestion_temps_dessus);

    layout_gestion_sat->addWidget(groupe_gestion_temps_dessus);

    QWidget *groupe_gestion_temps_ailleurs = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_temps_ailleurs = new QLabel("Time out of zone : ", groupe_gestion_temps_ailleurs);
    QHBoxLayout *layout_gestion_temps_ailleurs = new QHBoxLayout;

    champ_temps_ailleurs_sat = new QSpinBox(groupe_gestion_temps_ailleurs);
    champ_temps_ailleurs_sat->setValue(liste_sat[sat_actuel].temps_ailleurs);
    champ_temps_ailleurs_sat->setEnabled(liste_sat[sat_actuel].type == 1);
    champ_temps_ailleurs_sat->setMaximum(310490251);//2
    layout_gestion_temps_ailleurs->addWidget(label_gestion_temps_ailleurs);
    layout_gestion_temps_ailleurs->addWidget(champ_temps_ailleurs_sat);
    groupe_gestion_temps_ailleurs->setLayout(layout_gestion_temps_ailleurs);

    layout_gestion_sat->addWidget(groupe_gestion_temps_ailleurs);

    QWidget *groupe_gestion_precision = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_precision = new QLabel("Resolution : ", groupe_gestion_precision);
    QHBoxLayout *layout_gestion_precision = new QHBoxLayout;

    champ_precision_sat = new QSpinBox(groupe_gestion_precision);
    champ_precision_sat->setValue(liste_sat[sat_actuel].precision);
    champ_precision_sat->setMaximum(100);
    layout_gestion_precision->addWidget(label_gestion_precision);
    layout_gestion_precision->addWidget(champ_precision_sat);
    groupe_gestion_precision->setLayout(layout_gestion_precision);

    layout_gestion_sat->addWidget(groupe_gestion_precision);

    QWidget *groupe_gestion_description = new QWidget(fenetre_gestion_sat);
    QLabel *label_gestion_description = new QLabel("Description : ", groupe_gestion_description);
    QHBoxLayout *layout_gestion_description = new QHBoxLayout;

    champ_description_sat = new QLineEdit(groupe_gestion_description);
    champ_description_sat->setText(liste_sat[sat_actuel].description);
    layout_gestion_description->addWidget(label_gestion_description);
    layout_gestion_description->addWidget(champ_description_sat);
    groupe_gestion_description->setLayout(layout_gestion_description);

    layout_gestion_sat->addWidget(groupe_gestion_description);

    QPushButton *bouton_sat_delete = new QPushButton("Delete satellite", fenetre_gestion_sat);
    layout_gestion_sat->addWidget(bouton_sat_delete);

    fenetre_gestion_sat->setLayout(layout_gestion_sat);

    fenetre_gestion_sat->setVisible(true);
    fenetre_gestion_sat->setWindowTitle(liste_sat[a].nom);

    QObject::connect(bouton_sat_delete, SIGNAL(clicked()), this, SLOT(chg_bouton_sat_delete()));
    QObject::connect(champ_gestion_nom_sat, SIGNAL(textChanged(QString)), this, SLOT(chg_champ_gestion_nom_sat(QString)));
    QObject::connect(combo_pays_sat, SIGNAL(currentIndexChanged(int)), this, SLOT(chg_combo_pays_sat(int)));
    QObject::connect(champ_geo_sat, SIGNAL(stateChanged(int)), this, SLOT(chg_champ_geo_sat(int)));
    QObject::connect(champ_temps_dessus_sat, SIGNAL(valueChanged(int)), this, SLOT(chg_champ_temps_dessus_sat(int)));
    QObject::connect(champ_temps_ailleurs_sat, SIGNAL(valueChanged(int)), this, SLOT(chg_champ_temps_ailleurs_sat(int)));
    QObject::connect(champ_precision_sat, SIGNAL(valueChanged(int)), this, SLOT(chg_champ_precision_sat(int)));
    QObject::connect(champ_description_sat, SIGNAL(textChanged(QString)), this, SLOT(chg_champ_description_sat(QString)));
}

void Fenetre::chg_bouton_sat_delete()
{
    for (int i=sat_actuel; i<liste_sat.size()-1; i++)
    {
        liste_sat[i]=liste_sat[i+1];
    }
    liste_sat.pop_back();

    delete fenetre_gestion_sat;

    delete groupe_sat_sat2;
    construction_groupe_sat();

    fenetre_sat->setFixedHeight(90 + liste_sat.size()*57);
}

void Fenetre::chg_champ_gestion_nom_sat(QString a)
{
    if (a.size()>10)
    {
        a.resize(10);
    }
    liste_sat[sat_actuel].nom=a;

    delete groupe_sat_sat2;
    construction_groupe_sat();
}

void Fenetre::chg_champ_geo_sat(int a){
    a++;
    a--;
    if (champ_geo_sat->isChecked()){
        liste_sat[sat_actuel].type = 0;
    }
    else {
        liste_sat[sat_actuel].type = 1;
    }
    champ_temps_dessus_sat->setEnabled(liste_sat[sat_actuel].type == 1);
    champ_temps_ailleurs_sat->setEnabled(liste_sat[sat_actuel].type == 1);
}

void Fenetre::chg_champ_temps_dessus_sat(int a){
    liste_sat[sat_actuel].temps_dessus = a;
}

void Fenetre::chg_champ_temps_ailleurs_sat(int a){
    liste_sat[sat_actuel].temps_ailleurs = a;
}

void Fenetre::chg_combo_pays_sat(int i){
    liste_sat[sat_actuel].pays = i;
}

void Fenetre::chg_champ_description_sat(QString s){
    liste_sat[sat_actuel].description = s;
}

void Fenetre::chg_champ_precision_sat(int s){
    liste_sat[sat_actuel].precision = s;
}

void Fenetre::construction_fenetre_script()
{
    fenetre_script = new QWidget();
    QVBoxLayout *layout_script = new QVBoxLayout;

    bouton_script_ajout = new QPushButton("Add new script", fenetre_script);
    layout_script->addWidget(bouton_script_ajout);

    groupe_script_script = new QGroupBox("Current script(s)", fenetre_script);
    layout_script_script = new QVBoxLayout;

    construction_groupe_script();

    layout_script->addWidget(groupe_script_script);

    fenetre_script->setFixedWidth(210);
    fenetre_script->setFixedHeight(90 + liste_script.size()*57);

    fenetre_script->setLayout(layout_script);
    fenetre_script->setVisible(true);
    fenetre_script->setWindowTitle("Script management");

    QObject::connect(bouton_script_ajout, SIGNAL(clicked()), this, SLOT(chg_bouton_script_ajout()));
}

void Fenetre::construction_groupe_script()
{
    liste_bouton_script.clear();

    QSignalMapper *signalMapper = new QSignalMapper();

    groupe_script_script2 = new QWidget(groupe_script_script);
    QVBoxLayout *layout_script_script2 = new QVBoxLayout;

    for (unsigned int i=0; i<liste_script.size(); i++)
    {

        QWidget *groupe = new QWidget(groupe_script_script2);
        QHBoxLayout *layout = new QHBoxLayout;

        QLabel *lab = new QLabel(liste_script[i].nom, groupe_script_script2);
        QPushButton *bouton = new QPushButton("Modify", groupe_script_script2);

        layout->addWidget(lab);
        layout->addWidget(bouton);
        groupe->setLayout(layout);

        layout_script_script2->addWidget(groupe);

        liste_bouton_script.push_back(bouton);

        QObject::connect(liste_bouton_script[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(liste_bouton_script[i], i);
    }

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_script(int)));

    groupe_script_script2->setLayout(layout_script_script2);

    layout_script_script->addWidget(groupe_script_script2);
    groupe_script_script2->setVisible(true);
}

void Fenetre::chg_bouton_script_ajout()
{
    script copie;

    copie.nom="Script " + QString::number(liste_script.size()+1);
    liste_script.push_back(copie);

    construction_groupe_script();
    groupe_script_script2->setVisible(true);
    layout_script_script->addWidget(groupe_script_script2);

    fenetre_script->setFixedHeight(90 + liste_script.size()*57);

}

void Fenetre::chg_bouton_script(int a)
{
    script_actuel=a;

    fenetre_gestion = new QWidget();
    layout_gestion = new QVBoxLayout;

    QWidget *groupe_gestion_nom = new QWidget(fenetre_gestion);
    QLabel *label_gestion_nom = new QLabel("Script's name : ", groupe_gestion_nom);
    QHBoxLayout *layout_gestion_nom = new QHBoxLayout;

    champ_gestion_nom = new QLineEdit(liste_script[a].nom, groupe_gestion_nom);
    layout_gestion_nom->addWidget(label_gestion_nom);
    layout_gestion_nom->addWidget(champ_gestion_nom);
    groupe_gestion_nom->setLayout(layout_gestion_nom);

    layout_gestion->addWidget(groupe_gestion_nom);


    QWidget *groupe_gestion_nvcause = new QWidget(fenetre_gestion);
    QHBoxLayout *layout_gestion_nvcause = new QHBoxLayout;

    combo_gestion_nvcause = new QComboBox(groupe_gestion_nvcause);
    combo_gestion_nvcause->addItem("Time");
    bouton_gestion_nvcause = new QPushButton("Add new cause", groupe_gestion_nvcause);
    layout_gestion_nvcause->addWidget(combo_gestion_nvcause);
    layout_gestion_nvcause->addWidget(bouton_gestion_nvcause);
    groupe_gestion_nvcause->setLayout(layout_gestion_nvcause);

    layout_gestion->addWidget(groupe_gestion_nvcause);

    QWidget *groupe_gestion_nvconsequence = new QWidget(fenetre_gestion);
    QHBoxLayout *layout_gestion_nvconsequence = new QHBoxLayout;

    combo_gestion_nvconsequence = new QComboBox(groupe_gestion_nvconsequence);
    combo_gestion_nvconsequence->addItem("Victory");
    combo_gestion_nvconsequence->addItem("Defeat");
    bouton_gestion_nvconsequence = new QPushButton("Add new consequence", groupe_gestion_nvconsequence);
    layout_gestion_nvconsequence->addWidget(combo_gestion_nvconsequence);
    layout_gestion_nvconsequence->addWidget(bouton_gestion_nvconsequence);
    groupe_gestion_nvconsequence->setLayout(layout_gestion_nvconsequence);

    layout_gestion->addWidget(groupe_gestion_nvconsequence);

    construction_groupe_cause();
    construction_groupe_consequence();

    QPushButton *bouton_script_delete = new QPushButton("Delete Script", fenetre_gestion);
    layout_gestion->addWidget(bouton_script_delete);

    fenetre_gestion->setLayout(layout_gestion);

    fenetre_gestion->setVisible(true);
    fenetre_gestion->setWindowTitle(liste_script[a].nom);

    QObject::connect(bouton_gestion_nvcause, SIGNAL(clicked()), this, SLOT(chg_bouton_gestion_nvcause()));
    QObject::connect(bouton_gestion_nvconsequence, SIGNAL(clicked()), this, SLOT(chg_bouton_gestion_nvconsequence()));
    QObject::connect(bouton_script_delete, SIGNAL(clicked()), this, SLOT(chg_bouton_script_delete()));
    QObject::connect(champ_gestion_nom, SIGNAL(textChanged(QString)), this, SLOT(chg_champ_gestion_nom(QString)));
}

void Fenetre::chg_bouton_script_delete()
{
    for (unsigned int i=script_actuel; i<liste_script.size()-1; i++)
    {
        liste_script[i]=liste_script[i+1];
    }
    liste_script.pop_back();

    delete fenetre_gestion;

    delete groupe_script_script2;
    construction_groupe_script();

    fenetre_script->setFixedHeight(90 + liste_script.size()*57);
}

void Fenetre::chg_champ_gestion_nom(QString a)
{
    if (a.size()>10)
    {
        a.resize(10);
    }
    liste_script[script_actuel].nom=a;

    delete groupe_script_script2;
    construction_groupe_script();
}

void Fenetre::construction_groupe_cause()
{
    QSignalMapper *signalMapper = new QSignalMapper();
    liste_bouton_cause_delete.clear();

    groupe_gestion_cause = new QWidget(fenetre_gestion);
    layout_gestion_cause = new QVBoxLayout;

    for (unsigned int i=0; i<liste_script[script_actuel].liste_cause.size(); i++)
    {
        if (liste_script[script_actuel].liste_cause[i].type==0) // time
        {
            QGroupBox *groupe = new QGroupBox("Cause " + QString::number(i+1), groupe_gestion_cause);
            QVBoxLayout *layout = new QVBoxLayout;

            QLabel *lab = new QLabel("Time  : " + QString::number(liste_script[script_actuel].liste_cause[i].temps) + " s", groupe);
            QPushButton *butt = new QPushButton("Delete cause", groupe);

            layout->addWidget(lab);
            layout->addWidget(butt);
            groupe->setLayout(layout);

            layout_gestion_cause->addWidget(groupe);

            liste_bouton_cause_delete.push_back(butt);

            QObject::connect(liste_bouton_cause_delete[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(liste_bouton_cause_delete[i], i);
        }
    }
    groupe_gestion_cause->setLayout(layout_gestion_cause);

    layout_gestion->addWidget(groupe_gestion_cause);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_cause_delete(int)));
}

void Fenetre::construction_groupe_consequence()
{
    QSignalMapper *signalMapper = new QSignalMapper();
    liste_bouton_consequence_delete.clear();

    groupe_gestion_consequence = new QWidget(fenetre_gestion);
    layout_gestion_consequence = new QVBoxLayout;

    for (unsigned int i=0; i<liste_script[script_actuel].liste_consequence.size(); i++)
    {
        QGroupBox *groupe = new QGroupBox("Consequence " + QString::number(i+1), groupe_gestion_consequence);
        QVBoxLayout *layout = new QVBoxLayout;

        QPushButton *butt = new QPushButton("Delete consequence", groupe);

        if (liste_script[script_actuel].liste_consequence[i].type==0) // victory
        {
            QLabel *lab = new QLabel("Player will win", groupe);

            layout->addWidget(lab);
        }

        if (liste_script[script_actuel].liste_consequence[i].type==1) // defeat
        {
            QLabel *lab = new QLabel("Player will lose", groupe);

            layout->addWidget(lab);
        }

        layout->addWidget(butt);

        groupe->setLayout(layout);

        layout_gestion_consequence->addWidget(groupe);

        liste_bouton_consequence_delete.push_back(butt);

        QObject::connect(liste_bouton_consequence_delete[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(liste_bouton_consequence_delete[i], i);
    }
    groupe_gestion_consequence->setLayout(layout_gestion_consequence);

    layout_gestion->addWidget(groupe_gestion_consequence);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(chg_bouton_consequence_delete(int)));
}

void Fenetre::chg_bouton_gestion_nvcause()
{
    int y=combo_gestion_nvcause->currentIndex();

    if (y==0)  // time
    {
        cause_actuelle=y;

        fenetre_cause = new QWidget();
        QVBoxLayout *layout_cause = new QVBoxLayout;

        QWidget *groupe_cause1 = new QWidget(fenetre_cause);
        QLabel *label_cause1 = new QLabel("Time (s) : ", groupe_cause1);
        QHBoxLayout *layout_cause1 = new QHBoxLayout;

        champ_cause_temps = new QSpinBox(groupe_cause1);
        champ_cause_temps->setMaximum(999999);

        layout_cause1->addWidget(label_cause1);
        layout_cause1->addWidget(champ_cause_temps);
        groupe_cause1->setLayout(layout_cause1);

        layout_cause->addWidget(groupe_cause1);

        bouton_cause_ok = new QPushButton("Ok", fenetre_cause);
        layout_cause->addWidget(bouton_cause_ok);

        fenetre_cause->setLayout(layout_cause);

        fenetre_cause->setVisible(true);
        fenetre_cause->setWindowTitle("Time");

        QObject::connect(bouton_cause_ok, SIGNAL(clicked()), this, SLOT(chg_bouton_cause_ok()));

    }
}

void Fenetre::chg_bouton_gestion_nvconsequence()
{
    int y=combo_gestion_nvconsequence->currentIndex();

    consequence_actuelle=y;

    fenetre_consequence = new QWidget();
    QVBoxLayout *layout_consequence = new QVBoxLayout;

    if (y==0)  // victory
    {
        QWidget *groupe_consequence1 = new QWidget(fenetre_consequence);
        QLabel *label_consequence1 = new QLabel("This consequence will make the player win the game.", groupe_consequence1);
        QHBoxLayout *layout_consequence1 = new QHBoxLayout;

        layout_consequence1->addWidget(label_consequence1);
        groupe_consequence1->setLayout(layout_consequence1);

        layout_consequence->addWidget(groupe_consequence1);

        fenetre_consequence->setWindowTitle("Victory");
    }
    if (y==1)  // defeat
    {
        QWidget *groupe_consequence1 = new QWidget(fenetre_consequence);
        QLabel *label_consequence1 = new QLabel("This consequence will make the player lose the game.", groupe_consequence1);
        QHBoxLayout *layout_consequence1 = new QHBoxLayout;

        layout_consequence1->addWidget(label_consequence1);
        groupe_consequence1->setLayout(layout_consequence1);

        layout_consequence->addWidget(groupe_consequence1);

        fenetre_consequence->setWindowTitle("Victory");
    }

    bouton_consequence_ok = new QPushButton("Ok", fenetre_consequence);
    layout_consequence->addWidget(bouton_consequence_ok);

    fenetre_consequence->setLayout(layout_consequence);

    fenetre_consequence->setVisible(true);

    QObject::connect(bouton_consequence_ok, SIGNAL(clicked()), this, SLOT(chg_bouton_consequence_ok()));
}

void Fenetre::chg_bouton_cause_ok()
{
    cause copie_cause;

    copie_cause.type=cause_actuelle;

    if (cause_actuelle==0)  // time
    {
        copie_cause.temps=champ_cause_temps->value();

        liste_script[script_actuel].liste_cause.push_back(copie_cause);

        delete fenetre_cause;

        delete groupe_gestion_cause;
        construction_groupe_cause();
        delete groupe_gestion_consequence;
        construction_groupe_consequence();

    }
}

void Fenetre::chg_bouton_consequence_ok()
{
    consequence copie_consequence;

    copie_consequence.type=consequence_actuelle;

    if (consequence_actuelle==0)  // victory
    {

    }
    else if (consequence_actuelle==1)  // defeat
    {

    }

    liste_script[script_actuel].liste_consequence.push_back(copie_consequence);

    delete fenetre_consequence;

    delete groupe_gestion_cause;
    construction_groupe_cause();
    delete groupe_gestion_consequence;
    construction_groupe_consequence();


}

void Fenetre::chg_bouton_cause_delete(int a)
{
    for (unsigned int i=a; i<liste_script[script_actuel].liste_cause.size()-1; i++)
    {
        liste_script[script_actuel].liste_cause[i]=liste_script[script_actuel].liste_cause[i+1];
    }
    liste_script[script_actuel].liste_cause.pop_back();

    delete groupe_gestion_cause;
    construction_groupe_cause();

    fenetre_gestion->setVisible(false);
    fenetre_gestion->setVisible(true);
}

void Fenetre::chg_bouton_consequence_delete(int a)
{
    for (unsigned int i=a; i<liste_script[script_actuel].liste_consequence.size()-1; i++)
    {
        liste_script[script_actuel].liste_consequence[i]=liste_script[script_actuel].liste_consequence[i+1];
    }
    liste_script[script_actuel].liste_consequence.pop_back();

    delete groupe_gestion_consequence;
    construction_groupe_consequence();

    fenetre_gestion->setVisible(false);
    fenetre_gestion->setVisible(true);
}

void Fenetre::construction_fenetre_ia(){
    QWidget* fenetre_ia = new QWidget();
    QVBoxLayout *layout_ia = new QVBoxLayout;

    QWidget* groupe_ia1 = new QWidget(fenetre_ia);
    QHBoxLayout *layout_groupe_ia1 = new QHBoxLayout;

    QLabel* label_ia = new QLabel("AI's country : ", groupe_ia1);
    QComboBox* combo_ia = new QComboBox(groupe_ia1);

    for (unsigned int i = 0; i < NOMBRE_PAYS; i++){
        combo_ia->addItem(liste_pays[i].nom);
    }

    combo_ia->setCurrentIndex(ia_actuelle);

    layout_groupe_ia1->addWidget(label_ia);
    layout_groupe_ia1->addWidget(combo_ia);
    groupe_ia1->setLayout(layout_groupe_ia1);

    layout_ia->addWidget(groupe_ia1);


    QGroupBox* groupe_ia3 = new QGroupBox("Aircraft policy", fenetre_ia);
    QVBoxLayout *layout_groupe_ia3 = new QVBoxLayout;

    QWidget* sub1_groupe_ia3 = new QWidget(groupe_ia3);
    QHBoxLayout *layout_sub1_groupe_ia3 = new QHBoxLayout;

    QLabel* label_ia_carbu_retour = new QLabel("Percentage of fuel left when aircrafts automatically land", sub1_groupe_ia3);
    spin_ia_carbu_retour = new QSpinBox(sub1_groupe_ia3);
    spin_ia_carbu_retour->setMaximum(100);

    layout_sub1_groupe_ia3->addWidget(label_ia_carbu_retour);
    layout_sub1_groupe_ia3->addWidget(spin_ia_carbu_retour);
    sub1_groupe_ia3->setLayout(layout_sub1_groupe_ia3);
    layout_groupe_ia3->addWidget(sub1_groupe_ia3);

    groupe_ia3->setLayout(layout_groupe_ia3);

    QGroupBox* groupe_ia2 = new QGroupBox("Missile policy", fenetre_ia);
    QVBoxLayout *layout_groupe_ia2 = new QVBoxLayout;

    QWidget* sub1_groupe_ia2 = new QWidget(groupe_ia2);
    QHBoxLayout *layout_sub1_groupe_ia2 = new QHBoxLayout;

    QLabel* label_ia_nombre_missile = new QLabel("Number of missiles fired on incoming missiles", sub1_groupe_ia2);
    spin_ia_nombre_missile = new QSpinBox(sub1_groupe_ia2);

    layout_sub1_groupe_ia2->addWidget(label_ia_nombre_missile);
    layout_sub1_groupe_ia2->addWidget(spin_ia_nombre_missile);
    sub1_groupe_ia2->setLayout(layout_sub1_groupe_ia2);
    layout_groupe_ia2->addWidget(sub1_groupe_ia2);

    QWidget* sub2_groupe_ia2 = new QWidget(groupe_ia2);
    QHBoxLayout *layout_sub2_groupe_ia2 = new QHBoxLayout;

    QLabel* label_ia_nombre_missile_variable = new QLabel("Fire variable number of missiles, according to the incoming missile's hitpoints", sub2_groupe_ia2);
    check_ia_nombre_missile_variable = new QCheckBox(sub2_groupe_ia2);

    layout_sub2_groupe_ia2->addWidget(label_ia_nombre_missile_variable);
    layout_sub2_groupe_ia2->addWidget(check_ia_nombre_missile_variable);
    sub2_groupe_ia2->setLayout(layout_sub2_groupe_ia2);
    layout_groupe_ia2->addWidget(sub2_groupe_ia2);

    QWidget* sub3_groupe_ia2 = new QWidget(groupe_ia2);
    QHBoxLayout *layout_sub3_groupe_ia2 = new QHBoxLayout;

    QLabel* label_ia_distance_missile = new QLabel("Range of engagement of incoming missiles", sub3_groupe_ia2);
    spin_ia_distance_missile = new QSpinBox(sub3_groupe_ia2);
    spin_ia_distance_missile->setMaximum(300000);

    layout_sub3_groupe_ia2->addWidget(label_ia_distance_missile);
    layout_sub3_groupe_ia2->addWidget(spin_ia_distance_missile);
    sub3_groupe_ia2->setLayout(layout_sub3_groupe_ia2);
    layout_groupe_ia2->addWidget(sub3_groupe_ia2);

    groupe_ia2->setLayout(layout_groupe_ia2);

    layout_ia->addWidget(groupe_ia2);
    layout_ia->addWidget(groupe_ia3);

    fenetre_ia->setLayout(layout_ia);
    fenetre_ia->setVisible(true);
    fenetre_ia->setWindowTitle("AI management");

    QObject::connect(combo_ia, SIGNAL(currentIndexChanged(int)), this, SLOT(chg_combo_ia(int)));
    QObject::connect(spin_ia_nombre_missile, SIGNAL(valueChanged(int)), this, SLOT(chg_spin_ia_nombre_missile(int)));
    QObject::connect(spin_ia_distance_missile, SIGNAL(valueChanged(int)), this, SLOT(chg_spin_ia_distance_missile(int)));
    QObject::connect(spin_ia_carbu_retour, SIGNAL(valueChanged(int)), this, SLOT(chg_spin_ia_carbu_retour(int)));
    QObject::connect(check_ia_nombre_missile_variable, SIGNAL(stateChanged(int)), this, SLOT(chg_check_ia_nombre_missile_variable(int)));

    chg_combo_ia(ia_actuelle);
}

void Fenetre::chg_combo_ia(int index){
    if (index < 0 || index >= NOMBRE_PAYS){
        erreur_log("index of combo not in range");
        return;
    }

    ia_actuelle = index;

    spin_ia_nombre_missile->setValue(liste_pays[index].data_ia.nombre_missile);
    spin_ia_distance_missile->setValue(liste_pays[index].data_ia.distance_missile);
    spin_ia_carbu_retour->setValue(liste_pays[index].data_ia.pourcentage_carbu);

    if (liste_pays[index].data_ia.nombre_missile_variable){
        check_ia_nombre_missile_variable->setChecked(true);
        spin_ia_nombre_missile->setEnabled(false);
    }
    else {
        check_ia_nombre_missile_variable->setChecked(false);
        spin_ia_nombre_missile->setEnabled(true);
    }
}

void Fenetre::chg_spin_ia_nombre_missile(int val){
    liste_pays[ia_actuelle].data_ia.nombre_missile = val;
}

void Fenetre::chg_spin_ia_distance_missile(int val){
    liste_pays[ia_actuelle].data_ia.distance_missile = val;
}

void Fenetre::chg_spin_ia_carbu_retour(int val){
    liste_pays[ia_actuelle].data_ia.pourcentage_carbu = val;
}

void Fenetre::chg_check_ia_nombre_missile_variable(int val){

    if (check_ia_nombre_missile_variable->isChecked()){
        liste_pays[ia_actuelle].data_ia.nombre_missile_variable = true;
        spin_ia_nombre_missile->setEnabled(false);
    }
    else {
        liste_pays[ia_actuelle].data_ia.nombre_missile_variable = false;
        spin_ia_nombre_missile->setEnabled(true);
    }
}

void Fenetre::slot_sauvegarde_sous()
{
    bool ok;
    QString nom = QInputDialog::getText(this, "File name", "Enter the mission's name.", QLineEdit::Normal, QString(), &ok);

    if (ok && !nom.isEmpty())
    {
        nom_fichier=nom;
        sauvegarde (nom);
    }
}
