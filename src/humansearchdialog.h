#ifndef HUMANSEARCHDIALOG_H
#define HUMANSEARCHDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class HumanSearchDialog;
}

class HumanSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HumanSearchDialog(QWidget *parent = 0);
    ~HumanSearchDialog();

private:
    Ui::HumanSearchDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *staff_model;

private slots:
    void btn_search_clicked();
};

#endif // HUMANSEARCHDIALOG_H
