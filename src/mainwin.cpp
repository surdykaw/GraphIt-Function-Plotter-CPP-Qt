#include "mainwin.h"
//
#include <locale>
//
#include <qlabel.h>
#include <QComboBox>
#include <QString>
#include <QTimer>

const int MAIN_WIN_WIDTH = 1002;
const int MAIN_WIN_HEIGHT = 600;

MainWin::MainWin(QWidget* Parent): QMainWindow(Parent) {
    setupMainWin(this);
}

MainWin::~MainWin() {
    delete validator;
}

void MainWin::setupMainWin(QMainWindow* MainWinPointer) {

    // MAIN WINDOW
    mainWinPtr = MainWinPointer;
    mainWinPtr->setObjectName("mainwindow");
    mainWinPtr->setWindowTitle("GraphIt");
    mainWinPtr->setMinimumSize(600,400);
    mainWinPtr->resize(MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT);

    // CENTRAL WIDGET
    centralwidget = new QWidget(mainWinPtr);
    centralwidget->setObjectName("centralwidget");
    centralwidget->setGeometry(0,0,MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT);

    // GRAPH LAYER
    graphLrPtr = new GraphLayer(centralwidget);

    optionPanelButton.setParent(centralwidget);
    optionPanelButton.setIcon(QIcon(":/resources/arrowDown.png"));
    connect(&optionPanelButton, &QPushButton::clicked, this, &MainWin::onBnClick);

    optionPanel.setParent(centralwidget);
    optionPanel.setGeometry(251,465,500,250);
    optionPanel.setFrameShape(QFrame::StyledPanel);
    optionPanel.setFrameShadow(QFrame::Sunken);
    optionPanel.setAutoFillBackground(true);
    optionPanel.setVisible(true);

    // SCROLL'S AREA
    scrollArea.setParent(&optionPanel);
    scrollArea.setGeometry(10,10,480,230);
    scrollArea.setWidget(&inputsArea);
    inputsArea.setParent(&scrollArea);
    scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    scrollArea.setWidgetResizable(false);
    scrollArea.setFrameShape(QFrame::NoFrame);
    inputsArea.resize(480,40);

	// OPTION PANEL
    plusPixmap = QPixmap(":/resources/plusCircle_40.png");
    minusPixmap = QPixmap(":/resources/minusRounded_40.png");

	// BUTTONS FOR ADDING AND DELETING INPUT FIELDS
    addDelButtons.emplace_back(new Buttons(this, &inputsArea));
    addDelButtons.front()->setGeometry(0,0,40,40);
    addDelButtons.front()->setPixmap(plusPixmap);

    validator = new QRegularExpressionValidator(rexpr,this);

    fieldsCount = 1;
    setCentralWidget(centralwidget);
    editTimer = new QTimer(this);
    editTimer->setSingleShot(true);

    connect(editTimer, &QTimer::timeout, this, [this]() {
        if (pendingFrame)
            updateSingleFunction(pendingFrame);
    });
}

void MainWin::setupInputArea(QFrame* frame) {
    frame->setGeometry(45, (fieldsCount-1)*40 + (fieldsCount-1)*5, 437, 40);
    frame->setFrameShape(QFrame::NoFrame);
    QLineEdit *input = new QLineEdit(frame);
    QLineEdit *d1 = new QLineEdit(frame);
    QLineEdit *d2 = new QLineEdit(frame);
    QComboBox *colors = new QComboBox(frame);
    colors->setFrame(false);
    colors->addItems({"Czarny", "Czerwony", "Niebieski", "Żółty", "Zielony", "Fioletowy"});
    colors->setGeometry(325, 0, 90, 40);
    input->setGeometry(0, 0, 220, 40);
    input->setFont(font);
    input->setPlaceholderText("y:= ...");
    d1->setGeometry(230, 0, 40, 40);
    d2->setGeometry(275, 0, 40, 40);
    d1->setFont(font);
    d2->setFont(font);
    d1->setAlignment(Qt::AlignCenter);
    d2->setAlignment(Qt::AlignCenter);
    d1->setPlaceholderText("-1");
    d2->setPlaceholderText("1");
    d1->setValidator(validator);
    d2->setValidator(validator);
    //connect(input, &QLineEdit::editingFinished, this, &MainWin::onEditFinished);
    connect(input, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(400);
    });

    connect(d1, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(400);
    });

    connect(d2, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(400);
    });

    connect(colors, &QComboBox::currentIndexChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(200);
    });
    frame->show();
}


