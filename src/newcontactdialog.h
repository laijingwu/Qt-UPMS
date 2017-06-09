#ifndef NEWCONTACTDIALOG_H
#define NEWCONTACTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class NewContactDialog;
}

class NewContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewContactDialog(QWidget *parent = 0);
    ~NewContactDialog();
    void closeEvent(QCloseEvent *event);
    void setEditorEnabled(bool enabled);
    void setEditorText(QString name, QString sex, QString fphone, QString ophone, QString mphone,
                       QString mshort, QString qq, QString wechat, QString email, QString unit, QString remark);
    void getEditorText(QString &name, QString &sex, QString &fphone, QString &ophone, QString &mphone,
                       QString &mshort, QString &qq, QString &wechat, QString &email, QString &unit, QString &remark);

private:
    Ui::NewContactDialog *ui;

private slots:
    void btn_ok_clicked();
};

#endif // NEWCONTACTDIALOG_H
