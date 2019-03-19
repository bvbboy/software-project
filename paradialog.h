#ifndef PARADIALOG_H
#define PARADIALOG_H

#include <QDialog>

namespace Ui {
class paraDialog;
}

class paraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit paraDialog(QWidget *parent = 0);
    int interationNumber;
    double stride;
    bool isVariantPace;
    ~paraDialog();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::paraDialog *ui;
signals:
    //
    void settingDoneW(int interationNum,bool isVariantpace, double stride);
};

#endif // PARADIALOG_H
