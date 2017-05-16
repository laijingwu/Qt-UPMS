#ifndef ADMINISTRATIVEPOSTDIALOG_H
#define ADMINISTRATIVEPOSTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class AdministrativePostDialog;
}

class AdministrativePostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdministrativePostDialog(QWidget *parent = 0);
    ~AdministrativePostDialog();

private:
    Ui::AdministrativePostDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *administrative_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // ADMINISTRATIVEPOSTDIALOG_H
