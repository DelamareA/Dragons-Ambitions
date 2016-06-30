#ifndef LISTEU_H
#define LISTEU_H

#include "constante.h"
#include "header.h"
#include <QVector>

class listeu{

    public:
        listeu();
        unite& operator[](int i);
        void push_back(unite u);
        void pop_back();
        void clear();
        unsigned int size();

    private:
        QVector<unite> liste;
};

#endif // LISTEU_H
