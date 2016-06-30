#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>

Fenetre::Fenetre() : QGraphicsView(){
    //QImage im("test.png");
    //imageToAscii(im, 120, 80);

    chargement_option();

    LARGEUR_FENETRE = option.res_x;
    HAUTEUR_FENETRE = option.res_y;

    if (option.fullscreen){
        DEVMODE *dm=new DEVMODE();
        EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,dm);
        dm->dmPelsWidth=(unsigned long)LARGEUR_FENETRE;
        dm->dmPelsHeight=(unsigned long)HAUTEUR_FENETRE;
        ChangeDisplaySettings(dm,CDS_FULLSCREEN);

        showFullScreen();
    }

    setWindowTitle("Dragon's Ambitions");

    scene = new Scene(0,0,LARGEUR_FENETRE-10,HAUTEUR_FENETRE-10);
    setScene(scene);

    srand(time(NULL));

    mode = 0;
    pause = 0;
    vitesse_jeu=1;
    joueur=0;

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;

    setFixedWidth(LARGEUR_FENETRE);
    setFixedHeight(HAUTEUR_FENETRE);

    setLayout(layoutPrincipal);

    timer= new QTimer (this);

    timer->start(10);



    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));



    a_appuye=0;
    s_appuye=0;
    w_appuye=0;
    y_appuye=0;
    esc_appuye=0;
    shift_appuye=0;
    ctrl_appuye=0;
    entree_appuye = false;

    for (int i = 0; i < 10; i++){
        chiffre_appuye[i] = false;
    }

    gauche_appuye=0;
    droite_appuye=0;
    mid_appuye = 0;

    position_deplacement_mid.x = 0;
    position_deplacement_mid.y = 0;

    select_rect=0;
    select_minimap = false;

    largeur_map=2000;
    hauteur_map=2000;
    largeur_minimap = 200 * LARGEUR_FENETRE / 1280;
    hauteur_minimap = 200 * HAUTEUR_FENETRE / 720;

    setMouseTracking(true);



    valeur_zoom[0]=1.0;
    valeur_zoom[1]=1.5;
    valeur_zoom[2]=2.0;
    valeur_zoom[3]=3.0;
    valeur_zoom[4]=4.0;

    onglet_selection = new QTabWidget(this);
    onglet_selection->setFixedHeight(220);
    onglet_selection->setFixedWidth(880);
    onglet_selection->move(LARGEUR_FENETRE/2-onglet_selection->width()/2, HAUTEUR_FENETRE-onglet_selection->height());
    onglet_selection->setVisible(false);
    onglet_selection->setTabsClosable(true);
    onglet_selection->setCursor(Qt::ArrowCursor);


    image_menu = 0;
    fenetre_menu_jeu = 0;
    fenetre_sat_jeu = 0;
    fenetre_log_jeu = 0;
    fenetre_menu_option = 0;
    barre_principale_jeu = 0;
    barre_bouton = 0;
    for (int i =0; i < 10; i++){
        bouton_groupe[i] = 0;
    }

    QObject::connect(onglet_selection, SIGNAL(currentChanged (int)), this, SLOT(changement_tab(int)));
    QObject::connect(onglet_selection, SIGNAL(tabCloseRequested (int)), this, SLOT(fermeture_tab(int)));

    pixmap_map.pixmap = new QGraphicsPixmapItem();
    pixmap_map.place=0;
    pixmap_map.chg=0;

    liste_pays[0].nom="USA";
    liste_pays[1].nom="China";
    liste_pays[0].present = false;
    liste_pays[1].present = true;
    liste_pays[0].demonym="American";
    liste_pays[1].demonym="Chinese";


    set_numbers();
    clear_log();
    chargement_liste_module ();
    chargement_liste_classe ();
    chargement_liste_objet ();

    for (unsigned int i=0; i<NOMBRE_DIVERS; i++)
    {
        image_divers[i]=new QPixmap();
    }

    image_divers[0]->load("ressource/other/destination.png");
    image_divers[1]->load("ressource/other/croix.png");
    image_divers[2]->load("ressource/other/base_barre.png");
    image_divers[3]->load("ressource/other/barre_verte.png");
    image_divers[4]->load("ressource/other/barre_jaune.png");
    image_divers[5]->load("ressource/other/barre_rouge.png");
    image_divers[6]->load("ressource/other/barre_noire.png");

    for (int i = 0; i < 101; i++){
        int h, w;
        h = 20;
        w = 150;

        QImage image = image_divers[2]->toImage();
        QImage vert = image_divers[3]->toImage();
        QImage jaune = image_divers[4]->toImage();
        QImage rouge = image_divers[5]->toImage();
        QImage noir = image_divers[6]->toImage();

        for (int y = 0; y < h; y++){
            QRgb* rgb = (QRgb*)image.scanLine(y);

            for (int x = 0; x < w; x++) {
                if (rgb[x] == qRgb(100, 0, 0)){
                    QRgb rgb2;
                    if (x < w * i / 100.0){
                        if (i > 66){
                            rgb2 = vert.pixel(x,y);
                        }
                        else if (i > 33){
                            rgb2 = jaune.pixel(x,y);
                        }
                        else {
                            rgb2 = rouge.pixel(x,y);
                        }
                    }
                    else {
                        rgb2 = noir.pixel(x,y);
                    }

                    rgb[x] = rgb2;
                }
            }
        }

        liste_barre_vie[i] = QPixmap::fromImage(image);
    }

    liste_commande.push_back("stop");
    liste_commande.push_back("fire");
    liste_commande.push_back("launch");
    liste_commande.push_back("disembark");
    liste_commande.push_back("land");
    liste_commande.push_back("embark");
    liste_commande.push_back("climb");
    liste_commande.push_back("go up");
    liste_commande.push_back("rise");
    liste_commande.push_back("go down");
    liste_commande.push_back("descend");
    liste_commande.push_back("dive");
    liste_commande.push_back("speed up");
    liste_commande.push_back("slow down");
    liste_commande.push_back("speed");
    liste_commande.push_back("slow");
    liste_commande.push_back("engage");
    liste_commande.push_back("shut down radar");
    liste_commande.push_back("radar off");
    liste_commande.push_back("turn on radar");
    liste_commande.push_back("radar on");
    liste_commande.push_back("destroy sat");
    liste_commande.push_back("cease fire");
    liste_commande.push_back("drop");

    QString style;
    style+="QWidget#merde {background-color: rgb(255, 255, 255);border-width: 2px; border-radius: 15px;border-color: rgb(0, 0, 0);}";
    //style+="QGroupBox {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E0E0E0, stop: 1 #FFFFFF);border: 2px solid gray;border-radius: 5px;margin-top: 1ex; }QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0 3px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #FF0ECE, stop: 1 #FFFFFF);}";
    //style+="QPushButton#jeu {background-color: rgb(200, 200, 200); border-width: 2px; border-radius: 10px;border-color: rgb(0, 0, 0); border-style: solid;font: bold 25px; font-family: Rockwell; min-width: 10em; min-height: 1em; padding: 6px;} QPushButton#jeu:pressed {  background-color: rgb(150, 150, 150); } QPushButton#jeu:hover {  background-color:  rgb(150, 150, 150);}";
    style+="QPushButton#s1 {background-color: rgb(50, 71, 132); border-width: 2px; border-color: rgb(0, 0, 0); border-style: solid;font: bold 10px; font-family: Rockwell; padding: 1px;color: white;} QPushButton#s1:pressed {  background-color: rgb(33, 46, 84); } QPushButton#s1:hover {  background-color:  rgb(33, 46, 84);}";
    style+="QGroupBox#s2 {background-color: rgb(50, 71, 132);border: 2px solid black;margin-top: 1ex; }QGroupBox#s2::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0px 3px;}";
    style+="QWidget#s3 {border: 2px solid black;}";
    style+=load_file("data/style/selection.css");
    style+=load_file("data/style/champ_action.css");
    style+=load_file("data/style/validation_action.css");
    style+=load_file("data/style/groupe_action.css");
    style+=load_file("data/style/white_label.css").replace("xxxxx", "label_action");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe");
    style+=load_file("data/style/gray_widget.css").replace("xxxxx", "super2");
    style+=load_file("data/style/gray_widget.css").replace("xxxxx", "scroll");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_info");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_module");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_avion");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_missile");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_helo");
    style+=load_file("data/style/white_group.css").replace("xxxxx", "groupe_well_dock");
    style+=load_file("data/style/menu_button.css");
    qApp->setStyleSheet(style);


    lancer_menu();
}

bool Fenetre:: chargement_map (QString nom)
{
    string s = "data/map/" + nom.toStdString() + ".damap";
    FILE * fichier = fopen(s.c_str(), "r");
    char c;


    //image_altitude = new QImage("test_map2.png");

    map = new pixel* [largeur_map];

    for (int i=0; i<largeur_map; i++)
    {
        map[i] = new pixel[hauteur_map];
    }

    for (int i=0; i<largeur_map; i++)
    {
        for (int j=0; j<hauteur_map; j++)
        {
            map[i][j].mer=1;
            map[i][j].altitude=-100;
            map[i][j].special=0;
        }
    }

    for (int i=0; i<hauteur_map; i++)
    {
        for (int j=0; j<largeur_map; j++)
        {
            c=fgetc(fichier);

            map[j][i].type = 0;
            map[j][i].special = 0;
            map[j][i].mer=1;
            map[j][i].altitude = -100;

            if (c == 'a'){
                map[j][i].mer=1;
                map[j][i].altitude = -100;
            }
            else if (c == 'b'){
                map[j][i].mer=1;
                map[j][i].altitude = -500;
            }
            else if (c == 'c'){
                map[j][i].mer=1;
                map[j][i].altitude = -1000;
            }
            else if (c == 'A'){
                map[j][i].mer=0;
                map[j][i].altitude = 1;
            }
            else if (c == 'B'){
                map[j][i].mer=0;
                map[j][i].altitude = 100;
            }
            else if (c == 'C'){
                map[j][i].mer=0;
                map[j][i].altitude = 500;
            }
            else if (c == '+'){
                map[j][i].mer=0;
                map[j][i].altitude = 1;
                map[j][i].type = 1;
            }
            else if (c == '"'){
                map[j][i].mer=0;
                map[j][i].altitude = 100;
                map[j][i].type = 1;
            }
            else if (c == '*'){
                map[j][i].mer=0;
                map[j][i].altitude = 500;
                map[j][i].type = 1;
            }

            // a completer
        }
    }

    QImage s0("ressource/texture/s0.png");
    QImage s100("ressource/texture/s100.png");
    QImage s500("ressource/texture/s500.png");
    QImage l0("ressource/texture/l0.png");
    QImage l100("ressource/texture/l100.png");
    QImage l500("ressource/texture/l500.png");
    QImage d0("ressource/texture/d0.png");
    QImage d100("ressource/texture/d100.png");
    QImage d500("ressource/texture/d500.png");

    for (int i=0; i<NOMBRE_ZOOM; i++){
        for (int a = 0; a < largeur_map*valeur_zoom[i] / TAILLE_TILE2 + 1; a++){
            for (int b = 0; b < hauteur_map*valeur_zoom[i] / TAILLE_TILE2 + 1; b++){
                long pos = a + CONSTANTE_LONG * b;

                tile* t = (tile*)malloc(sizeof(tile));

                if (t == NULL){
                    exit(666);
                }

                t->chg = true;
                t->place = false;

                t->image = new QImage(TAILLE_TILE2, TAILLE_TILE2, QImage::Format_ARGB32_Premultiplied);

                if (t->image == NULL)
                    exit(a + CONSTANTE_LONG * b);


                for (int y = 0; y < TAILLE_TILE2; y++){
                    QRgb* rgb = (QRgb*)t->image->scanLine(y);

                    for (int x = 0; x < TAILLE_TILE2; x++) {
                        QRgb rgb2 = qRgba(0,0,0,0);
                        int xx = (a * TAILLE_TILE2 + x) / valeur_zoom[i]; // position du pixel dans la carte
                        int yy = (b * TAILLE_TILE2 + y) / valeur_zoom[i];

                        int xxx = a * TAILLE_TILE2 + x; // position dans la texture
                        int yyy = b * TAILLE_TILE2 + y;



                        if (xx < largeur_map && yy < hauteur_map){
                            bool done = false;
                            if (valeur_zoom[i] == 4.0 && xx > 0 && yy > 0 && xx < largeur_map-1 && yy < hauteur_map-1) { // anti aliasing
                                bool xmoinsun = !map[xx-1][yy].mer;
                                bool xplusun = !map[xx+1][yy].mer;
                                bool ymoinsun = !map[xx][yy-1].mer;
                                bool yplusun = !map[xx][yy+1].mer;
                                QImage* imageptr = &l0; // image juste au dessus de l'altitude

                                if (map[xx][yy].type == 0){
                                    imageptr = &l0;
                                }
                                else {
                                    imageptr = &d0;
                                }


                                if (!map[xx][yy].mer && map[xx][yy].altitude==1){
                                    xmoinsun = map[xx-1][yy].altitude==100;
                                    xplusun = map[xx+1][yy].altitude==100;
                                    ymoinsun = map[xx][yy-1].altitude==100;
                                    yplusun = map[xx][yy+1].altitude==100;

                                    if (map[xx][yy].type == 0){
                                        imageptr = &l100;
                                    }
                                    else {
                                        imageptr = &d100;
                                    }

                                }
                                else if (!map[xx][yy].mer && map[xx][yy].altitude==100){
                                    xmoinsun = map[xx-1][yy].altitude==500;
                                    xplusun = map[xx+1][yy].altitude==500;
                                    ymoinsun = map[xx][yy-1].altitude==500;
                                    yplusun = map[xx][yy+1].altitude==500;
                                    if (map[xx][yy].type == 0){
                                        imageptr = &l500;
                                    }
                                    else {
                                        imageptr = &d500;
                                    }
                                }
                                else if (!map[xx][yy].mer && map[xx][yy].altitude==500){
                                    xmoinsun = map[xx-1][yy].altitude==1000;
                                    xplusun = map[xx+1][yy].altitude==1000;
                                    ymoinsun = map[xx][yy-1].altitude==1000;
                                    yplusun = map[xx][yy+1].altitude==1000;
                                    if (map[xx][yy].type == 0){
                                        imageptr = &l500;
                                    }
                                    else {
                                        imageptr = &d500;
                                    } // A CHANGER
                                }
                                else if (map[xx][yy].mer && map[xx][yy].altitude==-100){
                                    // default values
                                }
                                else if (map[xx][yy].mer && map[xx][yy].altitude==-500){
                                    xmoinsun = map[xx-1][yy].altitude==-100;
                                    xplusun = map[xx+1][yy].altitude==-100;
                                    ymoinsun = map[xx][yy-1].altitude==-100;
                                    yplusun = map[xx][yy+1].altitude==-100;
                                    imageptr = &s0;
                                }
                                else if (map[xx][yy].mer && map[xx][yy].altitude==-1000){
                                    xmoinsun = map[xx-1][yy].altitude==-500;
                                    xplusun = map[xx+1][yy].altitude==-500;
                                    ymoinsun = map[xx][yy-1].altitude==-500;
                                    yplusun = map[xx][yy+1].altitude==-500;
                                    imageptr = &s100;
                                }

                                if (xmoinsun && ymoinsun){
                                    if (x%4 == 0 || (x%4 == 1 && y%4 < 3) || (x%4 == 2 && y%4 < 2)){
                                        rgb2 = imageptr->pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        done = true;
                                    }
                                }
                                else if (xmoinsun && yplusun){
                                    if ((x%4 == 0 && y%4 > 0)|| (x%4 == 1 && y%4 > 1) || (x%4 == 2 && y%4 > 1) || (x%4 == 3 && y%4 > 2)){
                                        rgb2 = imageptr->pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        done = true;
                                    }
                                }
                                else if (xplusun && ymoinsun){
                                    if ((x%4 == 0 && y%4 < 1)|| (x%4 == 1 && y%4 < 2) || (x%4 == 2 && y%4 < 3) || (x%4 == 3 && y%4 < 4)){
                                        rgb2 = imageptr->pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        done = true;
                                    }
                                }
                                else if (xplusun && yplusun){
                                    if ((x%4 == 0 && y%4 > 2)|| (x%4 == 1 && y%4 > 1) || (x%4 == 2 && y%4 > 0) || (x%4 == 3)){
                                        rgb2 = imageptr->pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        done = true;
                                    }
                                }
                            }
                            if (!done) {
                                if (map[xx][yy].mer){
                                    if (map[xx][yy].altitude==-100){
                                        rgb2 = s0.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                    }
                                    else if (map[xx][yy].altitude==-500){
                                        rgb2 = s100.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                    }
                                    else{ //if (map[xx][yy].altitude==-1000){
                                        rgb2 = s500.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                    }

                                }
                                else{
                                    if (map[xx][yy].altitude==1){
                                        if (map[xx][yy].type == 0){
                                            rgb2 = l0.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }
                                        else {
                                            rgb2 = d0.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }

                                    }
                                    else if (map[xx][yy].altitude==100){
                                        if (map[xx][yy].type == 0){
                                            rgb2 = l100.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }
                                        else {
                                            rgb2 = d100.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }
                                    }
                                    else { //if (map[xx][yy].altitude==500){
                                        if (map[xx][yy].type == 0){
                                            rgb2 = l500.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }
                                        else {
                                            rgb2 = d500.pixel(xxx%TAILLE_TILE, yyy%TAILLE_TILE);
                                        }
                                    }
                                }
                            }
                        }
                        rgb[x] = rgb2;
                    }
                }


                t->pixmap = new QGraphicsPixmapItem();


                t->place = 1;
                QPixmap p = QPixmap::fromImage(*t->image);
                t->pixmap->setPixmap(p);
                t->pixmap->setPos(-camera.x + a * TAILLE_TILE2,-camera.y + b * TAILLE_TILE2);
                t->pixmap->setVisible(true);
                scene->addItem(t->pixmap);
                t->pixmap->setZValue(-20);

                image_map[i].insert(pos, t);


            }
        }
    }

    image_minimap = new QImage(largeur_minimap, hauteur_minimap, QImage::Format_ARGB32_Premultiplied);

    for (int i = 0; i < hauteur_minimap; i++){
        QRgb* rgb = (QRgb*)image_minimap->scanLine(i);
        for (int j = 0; j < largeur_minimap; j++){
            QRgb rgb2 = qRgba(0,0,0,0);
            int xx = j * largeur_map / largeur_minimap;
            int yy = i * hauteur_map / hauteur_minimap;

            if (map[xx][yy].mer){
                if (map[xx][yy].altitude==-100){
                    rgb2 = s0.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                }
                else if (map[xx][yy].altitude==-500){
                    rgb2 = s100.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                }
                else{ //if (map[xx][yy].altitude==-1000){
                    rgb2 = s500.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                }

            }
            else{
                if (map[xx][yy].altitude==1){
                    if (map[xx][yy].type == 0){
                        rgb2 = l0.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }
                    else {
                        rgb2 = d0.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }

                }
                else if (map[xx][yy].altitude==100){
                    if (map[xx][yy].type == 0){
                        rgb2 = l100.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }
                    else {
                        rgb2 = d100.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }
                }
                else { //if (map[xx][yy].altitude==500){
                    if (map[xx][yy].type == 0){
                        rgb2 = l500.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }
                    else {
                        rgb2 = d500.pixel(j%TAILLE_TILE, i%TAILLE_TILE);
                    }
                }
            }


            rgb[j] = rgb2;
        }
    }

    pixmap_minimap = new QGraphicsPixmapItem();
    pixmap_minimap->setPixmap(QPixmap::fromImage(*image_minimap));
    pixmap_minimap->setPos(-4,HAUTEUR_FENETRE-hauteur_minimap-6);
    pixmap_minimap->setVisible(true);
    scene->addItem(pixmap_minimap);
    pixmap_minimap->setZValue(50);

    rectangle_minimap = scene->addRect(-4, HAUTEUR_FENETRE-hauteur_minimap-6, largeur_minimap-1, hauteur_minimap-1);
    rectangle_minimap->setZValue(51);

    map2 = new int* [largeur_map/TAILLE_MAP2];
    map_transition = new int*[largeur_map/TAILLE_MAP2];
    map_altitude = new int*[largeur_map/TAILLE_MAP2];

    for (int i=0; i<largeur_map/TAILLE_MAP2; i++){
        map2[i] = new int[hauteur_map/TAILLE_MAP2];
        map_transition[i] = new int[hauteur_map/TAILLE_MAP2];
        map_altitude[i] = new int[hauteur_map/TAILLE_MAP2];
    }

    for (int i=0; i<largeur_map/TAILLE_MAP2; i++){
        for (int j=0; j<hauteur_map/TAILLE_MAP2; j++){
            map2[i][j]=0;
            map_transition[i][j] = 0;
            map_altitude[i][j] = -1000; // A CHANGER
        }
    }

    for (int i=0; i<largeur_map; i++){
        for (int j=0; j<hauteur_map; j++){
            if (!map[i][j].mer){
                map2[i/TAILLE_MAP2][j/TAILLE_MAP2]=1;
            }

            if (map[i][j].altitude > map_altitude[i/TAILLE_MAP2][j/TAILLE_MAP2]){
                map_altitude[i/TAILLE_MAP2][j/TAILLE_MAP2] = map[i][j].altitude;
            }
        }
    }

    for (int i=0; i<largeur_map/TAILLE_MAP2; i++){
        for (int j=0; j<hauteur_map/TAILLE_MAP2; j++){
            if (map2[i][j] == 0 && ((i > 0 && map2[i-1][j] == 1) || (i < largeur_map/TAILLE_MAP2 -1 && map2[i+1][j] == 1) || (j < hauteur_map/TAILLE_MAP2 - 1 && map2[i][j+1] == 1) || (j > 0 && map2[i][j-1] == 1))){
                map_transition[i][j] = 1;
            }
        }
    }

    return 1;
}

