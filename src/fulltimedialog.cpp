#include "fulltimedialog.h"
#include "ui_fulltimedialog.h"

FulltimeDialog::FulltimeDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::FulltimeDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    fulltime_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    fulltime_model->setTable("fulltime");
    fulltime_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    fulltime_model->select();
    fulltime_model->setHeaderData(0, Qt::Horizontal, tr("机构编码"));
    fulltime_model->setHeaderData(1, Qt::Horizontal, tr("机构名称"));
    fulltime_model->setHeaderData(2, Qt::Horizontal, tr("机构领导"));
    fulltime_model->setHeaderData(3, Qt::Horizontal, tr("办公地点"));
    fulltime_model->setHeaderData(4, Qt::Horizontal, tr("机构设置"));
    fulltime_model->setHeaderData(5, Qt::Horizontal, tr("科研成果"));
    while (fulltime_model->canFetchMore())
        fulltime_model->fetchMore();

    ui->list_fulltime->setModel(fulltime_model);
    ui->list_fulltime->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->combo_condition->addItem(tr("按机构编码查询"), "code");
    ui->combo_condition->addItem(tr("按机构名称查询"), "name");
    ui->combo_condition->addItem(tr("按机构领导查询"), "leader");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->list_fulltime->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_fulltime_changed(const QModelIndex &)));
    connect(ui->list_fulltime, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_fulltime_changed(const QModelIndex &)));
    ui->list_fulltime->selectRow(0);
}

FulltimeDialog::~FulltimeDialog()
{
    delete fulltime_model;
    delete ui;
}

void FulltimeDialog::btn_add_clicked()
{
    // 清空专职机构详细信息中的所有数据
    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_fruit->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void FulltimeDialog::btn_save_clicked()
{
    // fulltime 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存专职机构"), tr("请输入机构编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存专职机构"), tr("请输入机构名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存专职机构"), tr("请输入机构领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = fulltime_model->query();
    query.prepare("SELECT * FROM `fulltime` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 新增专职机构
    if (query.size() > 0)
    {
        // 存在专职机构
        QMessageBox::critical(this, tr("新增专职机构失败"), tr("该机构编码已存在！"));
        return;
    }
    int rowNum = fulltime_model->rowCount();
    fulltime_model->insertRow(rowNum);
    fulltime_model->setData(fulltime_model->index(rowNum, 0), ui->edit_code->text().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 5), ui->edit_fruit->toPlainText().trimmed());
    fulltime_model->submitAll();
    fulltime_model->select();
    QMessageBox::information(this, tr("提示"), tr("专职机构添加成功！"));
    ui->list_fulltime->selectRow(rowNum);
}

void FulltimeDialog::btn_update_clicked()
{
    // fulltime 表三个非空属性
    if (ui->edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改专职机构"), tr("请输入机构编码！"));
        ui->edit_code->setFocus();
        return;
    }
    if (ui->combo_name->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改专职机构"), tr("请输入机构名称！"));
        ui->combo_name->setFocus();
        return;
    }
    if (ui->combo_leader->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改专职机构"), tr("请输入机构领导！"));
        ui->combo_leader->setFocus();
        return;
    }

    QSqlQuery query = fulltime_model->query();
    query.prepare("SELECT * FROM `fulltime` WHERE `code` = :code");
    query.bindValue(":code", ui->edit_code->text().trimmed());
    query.exec();

    // 修改专职机构
    if (query.size() == 0)
    {
        // 不存在专职机构
        QMessageBox::critical(this, tr("修改专职机构失败"), tr("该机构编码不存在！"));
        return;
    }
    int rowNum = ui->list_fulltime->currentIndex().row(); // 获取选中行
    fulltime_model->setData(fulltime_model->index(rowNum, 1), ui->combo_name->currentText().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 2), ui->combo_leader->currentText().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 3), ui->edit_location->text().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 4), ui->edit_setting->text().trimmed());
    fulltime_model->setData(fulltime_model->index(rowNum, 5), ui->edit_fruit->toPlainText().trimmed());
    fulltime_model->submitAll();
    fulltime_model->select();
    QMessageBox::information(this, tr("提示"), tr("专职机构修改成功！"));
    ui->list_fulltime->selectRow(rowNum);
}

void FulltimeDialog::btn_delete_clicked()
{
    if (fulltime_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除专职机构"), tr("您是否确定删除当前选中的专职机构信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_fulltime->currentIndex().row(); // 获取选中行
        fulltime_model->removeRow(nIndexRow--);
        fulltime_model->submitAll();
        fulltime_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_fulltime->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_fulltime->selectRow(nIndexRow);
        else
        {
            ui->edit_code->clear();
            ui->combo_name->clearEditText();
            ui->combo_leader->clearEditText();
            ui->edit_location->clear();
            ui->edit_setting->clear();
            ui->edit_fruit->clear();
            ui->btn_delete->setEnabled(false);
        }
    }
}

void FulltimeDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        fulltime_model->setFilter("1");
    else
        fulltime_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    fulltime_model->select();

    ui->edit_code->clear();
    ui->combo_name->clearEditText();
    ui->combo_leader->clearEditText();
    ui->edit_location->clear();
    ui->edit_setting->clear();
    ui->edit_fruit->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void FulltimeDialog::list_fulltime_changed(const QModelIndex &current)
{
    ui->edit_code->setText(fulltime_model->index(current.row(), 0).data().toString());
    ui->combo_name->setEditText(fulltime_model->index(current.row(), 1).data().toString());
    ui->combo_leader->setEditText(fulltime_model->index(current.row(), 2).data().toString());
    ui->edit_location->setText(fulltime_model->index(current.row(), 3).data().toString());
    ui->edit_setting->setText(fulltime_model->index(current.row(), 4).data().toString());
    ui->edit_fruit->setPlainText(fulltime_model->index(current.row(), 5).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
