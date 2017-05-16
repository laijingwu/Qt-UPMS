#ifndef USERSETTINGDIALOG_H
#define USERSETTINGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class UserSettingDialog;
}

class UserSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserSettingDialog(QWidget *parent = 0);
    ~UserSettingDialog();

private:
    Ui::UserSettingDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *user_model;

private slots:
    void btn_cancel_clicked();
    void btn_delete_clicked();
    void btn_add_clicked();
    void list_user_changed(const QModelIndex &current);
    void btn_save_clicked();
};

#endif // USERSETTINGDIALOG_H