void Fenetre::afficher_map (){
    /*int zoom2=zoom;

    if (pixmap_map.place==0)
    {
        QPixmap bla;

        pixmap_map.place=1;
        pixmap_map.chg=0;
        pixmap_map.pixmap->setPixmap(bla.fromImage(image_altitude2[zoom2]));
        pixmap_map.pixmap->setPos(-camera.x,-camera.y);
        scene->addItem(pixmap_map.pixmap);
        pixmap_map.pixmap->setZValue(-20);

    }
    if (pixmap_map.chg==1)
    {
        QPixmap bla;

        pixmap_map.chg=0;
        pixmap_map.pixmap->setPixmap(bla.fromImage(image_altitude2[zoom2]));
        pixmap_map.pixmap->setPos(-camera.x,-camera.y);
        pixmap_map.pixmap->setZValue(-20);

    }*/

    if (oldzoom != zoom){
       oldzoom = zoom;

       for (int i = 0; i < NOMBRE_ZOOM; i++){
           if (i != zoom){
               foreach (tile* value, image_map[i]){
                   value->pixmap->setVisible(false);
               }
           }
       }
    }

    /*
    if (!image_map[zoom].value(0)->place){
        for (unsigned int a = 0; a < largeur_map*valeur_zoom[zoom] / TAILLE_TILE2 + 1; a++){
            for (unsigned int b = 0; b < hauteur_map*valeur_zoom[zoom] / TAILLE_TILE2 + 1; b++){
                tile* t = image_map[zoom].value(a + CONSTANTE_LONG * b);

                t->place = 1;
                if (zoom == 5)
                    qDebug() << a << " - " << b;
                t->pixmap->setPixmap(QPixmap::fromImage(*t->image));
                t->pixmap->setPos(-camera.x + a * TAILLE_TILE2,-camera.y + b * TAILLE_TILE2);
                t->pixmap->setVisible(true);
                scene->addItem(t->pixmap);
                t->pixmap->setZValue(-20);
            }
        }
    }*/
    if (pixmap_map.chg){
        for (int a = 0; a < largeur_map*valeur_zoom[zoom] / TAILLE_TILE2 + 1; a++){
            for (int b = 0; b < hauteur_map*valeur_zoom[zoom] / TAILLE_TILE2 + 1; b++){
                tile* t = image_map[zoom].value(a + CONSTANTE_LONG * b);

                t->pixmap->setPos(-camera.x + a * TAILLE_TILE2,-camera.y + b * TAILLE_TILE2);
                t->pixmap->setVisible(true);
            }
        }
    }
}

bool Fenetre::afficher_unite ()
{
    qreal x=0.0;
    qreal y=0.0;

    QPen pen, pen2, pen3;
    QColor color, color3, color4;




    color.setRed(255);
    pen.setColor(color);
    pen.setStyle(Qt::DotLine);

    color3.setBlue(255);
    pen3.setColor(color3);
    pen3.setStyle(Qt::DotLine);

    color4.setRed(255);
    color4.setGreen(255);
    color4.setBlue(255);

    QColor couleur_nom_unite[5];
    couleur_nom_unite[0].setBlue(255);
    couleur_nom_unite[1].setRed(255);
    couleur_nom_unite[1].setGreen(71);
    couleur_nom_unite[1].setBlue(71);
    couleur_nom_unite[2].setRed(200);
    couleur_nom_unite[3].setGreen(255);
    couleur_nom_unite[4].setRed(171);
    couleur_nom_unite[4].setGreen(171);
    couleur_nom_unite[4].setBlue(171);

    QGraphicsLineItem * ligne;
    QGraphicsRectItem * rect;

    QPen pen_radar[NOMBRE_TYPE];

    color.setRed(0);
    color.setBlue(0);
    color.setGreen(0);
    pen_radar[0].setColor(color);

    color.setRed(0);
    color.setBlue(0);
    color.setGreen(255);
    pen_radar[1].setColor(color);
    pen_radar[2].setColor(color);

    color.setRed(0);
    color.setBlue(255);
    color.setGreen(0);
    pen_radar[3].setColor(color);

    for (unsigned int i=0; i<liste_ligne.size(); i++)
    {
        delete liste_ligne[i];
    }
    liste_ligne.clear();

    QGraphicsTextItem * texte;
    QGraphicsPixmapItem * pixmap;


    // A CHANGER
    texte = scene->addText(QString::number(camera.x + scene->x) + " , " + QString::number(camera.y + scene->y));
    texte->setPos(0, 680);
    liste_ligne.push_back(texte);

    QMap<int, doubleCoo> map_dimension_unite;
    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (true)
        {
            int k=0;
            if (mode==1) // jeu
            {
                if (liste_classe[liste_unite[i].type].pays!=joueur && liste_pays[joueur].relation[liste_classe[liste_unite[i].type].pays]!=1)
                {
                    if (liste_unite[i].est_detecte[joueur]==2)
                    {
                        if (liste_pays[joueur].relation[liste_classe[liste_unite[i].type].pays]==2)
                        {
                            k=2;
                        }
                        else
                        {
                            k=1;
                        }
                    }
                    else
                    {
                        if (liste_unite[i].est_detecte[joueur]==1)
                        {
                            k=1;
                        }
                        else
                        {
                            k=-1;
                        }
                    }


                }
                else
                {
                    if (liste_classe[liste_unite[i].type].pays==joueur)
                    {
                        k=0;
                    }
                    else
                    {
                        k=3;
                    }
                }
            }
            else
            {
                if (liste_classe[liste_unite[i].type].pays!=joueur)
                {
                    if (liste_pays[joueur].relation[liste_classe[liste_unite[i].type].pays]==2)
                    {
                        k=2;
                    }
                    else
                    {
                        if (liste_pays[joueur].relation[liste_classe[liste_unite[i].type].pays]==1)
                        {
                            k=3;
                        }
                        else
                        {
                            k=1;
                        }
                    }
                }
            }

            for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                liste_unite[i].liste_graphical_item[j].used = false;

                liste_unite[i].liste_graphical_item[j].item->setVisible(false);
            }

            if (k!=-1){
                int nb_affichage_total = 1;

                if (mode==1 && liste_classe[liste_unite[i].type].pays==joueur){
                    if (option.affichage_fuel && liste_classe[liste_unite[i].type].type != 5 && liste_classe[liste_unite[i].type].type != 1 && (liste_unite[i].fuel==0 || liste_unite[i].conso>0)){
                        nb_affichage_total++;
                    }
                    if (option.affichage_vitesse && liste_classe[liste_unite[i].type].type != 1 && liste_unite[i].vitesse>0){
                        nb_affichage_total++;
                    }
                    if (option.affichage_arme){
                        for (unsigned int k=0; k<NOMBRE_OBJET; k++){
                            if (liste_objet[k].type==0 && liste_unite[i].nombre_objet[k]>0){
                                nb_affichage_total++;
                            }
                        }
                    }
                }



                x=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-10;
                y=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-10;

                doubleCoo self;
                self.a.x = x - 2;
                self.a.y = y - 2;
                self.b.x = self.a.x + CONSTANTE_DIMENSION_TEXTE_X;
                self.b.y = self.a.y + max(nb_affichage_total * 10, 20);
                self.coo_pixmap.x = x;
                self.coo_pixmap.y = y;
                self.coo_texte.x = x + 21;
                self.coo_texte.y = y - 7 + nb_affichage_total * 10;
                self.ancien_k = k;

                double xx = x + 21;
                double yy = y - 7;

                double xxx = x;
                double yyy = y;

                int nb_affichage=0;
                bool found_neighbour = false;
                bool small = false;

                if (option.affichage_stack == 1){
                    for (unsigned int j = 0; j < liste_unite.size() && !found_neighbour; j++){
                        if (i != j && map_dimension_unite.contains(liste_unite[j].id)){
                            doubleCoo val = map_dimension_unite.value(liste_unite[j].id);

                            if ((val.a.x >= self.a.x && val.a.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y) || (val.b.x >= self.a.x && val.b.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y) || (val.b.x >= self.a.x && val.b.x <= self.b.x && val.b.y >= self.a.y && val.b.y <= self.b.y) || (val.a.x >= self.a.x && val.a.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y)    ||       (self.a.x >= val.a.x && self.a.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y) || (self.b.x >= val.a.x && self.b.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y) || (self.b.x >= val.a.x && self.b.x <= val.b.x && self.b.y >= val.a.y && self.b.y <= val.b.y) || (self.a.x >= val.a.x && self.a.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y)){
                                found_neighbour = true;

                                xx = val.coo_texte.x;
                                yy = val.coo_texte.y;
                                xxx = val.coo_pixmap.x;
                                yyy = val.coo_pixmap.y + 20;

                                val.b.y += nb_affichage_total * 10;
                                val.coo_texte.y += nb_affichage_total * 10;
                                val.coo_pixmap.y += 20;

                                map_dimension_unite.insert(liste_unite[j].id, val);
                            }
                        }
                    }

                    if (!found_neighbour){
                        map_dimension_unite.insert(liste_unite[i].id, self);
                    }
                }
                else if (option.affichage_stack == 2){
                    for (unsigned int j = 0; j < liste_unite.size() && !small; j++){
                        if (i != j && map_dimension_unite.contains(liste_unite[j].id)){
                            doubleCoo val = map_dimension_unite.value(liste_unite[j].id);

                            if ((val.a.x >= self.a.x && val.a.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y) || (val.b.x >= self.a.x && val.b.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y) || (val.b.x >= self.a.x && val.b.x <= self.b.x && val.b.y >= self.a.y && val.b.y <= self.b.y) || (val.a.x >= self.a.x && val.a.x <= self.b.x && val.a.y >= self.a.y && val.a.y <= self.b.y)    ||       (self.a.x >= val.a.x && self.a.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y) || (self.b.x >= val.a.x && self.b.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y) || (self.b.x >= val.a.x && self.b.x <= val.b.x && self.b.y >= val.a.y && self.b.y <= val.b.y) || (self.a.x >= val.a.x && self.a.x <= val.b.x && self.a.y >= val.a.y && self.a.y <= val.b.y)){
                                small = true;

                                val.a.x = liste_unite[j].position.x*valeur_zoom[zoom]/echelle-camera.x-5;
                                val.a.y = liste_unite[j].position.y*valeur_zoom[zoom]/echelle-camera.y-5;

                                val.b.x = liste_unite[j].position.x*valeur_zoom[zoom]/echelle-camera.x+5;
                                val.b.y = liste_unite[j].position.y*valeur_zoom[zoom]/echelle-camera.y+5;

                                for (int l = 0; l < liste_unite[j].liste_graphical_item.size(); l++){
                                    if (liste_unite[j].liste_graphical_item[l].type != 0 && liste_unite[j].liste_graphical_item[l].type != 5 && liste_unite[j].liste_graphical_item[l].type != 6 && liste_unite[j].liste_graphical_item[l].type != 7){
                                        liste_unite[j].liste_graphical_item[l].item->setVisible(false);
                                    }
                                    else if (liste_unite[j].liste_graphical_item[l].type == 0){ // petite ligne
                                        ((QGraphicsLineItem*)liste_unite[j].liste_graphical_item[l].item)->setLine(liste_unite[j].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[j].position.y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[j].position.x*valeur_zoom[zoom]/echelle-camera.x+15*cos(liste_unite[j].angle),liste_unite[j].position.y*valeur_zoom[zoom]/echelle-camera.y+15*sin(liste_unite[j].angle));
                                    }
                                }

                                liste_unite[j].pixmap->setPixmap(liste_classe[liste_unite[j].type].small_image[val.ancien_k]);
                                liste_unite[j].pixmap->setPos(liste_unite[j].position.x*valeur_zoom[zoom]/echelle-camera.x-5, liste_unite[j].position.y*valeur_zoom[zoom]/echelle-camera.y-5);

                                xxx = liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-5;
                                yyy = liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-5;

                                map_dimension_unite.insert(liste_unite[j].id, val);

                                self.a.x = liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-5;
                                self.a.y = liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-5;

                                self.b.x = liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x+5;
                                self.b.y = liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y+5;
                            }
                        }
                    }

                    map_dimension_unite.insert(liste_unite[i].id, self);
                }

                if (!liste_unite[i].place){
                   scene->addItem(liste_unite[i].pixmap);
                   liste_unite[i].pixmap->setZValue(-10);
                   liste_unite[i].pixmap->setVisible(true);
                   liste_unite[i].pixmap->setPos(xxx,yyy);
                   liste_unite[i].place=true;
                }

                if (true || liste_unite[i].chg || chg_camera){
                    liste_unite[i].pixmap->setVisible(true);
                    liste_unite[i].pixmap->setPos(xxx,yyy);

                    liste_unite[i].chg=false;
                }

                liste_unite[i].pixmap->setVisible(!found_neighbour);

                if (true || liste_unite[i].chg_pixmap){ //  A CHANGER
                    if (small){
                        liste_unite[i].pixmap->setPixmap(liste_classe[liste_unite[i].type].small_image[k]);
                    }
                    else {
                        liste_unite[i].pixmap->setPixmap(*liste_classe[liste_unite[i].type].image[k]);
                    }


                    liste_unite[i].chg_pixmap = false;
                }





                if (k!=1){

                    bool found = false;
                    if (!found_neighbour){

                        for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                            if (liste_unite[i].liste_graphical_item[j].type == 0){ // petite ligne
                                if (small){
                                    ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setLine(x+10,y+10,x+10+15*cos(liste_unite[i].angle),y+10+15*sin(liste_unite[i].angle));
                                }
                                else {
                                    ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setLine(x+10,y+10,x+10+30*cos(liste_unite[i].angle),y+10+30*sin(liste_unite[i].angle));
                                }
                                ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                                found = true;
                            }
                        }

                        if (!found){
                            if (small){
                                ligne=scene->addLine(x+10,y+10,x+10+15*cos(liste_unite[i].angle),y+10+15*sin(liste_unite[i].angle) ,pen2);  // petite ligne
                            }
                            else {
                                ligne=scene->addLine(x+10,y+10,x+10+30*cos(liste_unite[i].angle),y+10+30*sin(liste_unite[i].angle) ,pen2);  // petite ligne
                            }
                            GItem g;
                            g.type = 0;
                            g.item = ligne;


                            liste_unite[i].liste_graphical_item.push_back(g);
                        }
                    }
                    else {
                        for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                            if (liste_unite[i].liste_graphical_item[j].type == 0){ // petite ligne
                                ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(false);
                            }
                        }
                    }

                    if (!small){
                        found = false;
                        for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                            if (liste_unite[i].liste_graphical_item[j].type == 1){ // nom
                                ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setPlainText(liste_classe[liste_unite[i].type].nom);
                                ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setX(xx);
                                ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setY(yy+nb_affichage*10);
                                ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                                ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setDefaultTextColor(couleur_nom_unite[k]);
                                nb_affichage++;
                                found = true;
                            }
                        }

                        if (!found){
                            texte=scene->addText(liste_classe[liste_unite[i].type].nom); // nom
                            texte->setX(xx);
                            texte->setY(yy+nb_affichage*10);
                            texte->setDefaultTextColor(couleur_nom_unite[k]);
                            nb_affichage++;
                            GItem g;
                            g.type = 1;
                            g.item = texte;

                            liste_unite[i].liste_graphical_item.push_back(g);
                        }
                    }



                    if (mode==1 && liste_classe[liste_unite[i].type].pays==joueur && !small)  // affichage options + radar
                    {

                        if (option.affichage_fuel && liste_classe[liste_unite[i].type].type != 5 && liste_classe[liste_unite[i].type].type != 1){ // si infanterie ou missile, pas affichage fuel

                            if (liste_unite[i].fuel==0 || liste_unite[i].conso>0){
                                found = false;
                                QString tex;
                                if (liste_unite[i].fuel==0){
                                    tex = "Fuel left : none";
                                }
                                else {
                                    tex = "Fuel left : " + time_to_string(liste_unite[i].fuel/liste_unite[i].conso);
                                }

                                for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                                    if (liste_unite[i].liste_graphical_item[j].type == 2){ // fuel
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setPlainText(tex);
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setX(xx);
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setY(yy+nb_affichage*10);
                                        nb_affichage++;
                                        found = true;
                                    }
                                }

                                if (!found){
                                    texte=scene->addText(tex); // fuel
                                    texte->setX(xx);
                                    texte->setY(yy+nb_affichage*10);
                                    GItem g;
                                    g.type = 2;
                                    g.item = texte;

                                    nb_affichage++;

                                    liste_unite[i].liste_graphical_item.push_back(g);
                                }
                            }
                        }
                        if (option.affichage_vitesse && liste_classe[liste_unite[i].type].type != 1)
                        {
                            if (liste_unite[i].vitesse>0){
                                found = false;

                                for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                                    if (liste_unite[i].liste_graphical_item[j].type == 3){ // vitesse
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setPlainText("Speed : " + QString::number((int)liste_unite[i].vitesse) + " m/s");
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setX(xx);
                                        ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setY(yy+nb_affichage*10);
                                        nb_affichage++;
                                        found = true;
                                    }
                                }

                                if (!found){
                                    texte=scene->addText("Speed : " + QString::number(liste_unite[i].vitesse) + " m/s"); // vitesse
                                    texte->setX(xx);
                                    texte->setY(yy+nb_affichage*10);
                                    GItem g;
                                    g.type = 3;
                                    g.item = texte;

                                    nb_affichage++;

                                    liste_unite[i].liste_graphical_item.push_back(g);
                                }
                            }
                        }
                        if (option.affichage_arme){
                            for (unsigned int k=0; k<NOMBRE_OBJET; k++)
                            {
                                if (liste_objet[k].type==0 && liste_unite[i].nombre_objet[k]>0)
                                {
                                    found = false;

                                    for (int j = 0; j < liste_unite[i].liste_graphical_item.size() && !found; j++){
                                        if (liste_unite[i].liste_graphical_item[j].type == 4 && !liste_unite[i].liste_graphical_item[j].used){ // armes
                                            ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                                            ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setPlainText(liste_objet[k].nom + " : " + QString::number(liste_unite[i].nombre_objet[k]));
                                            ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setX(xx);
                                            ((QGraphicsTextItem*)liste_unite[i].liste_graphical_item[j].item)->setY(yy+nb_affichage*10);
                                            nb_affichage++;
                                            found = true;
                                            liste_unite[i].liste_graphical_item[j].used = true;
                                        }
                                    }

                                    if (!found){
                                        texte=scene->addText(liste_objet[k].nom + " : " + QString::number(liste_unite[i].nombre_objet[k])); // armes
                                        texte->setX(xx);
                                        texte->setY(yy+nb_affichage*10);
                                        GItem g;
                                        g.type = 4;
                                        g.item = texte;
                                        g.used = true;

                                        nb_affichage++;

                                        liste_unite[i].liste_graphical_item.push_back(g);
                                    }
                                }
                            }
                        }


                        /*for (unsigned int k=0; k<NOMBRE_TYPE; k++)
                        {
                            if (liste_unite[i].portee_radar[k]>0)
                            {
                                ellipse=scene->addEllipse(x-liste_unite[i].portee_radar[k]*valeur_zoom[zoom]/(echelle)+10,y-liste_unite[i].portee_radar[k]*valeur_zoom[zoom]/(echelle)+10,liste_unite[i].portee_radar[k]*2*valeur_zoom[zoom]/echelle,liste_unite[i].portee_radar[k]*2*valeur_zoom[zoom]/echelle,pen_radar[k]);
                                liste_ligne.push_back(ellipse);
                            }
                        }*/
                    }
                }

                if (liste_unite[i].en_deplacement && est_selectionne(liste_unite[i].id))  // lignes deplacement
                {
                    /*if (false && liste_classe[liste_unite[i].type].type==0 || liste_classe[liste_unite[i].type].type==3) // A CHANGER
                    {
                        if (liste_unite[i].liste_deplacement2.size()>0)
                        {
                            ligne=scene->addLine(liste_unite[i].liste_deplacement2[0].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement2[0].y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y ,pen2);  // ligne position-liste
                            liste_ligne.push_back(ligne);
                        }

                        for (int j=1; j<liste_unite[i].liste_deplacement2.size(); j++)
                        {
                            ligne=scene->addLine(liste_unite[i].liste_deplacement2[j-1].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement2[j-1].y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[i].liste_deplacement2[j].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement2[j].y*valeur_zoom[zoom]/echelle-camera.y ,pen2);  // ligne liste-liste
                            liste_ligne.push_back(ligne);
                        }
                    }
                    else*/ if (true)
                    {
                        x=liste_unite[i].destination.x*valeur_zoom[zoom]/echelle-camera.x-5;
                        y=liste_unite[i].destination.y*valeur_zoom[zoom]/echelle-camera.y-5;

                        ligne=scene->addLine(liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y,x+5,y+5 ,pen2);  // ligne unite-premier destination
                        liste_ligne.push_back(ligne);

                        if (liste_unite[i].liste_deplacement.size()>0)
                        {
                            ligne=scene->addLine(liste_unite[i].liste_deplacement[0].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement[0].y*valeur_zoom[zoom]/echelle-camera.y,x+5,y+5 ,pen2);  // ligne destination-liste
                            liste_ligne.push_back(ligne);
                        }

                        for (int j=1; j<liste_unite[i].liste_deplacement.size(); j++)
                        {
                            ligne=scene->addLine(liste_unite[i].liste_deplacement[j-1].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement[j-1].y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[i].liste_deplacement[j].x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].liste_deplacement[j].y*valeur_zoom[zoom]/echelle-camera.y ,pen2);  // ligne liste-liste
                            liste_ligne.push_back(ligne);
                        }
                    }

                }

                if (index_unite(liste_unite[i].cible)!=-1 && est_selectionne(liste_unite[i].id)){

                    bool found = false;
                    for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                        if (liste_unite[i].liste_graphical_item[j].type == 5){ // ligne rouge
                            ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setLine(liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[index_unite(liste_unite[i].cible)].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[index_unite(liste_unite[i].cible)].position.y*valeur_zoom[zoom]/echelle-camera.y);
                            ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                            found = true;
                        }
                    }

                    if (!found){
                        ligne=scene->addLine(liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[index_unite(liste_unite[i].cible)].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[index_unite(liste_unite[i].cible)].position.y*valeur_zoom[zoom]/echelle-camera.y ,pen);  // ligne rouge
                        GItem g;
                        g.type = 5;
                        g.item = ligne;

                        liste_unite[i].liste_graphical_item.push_back(g);
                    }
                }
                if (index_unite(liste_unite[i].cible_ami)!=-1 && est_selectionne(liste_unite[i].id)){
                    bool found = false;
                    for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                        if (liste_unite[i].liste_graphical_item[j].type == 6){ // ligne bleue
                            ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setLine(liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[index_unite(liste_unite[i].cible_ami)].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[index_unite(liste_unite[i].cible_ami)].position.y*valeur_zoom[zoom]/echelle-camera.y);
                            ((QGraphicsLineItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                            found = true;
                        }
                    }

                    if (!found){
                        ligne=scene->addLine(liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y,liste_unite[index_unite(liste_unite[i].cible_ami)].position.x*valeur_zoom[zoom]/echelle-camera.x,liste_unite[index_unite(liste_unite[i].cible_ami)].position.y*valeur_zoom[zoom]/echelle-camera.y ,pen3);  // ligne bleue
                        GItem g;
                        g.type = 6;
                        g.item = ligne;

                        liste_unite[i].liste_graphical_item.push_back(g);
                    }
                }

                bool found = false;
                for (int j = 0; j < liste_unite[i].liste_graphical_item.size(); j++){
                    if (liste_unite[i].liste_graphical_item[j].type == 7){ // point minimap
                        ((QGraphicsRectItem*)liste_unite[i].liste_graphical_item[j].item)->setPen(QPen(couleur_nom_unite[k]));
                        ((QGraphicsRectItem*)liste_unite[i].liste_graphical_item[j].item)->setRect(pixmap_minimap->pos().x() + liste_unite[i].position.x * largeur_minimap/(echelle * largeur_map),pixmap_minimap->pos().y() + liste_unite[i].position.y * largeur_minimap/(echelle * largeur_map), 20 * largeur_minimap/ largeur_map, 20 * hauteur_minimap/ hauteur_map);
                        ((QGraphicsRectItem*)liste_unite[i].liste_graphical_item[j].item)->setVisible(true);
                        found = true;
                    }
                }

                if (!found){
                    rect=scene->addRect(pixmap_minimap->pos().x() + liste_unite[i].position.x * largeur_minimap/(echelle * largeur_map),pixmap_minimap->pos().y() + liste_unite[i].position.y * largeur_minimap/(echelle * largeur_map), 20 * largeur_minimap/ largeur_map, 20 * hauteur_minimap/ hauteur_map,QPen(couleur_nom_unite[k]));  // point minimap
                    rect->setZValue(51);
                    GItem g;
                    g.type = 7;
                    g.item = rect;

                    liste_unite[i].liste_graphical_item.push_back(g);
                }
            }
            else
            {
                liste_unite[i].pixmap->setVisible(false);
            }

        }
    }

    if (select_rect) // rectangle selection
    {
        if (position_rect.x<position_rect2.x)
        {
            if (position_rect.y<position_rect2.y)
            {
                rect=scene->addRect(position_rect.x, position_rect.y, position_rect2.x-position_rect.x, position_rect2.y-position_rect.y);
                liste_ligne.push_back(rect);
            }
            else
            {
                rect=scene->addRect(position_rect.x, position_rect2.y, position_rect2.x-position_rect.x, position_rect.y-position_rect2.y);
                liste_ligne.push_back(rect);
            }
        }
        else
        {
            if (position_rect.y<position_rect2.y)
            {
                rect=scene->addRect(position_rect2.x, position_rect.y, position_rect.x-position_rect2.x, position_rect2.y-position_rect.y);
                liste_ligne.push_back(rect);
            }
            else
            {
                rect=scene->addRect(position_rect2.x, position_rect2.y, position_rect.x-position_rect2.x, position_rect.y-position_rect2.y);
                liste_ligne.push_back(rect);
            }
        }

    }

    rect = scene->addRect(pixmap_minimap->pos().x() + camera.x * largeur_minimap/ (largeur_map * valeur_zoom[zoom]),pixmap_minimap->pos().y() + camera.y * hauteur_minimap/ (hauteur_map * valeur_zoom[zoom]), LARGEUR_FENETRE * largeur_minimap/ (largeur_map * valeur_zoom[zoom]) - 1, HAUTEUR_FENETRE * hauteur_minimap/ (hauteur_map * valeur_zoom[zoom]));
    rect->setZValue(52);
    liste_ligne.push_back(rect);

    if (mode==1 && bouton_pause!=0)
    {
        if (pause)
        {
            bouton_pause->setText("Simulation suspended");
        }
        else
        {
            bouton_pause->setText(QString::number(vitesse_jeu) + " X ");
        }
    }

    for (unsigned int i=1; i<=(largeur_map*echelle)/100000; i++)
    {
        for (unsigned int j=1; j<(hauteur_map*echelle)/100000; j++)
        {
            long key = i + j * CONSTANTE_LONG;
            pixmap = image_croix.value(key, NULL);

            if (pixmap == NULL){
                pixmap=scene->addPixmap(*image_divers[1]);
                x=i*100000*valeur_zoom[zoom]/echelle-camera.x-1;
                y=j*100000*valeur_zoom[zoom]/echelle-camera.y-1;
                pixmap->setPos(x,y);
                image_croix.insert(key, pixmap);
            }
            else {
                x=i*100000*valeur_zoom[zoom]/echelle-camera.x-1;
                y=j*100000*valeur_zoom[zoom]/echelle-camera.y-1;
                pixmap->setPos(x,y);
            }
        }
    }
}

