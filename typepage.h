#ifndef TYPEPAGE_H
#define TYPEPAGE_H

#include <QWidget>

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

private:
    Ui::TypePage *ui;

signals:
    void practice();
    void visual_test();
    void auditory_test();
    void tactile_test();
};

#endif // TYPEPAGE_H
