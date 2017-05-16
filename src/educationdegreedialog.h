#ifndef EDUCATIONDEGREEDIALOG_H
#define EDUCATIONDEGREEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class EducationDegreeDialog;
}

class EducationDegreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EducationDegreeDialog(QWidget *parent = 0);
    ~EducationDegreeDialog();

private:
    Ui::EducationDegreeDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *education_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // EDUCATIONDEGREEDIALOG_H