void Fenetre::keyPressEvent(QKeyEvent* event)
{

    switch(event->key())
    {
        case Qt::Key_A:
            a_appuye=1;
            event->accept();
        break;
        case Qt::Key_D:
            s_appuye=1;
            event->accept();
        break;
        case Qt::Key_W:
            w_appuye=1;
            event->accept();
        break;
        case Qt::Key_S:
            y_appuye=1;
            event->accept();
        break;
        case Qt::Key_Shift:
            shift_appuye=1;
            event->accept();
        break;
        case Qt::Key_Control:
            ctrl_appuye=1;
            event->accept();
        break;
        case Qt::Key_Escape:
            esc_appuye=1;
            event->accept();
        break;
        case Qt::Key_Return:
            entree_appuye=1;
            event->accept();
        break;
        case Qt::Key_Plus:
            if (vitesse_jeu<50)
            {
                vitesse_jeu++;
            }
            event->accept();
        break;
        case Qt::Key_Minus:
            if (vitesse_jeu>1)
            {
                vitesse_jeu--;
            }
            event->accept();
        break;
        case Qt::Key_Space:
            if (!pause)
            {
                pause=1;
            }
            else
            {
                pause=0;
            }
            event->accept();
        break;
        case Qt::Key_0:
            chiffre_appuye[0]=true;
            event->accept();
        break;
        case Qt::Key_1:
            chiffre_appuye[1]=true;
            event->accept();
        break;
        case Qt::Key_2:
            chiffre_appuye[2]=true;
            event->accept();
        break;
        case Qt::Key_3:
            chiffre_appuye[3]=true;
            event->accept();
        break;
        case Qt::Key_4:
            chiffre_appuye[4]=true;
            event->accept();
        break;
        case Qt::Key_5:
            chiffre_appuye[5]=true;
            event->accept();
        break;
        case Qt::Key_6:
            chiffre_appuye[6]=true;
            event->accept();
        break;
        case Qt::Key_7:
            chiffre_appuye[7]=true;
            event->accept();
        break;
        case Qt::Key_8:
            chiffre_appuye[8]=true;
            event->accept();
        break;
        case Qt::Key_9:
            chiffre_appuye[9]=true;
            event->accept();
        break;
        default:
            event->ignore();
        break;
    }
}

void Fenetre::keyReleaseEvent(QKeyEvent* event)
{
    QPixmap p;
    switch(event->key())
    {
        case Qt::Key_A:
            a_appuye=0;
            event->accept();
        break;
        case Qt::Key_D:
            s_appuye=0;
            event->accept();
        break;
        case Qt::Key_W:
            w_appuye=0;
        break;
        case Qt::Key_S:
            y_appuye=0;
            event->accept();
        break;
        case Qt::Key_Shift:
            shift_appuye=0;
            event->accept();
        break;
        case Qt::Key_Control:
            ctrl_appuye=0;
            event->accept();
        break;
        case Qt::Key_Escape:
            esc_appuye=0;
            //sauvegarde_ia(); // A CHANGER
            event->accept();
        break;
        case Qt::Key_0:
            chiffre_appuye[0]=false;
            event->accept();
        break;
        case Qt::Key_1:
            chiffre_appuye[1]=false;
            event->accept();
        break;
        case Qt::Key_2:
            chiffre_appuye[2]=false;
            event->accept();
        break;
        case Qt::Key_3:
            chiffre_appuye[3]=false;
            event->accept();
        break;
        case Qt::Key_4:
            chiffre_appuye[4]=false;
            event->accept();
        break;
        case Qt::Key_5:
            chiffre_appuye[5]=false;
            event->accept();
        break;
        case Qt::Key_6:
            chiffre_appuye[6]=false;
            event->accept();
        break;
        case Qt::Key_7:
            chiffre_appuye[7]=false;
            event->accept();
        break;
        case Qt::Key_8:
            chiffre_appuye[8]=false;
            event->accept();
        break;
        case Qt::Key_9:
            chiffre_appuye[9]=false;
            event->accept();
        break;
        default:
            event->ignore();
        break;
    }
}

void Fenetre::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
            gauche_appuye=1;
            event->accept();
            if (mode == 1)
                event_souris_gauche_jeu();
        break;
        case Qt::RightButton:
            droite_appuye=1;
            event->accept();
        break;
        case Qt::MidButton:
            mid_appuye=1;
            position_deplacement_mid.x = scene->x;
            position_deplacement_mid.y = scene->y;
            event->accept();
        break;
        default:
            event->ignore();
        break;
    }
}

void Fenetre::mouseReleaseEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
            gauche_appuye = 0;
            if (mode == 1){
                event->accept();
                release_selection();
            }
            else if (mode == 2){
                event_souris_gauche_editeur();
                event->accept();
            }
        break;
        case Qt::RightButton:
            event->accept();
            droite_appuye=0;
        break;
        case Qt::MidButton:
            mid_appuye=0;
            event->accept();
        break;
        default:
            event->ignore();
        break;
    }
}

void Fenetre::wheelEvent (QWheelEvent* event)
{
    if (mode != 1 && mode !=2){
        return;
    }
    if (event->delta()>0)
    {
        if (zoom<NOMBRE_ZOOM-1)
        {

            camera.x=((camera.x+scene->x)*valeur_zoom[zoom+1])/valeur_zoom[zoom]-scene->x;
            camera.y=((camera.y+scene->y)*valeur_zoom[zoom+1])/valeur_zoom[zoom]-scene->y;

            zoom++;
            pixmap_map.chg=1;

            if (camera.x<10)
            {
                camera.x=10;
            }
            if (camera.y<10)
            {
                camera.y=10;
            }

            if (camera.x>largeur_map*valeur_zoom[zoom]-scene->width()-5)
            {
                camera.x=largeur_map*valeur_zoom[zoom]-scene->width()-5;
            }
            if (camera.y>hauteur_map*valeur_zoom[zoom]-scene->height()-5)
            {
                camera.y=hauteur_map*valeur_zoom[zoom]-scene->height()-5;
            }

            chg_camera=1;
        }
    }
    else
    {
        if (zoom>0)
        {

            camera.x=((camera.x+scene->x)*valeur_zoom[zoom-1])/valeur_zoom[zoom]-scene->x;
            camera.y=((camera.y+scene->y)*valeur_zoom[zoom-1])/valeur_zoom[zoom]-scene->y;

            zoom--;
            pixmap_map.chg=1;

            if (camera.x<10)
            {
                camera.x=10;
            }
            if (camera.y<10)
            {
                camera.y=10;
            }

            if (camera.x>largeur_map*valeur_zoom[zoom]-scene->width()-10)
            {
                camera.x=largeur_map*valeur_zoom[zoom]-scene->width()-10;
            }
            if (camera.y>hauteur_map*valeur_zoom[zoom]-scene->height()-10)
            {
                camera.y=hauteur_map*valeur_zoom[zoom]-scene->height()-10;
            }

            chg_camera=1;
        }
    }
}

void Fenetre::update(){

    if (sec!=time(NULL))
    {
        if (fps!=0)
        {
            fps_precedent=fps;
        }
        else {
            fps_precedent = 1;
            erreur_log("Zero fps");
        }
        fps=0;
        sec=time(NULL);

        temps++;
    }
    else
    {
        fps++;
    }

    if (mode==0){
        bouton_jeu->move(LARGEUR_FENETRE/2 - 1.25 * bouton_jeu->width(), HAUTEUR_FENETRE - 6 * bouton_jeu->height());
        bouton_editeur->move(LARGEUR_FENETRE/2 + 0.25 * bouton_editeur->width(), HAUTEUR_FENETRE - 6 * bouton_editeur->height());
        bouton_generateur->move(LARGEUR_FENETRE/2 - 1.25 * bouton_generateur->width(), HAUTEUR_FENETRE - 4 * bouton_generateur->height());
        bouton_option->move(LARGEUR_FENETRE/2 + 0.25 * bouton_option->width(), HAUTEUR_FENETRE - 4 * bouton_option->height());
        bouton_quit->move(LARGEUR_FENETRE/2 - bouton_quit->width()/2, HAUTEUR_FENETRE - 2 * bouton_quit->height());
    }
    else if (mode==1){
        if (esc_appuye)
        {
            event_echap();
        }
        check_groupe();
        if (!pause)
        {
            check_lancement ();
            check_module_unite ();
            check_projectile ();
            check_deplacement ();
            check_autodef();
            check_engaging();
            check_unite_selectionnee();
            check_info_texte();
            check_dead();


            if (fps==0)
            {
                check_radar();
                check_sat();
                check_fail();
                check_script();

                if (temps%10 == 0){
                    check_ia();
                }

                //check_ia();
            }
        }

        release_souris_jeu();
        mouvement_souris_jeu();
        double_clic_souris_jeu();
        if (a_appuye)
        {
            event_gauche();
        }
        if (s_appuye)
        {
            event_droite();
        }
        if (w_appuye)
        {
            event_haut();
        }
        if (y_appuye)
        {
            event_bas();
        }
        if (entree_appuye)
        {
            event_entree();
        }
        if (gauche_appuye)
        {

        }
        if (droite_appuye)
        {
            event_souris_droite_jeu();
            droite_appuye=0;
        }

        afficher_map();
        afficher_unite ();

        chg_camera=0;
    }
    if (mode==2)
    {
        if (esc_appuye)
        {
            event_echap();
        }
        release_souris_editeur();
        mouvement_souris_editeur();
        double_clic_souris_editeur();
        if (a_appuye)
        {
            event_gauche();
        }
        if (s_appuye)
        {
            event_droite();
        }
        if (w_appuye)
        {
            event_haut();
        }
        if (y_appuye)
        {
            event_bas();
        }
        if (gauche_appuye)
        {
            //event_souris_gauche_editeur();
        }
        if (droite_appuye)
        {
            event_souris_droite_editeur();
        }

        afficher_map();
        afficher_unite ();

        chg_camera=0;
    }
    if (mode==3)
    {
        if (esc_appuye)
        {
            event_echap();
        }
        if (a_appuye)
        {
            event_gauche();
        }
        if (s_appuye)
        {
            event_droite();
        }
        if (w_appuye)
        {
            event_haut();
        }
        if (y_appuye)
        {
            event_bas();
        }

        afficher_map();
        afficher_unite();

        chg_camera=0;
    }
}

void Fenetre::event_gauche()
{
    if (camera.x>=25)
    {
       camera.x-=20;
       pixmap_map.chg=1;
       chg_camera=1;
    }
}

void Fenetre::event_droite()
{
    if (camera.x+scene->width()<largeur_map*valeur_zoom[zoom]-25)
    {
       camera.x+=20;
       pixmap_map.chg=1;
       chg_camera=1;
    }
}

void Fenetre::event_haut()
{
    if (camera.y>=25)
    {
       camera.y-=20;
       pixmap_map.chg=1;
       chg_camera=1;
    }
}

void Fenetre::event_bas()
{
    if (camera.y+scene->height()<hauteur_map*valeur_zoom[zoom]-25)
    {
       camera.y+=20;
       pixmap_map.chg=1;
       chg_camera=1;
    }
}

void Fenetre::event_souris_droite_jeu()
{
    qreal x=scene->x;
    qreal y=scene->y;
    bool pas_bouge=0;

    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (x>=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-10 && x<=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x+10 && y>=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-10 && y<=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y+10)
        {
            if (joueur!=liste_classe[liste_unite[i].type].pays && liste_pays[joueur].relation[liste_classe[liste_unite[i].type].pays]==2)
            {
                pas_bouge=1;
                for (int j=0; j<liste_selection.size(); j++)
                {
                    int k = index_unite(liste_selection[j]);
                    if (k!=-1)
                    {
                        liste_unite[k].cible=liste_unite[i].id;
                    }
                }
            }
            if (joueur==liste_classe[liste_unite[i].type].pays)
            {
                pas_bouge=1;
                for (int j=0; j<liste_selection.size(); j++)
                {
                    int k = index_unite(liste_selection[j]);
                    if (k!=-1 && !liste_unite[k].atterrissage)
                    {
                        liste_unite[k].cible_ami=liste_unite[i].id;
                    }
                }
            }
        }
    }
    if (!pas_bouge)
    {
        for (int i=0; i<liste_selection.size(); i++)
        {
            int j=index_unite(liste_selection[i]);
            if (j!=-1)
            {
                if (liste_classe[liste_unite[j].type].type!=7 && liste_classe[liste_unite[j].type].type!=8){

                    coo pos;
                    pos.x = (x+camera.x)*echelle/valeur_zoom[zoom];
                    pos.y = (y+camera.y)*echelle/valeur_zoom[zoom];

                    deplacement_unite(j, pos, shift_appuye, true);



                    /*else
                    {
                        DAVector <coo>liste=mouvement(depart, arrivee, map2, largeur_map/TAILLE_MAP2, hauteur_map/TAILLE_MAP2, liste_classe[liste_unite[j].type].type);


                        DAVector <coo>liste2;


                        coo current = depart;
                        //liste2.push_back(depart);
                        int longest = 0;
                        for (int k = 0; k < liste.size(); k++){
                            int hau = hauteur_max(current, liste[k], map_altitude, largeur_map/TAILLE_MAP2, hauteur_map/TAILLE_MAP2, liste_classe[liste_unite[j].type].type);
                            if (hau < 0){
                                longest = k;

                                if (k == liste.size()-1 && k != 0){
                                    liste2.push_back(liste[k]);
                                }
                            }
                            else {
                                liste2.push_back(liste[longest]);
                                current = liste[longest];
                                longest = k;
                            }
                        }

                        for (int k = 0; k < liste2.size(); k++){
                            //qDebug() << liste2[k].x << " , " << liste2[k].y;
                        }

                        //liste = liste2;



                    }*/



                }
                else if (liste_classe[liste_unite[j].type].type==1 || liste_classe[liste_unite[j].type].type==2)
                {
                    liste_unite[j].destination.x=(x+camera.x)*echelle/valeur_zoom[zoom];
                    liste_unite[j].destination.y=(y+camera.y)*echelle/valeur_zoom[zoom];

                    liste_unite[j].en_deplacement=1;
                    liste_unite[j].chg=1;
                }



            }
        }
    }



}

void Fenetre::event_souris_gauche_jeu()
{
    qreal x=scene->x;
    qreal y=scene->y;

    if (x >= pixmap_minimap->pos().x() && y >= pixmap_minimap->pos().y() && x < pixmap_minimap->pos().x() + largeur_minimap && y < pixmap_minimap->pos().y() + hauteur_minimap){
        select_minimap = true;
    }
    else {
        select_rect=1;

        position_rect.x=x;
        position_rect.y=y;
        position_rect2.x=x;
        position_rect2.y=y;
    }
}

void Fenetre::event_souris_mid_jeu(){
    qreal x=scene->x;
    qreal y=scene->y;

    position_deplacement_mid.x = x;
    position_deplacement_mid.y = y;
}

void Fenetre::mouvement_souris_jeu()
{
    qreal x=scene->x;
    qreal y=scene->y;

    if (select_rect){
        position_rect2.x=x;
        position_rect2.y=y;
    }
    else if (mid_appuye){
        camera.x += position_deplacement_mid.x-scene->x;
        camera.y += position_deplacement_mid.y-scene->y;

        position_deplacement_mid.x = scene->x;
        position_deplacement_mid.y = scene->y;

        if (camera.x<10)
        {
            camera.x=10;
        }
        if (camera.y<10)
        {
            camera.y=10;
        }

        if (camera.x>largeur_map*valeur_zoom[zoom]-scene->width()-5)
        {
            camera.x=largeur_map*valeur_zoom[zoom]-scene->width()-5;
        }
        if (camera.y>hauteur_map*valeur_zoom[zoom]-scene->height()-5)
        {
            camera.y=hauteur_map*valeur_zoom[zoom]-scene->height()-5;
        }

        pixmap_map.chg=1;
        chg_camera=1;
    }
    else if (select_minimap){
        int xx = x - pixmap_minimap->x();
        int yy = y - pixmap_minimap->y();

        camera.x = xx * largeur_map * valeur_zoom[zoom]/ largeur_minimap - LARGEUR_FENETRE / 2;
        camera.y = yy * hauteur_map * valeur_zoom[zoom]/ hauteur_minimap - HAUTEUR_FENETRE / 2;

        if (camera.x<10)
        {
            camera.x=10;
        }
        if (camera.y<10)
        {
            camera.y=10;
        }

        if (camera.x>largeur_map*valeur_zoom[zoom]-scene->width()-5)
        {
            camera.x=largeur_map*valeur_zoom[zoom]-scene->width()-5;
        }
        if (camera.y>hauteur_map*valeur_zoom[zoom]-scene->height()-5)
        {
            camera.y=hauteur_map*valeur_zoom[zoom]-scene->height()-5;
        }

        pixmap_map.chg=1;
        chg_camera=1;
    }

}

void Fenetre::release_souris_jeu()
{
    if (scene->release==1)
    {

    }
}

void Fenetre::double_clic_souris_jeu()
{
    if (scene->dbl==1)
    {
        scene->dbl=0;


    }
}

void Fenetre::mouvement_souris_editeur()
{
    qreal x=scene->x;
    qreal y=scene->y;

    if (droite_appuye)
    {

        if (liste_selection.size()>0)
        {
            int j=index_unite(liste_selection[0]);
            if (j!=-1)
            {
                int xx=((x+camera.x)/valeur_zoom[zoom])/TAILLE_MAP2;
                int yy=((y+camera.y)/valeur_zoom[zoom])/TAILLE_MAP2;
                int type = liste_classe[liste_unite[j].type].type;
                if ((map2[xx][yy]==0 && (type == 0 || type == 3)) || (map2[xx][yy]==1 && (type == 5)))
                {
                    liste_unite[j].position.x = (x+camera.x)*echelle/valeur_zoom[zoom];
                    liste_unite[j].position.y = (y+camera.y)*echelle/valeur_zoom[zoom];

                    liste_unite[j].chg = true;
                }
            }
        }
    }
}

void Fenetre::release_souris_editeur()
{

}

