#pragma once

#include <QLabel>

class MainWin;

class Buttons : public QLabel {
    Q_OBJECT

private:
    MainWin* mainWin;
    bool state = true;

protected:
    void enterEvent(QEnterEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

public:
    Buttons(MainWin* mainWin, QWidget* parent = nullptr);
    bool changeState();
};
