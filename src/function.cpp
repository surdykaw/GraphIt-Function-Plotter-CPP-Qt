#include "function.h"
#include "exprtk.h"

const double ARG_JUMP_VALUE = 1.0 / INTERSPACE * 2; // REAL VALUE, WHICH DETERMINES THE PRECISION OF THE GRAPH (HOW MANY POINTS WILL BE CALCULATED)

Function::Function(QWidget* GraphLayer, std::string MathFunctionFormula, float FirstDomainValue, float LastDomainValue, int PenWidth, QColor LineColor) {
    formula = MathFunctionFormula;
    lColor = LineColor;
    pWidth = PenWidth;
    graphLayerPtr = GraphLayer;
    firstDomainValue = FirstDomainValue;
    lastDomainValue = LastDomainValue;
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
   // skok = INTERSPACE / 16; ---- WARTOSC W PX, CO KTORA LICZONA JEST WARTOSC RZECZYWISTA FUNKCJI
    if ( fmod(firstDomainValue, ARG_JUMP_VALUE) != 0) {
        if (firstDomainValue < 0)
             firstDomainValue = int(firstDomainValue) + ARG_JUMP_VALUE * int( ( firstDomainValue-int(firstDomainValue) ) / ARG_JUMP_VALUE -1);
        else
             firstDomainValue = int(firstDomainValue) + ARG_JUMP_VALUE * int( ( firstDomainValue-int(firstDomainValue) ) / ARG_JUMP_VALUE );
    }

    if ( fmod(lastDomainValue, ARG_JUMP_VALUE) != 0) {
        if (lastDomainValue < 0)
             lastDomainValue = int(lastDomainValue) + ARG_JUMP_VALUE * int( ( lastDomainValue-int(lastDomainValue) ) / ARG_JUMP_VALUE );
        else
             lastDomainValue = int(lastDomainValue) + ARG_JUMP_VALUE * int( ( lastDomainValue-int(lastDomainValue) ) / ARG_JUMP_VALUE +1);
    }

    mainWindowPtr = (graphLayerPtr->parentWidget())->parentWidget();

    domainOfTheFunction.reserve( static_cast<int>( (abs(firstDomainValue) + abs(lastDomainValue) ) / ARG_JUMP_VALUE ) +1);
    setOfValues.reserve( domainOfTheFunction.capacity() );

    // APPEARANCE OF THE GRAPH
    pen.setColor(lColor);
    pen.setWidth(pWidth);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    poly.reserve(domainOfTheFunction.capacity());

    // CALCULATING FUNCTION'S REAL VALUES
    for (double val = firstDomainValue; val <= lastDomainValue; val += ARG_JUMP_VALUE) {
        domainOfTheFunction.emplace_back(val);
        setOfValues.emplace_back(calcValue(val));
    }

    // CONVERTING FUNCTION REAL VALUES INTO PX VALUES
    int x,y;
    for (int i = 0; i < static_cast<int>(domainOfTheFunction.size()); i++) {

        if (domainOfTheFunction.at(i) < 0)
            x = graphLayerPtr->width()/2 - qRound(abs(domainOfTheFunction.at(i)) * INTERSPACE);
        else if (domainOfTheFunction.at(i) > 0)
            x = graphLayerPtr->width()/2 + qRound(abs(domainOfTheFunction.at(i)) * INTERSPACE);
        else x = graphLayerPtr->width()/2;

        if (setOfValues.at(i) < 0) {
            y = graphLayerPtr->height()/2 + qRound(abs(setOfValues.at(i) * INTERSPACE));
            if (y > graphLayerPtr->height() ) y = graphLayerPtr->height();
        }
        else if (setOfValues.at(i) > 0) {
            y = graphLayerPtr->height()/2 - qRound( abs(setOfValues.at(i) * INTERSPACE) );
            if (y < 0) y = 0;
        }
        else
            y = graphLayerPtr->height()/2;
        poly.push_back(QPoint(x+1,y));
    }
    //qDebug() << domainOfTheFunction.size() << domainOfTheFunction.capacity();
    //qDebug() << setOfValues.size() << setOfValues.capacity();
}

double Function::calcValue(double xVal) {
    currentX = xVal;
    return expression.value();
}

void Function::drawFunction(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->drawPolyline(poly);
}
