#include "humansearchdialog.h"
#include "ui_humansearchdialog.h"

HumanSearchDialog::HumanSearchDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::HumanSearchDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    // 民族信息
    QSqlQuery nationInf_query("SELECT folk FROM `nation_information`", SqlController->getDefaultConnection());
    while (nationInf_query.next())
        ui->combo_nation->addItem(nationInf_query.value(0).toString());
    // 职称类别
    ui->combo_technical->addItem(tr("无"));
    QSqlQuery technical_query("SELECT title FROM `technical_post`", SqlController->getDefaultConnection());
    while (technical_query.next())
        ui->combo_technical->addItem(technical_query.value(0).toString());
    // 行政职务
    ui->combo_post->addItem(tr("无"));
    QSqlQuery administrative_query("SELECT rank FROM `administrative_post`", SqlController->getDefaultConnection());
    while (administrative_query.next())
        ui->combo_post->addItem(administrative_query.value(0).toString());
    // 文化程度
    QSqlQuery education_query("SELECT degree FROM `education_degree`", SqlController->getDefaultConnection());
    while (education_query.next())
        ui->combo_education->addItem(education_query.value(0).toString());
    // 政治面貌
    ui->combo_political->addItem(tr("无"));
    QSqlQuery party_query("SELECT party FROM `political_status`", SqlController->getDefaultConnection());
    while (party_query.next())
        ui->combo_political->addItem(party_query.value(0).toString());
    // 院系部门
    QSqlQuery department_section_query("SELECT division FROM `department_section`", SqlController->getDefaultConnection());
    while (department_section_query.next())
        ui->combo_unit->addItem(department_section_query.value(0).toString());
    // 教工岗位
    QSqlQuery positionInf_query("SELECT post FROM `position_information`", SqlController->getDefaultConnection());
    while (positionInf_query.next())
        ui->combo_position->addItem(positionInf_query.value(0).toString());

    staff_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    staff_model->setTable("staff");
    staff_model->select();
    staff_model->setHeaderData(0, Qt::Horizontal, tr("工号"));
    staff_model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    staff_model->setHeaderData(2, Qt::Horizontal, tr("性别"));
    staff_model->setHeaderData(5, Qt::Horizontal, tr("年龄"));
    staff_model->setHeaderData(4, Qt::Horizontal, tr("婚姻状况"));
    staff_model->setHeaderData(17, Qt::Horizontal, tr("职务"));
    staff_model->setHeaderData(16, Qt::Horizontal, tr("职称"));
    staff_model->setHeaderData(10, Qt::Horizontal, tr("教工岗位"));
    staff_model->setHeaderData(7, Qt::Horizontal, tr("民族"));
    staff_model->setHeaderData(14, Qt::Horizontal, tr("学历"));
    staff_model->setHeaderData(6, Qt::Horizontal, tr("政治面貌"));
    staff_model->setHeaderData(8, Qt::Horizontal, tr("身份证号"));
    while (staff_model->canFetchMore())
        staff_model->fetchMore();

    ui->list_search->setModel(staff_model);
    ui->list_search->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_search->setColumnWidth(0, 80);
    ui->list_search->setColumnHidden(3, true); // 隐藏列
    ui->list_search->setColumnHidden(9, true);
    ui->list_search->setColumnHidden(11, true);
    ui->list_search->setColumnHidden(12, true);
    ui->list_search->setColumnHidden(13, true);
    ui->list_search->setColumnHidden(15, true);
    ui->list_search->setColumnHidden(18, true);
    ui->list_search->setColumnHidden(19, true);
    ui->list_search->setColumnHidden(20, true);
    ui->list_search->setColumnHidden(21, true);
    ui->list_search->setColumnHidden(22, true);
    ui->list_search->setColumnHidden(23, true);
    ui->list_search->setColumnHidden(24, true);

    connect(ui->btn_search, SIGNAL(clicked(bool)), this, SLOT(btn_search_clicked()));
}

HumanSearchDialog::~HumanSearchDialog()
{
    delete staff_model;
    delete ui;
}

void HumanSearchDialog::btn_search_clicked()
{
    QString strSql;
    // 姓名查询
    if (!ui->edit_name->text().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("name", ui->edit_name->text().trimmed());
    }
    // 性别查询
    if (!ui->combo_sex->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("sex", ui->combo_sex->currentText().trimmed());
    }
    // 年龄查询
    if (ui->edit_age->text().toInt() != 0)
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("age", ui->edit_age->text().trimmed());
    }
    // 民族查询
    if (!ui->combo_nation->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("race", ui->combo_nation->currentText().trimmed());
    }
    // 籍贯查询
    if (!ui->edit_native->text().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("native", ui->edit_native->text().trimmed());
    }
    // 婚姻状况查询
    if (!ui->combo_marriage->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("marriage", ui->combo_marriage->currentText().trimmed());
    }
    // 职称查询
    if (!ui->combo_technical->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("technical", ui->combo_technical->currentText().trimmed());
    }
    // 职务查询
    if (!ui->combo_post->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("job", ui->combo_post->currentText().trimmed());
    }
    // 文化程度查询
    if (!ui->combo_education->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("degree", ui->combo_education->currentText().trimmed());
    }
    // 政治面貌查询
    if (!ui->combo_political->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("party", ui->combo_political->currentText().trimmed());
    }
    // 院系部门查询
    if (!ui->combo_unit->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("department", ui->combo_unit->currentText().trimmed());
    }
    // 教工岗位查询
    if (!ui->combo_position->currentText().isEmpty())
    {
        if (!strSql.isEmpty())
            strSql += " AND ";
        strSql += QString("%1 = '%2'").arg("post", ui->combo_position->currentText().trimmed());
    }
    staff_model->setFilter(strSql);
    staff_model->select();
}
