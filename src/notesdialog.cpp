#include "notesdialog.h"
#include "ui_notesdialog.h"

NotesDialog::NotesDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::NotesDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    /***** 记事本列表 Start *****/
    notes_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    notes_model->setTable("notes");
    notes_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    notes_model->select();

    notes_model->setHeaderData(1, Qt::Horizontal, tr("事件标题"));
    notes_model->setHeaderData(2, Qt::Horizontal, tr("记录日期"));
    notes_model->setHeaderData(3, Qt::Horizontal, tr("事件类型"));
    notes_model->setHeaderData(4, Qt::Horizontal, tr("记录人"));
    while (notes_model->canFetchMore())
        notes_model->fetchMore();

    ui->list_notes->setModel(notes_model);
    ui->list_notes->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_notes->setColumnWidth(0, 90);
    ui->list_notes->setColumnHidden(0, true);
    ui->list_notes->setColumnHidden(5, true);
    ui->list_notes->setColumnHidden(6, true);

    connect(ui->list_notes->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_notes_changed(const QModelIndex &)));
    connect(ui->list_notes, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_notes_changed(const QModelIndex &)));
    /***** 记事本列表 End *****/

    /***** 查询模块 *****/
    ui->combo_condition->addItem(tr("按事件类型查询"), "type");
    ui->combo_condition->addItem(tr("按事件标题查询"), "title");
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    /***** 查询模块 End *****/

    /***** 事件类型模块 *****/
    QSqlQuery type_query("SELECT type FROM `notes`", SqlController->getDefaultConnection());
    while (type_query.next())
        ui->combo_type->addItem(type_query.value(0).toString());
    ui->combo_type->setCurrentText("");
    /***** 事件类型模块 End *****/

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(btn_cancel_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));

    ui->list_notes->selectRow(0);
}

NotesDialog::~NotesDialog()
{
    delete notes_model;
    delete ui;
}

void NotesDialog::btn_add_clicked()
{
    ui->edit_writer->clear();
    ui->edit_unit->clear();
    ui->date_tm->setDate(QDate::currentDate());
    ui->combo_type->setCurrentText("");
    ui->edit_title->clear();
    ui->edit_content->clear();
    ui->btn_delete->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_save->setEnabled(true);
    ui->btn_cancel->setEnabled(true);
}

void NotesDialog::btn_cancel_clicked()
{
    emit ui->list_notes->clicked(ui->list_notes->currentIndex());
}

void NotesDialog::btn_delete_clicked()
{
    if (notes_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除事件"), tr("您是否确定删除当前选中的事件记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_notes->currentIndex().row(); // 获取选中行
        notes_model->removeRow(nIndexRow--);
        notes_model->submitAll();
        notes_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_notes->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_notes->selectRow(nIndexRow);
        else
        {
            ui->btn_add->click();
        }
    }
}

void NotesDialog::btn_save_clicked()
{
    // 判断标题和内容非空
    if (ui->edit_title->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("添加事件"), tr("请输入事件标题！"));
        ui->edit_title->setFocus();
        return;
    }
    if (ui->edit_content->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("添加事件"), tr("请输入事件内容！"));
        ui->edit_content->setFocus();
        return;
    }

    int rowNum = notes_model->rowCount();
    notes_model->insertRow(rowNum);
    notes_model->setData(notes_model->index(rowNum, 0), 0);
    notes_model->setData(notes_model->index(rowNum, 1), ui->edit_title->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 2), ui->date_tm->date().toString("yyyy-MM-dd"));
    notes_model->setData(notes_model->index(rowNum, 3), ui->combo_type->currentText().trimmed());
    notes_model->setData(notes_model->index(rowNum, 4), ui->edit_writer->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 5), ui->edit_unit->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 6), ui->edit_content->toPlainText().trimmed());
    notes_model->submitAll();
    notes_model->select();
    QMessageBox::information(this, tr("提示"), tr("事件添加成功！"));
    ui->list_notes->selectRow(rowNum);
}

void NotesDialog::btn_select_clicked()
{
    if (ui->edit_select_content->text().trimmed().isEmpty())
        notes_model->setFilter("1");
    else
        notes_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_select_content->text().trimmed()));
    notes_model->select();

    if (notes_model->rowCount() > 0) // 非空列表
        ui->list_notes->selectRow(0);
    else
        ui->btn_add->click();
}

void NotesDialog::btn_update_clicked()
{
    // 判断标题和内容非空
    if (ui->edit_title->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改事件"), tr("请输入事件标题！"));
        ui->edit_title->setFocus();
        return;
    }
    if (ui->edit_content->toPlainText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改事件"), tr("请输入事件内容！"));
        ui->edit_content->setFocus();
        return;
    }

    QSqlQuery query = notes_model->query();
    query.prepare("SELECT * FROM `notes` WHERE `number` = :number");
    query.bindValue(":number", notes_model->index(ui->list_notes->currentIndex().row(), 0).data().toString());
    query.exec();

    // 修改事件
    if (query.size() == 0)
    {
        // 不存在事件
        QMessageBox::critical(this, tr("修改事件记录失败"), tr("该事件不存在！"));
        return;
    }
    int rowNum = ui->list_notes->currentIndex().row(); // 获取选中行
    notes_model->setData(notes_model->index(rowNum, 1), ui->edit_title->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 2), ui->date_tm->date().toString("yyyy-MM-dd"));
    notes_model->setData(notes_model->index(rowNum, 3), ui->combo_type->currentText().trimmed());
    notes_model->setData(notes_model->index(rowNum, 4), ui->edit_writer->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 5), ui->edit_unit->text().trimmed());
    notes_model->setData(notes_model->index(rowNum, 6), ui->edit_content->toPlainText().trimmed());
    notes_model->submitAll();
    notes_model->select();
    QMessageBox::information(this, tr("提示"), tr("事件修改成功！"));
    ui->list_notes->selectRow(rowNum);
}

void NotesDialog::list_notes_changed(const QModelIndex &current)
{
    ui->edit_writer->setText(notes_model->index(current.row(), 4).data().toString());
    ui->edit_unit->setText(notes_model->index(current.row(), 5).data().toString());
    ui->date_tm->setDate(QDate::fromString(notes_model->index(current.row(), 2).data().toString(), "yyyy-MM-dd"));
    ui->combo_type->setCurrentText(notes_model->index(current.row(), 3).data().toString());
    ui->edit_title->setText(notes_model->index(current.row(), 1).data().toString());
    ui->edit_content->setPlainText(notes_model->index(current.row(), 6).data().toString());
    ui->btn_delete->setEnabled(true);
    ui->btn_update->setEnabled(true);
    ui->btn_save->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
}
