#include "humanbrowsedialog.h"
#include "ui_humanbrowsedialog.h"

HumanBrowseDialog::HumanBrowseDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::HumanBrowseDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);
    tab_base_archive_init(); // 加载默认tab

    /***** 教工列表 Start *****/
    staff_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    staff_model->setTable("staff");
    staff_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    staff_model->select();
    staff_model->setHeaderData(0, Qt::Horizontal, tr("工号"));
    staff_model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    while (staff_model->canFetchMore())
        staff_model->fetchMore();

    ui->list_staff->setModel(staff_model);
    ui->list_staff->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_staff->setColumnWidth(0, 80);
    for (int i = 2; i < 25; i++) // 隐藏列
        ui->list_staff->setColumnHidden(i, true);

    connect(ui->list_staff->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_staff_changed(const QModelIndex &)));
    connect(ui->list_staff, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_staff_changed(const QModelIndex &)));
    /***** 教工列表 End *****/

    tab_family_member_init(); // 家庭成员信息
    tab_work_experience_init(); // 工作经历信息
    tab_reward_init(); // 奖励记录
    tab_punish_init(); // 违纪处罚
    tab_paper_init(); // 论文成果
    tab_project_init(); // 项目成果

    ui->list_staff->selectRow(0);
}

HumanBrowseDialog::~HumanBrowseDialog()
{
    delete staff_model;
    delete family_model;
    delete workexp_model;
    delete reward_model;
    delete punish_model;
    delete paper_model;
    delete project_model;
    delete ui;
}

void HumanBrowseDialog::list_staff_changed(const QModelIndex &current)
{
    ui->edit_current_name->setText(staff_model->index(current.row(), 1).data().toString());
    ui->edit_current_code->setText(staff_model->index(current.row(), 0).data().toString());
    ui->base_edit_code->setText(staff_model->index(current.row(), 0).data().toString());
    ui->base_edit_name->setText(staff_model->index(current.row(), 1).data().toString());
    ui->base_combo_sex->setCurrentText(staff_model->index(current.row(), 2).data().toString());
    ui->base_date_birth->setDate(QDate::fromString(staff_model->index(current.row(), 3).data().toString(), "yyyy-MM-dd"));
    ui->base_combo_marriage->setCurrentText(staff_model->index(current.row(), 4).data().toString());
    ui->base_edit_age->setText(staff_model->index(current.row(), 5).data().toString());
    ui->base_combo_party->setCurrentText(staff_model->index(current.row(), 6).data().toString());
    ui->base_combo_race->setCurrentText(staff_model->index(current.row(), 7).data().toString());
    ui->base_edit_idcard->setText(staff_model->index(current.row(), 8).data().toString());
    ui->base_date_workdate->setDate(QDate::fromString(staff_model->index(current.row(), 9).data().toString(), "yyyy-MM-dd"));
    ui->base_combo_post->setCurrentText(staff_model->index(current.row(), 10).data().toString());
    ui->base_edit_native->setText(staff_model->index(current.row(), 11).data().toString());
    ui->base_date_entrydate->setDate(QDate::fromString(staff_model->index(current.row(), 12).data().toString(), "yyyy-MM-dd"));
    ui->base_combo_salary->setCurrentText(staff_model->index(current.row(), 13).data().toString());
    ui->base_combo_degree->setCurrentText(staff_model->index(current.row(), 14).data().toString());
    ui->base_combo_department->setCurrentText(staff_model->index(current.row(), 15).data().toString());
    ui->base_combo_technical->setCurrentText(staff_model->index(current.row(), 16).data().toString());
    ui->base_combo_job->setCurrentText(staff_model->index(current.row(), 17).data().toString());
    ui->base_edit_officephone->setText(staff_model->index(current.row(), 18).data().toString());
    ui->base_edit_mobilephone->setText(staff_model->index(current.row(), 19).data().toString());
    ui->base_edit_email->setText(staff_model->index(current.row(), 20).data().toString());
    ui->base_edit_university->setText(staff_model->index(current.row(), 21).data().toString());
    ui->base_edit_subject->setText(staff_model->index(current.row(), 22).data().toString());
    ui->base_date_graduatedate->setDate(QDate::fromString(staff_model->index(current.row(), 23).data().toString(), "yyyy-MM-dd"));
    ui->base_edit_address->setText(staff_model->index(current.row(), 24).data().toString());
    ui->btn_total_delete->setEnabled(true);
    ui->btn_total_update->setEnabled(true);
    ui->btn_total_save->setEnabled(false);
    ui->btn_total_cancel->setEnabled(false);
    ui->base_edit_code->setEnabled(false);

    // 家庭成员信息
    // 清空成员信息编辑框
    ui->family_edit_name->clear();
    ui->family_edit_idcard->clear();
    ui->family_edit_relation->clear();
    ui->family_date_birth->setDate(QDate::currentDate());
    ui->family_edit_company->clear();
    ui->family_edit_phone->clear();
    ui->family_edit_duty->clear();
    ui->family_combo_political->setCurrentIndex(0);
    ui->family_edit_education->clear();
    ui->btn_family_delete->setEnabled(false);
    ui->btn_family_update->setEnabled(false);
    ui->btn_family_save->setEnabled(false);
    // 重新绑定过滤器
    family_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    family_model->select();

    // 工作经历信息
    // 清空工作经历信息编辑框
    ui->workexp_date_begin->setDate(QDate::currentDate());
    ui->workexp_date_end->setDate(QDate::currentDate());
    ui->workexp_edit_company->clear();
    ui->workexp_edit_department->clear();
    ui->workexp_edit_duty->clear();
    ui->workexp_edit_technical->clear();
    ui->workexp_combo_education->setCurrentIndex(0);
    ui->workexp_edit_location->clear();
    ui->workexp_edit_proof->clear();
    ui->btn_workexp_delete->setEnabled(false);
    ui->btn_workexp_update->setEnabled(false);
    ui->btn_workexp_save->setEnabled(false);
    // 重新绑定过滤器
    workexp_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    workexp_model->select();

    // 奖励记录
    // 清空奖励记录编辑框
    ui->reward_edit_title->clear();
    ui->reward_combo_sort->setCurrentIndex(0);
    ui->reward_combo_level->setCurrentIndex(0);
    ui->reward_edit_unit->clear();
    ui->reward_combo_category->setCurrentIndex(0);
    ui->reward_date->setDate(QDate::currentDate());
    ui->btn_reward_delete->setEnabled(false);
    ui->btn_reward_update->setEnabled(false);
    ui->btn_reward_save->setEnabled(false);
    // 重新绑定过滤器
    reward_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    reward_model->select();

    // 违纪处罚
    // 清空违纪处罚记录编辑框
    ui->punish_combo_sort->setCurrentIndex(0);
    ui->punish_combo_period->setCurrentIndex(0);
    ui->punish_edit_preason->clear();
    ui->punish_date_pdate->setDate(QDate::currentDate());
    ui->punish_date_ddate->setDate(QDate::currentDate());
    ui->punish_edit_dreason->clear();
    ui->punish_edit_unit->clear();
    ui->btn_punish_delete->setEnabled(false);
    ui->btn_punish_update->setEnabled(false);
    ui->btn_punish_save->setEnabled(false);
    // 重新绑定过滤器
    punish_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    punish_model->select();

    // 论文成果
    // 清空论文成果记录编辑框
    ui->paper_edit_title->clear();
    ui->paper_combo_rank->setCurrentIndex(0);
    ui->paper_edit_publication->clear();
    ui->paper_date_pub_time->setDate(QDate::currentDate());
    ui->paper_edit_pub_year->clear();
    ui->paper_edit_volume->clear();
    ui->paper_edit_issue->clear();
    ui->paper_edit_page->clear();
    ui->paper_combo_level->setCurrentIndex(0);
    ui->paper_combo_type->setCurrentIndex(0);
    ui->paper_combo_index->setCurrentIndex(0);
    ui->btn_paper_delete->setEnabled(false);
    ui->btn_paper_update->setEnabled(false);
    ui->btn_paper_save->setEnabled(false);
    // 重新绑定过滤器
    paper_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    paper_model->select();

    // 项目成果
    // 清空项目成果编辑框
    ui->project_edit_title->clear();
    ui->project_combo_type->setCurrentIndex(0);
    ui->project_combo_sort->setCurrentIndex(0);
    ui->project_combo_level->setCurrentIndex(0);
    ui->project_combo_rate->setCurrentIndex(0);
    ui->project_combo_state->setCurrentIndex(0);
    ui->project_edit_host->clear();
    ui->project_edit_rank->clear();
    ui->project_edit_first->clear();
    ui->project_date_release->setDate(QDate::currentDate());
    ui->project_edit_unit->clear();
    ui->project_edit_cost->clear();
    ui->project_combo_limit->setCurrentIndex(0);
    ui->project_date_begin->setDate(QDate::currentDate());
    ui->project_date_end->setDate(QDate::currentDate());
    ui->btn_project_delete->setEnabled(false);
    ui->btn_project_update->setEnabled(false);
    ui->btn_project_save->setEnabled(false);
    // 重新绑定过滤器
    project_model->setFilter(QString("code = '%1'").arg(ui->edit_current_code->text().trimmed()));
    project_model->select();

    ui->list_family->selectRow(0);
    ui->list_workexperience->selectRow(0);
    ui->list_reward->selectRow(0);
    ui->list_punish->selectRow(0);
    ui->list_paper->selectRow(0);
    ui->list_project->selectRow(0);
}

