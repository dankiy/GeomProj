#include "graph.h"

graph::graph(QWidget *parent) : QWidget(parent), ui(new Ui::graph)
{
    pictHeight = 380;
    pictWidth = 540;
    leftX = -20; rightX = 20;
    accuracy = 80000;
    step = (abs(leftX) + abs(rightX)) / accuracy;
    ratio = double(pictHeight) / double(pictWidth);
    botY = leftX * (ratio); topY = rightX * (ratio);
    zoomRate = 1;
    recountPixels();
}
