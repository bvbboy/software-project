#include "paradialog.h"
#include "ui_paradialog.h"
#include "qmessagebox.h"
paraDialog::paraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::paraDialog)
{
    ui->setupUi(this);
}

paraDialog::~paraDialog()
{
    delete ui;
}

void paraDialog::on_pushButton_clicked()
{
    stride=ui->strideEdit->text().toDouble();
    interationNumber=ui->iterationNumEdit->text().toInt();
    isVariantPace=ui->checkBox->isChecked();
    if(stride<0.5 && stride>0 && interationNumber<10 && interationNumber>=6)
    {
        emit settingDoneW(interationNumber,isVariantPace,stride);
        this->close();
    }else
    {
        QMessageBox::information(this, tr(""), tr("重新设置参数"));
    }

}


void paraDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->strideEdit->setEnabled(false);
    }else
    {
        ui->strideEdit->setEnabled(true);
    }

}
