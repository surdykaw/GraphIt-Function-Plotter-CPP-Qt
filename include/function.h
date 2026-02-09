#pragma once

#include <vector>
#include <string>
#include <qpolygon.h>
#include <qpainter.h>
#include <qwidget.h>

#include <exprtk.h>

extern const int GRAPH_LAYER_WIDTH;
extern const int GRAPH_LAYER_HEIGHT;
extern const int INTERSPACE;

class Function {

private:
    // FUNCTION PROPERTIES
    std::string formula;
    std::vector<float> domainOfTheFunction;
    std::vector<float> setOfValues;
    QPolygon poly;
    // POINTERS
    QWidget *graphLayerPtr;
    QWidget *mainWindowPtr;

    QPen pen;
    QColor lColor;

    float firstDomainValue, lastDomainValue;
    int pWidth;
    int curFirstValuePX, newFirstValuePX;
    int skok;
    //  PARSER PARAMETERS
    double currentX = 0.0;
    exprtk::symbol_table<double> symbolTable;
    exprtk::expression<double> expression;
    exprtk::parser<double> parser;

public:
    Function(QWidget* GraphLayer, std::string MathematicalFormula, float FirstDomainValue, float LastDomainValue,int PenWidth = 3, QColor LineColor = QColor(0,0,0,255));
    ~Function();
    //
    double calcValue(double xVal);
    void setUpFunction();
    void setValues();
    void drawFunction(QPainter* painter);
};