void HumanBrowseDialog::btn_total_add_clicked()
{
    ui->edit_current_name->clear();
    ui->edit_current_code->clear();
    ui->base_edit_code->clear();
    ui->base_edit_code->setEnabled(true);
    ui->base_edit_name->clear();
    ui->base_combo_sex->setCurrentIndex(0);
    ui->base_date_birth->setDate(QDate::currentDate());
    ui->base_combo_marriage->setCurrentIndex(0);
    ui->base_edit_age->clear();
    ui->base_combo_party->setCurrentIndex(0);
    ui->base_combo_race->setCurrentIndex(0);
    ui->base_edit_idcard->clear();
    ui->base_date_workdate->setDate(QDate::currentDate());
    ui->base_combo_post->setCurrentIndex(0);
    ui->base_edit_native->clear();
    ui->base_date_entrydate->setDate(QDate::currentDate());
    ui->base_combo_salary->setCurrentIndex(0);
    ui->base_combo_degree->setCurrentIndex(0);
    ui->base_combo_department->setCurrentIndex(0);
    ui->base_combo_technical->setCurrentIndex(0);
    ui->base_combo_job->setCurrentIndex(0);
    ui->base_edit_officephone->clear();
    ui->base_edit_mobilephone->clear();
    ui->base_edit_email->clear();
    ui->base_edit_university->clear();
    ui->base_edit_subject->clear();
    ui->base_date_graduatedate->setDate(QDate::currentDate());
    ui->base_edit_address->clear();
    ui->btn_total_delete->setEnabled(false);
    ui->btn_total_update->setEnabled(false);
    ui->btn_total_save->setEnabled(true);
    ui->btn_total_cancel->setEnabled(true);
}

void HumanBrowseDialog::btn_total_delete_clicked()
{
    if (staff_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除教工档案"), tr("您是否确定删除当前选中的教工的信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_staff->currentIndex().row(); // 获取选中行
        staff_model->removeRow(nIndexRow--);
        staff_model->submitAll();
        staff_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_staff->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_staff->selectRow(nIndexRow);
        else
            ui->btn_total_add->click();
    }
}

void HumanBrowseDialog::btn_total_save_clicked()
{
    // staff 表两个非空属性
    if (ui->base_edit_code->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教工档案"), tr("请输入职工编号！"));
        ui->base_edit_code->setFocus();
        return;
    }
    if (ui->base_edit_name->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教工档案"), tr("请输入职工姓名！"));
        ui->base_edit_name->setFocus();
        return;
    }

    QSqlQuery query = staff_model->query();
    query.prepare("SELECT * FROM `staff` WHERE `code` = :code");
    query.bindValue(":code", ui->base_edit_code->text().trimmed());
    query.exec();

    // 新增教工档案
    if (query.size() > 0)
    {
        // 存在教工档案
        QMessageBox::critical(this, tr("新增教工档案失败"), tr("该工号已存在！"));
        return;
    }
    int rowNum = staff_model->rowCount();
    staff_model->insertRow(rowNum);
    staff_model->setData(staff_model->index(rowNum, 0), ui->base_edit_code->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 1), ui->base_edit_name->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 2), ui->base_combo_sex->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 3), ui->base_date_birth->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 4), ui->base_combo_marriage->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 5), ui->base_edit_age->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 6), ui->base_combo_party->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 7), ui->base_combo_race->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 8), ui->base_edit_idcard->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 9), ui->base_date_workdate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 10), ui->base_combo_post->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 11), ui->base_edit_native->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 12), ui->base_date_entrydate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 13), ui->base_combo_salary->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 14), ui->base_combo_degree->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 15), ui->base_combo_department->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 16), ui->base_combo_technical->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 17), ui->base_combo_job->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 18), ui->base_edit_officephone->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 19), ui->base_edit_mobilephone->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 20), ui->base_edit_email->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 21), ui->base_edit_university->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 22), ui->base_edit_subject->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 23), ui->base_date_graduatedate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 24), ui->base_edit_address->text().trimmed());
    staff_model->submitAll();
    staff_model->select();
    QMessageBox::information(this, tr("提示"), tr("教工档案添加成功！"));
    ui->list_staff->selectRow(rowNum);
}

void HumanBrowseDialog::btn_total_update_clicked()
{
    // staff 表两个非空属性
    if (ui->base_edit_name->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改教工档案"), tr("请输入职工姓名！"));
        ui->base_edit_name->setFocus();
        return;
    }

    QSqlQuery query = staff_model->query();
    query.prepare("SELECT * FROM `staff` WHERE `code` = :code");
    query.bindValue(":code", ui->base_edit_code->text().trimmed());
    query.exec();

    // 修改教工档案
    if (query.size() == 0)
    {
        // 不存在教工档案
        QMessageBox::critical(this, tr("修改教工档案失败"), tr("该职工不存在！"));
        return;
    }
    int rowNum = ui->list_staff->currentIndex().row(); // 获取选中行
    staff_model->setData(staff_model->index(rowNum, 1), ui->base_edit_name->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 2), ui->base_combo_sex->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 3), ui->base_date_birth->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 4), ui->base_combo_marriage->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 5), ui->base_edit_age->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 6), ui->base_combo_party->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 7), ui->base_combo_race->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 8), ui->base_edit_idcard->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 9), ui->base_date_workdate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 10), ui->base_combo_post->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 11), ui->base_edit_native->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 12), ui->base_date_entrydate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 13), ui->base_combo_salary->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 14), ui->base_combo_degree->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 15), ui->base_combo_department->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 16), ui->base_combo_technical->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 17), ui->base_combo_job->currentText().trimmed());
    staff_model->setData(staff_model->index(rowNum, 18), ui->base_edit_officephone->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 19), ui->base_edit_mobilephone->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 20), ui->base_edit_email->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 21), ui->base_edit_university->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 22), ui->base_edit_subject->text().trimmed());
    staff_model->setData(staff_model->index(rowNum, 23), ui->base_date_graduatedate->date().toString("yyyy-MM-dd"));
    staff_model->setData(staff_model->index(rowNum, 24), ui->base_edit_address->text().trimmed());
    staff_model->submitAll();
    staff_model->select();
    QMessageBox::information(this, tr("提示"), tr("教工档案修改成功！"));
    ui->list_staff->selectRow(rowNum);
}