void Fenetre::double_clic_souris_editeur()
{
    qreal x=scene->x;
    qreal y=scene->y;

    if (scene->dbl==1)
    {
        scene->dbl=0;

        liste_selection.clear();

        unite copie_unite = preparation_unite(0);

        copie_unite.place=0;
        copie_unite.destination_place=0;
        copie_unite.chg=1;
        copie_unite.chg_pixmap=1;
        copie_unite.chg_module=1;
        copie_unite.chg_objet=0;
        copie_unite.immobilise=false;
        copie_unite.detruite=0;
        copie_unite.en_deplacement=0;
        copie_unite.arret_en_cours=0;
        copie_unite.montee=0;
        copie_unite.atterrissage=0;
        copie_unite.cible=-1;
        copie_unite.type=0;
        copie_unite.altitude_voulue=0;

        copie_unite.angle=0;
        copie_unite.vitesse=0;
        copie_unite.position.x=(x+camera.x)*echelle/valeur_zoom[zoom];
        copie_unite.position.y=(y+camera.y)*echelle/valeur_zoom[zoom];
        copie_unite.position.z=0;
        copie_unite.destination.x=(x+camera.x)*echelle/valeur_zoom[zoom];
        copie_unite.destination.y=(y+camera.y)*echelle/valeur_zoom[zoom];
        copie_unite.destination.z=0;

        copie_unite.nom="New Unit";

        liste_unite.push_back(copie_unite);

        construction_fenetre_unite(copie_unite.id);

        liste_selection.push_back(copie_unite.id);
    }
}

void Fenetre::event_souris_gauche_editeur()
{
    qreal x=scene->x;
    qreal y=scene->y;


    int k=liste_selection.size();
    int id_u;
    if (k>0)
    {
        id_u=liste_selection[0];
    }
    liste_selection.clear();

    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (x<=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x+10 && x>=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-10 && y<=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y+10 && y>=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-10)
        {
            if (liste_selection.size()==0)
            {
                liste_selection.push_back(liste_unite[i].id);

                if (k==0 || (k==1 && id_u!=liste_unite[i].id))
                {

                    construction_fenetre_unite(liste_unite[i].id);
                }
            }
        }
    }
}

void Fenetre::event_souris_droite_editeur()
{
    //qreal x=scene->x;
    //qreal y=scene->y;


}

void Fenetre::event_echap(){
    onglet_selection->setVisible(false);

    if (fenetre_sat_jeu != 0){
        fenetre_sat_jeu->setVisible(false);
    }

    if (fenetre_log_jeu != 0){
        fenetre_log_jeu->setVisible(false);
    }

    if (fenetre_menu_jeu == 0){
        construction_fenetre_menu_jeu();
        if (mode == 1){
            pause = true;
            bouton_pause->setEnabled(false);
            bouton_sat_jeu->setEnabled(false);
            bouton_log->setEnabled(false);
        }
        else if (mode == 2){
            bouton_script->setEnabled(false);
            bouton_pays->setEnabled(false);
            bouton_map->setEnabled(false);
            bouton_ia->setEnabled(false);
            bouton_sat->setEnabled(false);
            bouton_sauvegarde->setEnabled(false);
            bouton_sauvegarde_sous->setEnabled(false);
        }

    }
    else {
        if (mode == 1){
            pause = pause || !fenetre_menu_jeu->isVisible();
            bouton_pause->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_sat_jeu->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_log->setEnabled(fenetre_menu_jeu->isVisible());

        }
        else if (mode == 2){
            bouton_script->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_pays->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_map->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_ia->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_sat->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_sauvegarde->setEnabled(fenetre_menu_jeu->isVisible());
            bouton_sauvegarde_sous->setEnabled(fenetre_menu_jeu->isVisible());
        }
        fenetre_menu_jeu->setVisible(!fenetre_menu_jeu->isVisible());

    }
    esc_appuye=0;
}

void Fenetre::event_entree(){
    if (liste_selection.size() > 0){
        envoi_ordre ();
    }
    entree_appuye = false;
}

void Fenetre::release_selection(){

    int x1,x2,y1,y2;

    if (position_rect.x<position_rect2.x)
    {
        if (position_rect.y<position_rect2.y)
        {
            x1=position_rect.x;
            y1=position_rect.y;
            x2=position_rect2.x;
            y2=position_rect2.y;
        }
        else
        {
            x1=position_rect.x;
            y1=position_rect2.y;
            x2=position_rect2.x;
            y2=position_rect.y;
        }
    }
    else
    {
        if (position_rect.y<position_rect2.y)
        {
            x1=position_rect2.x;
            y1=position_rect.y;
            x2=position_rect.x;
            y2=position_rect2.y;
        }
        else
        {
            x1=position_rect2.x;
            y1=position_rect2.y;
            x2=position_rect.x;
            y2=position_rect.y;
        }
    }

    scene->release=0;

    select_rect=0;
    select_minimap = false;

    onglet_selection->clear();

    for (int i=0; i<liste_selection.size(); i++)
    {
        int k=index_unite(liste_selection[i]);
        if (k!=-1)
        {
            liste_unite[k].chg=1;
        }
    }

    liste_selection.clear();

    for (unsigned int i=0; i<liste_unite.size(); i++){
        if (x1<=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x+10 && x2>=liste_unite[i].position.x*valeur_zoom[zoom]/echelle-camera.x-10 && y1<=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y+10 && y2>=liste_unite[i].position.y*valeur_zoom[zoom]/echelle-camera.y-10 ){  //&& joueur==liste_classe[liste_unite[i].type].pays
            liste_selection.push_back(liste_unite[i].id);


            onglet_selection->addTab(liste_unite[i].widg_selection, liste_unite[i].nom + " / " + liste_classe[liste_unite[i].type].nom);
        }
    }

    int current = onglet_selection->currentIndex();
    changement_tab(current);

    onglet_selection->setVisible(onglet_selection->count() > 0);

    if (fenetre_sat_jeu != 0){
        fenetre_sat_jeu->setVisible(false);
    }
}

bool Fenetre::check_module_unite ()
{
    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (liste_unite[i].chg_module)
        {

            liste_unite[i].altitude_max=0;
            liste_unite[i].altitude_min=0;
            liste_unite[i].altitude_croisiere=0;
            liste_unite[i].vitesse_max=0;
            liste_unite[i].vitesse_min=0;
            liste_unite[i].vitesse_rotation=0;
            liste_unite[i].angle_montee=0;
            liste_unite[i].acceleration=0;
            liste_unite[i].proba_fail=1.0;
            liste_unite[i].proba_fail_fin=1.0;
            liste_unite[i].vision=0;
            liste_unite[i].immobilise=true;
            liste_unite[i].detruite=1;
            liste_unite[i].vie_max = 0;
            liste_unite[i].repair_object = 0;


            for (unsigned int j=0; j<NOMBRE_OBJET; j++)
            {
                liste_unite[i].peut_lancer[j]=0;
            }
            liste_unite[i].puissance_radar_passif=0;
            for (unsigned int j=0; j<NOMBRE_TYPE; j++)
            {
                liste_unite[i].peut_detecter[j]=0;
                liste_unite[i].puissance_radar[j]=0;
                liste_unite[i].processing_radar[j]=1.0;
                liste_unite[i].portee_radar[j]=0;
            }

            for (unsigned int j=0; j<NOMBRE_MODULE; j++)
            {
                if (liste_classe[liste_unite[i].type].module_present[j] && liste_unite[i].vie_module[j]>0)
                {
                    liste_unite[i].repair_object += liste_module[j].repair_object;
                    if (liste_module[j].type==0 || (liste_module[j].type==5 && liste_classe[liste_unite[i].type].type == 5))
                    {
                        liste_unite[i].detruite=0;
                        liste_unite[i].vie = liste_unite[i].vie_module[j];
                        liste_unite[i].vie_max = liste_module[j].pv;
                    }
                    if ((liste_module[j].type==1 && liste_unite[i].fuel>0) || (liste_module[j].type==5 && liste_classe[liste_unite[i].type].type == 5))
                    {
                        liste_unite[i].immobilise=false;

                        liste_unite[i].altitude_max=liste_module[j].altitude_max;
                        liste_unite[i].altitude_min=liste_module[j].altitude_min;
                        liste_unite[i].altitude_croisiere=liste_module[j].altitude_croisiere;
                        liste_unite[i].vitesse_max=liste_module[j].vitesse_max;
                        liste_unite[i].vitesse_min=liste_module[j].vitesse_min;
                        liste_unite[i].vitesse_rotation=liste_module[j].vitesse_rotation;
                        liste_unite[i].acceleration=liste_module[j].acceleration;
                        liste_unite[i].decceleration=liste_module[j].decceleration;
                        liste_unite[i].angle_montee=liste_module[j].angle_montee;


                    }
                    if (liste_module[j].type==2)
                    {
                        for (unsigned int k=0; k<NOMBRE_OBJET; k++)
                        {
                            if (liste_unite[i].peut_lancer[k]==0)
                            {
                                liste_unite[i].peut_lancer[k]=liste_module[j].peut_lancer[k];
                            }
                        }
                    }
                    if (liste_module[j].type==4) // radar
                    {
                        for (unsigned int k=0; k<NOMBRE_TYPE; k++)
                        {
                            if (liste_module[j].peut_detecter[k] && liste_module[j].puissance_radar[k]*liste_module[j].processing_radar[k]>liste_unite[i].puissance_radar[k]*liste_unite[i].processing_radar[k]){
                                liste_unite[i].puissance_radar[k]=liste_module[j].puissance_radar[k];
                                liste_unite[i].processing_radar[k]=liste_module[j].processing_radar[k];
                                liste_unite[i].portee_radar[k]=liste_module[j].portee_radar[k];
                                liste_unite[i].peut_detecter[k]=1;
                            }
                        }
                        if (liste_module[j].puissance_radar_passif>liste_unite[i].puissance_radar_passif){
                            liste_unite[i].puissance_radar_passif=liste_module[j].puissance_radar_passif;
                        }
                    }
                    if (liste_module[j].type==6){ // guidance
                        liste_unite[i].proba_fail *= liste_module[j].proba_fail;
                        liste_unite[i].proba_fail_fin *= liste_module[j].proba_fail_fin;
                    }


                    if (liste_module[j].vision > liste_unite[i].vision){
                        liste_unite[i].vision = liste_module[j].vision;
                    }
                }
            }
            liste_unite[i].chg_module=0;

            if (liste_unite[i].detruite){
                ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has been destroyed");
            }
        }

        int val = liste_unite[i].repair_object;

        if (liste_unite[i].liste_reparation.size() > 0){
            liste_unite[i].liste_reparation[0].y -= val;

            if (liste_unite[i].liste_reparation[0].y <= 0){
                liste_unite[i].nombre_objet[liste_unite[i].liste_reparation[0].x] ++;

                for (int j = 0; j < liste_unite[i].liste_reparation.size() - 1; j++){
                    liste_unite[i].liste_reparation[j] = liste_unite[i].liste_reparation[j+1];
                }
                liste_unite[i].liste_reparation.pop_back();
            }
        }


    }
    return 1;
}

void Fenetre::check_dead (){
    bool done = false;
    while(!done){
        int index = -1;
        for (unsigned int i=0; i<liste_unite.size(); i++){
            if (liste_unite[i].detruite){
                index = i;
            }
        }

        if (index == -1){
            done = true;
        }
        else {
            destruction_unite(liste_unite[index].id);

            for (unsigned int i=index; i<liste_unite.size()-1; i++){
                liste_unite[i] = liste_unite[i+1];
            }
            liste_unite.pop_back();
        }
    }
}

void Fenetre::check_fail(){
    for (unsigned int i=0; i<liste_unite.size(); i++){
        if (liste_classe[liste_unite[i].type].type == 1){ // missile
            double ran = rand()%10000;

            if (ran < liste_unite[i].proba_fail * 10000){
                liste_unite[i].crazee = true;

                ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has lost its target");
            }
        }
    }
}

