//
// Created by 23576 on 2023/4/6.
//

#ifndef WORDGAMEGUI_MAINWINDOW_H
#define WORDGAMEGUI_MAINWINDOW_H

#include "QWidget"
#include "QtWidgets"
#include "GameMap.h"
#include <iostream>
#include "stack"

static int selectParent;
static int selectIndex;
static std::stack<std::pair<int, int> > chooseList;
static bool updating = false;
static int progress = 0;
static bool difficult = false;
static bool trick = false;

class MetaButton : public QLabel {
public:
    explicit MetaButton(QWidget *parent = nullptr) : QLabel(parent) {}

    void setNumber(int n) { number = n; }

    void setParent(int p) { parent = p; }

    int getParent() { return parent; }

    int getIndex() { return number; }


protected:


    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event);
        QPainter painter(this);
        painter.setPen(QColor("#666666"));
        painter.setFont(QFont("Arial", 10));
        painter.drawText(rect().adjusted(0, 3, -5, 0), Qt::AlignRight | Qt::AlignTop, QString::number(number));
    }

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    int number = 0;
    int parent = -1;
};

class BarButton : public QPushButton {
public:
    explicit BarButton(QWidget *parent = nullptr, int wi = -1) : QPushButton(parent), index(wi) {}

    int index = -1;

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;

    void newGameBtn();

    void modeChangeBtn();

    void tipBtn();

    void revokeBtn();

    void trickBtn();

    void helpBtn();
};


class MainWindow : public QWidget {
Q_OBJECT
public:
    GameMap *game = nullptr;
    QTimer *timer;

    void syncToMap();

    void clearWord();

    void clearWord(int p);

    MainWindow();

    void showTip();

    void hideTip();

    void enableTrick();

    void disableTrick(int p);

    void loadNewGame();

    void resetAnimation() {
        while (!chooseList.empty()) {
            loadAnimationShrink(chooseList.top().first, chooseList.top().second);
            QString style;
            if (difficult)
                style = "QLabel {"
                        "border-style: solid;" //设置边框样式
                        "border-width: 3px;" //设置边框宽度
                        "border-color: #555555;" //设置边框颜色
                        "background-color: #FFFFFF;" //设置背景色
                        "border-radius: 10px;" //设置圆角
                        "}";
            else
                style = "QLabel {"
                        "border-style: solid;" //设置边框样式
                        "border-width: 3px;" //设置边框宽度
                        "border-color: " +
                        color[buttons[chooseList.top().first][chooseList.top().second]->getParent()] +
                        ";" //设置边框颜色
                        "background-color: #FFFFFF;" //设置背景色
                        "border-radius: 10px;" //设置圆角
                        "}";
            buttons[chooseList.top().first][chooseList.top().second]->setStyleSheet(style);
            chooseList.pop();
        }
    }

protected:
    const QString color[16] = {"#2C503E", "#445F7A", "#AA5555", "#95A5A6",
                               "#13D770", "#27AE60", "#2980B9", "#8E44AD",
                               "#502C3E", "#D35400", "#C0392B", "#F1C40F",
                               "#E67E22", "#E74C3C", "#F39C12", "#1ABC9C"};
    MetaButton *buttons[8][8];
    QPropertyAnimation *animationButton[8][8];
    QPropertyAnimation *animationFall[8][8];
    QRect initButtonRect[8][8];
    BarButton *controlButton[6];
    QLabel *timeLabel;
    QTime time;

    bool eventFilter(QObject *object, QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;


    void loadAnimationExpand(int i, int j);

    void loadAnimationShrink(int i, int j);

    void updateTimer();

    void paintEvent(QPaintEvent *event) override;

};


#endif //WORDGAMEGUI_MAINWINDOW_H