void HumanBrowseDialog::btn_total_cancel_clicked()
{
    emit ui->list_staff->clicked(ui->list_staff->currentIndex());
}

void HumanBrowseDialog::tab_base_archive_init()
{
    connect(ui->btn_total_add, SIGNAL(clicked(bool)), this, SLOT(btn_total_add_clicked()));
    connect(ui->btn_total_delete, SIGNAL(clicked(bool)), this, SLOT(btn_total_delete_clicked()));
    connect(ui->btn_total_save, SIGNAL(clicked(bool)), this, SLOT(btn_total_save_clicked()));
    connect(ui->btn_total_update, SIGNAL(clicked(bool)), this, SLOT(btn_total_update_clicked()));
    connect(ui->btn_total_cancel, SIGNAL(clicked(bool)), this, SLOT(btn_total_cancel_clicked()));

    // 院系部门
    QSqlQuery department_section_query("SELECT division FROM `department_section`", SqlController->getDefaultConnection());
    while (department_section_query.next())
        ui->base_combo_department->addItem(department_section_query.value(0).toString());
    // 行政职务
    ui->base_combo_job->addItem(tr("无"));
    QSqlQuery administrative_query("SELECT rank FROM `administrative_post`", SqlController->getDefaultConnection());
    while (administrative_query.next())
        ui->base_combo_job->addItem(administrative_query.value(0).toString());
    // 职称类别
    ui->base_combo_technical->addItem(tr("无"));
    QSqlQuery technical_query("SELECT title FROM `technical_post`", SqlController->getDefaultConnection());
    while (technical_query.next())
        ui->base_combo_technical->addItem(technical_query.value(0).toString());
    // 教工岗位
    QSqlQuery positionInf_query("SELECT post FROM `position_information`", SqlController->getDefaultConnection());
    while (positionInf_query.next())
        ui->base_combo_post->addItem(positionInf_query.value(0).toString());
    // 工资等级
    QSqlQuery wage_query("SELECT salary FROM `wage_level`", SqlController->getDefaultConnection());
    while (wage_query.next())
        ui->base_combo_salary->addItem(wage_query.value(0).toString());
    // 民族信息
    QSqlQuery nationInf_query("SELECT folk FROM `nation_information`", SqlController->getDefaultConnection());
    while (nationInf_query.next())
        ui->base_combo_race->addItem(nationInf_query.value(0).toString());
    // 政治面貌
    ui->base_combo_party->addItem(tr("无"));
    QSqlQuery party_query("SELECT party FROM `political_status`", SqlController->getDefaultConnection());
    while (party_query.next())
        ui->base_combo_party->addItem(party_query.value(0).toString());
    // 文化程度
    QSqlQuery education_query("SELECT degree FROM `education_degree`", SqlController->getDefaultConnection());
    while (education_query.next())
        ui->base_combo_degree->addItem(education_query.value(0).toString());
}

void HumanBrowseDialog::tab_family_member_init()
{
    connect(ui->btn_family_add, SIGNAL(clicked(bool)), this, SLOT(btn_family_add_clicked()));
    connect(ui->btn_family_delete, SIGNAL(clicked(bool)), this, SLOT(btn_family_delete_clicked()));
    connect(ui->btn_family_save, SIGNAL(clicked(bool)), this, SLOT(btn_family_save_clicked()));
    connect(ui->btn_family_update, SIGNAL(clicked(bool)), this, SLOT(btn_family_update_clicked()));

    // 政治面貌
    ui->family_combo_political->addItem(tr("无"));
    QSqlQuery party_query("SELECT party FROM `political_status`", SqlController->getDefaultConnection());
    while (party_query.next())
        ui->family_combo_political->addItem(party_query.value(0).toString());

    // 家庭成员列表
    family_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    family_model->setTable("family_member");
    family_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    family_model->select();
    family_model->setHeaderData(2, Qt::Horizontal, tr("姓名"));
    family_model->setHeaderData(3, Qt::Horizontal, tr("身份证号"));
    family_model->setHeaderData(4, Qt::Horizontal, tr("与教工关系"));
    family_model->setHeaderData(5, Qt::Horizontal, tr("生日"));
    family_model->setHeaderData(6, Qt::Horizontal, tr("工作单位"));
    family_model->setHeaderData(7, Qt::Horizontal, tr("联系电话"));
    family_model->setHeaderData(8, Qt::Horizontal, tr("职务"));
    family_model->setHeaderData(9, Qt::Horizontal, tr("政治面貌"));
    family_model->setHeaderData(10, Qt::Horizontal, tr("文化程度"));
    while (family_model->canFetchMore())
        family_model->fetchMore();

    ui->list_family->setModel(family_model);
    ui->list_family->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_family->setColumnHidden(0, true);
    ui->list_family->setColumnHidden(1, true);
    connect(ui->list_family->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_family_changed(const QModelIndex &)));
    connect(ui->list_family, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_family_changed(const QModelIndex &)));
    ui->list_family->selectRow(0);
}

void HumanBrowseDialog::btn_family_add_clicked()
{
    ui->family_edit_name->clear();
    ui->family_edit_idcard->clear();
    ui->family_edit_relation->clear();
    ui->family_date_birth->setDate(QDate::currentDate());
    ui->family_edit_company->clear();
    ui->family_edit_phone->clear();
    ui->family_edit_duty->clear();
    ui->family_combo_political->setCurrentIndex(0);
    ui->family_edit_education->clear();
    ui->btn_family_delete->setEnabled(false);
    ui->btn_family_update->setEnabled(false);
    ui->btn_family_save->setEnabled(true);
}

void HumanBrowseDialog::btn_family_delete_clicked()
{
    if (family_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除家庭成员"), tr("您是否确定删除当前选中的教工的家庭成员信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_family->currentIndex().row(); // 获取选中行
        family_model->removeRow(nIndexRow--);
        family_model->submitAll();
        family_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_family->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_family->selectRow(nIndexRow);
        else
            ui->btn_family_add->click();
    }
}

