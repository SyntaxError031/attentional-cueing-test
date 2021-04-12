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
//    bool isPractice;
    bool isInTrial;
    int testCode;
    static const int TARGET_SIZE = 10;
    static const int EASY_TARGET_SIZE = 20;
    static const int NUM_TRIAL = 20;
    static const int PRACTICE_NUM_TRIAL = 5;
    QPoint currentTargetPos;
    QTime timer;
    std::vector<double> reactionTime;
    int trialCnt;
    int right;
    int wrong;

    const std::vector<std::vector<int>> kSequence = {
        { 0, 1, 1, 0, 0, },
        { 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, },
        { 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, },
        { 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, },
        { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, },
        { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, },
        { 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, },
        { 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, },
        { 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, },
    };

    void showTarget(int level);
    void showVisualCue();
    void playAuditoryCue();
    bool isOnTarget(const QPoint &pos);
    void drawMiddleLine(QPainter &painter);
    void refresh();
    void logToFile();

private slots:
    void show_visual_test(int level, bool practice);
    void show_auditory_test(int level, bool practice);
    void show_tactile_test(int level, bool practice);

signals:
    void end_test();
};

#endif // MAINWINDOW_H
