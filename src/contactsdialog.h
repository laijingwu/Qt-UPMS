#ifndef CONTACTSDIALOG_H
#define CONTACTSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"
#include "newcontactdialog.h"

namespace Ui {
class ContactsDialog;
}

class ContactsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContactsDialog(QWidget *parent = 0);
    ~ContactsDialog();

private:
    Ui::ContactsDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *contacts_model;

private slots:
    void btn_select_clicked();
    void btn_add_clicked();
    void btn_check_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void list_contacts_changed(const QModelIndex &current);
    void list_contacts_doubleclicked(const QModelIndex &current);
};

#endif // CONTACTSDIALOG_H
