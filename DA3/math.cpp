#include "header.h"
#include <math.h>

#define PI 3.14159265358979323846

double radtodeg (double valeur)
{
    double a=180.0;
    return valeur*(a/PI);
}

double degtorad (double valeur)
{
    double a=180.0;
    return valeur*(PI/a);
}
double distance (double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}

double distance (double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2)+pow(z1-z2,2));
}

int proximite (int x, int y)
{
    return abs(x-y);
}
int max(int x1, int x2)
{
    if (x1>x2)
    {
        return x1;
    }
    return x2;
}

int min(int x1, int x2)
{
    if (x1>x2)
    {
        return x1;
    }
    return x2;
}

double angleFromPosAndDes(double x1, double y1, double x2, double y2){

    if (x2<x1){

        if (y2<y1){
            return PI + acos ((x1-x2)/distance(x1,y1,x2,y2));
        }
        else{
            return PI - acos ((x1-x2)/distance(x1,y1,x2,y2));
        }
    }
    else{

        if (y2<y1){
            return -acos ((x2-x1)/distance(x1,y1,x2,y2));
        }
        else{
            return acos ((x2-x1)/distance(x1,y1,x2,y2));
        }
    }


}