bool Fenetre::check_deplacement ()
{
    double x1,y1,x2,y2;

    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (liste_unite[i].vitesse_voulue > liste_unite[i].vitesse_max){
            liste_unite[i].vitesse_voulue = liste_unite[i].vitesse_max;
        }


        x1=liste_unite[i].position.x;
        y1=liste_unite[i].position.y;
        x2=liste_unite[i].destination.x;
        y2=liste_unite[i].destination.y;

        bool asat = false;

        if (liste_classe[liste_unite[i].type].type==1) // missile
        {
            if (liste_unite[i].crazee){

                if (rand()%100 < 5){
                    liste_unite[i].destination.x=rand()%((int)(largeur_map * echelle / 100));
                    liste_unite[i].destination.y=rand()%((int)(hauteur_map * echelle / 100));
                    liste_unite[i].destination.x*=100;
                    liste_unite[i].destination.y*=100;
                }

                liste_unite[i].altitude_voulue=liste_unite[i].altitude_max;
                liste_unite[i].en_deplacement=true;

                if (liste_unite[i].fuel <= 0 || rand()%100 < 1){
                    liste_unite[i].detruite = true;
                }
            }
            else {
                int j=index_unite(liste_unite[i].cible);
                if (j!=-1)
                {
                    liste_unite[i].destination.x=liste_unite[j].position.x;
                    liste_unite[i].destination.y=liste_unite[j].position.y;
                    liste_unite[i].altitude_voulue=liste_unite[j].position.z;
                    liste_unite[i].en_deplacement=1;
                }
                else // peut etre changer cible
                {

                    for (int k = 0; k < liste_sat.size(); k++){
                        if (liste_sat[k].id == liste_unite[i].cible){
                            if (liste_sat[k].type == 0 || (liste_sat[k].type == 1 && liste_sat[k].temps_propre > liste_sat[k].temps_ailleurs)){
                                asat = true;
                            }
                            else {
                                ajout_info_texte("Satellite " + liste_sat[k].nom + " not in zone anymore, " + liste_classe[liste_unite[i].type].nom + " lost its target");
                            }
                        }
                    }

                    liste_unite[i].en_deplacement=1;

                    liste_unite[i].crazee = !asat;
                }
            }

        }

        if (liste_unite[i].immobilise)
        {

            liste_unite[i].vitesse-=liste_classe[liste_unite[i].type].frottement*vitesse_jeu/fps_precedent;
            if (liste_unite[i].vitesse<0)
            {
                liste_unite[i].vitesse=0;
            }

            if (liste_classe[liste_unite[i].type].type == 1){
                ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " self-destructed");

                liste_unite[i].detruite = true;
            }
            else if (liste_classe[liste_unite[i].type].type == 2 || liste_classe[liste_unite[i].type].type == 4 || liste_classe[liste_unite[i].type].type == 8){
                if (liste_unite[i].position.z>0)
                {
                    liste_unite[i].vitesse_chute+=9.81*vitesse_jeu/fps_precedent;
                    liste_unite[i].position.z-=liste_unite[i].vitesse_chute*vitesse_jeu/fps_precedent;

                    if (liste_unite[i].position.z<=0)
                    {
                        if (liste_classe[liste_unite[i].type].type != 8){
                            ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " crashed");
                            //destruction_unite(liste_unite[i].id);

                            liste_unite[i].detruite = true;
                        }
                        else {
                            liste_unite[i].position.z = 0;
                        }

                    }
                }
            }

        }
        else
        {
            liste_unite[i].vitesse_chute=0;
            if (liste_unite[i].vitesse_voulue == 0){
                liste_unite[i].vitesse_voulue = liste_unite[i].vitesse_max;
            }

            if (liste_unite[i].remontee){
                if (liste_classe[liste_unite[i].type].type != 4){
                    liste_unite[i].angle+=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                }

                if (liste_unite[i].vitesse > liste_unite[i].vitesse_voulue){
                    liste_unite[i].vitesse-=liste_unite[i].decceleration*vitesse_jeu/fps_precedent;

                    if (liste_unite[i].vitesse < liste_unite[i].vitesse_voulue){
                        liste_unite[i].vitesse=liste_unite[i].vitesse_voulue;
                    }
                }
                else {
                    liste_unite[i].vitesse+=liste_unite[i].acceleration*vitesse_jeu/fps_precedent;

                    if (liste_unite[i].vitesse > liste_unite[i].vitesse_voulue){
                        liste_unite[i].vitesse=liste_unite[i].vitesse_voulue;
                    }
                }

            }
            else if (liste_unite[i].en_deplacement)
            {
                double angle=angleFromPosAndDes(x1, y1, x2, y2);

                liste_unite[i].angle_voulu=angle;

                if (!liste_unite[i].atterrissage || liste_unite[i].vitesse_min == 0)
                {
                    if (liste_unite[i].vitesse > liste_unite[i].vitesse_voulue){
                        liste_unite[i].vitesse-=liste_unite[i].decceleration*vitesse_jeu/fps_precedent;

                        if (liste_unite[i].vitesse < liste_unite[i].vitesse_voulue){
                            liste_unite[i].vitesse=liste_unite[i].vitesse_voulue;
                        }
                    }
                    else {
                        liste_unite[i].vitesse+=liste_unite[i].acceleration*vitesse_jeu/fps_precedent;

                        if (liste_unite[i].vitesse > liste_unite[i].vitesse_voulue){
                            liste_unite[i].vitesse=liste_unite[i].vitesse_voulue;
                        }
                    }


                }

                double angle_min;
                int index_min;  // 0, 1 ou 2 ^^

                angle_min=abs(liste_unite[i].angle_voulu-liste_unite[i].angle);
                index_min=0;

                if (abs(liste_unite[i].angle_voulu+2*PI-liste_unite[i].angle)<angle_min)
                {
                    angle_min=abs(liste_unite[i].angle_voulu+2*PI-liste_unite[i].angle);
                    index_min=1;
                }
                if (abs(liste_unite[i].angle_voulu-2*PI-liste_unite[i].angle)<angle_min)
                {
                    angle_min=abs(liste_unite[i].angle_voulu-2*PI-liste_unite[i].angle);
                    index_min=2;
                }

                switch (index_min)
                {
                    default:
                        if (liste_unite[i].angle+liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent<liste_unite[i].angle_voulu)
                        {
                            liste_unite[i].angle+=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                        }
                        else
                        {
                            if (liste_unite[i].angle-liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent>liste_unite[i].angle_voulu)
                            {
                                liste_unite[i].angle-=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                            }
                            else
                            {
                                liste_unite[i].angle=liste_unite[i].angle_voulu;
                            }
                        }
                    break;
                    case 1:
                        if (liste_unite[i].angle+liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent<liste_unite[i].angle_voulu+2*PI)
                        {
                            liste_unite[i].angle+=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                        }
                        else
                        {
                            if (liste_unite[i].angle-liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent>liste_unite[i].angle_voulu+2*PI)
                            {
                                liste_unite[i].angle-=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                            }
                            else
                            {
                                liste_unite[i].angle=liste_unite[i].angle_voulu;
                            }
                        }
                    break;
                    case 2:
                        if (liste_unite[i].angle+liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent<liste_unite[i].angle_voulu-2*PI)
                        {
                            liste_unite[i].angle+=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                        }
                        else
                        {
                            if (liste_unite[i].angle-liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent>liste_unite[i].angle_voulu-2*PI)
                            {
                                liste_unite[i].angle-=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;
                            }
                            else
                            {
                                liste_unite[i].angle=liste_unite[i].angle_voulu;
                            }
                        }
                    break;
                }

                if (distance(liste_unite[i].position.x,liste_unite[i].position.y,x2,y2)<liste_unite[i].vitesse*vitesse_jeu/fps_precedent)
                {
                    if (liste_unite[i].liste_deplacement.size()==0)
                    {
                        liste_unite[i].en_deplacement=0;
                        liste_unite[i].arret_en_cours=1;

                        if (liste_unite[i].atterrissage==1)
                        {
                            liste_unite[i].atterrissage=2;
                        }
                    }
                    else
                    {
                        coo3 copie;
                        copie.x=liste_unite[i].destination.x;
                        copie.y=liste_unite[i].destination.y;

                        liste_unite[i].destination.x=liste_unite[i].liste_deplacement[0].x;
                        liste_unite[i].destination.y=liste_unite[i].liste_deplacement[0].y;

                        for (int k=0; k<liste_unite[i].liste_deplacement.size()-1; k++)
                        {
                            liste_unite[i].liste_deplacement[k]=liste_unite[i].liste_deplacement[k+1];
                        }
                        liste_unite[i].liste_deplacement.pop_back();

                        if (liste_unite[i].patrouille){
                            liste_unite[i].liste_deplacement.push_back(copie);
                        }
                    }
                    /*if (liste_unite[i].liste_deplacement2.size() > 0){
                        if (distance(liste_unite[i].position.x,liste_unite[i].position.y,liste_unite[i].liste_deplacement2[0].x,liste_unite[i].liste_deplacement2[0].y)<liste_unite[i].vitesse*vitesse_jeu/fps_precedent)
                        {
                            for (int k=0; k<liste_unite[i].liste_deplacement2.size()-1; k++)
                            {
                                liste_unite[i].liste_deplacement2[k]=liste_unite[i].liste_deplacement2[k+1];
                            }
                            liste_unite[i].liste_deplacement2.pop_back();
                        }
                    }
                    else {
                        //erreur_log("MovingList size = 0 of unit : " + QString::number(i));
                    }*/
                }

                liste_unite[i].chg=1;
            }
            else
            {
                if (liste_unite[i].arret_en_cours)
                {
                    if (liste_unite[i].vitesse-liste_unite[i].decceleration*vitesse_jeu/fps_precedent>liste_unite[i].vitesse_min)
                    {
                        liste_unite[i].vitesse-=liste_unite[i].decceleration*vitesse_jeu/fps_precedent;

                    }
                    else
                    {
                        liste_unite[i].vitesse=liste_unite[i].vitesse_min;
                        liste_unite[i].arret_en_cours=0;
                    }

                    liste_unite[i].chg=1;
                }
            }
        }


        /*if (liste_unite[i].montee)
        {
            qDebug() << "Jekek";

            if (liste_unite[i].position.z<liste_unite[i].altitude_voulue)  // montee
            {


                if (liste_unite[i].position.z+sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent<liste_unite[i].altitude_voulue)
                {
                    liste_unite[i].position.z+=sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                    liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                    liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                }
                else
                {
                    liste_unite[i].position.z=liste_unite[i].altitude_voulue;
                    liste_unite[i].montee=0;

                    liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                    liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                }
            }
            else  // descente
            {
                if (liste_unite[i].position.z-sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent>liste_unite[i].altitude_voulue)
                {
                    liste_unite[i].position.z-=sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                    liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                    liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                }
                else
                {
                    liste_unite[i].position.z=liste_unite[i].altitude_voulue;
                    liste_unite[i].montee=0;

                    liste_unite[i].position.x+=cos(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                    liste_unite[i].position.y+=sin(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                }
            }
        }
        else
        {
            liste_unite[i].position.x+=cos(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
            liste_unite[i].position.y+=sin(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
        }*/

        if (liste_unite[i].position.z<liste_unite[i].altitude_voulue)  // montee
        {
            if (liste_unite[i].position.z+sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent<liste_unite[i].altitude_voulue)
            {
                liste_unite[i].position.z+=sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
            }
            else
            {
                liste_unite[i].position.z=liste_unite[i].altitude_voulue;
                liste_unite[i].montee=0;

                liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                if (liste_unite[i].remontee){
                    liste_unite[i].remontee = false;
                    liste_unite[i].arret_en_cours = true;
                }
            }
        }
        else if (liste_unite[i].position.z>liste_unite[i].altitude_voulue) // descente
        {
            if (liste_unite[i].position.z-sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent>liste_unite[i].altitude_voulue)
            {
                liste_unite[i].position.z-=sin(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                liste_unite[i].position.x+=cos(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                liste_unite[i].position.y+=sin(liste_unite[i].angle)*cos(liste_unite[i].angle_montee)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
            }
            else
            {
                liste_unite[i].position.z=liste_unite[i].altitude_voulue;
                liste_unite[i].montee=0;

                liste_unite[i].position.x+=cos(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
                liste_unite[i].position.y+=sin(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;

                if (liste_unite[i].remontee){
                    liste_unite[i].remontee = false;
                    liste_unite[i].arret_en_cours = true;
                }
            }
        }
        else
        {
            liste_unite[i].position.x+=cos(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
            liste_unite[i].position.y+=sin(liste_unite[i].angle)*liste_unite[i].vitesse*vitesse_jeu/fps_precedent;
        }


        liste_unite[i].chg = (liste_unite[i].vitesse != 0);


        if (liste_classe[liste_unite[i].type].type==1) // missile
        {
            int j=index_unite(liste_unite[i].cible);
            if (j!=-1)
            {

                if (distance(x1,y1,liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y,liste_unite[j].position.z)<liste_unite[i].vitesse*vitesse_jeu*2/fps_precedent)
                {
                    int pays = liste_classe[liste_unite[i].type].pays;
                    for (int k = 0; k < liste_unite[i].liste_id_element_ia.size(); k++){
                        int index = index_element_ia(liste_unite[i].liste_id_element_ia[k], pays);

                        if (index != -1){
                            liste_pays[pays].liste_element[index].elem.res.result += 10.0;
                        }
                    }

                    pays = liste_classe[liste_unite[j].type].pays;
                    for (int k = 0; k < liste_unite[j].liste_id_element_ia.size(); k++){
                        int index = index_element_ia(liste_unite[j].liste_id_element_ia[k], pays);

                        if (index != -1 && temps-liste_pays[pays].liste_element[index].temps < 1000 && temps-liste_pays[pays].liste_element[index].temps > 60){
                            liste_pays[pays].liste_element[index].elem.res.result -= 10.0 * ((-1.0/(1000 - 60)) * (temps-liste_pays[pays].liste_element[index].temps - 60) + 1) ;
                        }
                    }

                    double ran = rand()%10000;
                    if (ran < liste_unite[i].proba_fail_fin * 10000){
                        ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has miss a " + liste_classe[liste_unite[j].type].nom);
                    }
                    else {
                        for (unsigned int k=0; k<NOMBRE_MODULE; k++)
                        {
                            if (liste_classe[liste_unite[j].type].module_present[k])
                            {
                                double ran=rand()%1000 + 1000;
                                ran/=2000;
                                liste_unite[j].vie_module[k]-=ran*liste_classe[liste_unite[i].type].warhead*liste_module[k].resistance;

                                if (liste_unite[j].vie_module[k]<0)
                                {
                                    liste_unite[j].vie_module[k]=0;
                                }
                            }
                        }

                        liste_unite[j].chg_module=1;

                        ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has hit a " + liste_classe[liste_unite[j].type].nom);
                    }


                    liste_unite[i].detruite = true;
                }
            }
            else if (asat){

                if (distance(x1,y1,liste_unite[i].position.z, x2, y2,liste_unite[i].altitude_voulue)<liste_unite[i].vitesse*vitesse_jeu*2/fps_precedent){

                    int index = -1;
                    for (int k = 0; k < liste_sat.size(); k++){
                        if (liste_sat[k].id == liste_unite[i].cible){
                            index = k;
                        }
                    }

                    if (index != -1){
                        double ran = rand()%10000;
                        if (ran < liste_unite[i].proba_fail_fin * 10000){
                            ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has miss satellite " + liste_sat[index].nom);
                        }
                        else {
                            ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has destroyed satellite " + liste_sat[index].nom);

                            for (int k = index; k < liste_sat.size()-1; k++){
                                liste_sat[k] = liste_sat[k+1];
                            }
                            liste_sat.pop_back();
                        }
                    }

                    liste_unite[i].detruite = true;
                }
                else if (distance(x1,y1, x2, y2)<liste_unite[i].vitesse*vitesse_jeu*2/fps_precedent){
                    liste_unite[i].remontee = true;
                }

            }
        }
        else if (liste_classe[liste_unite[i].type].type==5 || liste_classe[liste_unite[i].type].type==6 || liste_classe[liste_unite[i].type].type==7){
            int x = liste_unite[i].position.x / echelle;
            int y = liste_unite[i].position.y / echelle;

            if (! (x < 0 || y < 0 || x >= largeur_map || y >= hauteur_map)){
                liste_unite[i].position.z = map[x][y].altitude;
                liste_unite[i].altitude_voulue = map[x][y].altitude;
            }

        }

        if (liste_unite[i].vitesse_min>0 && !liste_unite[i].en_deplacement)  // ronds
        {
            liste_unite[i].angle+=liste_unite[i].vitesse_rotation*vitesse_jeu/fps_precedent;


        }
        if (liste_unite[i].atterrissage==2)
        {
            int k=index_unite(liste_unite[i].cible_ami);
            if (k!=-1)
            {
                if ((liste_unite[i].angle+0.1>liste_unite[k].angle && liste_unite[i].angle-0.1<liste_unite[k].angle) || (liste_unite[i].angle+0.1>liste_unite[k].angle+2*PI && liste_unite[i].angle-0.1<liste_unite[k].angle+2*PI) || (liste_unite[i].angle+0.1>liste_unite[k].angle-2*PI && liste_unite[i].angle-0.1<liste_unite[k].angle-2*PI))
                {
                    liste_unite[i].destination.x=liste_unite[k].position.x;
                    liste_unite[i].destination.y=liste_unite[k].position.y;
                    liste_unite[i].atterrissage=3;
                    liste_unite[i].en_deplacement=1;
                }
                else
                {
                    liste_unite[i].en_deplacement=0;
                }
            }
            else
            {
                liste_unite[i].cible_ami=-1;
                liste_unite[i].atterrissage=0;
            }
        }

        if (liste_unite[i].atterrissage==3)
        {
            int k=index_unite(liste_unite[i].cible_ami);
            if (k!=-1)
            {
                if (liste_unite[k].en_deplacement){

                    coo pos;
                    pos.x = liste_unite[k].position.x;
                    pos.y = liste_unite[k].position.y;

                    if (!deplacement_unite(i, pos, false, false)){
                        liste_unite[i].atterrissage=0;
                        liste_unite[i].en_deplacement=false;
                        liste_unite[i].arret_en_cours = true;
                    }

                }

                if (distance(liste_unite[i].position.x, liste_unite[i].position.y,liste_unite[k].position.x, liste_unite[k].position.y)<liste_unite[i].vitesse*vitesse_jeu/fps_precedent)
                {
                    int id_objet=-1;
                    for (unsigned int l=0; l<NOMBRE_OBJET; l++)
                    {
                        if (liste_objet[l].future_unite==liste_unite[i].type)
                        {
                            id_objet=l;
                        }
                    }
                    if (id_objet!=-1)
                    {
                        for (unsigned int m=0; m<NOMBRE_OBJET; m++)
                        {
                            liste_unite[k].nombre_objet[m]+=liste_unite[i].nombre_objet[m];
                        }

                        int degat = 0;
                        int fuel_max = 0;

                        for (unsigned int m = 0; m < NOMBRE_MODULE; m++){
                            if (liste_classe[liste_unite[i].type].module_present[m]){
                                degat += liste_module[m].pv - liste_unite[i].vie_module[m];
                            }

                            if (liste_classe[liste_unite[k].type].module_present[m] && liste_unite[k].vie_module[m] > 0){
                                fuel_max += liste_module[m].fuel;
                            }


                        }

                        liste_unite[k].fuel += liste_unite[i].fuel;

                        if (liste_unite[k].fuel > fuel_max){
                            liste_unite[k].fuel = fuel_max;
                        }

                        if (degat > 0){
                            coo c;
                            c.x = id_objet;
                            c.y = degat;

                            liste_unite[k].liste_reparation.push_back(c);
                        }
                        else {
                            liste_unite[k].nombre_objet[id_objet]++;
                        }

                        if (liste_classe[liste_unite[i].type].pays == joueur){
                            ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has landed on a " + liste_classe[liste_unite[k].type].nom);
                        }

                        liste_unite[i].detruite = true;
                    }
                }
            }
            else {
                liste_unite[i].atterrissage=0;
            }
        }

        if (liste_unite[i].fuel>0)
        {
            for (unsigned int j=0; j<NOMBRE_MODULE; j++)
            {
                if (liste_classe[liste_unite[i].type].module_present[j] && liste_module[j].type==1)
                {
                    liste_unite[i].conso=liste_module[j].conso*(liste_unite[i].vitesse/liste_unite[i].vitesse_max);
                    liste_unite[i].fuel-=liste_unite[i].conso*vitesse_jeu/fps_precedent;

                    if (liste_unite[i].fuel<=0)
                    {
                        liste_unite[i].fuel=0;
                        liste_unite[i].chg_module=1;
                    }
                }
            }
        }

        if (liste_unite[i].angle>3*PI/2){
            liste_unite[i].angle-=2*PI;
        }
    }



    return 1;
}

void Fenetre::check_projectile (){
    bool delete_complete = true;
    for (int i = 0; i < liste_projectile.size(); i++){
        liste_projectile[i].temps_restant -= vitesse_jeu/fps_precedent;

        if (liste_projectile[i].temps_restant <= 0){
            delete_complete = false;
            QVector<int> liste_unite_touche;
            if (liste_projectile[i].fixe){
                for (unsigned int j = 0; j < liste_unite.size(); j++){
                    double d = distance(liste_projectile[i].destination.x, liste_projectile[i].destination.y, liste_projectile[i].destination.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z);

                    if (d < liste_classe[liste_unite[j].type].taille * CONSTANTE_PROJECTILE){
                        liste_unite_touche.push_back(liste_unite[j].id);
                    }
                }
            }
            else {
                double ran=rand()%1000;

                int k = index_unite(liste_projectile[i].cible);

                if (k != -1){
                    double val_taille = 0.5 + liste_classe[liste_unite[k].type].taille/200.0;
                    if (ran < liste_objet[liste_projectile[i].type].data.precision * 1000 * val_taille){
                        liste_unite_touche.push_back(liste_projectile[i].cible);
                    }
                }
            }


            for (int j = 0; j < liste_unite_touche.size(); j++){
                int in = index_unite(liste_unite_touche[j]);
                if (in != -1){
                    int pays = liste_classe[liste_unite[in].type].pays;
                    for (int k = 0; k < liste_unite[in].liste_id_element_ia.size(); k++){
                        int index = index_element_ia(liste_unite[in].liste_id_element_ia[k], pays);

                        if (index != -1 && temps-liste_pays[pays].liste_element[index].temps < 180){
                            if (temps-liste_pays[pays].liste_element[index].temps <= 0){
                                liste_pays[pays].liste_element[index].elem.res.result -= 2.0;
                            }
                            else {
                                liste_pays[pays].liste_element[index].elem.res.result -= 2.0 * ((-1.0 / 180.0) * (temps-liste_pays[pays].liste_element[index].temps) + 1);
                            }
                        }
                    }

                    for (unsigned int k=0; k<NOMBRE_MODULE; k++){
                        if (liste_classe[liste_unite[in].type].module_present[k]){
                            double ran=rand()%1000 + 1000;
                            ran/=2000;

                            double ran2 = 1.0;

                            int chance = 1/liste_objet[liste_projectile[i].type].data.warhead;

                            if (liste_objet[liste_projectile[i].type].data.warhead < 1.0 && rand()% chance != 0){
                                ran2 = 0.0;
                            }

                            liste_unite[in].vie_module[k]-=ran2*ran*liste_objet[liste_projectile[i].type].data.warhead*liste_module[k].resistance;

                            if (liste_unite[in].vie_module[k]<0){
                                liste_unite[in].vie_module[k]=0;
                            }


                        }
                    }

                    liste_unite[in].chg_module=1;
                }
                else {
                    erreur_log("Index = -1 in check_projectile");
                }
            }
        }
    }

    while (!delete_complete){
        int index = -1;
        for (int i = 0; i < liste_projectile.size(); i++){
            if (liste_projectile[i].temps_restant <= 0){
                index = i;
            }
        }

        if (index == -1){
            delete_complete = true;
        }
        else {
            for (int i = index; i < liste_projectile.size() - 1; i++){
                liste_projectile[i] = liste_projectile[i+1];
            }
            liste_projectile.pop_back();
        }
    }
}

void Fenetre::check_sat(){

    for (int i = 0; i < liste_sat.size(); i++){
        liste_sat[i].temps_propre+=vitesse_jeu;

        while (liste_sat[i].temps_propre > liste_sat[i].temps_ailleurs + liste_sat[i].temps_dessus && liste_sat[i].temps_ailleurs + liste_sat[i].temps_dessus > 0){
            liste_sat[i].temps_propre -= liste_sat[i].temps_ailleurs + liste_sat[i].temps_dessus;
        }


    }

    if (fenetre_sat_jeu != 0){
        bool av = fenetre_sat_jeu->isVisible();

        construction_fenetre_sat_jeu();

        fenetre_sat_jeu->setVisible(av);
    }


}

bool Fenetre::check_unite_selectionnee ()
{


    if (liste_selection.size()!=0 && onglet_selection->currentIndex() != -1)
    {
        int index=onglet_selection->currentIndex();
        int j=index_unite(liste_selection[index]);


        if (j!=-1){

            QScrollArea *scroll = liste_unite[j].widg_selection->findChild<QScrollArea *>("scroll");

            QWidget *super2 = 0;


            QGroupBox * groupe_infoi = 0;
            QGroupBox * groupe_module = 0;
            QGroupBox * groupe_missile = 0;
            QGroupBox * groupe_avion = 0;
            QGroupBox * groupe_helo = 0;
            QGroupBox * groupe_well_dock = 0;

            QLabel* info_altitudei = 0;
            QLabel* info_altitude_vouluei = 0;
            QLabel* info_vitessehi = 0;
            QLabel* info_vitessevi = 0;
            QLabel* info_fueli = 0;
            QLabel* info_persoi = 0;

            if (scroll == 0){
                erreur_log("Scroll is NULL");
                return false;
            }

            super2 = scroll->widget();

            if (super2 == 0){
                erreur_log("Super2 is NULL");
                return false;
            }

            groupe_infoi = super2->findChild<QGroupBox *>("groupe_info");
            groupe_module = super2->findChild<QGroupBox *>("groupe_module");
            groupe_missile = super2->findChild<QGroupBox *>("groupe_missile");
            groupe_avion = super2->findChild<QGroupBox *>("groupe_avion");
            groupe_helo = super2->findChild<QGroupBox *>("groupe_helo");
            groupe_well_dock = super2->findChild<QGroupBox *>("groupe_well_dock");

            if (groupe_infoi == 0 || groupe_module == 0 || groupe_missile == 0 || groupe_avion == 0 || groupe_helo == 0 || groupe_well_dock == 0){
                erreur_log("groupe_??? is NULL");
                return false;
            }

            info_altitudei = groupe_infoi->findChild<QLabel *>("info_altitude");
            info_altitude_vouluei = groupe_infoi->findChild<QLabel *>("info_altitude_voulue");
            info_vitessehi = groupe_infoi->findChild<QLabel *>("info_vitesseh");
            info_vitessevi = groupe_infoi->findChild<QLabel *>("info_vitessev");
            info_fueli = groupe_infoi->findChild<QLabel *>("info_fuel");
            info_persoi = groupe_infoi->findChild<QLabel *>("info_perso");

            if (info_altitudei == 0 || info_altitude_vouluei == 0 || info_vitessehi == 0 || info_vitessevi == 0 || info_fueli == 0 || info_persoi == 0){
                erreur_log("info_??? is NULL");
                return false;
            }



            if (liste_unite[j].chg_objet)
            {

                liste_unite[j].chg_objet=0;
            }

            if (liste_classe[liste_unite[j].type].type==0) //  || liste_classe[liste_unite[j].type].type==5 || liste_classe[liste_unite[j].type].type==6 || liste_classe[liste_unite[j].type].type==7
            {
                info_vitessehi->setText("Speed : " + QString::number((int)liste_unite[j].vitesse) + " m/s");

                info_altitudei->setVisible(false);
                info_altitude_vouluei->setVisible(false);
                info_vitessehi->setVisible(true);
                info_vitessevi->setVisible(false);
            }
            else
            {
                if (liste_unite[j].position.z==liste_unite[j].altitude_voulue)
                {
                    info_altitudei->setText("Altitude : " + QString::number((int)liste_unite[j].position.z) + " m");
                    info_vitessehi->setText("Speed : " + QString::number((int)liste_unite[j].vitesse) + " m/s");

                    info_altitudei->setVisible(true);
                    info_altitude_vouluei->setVisible(false);
                    info_vitessehi->setVisible(true);
                    info_vitessevi->setVisible(false);
                }
                else
                {
                    info_altitudei->setText("Current altitude : " + QString::number((int)liste_unite[j].position.z) + " m");
                    info_altitude_vouluei->setText("Cruise altitude : " + QString::number((int)liste_unite[j].altitude_voulue) + " m");
                    info_vitessehi->setText("Horizontal Speed : " + QString::number((int)liste_unite[j].vitesse*cos(liste_unite[j].angle_montee)) + " m/s");
                    info_vitessevi->setText("Vertical Speed : " + QString::number((int)liste_unite[j].vitesse*sin(liste_unite[j].angle_montee)) + " m/s");

                    info_altitudei->setVisible(true);
                    info_altitude_vouluei->setVisible(true);
                    info_vitessehi->setVisible(true);
                    info_vitessevi->setVisible(true);
                }
            }

            if (liste_classe[liste_unite[j].type].type != 5 && liste_unite[j].fuel==0)
            {
                info_fueli->setText("Fuel left : none");
                info_fueli->setVisible(true);
            }
            else
            {
                if (liste_unite[j].conso!=0 && liste_unite[j].vitesse!=0 && liste_classe[liste_unite[j].type].type != 5)
                {
                    //info_fueli->setText("Fuel left : " + QString::number(liste_unite[j].fuel/liste_unite[j].conso) + " s");
                    info_fueli->setText("Fuel left : " + time_to_string(liste_unite[j].fuel/liste_unite[j].conso));
                    info_fueli->setVisible(true);
                }
                else
                {
                    info_fueli->setVisible(false);
                }
            }

            info_persoi->setVisible(false);
            if (liste_classe[liste_unite[j].type].type == 5){
                for (unsigned int k=0; k<NOMBRE_MODULE; k++){
                    if (liste_classe[liste_unite[j].type].module_present[k] && liste_module[k].type == 5){
                        info_persoi->setText("Personel : " + QString::number(liste_unite[j].vie_module[k]) + " / " + QString::number(liste_module[k].pv));
                        info_persoi->setVisible(true);
                    }
                }
            }

            groupe_module->setVisible(false);

            for (unsigned int i=0; i<NOMBRE_MODULE; i++){
                if (liste_classe[liste_unite[j].type].module_present[i] && liste_module[i].type != 5){
                    QWidget *widg = groupe_module->findChild<QWidget *>("module_" + QString::number(i));
                    QLabel* label_vie = 0;

                    if (widg == 0){
                        erreur_log("widg_module is NULL");
                        return false;
                    }

                    label_vie = widg->findChild<QLabel *>("label_vie");

                    if (label_vie == 0){
                        erreur_log("label_vie is NULL");
                        return false;
                    }

                    for (int k = 0; k < liste_unite[j].liste_ancienne_vie_module.size(); k++){
                        if (liste_unite[j].liste_ancienne_vie_module[k].x == i && liste_unite[j].liste_ancienne_vie_module[k].y != liste_unite[j].vie_module[i]){

                            label_vie->setPixmap(creation_pixmap_vie(j, i));

                            liste_unite[j].liste_ancienne_vie_module[k].y = liste_unite[j].vie_module[i];
                        }
                    }

                    //label_vie->setText("   -    HP : " + QString::number(liste_unite[j].vie_module[i]) + " / " + QString::number(liste_module[i].pv));
                }
            }

            groupe_module->setVisible(true);


            for (unsigned int i=0; i<NOMBRE_OBJET; i++){
                QWidget *widg = 0;
                QLabel* label_nom = 0;
                QLabel* label_nombre = 0;
                QLabel* label_image = 0;

                if (liste_objet[i].type==0 || liste_objet[i].type==2 || liste_objet[i].type==8){
                    widg = groupe_missile->findChild<QWidget *>("obj_" + QString::number(i));
                }
                else if (liste_objet[i].type==1){
                    widg = groupe_avion->findChild<QWidget *>("obj_" + QString::number(i));
                }
                else if (liste_objet[i].type==3 || liste_objet[i].type==4 || liste_objet[i].type==5 || liste_objet[i].type==6){
                    widg = groupe_well_dock->findChild<QWidget *>("obj_" + QString::number(i));
                }
                else {
                    widg = groupe_helo->findChild<QWidget *>("obj_" + QString::number(i));
                }

                if (widg == 0){
                    erreur_log("widg_object is NULL");
                    return false;
                }

                label_nom = widg->findChild<QLabel *>("label_nom");
                label_nombre = widg->findChild<QLabel *>("label_nombre");
                label_image = widg->findChild<QLabel *>("label_image");

                if (label_nom == 0 || label_nombre == 0 || label_image == 0){
                    erreur_log("label? is NULL");
                    return false;
                }

                label_nombre->setText(QString::number(liste_unite[j].nombre_objet[i]) + " left");

                widg->setVisible(liste_unite[j].nombre_objet[i] > 0);
                label_nom->setVisible(liste_unite[j].nombre_objet[i] > 0);
                label_nombre->setVisible(liste_unite[j].nombre_objet[i] > 0);
                label_image->setVisible(liste_unite[j].nombre_objet[i] > 0);
            }

            // ajouter mettre non-visible les groupes ou il n'y a plus d'objet
        }
        else
        {
            int ind=onglet_selection->currentIndex();

            for (int i=ind; i<liste_selection.size()-1; i++)
            {
                liste_selection[i]=liste_selection[i+1];
            }
            liste_selection.pop_back();

            onglet_selection->removeTab(ind);


            if (liste_selection.size()==0)
            {
                onglet_selection->setVisible(false);
            }
        }
    }

    bool done = false;
    while(!done){
        int index = -1;
        for (int i=0; i<liste_selection.size(); i++){
            if (index_unite(liste_selection[i]) == -1){
                index = i;
            }
        }

        if (index == -1){
            done = true;
        }
        else {
            liste_selection.remove(index);
            onglet_selection->removeTab(index);
        }
    }

    return true;
}

void Fenetre::check_radar ()
{
    int test[NOMBRE_PAYS];
    int* unite_detection = new int[liste_unite.size()];
    double bruit;
    for (unsigned int i=0; i<liste_unite.size(); i++)  // unite visee
    {
        bruit=1;
        for (unsigned int j=0; j<NOMBRE_PAYS; j++)
        {
            test[j]=liste_unite[i].est_detecte[j];
            liste_unite[i].est_detecte[j]=0;
        }
        for (unsigned int j=0; j<NOMBRE_MODULE; j++)
        {
            if (liste_module[j].type==0 && liste_classe[liste_unite[i].type].module_present[j])
            {
                if (liste_unite[i].vie_module[j] > 0){
                    bruit=sqrt(liste_module[j].pv/liste_unite[i].vie_module[j]);
                }
                else {
                    bruit = 100;
                }
            }
        }

        if (liste_unite[i].vitesse_max>0 && liste_unite[i].vitesse>0)
        {
            bruit*=1 + liste_unite[i].vitesse/liste_unite[i].vitesse_max;
        }

        int type=liste_classe[liste_unite[i].type].type;

        unite_detection[i] = -1;

        for (unsigned int j=0; j<liste_unite.size(); j++)  // unite radar
        {
            if (liste_classe[liste_unite[j].type].pays!=liste_classe[liste_unite[i].type].pays && liste_pays[liste_classe[liste_unite[j].type].pays].relation[liste_classe[liste_unite[i].type].pays]!=3)
            {
                double d=distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z);
                if (liste_unite[j].peut_detecter[type] && liste_unite[j].puissance_radar[type]>0 && !liste_unite[j].radar_off){
                    if (d*liste_classe[liste_unite[i].type].stealth<liste_unite[j].puissance_radar[type]*liste_unite[j].processing_radar[type]*CONSTANTE_RADAR*bruit && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays] < 2 && d<=liste_unite[j].portee_radar[type]){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=2;
                        unite_detection[i] = j;
                    }
                    else if (d*liste_classe[liste_unite[i].type].stealth<liste_unite[j].puissance_radar[type]*liste_unite[j].processing_radar[type]*CONSTANTE_RADAR2*bruit && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]==0 && d<=liste_unite[j].portee_radar[type]){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=1;
                        unite_detection[i] = j;
                    }
                }

                if (liste_unite[j].vision > 0 && d < 10000){
                    if (d < liste_unite[j].vision*liste_classe[liste_unite[i].type].taille*CONSTANTE_VISION && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays] < 2){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=2;
                        unite_detection[i] = j;
                    }
                    else if (d < liste_unite[j].vision*liste_classe[liste_unite[i].type].taille*CONSTANTE_VISION2 && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]==0){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=1;
                        unite_detection[i] = j;
                    }
                }

                if (!liste_unite[i].radar_off && liste_unite[j].puissance_radar_passif>0){

                    double puissance = 0.0;
                    for (int l = 0; l < NOMBRE_TYPE; l++){
                        puissance += liste_unite[i].puissance_radar[l];
                    }
                    puissance /= NOMBRE_TYPE;

                    if (d < liste_unite[j].puissance_radar_passif*puissance*CONSTANTE_RADAR_PASSIF && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays] < 2){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=2;
                        unite_detection[i] = j;
                    }
                    else if (d < liste_unite[j].puissance_radar_passif*puissance*CONSTANTE_RADAR_PASSIF2 && liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]==0){
                        liste_unite[i].est_detecte[liste_classe[liste_unite[j].type].pays]=1;
                        unite_detection[i] = j;
                    }
                }
            }
        }

        for (unsigned int j = 0; j < liste_sat.size(); j++){
            if (liste_sat[j].type == 0 || (liste_sat[j].type == 1 && liste_sat[j].temps_propre > liste_sat[j].temps_ailleurs)){
                if (liste_classe[liste_unite[i].type].taille >= liste_sat[j].precision && liste_unite[i].est_detecte[liste_sat[j].pays] < 2 && liste_classe[liste_unite[i].type].pays != liste_sat[j].pays && liste_pays[liste_classe[liste_unite[i].type].pays].relation[liste_sat[j].pays] != 1){
                    liste_unite[i].est_detecte[liste_sat[j].pays]=2;
                }
            }
        }

        for (unsigned int j=0; j<NOMBRE_PAYS; j++){
            if (test[j] < liste_unite[i].est_detecte[j] && unite_detection[i] != -1){ // normalement tjrs unite_detection[i] != -1
                for (int k = 0; k < liste_unite[unite_detection[i]].liste_id_element_ia.size(); k++){
                    int index = index_element_ia(liste_unite[unite_detection[i]].liste_id_element_ia[k], j);

                    if (index != -1 && temps-liste_pays[j].liste_element[index].temps < 10000){
                        double coeff = 0.0;
                        switch(type){
                            default:
                                coeff = 10;
                            break;
                            case 1:
                                coeff = 0.5;
                            break;
                            case 2:
                                coeff = 3;
                            break;
                            case 3:
                                coeff = 10;
                            break;
                            case 4:
                                coeff = 2;
                            break;
                            case 5:
                                coeff = 2;
                            break;
                            case 6:
                                coeff = 2;
                            break;
                            case 7:
                                coeff = 1;
                            break;
                        }
                        if (temps-liste_pays[j].liste_element[index].temps <= 0){
                            liste_pays[j].liste_element[index].elem.res.result += coeff;

                        }
                        else {
                            liste_pays[j].liste_element[index].elem.res.result += coeff * ((-1.0 / 10000.0) * (temps-liste_pays[j].liste_element[index].temps) + 1);
                        }

                    }
                }
            }
        }

        if (test[joueur] != liste_unite[i].est_detecte[joueur]){
            liste_unite[i].chg_pixmap = true;
        }

        if (test[joueur]!=2 && liste_unite[i].est_detecte[joueur]==2)
        {
            ajout_info_texte("A " + liste_classe[liste_unite[i].type].nom + " has been spotted");
        }
        else
        {
            if (test[joueur]==0 && liste_unite[i].est_detecte[joueur]==1)
            {
                QString s;
                switch(type)
                {
                    default:
                        s="ship";
                    break;
                    case 1:
                        s="missile";
                    break;
                    case 2:
                        s="aircraft";
                    break;
                    case 5:
                        s="infantry squad";
                    break;
                }

                ajout_info_texte("A unknown " + s + " has been spotted");
            }
        }
    }
}

