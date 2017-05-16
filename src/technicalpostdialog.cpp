#include "technicalpostdialog.h"
#include "ui_technicalpostdialog.h"

TechnicalPostDialog::TechnicalPostDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::TechnicalPostDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    technical_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    technical_model->setTable("technical_post");
    technical_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    technical_model->select();
    technical_model->setHeaderData(1, Qt::Horizontal, tr("职称"));
    technical_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (technical_model->canFetchMore())
        technical_model->fetchMore();

    ui->list_technicalpost->setModel(technical_model);
    ui->list_technicalpost->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_technicalpost->setColumnHidden(0, true);
    ui->list_technicalpost->setColumnWidth(0, 60);
    ui->list_technicalpost->setColumnWidth(1, 80);
    ui->list_technicalpost->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

TechnicalPostDialog::~TechnicalPostDialog()
{
    delete technical_model;
    delete ui;
}

void TechnicalPostDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("职称级别"), tr("职称："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = technical_model->query();
        query.prepare("SELECT * FROM `technical_post` WHERE `title` = :titlename");
        query.bindValue(":titlename", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加职称级别
        if (query.size() > 0)
        {
            // 存在职称级别
            QMessageBox::critical(this, tr("添加职称级别失败"), tr("该职称级别已存在！"));
            return;
        }
        int rowNum = technical_model->rowCount();
        technical_model->insertRow(rowNum);
        technical_model->setData(technical_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        technical_model->setData(technical_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        technical_model->submitAll();
        technical_model->select();
        QMessageBox::information(this, tr("提示"), tr("职称级别添加成功！"));
        ui->list_technicalpost->selectRow(rowNum);
    }
}

void TechnicalPostDialog::btn_delete_clicked()
{
    if (technical_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除职称级别"), tr("您是否确定删除当前选中的职称级别信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_technicalpost->currentIndex().row(); // 获取选中行
        technical_model->removeRow(nIndexRow--);
        technical_model->submitAll();
        technical_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_technicalpost->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_technicalpost->selectRow(nIndexRow);
    }
}
