#pragma once

#include <vector>
#include <string>
#include <qpolygon.h>
#include <qpainter.h>
#include <qwidget.h>
//

extern const int GRAPH_LAYER_WIDTH;
extern const int GRAPH_LAYER_HEIGHT;
extern const int INTERSPACE;

class Function: public QPolygon {

private:
    // FUNCTION PROPERTIES
    std::string formula;
    std::vector<float> domainOfTheFunction;
    std::vector<float> setOfValues;
    // POINTERS
    QWidget *graphLayerPtr;
    QWidget *mainWindowPtr;

    QPen pen;
    QColor lColor;

    float firstDVal, lastDVal;
    int pWidth;
    int curFirstValuePX, newFirstValuePX;
    int skok;

public:
    Function(QWidget *GraphLayer, std::string MathematicalFormula, float FirstDomainValue, float LastDomainValue,int PenWidth = 3, QColor LineColor = QColor(0,0,0,255));
    ~Function();
    //
    double calcValue(std::string &FunctionFormula, double ArgVal);
    void setUpFunction();
    void setValues();
    void drawFunction(QPainter *painter);
};
