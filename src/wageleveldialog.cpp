#include "wageleveldialog.h"
#include "ui_wageleveldialog.h"

WageLevelDialog::WageLevelDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::WageLevelDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    wage_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    wage_model->setTable("wage_level");
    wage_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    wage_model->select();
    wage_model->setHeaderData(1, Qt::Horizontal, tr("等级"));
    wage_model->setHeaderData(2, Qt::Horizontal, tr("描述"));
    while (wage_model->canFetchMore())
        wage_model->fetchMore();

    ui->list_wagelevel->setModel(wage_model);
    ui->list_wagelevel->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_wagelevel->setColumnHidden(0, true);
    ui->list_wagelevel->setColumnWidth(0, 60);
    ui->list_wagelevel->setColumnWidth(1, 80);
    ui->list_wagelevel->selectRow(0);

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
}

WageLevelDialog::~WageLevelDialog()
{
    delete wage_model;
    delete ui;
}

void WageLevelDialog::btn_add_clicked()
{
    AddBaseInfoDialog dialog_addbaseinfo(this);
    dialog_addbaseinfo.setStaticUi(tr("工资等级"), tr("等级："));
    if (dialog_addbaseinfo.exec() == QDialog::Accepted)
    {
        QSqlQuery query = wage_model->query();
        query.prepare("SELECT * FROM `wage_level` WHERE `salary` = :salaryname");
        query.bindValue(":salaryname", dialog_addbaseinfo.getStaticText());
        query.exec();

        // 添加工资等级
        if (query.size() > 0)
        {
            // 存在工资等级
            QMessageBox::critical(this, tr("添加工资等级失败"), tr("该工资等级已存在！"));
            return;
        }
        int rowNum = wage_model->rowCount();
        wage_model->insertRow(rowNum);
        wage_model->setData(wage_model->index(rowNum, 1), dialog_addbaseinfo.getStaticText());
        wage_model->setData(wage_model->index(rowNum, 2), dialog_addbaseinfo.getDescriptionText());
        wage_model->submitAll();
        wage_model->select();
        QMessageBox::information(this, tr("提示"), tr("工资等级添加成功！"));
        ui->list_wagelevel->selectRow(rowNum);
    }
}

void WageLevelDialog::btn_delete_clicked()
{
    if (wage_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除工资等级"), tr("您是否确定删除当前选中的工资等级信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_wagelevel->currentIndex().row(); // 获取选中行
        wage_model->removeRow(nIndexRow--);
        wage_model->submitAll();
        wage_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_wagelevel->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_wagelevel->selectRow(nIndexRow);
    }
}
