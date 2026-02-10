#pragma once

#include "graphlayer.h"
//
#include "qregularexpression.h"
#include <qvalidator.h>
#include <qmainwindow.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qpalette.h>
#include <qframe.h>
#include <qscrollarea.h>
#include <qlineedit.h>

class Buttons;

class MainWin : public QMainWindow {
   Q_OBJECT
private:
    // POINTERS
    QMainWindow *mainWinPtr;
    QWidget *centralwidget;
    GraphLayer *graphLayerPtr;

    // FUNCTION PROPERTIES
    std::vector<Function *> funPtrs;

    // INPUT PANEL
    QFrame optionPanel;
    QScrollArea scrollArea;
    QWidget inputsArea;
    QPixmap plusPixmap, minusPixmap;
    std::vector<Buttons *> addDelButtons;
    std::vector<QFrame *> inputArea;
    int rowCount;
    QPushButton optionPanelButton;
    QFont font = QFont("Verdana",12,300, false);
    QRegularExpression rexpr = QRegularExpression("(-?0?|-?[1-9]{1}[0-9]{0,2}){1}");
    QValidator *validator;
    QTimer *editTimer;
    QFrame *pendingFrame = nullptr;
    void updateSingleFunction(QFrame* frame);

    //OTHERS
    bool firstResize = true;

private slots:
    void onBnClick();

protected:
    virtual void resizeEvent(QResizeEvent *evt) override;

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();

    Q_INVOKABLE
    void addDelBnClicked(Buttons *ptr);
    //
    void setupMainWin(QMainWindow *mWinPtr);
    void setupInputArea(QFrame *frame);
};


class Buttons : public QLabel {
    Q_OBJECT
private:
    MainWin* mainWin;
    bool state = true; // TRUE MEANS PLUS, FALSE MEANS MINUS
protected:
   void enterEvent(QEnterEvent* event) override {
        Q_UNUSED(event);
        setCursor(Qt::PointingHandCursor);
    }
   void mousePressEvent(QMouseEvent* event) override {
        Q_UNUSED(event);
        QMetaObject::invokeMethod(mainWin, "addDelBnClicked", Qt::DirectConnection, Q_ARG(Buttons*, this));
    }

public:
    bool changeState() {
        if (state)
            state = false;
        else
            state = true;
       return state;
    }
    Buttons(MainWin* mainWin, QWidget *parent = nullptr) : QLabel(parent), mainWin(mainWin) {}
};

