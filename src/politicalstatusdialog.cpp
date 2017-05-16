#include "politicalstatusdialog.h"
#include "ui_politicalstatusdialog.h"

PoliticalStatusDialog::PoliticalStatusDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::PoliticalStatusDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    political_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    political_model->setTable("political_status");
    political_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    political_model->select();
    political_model->setHeaderData(1, Qt::Horizontal, tr("党派"));
    political_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (political_model->canFetchMore())
        political_model->fetchMore();

    ui->list_politicalstatus->setModel(political_model);
    ui->list_politicalstatus->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_politicalstatus->setColumnHidden(0, true);
    ui->list_politicalstatus->setColumnWidth(0, 60);
    ui->list_politicalstatus->setColumnWidth(1, 80);
    ui->list_politicalstatus->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

PoliticalStatusDialog::~PoliticalStatusDialog()
{
    delete political_model;
    delete ui;
}

void PoliticalStatusDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("政治面貌"), tr("党派："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = political_model->query();
        query.prepare("SELECT * FROM `political_status` WHERE `party` = :partyname");
        query.bindValue(":partyname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加党派
        if (query.size() > 0)
        {
            // 存在党派
            QMessageBox::critical(this, tr("添加党派失败"), tr("该党派已存在！"));
            return;
        }
        int rowNum = political_model->rowCount();
        political_model->insertRow(rowNum);
        political_model->setData(political_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        political_model->setData(political_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        political_model->submitAll();
        political_model->select();
        QMessageBox::information(this, tr("提示"), tr("党派添加成功！"));
        ui->list_politicalstatus->selectRow(rowNum);
    }
}

void PoliticalStatusDialog::btn_delete_clicked()
{
    if (political_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除党派"), tr("您是否确定删除当前选中的党派信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_politicalstatus->currentIndex().row(); // 获取选中行
        political_model->removeRow(nIndexRow--);
        political_model->submitAll();
        political_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_politicalstatus->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_politicalstatus->selectRow(nIndexRow);
    }
}
