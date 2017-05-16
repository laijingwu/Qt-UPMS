#include "sectiondialog.h"
#include "ui_sectiondialog.h"

SectionDialog::SectionDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::SectionDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    section_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    section_model->setTable("section");
    section_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    section_model->select();
    section_model->setHeaderData(0, Qt::Horizontal, tr("部门编码"));
    section_model->setHeaderData(1, Qt::Horizontal, tr("部门名称"));
    section_model->setHeaderData(2, Qt::Horizontal, tr("部门领导"));
    section_model->setHeaderData(3, Qt::Horizontal, tr("办公地点"));
    section_model->setHeaderData(4, Qt::Horizontal, tr("组织机构"));
    section_model->setHeaderData(5, Qt::Horizontal, tr("规章制度"));
    while (section_model->canFetchMore())
        section_model->fetchMore();

    ui->list_section->setModel(section_model);
    ui->list_section->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->combo_condition->addItem(tr("按部门编码查询"), "code");
    ui->combo_condition->addItem(tr("按部门名称查询"), "name");
    ui->combo_condition->addItem(tr("按部门领导查询"), "leader");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->list_section->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_section_changed(const QModelIndex &)));
    connect(ui->list_section, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_section_changed(const QModelIndex &)));
    ui->list_section->selectRow(0);
}

SectionDialog::~SectionDialog()
{
    delete section_model;
    delete ui;
}

void SectionDialog::btn_add_clicked()
{
    // 清空管理部门详细信息中的所有数据
    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_rule->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void SectionDialog::btn_save_clicked()
{
    // section 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存管理部门"), tr("请输入部门编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存管理部门"), tr("请输入部门名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存管理部门"), tr("请输入部门领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = section_model->query();
    query.prepare("SELECT * FROM `section` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 新增管理部门
    if (query.size() > 0)
    {
        // 存在管理部门
        QMessageBox::critical(this, tr("新增管理部门失败"), tr("该部门编码已存在！"));
        return;
    }
    int rowNum = section_model->rowCount();
    section_model->insertRow(rowNum);
    section_model->setData(section_model->index(rowNum, 0), ui->edit_code->text().trimmed());
    section_model->setData(section_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    section_model->setData(section_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    section_model->setData(section_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    section_model->setData(section_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    section_model->setData(section_model->index(rowNum, 5), ui->edit_rule->toPlainText().trimmed());
    section_model->submitAll();
    section_model->select();
    QMessageBox::information(this, tr("提示"), tr("管理部门添加成功！"));
    ui->list_section->selectRow(rowNum);
}

void SectionDialog::btn_update_clicked()
{
    // section 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改管理部门"), tr("请输入部门编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改管理部门"), tr("请输入部门名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改管理部门"), tr("请输入部门领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = section_model->query();
    query.prepare("SELECT * FROM `section` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 修改管理部门
    if (query.size() == 0)
    {
        // 不存在管理部门
        QMessageBox::critical(this, tr("修改管理部门失败"), tr("该部门编码不存在！"));
        return;
    }
    int rowNum = ui->list_section->currentIndex().row(); // 获取选中行
    section_model->setData(section_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    section_model->setData(section_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    section_model->setData(section_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    section_model->setData(section_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    section_model->setData(section_model->index(rowNum, 5), ui->edit_rule->toPlainText().trimmed());
    section_model->submitAll();
    section_model->select();
    QMessageBox::information(this, tr("提示"), tr("管理部门修改成功！"));
    ui->list_section->selectRow(rowNum);
}

void SectionDialog::btn_delete_clicked()
{
    if (section_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除管理部门"), tr("您是否确定删除当前选中的管理部门信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_section->currentIndex().row(); // 获取选中行
        section_model->removeRow(nIndexRow--);
        section_model->submitAll();
        section_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_section->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_section->selectRow(nIndexRow);
        else
        {
            ui->edit_code->clear();
            ui->combo_name->clearEditText();
            ui->combo_leader->clearEditText();
            ui->edit_location->clear();
            ui->edit_setting->clear();
            ui->edit_rule->clear();
            ui->btn_delete->setEnabled(false);
        }
    }
}

void SectionDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        section_model->setFilter("1");
    else
        section_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    section_model->select();

    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_rule->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void SectionDialog::list_section_changed(const QModelIndex &current)
{
    ui->edit_code->setText(section_model->index(current.row(), 0).data().toString());
    ui->combo_name->setEditText(section_model->index(current.row(), 1).data().toString());
    ui->combo_leader->setEditText(section_model->index(current.row(), 2).data().toString());
    ui->edit_location->setText(section_model->index(current.row(), 3).data().toString());
    ui->edit_setting->setText(section_model->index(current.row(), 4).data().toString());
    ui->edit_rule->setPlainText(section_model->index(current.row(), 5).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
