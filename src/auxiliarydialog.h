#ifndef AUXILIARYDIALOG_H
#define AUXILIARYDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class AuxiliaryDialog;
}

class AuxiliaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuxiliaryDialog(QWidget *parent = 0);
    ~AuxiliaryDialog();

private:
    Ui::AuxiliaryDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *auxiliary_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void list_auxiliary_changed(const QModelIndex &current);
};

#endif // AUXILIARYDIALOG_H
