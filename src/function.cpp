#include "function.h"
//
#include "exprtk.h"

const double ARG_JUMP_VALUE = 1.0 / INTERSPACE * 2; // REAL VALUE, WHICH DETERMINES THE PRECISION OF THE GRAPH (HOW MANY POINTS WILL BE CALCULATED)

Function::Function(QWidget* GraphLayer, std::string MathFunctionFormula, float FirstDomainValue, float LastDomainValue, int PenWidth, QColor LineColor): QPolygon() {
    formula = MathFunctionFormula;
    lColor = LineColor;
    pWidth = PenWidth;
    graphLayerPtr = GraphLayer;
    firstDVal = FirstDomainValue;
    lastDVal = LastDomainValue;
    setUpFunction();
}

Function::~Function() {}

void Function::setUpFunction() {
   // skok = INTERSPACE / 16; ---- WARTOSC W PX, CO KTORA LICZONA JEST WARTOSC RZECZYWISTA FUNKCJI
    if ( fmod(firstDVal, ARG_JUMP_VALUE) != 0) {
        if (firstDVal < 0)
             firstDVal = int(firstDVal) + ARG_JUMP_VALUE * int( ( firstDVal-int(firstDVal) ) / ARG_JUMP_VALUE -1);
        else
             firstDVal = int(firstDVal) + ARG_JUMP_VALUE * int( ( firstDVal-int(firstDVal) ) / ARG_JUMP_VALUE );
    }

    if ( fmod(lastDVal, ARG_JUMP_VALUE) != 0) {
        if (lastDVal < 0)
             lastDVal = int(lastDVal) + ARG_JUMP_VALUE * int( ( lastDVal-int(lastDVal) ) / ARG_JUMP_VALUE );
        else
             lastDVal = int(lastDVal) + ARG_JUMP_VALUE * int( ( lastDVal-int(lastDVal) ) / ARG_JUMP_VALUE +1);
    }

    mainWindowPtr = (graphLayerPtr->parentWidget())->parentWidget();

    domainOfTheFunction.reserve( static_cast<int>( (abs(firstDVal) + abs(lastDVal) ) / ARG_JUMP_VALUE ) +1);
    setOfValues.reserve( domainOfTheFunction.capacity() );

    // APPEARANCE OF THE GRAPH
    pen.setColor(lColor);
    pen.setWidth(pWidth);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    reserve(domainOfTheFunction.capacity());

    // CALCULATING FUNCTION'S REAL VALUES

    for (double val = firstDVal; val <= lastDVal; val += ARG_JUMP_VALUE) {
        domainOfTheFunction.emplace_back(val);
        setOfValues.emplace_back( calcValue(formula, val) );
    }

    //qDebug() << *(domainOfTheFunction.begin()) << " " << domainOfTheFunction.back();
    //qDebug() << "zb wartosci " << *(setOfValues.begin()) << setOfValues.back();

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
        push_back(QPoint(x+1,y));
    }
    //qDebug() << domainOfTheFunction.size() << domainOfTheFunction.capacity();
    //qDebug() << setOfValues.size() << setOfValues.capacity();
}

double Function::calcValue(std::string &FunctionFormula, double ArgVal) {
       exprtk::symbol_table<double> symbol_table;
       symbol_table.add_variable("x", ArgVal);

       exprtk::expression<double> expression;
       expression.register_symbol_table(symbol_table);

       exprtk::parser<double> parser;
       parser.compile(FunctionFormula,expression);
       return expression.value();
}

void Function::drawFunction(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(pen);
    painter->drawPolyline(*this);
}
