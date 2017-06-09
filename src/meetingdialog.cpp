#include "meetingdialog.h"
#include "ui_meetingdialog.h"

MeetingDialog::MeetingDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::MeetingDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    meeting_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    meeting_model->setTable("meeting_record");
    meeting_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    meeting_model->select();
    meeting_model->setHeaderData(1, Qt::Horizontal, tr("会议时间"));
    meeting_model->setHeaderData(2, Qt::Horizontal, tr("会议地点"));
    meeting_model->setHeaderData(3, Qt::Horizontal, tr("会议类别"));
    meeting_model->setHeaderData(4, Qt::Horizontal, tr("会议主题"));
    meeting_model->setHeaderData(5, Qt::Horizontal, tr("举办单位"));
    meeting_model->setHeaderData(6, Qt::Horizontal, tr("参与单位"));
    meeting_model->setHeaderData(7, Qt::Horizontal, tr("主持人"));
    meeting_model->setHeaderData(8, Qt::Horizontal, tr("记录人"));
    meeting_model->setHeaderData(9, Qt::Horizontal, tr("会议内容"));
    meeting_model->setHeaderData(10, Qt::Horizontal, tr("备注"));
    while (meeting_model->canFetchMore())
        meeting_model->fetchMore();

    ui->list_record->setModel(meeting_model);
    ui->list_record->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_record->setColumnWidth(0, 90);
    ui->list_record->setColumnHidden(0, true);

    ui->combo_condition->addItem(tr("按会议类别查询"), "sort");
    ui->combo_condition->addItem(tr("按会议主题查询"), "subject");
    ui->combo_condition->addItem(tr("按会议地点查询"), "location");
    ui->combo_condition->addItem(tr("按举办单位查询"), "host_unit");
    ui->combo_condition->addItem(tr("按主持人查询"), "host");

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    connect(ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(btn_cancel_clicked()));
    connect(ui->list_record->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_meeting_changed(const QModelIndex &)));
    connect(ui->list_record, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_meeting_changed(const QModelIndex &)));
    ui->list_record->selectRow(0);
}

MeetingDialog::~MeetingDialog()
{
    delete meeting_model;
    delete ui;
}

void MeetingDialog::btn_add_clicked()
{
    // 清空所有输入框数据
    ui->date_mdate->setDate(QDate::currentDate());
    ui->edit_location->clear();
    ui->combo_sort->setCurrentIndex(0);
    ui->edit_subject->clear();
    ui->edit_host_unit->clear();
    ui->edit_participate_unit->clear();
    ui->edit_host->clear();
    ui->edit_writer->clear();
    ui->edit_meetingcontent->clear();
    ui->edit_remark->clear();
    ui->btn_save->setEnabled(true);
    ui->btn_cancel->setEnabled(true);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void MeetingDialog::btn_save_clicked()
{
    if (ui->edit_subject->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存会议记录"), tr("会议主题不能为空，请输入！"));
        ui->edit_subject->setFocus();
        return;
    }

    int rowNum = meeting_model->rowCount();
    meeting_model->insertRow(rowNum);
    meeting_model->setData(meeting_model->index(rowNum, 0), 0);
    meeting_model->setData(meeting_model->index(rowNum, 1), ui->date_mdate->date().toString("yyyy-MM-dd"));
    meeting_model->setData(meeting_model->index(rowNum, 2), ui->edit_location->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 3), ui->combo_sort->currentText().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 4), ui->edit_subject->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 5), ui->edit_host_unit->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 6), ui->edit_participate_unit->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 7), ui->edit_host->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 8), ui->edit_writer->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 9), ui->edit_meetingcontent->toPlainText().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 10), ui->edit_remark->text().trimmed());
    meeting_model->submitAll();
    meeting_model->select();
    QMessageBox::information(this, tr("提示"), tr("会议记录添加成功！"));
    ui->list_record->selectRow(rowNum);
}