void HumanBrowseDialog::btn_family_save_clicked()
{
    // family member 表两个非空属性
    if (ui->family_edit_name->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改家庭成员"), tr("请输入家庭成员姓名！"));
        ui->family_edit_name->setFocus();
        return;
    }
    if (ui->family_edit_idcard->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改家庭成员"), tr("请输入家庭成员身份证号！"));
        ui->family_edit_idcard->setFocus();
        return;
    }

    QSqlQuery query = family_model->query();
    query.prepare("SELECT * FROM `family_member` WHERE `code` = :code AND `identity_card` = :idcard");
    query.bindValue(":code", ui->edit_current_code->text().trimmed());
    query.bindValue(":idcard", ui->family_edit_idcard->text().trimmed());
    query.exec();

    // 新增家庭成员
    if (query.size() > 0)
    {
        // 存在家庭成员
        QMessageBox::critical(this, tr("新增家庭成员失败"), tr("该成员已存在！"));
        return;
    }
    int rowNum = family_model->rowCount();
    family_model->insertRow(rowNum);
    family_model->setData(family_model->index(rowNum, 0), 0);
    family_model->setData(family_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    family_model->setData(family_model->index(rowNum, 2), ui->family_edit_name->text().trimmed());
    family_model->setData(family_model->index(rowNum, 3), ui->family_edit_idcard->text().trimmed());
    family_model->setData(family_model->index(rowNum, 4), ui->family_edit_relation->text().trimmed());
    family_model->setData(family_model->index(rowNum, 5), ui->family_date_birth->date().toString("yyyy-MM-dd"));
    family_model->setData(family_model->index(rowNum, 6), ui->family_edit_company->text().trimmed());
    family_model->setData(family_model->index(rowNum, 7), ui->family_edit_phone->text().trimmed());
    family_model->setData(family_model->index(rowNum, 8), ui->family_edit_duty->text().trimmed());
    family_model->setData(family_model->index(rowNum, 9), ui->family_combo_political->currentText().trimmed());
    family_model->setData(family_model->index(rowNum, 10), ui->family_edit_education->text().trimmed());
    family_model->submitAll();
    family_model->select();
    QMessageBox::information(this, tr("提示"), tr("家庭成员添加成功！"));
    ui->list_family->selectRow(rowNum);
}

void HumanBrowseDialog::btn_family_update_clicked()
{
    // family member 表两个非空属性
    if (ui->family_edit_name->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改家庭成员"), tr("请输入家庭成员姓名！"));
        ui->family_edit_name->setFocus();
        return;
    }
    if (ui->family_edit_idcard->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("修改家庭成员"), tr("请输入家庭成员身份证号！"));
        ui->family_edit_idcard->setFocus();
        return;
    }

    QSqlQuery query = family_model->query();
    query.prepare("SELECT * FROM `family_member` WHERE `code` = :code AND `number` = :number");
    query.bindValue(":code", ui->edit_current_code->text().trimmed());
    query.bindValue(":number", family_model->index(ui->list_family->currentIndex().row(), 0).data().toString());
    query.exec();

    // 修改家庭成员
    if (query.size() == 0)
    {
        // 不存在家庭成员
        QMessageBox::critical(this, tr("修改家庭成员失败"), tr("该成员不存在！"));
        return;
    }
    int rowNum = ui->list_family->currentIndex().row(); // 获取选中行
    family_model->setData(family_model->index(rowNum, 2), ui->family_edit_name->text().trimmed());
    family_model->setData(family_model->index(rowNum, 3), ui->family_edit_idcard->text().trimmed());
    family_model->setData(family_model->index(rowNum, 4), ui->family_edit_relation->text().trimmed());
    family_model->setData(family_model->index(rowNum, 5), ui->family_date_birth->date().toString("yyyy-MM-dd"));
    family_model->setData(family_model->index(rowNum, 6), ui->family_edit_company->text().trimmed());
    family_model->setData(family_model->index(rowNum, 7), ui->family_edit_phone->text().trimmed());
    family_model->setData(family_model->index(rowNum, 8), ui->family_edit_duty->text().trimmed());
    family_model->setData(family_model->index(rowNum, 9), ui->family_combo_political->currentText().trimmed());
    family_model->setData(family_model->index(rowNum, 10), ui->family_edit_education->text().trimmed());
    family_model->submitAll();
    family_model->select();
    QMessageBox::information(this, tr("提示"), tr("家庭成员修改成功！"));
    ui->list_family->selectRow(rowNum);
}

void HumanBrowseDialog::list_family_changed(const QModelIndex &current)
{
    ui->family_edit_name->setText(family_model->index(current.row(), 2).data().toString());
    ui->family_edit_idcard->setText(family_model->index(current.row(), 3).data().toString());
    ui->family_edit_relation->setText(family_model->index(current.row(), 4).data().toString());
    ui->family_date_birth->setDate(QDate::fromString(family_model->index(current.row(), 5).data().toString(), "yyyy-MM-dd"));
    ui->family_edit_company->setText(family_model->index(current.row(), 6).data().toString());
    ui->family_edit_phone->setText(family_model->index(current.row(), 7).data().toString());
    ui->family_edit_duty->setText(family_model->index(current.row(), 8).data().toString());
    ui->family_combo_political->setCurrentText(family_model->index(current.row(), 9).data().toString());
    ui->family_edit_education->setText(family_model->index(current.row(), 10).data().toString());
    ui->btn_family_delete->setEnabled(true);
    ui->btn_family_update->setEnabled(true);
    ui->btn_family_save->setEnabled(false);
}

void HumanBrowseDialog::tab_work_experience_init()
{
    connect(ui->btn_workexp_add, SIGNAL(clicked(bool)), this, SLOT(btn_workexp_add_clicked()));
    connect(ui->btn_workexp_delete, SIGNAL(clicked(bool)), this, SLOT(btn_workexp_delete_clicked()));
    connect(ui->btn_workexp_save, SIGNAL(clicked(bool)), this, SLOT(btn_workexp_save_clicked()));
    connect(ui->btn_workexp_update, SIGNAL(clicked(bool)), this, SLOT(btn_workexp_update_clicked()));

    // 文化程度
    QSqlQuery education_query("SELECT degree FROM `education_degree`", SqlController->getDefaultConnection());
    while (education_query.next())
        ui->workexp_combo_education->addItem(education_query.value(0).toString());

    // 工作经历列表
    workexp_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    workexp_model->setTable("work_experience");
    workexp_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    workexp_model->select();
    workexp_model->setHeaderData(2, Qt::Horizontal, tr("开始时间"));
    workexp_model->setHeaderData(3, Qt::Horizontal, tr("结束时间"));
    workexp_model->setHeaderData(4, Qt::Horizontal, tr("工作单位"));
    workexp_model->setHeaderData(5, Qt::Horizontal, tr("所属部门"));
    workexp_model->setHeaderData(6, Qt::Horizontal, tr("职务"));
    workexp_model->setHeaderData(7, Qt::Horizontal, tr("职称"));
    workexp_model->setHeaderData(8, Qt::Horizontal, tr("学历"));
    workexp_model->setHeaderData(9, Qt::Horizontal, tr("所在区域"));
    workexp_model->setHeaderData(10, Qt::Horizontal, tr("证明人"));
    while (workexp_model->canFetchMore())
        workexp_model->fetchMore();

    ui->list_workexperience->setModel(workexp_model);
    ui->list_workexperience->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_workexperience->setColumnHidden(0, true);
    ui->list_workexperience->setColumnHidden(1, true);
    connect(ui->list_workexperience->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_workexp_changed(const QModelIndex &)));
    connect(ui->list_workexperience, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_workexp_changed(const QModelIndex &)));
    ui->list_workexperience->selectRow(0);
}

void HumanBrowseDialog::btn_workexp_add_clicked()
{
    ui->workexp_date_begin->setDate(QDate::currentDate());
    ui->workexp_date_end->setDate(QDate::currentDate());
    ui->workexp_edit_company->clear();
    ui->workexp_edit_department->clear();
    ui->workexp_edit_duty->clear();
    ui->workexp_edit_technical->clear();
    ui->workexp_combo_education->setCurrentIndex(0);
    ui->workexp_edit_location->clear();
    ui->workexp_edit_proof->clear();
    ui->btn_workexp_delete->setEnabled(false);
    ui->btn_workexp_update->setEnabled(false);
    ui->btn_workexp_save->setEnabled(true);
}

