#include "auxiliarydialog.h"
#include "ui_auxiliarydialog.h"

AuxiliaryDialog::AuxiliaryDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AuxiliaryDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    auxiliary_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    auxiliary_model->setTable("auxiliary");
    auxiliary_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    auxiliary_model->select();
    auxiliary_model->setHeaderData(0, Qt::Horizontal, tr("部门编码"));
    auxiliary_model->setHeaderData(1, Qt::Horizontal, tr("部门名称"));
    auxiliary_model->setHeaderData(2, Qt::Horizontal, tr("部门领导"));
    auxiliary_model->setHeaderData(3, Qt::Horizontal, tr("办公地点"));
    auxiliary_model->setHeaderData(4, Qt::Horizontal, tr("组织机构"));
    auxiliary_model->setHeaderData(5, Qt::Horizontal, tr("岗位职责"));
    while (auxiliary_model->canFetchMore())
        auxiliary_model->fetchMore();

    ui->list_auxiliary->setModel(auxiliary_model);
    ui->list_auxiliary->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->combo_condition->addItem(tr("按部门编码查询"), "code");
    ui->combo_condition->addItem(tr("按部门名称查询"), "name");
    ui->combo_condition->addItem(tr("按部门领导查询"), "leader");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->list_auxiliary->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_auxiliary_changed(const QModelIndex &)));
    connect(ui->list_auxiliary, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_auxiliary_changed(const QModelIndex &)));
    ui->list_auxiliary->selectRow(0);
}

AuxiliaryDialog::~AuxiliaryDialog()
{
    delete auxiliary_model;
    delete ui;
}

void AuxiliaryDialog::btn_add_clicked()
{
    // 清空教辅部门详细信息中的所有数据
    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_duty->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void AuxiliaryDialog::btn_save_clicked()
{
    // auxiliary 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教辅部门"), tr("请输入部门编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教辅部门"), tr("请输入部门名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教辅部门"), tr("请输入部门领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = auxiliary_model->query();
    query.prepare("SELECT * FROM `auxiliary` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 新增教辅部门
    if (query.size() > 0)
    {
        // 存在教辅部门
        QMessageBox::critical(this, tr("新增教辅部门失败"), tr("该部门编码已存在！"));
        return;
    }
    int rowNum = auxiliary_model->rowCount();
    auxiliary_model->insertRow(rowNum);
    auxiliary_model->setData(auxiliary_model->index(rowNum, 0), ui->edit_code->text().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 5), ui->edit_duty->toPlainText().trimmed());
    auxiliary_model->submitAll();
    auxiliary_model->select();
    QMessageBox::information(this, tr("提示"), tr("教辅部门添加成功！"));
    ui->list_auxiliary->selectRow(rowNum);
}

void AuxiliaryDialog::btn_update_clicked()
{
    // auxiliary 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教辅部门"), tr("请输入部门编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教辅部门"), tr("请输入部门名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教辅部门"), tr("请输入部门领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = auxiliary_model->query();
    query.prepare("SELECT * FROM `auxiliary` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 修改教辅部门
    if (query.size() == 0)
    {
        // 不存在教辅部门
        QMessageBox::critical(this, tr("修改教辅部门失败"), tr("该部门编码不存在！"));
        return;
    }
    int rowNum = ui->list_auxiliary->currentIndex().row(); // 获取选中行
    auxiliary_model->setData(auxiliary_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    auxiliary_model->setData(auxiliary_model->index(rowNum, 5), ui->edit_duty->toPlainText().trimmed());
    auxiliary_model->submitAll();
    auxiliary_model->select();
    QMessageBox::information(this, tr("提示"), tr("教辅部门修改成功！"));
    ui->list_auxiliary->selectRow(rowNum);
}

void AuxiliaryDialog::btn_delete_clicked()
{
    if (auxiliary_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除教辅部门"), tr("您是否确定删除当前选中的教辅部门信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_auxiliary->currentIndex().row(); // 获取选中行
        auxiliary_model->removeRow(nIndexRow--);
        auxiliary_model->submitAll();
        auxiliary_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_auxiliary->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_auxiliary->selectRow(nIndexRow);
        else
        {
            ui->edit_code->clear();
            ui->combo_name->clearEditText();
            ui->combo_leader->clearEditText();
            ui->edit_location->clear();
            ui->edit_setting->clear();
            ui->edit_duty->clear();
            ui->btn_delete->setEnabled(false);
        }
    }
}

void AuxiliaryDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        auxiliary_model->setFilter("1");
    else
        auxiliary_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    auxiliary_model->select();

    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_duty->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void AuxiliaryDialog::list_auxiliary_changed(const QModelIndex &current)
{
    ui->edit_code->setText(auxiliary_model->index(current.row(), 0).data().toString());
    ui->combo_name->setEditText(auxiliary_model->index(current.row(), 1).data().toString());
    ui->combo_leader->setEditText(auxiliary_model->index(current.row(), 2).data().toString());
    ui->edit_location->setText(auxiliary_model->index(current.row(), 3).data().toString());
    ui->edit_setting->setText(auxiliary_model->index(current.row(), 4).data().toString());
    ui->edit_duty->setPlainText(auxiliary_model->index(current.row(), 5).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
