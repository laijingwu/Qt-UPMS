#ifndef WAGELEVELDIALOG_H
#define WAGELEVELDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class WageLevelDialog;
}

class WageLevelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WageLevelDialog(QWidget *parent = 0);
    ~WageLevelDialog();

private:
    Ui::WageLevelDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *wage_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // WAGELEVELDIALOG_H