void MeetingDialog::btn_update_clicked()
{
    if (ui->edit_subject->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("保存会议记录"), tr("会议主题不能为空，请输入！"));
        ui->edit_subject->setFocus();
        return;
    }

    QSqlQuery query = meeting_model->query();
    query.prepare("SELECT * FROM `meeting_record` WHERE `number` = :number");
    query.bindValue(":number", meeting_model->index(ui->list_record->currentIndex().row(), 0).data().toString());
    query.exec();

    // 修改会议记录
    if (query.size() == 0)
    {
        // 不存在会议记录
        QMessageBox::critical(this, tr("修改会议记录失败"), tr("该会议记录不存在！"));
        return;
    }
    int rowNum = ui->list_record->currentIndex().row(); // 获取选中行
    meeting_model->setData(meeting_model->index(rowNum, 1), ui->date_mdate->date().toString("yyyy-MM-dd"));
    meeting_model->setData(meeting_model->index(rowNum, 2), ui->edit_location->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 3), ui->combo_sort->currentText().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 4), ui->edit_subject->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 5), ui->edit_host_unit->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 6), ui->edit_participate_unit->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 7), ui->edit_host->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 8), ui->edit_writer->text().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 9), ui->edit_meetingcontent->toPlainText().trimmed());
    meeting_model->setData(meeting_model->index(rowNum, 10), ui->edit_remark->text().trimmed());
    meeting_model->submitAll();
    meeting_model->select();
    QMessageBox::information(this, tr("提示"), tr("会议记录修改成功！"));
    ui->list_record->selectRow(rowNum);
}

void MeetingDialog::btn_delete_clicked()
{
    if (meeting_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除会议记录"), tr("您是否确定删除当前选中的会议记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_record->currentIndex().row(); // 获取选中行
        meeting_model->removeRow(nIndexRow--);
        meeting_model->submitAll();
        meeting_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_record->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_record->selectRow(nIndexRow);
        else
        {
            ui->btn_cancel->click();
        }
    }
}

void MeetingDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        meeting_model->setFilter("1");
    else
        meeting_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    meeting_model->select();
    ui->list_record->selectRow(0);
}

void MeetingDialog::btn_cancel_clicked()
{
    ui->date_mdate->setDate(QDate::currentDate());
    ui->edit_location->clear();
    ui->combo_sort->setCurrentIndex(0);
    ui->edit_subject->clear();
    ui->edit_host_unit->clear();
    ui->edit_participate_unit->clear();
    ui->edit_host->clear();
    ui->edit_writer->clear();
    ui->edit_meetingcontent->clear();
    ui->edit_remark->clear();
    ui->btn_save->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
    ui->btn_update->setEnabled(false);
    ui->btn_delete->setEnabled(false);
}

void MeetingDialog::list_meeting_changed(const QModelIndex &current)
{
    ui->date_mdate->setDate(QDate::fromString(meeting_model->index(current.row(), 1).data().toString(), "yyyy-MM-dd"));
    ui->edit_location->setText(meeting_model->index(current.row(), 2).data().toString());
    ui->combo_sort->setCurrentText(meeting_model->index(current.row(), 3).data().toString());
    ui->edit_subject->setText(meeting_model->index(current.row(), 4).data().toString());
    ui->edit_host_unit->setText(meeting_model->index(current.row(), 5).data().toString());
    ui->edit_participate_unit->setText(meeting_model->index(current.row(), 6).data().toString());
    ui->edit_host->setText(meeting_model->index(current.row(), 7).data().toString());
    ui->edit_writer->setText(meeting_model->index(current.row(), 8).data().toString());
    ui->edit_meetingcontent->setPlainText(meeting_model->index(current.row(), 9).data().toString());
    ui->edit_remark->setText(meeting_model->index(current.row(), 10).data().toString());
    ui->btn_save->setEnabled(false);
    ui->btn_cancel->setEnabled(false);
    ui->btn_update->setEnabled(true);
    ui->btn_delete->setEnabled(true);
}
