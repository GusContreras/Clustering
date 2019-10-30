#include "mainwindow.h"
#include "ui_mainwindow.h"

const double pi = 3.1415926535;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent* )), SLOT(clickedGraph(QMouseEvent*)));

    ui->inputC->setText("5");
    ui->inputG->setText("500");
    ui->inputE->setText("0.00005");
    ui->inputF->setText("0.5");


    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,5));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->xAxis->setRange(-1, 1);
    ui->customPlot->yAxis->setRange(-1, 1);

    ui->errorPlot->addGraph();
    ui->errorPlot->graph(0)->setPen(QPen(Qt::darkBlue,3));
    ui->errorPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->errorPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickedGraph(QMouseEvent *event)
{
    QPoint point = event->pos();
    Point myPoint(ui->customPlot->xAxis->pixelToCoord(point.x()),ui->customPlot->yAxis->pixelToCoord(point.y()),-1);
    addPoint(myPoint);
    plotPoints();
}

void MainWindow::addPoint(Point p)
{
    pointVector.push_back(p);
    pointX.append(p.x);
    pointY.append(p.y);
}

void MainWindow::on_CrearRed_clicked()
{
    oculta.clear();
    int clusters = ui->inputC->text().toInt();
    SALIDA.inicializa(clusters);
    for(int i = 0; i < clusters; i++)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i+1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(i+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,10));
        ui->customPlot->graph(i+1)->setLineStyle(QCPGraph::lsNone);
        neuronaOculta myNeurona;
        oculta.append(myNeurona);
    }
    for(int i = clusters; i < 2*clusters; i++)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i+1)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(i+1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,5));
        ui->customPlot->graph(i+1)->setLineStyle(QCPGraph::lsNone);
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(2*clusters+1)->setPen(QPen(Qt::black,3));
    ui->customPlot->graph(2*clusters+1)->setLineStyle(QCPGraph::lsLine);
    plot();
}

void MainWindow::on_Inicializa_clicked()
{
    QVector<int> puntos;
    for(int i = 0; i < pointVector.size(); i++)
    {
        puntos.append(i);
    }

    for(int i = 0; i < oculta.size(); i++)
    {
        int p = rand() % puntos.size();
        oculta[i].inicializarCentroide(pointVector[puntos[p]].x, pointVector[puntos[p]].y);
        puntos.remove(p);
    }
    clearArea();
    clearError();
    plot();
}

void MainWindow::plot()
{
    plotAreaCluster();
    plotClusterPoint();
    plotPoints();
}

