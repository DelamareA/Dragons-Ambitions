#ifndef HEADER_FENETRE
#define HEADER_FENETRE

#include <limits>
#include "windows.h"
#include <QtWidgets>
#include <QGraphicsScene>
#include <QTimer>
#include <QComboBox>
#include <QMap>
#include "constante.h"
#include "scene.h"
#include "listeu.h"
#include "davector.h"


class Fenetre : public QGraphicsView
{
    Q_OBJECT

    public:
        Fenetre();
        void keyReleaseEvent(QKeyEvent* event);
        void keyPressEvent(QKeyEvent* event);

        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void wheelEvent (QWheelEvent * event );


    private:
        QString imageToAscii(QImage image, int col, int lin);

        bool chargement_map (QString nom);
        bool creation_map ();
        void chargement_liste_module();
        void chargement_liste_classe();
        void chargement_liste_objet();

        void set_numbers();
        void clear_log();
        QString load_file(QString filename);
        void chargement_classe(QString file_name);
        void chargement_module(QString file_name);
        void chargement_objet(QString file_name);

        void afficher_map ();
        bool afficher_unite ();

        void event_gauche();
        void event_droite();
        void event_haut();
        void event_bas();
        void mouvement_souris_jeu();
        void release_souris_jeu();
        void double_clic_souris_jeu();
        void event_souris_gauche_jeu();
        void event_souris_droite_jeu();
        void event_souris_mid_jeu();
        void mouvement_souris_editeur();
        void release_souris_editeur();
        void double_clic_souris_editeur();
        void event_souris_gauche_editeur();
        void event_souris_droite_editeur();
        void event_echap();
        void event_entree();

        void release_selection();

        bool check_module_unite ();
        bool check_deplacement ();
        void check_projectile ();
        void check_dead();
        bool check_unite_selectionnee();
        void check_radar();
        bool check_engaging ();
        bool check_info_texte ();
        void check_autodef();
        void check_script();
        void check_lancement();
        void check_fail();
        void check_sat();
        void check_groupe();

        void preparation_lancer(int id_depart, int num_objet, int id_cible, int *nb_objet_unite, int id_element_ia);
        void tirer_missile(int id_depart, int num_objet, int id_cible, int id_element_ia);
        void lancer_avion(int id_depart, int num_objet, int* nb_objet_unite, int id_element_ia);
        bool ordre_unite (int id_unite, QString a, QLabel* label);
        void destruction_unite(int id);
        void destruction_info(int index);
        void ajout_info_texte(QString text);
        void ajout_pop_up(QString text);
        void construction_info(int id, QWidget * w);  // widget pas info_texte

        int index_unite (int id);
        bool est_selectionne (int id);
        int attribution_id ();
        QPixmap creation_pixmap_vie(int uni, int mod);

        void lancer_menu();

        void construction_fenetre_unite(int id);
        void construction_groupe_objet(int id);
        void construction_groupe_diplomatie();
        void construction_groupe_script();
        void construction_groupe_cause();
        void construction_groupe_consequence();
        void construction_groupe_sat();

        void sauvegarde(QString s);
        void chargement(QString s);

        void sauvegarde_option();
        void chargement_option();

        unite preparation_unite(int classe);
        double place_restante(int id, int hangar);
        QString time_to_string(int a);

        situation current_situation(coo pos, int pays);
        double compare_situation(situation s1, situation s2);
        action action_selon_situation(situation si, QVector<element_ia> liste_elem);
        void gestion_situation(coo pos, int pays);
        void check_ia();
        int index_element_ia(int id, int pays);
        void sauvegarde_ia();
        void chargement_ia();

        QVector<int> unite_pays_position(coo pos, int pays, QSet<int>* type);
        bool deplacement_unite(int index, coo pos, bool shift, bool message);
        bool fonction_tirer_missile(coo pos, int pays, int id_element_ia, int nombre, int cible, int distance);
        bool fonction_deplacement(coo pos, int pays, int id_element_ia, int cible, int dista);

        bool fonction_rien_faire(coo pos, int pays, int id_element_ia);
        bool fonction_deplacement_vers_ennemi_proche(coo pos, int pays, int id_element_ia);
        bool fonction_deplacement_vers_ennemi_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_deplacement_vers_allie_proche(coo pos, int pays, int id_element_ia);
        bool fonction_deplacement_vers_allie_eloigne(coo pos, int pays, int id_element_ia);


