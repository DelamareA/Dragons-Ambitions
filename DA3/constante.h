#ifndef DEF_CONSTANTE
#define DEF_CONSTANTE

    #include <string>
    #include <vector>
    #include <QString>
    #include <QtGui>
    #include <QFrame>
    #include <QGraphicsScene>
    #include <QTimer>
    #include <QComboBox>

    #include "davector.h"

    /*#define LARGEUR_FENETRE 1280
    #define HAUTEUR_FENETRE 720*/

    #define NOMBRE_ZOOM 5
    #define NOMBRE_PAYS 2
    #define NOMBRE_TYPE 8
    #define NOMBRE_ACTION 33

    #define TAILLE_MAP2 16
    #define TAILLE_CASE_IA 50
    #define TAILLE_TILE 64
    #define TAILLE_TILE2 1024

    #define CONSTANTE_RADAR 20000
    #define CONSTANTE_RADAR2 40000

    #define CONSTANTE_VISION 15
    #define CONSTANTE_VISION2 20

    #define CONSTANTE_RADAR_PASSIF 50
    #define CONSTANTE_RADAR_PASSIF2 100

    #define CONSTANTE_PROJECTILE 1

    #define CONSTANTE_LONG 10000

    #define CONSTANTE_RAPPROCHEMENT 10
    #define CONSTANTE_DIMENSION_TEXTE_X 85

    #define CONSTANTE_WARHEAD_MISSILE 5

    #define NOMBRE_DIVERS 7

    #define NOMBRE_SON 0

    #define PI 3.14159265358979323846

    using namespace std;

    enum {HAUT, GAUCHE, BAS, DROITE};

    enum {PAYS, INCONNU, ENNEMI, ALLIE};

    enum {USA, CHINA};

    enum {RIEN_FAIRE, DEPLACEMENT_VERS_ENNEMI_PROCHE, DEPLACEMENT_VERS_ENNEMI_ELOIGNE, DEPLACEMENT_VERS_ALLIE_PROCHE, DEPLACEMENT_VERS_ALLIE_ELOIGNE, TIR_PEU_MISSILE_BATEAU_PROCHE, TIR_MOYEN_MISSILE_BATEAU_PROCHE, TIR_BCP_MISSILE_BATEAU_PROCHE, TIR_PEU_MISSILE_AVION_PROCHE, TIR_MOYEN_MISSILE_AVION_PROCHE, TIR_BCP_MISSILE_AVION_PROCHE, TIR_PEU_MISSILE_TERRESTRE_PROCHE, TIR_MOYEN_MISSILE_TERRESTRE_PROCHE, TIR_BCP_MISSILE_TERRESTRE_PROCHE, TIR_PEU_MISSILE_SUB_PROCHE, TIR_MOYEN_MISSILE_SUB_PROCHE, TIR_BCP_MISSILE_SUB_PROCHE, TIR_PEU_MISSILE_BATEAU_ELOIGNE, TIR_MOYEN_MISSILE_BATEAU_ELOIGNE, TIR_BCP_MISSILE_BATEAU_ELOIGNE, TIR_PEU_MISSILE_AVION_ELOIGNE, TIR_MOYEN_MISSILE_AVION_ELOIGNE, TIR_BCP_MISSILE_AVION_ELOIGNE, TIR_PEU_MISSILE_TERRESTRE_ELOIGNE, TIR_MOYEN_MISSILE_TERRESTRE_ELOIGNE, TIR_BCP_MISSILE_TERRESTRE_ELOIGNE, TIR_PEU_MISSILE_SUB_ELOIGNE, TIR_MOYEN_MISSILE_SUB_ELOIGNE, TIR_BCP_MISSILE_SUB_ELOIGNE, ENGAGEMENT, LANCEMENT_AVION_RADAR, LANCEMENT_AVION_ANTI_AIR, LANCEMENT_AVION_ATTAQUE};

    enum {PEU, MOYEN, BCP};

    enum {BATEAU, TERRESTRE, AVION, SUB};

    enum {PROCHE, ELOIGNE};

    enum {RADAR, ANTI_AIR, ATTAQUE};

    struct coo
    {
        int x;
        int y;
    };

    struct pairId
    {
        int id;
        double val;
    };

    struct coo2
    {
        double x;
        double y;
        double z;
    };

    struct coo3
    {
        double x;
        double y;
    };

    struct doubleCoo
    {
        coo a;
        coo b;
        coo coo_pixmap;
        coo coo_texte;
        int ancien_k;
    };

    struct bloc
    {
        bool place;
        bool chg;
        int type;
        QGraphicsPixmapItem *pixmap;
    };

    struct pixel
    {
        bool mer;
        int altitude;
        int special; // 0-> rien, 1-> cote, 2-> frontiere
        int type; // 0-> tropical, 1->desert
    };

    struct tile
    {
        bool place;
        bool chg;
        QImage *image;
        QGraphicsPixmapItem *pixmap;
    };

    struct carte
    {
        bool place;
        bool chg;
        QGraphicsPixmapItem *pixmap;
    };

    struct GItem
    {
        bool used; // seulement pour type 4
        int type; // 0->petite ligne, 1->nom, 2->fuel, 3->vitesse, 4->armes, 5->ligne rouge, 6->ligne bleue, 7->point minimap
        QGraphicsItem *item;
    };

    struct classe
    {
        bool* module_present;
        int type;  // 0->bateau, 1->missile, 2->avion, 3->sub, 4->helico, 5->infanterie, 6->amphi, 7->batiment, 8->boue
        int pays;
        int fuel_base;
        int stealth;  // 1-100
        int taille;   // 1-100 pour radar
        double warhead;
        double frottement; // perte de vitesse si plus de fuel
        double bruit;  // 0-1
        double hangar[4];  // 0->missile, 1->avion, 2->helico, 3->amphi
        QString nom;
        QPixmap *image[5];  // 0->yours, 1->unknown, 2->ennemy, 3->ally, 4->neutre
        QPixmap small_image[5];
    };

    struct unite
    {
        bool place;
        bool destination_place;
        bool chg;
        bool chg_pixmap;
        bool chg_module;
        bool chg_objet;
        bool immobilise;
        bool detruite;
        bool en_deplacement;
        bool arret_en_cours;
        bool montee; // inutilisé
        bool patrouille;
        bool widget_construit;
        bool crazee;
        bool radar_off;
        bool remontee;
        bool* peut_lancer;
        bool peut_detecter[NOMBRE_TYPE];
        int id;
        int cible;
        int cible_ami;
        int type;
        int altitude_max;
        int altitude_min;
        int altitude_croisiere;
        int altitude_voulue;
        int atterrissage;
        int vision;
        int vie;
        int vie_max;
        int puissance_radar_passif;
        int altitude_max_trajet;
        int repair_object;
        int engaging; // 0->pas d'engaging, 1->feu a volonté, 2->tir d'obus
        int* vie_module;
        int* nombre_objet;
        int puissance_radar[NOMBRE_TYPE];
        int est_detecte[NOMBRE_PAYS];   // 0-> non, 1->unknown, 2->oui
        int portee_radar[NOMBRE_TYPE];
        double vitesse_max;
        double vitesse_min;
        double vitesse_voulue;
        double acceleration;
        double decceleration;
        double vitesse_rotation;
        double angle;
        double angle_voulu;
        double angle_montee;
        double vitesse;
        double vitesse_chute;
        double fuel;
        double conso;
        double proba_fail;
        double proba_fail_fin;
        double processing_radar[NOMBRE_TYPE];
        coo2 position;
        coo2 destination;
        QString nom;
        QGraphicsPixmapItem *pixmap;
        QGraphicsPixmapItem *pixmap_destination;
        QWidget* widg_selection;
        QVector <pairId> liste_temps_recharge; // id, temps
        DAVector <coo3> liste_deplacement;
        QVector<GItem> liste_graphical_item;
        QVector<int> liste_id_element_ia;
        QVector<coo> liste_ancienne_vie_module;
        QVector<coo> liste_reparation;
        DAVector<coo> liste_tir_obus;
    };

    struct module
    {
        bool* peut_lancer;
        bool peut_detecter[NOMBRE_TYPE];
        int type;        // 0-> coque, 1->moteur, 2->lanceur, 3->reservoir, 4->radar, 5->personnel, 6->repair
        int pv;
        int fuel;
        int vision;                        // 1-100
        int puissance_radar_passif;  // 1-100
        int repair_object;
        int portee_radar[NOMBRE_TYPE];  // en m
        int puissance_radar[NOMBRE_TYPE];  // 1-100
        double conso;
        double vitesse_max;  // en m/s
        double vitesse_min;
        double altitude_max;  // en m
        double altitude_min;
        double altitude_croisiere;
        double angle_montee;
        double acceleration;
        double decceleration; // fote d'ortaugrafe
        double vitesse_rotation;
        double resistance;
        double proba_fail;
        double proba_fail_fin;
        double processing_radar[NOMBRE_TYPE];
        QString nom;
    };

    struct data_projectile
    {
        bool explosion;
        int nb_tir;
        double precision; // 0-1
        double zone_degat; // m
        double vitesse; // m/s
        double portee; // en m
        double warhead;
    };

    struct projectile
    {
        bool fixe; // false-> non-fixe, 'suit' la cible, true->fixe, va vers une destination fixe
        int type; // index de l'objet qui correspond au projectile
        int cible;
        double temps_restant; // s
        coo2 destination;
    };

    struct ia
    {
        bool nombre_missile_variable;
        int nombre_missile;
        int distance_missile;
        int pourcentage_carbu;
        DAVector<coo> liste_missile_tire; // x = id unite, y = nombre missile
    };

    struct config
    {
        QString nom;
        QVector <coo> liste; // x = index objet, y = nombre
    };

    struct objet
    {
        bool asat;
        bool peut_toucher[NOMBRE_TYPE];
        int type;  // 0->missile, 1->avion, 2->projectile, 3->amphi, (4->vehicule), 5->infanterie, 6->bato, 7->helico, 8->boue
        int future_unite;
        int pays;
        double temps_recharge;
        double taille;
        data_projectile data;
        QString nom;
        QPixmap *image;
        QVector <config> liste_config;
    };

    struct sat
    {
        int id;
        int type;   // 0->geostationnaire, 1-> normal
        int precision; // taille minimale d'un objet vu par le sat
        int pays;
        int temps_dessus;
        int temps_ailleurs;
        int temps_propre;
        QString nom;
        QString description;
    };

    struct unite_attente
    {
        int obj;
        int id_unite;
        int id_cible;
        int module_lanceur;
        int id_element_ia;
        int* nb_objet;
        double temps;
    };

    struct info_texte
    {
        bool place;
        int alpha;
        QString nom;
        QGraphicsTextItem * texte;
    };
    
    struct case_deplacement
    {
        int check;
        double valeur;
        coo position;
    };

    struct coo4
    {
        coo p;
        coo precedent;
        double valeur;
        double valeur2;
        double valeur2_bis;
    };

    struct coo5
    {
        coo precedent;
        double valeur;
        double valeur2;
    };

    struct cause
    {
        bool valide;
        int type;  // 0->time,
        int temps;
    };

    struct consequence
    {
        int type; // 0->victory, 1->defeat
    };

    struct script
    {
        bool actif;
        QVector <cause> liste_cause;
        QVector <consequence> liste_consequence;
        QString nom;
    };

    struct opt
    {
        bool affichage_fuel;
        bool affichage_arme;
        bool affichage_vitesse;
        bool fullscreen;
        bool completion_objet_smart;
        bool affichage_groupe;
        int affichage_stack;
        int nb_res;
        int res_x;
        int res_y;
        int completion_commande;
        int completion_objet;

    };

    struct situation
    {
        double ennemi_proche[4]; // 0-1
        double allie_proche; // 0-1
        double munition_projectile; // 0-1
        double munition[4]; // 0-1, pour tirer contre chaque type
        double global_situation; // 0-1
        double vie_unite_allie; // 0-1
        double vie_unite_ennemi; // 0-1
        double niveau_detection; // 0-1
        double fuel; // 0-1
        double nombre_objet[4]; // 0-1, chaque 'type', sauf missile
        double proche_terre; // 0-1
    };

    struct action
    {
        double probo[NOMBRE_ACTION];
        bool action_effectue[NOMBRE_ACTION];
    };

    struct resultat
    {
        double result;
    };

    struct element_ia
    {
        situation situ;
        action act;
        resultat res;
    };

    struct element_ia_current
    {
        int id;
        int temps;
        coo pos;
        element_ia elem;
    };

    struct pays
    {
        bool present; // false si aucune unite ou si joueur
        int relation[NOMBRE_PAYS]; //0-> neutre, 1->allie, 2->ennemi
        ia data_ia;
        QString nom;
        QString demonym;
        QVector<element_ia_current> liste_element;
    };


#endif



