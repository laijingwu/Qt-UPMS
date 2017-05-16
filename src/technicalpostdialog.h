#ifndef TECHNICALPOSTDIALOG_H
#define TECHNICALPOSTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "addbaseinfodialog.h"
#include "sqlmodel.h"

namespace Ui {
class TechnicalPostDialog;
}

class TechnicalPostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TechnicalPostDialog(QWidget *parent = 0);
    ~TechnicalPostDialog();

private:
    Ui::TechnicalPostDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *technical_model;

private slots:
    void btn_add_clicked();
    void btn_delete_clicked();
};

#endif // TECHNICALPOSTDIALOG_H
