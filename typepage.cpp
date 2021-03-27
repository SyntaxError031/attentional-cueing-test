#include "typepage.h"
#include "ui_typepage.h"

TypePage::TypePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TypePage)
{
    ui->setupUi(this);
}

TypePage::~TypePage()
{
    delete ui;
}

void TypePage::on_practice_clicked()
{
    this->hide();
    emit practice();
}

void TypePage::on_visual_clicked()
{
    this->hide();
    emit visual_test();
}

void TypePage::on_auditory_clicked()
{
    this->hide();
    emit auditory_test();
}

void TypePage::on_tactile_clicked()
{
    this->hide();
    emit tactile_test();
}

void TypePage::show_typepage()
{
    this->show();
}
