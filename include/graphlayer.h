#pragma once

#include <vector>
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
    // GraphLayer implements a camera-over-plane rendering model
    // The coordinate system is fixed and very large
    // User navigates by moving the layer instead of scaling the graph
private:
    // POINTERS
    QWidget* mainWinPtr = nullptr;
    QWidget* centralWidgetPtr = nullptr;
    QWidget* graphPtr = nullptr;

    // LAYER PROPERTIES
    bool leftBnClicked = false;
    QSize centralWidgetSize;
    QPalette pal4GraphLayer;
    QPen axisPen, p1, p2;
    QFont font4Values;
    QFont axisLabelFont;
    QFontMetrics axisFontMetrics{QFont()};
    QPointF curMousePos, newMousePos;
    QPoint graphLayerPos, newGraphLayerPos;

    // OTHER ELEMENTS
    QPixmap rightPixArr, upPixArr;
    QLabel rArrow, upArrow;
    int curRArrowXPos, curUpArrowYPos;

    // AXIS GRID
    std::vector<QLine> verticalLines;   // VERTICAL AXES
    std::vector<QLine> horizontalLines;   // HORIZONTAL AXES

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
    // FUNCTIONS OBJECTS (OWNED)
    std::vector<Function *> functions;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

public:
    GraphLayer(QWidget* parent);
    ~GraphLayer();
    //
    Function* addFunction(const std::string& formula, int firstDomainValue, int lastDomainValue, const QColor& color);
    void delFunction(Function* fun);
    void setAxisValue(QLabel* label, int linePos, int xValue, int yValue);
    void updateRArrowPos(QResizeEvent* event);
    void createAxis();
    void drawAxis(QPainter& painter, QPen& pen);
    void setupGraphLayer(QWidget* graphLayer);
};
