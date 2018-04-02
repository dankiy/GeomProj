#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include "curve.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pictHeight = 540;
    pictWidth = 540;
    step = 0.0001;
    leftX = -20; rightX = 20;
    botY = -20; topY = 20;
    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}
double MainWindow::fy(double x, bool t)
{
   if (Y2 != 0)
   {
        D = pow((XY * x + Y), 2) - 4 * (Y2 * (X2 * pow(x,2) + X * x + Z));
        if (t)
            return (sqrt(D) - (XY * x + Y)) / (2 * Y2);
        else
            return (-sqrt(D) - (XY * x + Y)) / (2 * Y2);
   }
   else
       return ((X * x + X2 * pow(x, 2) + Z)/(Y + x * XY));
}
double MainWindow::fx(bool t)
{
    if (X2 != 0)
    {
        D = pow(X, 2) - 4 * (X2 * Z);
        if (t)
            return (sqrt(D) - X) / (2 * X2);
        else
            return (-sqrt(D) - X) / (2 * X2);
    }
    else
        return -Z / X;
}
void MainWindow::recountPixels()
{
    onePixelX = pictWidth / (rightX-leftX);
    onePixelY = pictHeight / (topY-botY);
    Ox = fabs(leftX); Oy = topY;
}

void MainWindow::getData()
{
    X2 = ui -> inputX2 -> text().toDouble();
    Y2 = ui -> inputY2 -> text().toDouble();
    X = ui -> inputX -> text().toDouble();
    Y = ui -> inputY -> text().toDouble();
    XY = ui -> inputXY -> text().toDouble();
    Z = ui -> inputZ -> text().toDouble();
}

void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(540, 540);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0, 0, pictWidth, pictHeight);
    paint.drawLine(Ox * onePixelX, 0, Ox * onePixelX, pictHeight);
    paint.drawLine(0, Oy * onePixelY, pictWidth, Oy * onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX; i <= rightX; i += 1.0)
        paint.drawPoint((i + Ox) * onePixelX, Oy * onePixelY);
    for(double i = botY;i <= topY; i += 1.0)
        paint.drawPoint(Ox * onePixelX, (i + Oy) * onePixelY);

    paint.setPen(QPen(Qt::black, 1));
    for (double k = botY; k <= topY; k += 1.0)
    {
        for(double i = leftX; i <= rightX; i += 1.0)
            paint.drawPoint((i + Ox) * onePixelX, (k + Oy) * onePixelY);
    }

    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }


    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;

    if ((Y2 == 0) && (Y == 0) && (XY == 0))
    {
        bool t = true;

        if(!isnan(fx(t)))
        {
            path.moveTo((Oy + fx(t)) * onePixelY, 0);
            path.lineTo((Oy + fx(t)) * onePixelY, pictHeight);

        }

        t = false;

        if(!isnan(fx(t)))
        {
            path.moveTo((Oy + fx(t)) * onePixelY, 0);
            path.lineTo((Oy + fx(t)) * onePixelY, pictHeight);
        }
    }
    else
    {
        bool first = true;
        bool t = true;

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(fy(i, t)))
            {
                if(first)
                {
                    path.moveTo((i + Ox) * onePixelX, (fy(i, t) + Oy) * onePixelY);
                    first = false;
                }
                else
                    path.lineTo((i + Ox) * onePixelX, (fy(i, t) + Oy) * onePixelY);
            }
            else
                first = true;
        }

        first = true;
        t = false;

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(fy(i, t)))
            {
                if(first)
                {
                    path.moveTo((i + Ox) * onePixelX, (fy(i, t) + Oy) * onePixelY);
                    first = false;
                }
                else
                    path.lineTo((i + Ox) * onePixelX,(fy(i, t) + Oy) * onePixelY);
            }
            else
                first = true;
        }
    }
    paint.setPen(QPen(Qt::blue,1,Qt::SolidLine));
    paint.drawPath(path);
    paint.end();
    ui->outputGraph->setPixmap(graph);
    return;
}
/*
void MainWindow::on_clear_clicked()
{
    recountPixels();
    drawGraph();
}
*/
void MainWindow::on_drw_clicked()
{
    getData();
    recountPixels();
    drawGraph(1);
}
