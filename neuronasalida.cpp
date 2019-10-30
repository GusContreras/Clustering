#include "neuronasalida.h"
#include <QDebug>

neuronaSalida::neuronaSalida()
{

}

void neuronaSalida::inicializa(int x)
{
    pesos.clear();
    for(int i = 0; i < x; i++)
    {
        pesos.append(Rand());
    }
    pesos.append(-1);
}

void neuronaSalida::agregarEntrada(double x)
{
    entradas.append(x);
}

double neuronaSalida::funcion()
{
    double z = 0;
    for(int i = 0; i < pesos.size(); i++)
    {
        z+= pesos[i] * entradas[i];
    }
    return z;
}

void neuronaSalida::update(double y, double f)
{
    for(int i = 0; i < pesos.size(); i++)
    {
        pesos[i] += f*(y - funcion())*entradas[i];
    }
}

double neuronaSalida::Rand()
{
    return 1 - 2*(rand()/(double)RAND_MAX);
}

void neuronaSalida::limpiarEntradas()
{
    entradas.clear();
}

double neuronaSalida::getError(double y)
{
    return y - funcion();
}
