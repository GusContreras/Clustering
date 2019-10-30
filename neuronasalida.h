#ifndef NEURONASALIDA_H
#define NEURONASALIDA_H

#include <QVector>
#include <cstdlib>
#include <ctime>

class neuronaSalida
{
public:

    QVector<double> pesos;
    QVector<double> entradas;

    neuronaSalida();
    void inicializa(int);
    void agregarEntrada(double);
    double funcion();
    void update(double, double);
    double Rand();
    void limpiarEntradas();
    double getError(double y);
};

#endif // NEURONASALIDA_H
