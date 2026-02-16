#include "mainwin.h"
//
#include <locale>
//
#include <qlabel.h>
#include <QComboBox>
#include <QString>
#include <QTimer>

namespace UiConst {
    const int MAIN_WIN_WIDTH  = 1002;
    const int MAIN_WIN_HEIGHT = 600;
    const int ROW_HEIGHT = 40;
    const int ROW_SPACING = 5;
    const int BUTTON_SIZE = 40;
    const int LEFT_MARGIN = 45;
    const int PANEL_WIDTH = 500;
    const int PANEL_HEIGHT = 250;
    const int SCROLL_MARGIN = 10;
    const int EDIT_TEXT_DELAY = 400;
    const int EDIT_COLOR_DELAY = 200;
}

MainWin::MainWin(QWidget* parent): QMainWindow(parent) {
    setupMainWin(this);
    addDelButtons.reserve(10);
    inputArea.reserve(10);
    funPtrs.reserve(10);
}

MainWin::~MainWin() {
    delete validator;
}

void MainWin::setupMainWin(QMainWindow* mainWindow) {

    // MAIN WINDOW
    mainWin = mainWindow;
    mainWin->setObjectName("mainwindow");
    mainWin->setWindowTitle("GraphIt");
    mainWin->setMinimumSize(600,400);
    mainWin->resize(UiConst::MAIN_WIN_WIDTH, UiConst::MAIN_WIN_HEIGHT);

    // CENTRAL WIDGET
    centralWidget = new QWidget(mainWin);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setGeometry(0,0, UiConst::MAIN_WIN_WIDTH, UiConst::MAIN_WIN_HEIGHT);

    // GRAPH LAYER
    graphLayer = new GraphLayer(centralWidget);

    optionPanelButton.setParent(centralWidget);
    optionPanelButton.setIcon(QIcon(":/resources/arrowDown.png"));
    connect(&optionPanelButton, &QPushButton::clicked, this, &MainWin::onBnClick);

    optionPanel.setParent(centralWidget);
    optionPanel.setGeometry(251,465, UiConst::PANEL_WIDTH, UiConst::PANEL_HEIGHT);
    optionPanel.setFrameShape(QFrame::StyledPanel);
    optionPanel.setFrameShadow(QFrame::Sunken);
    optionPanel.setAutoFillBackground(true);
    optionPanel.setVisible(true);

    // SCROLL'S AREA
    scrollArea.setParent(&optionPanel);
    scrollArea.setGeometry(UiConst::SCROLL_MARGIN,UiConst::SCROLL_MARGIN,480,230);
    scrollArea.setWidget(&inputsArea);
    inputsArea.setParent(&scrollArea);
    scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    scrollArea.setWidgetResizable(false);
    scrollArea.setFrameShape(QFrame::NoFrame);
    inputsArea.resize(480, UiConst::ROW_HEIGHT);

	// OPTION PANEL
    plusPixmap = QPixmap(":/resources/plusCircle_40.png");
    minusPixmap = QPixmap(":/resources/minusRounded_40.png");

	// BUTTONS FOR ADDING AND DELETING INPUT FIELDS
    addDelButtons.emplace_back(new Buttons(this, &inputsArea));
    addDelButtons.front()->setGeometry(0,0,UiConst::BUTTON_SIZE,UiConst::BUTTON_SIZE);
    addDelButtons.front()->setPixmap(plusPixmap);

    validator = new QRegularExpressionValidator(rexpr,this);

    rowCount = 1;
    setCentralWidget(centralWidget);
    editTimer = new QTimer(this);
    editTimer->setSingleShot(true);

    connect(editTimer, &QTimer::timeout, this, [this]() {
        if (pendingFrame)
            updateSingleFunction(pendingFrame);
    });
}

void MainWin::setupInputArea(QFrame* frame) {
    frame->setGeometry(UiConst::LEFT_MARGIN, (rowCount-1)*UiConst::ROW_HEIGHT + (rowCount-1)*UiConst::ROW_SPACING, 437, UiConst::ROW_HEIGHT);
    frame->setFrameShape(QFrame::NoFrame);
    QLineEdit* input = new QLineEdit(frame);
    QLineEdit* d1 = new QLineEdit(frame);
    QLineEdit* d2 = new QLineEdit(frame);
    QComboBox* colors = new QComboBox(frame);
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

    connect(input, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(UiConst::EDIT_TEXT_DELAY);
    });

    connect(d1, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(UiConst::EDIT_TEXT_DELAY);
    });

    connect(d2, &QLineEdit::textChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(UiConst::EDIT_TEXT_DELAY);
    });

    connect(colors, &QComboBox::currentIndexChanged, this, [this, frame]() {
        pendingFrame = frame;
        editTimer->start(UiConst::EDIT_COLOR_DELAY);
    });
    frame->show();
}

