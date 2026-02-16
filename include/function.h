#pragma once

#include <vector>
#include <string>
#include <qpolygon.h>
#include <qpainter.h>
#include <qwidget.h>
#include "exprtk.h"

extern const int GRAPH_LAYER_WIDTH;
extern const int GRAPH_LAYER_HEIGHT;
extern const int INTERSPACE;

class Function {
private:
    // FUNCTION PROPERTIES
    std::string formula;
    std::vector<float> domain;
    std::vector<float> values;
    QPolygon polyline;
    // POINTERS
    QWidget* graphLayerPtr;
    QWidget* mainWindowPtr;

    QPen pen;
    QColor lineColor;

    float firstDomainValue, lastDomainValue;
    int penWidth;
    int curFirstValuePX, newFirstValuePX;
    //  PARSER PARAMETERS
    double currentX = 0.0;
    exprtk::symbol_table<double> symbolTable;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;

public:
    Function(QWidget* graphLayer, const std::string mathematicalFormula, float firstDomainValue, float lastDomainValue,int penWidth = 3, const QColor lineColor = QColor(0,0,0,255));
    ~Function();
    //
    double calcValue(double xVal);
    void setUpFunction();
    void setValues();
    void drawFunction(QPainter* painter);
};
