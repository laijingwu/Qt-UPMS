#include "educationdegreedialog.h"
#include "ui_educationdegreedialog.h"

EducationDegreeDialog::EducationDegreeDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::EducationDegreeDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    education_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    education_model->setTable("education_degree");
    education_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    education_model->select();
    education_model->setHeaderData(1, Qt::Horizontal, tr("学历"));
    education_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (education_model->canFetchMore())
        education_model->fetchMore();

    ui->list_education->setModel(education_model);
    ui->list_education->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_education->setColumnHidden(0, true);
    ui->list_education->setColumnWidth(0, 60);
    ui->list_education->setColumnWidth(1, 80);
    ui->list_education->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

EducationDegreeDialog::~EducationDegreeDialog()
{
    delete education_model;
    delete ui;
}

void EducationDegreeDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("文化程度"), tr("学历："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = education_model->query();
        query.prepare("SELECT * FROM `education_degree` WHERE `degree` = :degreename");
        query.bindValue(":degreename", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加学历
        if (query.size() > 0)
        {
            // 存在学历
            QMessageBox::critical(this, tr("添加学历失败"), tr("该学历已存在！"));
            return;
        }
        int rowNum = education_model->rowCount();
        education_model->insertRow(rowNum);
        education_model->setData(education_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        education_model->setData(education_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        education_model->submitAll();
        education_model->select();
        QMessageBox::information(this, tr("提示"), tr("学历添加成功！"));
        ui->list_education->selectRow(rowNum);
    }
}

void EducationDegreeDialog::btn_delete_clicked()
{
    if (education_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除学历"), tr("您是否确定删除当前选中的学历信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_education->currentIndex().row(); // 获取选中行
        education_model->removeRow(nIndexRow--);
        education_model->submitAll();
        education_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_education->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_education->selectRow(nIndexRow);
    }
}
