#ifndef DEPARTMENTSECTIONDIALOG_H
#define DEPARTMENTSECTIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class DepartmentSectionDialog;
}

class DepartmentSectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DepartmentSectionDialog(QWidget *parent = 0);
    ~DepartmentSectionDialog();

private:
    Ui::DepartmentSectionDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *department_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // DEPARTMENTSECTIONDIALOG_H
