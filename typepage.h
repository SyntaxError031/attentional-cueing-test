#ifndef TYPEPAGE_H
#define TYPEPAGE_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class TypePage;
}

class TypePage : public QWidget
{
    Q_OBJECT

public:
    explicit TypePage(QWidget *parent = nullptr);
    ~TypePage();

private slots:
    void on_practice_clicked();
    void on_visual_clicked();
    void on_auditory_clicked();
    void on_tactile_clicked();

    void show_typepage();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::TypePage *ui;

signals:
    void practice(int level);
    void visual_test(int level, bool practice);
    void auditory_test(int level, bool practice);
    void tactile_test(int level, bool practice);
};

#endif // TYPEPAGE_H
