#include "graphlayer.h"
//
#include <qpalette.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpixmap.h>

//#include <QDebug>

const int GRAPH_LAYER_WIDTH = 20001;   // PREDEFINED VALUES USED TO CALCULATE
const int GRAPH_LAYER_HEIGHT = 20001;  // SIZES OF WINDOW LAYERS
const int INTERSPACE = 64;

GraphLayer::GraphLayer(QWidget* parent): QWidget(parent) {
    centralWdtPtr = parent;
    setupGraphLyr(this);
}

GraphLayer::~GraphLayer() {
    if (functions.size() > 0)
        for (Function* pointer : functions)
            delete pointer;
    for (QLabel* ptr : vValues) delete ptr;
    for (QLabel* ptr : hValues) delete ptr;
}

Function* GraphLayer::addFunction(std::string &Formula, int D1, int D2, QColor &Color) {
    Function* ptr = new Function(this, Formula, D1, D2, 3, Color);
    functions.emplace_back(ptr);
    update();
    return ptr;
}

void GraphLayer::delFunction(Function* fun) {
    if (functions.size() > 0)
        for (int i = 0; i < static_cast<int>(functions.size()); i++) {
             if (functions.at(i) == fun) {
                 delete functions.at(i);
                 functions.erase(functions.begin() + i);
                 break;
             }
        }
    update();
}


void GraphLayer::setupGraphLyr(QWidget* graphLayer) {
    graphPtr = graphLayer;
    centralWdtSize = centralWdtPtr->size();
    graphLayer->setGeometry(-GRAPH_LAYER_WIDTH/2 + centralWdtSize.width()/2, -GRAPH_LAYER_HEIGHT/2 + centralWdtSize.height()/2, GRAPH_LAYER_WIDTH, GRAPH_LAYER_HEIGHT);
    curGraphLyrPos = QPoint(-GRAPH_LAYER_WIDTH/2 + centralWdtSize.width()/2, -GRAPH_LAYER_HEIGHT/2 + centralWdtSize.height()/2);
    mainWinPtr = centralWdtPtr->parentWidget();

    // APPEARANCE AND USABILITY
    setUpdatesEnabled(true);
    graphLayer->setMouseTracking(false);
    pal4GraphLayer = QPalette();
    pal4GraphLayer.setColor(QPalette::Window, QColor::fromRgb(240,240,240,255));
    graphLayer->setAutoFillBackground(true);
    graphLayer->setPalette(pal4GraphLayer);
    p1.setWidth(1);
    p2.setWidth(3);
    p2.setColor(QColor(0,0,128,255));
    // AXES AND THEIR VALUES
    ver.reserve( int(GRAPH_LAYER_WIDTH/INTERSPACE) );
    hor.reserve( int(GRAPH_LAYER_HEIGHT/INTERSPACE) );
    vValues.reserve(ver.capacity());
    hValues.reserve(hor.capacity());
    createAxis(graphLayer);
    // AXES ARROWS
    rightPixArr = QPixmap(":/resources/mainAxisArrRight.png");
    upPixArr = QPixmap(":/resources/mainAxisArrUp.png");
    rArrow.setPixmap(rightPixArr);
    upArrow.setPixmap(upPixArr);
    rArrow.setParent(graphLayer);
    rArrow.raise();
    rArrow.setAutoFillBackground(false);
    upArrow.setParent(graphLayer);
    upArrow.raise();
    upArrow.setAutoFillBackground(false);
    curRArrowXPos = GRAPH_LAYER_WIDTH/2 + mainWinPtr->width()/2 - 18;
    curUpArrowYPos = GRAPH_LAYER_HEIGHT/2 - mainWinPtr->height()/2;
}

void GraphLayer::createAxis(QWidget *graphPtr) {
    //QLine(x1,y1,x2,y2) setGeometry(int x, int y, int w, int h)
    font4Values.setFamily("Helvetica");
    font4Values.setPixelSize(15);
    int val = -static_cast<int>(vValues.capacity() / 2);
    // vertical
    for (int i= (GRAPH_LAYER_WIDTH/2) % INTERSPACE; i <= GRAPH_LAYER_WIDTH; i += INTERSPACE){
        ver.emplace_back(QLine(i,0,i,GRAPH_LAYER_HEIGHT));
       if (i != GRAPH_LAYER_WIDTH/2) {
           vValues.emplace_back(new QLabel(graphPtr));
           setAxisValue(vValues.back(),i,val,0);
       }
        val++;
    }
    // horizontal
    val = static_cast<int>(hValues.capacity() / 2);
    for (int i= (GRAPH_LAYER_HEIGHT/2) % INTERSPACE; i <= GRAPH_LAYER_HEIGHT; i += INTERSPACE){
        hor.emplace_back(QLine(0,i,GRAPH_LAYER_WIDTH,i));
        if (i != GRAPH_LAYER_HEIGHT/2) {
            hValues.emplace_back(new QLabel(graphPtr));
            setAxisValue(hValues.back(),i,0,val);
        }
         val--;
    }
}

