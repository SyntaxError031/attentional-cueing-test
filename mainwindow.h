#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTime>
#include <vector>
#include <QFile>
#include <unordered_map>
#include <QCoreApplication>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

enum Status
{
    IDLE, PRACTICE, VISUAL, AUDITORY, TACTILE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    Ui::MainWindow *ui;
    QPixmap pix;
    Status status;
    int difficulty;
    bool isInTrial;
    static const int TARGET_SIZE = 10;
    static const int EASY_TARGET_SIZE = 20;
    static const int NUM_TRIAL = 5;
    QPoint currentTargetPos;
    QTime timer;
    std::vector<double> reactionTime;
    int trialCnt;
    int right;
    int wrong;

    void showTarget(int key, int level);
    void showVisualCue(int key);
    void playAuditoryCue(int key);
    bool isOnTarget(const QPoint &pos);
    void drawMiddleLine(QPainter &painter);
    void refresh();
    void logToFile();

private slots:
    void show_practice(int level);
    void show_visual_test(int level);
    void show_auditory_test(int level);
    void show_tactile_test(int level);

signals:
    void end_test();
};

#endif // MAINWINDOW_H
