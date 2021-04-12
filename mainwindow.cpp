#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = IDLE;
    isInTrial = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_visual_test(int level, bool isPractice)
{
    this->show();
    status = VISUAL;
    difficulty = level;
    if (isPractice) {
        testCode = 0;
    } else {
        testCode = level + 1;
    }
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::show_auditory_test(int level, bool isPractice)
{
    this->show();
    status = AUDITORY;
    difficulty = level;
    if (isPractice) {
        testCode = 0;
    } else {
        testCode = level + 4;
    }
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::show_tactile_test(int level, bool isPractice)
{
    this->show();
    status = TACTILE;
    difficulty = level;
    if (isPractice) {
        testCode = 0;
    } else {
        testCode = level + 7;
    }
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    int w = this->size().width(), h = this->size().height();
    pix = QPixmap(w, h);
    pix.fill(Qt::black);
    QPainter painter(&pix);
    QPen pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawLine(w/2, 0, w/2, h);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pix);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
//    if (!isInTrial && (ev->key() == Qt::Key_Left || ev->key() == Qt::Key_Right)) {
    if (!isInTrial && (ev->key() == Qt::Key_Right)) {
        isInTrial = true;
        if (status == VISUAL) {
            showVisualCue();
            QTime t;
            t.start();
            while(t.elapsed() < 300) {
                QCoreApplication::processEvents();
            }
            refresh();
            QPainter painter(&pix);
            drawMiddleLine(painter);
            painter.end();
            while(t.elapsed() < 1000) {
                QCoreApplication::processEvents();
            }
            showTarget(difficulty);
        }
        else if (status == AUDITORY) {
            playAuditoryCue();
            QTime t;
            t.start();
            while(t.elapsed() < 1000) {
                QCoreApplication::processEvents();
            }
            showTarget(difficulty);
        }
        else if (status == TACTILE) {
            QTime t;
            t.start();
            while(t.elapsed() < 1000) {
                QCoreApplication::processEvents();
            }
            showTarget(difficulty);
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if (isInTrial && ev->button() == Qt::LeftButton) {  // only handle event when in trial
        int rt = timer.elapsed();
        ++trialCnt;
        if (isOnTarget(ev->pos())) {
            qDebug() << "correct";
            ++right;
        }
        else {
            qDebug() << "wrong";
            ++wrong;
        }
        reactionTime.push_back(rt);
        qDebug() << rt << "ms";
        refresh();
        QPainter painter(&pix);
        drawMiddleLine(painter);
        isInTrial = false;

        int totalNum = testCode == 0 ? PRACTICE_NUM_TRIAL : NUM_TRIAL;
        if (trialCnt == totalNum) {  // end test
            logToFile();
            this->hide();
            status = IDLE;
            emit end_test();
        }
    }
}

void MainWindow::showTarget(int difficulty)
{
    int key = kSequence[testCode][trialCnt];    // L: 0, R: 1
    int w = this->size().width(), h = this->size().height(), halfW = w / 2;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randX, randY, distractX, distractY;
    if (difficulty == 0) {
         randX = qrand() % (halfW - EASY_TARGET_SIZE), randY = qrand() % (h - EASY_TARGET_SIZE);
    }
    else {
        randX = qrand() % (halfW - TARGET_SIZE), randY = qrand() % (h - TARGET_SIZE);
    }

    if (key == 1) {    // target is on the right part
        randX += halfW;
    }

    if (difficulty == 2) {
        distractX = qrand() % (halfW - TARGET_SIZE), distractY = qrand() % (h - TARGET_SIZE);
        if (key == 0) {    // target is on the left part, so distractive target is on the right part
            distractX += halfW;
        }
    }
    qDebug() << randX << ", " << randY << endl;
    currentTargetPos = QPoint(randX, randY);
    refresh();
    QPainter painter(&pix);
    drawMiddleLine(painter);
    if (difficulty == 0) {
        painter.fillRect(randX, randY, EASY_TARGET_SIZE, EASY_TARGET_SIZE, Qt::blue);
    }
    else {
        if (difficulty == 2) {
            painter.fillRect(distractX, distractY, TARGET_SIZE, TARGET_SIZE, Qt::white);
        }
        painter.fillRect(randX, randY, TARGET_SIZE, TARGET_SIZE, Qt::blue);
    }
    update();
    timer.start();

}

void MainWindow::showVisualCue()
{
    int key = kSequence[testCode][trialCnt];
    int w = this->size().width(), h = this->size().height();
    int posX = w / 4, posY = h / 2;
    if (key == 1) {
        posX += w / 2;
    }
    refresh();
    QPainter painter(&pix);
    drawMiddleLine(painter);
    painter.fillRect(posX, posY, TARGET_SIZE, TARGET_SIZE, Qt::red);
    update();
}

void MainWindow::playAuditoryCue()
{
    int key = kSequence[testCode][trialCnt];
    QMediaPlayer *player = new QMediaPlayer();
    QString audio = key == 0 ? "Left.mp3" : "Right.mp3";
    player->setMedia(QUrl("qrc:/audio/res/audios/" + audio));
    player->play();
}

void MainWindow::logToFile()
{
    const std::unordered_map<Status, QString> kMap = {
        { VISUAL, "visual" },
        { AUDITORY, "auditory" },
        { TACTILE, "tactile" }
    };
    auto time = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
    QString fileName = kMap.at(status) + "_" + "level_" + QString::number(difficulty) + "_" + time;
    if (testCode == 0) {
        fileName += "_practice.txt";
    } else {
        fileName += ".txt";
    }
    QFile file(fileName);

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for (double rt : reactionTime) {
            stream << rt << endl;
        }
        stream << "right: " << right << endl;
        stream << "wrong: " << wrong << endl;
        file.close();
    }
}

bool MainWindow::isOnTarget(const QPoint &pos)
{
    int disX = pos.x() - currentTargetPos.x(), distY = pos.y() - currentTargetPos.y();
    if (difficulty == 0) {
        return disX >= 0 && disX <= EASY_TARGET_SIZE && distY >= 0 && distY <= EASY_TARGET_SIZE;
    }
    return disX >= 0 && disX <= TARGET_SIZE && distY >= 0 && distY <= TARGET_SIZE;
}

void MainWindow::drawMiddleLine(QPainter &painter)
{
    int w = this->size().width(), h = this->size().height();
    QPen pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawLine(w/2, 0, w/2, h);
    update();
}

void MainWindow::refresh()
{
    int w = this->size().width(), h = this->size().height();
    pix = QPixmap(w, h);
    pix.fill(Qt::black);
}