void GraphLayer::drawAxis(QPainter &painter, const QPen &pen) {
    // VERTICAL AXIS
    for(QLine vLine : ver) {
        if(vLine.x1() == GRAPH_LAYER_WIDTH/2) { // X = 0
            axisPen = painter.pen();
            painter.setPen(pen);
            painter.drawLine(vLine);
            painter.setPen(axisPen);
        }
        else
            painter.drawLine(vLine);
    }
    // HORIZONTAL AXIS
    for(QLine hLine : hor) {
        if(hLine.y1() == GRAPH_LAYER_HEIGHT/2) { // Y = 0
            axisPen = painter.pen();
            painter.setPen(pen);
            painter.drawLine(hLine);
            painter.setPen(axisPen);
        }
        else
            painter.drawLine(hLine);
    }
}

void GraphLayer::setAxisValue(QLabel* label, int linePos, int xValue, int yValue) {
    //setGeometry(int x, int y, int w, int h)
    if ( !(xValue == 0 && yValue == 0) ) {
        label->setFont(font4Values);
        if (yValue == 0) {
            label->setText(QString::number(xValue,10));
            label->adjustSize();
            label->setGeometry( linePos - label->width()/2 , GRAPH_LAYER_HEIGHT/2 - label->height()/2,
                                label->width() +2, label->height() );
        }
        else if (xValue == 0) {
            label->setText(QString::number(yValue,10));
            label->adjustSize();
            label->setGeometry( GRAPH_LAYER_WIDTH/2 - label->width()/2 , linePos - label->height()/2,
                                label->width() +2, label->height() );
        }
        label->setAutoFillBackground(true);
        label->setAlignment(Qt::AlignCenter);
        label->setFrameShape(QFrame::NoFrame);
    }
}

void GraphLayer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    painter.begin(this);
    painter.setPen(p1);
    painter.setOpacity(0.9);
    drawAxis(painter, p2);
    rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10,20,20);
    upArrow.setGeometry(GRAPH_LAYER_WIDTH/2 -10, curUpArrowYPos,20,20);
    if (functions.size() != 0)
        for (Function* wsk : functions)
            wsk->drawFunction(&painter);
    painter.end();
}

void GraphLayer::updateRArrowPos(QResizeEvent* event) {
      if (event->size().width() - event->oldSize().width() < 0) {
          curRArrowXPos = curRArrowXPos - abs(event->size().width() - event->oldSize().width());
          rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10,20,20);
      }
      else if ( event->size().width() - event->oldSize().width() > 0 ) {
          curRArrowXPos = curRArrowXPos + abs(event->size().width() - event->oldSize().width());
          rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10,20,20);
      }
}

void GraphLayer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
    setCursor(Qt::SizeAllCursor);
    curMousePos = event->pos();
    leftBnClicked = true;
   }
}

void GraphLayer::mouseMoveEvent(QMouseEvent* event) {
    if (leftBnClicked) {
        newMousePos = event->pos() - curMousePos;
        newGraphLyrPos.setX(curGraphLyrPos.x() + newMousePos.x() );
        newGraphLyrPos.setY(curGraphLyrPos.y() + newMousePos.y() );
        move(newGraphLyrPos);
        if (curGraphLyrPos.x() > newGraphLyrPos.x())
            curRArrowXPos = curRArrowXPos + ( abs(newGraphLyrPos.x() - curGraphLyrPos.x()) );
        else
            curRArrowXPos = curRArrowXPos - ( abs(newGraphLyrPos.x() - curGraphLyrPos.x()) );

        if (curGraphLyrPos.y() > newGraphLyrPos.y()) {
            curUpArrowYPos = curUpArrowYPos + ( abs(newGraphLyrPos.y() - curGraphLyrPos.y()) );
        } else
            curUpArrowYPos = curUpArrowYPos - ( abs(newGraphLyrPos.y() - curGraphLyrPos.y()) );
        curGraphLyrPos = newGraphLyrPos;
        graphPtr->update();
    }
}

void GraphLayer::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    leftBnClicked = false;
}
