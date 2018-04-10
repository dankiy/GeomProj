#include "curve.h"
#include <cmath>

Curve::Curve()
{
    X2 = 0;
    Y2 = 0;
    XY = 0;
    X = 0;
    Y = 0;
    Z = 0;
}
Curve::Curve(double x2, double y2, double xy, double x, double y, double z)
{
    X2 = x2;
    Y2 = y2;
    XY = xy;
    X = x;
    Y = y;
    Z = z;
}
int Curve::getType()
{
    if (X == 7357)
        return 0;
    else if ((Y2 == 0) && (XY == 0) && (Y == 0))
        return 1;
    else
        return 2;
}
double Curve::fx(bool signD)
{
    if (X2 != 0)
    {
        D = pow(X, 2) - 4 * (X2 * Z);
        if (signD)
            return (sqrt(D) - X) / (2 * X2);
        else
            return (-sqrt(D) - X) / (2 * X2);
    }
    else
        return -Z / X;
}

double Curve::fy(double x, bool signD)
{
   if (Y2 != 0)
   {
        D = pow((XY * x + Y), 2) - 4 * (Y2 * (X2 * pow(x,2) + X * x + Z));
        if (signD)
            return (sqrt(D) - (XY * x + Y)) / (2 * Y2);
        else
            return (-sqrt(D) - (XY * x + Y)) / (2 * Y2);
   }
   else
       return ((X * x + X2 * pow(x, 2) + Z)/(Y + x * XY));
}
double Curve::ftest(double x)
{
    return (1/(sin(x)));
}
