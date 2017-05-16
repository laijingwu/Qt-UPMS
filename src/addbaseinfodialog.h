#ifndef ADDBASEINFODIALOG_H
#define ADDBASEINFODIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class AddBaseInfoDialog;
}

class AddBaseInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBaseInfoDialog(QWidget *parent = 0);
    ~AddBaseInfoDialog();
    void setStaticUi(QString staticBox_label, QString staticLabel);
    QString getStaticText();
    QString getDescriptionText();
    void closeEvent(QCloseEvent *event);

private:
    Ui::AddBaseInfoDialog *ui;

private slots:
    void btn_ok_clicked();
};

#endif // ADDBASEINFODIALOG_H