void HumanBrowseDialog::btn_workexp_save_clicked()
{
    int rowNum = workexp_model->rowCount();
    workexp_model->insertRow(rowNum);
    workexp_model->setData(workexp_model->index(rowNum, 0), 0);
    workexp_model->setData(workexp_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 2), ui->workexp_date_begin->date().toString("yyyy-MM-dd"));
    workexp_model->setData(workexp_model->index(rowNum, 3), ui->workexp_date_end->date().toString("yyyy-MM-dd"));
    workexp_model->setData(workexp_model->index(rowNum, 4), ui->workexp_edit_company->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 5), ui->workexp_edit_department->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 6), ui->workexp_edit_duty->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 7), ui->workexp_edit_technical->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 8), ui->workexp_combo_education->currentText().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 9), ui->workexp_edit_location->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 10), ui->workexp_edit_proof->text().trimmed());
    workexp_model->submitAll();
    workexp_model->select();
    QMessageBox::information(this, tr("提示"), tr("工作经历添加成功！"));
    ui->list_workexperience->selectRow(rowNum);
}

void HumanBrowseDialog::btn_workexp_update_clicked()
{
    int rowNum = ui->list_workexperience->currentIndex().row(); // 获取选中行
    workexp_model->setData(workexp_model->index(rowNum, 2), ui->workexp_date_begin->date().toString("yyyy-MM-dd"));
    workexp_model->setData(workexp_model->index(rowNum, 3), ui->workexp_date_end->date().toString("yyyy-MM-dd"));
    workexp_model->setData(workexp_model->index(rowNum, 4), ui->workexp_edit_company->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 5), ui->workexp_edit_department->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 6), ui->workexp_edit_duty->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 7), ui->workexp_edit_technical->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 8), ui->workexp_combo_education->currentText().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 9), ui->workexp_edit_location->text().trimmed());
    workexp_model->setData(workexp_model->index(rowNum, 10), ui->workexp_edit_proof->text().trimmed());
    workexp_model->submitAll();
    workexp_model->select();
    QMessageBox::information(this, tr("提示"), tr("工作经历修改成功！"));
    ui->list_workexperience->selectRow(rowNum);
}

void HumanBrowseDialog::btn_workexp_delete_clicked()
{
    if (workexp_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除工作经历"), tr("您是否确定删除当前选中的教工的工作经历信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_workexperience->currentIndex().row(); // 获取选中行
        workexp_model->removeRow(nIndexRow--);
        workexp_model->submitAll();
        workexp_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_workexperience->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_workexperience->selectRow(nIndexRow);
        else
            ui->btn_workexp_add->click();
    }
}

void HumanBrowseDialog::list_workexp_changed(const QModelIndex &current)
{
    ui->workexp_date_begin->setDate(QDate::fromString(workexp_model->index(current.row(), 2).data().toString(), "yyyy-MM-dd"));
    ui->workexp_date_end->setDate(QDate::fromString(workexp_model->index(current.row(), 3).data().toString(), "yyyy-MM-dd"));
    ui->workexp_edit_company->setText(workexp_model->index(current.row(), 4).data().toString());
    ui->workexp_edit_department->setText(workexp_model->index(current.row(), 5).data().toString());
    ui->workexp_edit_duty->setText(workexp_model->index(current.row(), 6).data().toString());
    ui->workexp_edit_technical->setText(workexp_model->index(current.row(), 7).data().toString());
    ui->workexp_combo_education->setCurrentText(workexp_model->index(current.row(), 8).data().toString());
    ui->workexp_edit_location->setText(workexp_model->index(current.row(), 9).data().toString());
    ui->workexp_edit_proof->setText(workexp_model->index(current.row(), 10).data().toString());
    ui->btn_workexp_delete->setEnabled(true);
    ui->btn_workexp_update->setEnabled(true);
    ui->btn_workexp_save->setEnabled(false);
}

void HumanBrowseDialog::tab_reward_init()
{
    connect(ui->btn_reward_add, SIGNAL(clicked(bool)), this, SLOT(btn_reward_add_clicked()));
    connect(ui->btn_reward_delete, SIGNAL(clicked(bool)), this, SLOT(btn_reward_delete_clicked()));
    connect(ui->btn_reward_save, SIGNAL(clicked(bool)), this, SLOT(btn_reward_save_clicked()));
    connect(ui->btn_reward_update, SIGNAL(clicked(bool)), this, SLOT(btn_reward_update_clicked()));

    // 奖励记录列表
    reward_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    reward_model->setTable("reward_record");
    reward_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    reward_model->select();
    reward_model->setHeaderData(2, Qt::Horizontal, tr("奖励名称"));
    reward_model->setHeaderData(3, Qt::Horizontal, tr("奖励等级"));
    reward_model->setHeaderData(4, Qt::Horizontal, tr("奖励级别"));
    reward_model->setHeaderData(5, Qt::Horizontal, tr("授奖单位"));
    reward_model->setHeaderData(6, Qt::Horizontal, tr("奖励门类"));
    reward_model->setHeaderData(7, Qt::Horizontal, tr("奖励时间"));
    while (reward_model->canFetchMore())
        reward_model->fetchMore();

    ui->list_reward->setModel(reward_model);
    ui->list_reward->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_reward->setColumnHidden(0, true);
    ui->list_reward->setColumnHidden(1, true);
    connect(ui->list_reward->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_reward_changed(const QModelIndex &)));
    connect(ui->list_reward, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_reward_changed(const QModelIndex &)));
    ui->list_reward->selectRow(0);
}

void HumanBrowseDialog::btn_reward_add_clicked()
{
    ui->reward_edit_title->clear();
    ui->reward_combo_sort->setCurrentIndex(0);
    ui->reward_combo_level->setCurrentIndex(0);
    ui->reward_edit_unit->clear();
    ui->reward_combo_category->setCurrentIndex(0);
    ui->reward_date->setDate(QDate::currentDate());
    ui->btn_reward_delete->setEnabled(false);
    ui->btn_reward_update->setEnabled(false);
    ui->btn_reward_save->setEnabled(true);
}

void HumanBrowseDialog::btn_reward_save_clicked()
{
    int rowNum = reward_model->rowCount();
    reward_model->insertRow(rowNum);
    reward_model->setData(reward_model->index(rowNum, 0), 0);
    reward_model->setData(reward_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    reward_model->setData(reward_model->index(rowNum, 2), ui->reward_edit_title->text().trimmed());
    reward_model->setData(reward_model->index(rowNum, 3), ui->reward_combo_sort->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 4), ui->reward_combo_level->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 5), ui->reward_edit_unit->text().trimmed());
    reward_model->setData(reward_model->index(rowNum, 6), ui->reward_combo_category->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 7), ui->reward_date->date().toString("yyyy-MM-dd"));
    reward_model->submitAll();
    reward_model->select();
    QMessageBox::information(this, tr("提示"), tr("奖励记录添加成功！"));
    ui->list_reward->selectRow(rowNum);
}

void HumanBrowseDialog::btn_reward_update_clicked()
{
    int rowNum = ui->list_reward->currentIndex().row(); // 获取选中行
    reward_model->setData(reward_model->index(rowNum, 2), ui->reward_edit_title->text().trimmed());
    reward_model->setData(reward_model->index(rowNum, 3), ui->reward_combo_sort->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 4), ui->reward_combo_level->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 5), ui->reward_edit_unit->text().trimmed());
    reward_model->setData(reward_model->index(rowNum, 6), ui->reward_combo_category->currentText().trimmed());
    reward_model->setData(reward_model->index(rowNum, 7), ui->reward_date->date().toString("yyyy-MM-dd"));
    reward_model->submitAll();
    reward_model->select();
    QMessageBox::information(this, tr("提示"), tr("奖励记录修改成功！"));
    ui->list_reward->selectRow(rowNum);
}

