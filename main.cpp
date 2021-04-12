#include "mainwindow.h"
#include "typepage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    TypePage typepage;
    typepage.show();
    //w.show();
    QObject::connect(&typepage, SIGNAL(visual_test(int, bool)), &w, SLOT(show_visual_test(int, bool)));
    QObject::connect(&typepage, SIGNAL(auditory_test(int, bool)), &w, SLOT(show_auditory_test(int, bool)));
    QObject::connect(&typepage, SIGNAL(tactile_test(int, bool)), &w, SLOT(show_tactile_test(int, bool)));
    QObject::connect(&w, SIGNAL(end_test()), &typepage, SLOT(show_typepage()));

    return a.exec();
}
