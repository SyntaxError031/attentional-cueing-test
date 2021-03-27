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
    QObject::connect(&typepage, SIGNAL(practice()), &w, SLOT(show_practice()));
    QObject::connect(&typepage, SIGNAL(visual_test()), &w, SLOT(show_visual_test()));
    QObject::connect(&typepage, SIGNAL(auditory_test()), &w, SLOT(show_auditory_test()));
    QObject::connect(&typepage, SIGNAL(tactile_test()), &w, SLOT(show_tactile_test()));
    QObject::connect(&w, SIGNAL(end_test()), &typepage, SLOT(show_typepage()));

    return a.exec();
}
