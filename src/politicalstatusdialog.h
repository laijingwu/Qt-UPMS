#ifndef POLITICALSTATUSDIALOG_H
#define POLITICALSTATUSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class PoliticalStatusDialog;
}

class PoliticalStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PoliticalStatusDialog(QWidget *parent = 0);
    ~PoliticalStatusDialog();

private:
    Ui::PoliticalStatusDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *political_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // POLITICALSTATUSDIALOG_H
