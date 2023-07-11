//
// Created by 23576 on 2023/4/6.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    setMouseTracking(true);
    setFixedSize(1015, 740);
    setStyleSheet("background-color: white;");
    //QGridLayout *gridLayout = new QGridLayout(this);

    QString controlButtonText[6] = {"新游戏", "简单模式", "提示", "反悔", "道具", "帮助"};
    for (int i = 0; i < 6; i++) {
        controlButton[i] = new BarButton(this, i);
        controlButton[i]->setGeometry(800, 300 + 70 * i, 160, 60);
        controlButton[i]->setText(controlButtonText[i]);
        controlButton[i]->setFont(QFont("宋体", 15, QFont::Bold));
        QString style = "QPushButton {"
                        "border-style: solid;" //设置边框样式
                        "border-width: 2px;" //设置边框宽度
                        "border-color: #888888;" //设置边框颜色
                        "background-color: #FFFFFF;" //设置背景色
                        "border-radius: 15px;" //设置圆角
                        "}"
                        "QPushButton:hover{"
                        "background-color: #EEEEEE"
                        "}";
        controlButton[i]->setStyleSheet(style); //设置按钮样式表
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            buttons[i][j] = nullptr;
    timeLabel = new QLabel(this);
    timeLabel->setText("00:00");
    timeLabel->setFont(QFont("Arial", 20, QFont::Bold));
    timeLabel->setGeometry(800, 230, 160, 50);
    timeLabel->setAlignment(Qt::AlignCenter);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start();

    loadNewGame();


}

void MainWindow::loadNewGame() {
    if(game!=nullptr)delete game;
    game = new GameMap();
    game->generate();
    progress = 0;
    syncToMap();
    time = QTime(0, 0, 0);
    timer->start();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (updating) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (animationFall[i][j] != nullptr && animationFall[i][j]->state() != QAbstractAnimation::Stopped)
                    return QWidget::eventFilter(object, event);
            }
        }
        updating = false;
        return QWidget::eventFilter(object, event);
    }
    if (trick) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (object == buttons[i][j]) {
                    int p = buttons[i][j]->getParent();
                    if (event->type() == QEvent::Enter) { // 鼠标进入事件
                        for (int k = 0; k < 8; k++) {
                            for (int l = 0; l < 8; l++) {
                                if (buttons[k][l] != nullptr && buttons[k][l]->getParent() == p)
                                    buttons[k][l]->setFont(QFont("楷体", 35, QFont::Bold));
                            }
                        }
                    } else if (event->type() == QEvent::Leave) {
                        for (int k = 0; k < 8; k++) {
                            for (int l = 0; l < 8; l++) {
                                if (buttons[k][l] != nullptr) buttons[k][l]->setFont(QFont("宋体", 25, QFont::Bold));
                            }
                        }
                    }
                }
            }
        }
        return QWidget::eventFilter(object, event);
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (object == buttons[i][j]) {
                if (event->type() == QEvent::Enter) { // 鼠标进入事件
                    if (animationButton[i][j] == nullptr ||
                        animationButton[i][j]->state() == QAbstractAnimation::Stopped ||
                        animationButton[i][j]->duration() == 50) {
                        loadAnimationExpand(i, j);
                    }
                } else if (event->type() == QEvent::Leave) {
                    if (animationButton[i][j] == nullptr ||
                        animationButton[i][j]->state() == QAbstractAnimation::Stopped ||
                        animationButton[i][j]->duration() == 100) {
                        loadAnimationShrink(i, j);
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(object, event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (selectParent == -1) {
        return;
    }
    QPoint pos = mapFromGlobal(QCursor::pos());
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (buttons[i][j] == nullptr)continue;
            if (buttons[i][j]->geometry().contains(pos)) {
                if (buttons[i][j]->getParent() != selectParent) {
                    selectParent = -1;
                    selectIndex = 0;
                    if (selectIndex == 4) {
                        clearWord();
                    } else {
                        resetAnimation();
                    }
                    return;
                }
                if (buttons[i][j]->getIndex() == selectIndex + 1) {
                    if (selectIndex != 0 && abs(chooseList.top().first - i) + abs(chooseList.top().second - j) != 1) {
                        selectParent = -1;
                        selectIndex = 0;
                        resetAnimation();
                        return;
                    }
                    selectIndex++;
                    loadAnimationExpand(i, j);
                    chooseList.push(std::make_pair(i, j));
                    QString style;
                    if (difficult)
                        style = "QLabel {"
                                "border-style: solid;" //设置边框样式
                                "border-width: 3px;" //设置边框宽度
                                "border-color: #555555;" //设置边框颜色
                                "background-color: #DDDDDD;" //设置背景色
                                "border-radius: 10px;" //设置圆角
                                "}";
                    else
                        style = "QLabel {"
                                "border-style: solid;" //设置边框样式
                                "border-width: 3px;" //设置边框宽度
                                "border-color: " + color[buttons[i][j]->getParent()] +
                                ";" //设置边框颜色
                                "background-color: #DDDDDD;" //设置背景色
                                "border-radius: 10px;" //设置圆角
                                "}";
                    buttons[i][j]->setStyleSheet(style);
                    std::cout << i << " " << j << "\n";
                }
            }
        }
    }
}


void MainWindow::loadAnimationExpand(int i, int j) {
    if (animationButton[i][j] == nullptr)
        initButtonRect[i][j] = buttons[i][j]->geometry();
    animationButton[i][j] = new QPropertyAnimation(buttons[i][j], "geometry");
    animationButton[i][j]->setDuration(100);
    animationButton[i][j]->setStartValue(buttons[i][j]->geometry());
    animationButton[i][j]->setEndValue(initButtonRect[i][j].adjusted(-3, -3, 3, 3));
    animationButton[i][j]->start();//QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::loadAnimationShrink(int i, int j) {
    if (animationButton[i][j] == nullptr)
        initButtonRect[i][j] = buttons[i][j]->geometry();
    animationButton[i][j] = new QPropertyAnimation(buttons[i][j], "geometry");
    animationButton[i][j]->setDuration(50);
    animationButton[i][j]->setStartValue(buttons[i][j]->geometry());
    animationButton[i][j]->setEndValue(initButtonRect[i][j]);
    animationButton[i][j]->start();//QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::clearWord(int p) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (buttons[i][j] != nullptr && buttons[i][j]->getParent() == p)chooseList.push(std::make_pair(i, j));
        }
    }
    clearWord();
}

