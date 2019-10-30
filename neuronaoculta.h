#ifndef NEURONAOCULTA_H
#define NEURONAOCULTA_H

#include <QtMath>
#include <QVector>
#include <cstdlib>
#include <ctime>

class neuronaOculta
{
public:
    QVector<double> ejeX,ejeY;
    double centroideX, centroideY;
    double varianza;
    double salida;
    double mayorDistancia = 0;

    neuronaOculta();
    void agregarPunto(double, double);
    double distancia(double, double);
    int actualizarCentroide();
    void limpiarPuntos();
    void inicializarCentroide(double, double);
    int puntoMasAlejado();
    double distanciaMasAlejada();
    double Gaussiana(double);
};

#endif // NEURONAOCULTA_H
