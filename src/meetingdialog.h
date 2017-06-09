#ifndef MEETINGDIALOG_H
#define MEETINGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class MeetingDialog;
}

class MeetingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeetingDialog(QWidget *parent = 0);
    ~MeetingDialog();

private:
    Ui::MeetingDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *meeting_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void btn_cancel_clicked();
    void list_meeting_changed(const QModelIndex &current);
};

#endif // MEETINGDIALOG_H