        bool fonction_tirer_peu_missile_bateau_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_bateau_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_bateau_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_peu_missile_bateau_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_bateau_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_bateau_eloigne(coo pos, int pays, int id_element_ia);

        bool fonction_tirer_peu_missile_avion_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_avion_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_avion_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_peu_missile_avion_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_avion_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_avion_eloigne(coo pos, int pays, int id_element_ia);

        bool fonction_tirer_peu_missile_terrestre_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_terrestre_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_terrestre_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_peu_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_terrestre_eloigne(coo pos, int pays, int id_element_ia);

        bool fonction_tirer_peu_missile_sub_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_sub_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_sub_proche(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_peu_missile_sub_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_moyen_missile_sub_eloigne(coo pos, int pays, int id_element_ia);
        bool fonction_tirer_bcp_missile_sub_eloigne(coo pos, int pays, int id_element_ia);

        bool fonction_engagement(coo pos, int pays, int id_element_ia);

        bool fonction_lancer_avion(coo pos, int pays, int id_element_ia, int type);

        bool fonction_lancer_avion_radar(coo pos, int pays, int id_element_ia);
        bool fonction_lancer_avion_anti_air(coo pos, int pays, int id_element_ia);
        bool fonction_lancer_avion_attaque(coo pos, int pays, int id_element_ia);

    public slots:

        void update ();
        void changement_tab(int index);
        void fermeture_tab(int index);
        bool envoi_ordre ();
        void lancer_jeu();
        void lancer_editeur();
        void lancer_generateur();
        void lancer_option();
        void destruction_pop_up();

        void chg_bouton_groupe(int i);
        void chg_bouton_pause();
        void chg_bouton_sat_jeu();
        void chg_bouton_action_sat(int a);
        void chg_bouton_log();
        void devin(const QString & text);
        QString longest_common_prefix(QVector<QString> liste);

        void construction_fenetre_sat_jeu();
        void construction_fenetre_menu_jeu();
        void construction_fenetre_log();

        void chg_bouton_menu_jeu_retour();
        void chg_bouton_menu_jeu_save();
        void chg_bouton_menu_jeu_option();
        void chg_bouton_menu_jeu_menu();
        void chg_bouton_menu_jeu_exit();

        void initialisation();

        void chg_combo_unite_type(const QString & text);
        void chg_combo_unite_pays(const QString & text);
        void chg_champ_unite_nom(QString a);
        void chg_champ_unite_altitude(int v);
        void chg_champ_unite_angle(int v);
        void chg_combo_unite_objet(const QString & text);
        void chg_bouton_unite_ajout_objet();
        void chg_bouton_unite_reset_objet();
        void chg_bouton_unite_delete();

        void chg_combo_pays_joueur(const QString & text);
        void chg_bouton_pays_alliance();
        void chg_bouton_pays_guerre();
        void chg_bouton_pays_reset();

        void chg_bouton_map();

        void chg_bouton_script_ajout();
        void chg_bouton_script(int a);
        void chg_bouton_script_delete();
        void chg_bouton_gestion_nvcause();
        void chg_bouton_cause_ok();
        void chg_bouton_cause_delete(int a);
        void chg_bouton_gestion_nvconsequence();
        void chg_bouton_consequence_ok();
        void chg_bouton_consequence_delete(int a);
        void chg_champ_gestion_nom(QString a);

        void chg_bouton_sat_ajout();
        void chg_bouton_sat(int a);
        void chg_bouton_sat_delete();
        void chg_champ_gestion_nom_sat(QString a);
        void chg_champ_geo_sat(int a);
        void chg_champ_temps_dessus_sat(int);
        void chg_champ_temps_ailleurs_sat(int);
        void chg_combo_pays_sat(int);
        void chg_champ_description_sat(QString s);
        void chg_champ_precision_sat(int  s);

        void chg_bouton_selection_image();
        void chg_champ_generateur(const QString & text);
        void chg_bouton_generateur2();

        void construction_fenetre_pays();
        void construction_fenetre_script();
        void construction_fenetre_map();
        void construction_fenetre_sat();
        void construction_fenetre_ia();
        void slot_sauvegarde_sous();

        void chg_combo_ia(int index);
        void chg_spin_ia_nombre_missile(int val);
        void chg_spin_ia_distance_missile(int val);
        void chg_check_ia_nombre_missile_variable(int val);
        void chg_spin_ia_carbu_retour(int val);

        void chg_checkbox_option_fuel(int a);
        void chg_checkbox_option_arme(int a);
        void chg_checkbox_option_vitesse(int a);
        void chg_checkbox_option_groupe(int a);
        void chg_combo_affichage_stack(const QString &a);
        void chg_checkbox_option_fullscreen(int a);
        void chg_combo_affichage_res(const QString &a);
        void chg_combo_completion_commande(const QString & a);
        void chg_combo_completion_objet(const QString & a);
        void chg_checkbox_completion_objet_smart(int a);
        void chg_bouton_retour();

    private:
        QTimer *timer;

        unsigned int NOMBRE_OBJET;
        unsigned int NOMBRE_CLASSE;
        unsigned int NOMBRE_MODULE;

        unsigned int LARGEUR_FENETRE;
        unsigned int HAUTEUR_FENETRE;

        Scene *scene;
        QTabWidget *onglet_selection;

        QPixmap *image_divers[NOMBRE_DIVERS];

        pixel ** map;
        int ** map2;  // 0->mer 1->terre
        int ** map_altitude;
        int **map_transition; // 0->pas de transition, 1->transition
        carte pixmap_map;
        vector <QGraphicsItem *> liste_ligne;
        vector <info_texte> liste_info_texte;

        classe* liste_classe;
        module* liste_module;
        pays liste_pays[NOMBRE_PAYS];
        objet* liste_objet;
        listeu liste_unite;
        DAVector<unite_attente> liste_unite_attente;
        DAVector <script> liste_script;
        DAVector <sat> liste_sat;
        DAVector <projectile> liste_projectile;
        QVector <int> liste_selection;
        QVector<QString> liste_log;
        QVector<QString> liste_commande;
        QVector<int> liste_groupe[10];

        bool son_a_jouer [NOMBRE_SON];

        bool select_rect;
        coo position_rect;
        coo position_rect2;

        bool select_minimap;

        coo position_deplacement_mid;

        int fps;
        double fps_precedent;
        int sec;
        int temps;

        int mode;
        int joueur;
        int id;

        int id_unite_asat;

        int largeur_map;
        int hauteur_map;
        int largeur_minimap;
        int hauteur_minimap;
        int zoom;
        int oldzoom;
        double valeur_zoom[NOMBRE_ZOOM];
        double echelle;
        coo camera;
        bool chg_camera;

        bool pause;
        int vitesse_jeu;

        bool a_appuye;
        bool s_appuye;
        bool w_appuye;
        bool y_appuye;
        bool shift_appuye;
        bool ctrl_appuye;
        bool esc_appuye;
        bool entree_appuye;
        bool chiffre_appuye[10];

        bool gauche_appuye;
        bool droite_appuye;
        bool mid_appuye;

        QImage * image_minimap;
        QGraphicsPixmapItem* pixmap_minimap;
        QGraphicsRectItem* rectangle_minimap;

        QPixmap liste_barre_vie[101];

        QMap<long, tile*> image_map[NOMBRE_ZOOM];
        QMap<long, QGraphicsPixmapItem*> image_croix;

        QTabWidget *groupe_pop_up;
        QPushButton* bouton_pause;
        QPushButton* bouton_sat_jeu;
        QPushButton* bouton_log;
        QPushButton* bouton_groupe[10];
        QWidget* barre_principale_jeu;
        QWidget* barre_bouton; // groupe
        QGroupBox* fenetre_sat_jeu;
        QGroupBox* fenetre_log_jeu;
        QGroupBox* fenetre_menu_jeu;

        //_____________________________________________
        // MENU
        //_____________________________________________


        QPixmap *image_menu;
        QGraphicsPixmapItem *pixmap_menu;
        QPushButton *bouton_jeu;
        QPushButton *bouton_editeur;
        QPushButton *bouton_generateur;
        QPushButton *bouton_option;
        QPushButton *bouton_quit;

        //_____________________________________________
        // EDITEUR
        //_____________________________________________

        QWidget *barre_principale;
        QPushButton *bouton_script;
        QPushButton *bouton_pays;
        QPushButton *bouton_map;
        QPushButton *bouton_ia;
        QPushButton *bouton_sat;
        QPushButton *bouton_sauvegarde;
        QPushButton *bouton_sauvegarde_sous;

        bool fenetre_unite_cree;
        QWidget *fenetre_unite;
        QComboBox *combo_unite_pays;
        QComboBox *combo_unite_type;
        QLineEdit *champ_unite_nom;
        QSpinBox *champ_unite_altitude;
        QSpinBox *champ_unite_angle;
        QPushButton *bouton_unite_delete;

        bool groupe_unite_nombre_objet_cree;
        QGroupBox *groupe_unite_objet;
        QVBoxLayout *layout_unite_objet;
        QSpinBox *champ_unite_nombre_objet;
        QComboBox *combo_unite_objet;
        QPushButton *bouton_unite_ajout_objet;
        QLabel *label_unite_objet;
        QPushButton *bouton_unite_reset_objet;
        QWidget *groupe_unite_nombre_objet;

        QWidget *fenetre_pays;
        QComboBox *combo_pays_joueur;
        QGroupBox *groupe_pays_relation;
        QVBoxLayout *layout_pays_relation;
        QComboBox *combo_pays_pays1;
        QComboBox *combo_pays_pays2;
        QPushButton *bouton_pays_alliance;
        QPushButton *bouton_pays_guerre;
        QPushButton *bouton_pays_reset;
        QWidget *groupe_pays_diplomatie;

        QWidget *fenetre_script;
        QPushButton *bouton_script_ajout;
        QGroupBox *groupe_script_script;
        QVBoxLayout *layout_script_script;
        QWidget *groupe_script_script2;
        DAVector <QPushButton *> liste_bouton_script;

        QWidget *fenetre_gestion; // script
        QVBoxLayout *layout_gestion;
        QLineEdit *champ_gestion_nom;
        QComboBox *combo_gestion_nvcause;
        QPushButton *bouton_gestion_nvcause;
        QComboBox *combo_gestion_nvconsequence;
        QPushButton *bouton_gestion_nvconsequence;
        QWidget *groupe_gestion_cause;
        QVBoxLayout *layout_gestion_cause;
        QWidget *groupe_gestion_consequence;
        QVBoxLayout *layout_gestion_consequence;
        DAVector <QPushButton *> liste_bouton_cause_delete;
        DAVector <QPushButton *> liste_bouton_consequence_delete;

        QWidget *fenetre_cause;
        QSpinBox *champ_cause_temps;
        QPushButton *bouton_cause_ok;

        QWidget *fenetre_consequence;
        QPushButton *bouton_consequence_ok;

        QWidget *fenetre_sat;
        QPushButton *bouton_sat_ajout;
        QGroupBox *groupe_sat_sat;
        QVBoxLayout *layout_sat_sat;
        QWidget *groupe_sat_sat2;
        DAVector <QPushButton *> liste_bouton_sat;

        QWidget *fenetre_gestion_sat;
        QVBoxLayout *layout_gestion_sat;
        QLineEdit *champ_gestion_nom_sat;
        QSpinBox *champ_temps_dessus_sat;
        QSpinBox *champ_temps_ailleurs_sat;
        QSpinBox *champ_precision_sat;
        QCheckBox *champ_geo_sat;
        QComboBox *combo_pays_sat;
        QLineEdit *champ_description_sat;

        QWidget *fenetre_map;
        QComboBox *combo_map;

        QSpinBox* spin_ia_nombre_missile;
        QSpinBox* spin_ia_distance_missile;
        QCheckBox* check_ia_nombre_missile_variable;
        QSpinBox* spin_ia_carbu_retour;

        int script_actuel;
        int cause_actuelle;
        int consequence_actuelle;
        int sat_actuel;
        int ia_actuelle;

        QString nom_fichier;

        //_____________________________________________
        // GENERATEUR
        //_____________________________________________

        QWidget *fenetre_generateur;
        QLabel *label_generateur;
        QPushButton *bouton_generateur2;
        QLineEdit *champ_generateur;

        //_____________________________________________
        // OPTIONS
        //_____________________________________________

        opt option;
        QWidget* fenetre_menu_option;
        int acces_option;

        //_____________________________________________
        // IA
        //_____________________________________________

        QVector<element_ia> liste_element_ia;


};


#endif
