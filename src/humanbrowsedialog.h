#ifndef HUMANBROSEDIALOG_H
#define HUMANBROSEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class HumanBrowseDialog;
}

class HumanBrowseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HumanBrowseDialog(QWidget *parent = 0);
    ~HumanBrowseDialog();

private:
    Ui::HumanBrowseDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *staff_model;
    QSqlTableModel *family_model;
    QSqlTableModel *workexp_model;
    QSqlTableModel *reward_model;
    QSqlTableModel *punish_model;
    QSqlTableModel *paper_model;
    QSqlTableModel *project_model;
    void tab_base_archive_init();
    void tab_family_member_init();
    void tab_work_experience_init();
    void tab_reward_init();
    void tab_punish_init();
    void tab_paper_init();
    void tab_project_init();

private slots:
    void btn_total_add_clicked();
    void btn_total_save_clicked();
    void btn_total_update_clicked();
    void btn_total_delete_clicked();
    void btn_total_cancel_clicked();
    void list_staff_changed(const QModelIndex &current);
    void btn_family_add_clicked();
    void btn_family_save_clicked();
    void btn_family_update_clicked();
    void btn_family_delete_clicked();
    void list_family_changed(const QModelIndex &current);
    void btn_workexp_add_clicked();
    void btn_workexp_save_clicked();
    void btn_workexp_update_clicked();
    void btn_workexp_delete_clicked();
    void list_workexp_changed(const QModelIndex &current);
    void btn_reward_add_clicked();
    void btn_reward_save_clicked();
    void btn_reward_update_clicked();
    void btn_reward_delete_clicked();
    void list_reward_changed(const QModelIndex &current);
    void btn_punish_add_clicked();
    void btn_punish_save_clicked();
    void btn_punish_update_clicked();
    void btn_punish_delete_clicked();
    void list_punish_changed(const QModelIndex &current);
    void btn_paper_add_clicked();
    void btn_paper_save_clicked();
    void btn_paper_update_clicked();
    void btn_paper_delete_clicked();
    void list_paper_changed(const QModelIndex &current);
    void btn_project_add_clicked();
    void btn_project_save_clicked();
    void btn_project_update_clicked();
    void btn_project_delete_clicked();
    void list_project_changed(const QModelIndex &current);
//    void btn_select_clicked();
};

#endif // HUMANBROSEDIALOG_H