void MainWin::addDelBnClicked(Buttons* ptr) {
   for (Buttons* wsk : addDelButtons) {
       if ( wsk == ptr ) {
           if ( !ptr->changeState() ) { // "PLUS" BUTTON CLICKED
               if (fieldsCount == 1) {
                   addDelButtons.back()->setPixmap(minusPixmap);
                   addDelButtons.front()->show();
                   inputsArea.setGeometry(0,0,480, (fieldsCount+1)*40 + (fieldsCount+1)*5);
                   addDelButtons.emplace_back(new Buttons(this, &inputsArea));
                   addDelButtons.back()->setGeometry(0, (fieldsCount*40 + fieldsCount*5) ,40,40);
                   addDelButtons.back()->setPixmap(plusPixmap);
                   inputArea.emplace_back(new QFrame(&inputsArea));
                   setupInputArea(inputArea.back());
                   addDelButtons.back()->show();
                   fieldsCount++; 
               } else {
                   inputsArea.setGeometry(0,0,480, (fieldsCount+1)*40 + (fieldsCount+1)*5);
                   addDelButtons.back()->setPixmap(minusPixmap);
                   addDelButtons.emplace_back(new Buttons(this, &inputsArea));
                   addDelButtons.back()->setGeometry(0, (fieldsCount*40 + fieldsCount*5) ,40,40);
                   addDelButtons.back()->setPixmap(plusPixmap);
                   inputArea.emplace_back(new QFrame(&inputsArea));
                   setupInputArea(inputArea.back());
                   addDelButtons.back()->show();
                   fieldsCount++;
                 }
            } else {  //    "MINUS" BUTTON CLICKED
                  for (int i = 0; i < static_cast<int>(addDelButtons.size()); i++) {
                      if (addDelButtons.at(i) == ptr) {
                          delete ptr;
                          addDelButtons.erase(addDelButtons.begin() + i);

                          if (i < static_cast<int>(funPtrs.size())) {
                              graphLrPtr->delFunction(funPtrs.at(i));
                              funPtrs.erase(funPtrs.begin() + i);
                          }

                          delete inputArea.at(i);
                          inputArea.erase(inputArea.begin() + i);
                          --fieldsCount;
                          inputsArea.setGeometry(0, 0, 480, (fieldsCount) * 40 + (fieldsCount) * 5);
                          break;
                      }
                  }
                  int size = static_cast<int>(addDelButtons.size());
                  for (int j = 0; j < size; j++) {
                      addDelButtons.at(j)->setGeometry(0, j*40 + j*5, 40, 40);

                      if (j < size - 1)
                          inputArea.at(j)->setGeometry(45, j*40 + j*5, 435, 40);

                      addDelButtons.at(j)->show();
                  }
                }
        break;
       }
   }
   update();
}

void MainWin::resizeEvent(QResizeEvent* evt) {
    optionPanel.setGeometry(mainWinPtr->width()/2 - 250, mainWinPtr->height()-250, 500, 250);
    if (!optionPanel.isVisible() && !firstResize) optionPanelButton.setGeometry(mainWinPtr->width()/2-40, mainWinPtr->height()-28, 80, 30);
    else optionPanelButton.setGeometry(mainWinPtr->width()/2-40, mainWinPtr->height()-278, 80, 30);
    if (!firstResize) graphLrPtr->updateRArrowPos(evt);
    firstResize = false;

}

void MainWin::onBnClick() {
    if (optionPanel.isVisible()) {
        optionPanel.setVisible(false);
        optionPanelButton.setIcon(QIcon(":/resources/arrowUp.png"));
        optionPanelButton.setGeometry(mainWinPtr->width()/2-40, mainWinPtr->height()-28,80,30);
    }
    else {
        optionPanel.setVisible(true);
        optionPanelButton.setIcon(QIcon(":/resources/arrowDown.png"));
        optionPanelButton.setGeometry(mainWinPtr->width()/2-40, mainWinPtr->height()-278,80,30);
    }
}

