#ifndef CURVE_H
#define CURVE_H

class Curve
{    
public:
    Curve();
    Curve(double x2, double y2, double xy, double x, double y, double z);
    //int setExpr(double x2, double y2, double xy, double x, double y, double z);
    double ftest(double x);
    double fy(double x, bool t);
    double fx(bool t);
    int getType();
private:
    double X2, Y2, X, Y, XY, Z, D;
};

#endif // CURVE_H