bool Fenetre::check_engaging (){
    for (unsigned int i=0; i<liste_unite.size(); i++){
        if (liste_classe[liste_unite[i].type].type != 1){ // pas missile
            for (int l = 0; l < liste_unite[i].liste_temps_recharge.size(); l++){
                if (liste_unite[i].liste_temps_recharge[l].val > 0){
                    liste_unite[i].liste_temps_recharge[l].val -= vitesse_jeu/fps_precedent;
                }
                if (liste_unite[i].liste_temps_recharge[l].val < 0){
                    liste_unite[i].liste_temps_recharge[l].val = 0;
                }
            }

            if (liste_unite[i].engaging != 0){

                int k = index_unite(liste_unite[i].cible);

                if (k == -1){
                    liste_unite[i].engaging = 0;
                }
                else {

                    double d=distance(liste_unite[i].position.x, liste_unite[i].position.y, liste_unite[i].position.z, liste_unite[k].position.x, liste_unite[k].position.y, liste_unite[k].position.z);
                    bool ok = false;
                    for (unsigned int j=0; j<NOMBRE_OBJET; j++){
                        bool ok2 = false;
                        int in = -1;
                        if (liste_unite[i].engaging == 1){
                            ok2 = true;
                        }
                        else {
                            for (int k = 0; k < liste_unite[i].liste_tir_obus.size(); k++){
                                if (liste_unite[i].liste_tir_obus[k].x == j && liste_unite[i].liste_tir_obus[k].y > 0){
                                    in = k;
                                }
                            }

                            ok2 = liste_unite[i].liste_tir_obus.size() > 0 && in != -1;
                        }

                        if (ok2 && liste_unite[i].peut_lancer[j] && liste_objet[j].type == 2 && liste_unite[i].nombre_objet[j] > 0 && liste_objet[j].data.portee >= d && liste_objet[j].peut_toucher[liste_classe[liste_unite[k].type].type]){
                            ok = true;

                            bool will_fire = true;
                            for (int l = 0; l < liste_unite[i].liste_temps_recharge.size(); l++){
                                if (liste_unite[i].liste_temps_recharge[l].id == j && liste_unite[i].liste_temps_recharge[l].val > 0){
                                    will_fire = false;
                                }
                            }

                            if (will_fire){
                                int nb_left = liste_objet[j].data.nb_tir;
                                int count = 0;

                                if (liste_unite[i].engaging == 2){
                                    nb_left = min(nb_left, liste_unite[i].liste_tir_obus[in].y);
                                }

                                while (nb_left > 0 && liste_unite[i].nombre_objet[j] > 0){
                                    projectile pro;

                                    if (liste_classe[liste_unite[k].type].type == 0 || liste_classe[liste_unite[k].type].type == 1 || liste_classe[liste_unite[k].type].type == 2 || liste_classe[liste_unite[k].type].type == 4 || (liste_classe[liste_unite[k].type].type == 5 && (liste_classe[liste_unite[i].type].type == 5 || liste_classe[liste_unite[i].type].type == 6 ))){
                                        pro.fixe = false;
                                        pro.cible = liste_unite[i].cible;
                                    }
                                    else {
                                        pro.fixe = true;
                                        pro.destination = liste_unite[k].position;
                                    }

                                    pro.type = j;
                                    pro.temps_restant = d / (liste_objet[j].data.vitesse + liste_unite[k].vitesse); // a changer pour faire dependre de l'angle


                                    liste_projectile.push_back(pro);

                                    liste_unite[i].nombre_objet[j]--;
                                    nb_left--;
                                    count++;
                                }

                                if (liste_unite[i].engaging == 2){
                                    liste_unite[i].liste_tir_obus[in].y -= count;

                                    if (liste_unite[i].liste_tir_obus[in].y <= 0){
                                        liste_unite[i].liste_tir_obus.remove(in);
                                    }
                                }


                                bool is_in_list = false;

                                for (int l = 0; l < liste_unite[i].liste_temps_recharge.size(); l++){
                                    if (liste_unite[i].liste_temps_recharge[l].id == j){
                                        liste_unite[i].liste_temps_recharge[l].val = liste_objet[j].temps_recharge;
                                        is_in_list = true;
                                    }
                                }

                                if (!is_in_list){
                                    pairId p;
                                    p.id = j;
                                    p.val = liste_objet[j].temps_recharge;

                                    liste_unite[i].liste_temps_recharge.push_back(p);
                                }
                            }


                        }
                    }

                    if (!ok){
                        liste_unite[i].engaging = 0;
                    }
                }

            }
        }
    }

    return true;
}

void Fenetre::check_lancement(){
    bool delete_complete = true;
    for (int i = 0; i < liste_unite_attente.size(); i++){
        liste_unite_attente[i].temps -= vitesse_jeu/fps_precedent;

        if (liste_unite_attente[i].temps <= 0){
            delete_complete = false;
            if (liste_objet[liste_unite_attente[i].obj].type == 0){
                tirer_missile(liste_unite_attente[i].id_unite, liste_unite_attente[i].obj, liste_unite_attente[i].id_cible, liste_unite_attente[i].id_element_ia);
            }
            else {
                lancer_avion(liste_unite_attente[i].id_unite, liste_unite_attente[i].obj, liste_unite_attente[i].nb_objet, liste_unite_attente[i].id_element_ia);
            }
        }
    }

    while (!delete_complete){
        bool done = false;
        delete_complete = true;
        for (int i = 0; i < liste_unite_attente.size() && !done; i++){
            if (liste_unite_attente[i].temps <= 0){
                done = true;
                delete_complete = false;

                if (liste_unite_attente[i].nb_objet != NULL){
                    delete liste_unite_attente[i].nb_objet;
                }

                liste_unite_attente.remove(i);
            }
        }
    }

}

void Fenetre::check_groupe (){
    int n = 0;
    for (int i = 0; i < 10; i++){
        bool done = false;
        while(!done){
            int index = -1;
            for (int j=0; j<liste_groupe[i].size(); j++){
                if (index_unite(liste_groupe[i][j]) == -1){
                    index = j;
                }
            }

            if (index == -1){
                done = true;
            }
            else {
                liste_groupe[i].remove(index);
            }
        }

        if (ctrl_appuye && chiffre_appuye[i]){
            liste_groupe[i].clear();

            for (int j = 0; j < liste_selection.size(); j++){
                liste_groupe[i].push_back(liste_selection[j]);
            }
        }
        else if (chiffre_appuye[i]){ //  && liste_groupe[i].size() > 0
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
                    erreur_log("in == -1");
                }

            }

            int current = onglet_selection->currentIndex();
            changement_tab(current);

            onglet_selection->setVisible(onglet_selection->count() > 0);
        }

        chiffre_appuye[i] = false;

        bouton_groupe[i]->setVisible(liste_groupe[i].size() > 0);

        if (liste_groupe[i].size() > 0){
            n ++;
        }
    }

    barre_bouton->setFixedWidth(90 * n);
    barre_bouton->setVisible(option.affichage_groupe);
}

bool Fenetre::check_info_texte ()
{
    QColor couleur;
    QFont police("Rockwell", 10);

    couleur.setRed(0);
    for (int i=liste_info_texte.size()-1; i>=0; i--)
    {
        liste_info_texte[i].alpha+=7;
        couleur.setAlpha(255-liste_info_texte[i].alpha/10);

        liste_info_texte[i].texte->setX(scene->width()/2-(liste_info_texte[i].nom.size()*8)/2);
        liste_info_texte[i].texte->setY(480-(liste_info_texte.size()-i)*25);

        liste_info_texte[i].texte->setDefaultTextColor(couleur);
        liste_info_texte[i].texte->setFont(police);

        if (liste_info_texte[i].alpha>2530)
        {
            destruction_info(i);
        }
    }

    /*if (fenetre_log_jeu != 0){
        bool p = fenetre_log_jeu->isVisible();
        construction_fenetre_log();
        fenetre_log_jeu->setVisible(p);
    }*/
}

void Fenetre::tirer_missile(int id_depart, int num_objet, int id_cible, int id_element_ia)
{
    int a=index_unite(id_depart);
    int b=index_unite(id_cible);
    bool index_sat = false;

    if (b == -1){
        for (int i = 0; i < liste_sat.size(); i++){
            if (id_cible == liste_sat[i].id){
                b = i;
                index_sat = true;
            }
        }
    }

    //setWindowTitle(QString::number(a) + " / " + QString::number(b) + " / " + QString::number(liste_unite[a].nombre_objet[num_objet]));

    if (a!=-1 && b!=-1){

        unite copie_unite = preparation_unite(liste_objet[num_objet].future_unite);

        copie_unite.place=0;
        copie_unite.destination_place=0;
        copie_unite.chg=1;
        copie_unite.chg_pixmap=1;
        copie_unite.chg_module=true;
        copie_unite.chg_objet=0;
        copie_unite.immobilise=false;
        copie_unite.detruite=0;
        copie_unite.en_deplacement=1;
        copie_unite.arret_en_cours=0;
        copie_unite.montee=1;
        copie_unite.atterrissage=0;
        copie_unite.cible=id_cible;
        copie_unite.type=liste_objet[num_objet].future_unite;
        copie_unite.fuel = liste_classe[copie_unite.type].fuel_base;

        copie_unite.angle=liste_unite[a].angle;

        copie_unite.vitesse=0;
        copie_unite.position.x=liste_unite[a].position.x;
        copie_unite.position.y=liste_unite[a].position.y;
        copie_unite.position.z=liste_unite[a].position.z;

        if (index_sat){
            copie_unite.destination.x=rand()%((int)(largeur_map * echelle / 100));
            copie_unite.destination.y=rand()%((int)(hauteur_map * echelle / 100));
            copie_unite.destination.x*=100;
            copie_unite.destination.y*=100;
            copie_unite.destination.z=(150 + rand()%100) * 1000 + rand()%1000;
        }
        else {
            copie_unite.destination.x=liste_unite[b].position.x;
            copie_unite.destination.y=liste_unite[b].position.y;
            copie_unite.destination.z=liste_unite[b].position.z;
        }

        copie_unite.altitude_voulue=copie_unite.destination.z;



        if (liste_classe[liste_unite[a].type].type == 0){
            double x1=liste_unite[a].position.x;
            double y1=liste_unite[a].position.y;
            double x2=copie_unite.destination.x;
            double y2=copie_unite.destination.y;

            copie_unite.angle=angleFromPosAndDes(x1, y1, x2, y2);
        }

        copie_unite.nom="Missile";

        if (id_element_ia != -1){
            copie_unite.liste_id_element_ia.push_back(id_element_ia);
        }

        liste_unite.push_back(copie_unite);

        //liste_unite[a].nombre_objet[num_objet]--;
        liste_unite[a].chg_objet=1;


    }
}

void Fenetre::preparation_lancer(int id_depart, int num_objet, int id_cible, int *nb_objet_unite, int id_element_ia){
    unite_attente u;
    int num_unit = index_unite(id_depart);

    if (num_objet <0 || num_objet >= NOMBRE_OBJET || num_unit == -1){
        erreur_log("num_objet out of range");
        return;
    }

    liste_unite[num_unit].nombre_objet[num_objet]--;

    u.obj = num_objet;
    u.id_unite = id_depart;
    u.nb_objet = new int[NOMBRE_OBJET];
    u.id_cible = id_cible;
    u.id_element_ia = id_element_ia;

    if (nb_objet_unite != NULL){
        for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
            u.nb_objet[i] = nb_objet_unite[i];

            liste_unite[num_unit].nombre_objet[i] -= nb_objet_unite[i];
        }
    }

    double min_time = std::numeric_limits<double>::max();
    for (unsigned int j = 0; j < NOMBRE_MODULE; j++){
        if (liste_module[j].peut_lancer[num_objet] && liste_classe[liste_unite[num_unit].type].module_present[j] && liste_unite[num_unit].vie_module[j] > 0){
            double local_max = 0;
            for (int i = 0; i < liste_unite_attente.size(); i++){
                if (liste_unite_attente[i].id_unite == id_depart && liste_unite_attente[i].module_lanceur == j){
                    if (liste_unite_attente[i].temps + liste_objet[liste_unite_attente[i].obj].temps_recharge > local_max){
                        local_max = liste_unite_attente[i].temps + liste_objet[liste_unite_attente[i].obj].temps_recharge;
                    }
                }
            }

            if (local_max < min_time){
                min_time = local_max;
                u.module_lanceur = j;
            }
        }
    }




    u.temps = min_time;


    liste_unite_attente.push_back(u);

}

void Fenetre::lancer_avion(int id_depart, int num_objet, int *nb_objet_unite, int id_element_ia)
{
    int a=index_unite(id_depart);

    if (a!=-1){

        unite copie_unite = preparation_unite(liste_objet[num_objet].future_unite);

        copie_unite.place=0;
        copie_unite.destination_place=0;
        copie_unite.chg=1;
        copie_unite.chg_pixmap=1;
        copie_unite.chg_module=1;
        copie_unite.chg_objet=0;
        copie_unite.immobilise=false;
        copie_unite.detruite=0;
        copie_unite.en_deplacement=1;
        copie_unite.arret_en_cours=0;
        copie_unite.montee=1;
        copie_unite.atterrissage=0;
        copie_unite.cible=-1;
        copie_unite.altitude_voulue=2000;
        copie_unite.type=liste_objet[num_objet].future_unite;

        int f = liste_classe[copie_unite.type].fuel_base;
        if (liste_unite[a].fuel > f){
            liste_unite[a].fuel -= f;
            copie_unite.fuel = f;
        }
        else {
            copie_unite.fuel = liste_unite[a].fuel;
            liste_unite[a].fuel = 0;
        }


        for (unsigned int i=0; i<NOMBRE_MODULE; i++)
        {
            if (liste_classe[liste_objet[num_objet].future_unite].module_present[i])
            {
                copie_unite.vie_module[i]=liste_module[i].pv;

                if (liste_module[i].type == 1){ // moteur
                    copie_unite.altitude_voulue= liste_module[i].altitude_croisiere;
                }
            }
            else
            {
                copie_unite.vie_module[i]=0;
            }
        }
        for (unsigned int i=0; i<NOMBRE_OBJET; i++){
            copie_unite.nombre_objet[i]=nb_objet_unite[i];

        }

        int dist = 100000;
        QString s = "Navion";
        double angle = 0;
        if (liste_objet[num_objet].type == 3 || liste_objet[num_objet].type == 6){
            dist = -100;
            s = "Bato";
            angle = PI;
            copie_unite.altitude_voulue=0;
        }
        else if (liste_objet[num_objet].type == 5){
            dist = 100;
            s = "Batallion";
            copie_unite.altitude_voulue=0;
        }
        else if (liste_objet[num_objet].type == 8){
            dist = 0;
            s = "Buoy";
            copie_unite.altitude_voulue=0;
        }

        copie_unite.angle=liste_unite[a].angle + angle;
        copie_unite.vitesse=liste_unite[a].vitesse;
        copie_unite.position.x=liste_unite[a].position.x;
        copie_unite.position.y=liste_unite[a].position.y;
        copie_unite.position.z=liste_unite[a].position.z;
        copie_unite.destination.x=liste_unite[a].position.x + cos(liste_unite[a].angle)*dist;
        copie_unite.destination.y=liste_unite[a].position.y + sin(liste_unite[a].angle)*dist;
        copie_unite.destination.z=0;

        if (liste_objet[num_objet].type == 8){
            copie_unite.vitesse = 0;
        }

        copie_unite.nom=s;

        if (id_element_ia != -1){
            copie_unite.liste_id_element_ia.push_back(id_element_ia);
        }

        liste_unite.push_back(copie_unite);

        //liste_unite[a].nombre_objet[num_objet]--;
        liste_unite[a].chg_objet=1;
    }

}

