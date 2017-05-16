#ifndef POSITIONINFORMATIONDIALOG_H
#define POSITIONINFORMATIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class PositionInformationDialog;
}

class PositionInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PositionInformationDialog(QWidget *parent = 0);
    ~PositionInformationDialog();

private:
    Ui::PositionInformationDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *positionInf_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // POSITIONINFORMATIONDIALOG_H
