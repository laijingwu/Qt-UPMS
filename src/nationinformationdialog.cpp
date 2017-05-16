#include "nationinformationdialog.h"
#include "ui_nationinformationdialog.h"

NationInformationDialog::NationInformationDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::NationInformationDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    nationInf_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    nationInf_model->setTable("nation_information");
    nationInf_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    nationInf_model->select();
    nationInf_model->setHeaderData(1, Qt::Horizontal, tr("民族"));
    nationInf_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (nationInf_model->canFetchMore())
        nationInf_model->fetchMore();

    ui->list_nationinformation->setModel(nationInf_model);
    ui->list_nationinformation->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_nationinformation->setColumnHidden(0, true);
    ui->list_nationinformation->setColumnWidth(0, 60);
    ui->list_nationinformation->setColumnWidth(1, 80);
    ui->list_nationinformation->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

NationInformationDialog::~NationInformationDialog()
{
    delete nationInf_model;
    delete ui;
}

void NationInformationDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("民族信息"), tr("民族："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = nationInf_model->query();
        query.prepare("SELECT * FROM `nation_information` WHERE `folk` = :folkname");
        query.bindValue(":folkname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加民族
        if (query.size() > 0)
        {
            // 存在民族
            QMessageBox::critical(this, tr("添加民族失败"), tr("该民族已存在！"));
            return;
        }
        int rowNum = nationInf_model->rowCount();
        nationInf_model->insertRow(rowNum);
        nationInf_model->setData(nationInf_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        nationInf_model->setData(nationInf_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        nationInf_model->submitAll();
        nationInf_model->select();
        QMessageBox::information(this, tr("提示"), tr("民族添加成功！"));
        ui->list_nationinformation->selectRow(rowNum);
    }
}

void NationInformationDialog::btn_delete_clicked()
{
    if (nationInf_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除民族"), tr("您是否确定删除当前选中的民族信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_nationinformation->currentIndex().row(); // 获取选中行
        nationInf_model->removeRow(nIndexRow--);
        nationInf_model->submitAll();
        nationInf_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_nationinformation->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_nationinformation->selectRow(nIndexRow);
    }
}
