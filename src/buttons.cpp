#include "buttons.h"
#include "mainwin.h"
#include <QMouseEvent>
#include <QMetaObject>

Buttons::Buttons(MainWin* mainWin, QWidget* parent) : QLabel(parent), mainWin(mainWin) {}

bool Buttons::changeState() {
    state = !state;
    return state;
}

void Buttons::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    setCursor(Qt::PointingHandCursor);
}

void Buttons::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    QMetaObject::invokeMethod(mainWin, "addDelBnClicked", Qt::DirectConnection, Q_ARG(Buttons*, this));
}
