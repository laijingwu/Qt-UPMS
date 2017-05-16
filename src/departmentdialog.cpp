#include "departmentdialog.h"
#include "ui_departmentdialog.h"

DepartmentDialog::DepartmentDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::DepartmentDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    department_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    department_model->setTable("department");
    department_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    department_model->select();
    department_model->setHeaderData(0, Qt::Horizontal, tr("院系编码"));
    department_model->setHeaderData(1, Qt::Horizontal, tr("院系名称"));
    department_model->setHeaderData(2, Qt::Horizontal, tr("院系领导"));
    department_model->setHeaderData(3, Qt::Horizontal, tr("办公地点"));
    department_model->setHeaderData(4, Qt::Horizontal, tr("组织机构"));
    department_model->setHeaderData(5, Qt::Horizontal, tr("教学工作"));
    department_model->setHeaderData(6, Qt::Horizontal, tr("科研工作"));
    department_model->setHeaderData(7, Qt::Horizontal, tr("学生工作"));
    while (department_model->canFetchMore())
        department_model->fetchMore();

    ui->list_department->setModel(department_model);
    ui->list_department->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->combo_condition->addItem(tr("按院系编码查询"), "code");
    ui->combo_condition->addItem(tr("按院系名称查询"), "name");
    ui->combo_condition->addItem(tr("按院系领导查询"), "leader");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->list_department->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_department_changed(const QModelIndex &)));
    connect(ui->list_department, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_department_changed(const QModelIndex &)));
    ui->list_department->selectRow(0);
}

DepartmentDialog::~DepartmentDialog()
{
    delete department_model;
    delete ui;
}

void DepartmentDialog::btn_add_clicked()
{
    // 清空院系详细信息中的所有数据
    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_teaching->clear();
    ui->edit_researching->clear();
    ui->edit_student->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void DepartmentDialog::btn_save_clicked()
{
    // department 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教学院系"), tr("请输入院系编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教学院系"), tr("请输入院系名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存教学院系"), tr("请输入院系领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = department_model->query();
    query.prepare("SELECT * FROM `department` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 新增教学院系
    if (query.size() > 0)
    {
        // 存在教学院系
        QMessageBox::critical(this, tr("新增教学院系失败"), tr("该院系编码已存在！"));
        return;
    }
    int rowNum = department_model->rowCount();
    department_model->insertRow(rowNum);
    department_model->setData(department_model->index(rowNum, 0), ui->edit_code->text().trimmed());
    department_model->setData(department_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    department_model->setData(department_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    department_model->setData(department_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    department_model->setData(department_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    department_model->setData(department_model->index(rowNum, 5), ui->edit_teaching->text().trimmed());
    department_model->setData(department_model->index(rowNum, 6), ui->edit_researching->text().trimmed());
    department_model->setData(department_model->index(rowNum, 7), ui->edit_student->text().trimmed());
    department_model->submitAll();
    department_model->select();
    QMessageBox::information(this, tr("提示"), tr("教学院系添加成功！"));
    ui->list_department->selectRow(rowNum);
}

void DepartmentDialog::btn_update_clicked()
{
    // department 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教学院系"), tr("请输入院系编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教学院系"), tr("请输入院系名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教学院系"), tr("请输入院系领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = department_model->query();
    query.prepare("SELECT * FROM `department` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 修改教学院系
    if (query.size() == 0)
    {
        // 不存在教学院系
        QMessageBox::critical(this, tr("修改教学院系失败"), tr("该院系编码不存在！"));
        return;
    }
    int rowNum = ui->list_department->currentIndex().row(); // 获取选中行
    department_model->setData(department_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    department_model->setData(department_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    department_model->setData(department_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    department_model->setData(department_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    department_model->setData(department_model->index(rowNum, 5), ui->edit_teaching->text().trimmed());
    department_model->setData(department_model->index(rowNum, 6), ui->edit_researching->text().trimmed());
    department_model->setData(department_model->index(rowNum, 7), ui->edit_student->text().trimmed());
    department_model->submitAll();
    department_model->select();
    QMessageBox::information(this, tr("提示"), tr("教学院系修改成功！"));
    ui->list_department->selectRow(rowNum);
}

void DepartmentDialog::btn_delete_clicked()
{
    if (department_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除教学院系"), tr("您是否确定删除当前选中的教学院系信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_department->currentIndex().row(); // 获取选中行
        department_model->removeRow(nIndexRow--);
        department_model->submitAll();
        department_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_department->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_department->selectRow(nIndexRow);
        else
        {
            ui->edit_code->clear();
            ui->combo_name->clearEditText();
            ui->combo_leader->clearEditText();
            ui->edit_location->clear();
            ui->edit_setting->clear();
            ui->edit_teaching->clear();
            ui->edit_researching->clear();
            ui->edit_student->clear();
            ui->btn_delete->setEnabled(false);
        }
    }
}

void DepartmentDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        department_model->setFilter("1");
    else
        department_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    department_model->select();

    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_teaching->clear();
    ui->edit_researching->clear();
    ui->edit_student->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void DepartmentDialog::list_department_changed(const QModelIndex &current)
{
    ui->edit_code->setText(department_model->index(current.row(), 0).data().toString());
    ui->combo_name->setEditText(department_model->index(current.row(), 1).data().toString());
    ui->combo_leader->setEditText(department_model->index(current.row(), 2).data().toString());
    ui->edit_location->setText(department_model->index(current.row(), 3).data().toString());
    ui->edit_setting->setText(department_model->index(current.row(), 4).data().toString());
    ui->edit_teaching->setText(department_model->index(current.row(), 5).data().toString());
    ui->edit_researching->setText(department_model->index(current.row(), 6).data().toString());
    ui->edit_student->setText(department_model->index(current.row(), 7).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
