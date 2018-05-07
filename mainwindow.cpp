#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <fstream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //scrollArea(new QScrollArea),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //logger = Log();
    //graph outputGraph = new graph();
    //scrollArea -> setVisible(false);
    //scrollArea -> setWidget(ui -> outputGraph);
    //ui -> scrollArea -> setVisible(false);
    pictHeight = 380;
    pictWidth = 540;
    leftX = -20; rightX = 20;
    accuracy = 80000;
    step = (abs(leftX) + abs(rightX)) / accuracy;
    ratio = double(pictHeight) / double(pictWidth);
    botY = leftX * (ratio); topY = rightX * (ratio);
    zoomRate = 1;
    recountPixels();
    drawGraph(false); 
}

MainWindow::~MainWindow()
{
    logger.AddNote("_______________END_____________");
    delete ui;
}
/*
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
*/
void MainWindow::recountPixels()
{
    logger.AddNote(">func: recountPixels \n");
    onePixelX = pictWidth / (rightX-leftX);
    onePixelY = pictHeight / (topY-botY);
    Ox = fabs(rightX); Oy = fabs(topY);
    step = (abs(leftX) + abs(rightX)) / accuracy;
    logger.AddNote("onePixelX: " + to_string(onePixelX) +
                   "\nonePixelY: " + to_string(onePixelY) +
                   "\nOx: " + to_string(Ox) +
                   "\nOy: " + to_string(Oy) +
                   "\nstep: " + to_string(step) + '\n');
    logger.AddNote(">funcOut: recountPixels \n");
}
void MainWindow::getData()
{
    logger.AddNote(">func: getData \n");
    func =  Curve(ui -> inputX2 -> text().toDouble(),
                  ui -> inputY2 -> text().toDouble(),
                  ui -> inputXY -> text().toDouble(),
                  ui -> inputX -> text().toDouble(),
                  ui -> inputY -> text().toDouble(),
                  ui -> inputZ -> text().toDouble());
    type = func.getType();
    logger.AddNote("Curve type: " + to_string(type) + '\n');
    logger.AddNote(">funcOut: getData \n");
}
/*
void MainWindow::getLine()
{
    func = Curve()
}
*/
void MainWindow::drawGraph(bool notEmpty)
{
    logger.AddNote(">func: drawGraph \n");
    QPixmap Graph(pictWidth, pictHeight);
    QPainter paint;
    paint.begin(&Graph);
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
        ui->outputGraph->setPixmap(Graph);
        return;
    }

    paint.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    if (type == 0)
    {
        bool first = true;
        double last = func.ftest(leftX);

        for(double i = (double)leftX + step; i <= (double)rightX; i += step)
        {
            if(!isnan(func.ftest(i)))
            {
                if(first)
                {
                    last = func.ftest(i);
                    path.moveTo((i + Ox) * onePixelX, (func.ftest(i) + Oy) * onePixelY);
                    first = false;
                }
                else
                {
                    if ((abs(last - func.ftest(i)) >= (topY-botY)))
                    {
                        first = true;
                    }
                    else
                    {
                    last = func.ftest(i);
                    path.lineTo((i + Ox) * onePixelX, (func.ftest(i) + Oy) * onePixelY);
                    }
                }
            }
            else
                first = true;
        }
    }
    else
    {
        if (type == 1)
        {
            bool signD = true;

            if(!isnan(func.fx(signD)))
            {
                path.moveTo((Ox + func.fx(signD)) * onePixelY, 0);
                path.lineTo((Ox + func.fx(signD)) * onePixelY, pictHeight);

            }

            signD = false;

            if(!isnan(func.fx(signD)))
            {
                path.moveTo((Ox + func.fx(signD)) * onePixelY, 0);
                path.lineTo((Ox + func.fx(signD)) * onePixelY, pictHeight);
            }
        }
        else
        {
            bool first = true;
            bool signD = true;
            double last;

            for(double i = (double)leftX + step; i <= (double)rightX; i += step)
            {
                if(!isnan(func.fy(i, signD)))
                {

                    if(first)
                    {
                        last = func.fy(i, signD);
                        path.moveTo((i + Ox) * onePixelX, (func.fy(i, signD) + Oy) * onePixelY);
                        first = false;
                    }
                    else
                    {
                        if ((abs(last - func.fy(i, signD)) >= (topY-botY)))
                        {
                            first = true;
                        }
                        else
                        {
                        last = func.fy(i, signD);
                        path.lineTo((i + Ox) * onePixelX, (func.fy(i, signD) + Oy) * onePixelY);
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
                if(!isnan(func.fy(i, signD)))
                {


                    if(first)
                    {
                        last = func.fy(i, signD);
                        path.moveTo((i + Ox) * onePixelX, (func.fy(i, signD) + Oy) * onePixelY);
                        first = false;
                    }
                    else
                    {
                        if ((abs(last - func.fy(i, signD)) >= (topY-botY)))
                        {
                            first = true;
                        }
                        else
                        {
                        last = func.fy(i, signD);
                        path.lineTo((i + Ox) * onePixelX, (func.fy(i, signD) + Oy) * onePixelY);
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
    ui->outputGraph->setPixmap(Graph);
    logger.AddNote("pictHeight: " + to_string(pictHeight) +
                   "\npictWidth: " + to_string(pictWidth) +
                   "\nleftX: " + to_string(leftX) +
                   "\nrightX: " + to_string(rightX) +
                   "\nbotY: " + to_string(botY) +
                   "\nrightY: " + to_string(topY) +
                   "\naccuracy: " + to_string(accuracy) +
                   "\nstep: " + to_string(step) +
                   "\nratio: " + to_string(ratio) +
                   "\nzoomRate: " + to_string(zoomRate) + '\n');
    logger.AddNote(">funcOut: drawGraph \n");
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
    logger.AddNote(">event: on_drw_clicked \n");
    QString Draw = "Draw(";
    Draw += (((ui -> inputX2 -> text() == NULL) ? "0" : ui -> inputX2 -> text()) + ", ");
    Draw += (((ui -> inputY2 -> text() == NULL) ? "0" : ui -> inputY2 -> text()) + ", ");
    Draw += (((ui -> inputXY -> text() == NULL) ? "0" : ui -> inputXY -> text()) + ", ");
    Draw += (((ui -> inputX -> text() == NULL) ? "0" : ui -> inputX -> text()) + ", ");
    Draw += (((ui -> inputY -> text() == NULL) ? "0" : ui -> inputY -> text()) + ", ");
    Draw += (((ui -> inputZ -> text() == NULL) ? "0" : ui -> inputZ -> text()) + ");");
    ui -> log -> append(Draw);
    logger.AddNote(Draw.toStdString() + '\n');
    getData();
    recountPixels();
    drawGraph(true);
    logger.AddNote(">eventEnd: on_drw_clicked \n");
}
/*void MainWindow::on_drws_clicked()
{
    QString Draw = "Draw(";
    Draw += ui->inputLine->text();
    Draw += ");"
    //getLine();
    recountPixels();
    drawGraph(true);
}
*/
void MainWindow::wheelEvent(QWheelEvent *event)
{
    logger.AddNote(">event: wheelEvent \n");
    if (event -> delta() > 0)
    {
        if ((leftX < zoomRate * 5) && (rightX > zoomRate * 5))
        {
            leftX += zoomRate;
            rightX -= zoomRate;
            botY += zoomRate * ratio;
            topY -= zoomRate * ratio;
            ui -> log -> append("ZoomIn();");
            logger.AddNote("Zoom In with Rate: " + to_string(zoomRate) + '\n');
        }
    }
    else
    {
        leftX -= zoomRate;
        rightX += zoomRate;
        botY -= zoomRate * ratio;
        topY += zoomRate * ratio;
        logger.AddNote("Zoom Out with Rate: " + to_string(zoomRate) + '\n');
        ui -> log -> append("ZoomOut();");
    }
    recountPixels();
    drawGraph(1);
    logger.AddNote(">eventEnd: wheelEvent \n");
}
/*void MainWindow::mouseEvent(QMouseEvent *event)
{
    ui -> posX -> setText(string(event->localPos().x()));
    ui -> posY -> setText(string(event->localPos().y()));
}*/
