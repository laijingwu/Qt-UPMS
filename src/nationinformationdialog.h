#ifndef NATIONINFORMATIONDIALOG_H
#define NATIONINFORMATIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class NationInformationDialog;
}

class NationInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NationInformationDialog(QWidget *parent = 0);
    ~NationInformationDialog();

private:
    Ui::NationInformationDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *nationInf_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // NATIONINFORMATIONDIALOG_H