void HumanBrowseDialog::btn_reward_delete_clicked()
{
    if (reward_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除奖励记录"), tr("您是否确定删除当前选中的奖励记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_reward->currentIndex().row(); // 获取选中行
        reward_model->removeRow(nIndexRow--);
        reward_model->submitAll();
        reward_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_reward->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_reward->selectRow(nIndexRow);
        else
            ui->btn_reward_add->click();
    }
}

void HumanBrowseDialog::list_reward_changed(const QModelIndex &current)
{
    ui->reward_edit_title->setText(reward_model->index(current.row(), 2).data().toString());
    ui->reward_combo_sort->setCurrentText(reward_model->index(current.row(), 3).data().toString());
    ui->reward_combo_level->setCurrentText(reward_model->index(current.row(), 4).data().toString());
    ui->reward_edit_unit->setText(reward_model->index(current.row(), 5).data().toString());
    ui->reward_combo_category->setCurrentText(reward_model->index(current.row(), 6).data().toString());
    ui->reward_date->setDate(QDate::fromString(reward_model->index(current.row(), 7).data().toString(), "yyyy-MM-dd"));
    ui->btn_reward_delete->setEnabled(true);
    ui->btn_reward_update->setEnabled(true);
    ui->btn_reward_save->setEnabled(false);
}

void HumanBrowseDialog::tab_punish_init()
{
    connect(ui->btn_punish_add, SIGNAL(clicked(bool)), this, SLOT(btn_punish_add_clicked()));
    connect(ui->btn_punish_delete, SIGNAL(clicked(bool)), this, SLOT(btn_punish_delete_clicked()));
    connect(ui->btn_punish_save, SIGNAL(clicked(bool)), this, SLOT(btn_punish_save_clicked()));
    connect(ui->btn_punish_update, SIGNAL(clicked(bool)), this, SLOT(btn_punish_update_clicked()));

    // 违纪处罚记录列表
    punish_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    punish_model->setTable("discipline_punish");
    punish_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    punish_model->select();
    punish_model->setHeaderData(2, Qt::Horizontal, tr("违纪种类"));
    punish_model->setHeaderData(3, Qt::Horizontal, tr("处分期"));
    punish_model->setHeaderData(4, Qt::Horizontal, tr("违纪原因"));
    punish_model->setHeaderData(5, Qt::Horizontal, tr("处罚时间"));
    punish_model->setHeaderData(6, Qt::Horizontal, tr("解除时间"));
    punish_model->setHeaderData(7, Qt::Horizontal, tr("解除原因"));
    punish_model->setHeaderData(8, Qt::Horizontal, tr("处罚批准单位"));
    while (punish_model->canFetchMore())
        punish_model->fetchMore();

    ui->list_punish->setModel(punish_model);
    ui->list_punish->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_punish->setColumnHidden(0, true);
    ui->list_punish->setColumnHidden(1, true);
    connect(ui->list_punish->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_punish_changed(const QModelIndex &)));
    connect(ui->list_punish, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_punish_changed(const QModelIndex &)));
    ui->list_punish->selectRow(0);
}

void HumanBrowseDialog::btn_punish_add_clicked()
{
    ui->punish_combo_sort->setCurrentIndex(0);
    ui->punish_combo_period->setCurrentIndex(0);
    ui->punish_edit_preason->clear();
    ui->punish_date_pdate->setDate(QDate::currentDate());
    ui->punish_date_ddate->setDate(QDate::currentDate());
    ui->punish_edit_dreason->clear();
    ui->punish_edit_unit->clear();
    ui->btn_punish_delete->setEnabled(false);
    ui->btn_punish_update->setEnabled(false);
    ui->btn_punish_save->setEnabled(true);
}

void HumanBrowseDialog::btn_punish_save_clicked()
{
    int rowNum = punish_model->rowCount();
    punish_model->insertRow(rowNum);
    punish_model->setData(punish_model->index(rowNum, 0), 0);
    punish_model->setData(punish_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    punish_model->setData(punish_model->index(rowNum, 2), ui->punish_combo_sort->currentText().trimmed());
    punish_model->setData(punish_model->index(rowNum, 3), ui->punish_combo_period->currentText().trimmed());
    punish_model->setData(punish_model->index(rowNum, 4), ui->punish_edit_preason->text().trimmed());
    punish_model->setData(punish_model->index(rowNum, 5), ui->punish_date_pdate->date().toString("yyyy-MM-dd"));
    punish_model->setData(punish_model->index(rowNum, 6), ui->punish_date_ddate->date().toString("yyyy-MM-dd"));
    punish_model->setData(punish_model->index(rowNum, 7), ui->punish_edit_dreason->text().trimmed());
    punish_model->setData(punish_model->index(rowNum, 8), ui->punish_edit_unit->text().trimmed());
    punish_model->submitAll();
    punish_model->select();
    QMessageBox::information(this, tr("提示"), tr("违纪处罚记录添加成功！"));
    ui->list_punish->selectRow(rowNum);
}

void HumanBrowseDialog::btn_punish_update_clicked()
{
    int rowNum = ui->list_punish->currentIndex().row(); // 获取选中行
    punish_model->setData(punish_model->index(rowNum, 2), ui->punish_combo_sort->currentText().trimmed());
    punish_model->setData(punish_model->index(rowNum, 3), ui->punish_combo_period->currentText().trimmed());
    punish_model->setData(punish_model->index(rowNum, 4), ui->punish_edit_preason->text().trimmed());
    punish_model->setData(punish_model->index(rowNum, 5), ui->punish_date_pdate->date().toString("yyyy-MM-dd"));
    punish_model->setData(punish_model->index(rowNum, 6), ui->punish_date_ddate->date().toString("yyyy-MM-dd"));
    punish_model->setData(punish_model->index(rowNum, 7), ui->punish_edit_dreason->text().trimmed());
    punish_model->setData(punish_model->index(rowNum, 8), ui->punish_edit_unit->text().trimmed());
    punish_model->submitAll();
    punish_model->select();
    QMessageBox::information(this, tr("提示"), tr("违纪处罚记录修改成功！"));
    ui->list_punish->selectRow(rowNum);
}

void HumanBrowseDialog::btn_punish_delete_clicked()
{
    if (punish_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除违纪处罚记录"), tr("您是否确定删除当前选中的违纪处罚记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_punish->currentIndex().row(); // 获取选中行
        punish_model->removeRow(nIndexRow--);
        punish_model->submitAll();
        punish_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_punish->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_punish->selectRow(nIndexRow);
        else
            ui->btn_punish_add->click();
    }
}

void HumanBrowseDialog::list_punish_changed(const QModelIndex &current)
{
    ui->punish_combo_sort->setCurrentText(punish_model->index(current.row(), 2).data().toString());
    ui->punish_combo_period->setCurrentText(punish_model->index(current.row(), 3).data().toString());
    ui->punish_edit_preason->setText(punish_model->index(current.row(), 4).data().toString());
    ui->punish_date_pdate->setDate(QDate::fromString(punish_model->index(current.row(), 5).data().toString(), "yyyy-MM-dd"));
    ui->punish_date_ddate->setDate(QDate::fromString(punish_model->index(current.row(), 6).data().toString(), "yyyy-MM-dd"));
    ui->punish_edit_dreason->setText(punish_model->index(current.row(), 7).data().toString());
    ui->punish_edit_unit->setText(punish_model->index(current.row(), 8).data().toString());
    ui->btn_punish_delete->setEnabled(true);
    ui->btn_punish_update->setEnabled(true);
    ui->btn_punish_save->setEnabled(false);
}

void HumanBrowseDialog::tab_paper_init()
{
    connect(ui->btn_paper_add, SIGNAL(clicked(bool)), this, SLOT(btn_paper_add_clicked()));
    connect(ui->btn_paper_delete, SIGNAL(clicked(bool)), this, SLOT(btn_paper_delete_clicked()));
    connect(ui->btn_paper_save, SIGNAL(clicked(bool)), this, SLOT(btn_paper_save_clicked()));
    connect(ui->btn_paper_update, SIGNAL(clicked(bool)), this, SLOT(btn_paper_update_clicked()));

    // 论文成果记录列表
    paper_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    paper_model->setTable("paper");
    paper_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    paper_model->select();
    paper_model->setHeaderData(2, Qt::Horizontal, tr("题目"));
    paper_model->setHeaderData(3, Qt::Horizontal, tr("作者排名"));
    paper_model->setHeaderData(4, Qt::Horizontal, tr("刊物名称"));
    paper_model->setHeaderData(5, Qt::Horizontal, tr("发表时间"));
    paper_model->setHeaderData(6, Qt::Horizontal, tr("出版年"));
    paper_model->setHeaderData(7, Qt::Horizontal, tr("卷"));
    paper_model->setHeaderData(8, Qt::Horizontal, tr("期"));
    paper_model->setHeaderData(9, Qt::Horizontal, tr("页码"));
    paper_model->setHeaderData(10, Qt::Horizontal, tr("刊物级别"));
    paper_model->setHeaderData(11, Qt::Horizontal, tr("文献类型"));
    paper_model->setHeaderData(12, Qt::Horizontal, tr("检索类型"));
    while (paper_model->canFetchMore())
        paper_model->fetchMore();

    ui->list_paper->setModel(paper_model);
    ui->list_paper->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_paper->setColumnHidden(0, true);
    ui->list_paper->setColumnHidden(1, true);
    connect(ui->list_paper->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_paper_changed(const QModelIndex &)));
    connect(ui->list_paper, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_paper_changed(const QModelIndex &)));
    ui->list_paper->selectRow(0);
}