void MainWin::addDelBnClicked(Buttons* btnClicked) {
   for (Buttons* btn : addDelButtons) {
       if ( btn == btnClicked ) {
           if ( !btnClicked->changeState() ) { // "PLUS" BUTTON CLICKED
               if (rowCount == 1) {
                   addDelButtons.back()->setPixmap(minusPixmap);
                   addDelButtons.front()->show();
                   inputsArea.setGeometry(0,0,480, (rowCount+1)*UiConst::ROW_HEIGHT + (rowCount+1)*UiConst::ROW_SPACING);
                   addDelButtons.emplace_back(new Buttons(this, &inputsArea));
                   addDelButtons.back()->setGeometry(0, (rowCount*UiConst::ROW_HEIGHT + rowCount*UiConst::ROW_SPACING), UiConst::BUTTON_SIZE, UiConst::BUTTON_SIZE);
                   addDelButtons.back()->setPixmap(plusPixmap);
                   inputArea.emplace_back(new QFrame(&inputsArea));
                   setupInputArea(inputArea.back());
                   addDelButtons.back()->show();
                   rowCount++;
               } else {
                   inputsArea.setGeometry(0,0,480, (rowCount+1)*UiConst::ROW_HEIGHT + (rowCount+1)*UiConst::ROW_SPACING);
                   addDelButtons.back()->setPixmap(minusPixmap);
                   addDelButtons.emplace_back(new Buttons(this, &inputsArea));
                   addDelButtons.back()->setGeometry(0, (rowCount*UiConst::ROW_HEIGHT + rowCount*UiConst::ROW_SPACING), UiConst::BUTTON_SIZE, UiConst::BUTTON_SIZE);
                   addDelButtons.back()->setPixmap(plusPixmap);
                   inputArea.emplace_back(new QFrame(&inputsArea));
                   setupInputArea(inputArea.back());
                   addDelButtons.back()->show();
                   rowCount++;
                 }
            } else {  //    "MINUS" BUTTON CLICKED
                  for (int i = 0; i < static_cast<int>(addDelButtons.size()); i++) {
                      if (addDelButtons.at(i) == btnClicked) {
                          delete btnClicked;
                          addDelButtons.erase(addDelButtons.begin() + i);

                          if (i < static_cast<int>(funPtrs.size())) {
                              graphLayer->delFunction(funPtrs.at(i));
                              funPtrs.erase(funPtrs.begin() + i);
                          }

                          delete inputArea.at(i);
                          inputArea.erase(inputArea.begin() + i);
                          --rowCount;
                          inputsArea.setGeometry(0, 0, 480, (rowCount) * UiConst::ROW_HEIGHT + (rowCount) * UiConst::ROW_SPACING);
                          break;
                      }
                  }
                  int size = static_cast<int>(addDelButtons.size());
                  for (int j = 0; j < size; j++) {
                      addDelButtons.at(j)->setGeometry(0, j*UiConst::ROW_HEIGHT + j*UiConst::ROW_SPACING, UiConst::BUTTON_SIZE, UiConst::BUTTON_SIZE);

                      if (j < size - 1)
                          inputArea.at(j)->setGeometry(UiConst::LEFT_MARGIN, j*UiConst::ROW_HEIGHT + j*UiConst::ROW_SPACING, 435, UiConst::ROW_HEIGHT);

                      addDelButtons.at(j)->show();
                  }
                }
        break;
       }
   }
   update();
}

void MainWin::resizeEvent(QResizeEvent* event) {
    optionPanel.setGeometry(mainWin->width()/2 - 250, mainWin->height()-250, UiConst::PANEL_WIDTH, UiConst::PANEL_HEIGHT);
    if (!optionPanel.isVisible() && !firstResize) optionPanelButton.setGeometry(mainWin->width()/2 -UiConst::BUTTON_SIZE, mainWin->height()-28, 80, 30);
    else optionPanelButton.setGeometry(mainWin->width()/2 -UiConst::BUTTON_SIZE, mainWin->height() -278, 80, 30);
    if (!firstResize) graphLayer->updateRArrowPos(event);
    firstResize = false;

}

void MainWin::onBnClick() {
    if (optionPanel.isVisible()) {
        optionPanel.setVisible(false);
        optionPanelButton.setIcon(QIcon(":/resources/arrowUp.png"));
        optionPanelButton.setGeometry(mainWin->width()/2 -UiConst::BUTTON_SIZE, mainWin->height()-28,80,30);
    }
    else {
        optionPanel.setVisible(true);
        optionPanelButton.setIcon(QIcon(":/resources/arrowDown.png"));
        optionPanelButton.setGeometry(mainWin->width()/2 -UiConst::BUTTON_SIZE, mainWin->height()-278,80,30);
    }
}

// Rebuilds a single function based on input row
// Each input panel row owns exactly one Function instance
// Any parameter change destroys and recreates the plot (no incremental updates)
void MainWin::updateSingleFunction(QFrame* frame)
{
    int index = std::distance(inputArea.begin(), std::find(inputArea.begin(), inputArea.end(), frame));
    if (index < 0) return;

    QObjectList functionDetails = frame->children();

    QLineEdit* input = qobject_cast<QLineEdit* >(functionDetails[0]);
    QLineEdit* d1    = qobject_cast<QLineEdit* >(functionDetails[1]);
    QLineEdit* d2    = qobject_cast<QLineEdit* >(functionDetails[2]);
    QComboBox* colors= qobject_cast<QComboBox* >(functionDetails[3]);

    std::string formula = input->text().toStdString();
    int first = d1->text().toInt();
    int last = d2->text().toInt();

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
                graphLayer->delFunction(funPtrs[index]);
                funPtrs[index] = graphLayer->addFunction(formula, first, last, color);
            } else {
                funPtrs.emplace_back(graphLayer->addFunction(formula, first, last, color));
            }
        } else {
            if (index < static_cast<int>(funPtrs.size())) {
                graphLayer->delFunction(funPtrs[index]);
                funPtrs.erase(funPtrs.begin() + index);
            }
        }
    } catch (...) {
        if (index < static_cast<int>(funPtrs.size())) {
            graphLayer->delFunction(funPtrs[index]);
            funPtrs.erase(funPtrs.begin() + index);
        }
    }
}