void MainWindow::plotPoints()
{
    ui->customPlot->graph(0)->setData(pointX,pointY);
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::plotClusterPoint()
{
    for(int i = 0; i < oculta.size(); i++)
    {
        QVector<double> tempX, tempY;
        tempX.append(oculta[i].centroideX);
        tempY.append(oculta[i].centroideY);
        ui->customPlot->graph(i+1)->setData(tempX,tempY);
        ui->customPlot->replot();
        ui->customPlot->update();
    }
}

void MainWindow::plotAreaCluster()
{
    for(int i = 0; i < oculta.size(); i++)
    {
        QVector<double> tempX, tempY;
        double limitx = oculta[i].centroideX - oculta[i].mayorDistancia;
        double limitX = oculta[i].centroideX + oculta[i].mayorDistancia;
        double limity = oculta[i].centroideY - oculta[i].mayorDistancia;
        double limitY = oculta[i].centroideY + oculta[i].mayorDistancia;
        for(double j = limitx; j < limitX; j+= 0.005)
        {
            tempX.append(j);
            tempY.append(limity);
            tempX.append(j);
            tempY.append(limitY);
        }
        for(double j = limity; j < limitY; j+= 0.005)
        {
            tempX.append(limitx);
            tempY.append(j);
            tempX.append(limitX);
            tempY.append(j);
        }
        ui->customPlot->graph(i+oculta.size()+1)->setData(tempX,tempY);
        ui->customPlot->replot();
        ui->customPlot->update();
    }
}

void MainWindow::plotFuncionAproximada()
{
    int clusters = ui->inputC->text().toInt();
    QVector<double> tempX, tempY;
    for(double i = -1; i < 1; i+= 0.1)
    {
        for(int j = 0; j < oculta.size(); j++)
        {
            SALIDA.agregarEntrada(oculta[j].Gaussiana(i));
        }
        SALIDA.agregarEntrada(1);
        tempX.append(i);
        tempY.append(SALIDA.funcion());
        SALIDA.limpiarEntradas();
    }
    ui->customPlot->graph(2*clusters+1)->setData(tempX,tempY);
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::plotError()
{
    ui->errorPlot->graph(0)->setData(errorX,errorY);
    ui->errorPlot->replot();
    ui->errorPlot->update();
}

void MainWindow::entrenarCluster()
{
    int epocas = ui->inputG->text().toInt();
    for(int g = 0; g < epocas; g++)
    {
        for(int i = 0; i < pointVector.size(); i++)
        {
            int pos = 0;
            double menor = 1000;
            for(int j = 0; j < oculta.size(); j++)
            {
                double dis = oculta[j].distancia(pointVector[i].x, pointVector[i].y);
                if(dis < menor)
                {
                    menor = dis;
                    pos = j;
                }
            }
            oculta[pos].agregarPunto(pointVector[i].x, pointVector[i].y);
        }
        int cambio = 0;
        for(int j = 0; j < oculta.size(); j++)
        {
            cambio+= oculta[j].actualizarCentroide();
            oculta[j].limpiarPuntos();
        }
        plot();
        ui->salida->setText("Clusterizando.... Época : " + QString::number(g + 1));
        if(cambio == 0)
        {
            break;
        }
    }
}

void MainWindow::entrenarAproximacion()
{
    int epocas = ui->inputG->text().toInt();
    double deseado = ui->inputE->text().toDouble();
    double factor = ui->inputF->text().toDouble();
    ui->errorPlot->xAxis->setRange(0, epocas);
    ui->errorPlot->yAxis->setRange(0, 0.5);
    int g2;
    double error2 = 0;
    for(int g = 0; g < epocas; g++)
    {
        double errorCuadratico = 0;
        for(int i = 0; i < pointVector.size(); i++)
        {
            for(int j = 0; j < oculta.size(); j++)
            {
                SALIDA.agregarEntrada(oculta[j].Gaussiana(pointVector[i].x));
            }
            SALIDA.agregarEntrada(1);
            errorCuadratico += pow(SALIDA.getError(pointVector[i].y),2);
            SALIDA.update(pointVector[i].y,factor);
            SALIDA.limpiarEntradas();
        }
        errorCuadratico = errorCuadratico/pointVector.size();
        errorY.append(errorCuadratico);
        errorX.append(g);
        plotError();
        ui->salida_2->setText("Aproximando función error actual  " +
                              QString::number(errorCuadratico) + " Época: " + QString::number(g + 1));
        g2 = g;
        if(error2 < errorCuadratico)
        {
            error2 = errorCuadratico;
        }
        if(errorCuadratico < deseado)
        {
            break;
        }
    }
    ui->errorPlot->xAxis->setRange(0, g2);
    ui->errorPlot->yAxis->setRange(0, error2);
    plotError();
}

void MainWindow::on_Comenzar_clicked()
{
    entrenarCluster();
    entrenarAproximacion();
    plotFuncionAproximada();
}

void MainWindow::clear()
{
    clearArea();
    clearCluster();
    clearPoint();
    clearAproximacion();
    clearError();
    ui->salida->setText(" ");
    ui->salida_2->setText(" ");
}

void MainWindow::clearArea()
{
    for(int i = 0; i < oculta.size(); i++)
    {
        oculta[i].mayorDistancia = 0;
        ui->customPlot->graph(i+oculta.size()+1)->data()->clear();
        ui->customPlot->replot();
        ui->customPlot->update();
    }
}

void MainWindow::clearCluster()
{
    for(int i = 0; i < oculta.size(); i++)
    {
        ui->customPlot->graph(i+1)->data()->clear();
        ui->customPlot->replot();
        ui->customPlot->update();
    }
}

void MainWindow::clearPoint()
{
    ui->customPlot->graph(0)->data()->clear();
    ui->customPlot->replot();
    ui->customPlot->update();
    pointVector.clear();
    pointX.clear();
    pointY.clear();
    oculta.clear();
}

void MainWindow::clearAproximacion()
{
    int clusters = ui->inputC->text().toInt();
    ui->customPlot->graph(2*clusters+1)->data()->clear();
    ui->customPlot->replot();
    ui->customPlot->update();
}

void MainWindow::clearError()
{
    errorX.clear();
    errorY.clear();
    ui->errorPlot->graph(0)->data()->clear();
    ui->errorPlot->replot();
    ui->errorPlot->update();
}

void MainWindow::on_Limpiar_clicked()
{
    clear();
}

void MainWindow::on_Cuadratica_clicked()
{
    for(double i = -1; i <= 1; i+= 0.01)
    {
        Point myPoint(i,(i*i*2)-1,-1);
        addPoint(myPoint);
    }
    plotPoints();
}

void MainWindow::on_Coseno_clicked()
{
    double x = -360;
    for(double i = -1; i < 1; i+= 0.02)
    {
        Point myPoint(i,cos(x*(pi/180)),-1);
        addPoint(myPoint);

        x+= 10;
    }
    plotPoints();
}

void MainWindow::on_Seno_clicked()
{
    double x = -360;
    for(double i = -1; i < 1; i+= 0.02)
    {
        Point myPoint(i,sin(x*(pi/180)),-1);
        addPoint(myPoint);

        x+= 10;
    }
    plotPoints();
}
