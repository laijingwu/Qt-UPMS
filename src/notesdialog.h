#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class NotesDialog;
}

class NotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialog(QWidget *parent = 0);
    ~NotesDialog();

private:
    Ui::NotesDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *notes_model;

private slots:
    void btn_select_clicked();
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_cancel_clicked();
    void list_notes_changed(const QModelIndex &current);
};

#endif // NOTESDIALOG_H
