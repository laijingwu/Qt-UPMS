#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include "sqlmodel.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    SQLModel *SqlController;

private slots:
    void btn_login();
};

#endif // LOGINDIALOG_H
