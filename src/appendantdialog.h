#ifndef APPENDANTDIALOG_H
#define APPENDANTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class AppendantDialog;
}

class AppendantDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppendantDialog(QWidget *parent = 0);
    ~AppendantDialog();

private:
    Ui::AppendantDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *appendant_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void list_appendant_changed(const QModelIndex &current);
};

#endif // APPENDANTDIALOG_H