void Fenetre::destruction_unite(int id)
{
    int j=index_unite(id);
    if (j!=-1)
    {
        if (liste_unite[j].place){
            scene->removeItem(liste_unite[j].pixmap);
            delete liste_unite[j].pixmap;
        }
        if (liste_unite[j].destination_place){
            scene->removeItem(liste_unite[j].pixmap_destination);
            delete liste_unite[j].pixmap_destination;
        }

        for (int i = 0; i < liste_unite[j].liste_graphical_item.size(); i++){
            scene->removeItem(liste_unite[j].liste_graphical_item[i].item);
            delete liste_unite[j].liste_graphical_item[i].item;
        }

        delete liste_unite[j].peut_lancer;
        delete liste_unite[j].vie_module;
        delete liste_unite[j].nombre_objet;
    }
}

void Fenetre::destruction_info(int index)
{
    scene->removeItem(liste_info_texte[index].texte);
    for (unsigned int i=index; i<liste_info_texte.size()-1; i++)
    {
        liste_info_texte[i]=liste_info_texte[i+1];
    }
    liste_info_texte.pop_back();
}

void Fenetre::ajout_info_texte(QString text)
{
    info_texte copie;

    copie.alpha=0;
    copie.nom=text;
    copie.place=0;
    copie.texte=new QGraphicsTextItem(text);

    scene->addItem(copie.texte);
    liste_info_texte.push_back(copie);

    double t = temps + fps/fps_precedent;
    liste_log.push_back(time_to_string(t) + " : " + text);
}

int Fenetre::index_unite (int id)
{
    for (unsigned int i=0; i<liste_unite.size(); i++)
    {
        if (liste_unite[i].id==id)
        {
            return i;
        }
    }
    return -1;
}

bool Fenetre::est_selectionne (int id)
{
    for (int i=0; i<liste_selection.size(); i++)
    {
        if (liste_selection[i]==id)
        {
            return 1;
        }
    }
    return 0;
}

int Fenetre::attribution_id ()
{
    id++;
    return id;
}

void Fenetre::construction_info(int id, QWidget * super)
{
    int i=index_unite(id);
    if (i!=-1)
    {
        liste_unite[i].chg=1;

        QScrollArea *scroll = new QScrollArea(super);
        scroll->verticalScrollBar()->setStyleSheet(load_file("data/style/scroll.css"));
        scroll->horizontalScrollBar()->setObjectName("scroll");
        scroll->setObjectName("scroll");
        scroll->setWidgetResizable(true);
        QWidget *super2 = new QWidget(scroll);
        super2->setObjectName("super2");

        QGroupBox *groupe = new QGroupBox("Command", super);
        QGroupBox *groupe_infoi = new QGroupBox("Info", super2);
        QGroupBox *groupe_module = new QGroupBox("Module", super2);
        QGroupBox *groupe_missile = new QGroupBox("Missile/Ammo", super2);
        QGroupBox *groupe_avion = new QGroupBox("Plane", super2);
        QGroupBox *groupe_helo = new QGroupBox("Helo", super2);
        QGroupBox *groupe_well_dock = new QGroupBox("Vehicle/Inf", super2);
        QVBoxLayout *layout_module = new QVBoxLayout;
        QHBoxLayout *layout_missile = new QHBoxLayout;
        QHBoxLayout *layout_avion = new QHBoxLayout;
        QHBoxLayout *layout_helo = new QHBoxLayout;
        QHBoxLayout *layout_well_dock = new QHBoxLayout;
        QWidget *sous1 = new QWidget(groupe);
        QWidget *sous2 = new QWidget(groupe);

        QHBoxLayout *scroll_layout = new QHBoxLayout;
        QHBoxLayout *super_layout = new QHBoxLayout;
        QVBoxLayout *super_layout2 = new QVBoxLayout;
        QVBoxLayout *layout = new QVBoxLayout;
        QVBoxLayout *info_layout = new QVBoxLayout;
        QHBoxLayout *layout1 = new QHBoxLayout;
        QHBoxLayout *layout2 = new QHBoxLayout;

        groupe->setObjectName("groupe");
        groupe_infoi->setObjectName("groupe_info");
        groupe_module->setObjectName("groupe_module");
        groupe_missile->setObjectName("groupe_missile");
        groupe_avion->setObjectName("groupe_avion");
        groupe_helo->setObjectName("groupe_helo");
        groupe_well_dock->setObjectName("groupe_well_dock");

        sous1->setObjectName("sous1");
        sous2->setObjectName("sous2");

        QLineEdit *champ_actioni = new QLineEdit("", groupe);
        QPushButton* validation_actioni = new QPushButton("Send order", sous1);
        QLabel* label_actioni = new QLabel("I'm waiting for your order Admiral !", sous2);
        QCheckBox* groupe_actioni = new QCheckBox("Group order", sous1);

        champ_actioni->setObjectName("champ_action");
        validation_actioni->setObjectName("validation_action");
        label_actioni->setObjectName("label_action");
        groupe_actioni->setObjectName("groupe_action");

        QObject::connect(champ_actioni, SIGNAL(textEdited(const QString &)), this, SLOT(devin(const QString &)));

        layout1->addWidget(validation_actioni);
        layout2->addWidget(label_actioni);
        layout1->addWidget(groupe_actioni);

        sous1->setLayout(layout1);
        sous2->setLayout(layout2);

        layout->addWidget(champ_actioni);
        layout->addWidget(sous1);
        layout->addWidget(sous2);
        groupe->setLayout(layout);

        QLabel* info_altitudei=new QLabel(groupe_infoi);
        QLabel* info_altitude_vouluei=new QLabel(groupe_infoi);
        QLabel* info_vitessehi=new QLabel("Speed : " + QString::number((int)liste_unite[i].vitesse),groupe_infoi);
        QLabel* info_vitessevi=new QLabel(groupe_infoi);

        info_layout->addWidget(info_altitudei);
        info_layout->addWidget(info_altitude_vouluei);
        info_layout->addWidget(info_vitessehi);
        info_layout->addWidget(info_vitessevi);

        info_altitudei->setObjectName("info_altitude");
        info_altitude_vouluei->setObjectName("info_altitude_voulue");
        info_vitessehi->setObjectName("info_vitesseh");
        info_vitessevi->setObjectName("info_vitessev");

        //super2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //groupe_infoi->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        if (liste_classe[liste_unite[i].type].type==0 || liste_classe[liste_unite[i].type].type==5 || liste_classe[liste_unite[i].type].type==6 || liste_classe[liste_unite[i].type].type==7){
            info_altitudei->setVisible(false);
            info_altitude_vouluei->setVisible(false);
            info_vitessevi->setVisible(false);

            /*info_altitude.push_back(info_altitudei);
            info_altitude_voulue.push_back(info_altitude_vouluei);
            info_vitesseh.push_back(info_vitessehi);
            info_vitessev.push_back(info_vitessevi);*/
        }
        else
        {
            if (liste_unite[i].position.z==liste_unite[i].altitude_voulue)
            {
                info_altitudei->setText("Altitude : " + QString::number((int)liste_unite[i].position.z));
                info_altitude_vouluei->setText("");
                info_vitessehi->setText("Speed : " + QString::number((int)liste_unite[i].vitesse));
                info_vitessevi->setText("");

                info_altitude_vouluei->setVisible(false);
                info_vitessevi->setVisible(false);
            }
            else
            {
                info_altitudei->setText("Current altitude : " + QString::number((int)liste_unite[i].position.z));
                info_altitude_vouluei->setText("Cruise altitude : " + QString::number((int)liste_unite[i].altitude_voulue));
                info_vitessehi->setText("Horizontal Speed : " + QString::number((int)liste_unite[i].vitesse*cos(liste_unite[i].angle_montee)));
                info_vitessevi->setText("Vertical Speed : " + QString::number((int)liste_unite[i].vitesse*sin(liste_unite[i].angle_montee)));
            }
        }

        QLabel* info_fueli=new QLabel("Fuel left : " + QString::number((int)liste_unite[i].fuel/liste_unite[i].conso) + " s",groupe_infoi);
        info_fueli->setObjectName("info_fuel");
        //info_fuel.push_back(info_fueli);
        info_layout->addWidget(info_fueli);

        if (liste_unite[i].conso == 0){
            info_fueli->setVisible(false);
        }

        QLabel* info_persoi=new QLabel(groupe_infoi);
        info_persoi->setObjectName("info_perso");
        info_layout->addWidget(info_persoi);

        if (liste_classe[liste_unite[i].type].type == 5){


            for (unsigned int j=0; j<NOMBRE_MODULE; j++){
                if (liste_classe[liste_unite[i].type].module_present[j] && liste_module[j].type == 5){
                    info_persoi->setText("Personel : " + QString::number(liste_unite[i].vie_module[j]) + " / " + QString::number(liste_module[j].pv));
                }
            }

            //info_personel.push_back(info_persoi);

        }
        else {
            info_persoi->setVisible(false);
        }

        groupe_infoi->setLayout(info_layout);




        /*champ_action.push_back(champ_actioni);
        validation_action.push_back(validation_actioni);
        label_action.push_back(label_actioni);
        groupe_action.push_back(groupe_actioni);*/



        for (unsigned int j=0; j<NOMBRE_OBJET; j++)
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

            widg->setObjectName("obj_" + QString::number(j));

            QVBoxLayout *layoutbla = new QVBoxLayout;

            QLabel *label_nomi = new QLabel(liste_objet[j].nom, widg);
            QLabel *label_nombrei = new QLabel(QString::number(liste_unite[i].nombre_objet[j]) + " left", widg);
            QLabel *label_imagei = new QLabel(widg);
            label_imagei->setPixmap(*liste_objet[j].image);

            label_nomi->setObjectName("label_nom");
            label_nombrei->setObjectName("label_nombre");
            label_imagei->setObjectName("label_image");

            layoutbla->addWidget(label_nomi);
            layoutbla->addWidget(label_imagei);
            layoutbla->addWidget(label_nombrei);


            /*liste_objet[j].label_nom.push_back(label_nomi);
            liste_objet[j].label_image.push_back(label_imagei);
            liste_objet[j].label_nombre.push_back(label_nombrei);*/


            widg->setLayout(layoutbla);

            if (liste_objet[j].type==0 || liste_objet[j].type==2 || liste_objet[j].type==8){
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

            if (liste_unite[i].nombre_objet[j]<=0)
            {
                widg->setVisible(false);
            }
        }

        int compteur = 0;

        for (unsigned int j=0; j<NOMBRE_MODULE; j++)
        {
            if (liste_classe[liste_unite[i].type].module_present[j] && liste_module[j].type != 5)
            {
                QWidget *widg = new QWidget(groupe_module);
                widg->setObjectName("module_" + QString::number(j));

                QHBoxLayout *layoutbla = new QHBoxLayout;

                /*liste_module[j].label_nom= new QLabel(liste_module[j].nom, widg);
                liste_module[j].label_vie= new QLabel("   -    HP : " + QString::number(liste_unite[i].vie_module[j]) + " / " + QString::number(liste_module[j].pv), widg);

                layoutbla->addWidget(liste_module[j].label_nom);
                layoutbla->addWidget(liste_module[j].label_vie);

                liste_module[j].label_nom->setVisible(true);
                liste_module[j].label_vie->setVisible(true);*/

                QLabel* label_nom = new QLabel(liste_module[j].nom, widg);
                //QLabel* label_vie = new QLabel("   -    HP : " + QString::number(liste_unite[i].vie_module[j]) + " / " + QString::number(liste_module[j].pv), widg);
                QLabel* label_vie = new QLabel(widg);
                label_vie->setPixmap(creation_pixmap_vie(i, j));
                //label_vie->pixmap()->save("kapoue2.png");


                label_nom->setObjectName("label_nom");
                label_vie->setObjectName("label_vie");

                layoutbla->addWidget(label_nom);
                layoutbla->addWidget(label_vie);

                widg->setLayout(layoutbla);

                layout_module->addWidget(widg);

                compteur++;

                coo c;
                c.x = j;
                c.y = liste_unite[i].vie_module[j];
                liste_unite[i].liste_ancienne_vie_module.push_back(c);
            }
        }
        if (compteur == 0){
            groupe_module->setVisible(false);
        }

        groupe_module->setLayout(layout_module);
        groupe_missile->setLayout(layout_missile);
        groupe_avion->setLayout(layout_avion);
        groupe_helo->setLayout(layout_helo);
        groupe_well_dock->setLayout(layout_well_dock);




        super_layout2->addWidget(groupe_infoi);
        super_layout2->addWidget(groupe_module);
        super_layout2->addWidget(groupe_missile);
        super_layout2->addWidget(groupe_avion);
        super_layout2->addWidget(groupe_helo);
        super_layout2->addWidget(groupe_well_dock);

        //groupe_info.push_back(groupe_infoi);

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



        super2->setLayout(super_layout2);
        scroll_layout->addWidget(super2);
        scroll->setLayout(scroll_layout);

        scroll->setWidget(super2);

        super_layout->addWidget(groupe);
        super_layout->addWidget(scroll);
        super->setLayout(super_layout);

        QObject::connect(validation_actioni, SIGNAL(clicked()), this, SLOT(envoi_ordre()));
    }
}

bool Fenetre::envoi_ordre (){
    int index=onglet_selection->currentIndex();

    int j = index_unite(liste_selection[index]);

    if (j < 0 || j >= liste_unite.size()){
        erreur_log("Sending order to a non existing unit");
        return false;
    }

    QGroupBox *groupe = liste_unite[j].widg_selection->findChild<QGroupBox *>("groupe");
    QWidget *sous1 = 0;
    QWidget *sous2 = 0;
    QLineEdit *champ_actioni = 0;
    QCheckBox* groupe_actioni = 0;
    QLabel* label_actioni = 0;

    if (groupe == 0){
        erreur_log("Groupe is NULL");
        return false;
    }

    sous1 = groupe->findChild<QWidget *>("sous1");
    sous2 = groupe->findChild<QWidget *>("sous2");

    if (sous1 == 0 || sous2 == 0){
        erreur_log("sous? is NULL");
        return false;
    }

    champ_actioni = groupe->findChild<QLineEdit *>("champ_action");
    groupe_actioni = sous1->findChild<QCheckBox *>("groupe_action");
    label_actioni = sous2->findChild<QLabel *>("label_action");

    if (champ_actioni == 0 || groupe_actioni == 0 || label_actioni == 0){
        erreur_log("????_action is NULL");
        return false;
    }


    QString a = champ_actioni->text();


    if (groupe_actioni->isChecked())
    {
        for (int i=0; i<liste_selection.size(); i++)
        {
            ordre_unite (liste_selection[i], a, label_actioni);
        }
    }
    else
    {
        ordre_unite (liste_selection[index], a, label_actioni);
    }

    champ_actioni->clear();

    return true;
}

