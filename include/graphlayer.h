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
#include <QRect>
#include <QString>

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
    QPalette pal4GraphLayer;
    QPen axisPen, p1, p2;
    QFont font4Values;
    QFont axisLabelFont;
    QFontMetrics* axisFontMetrics;
    QPointF curMousePos, newMousePos;
    QPoint curGraphLyrPos, newGraphLyrPos;

    // OTHER ELEMENTS
    QPixmap rightPixArr, upPixArr;
    QLabel rArrow, upArrow;
    int curRArrowXPos, curUpArrowYPos;

    // CONTAINERS OF ELEMENTS
    std::vector<QLine> ver;   // VERTICAL AXIS
    std::vector<QLine> hor;   // HORIZONTAL AXIS

    struct AxisLabelData {
        QRect rect;
        QString text;
    };
    std::vector<AxisLabelData> vLabelData;
    std::vector<AxisLabelData> hLabelData;
    void drawAxisLabelBubbles(QPainter& painter, const std::vector<AxisLabelData>& labels, const QFontMetrics& fm);
    QLabel xAxisLabel;
    QLabel yAxisLabel;
    void updateAxisNamePositions();
    // FUNCTIONS CONTAINER
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
    //void setAxisArrow(QLabel *label, const bool rightArrow);
    void setAxisValue(QLabel *label, int linePos, int xValue, int yValue);
    void updateRArrowPos(QResizeEvent *event);
    void createAxis();
    void drawAxis(QPainter &painter, const QPen &pen);
    void setupGraphLyr(QWidget *graphLayer);
};
