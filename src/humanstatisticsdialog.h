#ifndef HUMANSTATISTICSDIALOG_H
#define HUMANSTATISTICSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include "sqlmodel.h"

namespace Ui {
class HumanStatisticsDialog;
}

class HumanStatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HumanStatisticsDialog(QWidget *parent = 0);
    ~HumanStatisticsDialog();

private:
    Ui::HumanStatisticsDialog *ui;
    SQLModel *SqlController;
    QStandardItemModel *select_model;
    QStandardItemModel *result_model;

private slots:
    void list_selection_changed(const QModelIndex & current);
};

#endif // HUMANSTATISTICSDIALOG_H
