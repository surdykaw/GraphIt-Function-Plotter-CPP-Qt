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
    delete axisFontMetrics;
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
    createAxis();

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

    // AXIS NAMES
    xAxisLabel.setParent(graphLayer);
    yAxisLabel.setParent(graphLayer);
    xAxisLabel.setText("X");
    yAxisLabel.setText("Y");
    xAxisLabel.setFont(axisLabelFont);
    yAxisLabel.setFont(axisLabelFont);
    xAxisLabel.adjustSize();
    yAxisLabel.adjustSize();
    xAxisLabel.setAttribute(Qt::WA_TransparentForMouseEvents);
    yAxisLabel.setAttribute(Qt::WA_TransparentForMouseEvents);
    xAxisLabel.show();
    yAxisLabel.show();
}

void GraphLayer::createAxis() {
    font4Values.setFamily("Helvetica");
    font4Values.setPixelSize(15);
    axisLabelFont = font4Values;
    axisLabelFont.setBold(true);
    axisFontMetrics = new QFontMetrics(axisLabelFont);

    int val = -(GRAPH_LAYER_WIDTH / INTERSPACE) / 2;

    // VERTICAL LINES + X axis values
    for (int i = (GRAPH_LAYER_WIDTH/2) % INTERSPACE; i <= GRAPH_LAYER_WIDTH; i += INTERSPACE) {

        ver.emplace_back(QLine(i, 0, i, GRAPH_LAYER_HEIGHT));
        if (i != GRAPH_LAYER_WIDTH/2) {
            AxisLabelData data;
            data.text = QString::number(val);
            data.rect = QRect(i - 20, GRAPH_LAYER_HEIGHT/2 - 10, 40, 20);
            vLabelData.push_back(data);
        }
        val++;
    }

    val = (GRAPH_LAYER_HEIGHT / INTERSPACE) / 2;

    // HORIZONTAL LINES + Y axis values
    for (int i = (GRAPH_LAYER_HEIGHT/2) % INTERSPACE; i <= GRAPH_LAYER_HEIGHT; i += INTERSPACE) {

        hor.emplace_back(QLine(0, i, GRAPH_LAYER_WIDTH, i));

        if (i != GRAPH_LAYER_HEIGHT/2) {
            AxisLabelData data;
            data.text = QString::number(val);
            data.rect = QRect(GRAPH_LAYER_WIDTH/2 - 20, i - 10, 40, 20);
            hLabelData.push_back(data);
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

void GraphLayer::drawAxisLabelBubbles(QPainter& painter, const std::vector<AxisLabelData>& labels, const QFontMetrics& fm) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,255,255,220));

    for (const auto& lbl : labels) {
        QRect textSize = fm.boundingRect(lbl.text);
        int size = std::max(textSize.width(), textSize.height()) + 6;
        QRect bgRect(lbl.rect.center().x() - size/2, lbl.rect.center().y() - size/2, size, size);
        painter.drawEllipse(bgRect);
        painter.setPen(Qt::black);
        painter.drawText(bgRect, Qt::AlignCenter, lbl.text);
        painter.setPen(Qt::NoPen);
    }
}

void GraphLayer::updateAxisNamePositions() {
    // X AXIS LABEL
    xAxisLabel.move(curRArrowXPos - rArrow.width(), rArrow.y() + rArrow.height());

    // Y AXIS LABEL
    yAxisLabel.move(upArrow.x() + upArrow.width(), curUpArrowYPos + upArrow.height());

    // SET VISIBILITY
    xAxisLabel.setVisible(rArrow.isVisible());
    yAxisLabel.setVisible(upArrow.isVisible());
}

void GraphLayer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setOpacity(0.9);

    // ARROWS
    rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10, 20, 20);
    upArrow.setGeometry(GRAPH_LAYER_WIDTH/2 -10, curUpArrowYPos, 20, 20);

    updateAxisNamePositions();

    // AXES
    drawAxis(painter, p2);

    // WARTOŚCI OSI
    painter.setFont(axisLabelFont);
    drawAxisLabelBubbles(painter, vLabelData, *axisFontMetrics);
    drawAxisLabelBubbles(painter, hLabelData, *axisFontMetrics);

    // FUNKCJE
    for (Function* fun : functions)
        fun->drawFunction(&painter);
}

void GraphLayer::updateRArrowPos(QResizeEvent* event) {
    if (event->size().width() - event->oldSize().width() < 0) {
        curRArrowXPos = curRArrowXPos - abs(event->size().width() - event->oldSize().width());
        rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10,20,20);
    }
    else if (event->size().width() - event->oldSize().width() > 0) {
        curRArrowXPos = curRArrowXPos + abs(event->size().width() - event->oldSize().width());
        rArrow.setGeometry(curRArrowXPos, GRAPH_LAYER_HEIGHT/2 -10,20,20);
    }
    updateAxisNamePositions();
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
        updateAxisNamePositions();
        curGraphLyrPos = newGraphLyrPos;
        graphPtr->update();
    }
}

void GraphLayer::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    leftBnClicked = false;
}