void HumanBrowseDialog::btn_paper_add_clicked()
{
    ui->paper_edit_title->clear();
    ui->paper_combo_rank->setCurrentIndex(0);
    ui->paper_edit_publication->clear();
    ui->paper_date_pub_time->setDate(QDate::currentDate());
    ui->paper_edit_pub_year->clear();
    ui->paper_edit_volume->clear();
    ui->paper_edit_issue->clear();
    ui->paper_edit_page->clear();
    ui->paper_combo_level->setCurrentIndex(0);
    ui->paper_combo_type->setCurrentIndex(0);
    ui->paper_combo_index->setCurrentIndex(0);
    ui->btn_paper_delete->setEnabled(false);
    ui->btn_paper_update->setEnabled(false);
    ui->btn_paper_save->setEnabled(true);
}

void HumanBrowseDialog::btn_paper_save_clicked()
{
    int rowNum = paper_model->rowCount();
    paper_model->insertRow(rowNum);
    paper_model->setData(paper_model->index(rowNum, 0), 0);
    paper_model->setData(paper_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 2), ui->paper_edit_title->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 3), ui->paper_combo_rank->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 4), ui->paper_edit_publication->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 5), ui->paper_date_pub_time->date().toString("yyyy-MM-dd"));
    paper_model->setData(paper_model->index(rowNum, 6), ui->paper_edit_pub_year->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 7), ui->paper_edit_volume->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 8), ui->paper_edit_issue->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 9), ui->paper_edit_page->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 10), ui->paper_combo_level->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 11), ui->paper_combo_type->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 12), ui->paper_combo_index->currentText().trimmed());
    paper_model->submitAll();
    paper_model->select();
    QMessageBox::information(this, tr("提示"), tr("论文成果记录添加成功！"));
    ui->list_paper->selectRow(rowNum);
}

void HumanBrowseDialog::btn_paper_update_clicked()
{
    int rowNum = ui->list_paper->currentIndex().row(); // 获取选中行
    paper_model->setData(paper_model->index(rowNum, 2), ui->paper_edit_title->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 3), ui->paper_combo_rank->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 4), ui->paper_edit_publication->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 5), ui->paper_date_pub_time->date().toString("yyyy-MM-dd"));
    paper_model->setData(paper_model->index(rowNum, 6), ui->paper_edit_pub_year->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 7), ui->paper_edit_volume->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 8), ui->paper_edit_issue->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 9), ui->paper_edit_page->text().trimmed());
    paper_model->setData(paper_model->index(rowNum, 10), ui->paper_combo_level->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 11), ui->paper_combo_type->currentText().trimmed());
    paper_model->setData(paper_model->index(rowNum, 12), ui->paper_combo_index->currentText().trimmed());
    paper_model->submitAll();
    paper_model->select();
    QMessageBox::information(this, tr("提示"), tr("论文成果记录修改成功！"));
    ui->list_paper->selectRow(rowNum);
}

void HumanBrowseDialog::btn_paper_delete_clicked()
{
    if (paper_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除论文成果记录"), tr("您是否确定删除当前选中的论文成果记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_paper->currentIndex().row(); // 获取选中行
        paper_model->removeRow(nIndexRow--);
        paper_model->submitAll();
        paper_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_paper->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_paper->selectRow(nIndexRow);
        else
            ui->btn_paper_add->click();
    }
}

void HumanBrowseDialog::list_paper_changed(const QModelIndex &current)
{
    ui->paper_edit_title->setText(paper_model->index(current.row(), 2).data().toString());
    ui->paper_combo_rank->setCurrentText(paper_model->index(current.row(), 3).data().toString());
    ui->paper_edit_publication->setText(paper_model->index(current.row(), 4).data().toString());
    ui->paper_date_pub_time->setDate(QDate::fromString(paper_model->index(current.row(), 5).data().toString(), "yyyy-MM-dd"));
    ui->paper_edit_pub_year->setText(paper_model->index(current.row(), 6).data().toString());
    ui->paper_edit_volume->setText(paper_model->index(current.row(), 7).data().toString());
    ui->paper_edit_issue->setText(paper_model->index(current.row(), 8).data().toString());
    ui->paper_edit_page->setText(paper_model->index(current.row(), 9).data().toString());
    ui->paper_combo_level->setCurrentText(paper_model->index(current.row(), 10).data().toString());
    ui->paper_combo_type->setCurrentText(paper_model->index(current.row(), 11).data().toString());
    ui->paper_combo_index->setCurrentText(paper_model->index(current.row(), 12).data().toString());
    ui->btn_paper_delete->setEnabled(true);
    ui->btn_paper_update->setEnabled(true);
    ui->btn_paper_save->setEnabled(false);
}

void HumanBrowseDialog::tab_project_init()
{
    connect(ui->btn_project_add, SIGNAL(clicked(bool)), this, SLOT(btn_project_add_clicked()));
    connect(ui->btn_project_delete, SIGNAL(clicked(bool)), this, SLOT(btn_project_delete_clicked()));
    connect(ui->btn_project_save, SIGNAL(clicked(bool)), this, SLOT(btn_project_save_clicked()));
    connect(ui->btn_project_update, SIGNAL(clicked(bool)), this, SLOT(btn_project_update_clicked()));

    // 项目成果列表
    project_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    project_model->setTable("project");
    project_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    project_model->select();
    project_model->setHeaderData(2, Qt::Horizontal, tr("题目"));
    project_model->setHeaderData(3, Qt::Horizontal, tr("类型"));
    project_model->setHeaderData(4, Qt::Horizontal, tr("类别"));
    project_model->setHeaderData(5, Qt::Horizontal, tr("级别"));
    project_model->setHeaderData(6, Qt::Horizontal, tr("资助类别"));
    project_model->setHeaderData(7, Qt::Horizontal, tr("状态"));
    project_model->setHeaderData(8, Qt::Horizontal, tr("主持人"));
    project_model->setHeaderData(9, Qt::Horizontal, tr("排名"));
    project_model->setHeaderData(10, Qt::Horizontal, tr("第一完成单位"));
    project_model->setHeaderData(11, Qt::Horizontal, tr("下达时间"));
    project_model->setHeaderData(12, Qt::Horizontal, tr("下达单位"));
    project_model->setHeaderData(13, Qt::Horizontal, tr("下达经费"));
    project_model->setHeaderData(14, Qt::Horizontal, tr("研究期限"));
    project_model->setHeaderData(15, Qt::Horizontal, tr("开始时间"));
    project_model->setHeaderData(16, Qt::Horizontal, tr("结束时间"));
    while (project_model->canFetchMore())
        project_model->fetchMore();

    ui->list_project->setModel(project_model);
    ui->list_project->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_project->setColumnHidden(0, true);
    ui->list_project->setColumnHidden(1, true);
    connect(ui->list_project->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_project_changed(const QModelIndex &)));
    connect(ui->list_project, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_project_changed(const QModelIndex &)));
    ui->list_project->selectRow(0);
}

