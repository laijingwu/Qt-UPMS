#ifndef DEPARTMENTDIALOG_H
#define DEPARTMENTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class DepartmentDialog;
}

class DepartmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DepartmentDialog(QWidget *parent = 0);
    ~DepartmentDialog();

private:
    Ui::DepartmentDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *department_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void list_department_changed(const QModelIndex &current);
};

#endif // DEPARTMENTDIALOG_H
