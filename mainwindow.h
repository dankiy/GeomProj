#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <curve.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(bool notEmpty = 0);
    void recountPixels();
    void getData();
    //double fy(double x, bool t);
    //double fx(bool t);
    // double fsqrt(double x);
private slots:
    void on_drw_clicked();
    // void on_clear_clicked();
protected:
    virtual void wheelEvent(QWheelEvent *event);
    //virtual void mouseEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    Curve func;
    int type;
    double leftX, rightX;
    double botY, topY;
    int pictWidth, pictHeight;
    double ratio;
    double step;
    double onePixelX, onePixelY;
    double Ox, Oy;
};

#endif // MAINWINDOW_H
