#include "function.h"
#include "exprtk.h"

// Sampling step in mathematical domain
// Determines graph precision; smaller value = smoother but slower
const double ARG_STEP_VALUE = 2.0 / static_cast<double>(INTERSPACE); // REAL VALUE, WHICH DETERMINES THE PRECISION OF THE GRAPH (HOW MANY POINTS WILL BE CALCULATED)

Function::Function(QWidget* graphLayer, const std::string mathFunctionFormula, float firstDomainValue, float lastDomainValue, int penWidth, const QColor lineColor) {
    formula = mathFunctionFormula;
    this->lineColor = lineColor;
    this->penWidth = penWidth;
    graphLayerPtr = graphLayer;
    this->firstDomainValue = firstDomainValue;
    this->lastDomainValue = lastDomainValue;
    // PARSER PREPARATION
    symbolTable.add_variable("x", currentX);
    symbolTable.add_constants();
    expression.register_symbol_table(symbolTable);

    if (!parser.compile(formula, expression)) {
        throw std::runtime_error("Expression parse error");
    }

    setUpFunction();
}

Function::~Function() {}

void Function::setUpFunction() {
    // ALIGN DOMAIN BOUNDARIES TO SAMPLING STEP TO PREVENT VISUAL SHIFTING OF THE PLOT
    // AND FLOATING-POINT ACCUMULATIONS ERRORS WHEN MAPPING FUNCTION VALUES TO GRID PIXELS
    if ( fmod(firstDomainValue, ARG_STEP_VALUE) != 0) {
        if (firstDomainValue < 0)
             firstDomainValue = int(firstDomainValue) + ARG_STEP_VALUE * int( ( firstDomainValue-int(firstDomainValue) ) / ARG_STEP_VALUE -1);
        else
             firstDomainValue = int(firstDomainValue) + ARG_STEP_VALUE * int( ( firstDomainValue-int(firstDomainValue) ) / ARG_STEP_VALUE );
    }

    if ( fmod(lastDomainValue, ARG_STEP_VALUE) != 0) {
        if (lastDomainValue < 0)
             lastDomainValue = int(lastDomainValue) + ARG_STEP_VALUE * int( ( lastDomainValue-int(lastDomainValue) ) / ARG_STEP_VALUE );
        else
             lastDomainValue = int(lastDomainValue) + ARG_STEP_VALUE * int( ( lastDomainValue-int(lastDomainValue) ) / ARG_STEP_VALUE +1);
    }

    domain.reserve( static_cast<int>( (abs(firstDomainValue) + abs(lastDomainValue) ) / ARG_STEP_VALUE ) +1);
    values.reserve( domain.capacity() );

    // APPEARANCE OF THE PLOT
    pen.setColor(lineColor);
    pen.setWidth(penWidth);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    polyline.reserve(domain.capacity());

    // CALCULATING FUNCTION'S REAL VALUES
    for (double val = firstDomainValue; val <= lastDomainValue; val += ARG_STEP_VALUE) {
        domain.emplace_back(val);
        values.emplace_back(calcValue(val));
    }

    // CONVERTING FUNCTION REAL VALUES INTO PX VALUES
    int x,y;
    for (int i = 0; i < static_cast<int>(domain.size()); i++) {

        if (domain.at(i) < 0)
            x = graphLayerPtr->width()/2 - qRound(abs(domain.at(i)) * INTERSPACE);
        else if (domain.at(i) > 0)
            x = graphLayerPtr->width()/2 + qRound(abs(domain.at(i)) * INTERSPACE);
        else x = graphLayerPtr->width()/2;

        if (values.at(i) < 0) {
            y = graphLayerPtr->height()/2 + qRound(abs(values.at(i) * INTERSPACE));
            if (y > graphLayerPtr->height() ) y = graphLayerPtr->height();
        }
        else if (values.at(i) > 0) {
            y = graphLayerPtr->height()/2 - qRound( abs(values.at(i) * INTERSPACE) );
            if (y < 0) y = 0;
        }
        else
            y = graphLayerPtr->height()/2;
        polyline.push_back(QPoint(x+1,y));
    }
}

double Function::calcValue(double xVal) {
    currentX = xVal;
    return expression.value();
}

void Function::drawFunction(QPainter* painter) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->drawPolyline(polyline);
}
