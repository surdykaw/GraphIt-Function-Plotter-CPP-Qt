#pragma once

#include <vector>
//#include <memory>
//
#include "function.h"
//
#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>

extern const int GRAPH_LAYER_WIDTH;
extern const int GRAPH_LAYER_HEIGHT;
extern const int INTERSPACE;


class GraphLayer: public QWidget {
    //Q_OBJECT

private:
    //POINTERS
    QWidget *mainWinPtr = nullptr;
    QWidget *centralWdtPtr = nullptr;
    QWidget *graphPtr = nullptr;

    // LAYER PROPERTIES
    bool leftBnClicked = false;
    QSize centralWdtSize;
    QPainter painter;    // MAIN PAINTER
    QPalette pal4GraphLayer;
    QPen axisPen, p1, p2;
    QFont font4Values;
    QPointF curMousePos, newMousePos;
    QPoint curGraphLyrPos, newGraphLyrPos;

    // OTHER ELEMENTS
    QPixmap rightPixArr, upPixArr;
    QLabel rArrow, upArrow;
    int curRArrowXPos, curUpArrowYPos;

    // CONTAINERS OF ELEMENTS
    std::vector<QLine> ver;   // VERTICAL AXIS
    std::vector<QLine> hor;   // HORIZONTAL AXIS
    std::vector<QLabel *> vValues;  // VALUES FOR X AXIS
    std::vector<QLabel *> hValues;  // VALUES FOR Y AXIS

    std::vector<Function *> functions;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public:
    GraphLayer(QWidget *parent);
    ~GraphLayer();
    //
    Function* addFunction(std::string &Formula, int D1, int D2, QColor &Color);
    void delFunction(Function* fun);
    void setAxisArrow(QLabel *label, const bool rightArrow);
    void setAxisValue(QLabel *label, int linePos, int xValue, int yValue);
    void updateRArrowPos(QResizeEvent *event);
    void createAxis(QWidget *graphLayerPtr);
    void drawAxis(QPainter &painter, const QPen &pen);
    void setupGraphLyr(QWidget *graphLayer);
};

