#include "listeu.h"

listeu::listeu(){
}

unite& listeu::operator[](int i){
    if (i < 0 || i >= liste.size()){
        erreur_log("index of listeu too big : " + QString::number(i));
        return liste[i]; // fait expres
    }

    return liste[i];

}

void listeu::push_back(unite u){
    liste.push_back(u);
}

void listeu::pop_back(){
    liste.pop_back();
}

void listeu::clear(){
    liste.clear();
}

unsigned int listeu::size(){
    return liste.size();
}
