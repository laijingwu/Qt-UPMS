#include "administrativepostdialog.h"
#include "ui_administrativepostdialog.h"

AdministrativePostDialog::AdministrativePostDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AdministrativePostDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    administrative_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    administrative_model->setTable("administrative_post");
    administrative_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    administrative_model->select();
    administrative_model->setHeaderData(1, Qt::Horizontal, tr("职务"));
    administrative_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (administrative_model->canFetchMore())
        administrative_model->fetchMore();

    ui->list_administrativepost->setModel(administrative_model);
    ui->list_administrativepost->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_administrativepost->setColumnHidden(0, true);
    ui->list_administrativepost->setColumnWidth(0, 60);
    ui->list_administrativepost->setColumnWidth(1, 80);
    ui->list_administrativepost->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

AdministrativePostDialog::~AdministrativePostDialog()
{
    delete administrative_model;
    delete ui;
}

void AdministrativePostDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("行政职务"), tr("职务："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = administrative_model->query();
        query.prepare("SELECT * FROM `administrative_post` WHERE `rank` = :rankname");
        query.bindValue(":rankname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加行政职务
        if (query.size() > 0)
        {
            // 存在行政职务
            QMessageBox::critical(this, tr("添加行政职务失败"), tr("该行政职务已存在！"));
            return;
        }
        int rowNum = administrative_model->rowCount();
        administrative_model->insertRow(rowNum);
        administrative_model->setData(administrative_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        administrative_model->setData(administrative_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        administrative_model->submitAll();
        administrative_model->select();
        QMessageBox::information(this, tr("提示"), tr("行政职务添加成功！"));
        ui->list_administrativepost->selectRow(rowNum);
    }
}

void AdministrativePostDialog::btn_delete_clicked()
{
    if (administrative_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除行政职务"), tr("您是否确定删除当前选中的行政职务信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_administrativepost->currentIndex().row(); // 获取选中行
        administrative_model->removeRow(nIndexRow--);
        administrative_model->submitAll();
        administrative_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_administrativepost->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_administrativepost->selectRow(nIndexRow);
    }
}
