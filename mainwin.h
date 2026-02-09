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
//
#include <string.h>


class Buttons;

class MainWin : public QMainWindow {
   Q_OBJECT
private:
    // POINTERS
    QMainWindow *mainWinPtr;
    QWidget *centralwidget;
    GraphLayer *graphLrPtr;

    // FUNCTION PROPERTIES
    std::string formula;
    QColor color;
    int firstDValue, lastDValue;
    std::vector<Function *> funPtrs;

    // INPUT PANEL
    QFrame optionPanel;
    QScrollArea scrollArea;
    QWidget inputsArea;
    QPixmap plusPixmap, minusPixmap;
    std::vector<Buttons *> addDelButtons;
    std::vector<QFrame *> inputArea;
    int fieldsCount;
    QPushButton optionPanelButton;
    QFont font = QFont("Verdana",12,300, false);
    QRegularExpression rexpr = QRegularExpression("(-?0?|-?[1-9]{1}[0-9]{0,2}){1}");
    QValidator *validator;

    //OTHERS
    bool firstResize = true;

private slots:
    void onBnClick();
    void onEditFinish();

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
    void setFunctionValues(QLineEdit *Input, QLineEdit *D1, QLineEdit *D2, QComboBox *Colors);
};


class Buttons : public QLabel {
private:
    QMainWindow *mainWin;
    bool state = true; // TRUE MEANS PLUS, FALSE MEANS MINUS
protected:
   void enterEvent(QEnterEvent *event) override {
        setCursor(Qt::PointingHandCursor);
    }
   void mousePressEvent(QMouseEvent *event) override {
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
    Buttons(QWidget *parent) : QLabel(parent) {
        mainWin = static_cast<QMainWindow *>(this->parent()->parent()->parent()->parent()->parent());
    }
};