void MainWin::updateSingleFunction(QFrame* frame)
{
    int index = std::distance(inputArea.begin(), std::find(inputArea.begin(), inputArea.end(), frame));
    if (index < 0) return;

    QObjectList lista = frame->children();

    QLineEdit* input = qobject_cast<QLineEdit* >(lista[0]);
    QLineEdit* d1    = qobject_cast<QLineEdit* >(lista[1]);
    QLineEdit* d2    = qobject_cast<QLineEdit* >(lista[2]);
    QComboBox* colors= qobject_cast<QComboBox* >(lista[3]);

    std::string formula = input->text().toStdString();
    int first = d1->text().toInt();
    int last  = d2->text().toInt();

    formula.erase(std::remove_if(formula.begin(), formula.end(),
                                 std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())), formula.end());

    QColor color;
    switch (colors->currentIndex()) {
        case 0: color.setRgb(0,0,0,255); break;
        case 1: color.setRgb(255,0,0,255); break;
        case 2: color.setRgb(0,0,255,255); break;
        case 3: color.setRgb(255,255,0,255); break;
        case 4: color.setRgb(0,255,0,255); break;
        case 5: color.setRgb(128,0,128,255); break;
        default: color.setRgb(0,0,0,255); break;
    }

    bool valid = !formula.empty() && (first < last);

    try {
        if (valid) {
            if (index < static_cast<int>(funPtrs.size())) {
                graphLrPtr->delFunction(funPtrs[index]);
                funPtrs[index] = graphLrPtr->addFunction(formula, first, last, color);
            } else {
                funPtrs.emplace_back(graphLrPtr->addFunction(formula, first, last, color));
            }
        } else {
            if (index < static_cast<int>(funPtrs.size())) {
                graphLrPtr->delFunction(funPtrs[index]);
                funPtrs.erase(funPtrs.begin() + index);
            }
        }
    } catch (...) {
        if (index < static_cast<int>(funPtrs.size())) {
            graphLrPtr->delFunction(funPtrs[index]);
            funPtrs.erase(funPtrs.begin() + index);
        }
    }
}

/*
void MainWin::onEditFinished() {
    int iter = 0;
    QObjectList functionDetails;
    if (funPtrs.size() > 0) {
        for (QFrame* ptr : inputArea) {
            functionDetails = ptr->children();
            setFunctionValues(qobject_cast<QLineEdit* >(functionDetails[0]),
                              qobject_cast<QLineEdit* >(functionDetails[1]),
                              qobject_cast<QLineEdit* >(functionDetails[2]),
                              qobject_cast<QComboBox* >(functionDetails[3]));

                if (firstDValue < lastDValue) {
                    if (funPtrs.size() == inputArea.size()) {
                        graphLrPtr->delFunction(funPtrs.at(iter));
                        Function* ptr = graphLrPtr->addFunction(formula, firstDValue, lastDValue, color);
                        funPtrs.at(iter) = ptr;
                    } else {
                        Function* ptr = graphLrPtr->addFunction(formula, firstDValue, lastDValue, color);
                        funPtrs.emplace_back(ptr);
                    }
                }
            iter++;
         }
    } else {
        functionDetails = inputArea.at(0)->children();
        setFunctionValues(qobject_cast<QLineEdit* >(functionDetails[0]),
                          qobject_cast<QLineEdit* >(functionDetails[1]),
                          qobject_cast<QLineEdit* >(functionDetails[2]),
                          qobject_cast<QComboBox* >(functionDetails[3]));
            if (firstDValue < lastDValue) {
                Function* ptr = graphLrPtr->addFunction(formula, firstDValue, lastDValue, color);
                funPtrs.emplace_back(ptr);
            }
        }
}
*/

void MainWin::setFunctionValues(QLineEdit* Input, QLineEdit* D1, QLineEdit* D2, QComboBox* Colors) {
    if(Input->hasFocus() || D1->hasFocus() || D2->hasFocus()) {
        formula = (Input->text()).toStdString();
        firstDValue = (D1->text()).toInt();
        lastDValue = (D2->text()).toInt();
        formula.erase(std::remove_if(formula.begin(), formula.end(), std::bind(std::isspace <char> ,
                    std::placeholders::_1,
                    std::locale::classic())),
                    formula.end());
        switch (Colors->currentIndex()) {
        case 0:
            color.setRgb(0,0,0,255);
            break;
        case 1:
            color.setRgb(255,0,0,255);
            break;
        case 2:
            color.setRgb(0,0,255,255);
            break;
        case 3:
            color.setRgb(255,255,0,255);
            break;
        case 4:
            color.setRgb(0,255,0,255);
            break;
        case 5:
            color.setRgb(128,0,128,255);
            break;
        default:
            color.setRgb(0,0,0,255);
            break;
        }
    }
}
