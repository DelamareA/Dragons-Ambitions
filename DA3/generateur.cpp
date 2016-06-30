#include "Fenetre.h"
#include "constante.h"
#include "header.h"

#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sstream>
#include <time.h>

void Fenetre::lancer_generateur()
{


    pixmap_menu->setVisible(false);
    bouton_jeu->setVisible(false);
    bouton_editeur->setVisible(false);
    bouton_generateur->setVisible(false);
    bouton_option->setVisible(false);
    bouton_quit->setVisible(false);

    mode=3;

    largeur_map=2000;
    hauteur_map=2000;
    echelle=500;  // nb m pour 1 pixel
    zoom=0;
    camera.x=5;
    camera.y=5;

    id=1;

    chargement_map ("kapoue");



    fenetre_generateur = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *lab = new QLabel("Please select a correct .png picture (see code_couleur.txt)", fenetre_generateur);
    QPushButton* but = new QPushButton("Select picture", fenetre_generateur);
    label_generateur = new QLabel("No file selected", fenetre_generateur);
    bouton_generateur2 = new QPushButton("Generate map", fenetre_generateur);
    bouton_generateur2->setEnabled(false);

    QWidget *groupe = new QWidget(fenetre_generateur);
    QLabel *label = new QLabel("Map's name : ", groupe);
    QHBoxLayout *layout2 = new QHBoxLayout;

    champ_generateur = new QLineEdit(groupe);

    layout2->addWidget(label);
    layout2->addWidget(champ_generateur);
    groupe->setLayout(layout2);

    layout->addWidget(groupe);
    layout->addWidget(lab);
    layout->addWidget(but);
    layout->addWidget(label_generateur);
    layout->addWidget(bouton_generateur2);

    fenetre_generateur->setLayout(layout);
    fenetre_generateur->setVisible(true);
    fenetre_generateur->setWindowTitle("Map generator");

    QObject::connect(but, SIGNAL(clicked ()), this, SLOT(chg_bouton_selection_image()));
    QObject::connect(bouton_generateur2, SIGNAL(clicked ()), this, SLOT(chg_bouton_generateur2()));
    QObject::connect(champ_generateur, SIGNAL(textChanged (const QString &)), this, SLOT(chg_champ_generateur(const QString &)));
}

void Fenetre::chg_bouton_selection_image()
{
     QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png)");
     if (fichier.endsWith(".png"))
     {
         label_generateur->setText(fichier);

         if (!champ_generateur->text().isEmpty())
         {
             bouton_generateur2->setEnabled(true);
         }
         else
         {
             bouton_generateur2->setEnabled(false);
         }

     }
     else
     {
         label_generateur->setText("No file selected");
         bouton_generateur2->setEnabled(false);
     }
}

void Fenetre::chg_champ_generateur(const QString & text)
{
    if (!text.isEmpty() && label_generateur->text().endsWith(".png"))
    {
        bouton_generateur2->setEnabled(true);
    }
    else
    {
        bouton_generateur2->setEnabled(false);
    }
}

void Fenetre::chg_bouton_generateur2()
{
    creation_map ();
    QMessageBox::information(this, "Success", "Map \"" + champ_generateur->text() + " \" successfully generated !");
    chargement_map (champ_generateur->text());
    pixmap_map.chg=1;
}

bool Fenetre::creation_map ()
{
    string s = "data/map/" + champ_generateur->text().toStdString() + ".damap";
    FILE * fichier = fopen(s.c_str(), "w");

    QImage img(label_generateur->text());
    for (int y = 0; y < img.height(); y++)
    {
        QRgb* rgb = (QRgb*)img.scanLine(y);
        for (int x = 0; x < img.width(); x++)
        {

            if (qRed(rgb[x])==0)  // mer  && qBlue(rgb[x])==255
            {
                if (qGreen(rgb[x])>245)
                {
                    fprintf(fichier, "a");
                }
                else if (qGreen(rgb[x])<=245 && qGreen(rgb[x])>230)
                {
                    fprintf(fichier, "b");
                }
                else if (qGreen(rgb[x])<=230 && qGreen(rgb[x])>210)
                {
                    fprintf(fichier, "c");
                }

                // a completer
            }
            else
            {
                if (qRed(rgb[x])==0 && qBlue(rgb[x])==0 && qGreen(rgb[x])!=0)
                {
                    if (qGreen(rgb[x])>235)
                    {
                        fprintf(fichier, "A");
                    }
                    else if (qGreen(rgb[x])<=235 && qGreen(rgb[x])>210)
                    {
                        fprintf(fichier, "B");
                    }
                    else if (qGreen(rgb[x])<=210 && qGreen(rgb[x])>190)
                    {
                        fprintf(fichier, "C");
                    }

                    // a completer
                }
                else if (qRed(rgb[x])==100 && qBlue(rgb[x])==0 && qGreen(rgb[x])!=0){
                    if (qGreen(rgb[x])>235)
                    {
                        fprintf(fichier, "+");
                    }
                    else if (qGreen(rgb[x])<=235 && qGreen(rgb[x])>210)
                    {
                        fprintf(fichier, "\"");
                    }
                    else if (qGreen(rgb[x])<=210 && qGreen(rgb[x])>190)
                    {
                        fprintf(fichier, "*");
                    }

                    // a completer
                }
            }

        }
    }
    fclose(fichier);

    return 1;
}
