#include "positioninformationdialog.h"
#include "ui_positioninformationdialog.h"

PositionInformationDialog::PositionInformationDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::PositionInformationDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    positionInf_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    positionInf_model->setTable("position_information");
    positionInf_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    positionInf_model->select();
    positionInf_model->setHeaderData(1, Qt::Horizontal, tr("岗位"));
    positionInf_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (positionInf_model->canFetchMore())
        positionInf_model->fetchMore();

    ui->list_positioninformation->setModel(positionInf_model);
    ui->list_positioninformation->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_positioninformation->setColumnHidden(0, true);
    ui->list_positioninformation->setColumnWidth(0, 60);
    ui->list_positioninformation->setColumnWidth(1, 80);
    ui->list_positioninformation->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

PositionInformationDialog::~PositionInformationDialog()
{
    delete positionInf_model;
    delete ui;
}

void PositionInformationDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("教工岗位"), tr("岗位："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = positionInf_model->query();
        query.prepare("SELECT * FROM `position_information` WHERE `post` = :postname");
        query.bindValue(":postname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加岗位
        if (query.size() > 0)
        {
            // 存在岗位
            QMessageBox::critical(this, tr("添加岗位失败"), tr("该岗位已存在！"));
            return;
        }
        int rowNum = positionInf_model->rowCount();
        positionInf_model->insertRow(rowNum);
        positionInf_model->setData(positionInf_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        positionInf_model->setData(positionInf_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        positionInf_model->submitAll();
        positionInf_model->select();
        QMessageBox::information(this, tr("提示"), tr("岗位添加成功！"));
        ui->list_positioninformation->selectRow(rowNum);
    }
}

void PositionInformationDialog::btn_delete_clicked()
{
    if (positionInf_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除岗位"), tr("您是否确定删除当前选中的岗位信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_positioninformation->currentIndex().row(); // 获取选中行
        positionInf_model->removeRow(nIndexRow--);
        positionInf_model->submitAll();
        positionInf_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_positioninformation->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_positioninformation->selectRow(nIndexRow);
    }
}
