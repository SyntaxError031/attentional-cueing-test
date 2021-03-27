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

void MainWindow::show_practice()
{
    this->show();
    status = PRACTICE;
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::show_visual_test()
{
    this->show();
    status = VISUAL;
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::show_auditory_test()
{
    this->show();
    status = AUDITORY;
    trialCnt = 0;
    right = wrong = 0;
    reactionTime.clear();
}

void MainWindow::show_tactile_test()
{
    this->show();
    status = TACTILE;
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
    if (!isInTrial && (ev->key() == Qt::Key_Left || ev->key() == Qt::Key_Right)) {
        isInTrial = true;
        if (status == PRACTICE) {
            showTarget(ev->key());
        }
        else if (status == VISUAL) {
            showVisualCue(ev->key());
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
            showTarget(ev->key());
        }
        else if (status == AUDITORY) {
            playAuditoryCue(ev->key());
            QTime t;
            t.start();
            while(t.elapsed() < 1000) {
                QCoreApplication::processEvents();
            }
            showTarget(ev->key());
        }
        else if (status == TACTILE) {
            QTime t;
            t.start();
            while(t.elapsed() < 1000) {
                QCoreApplication::processEvents();
            }
            showTarget(ev->key());
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

        if (trialCnt == NUM_TRIAL) {  // end test
            logToFile();
            this->hide();
            status = IDLE;
            emit end_test();
        }
    }
}

void MainWindow::showTarget(int key)
{
    int w = this->size().width(), h = this->size().height(), halfW = w / 2;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int randX = qrand() % (halfW - TARGET_SIZE), randY = qrand() % (h - TARGET_SIZE);
    if (key == Qt::Key_Right) {
        randX += halfW;
    }
    qDebug() << randX << ", " << randY << endl;
    currentTargetPos = QPoint(randX, randY);
    refresh();
    QPainter painter(&pix);
    drawMiddleLine(painter);
    painter.fillRect(randX, randY, TARGET_SIZE, TARGET_SIZE, Qt::blue);
    update();
    timer.start();

}

void MainWindow::showVisualCue(int key)
{
    int w = this->size().width(), h = this->size().height();
    int posX = w / 4, posY = h / 2;
    if (key == Qt::Key_Right) {
        posX += w / 2;
    }
    refresh();
    QPainter painter(&pix);
    drawMiddleLine(painter);
    painter.fillRect(posX, posY, TARGET_SIZE, TARGET_SIZE, Qt::red);
    update();
}

void MainWindow::playAuditoryCue(int key)
{
    QMediaPlayer *player = new QMediaPlayer();
    QString audio = key == Qt::Key_Left ? "Left.mp3" : "Right.mp3";
    player->setMedia(QUrl("qrc:/audio/res/audios/" + audio));
    player->play();
}

void MainWindow::logToFile()
{
    const std::unordered_map<Status, QString> kMap = {
        { PRACTICE, "practice" },
        { VISUAL, "visual" },
        { AUDITORY, "auditory" },
        { TACTILE, "tactile" }
    };
    auto time = QDateTime::currentDateTime().toString("yyyyMMdd_hh-mm-ss");
    QString fileName = kMap.at(status) + "_" + time + ".txt";
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



