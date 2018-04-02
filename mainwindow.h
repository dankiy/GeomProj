#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

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
    double fy(double x, bool t);
    double fx(bool t);
private slots:

    void on_drw_clicked();

    // void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    double X2, Y2, X, Y, XY, Z, D;
    double leftX, rightX;
    double botY, topY;
    int pictWidth, pictHeight;
    double step;
    double onePixelX, onePixelY;
    double Ox, Oy;
};

#endif // MAINWINDOW_H