void HumanBrowseDialog::btn_project_add_clicked()
{
    ui->project_edit_title->clear();
    ui->project_combo_type->setCurrentIndex(0);
    ui->project_combo_sort->setCurrentIndex(0);
    ui->project_combo_level->setCurrentIndex(0);
    ui->project_combo_rate->setCurrentIndex(0);
    ui->project_combo_state->setCurrentIndex(0);
    ui->project_edit_host->clear();
    ui->project_edit_rank->clear();
    ui->project_edit_first->clear();
    ui->project_date_release->setDate(QDate::currentDate());
    ui->project_edit_unit->clear();
    ui->project_edit_cost->clear();
    ui->project_combo_limit->setCurrentIndex(0);
    ui->project_date_begin->setDate(QDate::currentDate());
    ui->project_date_end->setDate(QDate::currentDate());
    ui->btn_project_delete->setEnabled(false);
    ui->btn_project_update->setEnabled(false);
    ui->btn_project_save->setEnabled(true);
}

void HumanBrowseDialog::btn_project_save_clicked()
{
    int rowNum = project_model->rowCount();
    project_model->insertRow(rowNum);
    project_model->setData(project_model->index(rowNum, 0), 0);
    project_model->setData(project_model->index(rowNum, 1), ui->edit_current_code->text().trimmed());
    project_model->setData(project_model->index(rowNum, 2), ui->project_edit_title->text().trimmed());
    project_model->setData(project_model->index(rowNum, 3), ui->project_combo_type->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 4), ui->project_combo_sort->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 5), ui->project_combo_level->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 6), ui->project_combo_rate->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 7), ui->project_combo_state->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 8), ui->project_edit_host->text().trimmed());
    project_model->setData(project_model->index(rowNum, 9), ui->project_edit_rank->text().trimmed());
    project_model->setData(project_model->index(rowNum, 10), ui->project_edit_first->text().trimmed());
    project_model->setData(project_model->index(rowNum, 11), ui->project_date_release->date().toString("yyyy-MM-dd"));
    project_model->setData(project_model->index(rowNum, 12), ui->project_edit_unit->text().trimmed());
    project_model->setData(project_model->index(rowNum, 13), ui->project_edit_cost->text().trimmed());
    project_model->setData(project_model->index(rowNum, 14), ui->project_combo_limit->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 15), ui->project_date_begin->date().toString("yyyy-MM-dd"));
    project_model->setData(project_model->index(rowNum, 16), ui->project_date_end->date().toString("yyyy-MM-dd"));
    project_model->submitAll();
    project_model->select();
    QMessageBox::information(this, tr("提示"), tr("项目成果记录添加成功！"));
    ui->list_project->selectRow(rowNum);
}

void HumanBrowseDialog::btn_project_update_clicked()
{
    int rowNum = ui->list_project->currentIndex().row(); // 获取选中行
    project_model->setData(project_model->index(rowNum, 2), ui->project_edit_title->text().trimmed());
    project_model->setData(project_model->index(rowNum, 3), ui->project_combo_type->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 4), ui->project_combo_sort->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 5), ui->project_combo_level->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 6), ui->project_combo_rate->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 7), ui->project_combo_state->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 8), ui->project_edit_host->text().trimmed());
    project_model->setData(project_model->index(rowNum, 9), ui->project_edit_rank->text().trimmed());
    project_model->setData(project_model->index(rowNum, 10), ui->project_edit_first->text().trimmed());
    project_model->setData(project_model->index(rowNum, 11), ui->project_date_release->date().toString("yyyy-MM-dd"));
    project_model->setData(project_model->index(rowNum, 12), ui->project_edit_unit->text().trimmed());
    project_model->setData(project_model->index(rowNum, 13), ui->project_edit_cost->text().trimmed());
    project_model->setData(project_model->index(rowNum, 14), ui->project_combo_limit->currentText().trimmed());
    project_model->setData(project_model->index(rowNum, 15), ui->project_date_begin->date().toString("yyyy-MM-dd"));
    project_model->setData(project_model->index(rowNum, 16), ui->project_date_end->date().toString("yyyy-MM-dd"));
    project_model->submitAll();
    project_model->select();
    QMessageBox::information(this, tr("提示"), tr("项目成果记录修改成功！"));
    ui->list_project->selectRow(rowNum);
}

void HumanBrowseDialog::btn_project_delete_clicked()
{
    if (project_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除项目成果记录"), tr("您是否确定删除当前选中的项目成果记录？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_project->currentIndex().row(); // 获取选中行
        project_model->removeRow(nIndexRow--);
        project_model->submitAll();
        project_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_project->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_project->selectRow(nIndexRow);
        else
            ui->btn_project_add->click();
    }
}

void HumanBrowseDialog::list_project_changed(const QModelIndex &current)
{
    ui->project_edit_title->setText(project_model->index(current.row(), 2).data().toString());
    ui->project_combo_type->setCurrentText(project_model->index(current.row(), 3).data().toString());
    ui->project_combo_sort->setCurrentText(project_model->index(current.row(), 4).data().toString());
    ui->project_combo_level->setCurrentText(project_model->index(current.row(), 5).data().toString());
    ui->project_combo_rate->setCurrentText(project_model->index(current.row(), 6).data().toString());
    ui->project_combo_state->setCurrentText(project_model->index(current.row(), 7).data().toString());
    ui->project_edit_host->setText(project_model->index(current.row(), 8).data().toString());
    ui->project_edit_rank->setText(project_model->index(current.row(), 9).data().toString());
    ui->project_edit_first->setText(project_model->index(current.row(), 10).data().toString());
    ui->project_date_release->setDate(QDate::fromString(project_model->index(current.row(), 11).data().toString(), "yyyy-MM-dd"));
    ui->project_edit_unit->setText(project_model->index(current.row(), 12).data().toString());
    ui->project_edit_cost->setText(project_model->index(current.row(), 13).data().toString());
    ui->project_combo_limit->setCurrentText(project_model->index(current.row(), 14).data().toString());
    ui->project_date_begin->setDate(QDate::fromString(project_model->index(current.row(), 15).data().toString(), "yyyy-MM-dd"));
    ui->project_date_end->setDate(QDate::fromString(project_model->index(current.row(), 16).data().toString(), "yyyy-MM-dd"));
    ui->btn_project_delete->setEnabled(true);
    ui->btn_project_update->setEnabled(true);
    ui->btn_project_save->setEnabled(false);
}

// technical access   technical
// academic education    academic
// continue education    continue
// class teaching        teaching
