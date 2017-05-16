#include "appendantdialog.h"
#include "ui_appendantdialog.h"

AppendantDialog::AppendantDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AppendantDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    appendant_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    appendant_model->setTable("appendant");
    appendant_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    appendant_model->select();
    appendant_model->setHeaderData(0, Qt::Horizontal, tr("单位编码"));
    appendant_model->setHeaderData(1, Qt::Horizontal, tr("单位名称"));
    appendant_model->setHeaderData(2, Qt::Horizontal, tr("单位领导"));
    appendant_model->setHeaderData(3, Qt::Horizontal, tr("办公地点"));
    appendant_model->setHeaderData(4, Qt::Horizontal, tr("组织机构"));
    appendant_model->setHeaderData(5, Qt::Horizontal, tr("规章制度"));
    while (appendant_model->canFetchMore())
        appendant_model->fetchMore();

    ui->list_appendant->setModel(appendant_model);
    ui->list_appendant->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->combo_condition->addItem(tr("按单位编码查询"), "code");
    ui->combo_condition->addItem(tr("按单位名称查询"), "name");
    ui->combo_condition->addItem(tr("按单位领导查询"), "leader");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->list_appendant->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_appendant_changed(const QModelIndex &)));
    connect(ui->list_appendant, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_appendant_changed(const QModelIndex &)));
    ui->list_appendant->selectRow(0);
}

AppendantDialog::~AppendantDialog()
{
    delete appendant_model;
    delete ui;
}

void AppendantDialog::btn_add_clicked()
{
    // 清空附属单位详细信息中的所有数据
    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_regulation->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void AppendantDialog::btn_save_clicked()
{
    // appendant 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存附属单位"), tr("请输入单位编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存附属单位"), tr("请输入单位名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存附属单位"), tr("请输入单位领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = appendant_model->query();
    query.prepare("SELECT * FROM `appendant` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 新增附属单位
    if (query.size() > 0)
    {
        // 存在附属单位
        QMessageBox::critical(this, tr("新增附属单位失败"), tr("该单位编码已存在！"));
        return;
    }
    int rowNum = appendant_model->rowCount();
    appendant_model->insertRow(rowNum);
    appendant_model->setData(appendant_model->index(rowNum, 0), ui->edit_code->text().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 5), ui->edit_regulation->toPlainText().trimmed());
    appendant_model->submitAll();
    appendant_model->select();
    QMessageBox::information(this, tr("提示"), tr("附属单位添加成功！"));
    ui->list_appendant->selectRow(rowNum);
}

void AppendantDialog::btn_update_clicked()
{
    // appendant 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改附属单位"), tr("请输入单位编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改附属单位"), tr("请输入单位名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改附属单位"), tr("请输入单位领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = appendant_model->query();
    query.prepare("SELECT * FROM `appendant` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 修改附属单位
    if (query.size() == 0)
    {
        // 不存在附属单位
        QMessageBox::critical(this, tr("修改附属单位失败"), tr("该单位编码不存在！"));
        return;
    }
    int rowNum = ui->list_appendant->currentIndex().row(); // 获取选中行
    appendant_model->setData(appendant_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    appendant_model->setData(appendant_model->index(rowNum, 5), ui->edit_regulation->toPlainText().trimmed());
    appendant_model->submitAll();
    appendant_model->select();
    QMessageBox::information(this, tr("提示"), tr("附属单位修改成功！"));
    ui->list_appendant->selectRow(rowNum);
}

void AppendantDialog::btn_delete_clicked()
{
    if (appendant_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除附属单位"), tr("您是否确定删除当前选中的附属单位信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_appendant->currentIndex().row(); // 获取选中行
        appendant_model->removeRow(nIndexRow--);
        appendant_model->submitAll();
        appendant_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_appendant->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_appendant->selectRow(nIndexRow);
        else
        {
            ui->edit_code->clear();
            ui->combo_name->clearEditText();
            ui->combo_leader->clearEditText();
            ui->edit_location->clear();
            ui->edit_setting->clear();
            ui->edit_regulation->clear();
            ui->btn_delete->setEnabled(false);
        }
    }
}

void AppendantDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        appendant_model->setFilter("1");
    else
        appendant_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    appendant_model->select();

    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_regulation->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void AppendantDialog::list_appendant_changed(const QModelIndex &current)
{
    ui->edit_code->setText(appendant_model->index(current.row(), 0).data().toString());
    ui->combo_name->setEditText(appendant_model->index(current.row(), 1).data().toString());
    ui->combo_leader->setEditText(appendant_model->index(current.row(), 2).data().toString());
    ui->edit_location->setText(appendant_model->index(current.row(), 3).data().toString());
    ui->edit_setting->setText(appendant_model->index(current.row(), 4).data().toString());
    ui->edit_regulation->setPlainText(appendant_model->index(current.row(), 5).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
