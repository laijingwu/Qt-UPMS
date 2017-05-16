#ifndef FULLTIMEDIALOG_H
#define FULLTIMEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class FulltimeDialog;
}

class FulltimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FulltimeDialog(QWidget *parent = 0);
    ~FulltimeDialog();

private:
    Ui::FulltimeDialog *ui;
    SQLModel *SqlController;
    QSqlTableModel *fulltime_model;

private slots:
    void btn_add_clicked();
    void btn_save_clicked();
    void btn_update_clicked();
    void btn_delete_clicked();
    void btn_select_clicked();
    void list_fulltime_changed(const QModelIndex &current);
};

#endif // FULLTIMEDIALOG_H