void MainWindow::clearWord() {
    if (game->rmMeta(buttons[chooseList.top().first][chooseList.top().second]->getParent())) {
        int beginEmpty[8] = {8, 8, 8, 8, 8, 8, 8, 8}, fallHeight[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        while (!chooseList.empty()) {
            int i = chooseList.top().first, j = chooseList.top().second;
            chooseList.pop();
            fallHeight[j]++;
            beginEmpty[j] = fmin(beginEmpty[j], 7 - i);
        }
        for (int i = 0; i < 8; i++) { // i is column
            if (fallHeight[i] == 0)continue;
            for (int j = beginEmpty[i]; j < 8; j++) { // j is height = 7 - row
                if (j < beginEmpty[i] + fallHeight[i])delete buttons[7 - j][i];
                if (7 - j - fallHeight[i] >= 0 && buttons[7 - j - fallHeight[i]][i] != nullptr) {
                    buttons[7 - j][i] = buttons[7 - j - fallHeight[i]][i];
                    animationButton[7 - j][i] = nullptr;
                    animationFall[7 - j][i] = new QPropertyAnimation(buttons[7 - j][i], "pos");
                    animationFall[7 - j][i]->setStartValue(buttons[7 - j][i]->pos());
                    animationFall[7 - j][i]->setEndValue(buttons[7 - j][i]->pos() + QPoint(0, 90 * fallHeight[i]));
                    animationFall[7 - j][i]->setDuration(100);
                    animationFall[7 - j][i]->setEasingCurve(QEasingCurve::OutQuad);
                    animationFall[7 - j][i]->start();
                } else buttons[7 - j][i] = nullptr;
            }
        }
        updating = true;
        progress++;
        if (progress == 16) timer->stop();
        this->repaint();
    }
}

void MainWindow::syncToMap() {
    // Sync data to button
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            animationButton[i][j] = nullptr;
            animationFall[i][j] = nullptr;
            //i means row, j means column
            //j == map.x, i + map.y = 7
            int mx = j, my = 7 - i;
            if (buttons[i][j] != nullptr)delete buttons[i][j];
            if (my >= game->getHeight(mx)) {
                continue;
            }
            buttons[i][j] = new MetaButton(this);
            buttons[i][j]->setMinimumSize(QSize(80, 80));
            buttons[i][j]->setMaximumSize(QSize(86, 86));
            buttons[i][j]->setGeometry(j * 90 + 10, i * 90 + 10, 80, 80);
            buttons[i][j]->setText(QString::number(i * 8 + j));
            buttons[i][j]->setNumber(i * 8 + j);
            buttons[i][j]->setFont(QFont("宋体", 25, QFont::Bold));
            buttons[i][j]->setAlignment(Qt::AlignCenter);
            //gridLayout->addWidget(buttons[i][j], i, j);
            buttons[i][j]->installEventFilter(this);
            buttons[i][j]->setText(game->getBlock(mx, my).getCharacter());
            buttons[i][j]->setNumber(game->getBlock(mx, my).getIndex());
            buttons[i][j]->setParent(game->getBlock(mx, my).getParent());
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
                        "border-color: " + color[buttons[i][j]->getParent()] +
                        ";" //设置边框颜色
                        "background-color: #FFFFFF;" //设置背景色
                        "border-radius: 10px;" //设置圆角
                        "}";
            buttons[i][j]->setStyleSheet(style);
            buttons[i][j]->show();
        }
    }
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen(QColor("#DDDDDD"));
    pen.setWidth(3);
    pen.setStyle(Qt::DashDotDotLine);
    painter.setPen(pen);
    painter.drawLine(745, 0, 745, 740);

    painter.setRenderHint(QPainter::Antialiasing, true);
    QRect circleRect(800, 50, 160, 160);
    painter.drawEllipse(circleRect);
    painter.setPen(QPen(QBrush("#FF9999"), 10));
    int startAngle = 90 * 16;  // 圆弧起始角度
    int spanAngle = progress * 360;  // 圆弧跨度角度
    painter.drawArc(circleRect, startAngle, spanAngle);
    // 在圆环中央绘制进度百分比
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(circleRect, Qt::AlignCenter, QString::number(progress) + "/16");
    painter.setFont(QFont("Arial", 50, QFont::Bold));
    if (progress == 16) painter.drawText(QRect(200, 200, 340, 340), Qt::AlignCenter, QString("游戏完成！"));
}

