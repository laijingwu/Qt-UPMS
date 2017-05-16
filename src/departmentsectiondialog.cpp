#include "departmentsectiondialog.h"
#include "ui_departmentsectiondialog.h"

DepartmentSectionDialog::DepartmentSectionDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::DepartmentSectionDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    department_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    department_model->setTable("department_section");
    department_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    department_model->select();
    department_model->setHeaderData(1, Qt::Horizontal, tr("单位"));
    department_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (department_model->canFetchMore())
        department_model->fetchMore();

    ui->list_departmentsection->setModel(department_model);
    ui->list_departmentsection->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_departmentsection->setColumnHidden(0, true);
    ui->list_departmentsection->setColumnWidth(0, 60);
    ui->list_departmentsection->setColumnWidth(1, 80);
    ui->list_departmentsection->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

DepartmentSectionDialog::~DepartmentSectionDialog()
{
    delete department_model;
    delete ui;
}

void DepartmentSectionDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("院系部门"), tr("单位："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = department_model->query();
        query.prepare("SELECT * FROM `department_section` WHERE `division` = :divisionname");
        query.bindValue(":divisionname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加院系部门
        if (query.size() > 0)
        {
            // 存在院系部门
            QMessageBox::critical(this, tr("添加院系部门失败"), tr("该院系部门已存在！"));
            return;
        }
        int rowNum = department_model->rowCount();
        department_model->insertRow(rowNum);
        department_model->setData(department_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        department_model->setData(department_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        department_model->submitAll();
        department_model->select();
        QMessageBox::information(this, tr("提示"), tr("院系部门添加成功！"));
        ui->list_departmentsection->selectRow(rowNum);
    }
}

void DepartmentSectionDialog::btn_delete_clicked()
{
    if (department_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除院系部门"), tr("您是否确定删除当前选中的院系部门信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_departmentsection->currentIndex().row(); // 获取选中行
        department_model->removeRow(nIndexRow--);
        department_model->submitAll();
        department_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_departmentsection->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_departmentsection->selectRow(nIndexRow);
    }
}