bool Fenetre::ordre_unite (int id_unite, QString a, QLabel* label)
{
    if (label == 0){
        return false;
    }

    a = a.simplified();

    if (a.endsWith(' ')){
        a = a.remove(a.length()-1, 1);
    }

    bool ok=0;
    int j=index_unite(id_unite);
    if (j!=-1)
    {
        if (a.contains("stop") && !ok)
        {

            liste_unite[j].en_deplacement=0;
            liste_unite[j].arret_en_cours=1;
            label->setText("Stopped !");

            ok=1;
        }

        if((a.startsWith("fire",Qt::CaseInsensitive) || a.startsWith("drop",Qt::CaseInsensitive)) && !ok)  // tir de missiles / obus / bombes
        {
            a = a.right(a.length()-4);
            int k= index_unite(liste_unite[j].cible);
            bool pas_tirer=0;
            bool pas_toucher=0;
            bool existe_pas = true;
            bool pas_assez = false;
            bool pas_bonne_portee = false;
            if (k!=-1)
            {
                int num_objet=-1;
                int nb = 1;
                /*for (unsigned int i=0; i<NOMBRE_OBJET; i++)  // priorite si la phrase contient le nom du missile
                {
                    if (liste_objet[i].type==0 && a.contains(liste_objet[i].nom) && liste_unite[j].nombre_objet[i]>0 && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i])  // ok
                    {
                        num_objet=i;
                    }
                    if (liste_objet[i].type==0 && a.contains(liste_objet[i].nom) && liste_unite[j].nombre_objet[i]>0 && !liste_unite[j].peut_lancer[i])
                    {
                        pas_tirer=1;
                    }
                    if (liste_objet[i].type==0 && a.contains(liste_objet[i].nom) && liste_unite[j].nombre_objet[i]>0 && !liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i])
                    {
                        pas_toucher=1;
                    }
                }
                for (unsigned int i=0; i<NOMBRE_OBJET && num_objet==-1; i++)  // sinon on tire le premier qu'on trouve
                {
                    if (liste_objet[i].type==0 && liste_unite[j].nombre_objet[i]>0 && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i])
                    {
                        num_objet=i;
                    }
                    if (liste_objet[i].type==0 && liste_unite[j].nombre_objet[i]>0 && !liste_unite[j].peut_lancer[i])
                    {
                        pas_tirer=1;
                    }
                    if (liste_objet[i].type==0 && liste_unite[j].nombre_objet[i]>0 && !liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i])
                    {
                        pas_toucher=1;
                    }
                }*/

                a = a.simplified();

                QStringList li = a.split(' ');
                QString nom = a;
                bool nom_donne = true;

                if (a == ""){
                    nom_donne = false;
                    nom = "";
                    nb = 1;
                }
                else if (li.size() == 1){
                    if (a.toInt() > 0){
                        nb = a.toInt();
                        nom = "";
                        nom_donne = false;
                    }
                    else {
                        nb = 1;
                    }
                }
                else if (li.size() > 1){
                    nb = li[0].toInt();
                    nom = li[1];
                }

                double d = distance(liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z, liste_unite[k].position.x, liste_unite[k].position.y, liste_unite[k].position.z);

                for (unsigned int i=0; i<NOMBRE_OBJET && num_objet==-1; i++){  // priorite si la phrase contient le nom du missile
                    /*if (liste_objet[i].type==0 && a.contains(liste_objet[i].nom) && liste_unite[j].nombre_objet[i]>0 && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i]){  // ok
                        num_objet=i;
                    }*/

                    if (liste_objet[i].type==0 || liste_objet[i].type==2){
                        if (nom.contains(liste_objet[i].nom,Qt::CaseInsensitive)){
                            existe_pas = false;
                        }

                        if (nom.contains(liste_objet[i].nom,Qt::CaseInsensitive) && liste_unite[j].nombre_objet[i]>=nb && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i] && (!liste_objet[i].type == 2 || liste_objet[i].data.portee >= d)){
                            num_objet=i;
                        }
                        else if (nom.contains(liste_objet[i].nom,Qt::CaseInsensitive) && liste_unite[j].nombre_objet[i]<nb){
                            pas_assez = true;
                        }
                        else if (nom.contains(liste_objet[i].nom,Qt::CaseInsensitive) && liste_unite[j].nombre_objet[i]>=nb && !liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type]){
                            pas_toucher = true;
                        }
                        else if (nom.contains(liste_objet[i].nom,Qt::CaseInsensitive) && liste_unite[j].nombre_objet[i]>=nb && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && !liste_unite[j].peut_lancer[i]){
                            pas_tirer = true;
                        }
                        else if (liste_objet[i].type == 2 && nom.contains(liste_objet[i].nom,Qt::CaseInsensitive) && liste_unite[j].nombre_objet[i]>=nb && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i] && liste_objet[i].data.portee < d){
                            pas_bonne_portee = true;
                        }
                    }
                }

                for (unsigned int i=0; i<NOMBRE_OBJET && num_objet==-1; i++){
                    if (liste_objet[i].type==0 || liste_objet[i].type==2){
                        if (liste_unite[j].nombre_objet[i]>=nb && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && liste_unite[j].peut_lancer[i]){
                            num_objet=i;
                        }
                        else if (liste_unite[j].nombre_objet[i]<nb){
                            pas_assez = true;
                        }
                        else if (liste_unite[j].nombre_objet[i]>=nb && !liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type]){
                            pas_toucher = true;
                        }
                        else if (liste_unite[j].nombre_objet[i]>=nb && liste_objet[i].peut_toucher[liste_classe[liste_unite[k].type].type] && !liste_unite[j].peut_lancer[i]){
                            pas_tirer = true;
                        }
                    }
                }



                if (num_objet!=-1 && (!nom_donne || !existe_pas)){


                    if (nb==1){
                        label->setText(liste_objet[num_objet].nom + " fired at " + liste_classe[liste_unite[k].type].nom + " sir !");
                        ajout_info_texte(liste_objet[num_objet].nom + " fired at " + liste_classe[liste_unite[k].type].nom);
                    }
                    else{
                        label->setText(QString::number(nb) + " " + liste_objet[num_objet].nom + "s fired at " + liste_classe[liste_unite[k].type].nom + " sir !");
                        ajout_info_texte(QString::number(nb) + " " + liste_objet[num_objet].nom + "s fired at " + liste_classe[liste_unite[k].type].nom);
                    }

                    if (liste_objet[num_objet].type==0){
                        for (int i=0; i<nb; i++){
                            preparation_lancer(liste_unite[j].id, num_objet, liste_unite[k].id, 0, -1);
                            //tirer_missile(liste_unite[j].id, num_objet, liste_unite[k].id);
                        }
                    }
                    else if (liste_objet[num_objet].type==2){
                        bool found = false;
                        for (int i = 0; i < liste_unite[j].liste_tir_obus.size(); i++){
                            if (liste_unite[j].liste_tir_obus[i].x == num_objet){
                                found = true;
                                liste_unite[j].liste_tir_obus[i].y += nb;
                            }
                        }

                        if (!found){
                            coo c;
                            c.x = num_objet;
                            c.y = nb;
                            liste_unite[j].liste_tir_obus.push_back(c);
                        }

                        liste_unite[j].engaging = 2;
                    }
                    else {
                        erreur_log("pas bon type");
                    }


                }
                else if (pas_tirer){
                    label->setText("We don't have the equipment to fire this missile sir !");
                }
                else if (pas_toucher){
                    label->setText("We can't hit the target with that missile sir !");
                }
                else if (existe_pas && nom_donne){
                    label->setText("This missile doesn't exit Sir !");
                }
                else if (pas_assez){
                    label->setText("We don't have enough missile Sir !");
                }
                else if (pas_bonne_portee){
                    label->setText("We are too far away to fire this Sir !");
                }
                else{
                    label->setText("We don't have any missile sir !");
                }
            }
            else
            {
                label->setText("We don't have any target sir !");

            }


            ok=true;
        }

        if((a.startsWith("launch",Qt::CaseInsensitive) || a.startsWith("disembark",Qt::CaseInsensitive) || a.startsWith("drop",Qt::CaseInsensitive)) && !ok)  // lancer vehicule/inf
        {
            QStringList liste = a.simplified().split(" ");

            bool lance = false;
            bool cant_launch = false;
            int index_objet=-1;
            int nombre = 1;
            bool bon_type = false;
            bool zero_obj = false;
            bool not_enough_object = false;
            bool not_enough_unit = false;
            bool not_enough_space = false;
            bool not_good_terrain = false;
            bool name_existing = false;
            bool config_existing = false;
            int* nombre_obj;

            nombre_obj = new int[NOMBRE_OBJET];

            if (nombre_obj == NULL){
                exit(666);
            }

            for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                nombre_obj[i] = 0;
            }

            QString nom("");
            if (liste.size() == 2){
                nom = liste[1];
            }
            else if (liste.size() >= 3){
                nom = liste[2];
            }

            if (liste.size() == 1 || liste.size() == 2 || liste.size() >= 3){
                if (liste.size() >= 3){
                    nombre = liste[1].toInt();
                }
                else {
                    nombre = 1;
                }

                if (nombre > 0){
                    for (unsigned int i=0; i<NOMBRE_OBJET && !lance && !cant_launch; i++){

                        if (a.startsWith("launch",Qt::CaseInsensitive)){
                            bon_type = liste_objet[i].type==1 || liste_objet[i].type==3 || liste_objet[i].type==5 || liste_objet[i].type==6 || liste_objet[i].type==7 || liste_objet[i].type==8;
                        }
                        else if (a.startsWith("disembark",Qt::CaseInsensitive)){
                            bon_type = liste_objet[i].type==5;
                        }
                        else if (a.startsWith("drop",Qt::CaseInsensitive)){
                            bon_type = liste_objet[i].type==8;
                        }

                        if (liste.size() == 1){

                            if (bon_type && liste_unite[j].nombre_objet[i]>0 && liste_unite[j].peut_lancer[i]){
                                lance = true;
                                index_objet=i;
                                nom = liste_objet[i].nom;
                            }
                        }
                        else {


                            if (liste_objet[i].nom == nom){
                                name_existing = true;
                            }

                            if (bon_type && liste_objet[i].nom == nom && liste_unite[j].nombre_objet[i]>= nombre && liste_unite[j].peut_lancer[i]){
                                lance = true;
                                index_objet=i;
                            }
                            else if (bon_type && liste_objet[i].nom == nom && !liste_unite[j].peut_lancer[i]){
                                cant_launch = true;
                                index_objet=i;
                            }
                            else if (bon_type && liste_objet[i].nom == nom && !liste_unite[j].nombre_objet[i]>= nombre){
                                not_enough_unit = true;
                                index_objet=i;
                            }
                        }

                    }
                }
             }

             if (index_objet >= 0 && index_objet < NOMBRE_OBJET){

                 int type = liste_objet[index_objet].type;
                 coo pos;
                 pos.x = liste_unite[j].position.x/(TAILLE_MAP2)/echelle;
                 pos.y = liste_unite[j].position.y/(TAILLE_MAP2)/echelle;

                 if (pos.x < 0 || pos.y < 0 || pos.x >= largeur_map/TAILLE_MAP2 || pos.y >= hauteur_map/TAILLE_MAP2){
                     not_good_terrain = true;

                 }
                 else {
                     if ((type == 6 || type == 8) && map2[pos.x][pos.y] == 1){
                         not_good_terrain = true;
                     }
                     else if ((type == 4 || type == 5) && map2[pos.x][pos.y] == 0 && map_transition[pos.x][pos.y] == 0){
                         not_good_terrain = true;
                     }
                 }



                 if (liste.size() == 6 && liste[5] == "config"){ // launch x 'class_name' with default config  |  launch x 'class_name' with 'config_name' config
                    if (liste[4] == "default"){
                        config_existing = true;
                        if (liste_objet[index_objet].liste_config.size() > 0){
                            for (int l = 0; l < liste_objet[index_objet].liste_config[0].liste.size(); l++){
                                nombre_obj[liste_objet[index_objet].liste_config[0].liste[l].x] = liste_objet[index_objet].liste_config[0].liste[l].y;
                            }
                        }
                    }
                    else {
                        for (int k = 0; k < liste_objet[index_objet].liste_config.size(); k++){
                            if (liste_objet[index_objet].liste_config[k].nom == liste[4]){
                                config_existing = true;
                                for (int l = 0; l < liste_objet[index_objet].liste_config[0].liste.size(); l++){
                                    nombre_obj[liste_objet[index_objet].liste_config[k].liste[l].x] = liste_objet[index_objet].liste_config[k].liste[l].y;
                                }
                            }
                        }
                    }
                }
                else if (liste.size() >= 6 && liste.size()%2 == 0){
                    for (int i = 4; i < liste.size()-1; i+=2){
                        int nb = liste[i].toInt();

                        if (nb <= 0){
                            zero_obj = true;
                        }
                        else {
                            for (unsigned int k = 0; k < NOMBRE_OBJET; k++){
                                if (liste_objet[k].nom == liste[i+1]){
                                    nombre_obj[k] = nb;
                                }
                            }
                        }
                    }
                }

                double hangar[4];
                hangar[0] = liste_classe[liste_objet[index_objet].future_unite].hangar[0];
                hangar[1] = liste_classe[liste_objet[index_objet].future_unite].hangar[1];
                hangar[2] = liste_classe[liste_objet[index_objet].future_unite].hangar[2];
                hangar[3] = liste_classe[liste_objet[index_objet].future_unite].hangar[3];

                for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                    int type = liste_objet[i].type;

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
                    else if (type == 8){ // buoy goes to ammo
                        type = 0;
                    }

                    if (type < 0 || type > 3){
                        erreur_log("Erreur type launch");
                    }

                    hangar[type] -= nombre_obj[i] * liste_objet[i].taille;
                }

                if (hangar[0] < 0 || hangar[1] < 0 || hangar[2] < 0 || hangar[3] < 0){
                    not_enough_space = true;
                }

                for (unsigned int i = 0; i < NOMBRE_OBJET; i++){
                    if (liste_unite[j].nombre_objet[i] < nombre * nombre_obj[i]){
                        not_enough_object = true;
                    }
                }
             }




            if (cant_launch){
                label->setText("Sorry sir, we can't launch any " + nom);
            }
            else if (not_good_terrain){
                label->setText("This unit can't be launched at this position !");
            }
            else if (not_enough_unit){
                label->setText("We don't have enough " + nom + "(s) !");
            }
            else if (liste.size() == 6 && liste[5] == "config" && !config_existing){
                label->setText("This unit doesn't have a configuration named '" + liste[4] + "'");
            }
            else if (not_enough_object){
                label->setText("We don't have enough ordnance to launch with this unit !"); // a changer
            }
            else if (not_enough_space){
                label->setText("The unit you want to launch can't have that much ordnance !");
            }
            else if (lance){
                if (liste.size() == 6 && liste[5] == "config"){
                    label->setText(QString::number(nombre) + " " + nom + "(s) launched with " + liste[4] + " config sir !");
                }
                else if (liste.size() >= 6 && liste.size()%2 == 0){
                    label->setText(QString::number(nombre) + " " + nom + "(s) launched with the given ordnance sir !");
                }
                else {
                    label->setText(QString::number(nombre) + " " + nom + "(s) launched sir !");
                }

                for (int i = 0; i < nombre; i++){
                    preparation_lancer(liste_unite[j].id, index_objet, -1, nombre_obj, -1);
                    //lancer_avion(liste_unite[j].id, index_objet, nombre_obj);
                }

            }
            else if (!name_existing && liste.size() >= 2){
                label->setText("I don't know what a " + nom + " is !");
            }
            else {
                label->setText("We can't launch any units sir !");
            }
            ok = true;
            delete nombre_obj;
        }

        if(a.contains("land") && !ok)  // atterissage
        {
            if ((liste_classe[liste_unite[j].type].type==2 || liste_classe[liste_unite[j].type].type==4) && !liste_unite[j].atterrissage)
            {
                int k=index_unite(liste_unite[j].cible_ami);

                if (k!=-1)
                {
                    int id_objet=-1;
                    for (unsigned int i=0; i<NOMBRE_OBJET; i++)
                    {
                        if (liste_objet[i].future_unite==liste_unite[j].type)
                        {
                            id_objet=i;
                        }
                    }
                    if (id_objet!=-1 && liste_unite[k].peut_lancer[id_objet])
                    {
                        coo pos;
                        pos.x = liste_unite[k].position.x-cos(liste_unite[k].angle)*10000;
                        pos.y = liste_unite[k].position.y-sin(liste_unite[k].angle)*10000;

                        if (deplacement_unite(j, pos, false, true)){
                            liste_unite[j].altitude_voulue=100;
                            liste_unite[j].montee=1;
                            liste_unite[j].en_deplacement=1;
                            liste_unite[j].atterrissage=1;

                            label->setText("We will be landing soon !");
                        }
                        else {
                            liste_unite[j].atterrissage=0;

                            label->setText("We can't land at that position Sir !");
                        }


                    }
                    else
                    {
                        label->setText("We can't land on that sir !");

                    }

                }
                else
                {
                    label->setText("Can you precise the area of landing sir ?");

                }
            }
            else
            {
                label->setText("Are you kidding sir ?");

            }

            ok=1;
        }

        if(a.contains("embark") && !ok)  // atterissage
        {
            if (liste_classe[liste_unite[j].type].type!=1 && liste_classe[liste_unite[j].type].type!=2 && liste_classe[liste_unite[j].type].type!=4 && liste_classe[liste_unite[j].type].type!=7 && liste_classe[liste_unite[j].type].type!=8 && !liste_unite[j].atterrissage)
            {
                int k=index_unite(liste_unite[j].cible_ami);

                if (k!=-1)
                {
                    int id_objet=-1;
                    for (unsigned int i=0; i<NOMBRE_OBJET; i++)
                    {
                        if (liste_objet[i].future_unite==liste_unite[j].type)
                        {
                            id_objet=i;
                        }
                    }
                    if (id_objet!=-1 && liste_unite[k].peut_lancer[id_objet])
                    {



                        liste_unite[j].atterrissage=3;

                        coo pos;
                        pos.x = liste_unite[k].position.x;
                        pos.y = liste_unite[k].position.y;

                         if (deplacement_unite(j, pos, false, true)){
                             liste_unite[j].altitude_voulue=liste_unite[k].position.z;
                             liste_unite[j].montee=1;
                             liste_unite[j].en_deplacement=1;

                             label->setText("We will be embarking soon !");
                         }
                         else {
                             label->setText("We can't embark at this position Sir !");

                             liste_unite[j].atterrissage=0;
                         }


                    }
                    else
                    {
                        label->setText("We can't embark on that sir !");

                    }

                }
                else
                {
                    label->setText("Can you precise the area of embarkment sir ?");

                }
            }
            else
            {
                label->setText("Are you kidding sir ?");

            }

            ok=1;
        }

        if((a.startsWith("climb",Qt::CaseInsensitive) || a.startsWith("go up",Qt::CaseInsensitive) || a.startsWith("rise",Qt::CaseInsensitive) || a.startsWith("go down",Qt::CaseInsensitive) || a.startsWith("descend",Qt::CaseInsensitive)  || a.startsWith("dive",Qt::CaseInsensitive)) && !ok)  // montee descente
        {
            if (liste_classe[liste_unite[j].type].type==2 || liste_classe[liste_unite[j].type].type==3 || liste_classe[liste_unite[j].type].type==4)
            {
                a=a.toLower();
                QString mot;
                if (a.startsWith("climb",Qt::CaseInsensitive))
                {
                    mot="climb";
                }
                if (a.startsWith("go up",Qt::CaseInsensitive))
                {
                    mot="go up";
                }
                if (a.startsWith("rise",Qt::CaseInsensitive))
                {
                    mot="rise";
                }
                if (a.startsWith("descend",Qt::CaseInsensitive))
                {
                    mot="descend";
                }
                if (a.startsWith("go down",Qt::CaseInsensitive))
                {
                    mot="go down";
                }
                if (a.startsWith("dive",Qt::CaseInsensitive))
                {
                    mot="dive";
                }

                a=a.remove(mot);

                if (a.contains("to"))
                {
                    a=a.remove("to");
                }

                double valeur=1;
                int altitude=-1;

                if (a.contains("m"))
                {
                    a=a.remove("m");
                    valeur=1;
                }
                if (a.contains("km"))   // ajouter unites barbares
                {
                    a=a.remove("km");
                    valeur=1000;
                }
                bool fine = false;
                altitude=a.toInt(&fine);


                if (fine)
                {
                    int x = liste_unite[j].position.x / echelle;
                    int y = liste_unite[j].position.y / echelle;

                    if ((x < 0 || y < 0 || x >= largeur_map || y >= hauteur_map)){
                        return false;
                    }

                    if (map[x][y].altitude > altitude*valeur){
                        label->setText("We can't go at that altitude here Sir. The ground is too high ! ");
                    }
                    else if (liste_unite[j].en_deplacement && altitude*valeur <= liste_unite[j].altitude_max_trajet){
                        label->setText("We can't go at that altitude Sir. Some obstacles ahead are higher !");
                    }
                    else if (altitude*valeur <= liste_unite[j].altitude_max && altitude*valeur >= liste_unite[j].altitude_min)
                    {
                        liste_unite[j].altitude_voulue=altitude*valeur;
                        liste_unite[j].montee=1;

                        if (liste_unite[j].position.z>liste_unite[j].altitude_voulue)
                        {
                            label->setText("Going down to " + QString::number(liste_unite[j].altitude_voulue) + " m sir !");
                        }
                        else
                        {
                            label->setText("Going up to " + QString::number(liste_unite[j].altitude_voulue) + " m sir !");
                        }

                        if (liste_unite[j].vitesse == 0){
                            liste_unite[j].remontee = true;
                        }
                    }
                    else
                    {
                        if (liste_classe[liste_unite[j].type].type==2 || liste_classe[liste_unite[j].type].type==4) {
                            label->setText("I'm afraid our aircraft can't go to that altitude sir !");
                        }
                        else {
                            label->setText("I'm afraid our submarine can't go to that altitude sir !");
                        }
                    }



                }
                else
                {
                    label->setText("Can you precise our destination sir ?");

                }

                ok=1;
            }
        }

        if((a.startsWith("speed up",Qt::CaseInsensitive) || a.startsWith("slow down",Qt::CaseInsensitive) || a.startsWith("speed",Qt::CaseInsensitive) || a.startsWith("slow",Qt::CaseInsensitive)) && !ok)  // vitesse
        {
            if (liste_classe[liste_unite[j].type].type != 7)
            {
                a=a.toLower();
                QString mot;
                if (a.startsWith("speed up",Qt::CaseInsensitive))
                {
                    mot="speed up";
                }
                else if (a.startsWith("slow down",Qt::CaseInsensitive))
                {
                    mot="slow down";
                }
                else if (a.startsWith("speed",Qt::CaseInsensitive))
                {
                    mot="speed";
                }
                else if (a.startsWith("slow",Qt::CaseInsensitive))
                {
                    mot="slow";
                }

                a=a.remove(mot);

                if (a.contains("to"))
                {
                    a=a.remove("to");
                }

                double valeur=1;
                int vitesse=-1;

                if (a.contains("m/s"))
                {
                    a=a.remove("m/s");
                    valeur=1;
                }
                if (a.contains("km/h"))   // ajouter unites barbares
                {
                    a=a.remove("km/h");
                    valeur=1/3.6;
                }
                if (a.contains("%"))
                {
                    a=a.remove("%");
                    valeur=liste_unite[j].vitesse_max/100.0;
                }
                bool fine = false;
                vitesse=a.toInt(&fine);


                if (fine){
                    if (vitesse == 0){
                        liste_unite[j].en_deplacement=0;
                        liste_unite[j].arret_en_cours=1;
                        label->setText("Stopped !");
                    }
                    else if (vitesse*valeur <= liste_unite[j].vitesse_max && vitesse*valeur >= liste_unite[j].vitesse_min){
                        liste_unite[j].vitesse_voulue=vitesse*valeur;

                        if (liste_unite[j].vitesse>liste_unite[j].vitesse_voulue){
                            label->setText("Slowing down to " + QString::number(liste_unite[j].vitesse_voulue) + " m/s sir !");
                        }
                        else
                        {
                            label->setText("Speeding up to " + QString::number(liste_unite[j].vitesse_voulue) + " m/s sir !");
                        }
                    }
                    else{
                        label->setText("I'm afraid we can't go at that speed Sir !");
                    }



                }
                else{
                    label->setText("Can you precise our speed sir ?");

                }

                ok=1;
            }
        }

        if (!ok && a.startsWith("engage",Qt::CaseInsensitive)){
            int k= index_unite(liste_unite[j].cible);
            if (k!=-1){
                double d=distance(liste_unite[j].position.x, liste_unite[j].position.y, liste_unite[j].position.z, liste_unite[k].position.x, liste_unite[k].position.y, liste_unite[k].position.z);

                QSet<int> objet_restant;
                QList<int> liste;

                for (unsigned int l=0; l<NOMBRE_OBJET; l++){
                    if (liste_objet[l].type == 2){
                        objet_restant.insert(l);
                    }

                }

                liste = objet_restant.toList();
                for (int l = 0; l < liste.size(); l++){
                    if (!liste_unite[j].peut_lancer[liste[l]] || liste_unite[j].nombre_objet[liste[l]] <= 0){
                        objet_restant.remove(liste[l]);
                    }
                }

                if (objet_restant.isEmpty()){
                    label->setText("We can't fire anything sir !");
                }
                else{

                    liste = objet_restant.toList();
                    for (int l = 0; l < liste.size(); l++){
                        if (liste_objet[liste[l]].data.portee < d){
                            objet_restant.remove(liste[l]);
                        }
                    }

                    if (objet_restant.isEmpty()){
                        label->setText("The target is too far sir !");
                    }
                    else{
                        liste = objet_restant.toList();
                        for (int l = 0; l < liste.size(); l++){
                            if (!liste_objet[liste[l]].peut_toucher[liste_classe[liste_unite[k].type].type]){
                                objet_restant.remove(liste[l]);
                            }
                        }

                        if (objet_restant.isEmpty()){
                            label->setText("We can't hit the target with our current ammo !");
                        }
                        else{
                            label->setText("Engaging target sir !");
                            liste_unite[j].engaging = 1;
                        }
                    }
                }

                ok = true;
            }
            else {
                label->setText("We don't have any target Sir !");
            }
        }

        if ((a.startsWith("shut down radar",Qt::CaseInsensitive) || a.startsWith("radar off",Qt::CaseInsensitive)) && !ok){
            ok = true;
            if (liste_unite[j].radar_off){
                label->setText("Radar is already off Sir !");
            }
            else {
                label->setText("Radar offline Sir ! ");
                liste_unite[j].radar_off = true;
            }
        }

        if ((a.startsWith("turn on radar",Qt::CaseInsensitive) || a.startsWith("radar on",Qt::CaseInsensitive)) && !ok){
            ok = true;
            if (!liste_unite[j].radar_off){
                label->setText("Radar is already on Sir !");
            }
            else {
                label->setText("Radar online Sir ! ");
                liste_unite[j].radar_off = false;
            }
        }

        if (a.startsWith("destroy sat ", Qt::CaseInsensitive) && !ok){
            ok = true;
            a.remove("destroy sat ");

            int id_cible = -1;
            for (int i = 0; i < liste_sat.size(); i++){
                if (liste_sat[i].nom == a && liste_pays[liste_sat[i].pays].relation[liste_classe[liste_unite[j].type].pays] == 2){
                    id_cible = liste_sat[i].id;
                }
            }

            int index_o = -1;

            for (int i = 0; i < NOMBRE_OBJET; i++){
                if (liste_objet[i].asat && liste_unite[j].nombre_objet[i] > 0 && liste_unite[j].peut_lancer[i]){
                    index_o = i;
                }
            }

            if (id_cible == -1){
                label->setText("There is no such ennemy satellite Sir !");
            }
            else if (index_o == -1){
                label->setText("We can't fire anything at that satellite Sir !");
            }
            else {
                label->setText("Missile fired at satellite Sir !");
                preparation_lancer(liste_unite[j].id, index_o, id_cible, 0, -1);
            }
        }

        if (a.startsWith("cease fire",Qt::CaseInsensitive) && !ok){
            ok = true;
            if (liste_unite[j].engaging == 0){
                label->setText("We are not engaging anyone Sir !");
            }
            else if (liste_unite[j].engaging == 1){
                label->setText("Ceasing fire Sir !");
                liste_unite[j].engaging = 0;
            }
            else if (liste_unite[j].engaging == 2){
                label->setText("Ceasing fire Sir !");
                liste_unite[j].engaging = 0;
                liste_unite[j].liste_tir_obus.clear();
            }
        }

        if (!ok)
        {
            label->setText("I didn't get your order sir !");
        }
    }
    else
    {

    }
}


void Fenetre::changement_tab(int index){
    if (index != -1){
        int i = index_unite(liste_selection[index]);
        if (i == -1){
            //erreur_log("Current index == -1");
            return;
        }

        if (!liste_unite[i].widget_construit){
            construction_info(liste_unite[i].id, liste_unite[i].widg_selection);
            liste_unite[i].widget_construit = true;
        }
    }
}

void Fenetre::fermeture_tab(int index){
    if (liste_selection.size()>0){
        if (liste_selection.size() == 1){
            liste_selection.clear();
            onglet_selection->setVisible(false);
        }
        else {
            for (int i = index; i < liste_selection.size() - 1; i++){
                liste_selection[i] = liste_selection[i+1];
            }
            liste_selection.pop_back();

            onglet_selection->removeTab(index);
        }

    }
    else {
        qDebug() << "Erreur : tab > 0 && selection == 0";
    }
}

QPixmap Fenetre::creation_pixmap_vie(int uni, int mod){
    double vie = liste_unite[uni].vie_module[mod];
    double vie_max = liste_module[mod].pv;

    int index = vie*100/vie_max;

    if (index < 0 || index >= 101){
        return liste_barre_vie[0];
    }

    return liste_barre_vie[index];
}
