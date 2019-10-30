#include "neuronaoculta.h"

const double euler = 2.718281828459045235360287;

neuronaOculta::neuronaOculta()
{

}

void neuronaOculta::agregarPunto(double x, double y)
{
    ejeX.append(x);
    ejeY.append(y);
}

double neuronaOculta::distancia(double x, double y)
{
    return sqrt(pow((centroideX - x),2) + pow((centroideY - y),2));
}

int neuronaOculta::actualizarCentroide()
{
    double x = 0;
    double y = 0;
    for(int i = 0; i < ejeX.size(); i++)
    {
        x+= ejeX[i];
        y+= ejeY[i];
    }
    x = x/ejeX.size();
    y = y/ejeY.size();

    mayorDistancia = distanciaMasAlejada();
    if(x == centroideX and y == centroideY)
    {
        return 0;
    }
    centroideX = x;
    centroideY = y;
    return 1;
}

void neuronaOculta::limpiarPuntos()
{
    ejeX.clear();
    ejeY.clear();
}

void neuronaOculta::inicializarCentroide(double x, double y)
{
    centroideX = x;
    centroideY = y;
}

int neuronaOculta::puntoMasAlejado()
{
    int pos = 0;
    double lejano = 0;
    for(int i = 0; i < ejeX.size(); i++)
    {
        double temp = distancia(ejeX[i],ejeY[i]);
        if(temp > lejano)
        {
            mayorDistancia = temp;
            lejano = temp;
            pos = i;
        }
    }
    return pos;
}

double neuronaOculta::distanciaMasAlejada()
{
    double lejano = 0.01;
    for(int i = 0; i < ejeX.size(); i++)
    {
        double temp = distancia(ejeX[i],ejeY[i]);
        if(temp > lejano)
        {
            lejano = temp;
        }
    }
    return lejano;
}

double neuronaOculta::Gaussiana(double x)
{
    return pow(euler,(-(pow(centroideX - x,2))/(2*pow(mayorDistancia,2))));
}