void MainWindow::updateTimer() {
    time = time.addSecs(1);
    timeLabel->setText(time.toString("mm:ss"));
}

void MainWindow::showTip() {
    game->updateTip();
    for (int t = 0; t < game->tiplist.size(); t++) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (buttons[i][j] == nullptr)continue;
                if (buttons[i][j]->getParent() == game->tiplist[t])
                    buttons[i][j]->setFont(QFont("黑体", 25, QFont::Bold));
            }
        }
    }
}

void MainWindow::hideTip() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (buttons[i][j] == nullptr)continue;
            buttons[i][j]->setFont(QFont("宋体", 25, QFont::Bold));
        }
    }
}

void MainWindow::enableTrick() {
    trick = true;
    hideTip();
}

void MainWindow::disableTrick(int p) {
    trick = false;
    controlButton[4]->setText("道具");
    if (p != -1) {
        clearWord(p);
        syncToMap();
    }
}


void MetaButton::mouseReleaseEvent(QMouseEvent *event) {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    if (selectIndex == 4) {
        m->clearWord();
    } else {
        m->resetAnimation();
    }
    selectParent = -1;
    selectIndex = 0;
    //QLabel::mouseReleaseEvent(event);
}

void MetaButton::mousePressEvent(QMouseEvent *event) {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->hideTip();
    if (trick) {
        m->disableTrick(parent);
        return;
    }
    if (number != 1)return;
    selectIndex = 0;
    selectParent = parent;

}

void BarButton::mouseReleaseEvent(QMouseEvent *e) {
    switch (index) {
        case 0:
            newGameBtn();
            break;
        case 1:
            modeChangeBtn();
            break;
        case 2:
            tipBtn();
            break;
        case 3:
            revokeBtn();
            break;
        case 4:
            trickBtn();
            break;
        case 5:
            helpBtn();
            break;
    }
}

void BarButton::newGameBtn() {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->loadNewGame();
}

void BarButton::modeChangeBtn() {
    if (text() == "简单模式") {
        setText("困难模式");
        difficult = true;
    } else {
        setText("简单模式");
        difficult = false;
    }
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->syncToMap();
}

void BarButton::tipBtn() {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->showTip();
}

void BarButton::revokeBtn() {
    if (progress == 0)return;
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->game->revoke();
    progress--;
    m->syncToMap();
}

void BarButton::trickBtn() {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    if (text() == "道具") {
        m->enableTrick();
        setText("单击消除");
    } else {
        m->disableTrick(-1);
        setText("道具");
    }
}

void BarButton::helpBtn() {
    MainWindow *m = dynamic_cast<MainWindow *>(parentWidget());
    m->timer->stop();
    QMessageBox::information(nullptr, "帮助",
                             "1.按住拖动可以选中成语消除。只有当成语中各个字按顺序相邻时才能消除。\n"
                             "2.右上角的角标代表其是成语中的第几个字。简单模式下边框颜色相同的字属于同一个成语。\n"
                             "3.每局游戏必定有解，但错误的消除顺序可能导致进入死局。可以通过反悔重新寻找正确消除方式，也可以直接通过道具继续游戏。\n"
                             "4.侧栏功能介绍\n"
                             "·困难模式：不显示边框颜色，消除规则不变\n"
                             "·提示：将当前可以消除的字以黑体标注\n"
                             "·反悔：回退到上一步的动作，没有次数限制\n"
                             "·道具：单击一个格子，可以无视相邻的要求，直接消除其对应的整个成语");
    m->timer->start();
}