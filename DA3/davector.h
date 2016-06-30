#ifndef DAVECTOR_H
#define DAVECTOR_H

#include "header2.h"
#include <QVector>

using namespace std;

template <typename T>
class DAVector
{
    public:
        DAVector();

        T& operator[](int);
        void push_back(T);
        void pop_back();
        void clear();
        int size();
        void remove(int i);

    private:
        QVector<T> liste;
};

template <typename  T> DAVector<T>::DAVector(){
}

template <typename  T> T& DAVector<T>::operator[](int i){
    if (i < 0 || i >= liste.size()){
        erreur_log("index of DAVector too big : " + QString::number(i));
        return liste[i]; // fait expres pour que le debugueur trouve où est l'erreur
    }

    return liste[i];

}

template <typename  T> void DAVector<T>::push_back(T u){
    liste.push_back(u);
}

template <typename T> void DAVector<T>::pop_back(){
    liste.pop_back();
}

template <typename T> void DAVector<T>::clear(){
    liste.clear();
}

template <typename T> int DAVector<T>::size(){
    return liste.size();
}

template <typename T> void DAVector<T>::remove(int i){
    if (i < 0 || i >= liste.size()){
        erreur_log("index of DAVector too big in remove: " + QString::number(i));
    }

    liste.remove(i);
}


#endif // DAVECTOR_H
