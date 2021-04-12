#include "typepage.h"
#include "ui_typepage.h"

TypePage::TypePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TypePage)
{
    ui->setupUi(this);
    ui->comboBox->addItem("easy");
    ui->comboBox->addItem("medium");
    ui->comboBox->addItem("hard");
}

TypePage::~TypePage()
{
    delete ui;
}

void TypePage::on_visual_clicked()
{
    this->hide();
    emit visual_test(ui->comboBox->currentIndex(), ui->checkBox->isChecked());
}

void TypePage::on_auditory_clicked()
{
    this->hide();
    emit auditory_test(ui->comboBox->currentIndex(), ui->checkBox->isChecked());
}

void TypePage::on_tactile_clicked()
{
    this->hide();
    emit tactile_test(ui->comboBox->currentIndex(), ui->checkBox->isChecked());
}

void TypePage::show_typepage()
{
    this->show();
}

void TypePage::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;
}
