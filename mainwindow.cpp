#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include "curve.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //scrollArea(new QScrollArea),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //scrollArea -> setVisible(false);
    //scrollArea -> setWidget(ui -> outputGraph);
    //ui -> scrollArea -> setVisible(false);
    pictHeight = 380;
    pictWidth = 540;
    leftX = -20; rightX = 20;
    step = (abs(leftX) + abs(rightX)) / 40000;
    ratio = double(pictHeight) / double(pictWidth);
    botY = leftX * (ratio); topY = rightX * (ratio);
    recountPixels();
    drawGraph(false);
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
double MainWindow::fsqrt(double x)
{
    return (1/(sin(x)));
}
void MainWindow::recountPixels()
{
    onePixelX = pictWidth / (rightX-leftX);
    onePixelY = pictHeight / (topY-botY);
    Ox = fabs(rightX); Oy = fabs(topY);
    step = (abs(leftX) + abs(rightX)) / 40000;
}

void MainWindow::getData()
{
    X2 = ui -> inputX2 -> text().toDouble();
    Y2 = ui -> inputY2 -> text().toDouble();
    XY = ui -> inputXY -> text().toDouble();
    X = ui -> inputX -> text().toDouble();
    Y = ui -> inputY -> text().toDouble();
    Z = ui -> inputZ -> text().toDouble();
}

void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(pictWidth, pictHeight);
    QPainter paint;
    paint.begin(&graph);
    paint.eraseRect(0, 0, pictWidth, pictHeight);
    paint.drawLine(Ox * onePixelX, 0, Ox * onePixelX, pictHeight);
    paint.drawLine(0, Oy * onePixelY, pictWidth, Oy * onePixelY);
    paint.setPen(QPen(Qt::black,3));
    //Разметка осей
    for(double i = 0; i <= rightX; i += 1.0)
    {
        paint.drawPoint((i + Ox) * onePixelX, Oy * onePixelY);
        paint.drawPoint((-i + Ox) * onePixelX, Oy * onePixelY);
    }
    for(double i = 0; i <= topY; i += 1.0)
    {
        paint.drawPoint(Ox * onePixelX, (i + Oy) * onePixelY);
        paint.drawPoint(Ox * onePixelX, (-i + Oy) * onePixelY);
    }
    paint.setPen(QPen(Qt::black, 1));
    //Вопрос при динамичных границах
    //Точки масштаба
    for (double k = 0; k <= topY; k += 1.0)
    {
        for(double i = 0; i <= rightX; i += 1.0)
        {
            paint.drawPoint((i + Ox) * onePixelX, (k + Oy) * onePixelY);
            paint.drawPoint((-i + Ox) * onePixelX, (k + Oy) * onePixelY);
            paint.drawPoint((i + Ox) * onePixelX, (-k + Oy) * onePixelY);
            paint.drawPoint((-i + Ox) * onePixelX, (-k + Oy) * onePixelY);
        }
    }
    if(!notEmpty) {
        paint.end();
        ui->outputGraph->setPixmap(graph);
        return;
    }

    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    if (X == 7357)
    {
        bool first = true;
        double last = fsqrt(leftX);

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(fsqrt(i)))
            {
                if(first)
                {
                    last = fsqrt(i);
                    path.moveTo((i + Ox) * onePixelX, (fsqrt(i) + Oy) * onePixelY);
                    first = false;
                }
                else
                {
                    if ((abs(last - fsqrt(i)) >= (topY-botY)))
                    {
                        first = true;
                    }
                    else
                    {
                    last = fsqrt(i);
                    path.lineTo((i + Ox) * onePixelX, (fsqrt(i) + Oy) * onePixelY);
                    }
                }
            }
            else
                first = true;
        }
    }
    else
    {
    if ((Y2 == 0) && (Y == 0) && (XY == 0))
    {
        bool signD = true;

        if(!isnan(fx(signD)))
        {
            path.moveTo((Oy + fx(signD)) * onePixelY, 0);
            path.lineTo((Oy + fx(signD)) * onePixelY, pictHeight);

        }

        signD = false;

        if(!isnan(fx(signD)))
        {
            path.moveTo((Oy + fx(signD)) * onePixelY, 0);
            path.lineTo((Oy + fx(signD)) * onePixelY, pictHeight);
        }
    }
    else
    {
        bool first = true;
        bool signD = true;
        double last;

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(fy(i, signD)))
            {

                if(first)
                {
                    last = fy(i, signD);
                    path.moveTo((i + Ox) * onePixelX, (fy(i, signD) + Oy) * onePixelY);
                    first = false;
                }
                else
                {
                    if ((abs(last - fy(i, signD)) >= (topY-botY)))
                    {
                        first = true;
                    }
                    else
                    {
                    last = fy(i, signD);
                    path.lineTo((i + Ox) * onePixelX, (fy(i, signD) + Oy) * onePixelY);
                    }
                }
            }
            else
                first = true;
        }

        first = true;
        signD = false;

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(fy(i, signD)))
            {


                if(first)
                {
                    last = fy(i, signD);
                    path.moveTo((i + Ox) * onePixelX, (fy(i, signD) + Oy) * onePixelY);
                    first = false;
                }
                else
                {
                    if ((abs(last - fy(i, signD)) >= (topY-botY)))
                    {
                        first = true;
                    }
                    else
                    {
                    last = fy(i, signD);
                    path.lineTo((i + Ox) * onePixelX, (fy(i, signD) + Oy) * onePixelY);
                    }
                }
            }
            else
                first = true;
        }
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
    QString Draw = ("Draw(");
    Draw += (((ui -> inputX2 -> text() == NULL) ? "0" : ui -> inputX2 -> text()) + ", ");
    Draw += (((ui -> inputY2 -> text() == NULL) ? "0" : ui -> inputY2 -> text()) + ", ");
    Draw += (((ui -> inputXY -> text() == NULL) ? "0" : ui -> inputXY -> text()) + ", ");
    Draw += (((ui -> inputX -> text() == NULL) ? "0" : ui -> inputX -> text()) + ", ");
    Draw += (((ui -> inputY -> text() == NULL) ? "0" : ui -> inputY -> text()) + ", ");
    Draw += (((ui -> inputZ -> text() == NULL) ? "0" : ui -> inputZ -> text()) + ");");
    ui -> log -> append(Draw);
    getData();
    recountPixels();
    drawGraph(true);
}
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event -> delta() > 0)
    {
        leftX += 1;
        rightX -= 1;
        botY += 1 * ratio;
        topY -= 1 * ratio;
        ui -> log -> append("ZoomIn();");
    }
    else
    {
        leftX -= 1;
        rightX += 1;
        botY -= 1 * ratio;
        topY += 1 * ratio;
        ui -> log -> append("ZoomOut();");
    }
    recountPixels();
    getData();
    drawGraph(1);
}
/*void MainWindow::mouseEvent(QMouseEvent *event)
{
    ui -> posX -> setText(string(event->localPos().x()));
    ui -> posY -> setText(string(event->localPos().y()));
}*/
