#ifndef SECTIONDIALOG_H
#define SECTIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class SectionDialog;
}

class SectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SectionDialog(QWidget *parent = 0);
    ~SectionDialog();

private:
    Ui::SectionDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *section_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void list_section_changed(const QModelIndex &current);
};

#endif // SECTIONDIALOG_H
