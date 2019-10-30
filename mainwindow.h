#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVector>
#include <QDebug>
#include <math.h>
#include "point.h"
#include "neuronaoculta.h"
#include "neuronasalida.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void clickedGraph(QMouseEvent *event);

    void addPoint(Point);
    void on_CrearRed_clicked();

    void plot();
    void plotPoints();
    void plotClusterPoint();
    void plotAreaCluster();
    void plotFuncionAproximada();
    void plotError();

    void entrenarCluster();
    void entrenarAproximacion();

    void on_Inicializa_clicked();
    void on_Comenzar_clicked();

    void clear();
    void clearArea();
    void clearCluster();
    void clearPoint();
    void clearAproximacion();
    void clearError();


    void on_Limpiar_clicked();

    void on_Cuadratica_clicked();

    void on_Coseno_clicked();

    void on_Seno_clicked();

private:
    Ui::MainWindow *ui;

    QVector<Point> pointVector;
    QVector<double> pointX, pointY;
    QVector<neuronaOculta> oculta;
    neuronaSalida SALIDA;
    QVector<double> errorX, errorY;
};

#endif // MAINWINDOW_H
