#include "humanstatisticsdialog.h"
#include "ui_humanstatisticsdialog.h"

HumanStatisticsDialog::HumanStatisticsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::HumanStatisticsDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    result_model = new QStandardItemModel(this);

    select_model = new QStandardItemModel(this);
    select_model->insertRow(0, new QStandardItem(tr("按性别统计")));
    select_model->insertRow(1, new QStandardItem(tr("按年龄统计")));
    select_model->insertRow(2, new QStandardItem(tr("按民族统计")));
    select_model->insertRow(3, new QStandardItem(tr("按籍贯统计")));
    select_model->insertRow(4, new QStandardItem(tr("按职务统计")));
    select_model->insertRow(5, new QStandardItem(tr("按职称统计")));
    select_model->insertRow(6, new QStandardItem(tr("按学历统计")));
    select_model->insertRow(7, new QStandardItem(tr("按政治面貌统计")));
    select_model->insertRow(8, new QStandardItem(tr("按院系部门统计")));
    select_model->insertRow(9, new QStandardItem(tr("按教工岗位统计")));
    select_model->insertRow(10, new QStandardItem(tr("按工资等级统计")));
    select_model->insertRow(11, new QStandardItem(tr("按婚姻状况统计")));
    select_model->insertRow(12, new QStandardItem(tr("按毕业院校统计")));
    select_model->insertRow(13, new QStandardItem(tr("按所学专业统计")));
    select_model->setHeaderData(0, Qt::Horizontal, tr("统计类别"));
    ui->list_selection->setModel(select_model);
    ui->list_selection->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    connect(ui->list_selection->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_selection_changed(const QModelIndex &)));
}

HumanStatisticsDialog::~HumanStatisticsDialog()
{
    delete select_model;
    delete result_model;
    delete ui;
}

void HumanStatisticsDialog::list_selection_changed(const QModelIndex &current)
{
    result_model->clear();
    QString strSql = "SELECT COUNT(code) AS amount,%1 FROM staff GROUP BY %1";
    int index = current.row();

    switch (index)
    {
    case 0: strSql = strSql.arg("sex"); break;
    case 1: strSql = strSql.arg("age"); break;
    case 2: strSql = strSql.arg("race"); break;
    case 3: strSql = strSql.arg("native"); break;
    case 4: strSql = strSql.arg("job"); break;
    case 5: strSql = strSql.arg("technical"); break;
    case 6: strSql = strSql.arg("degree"); break;
    case 7: strSql = strSql.arg("party"); break;
    case 8: strSql = strSql.arg("department"); break;
    case 9: strSql = strSql.arg("post"); break;
    case 10: strSql = strSql.arg("salary"); break;
    case 11: strSql = strSql.arg("marriage"); break;
    case 12: strSql = strSql.arg("university"); break;
    case 13: strSql = strSql.arg("subject"); break;
    default: return;
    }

    QSqlQuery query(strSql, SqlController->getDefaultConnection());
    query.exec();
    while (query.next())
    {
        QList<QStandardItem *> list;
        list.append(new QStandardItem(query.value(1).toString()));
        list.append(new QStandardItem(query.value("amount").toString()));
        result_model->appendRow(list);
    }

    switch (index)
    {
    case 0: result_model->setHeaderData(0, Qt::Horizontal, tr("性别")); break;
    case 1: result_model->setHeaderData(0, Qt::Horizontal, tr("年龄")); break;
    case 2: result_model->setHeaderData(0, Qt::Horizontal, tr("民族")); break;
    case 3: result_model->setHeaderData(0, Qt::Horizontal, tr("籍贯")); break;
    case 4: result_model->setHeaderData(0, Qt::Horizontal, tr("职务")); break;
    case 5: result_model->setHeaderData(0, Qt::Horizontal, tr("职称")); break;
    case 6: result_model->setHeaderData(0, Qt::Horizontal, tr("学历")); break;
    case 7: result_model->setHeaderData(0, Qt::Horizontal, tr("政治面貌")); break;
    case 8: result_model->setHeaderData(0, Qt::Horizontal, tr("院系部门")); break;
    case 9: result_model->setHeaderData(0, Qt::Horizontal, tr("教工岗位")); break;
    case 10: result_model->setHeaderData(0, Qt::Horizontal, tr("工资等级")); break;
    case 11: result_model->setHeaderData(0, Qt::Horizontal, tr("婚姻状况")); break;
    case 12: result_model->setHeaderData(0, Qt::Horizontal, tr("毕业院校")); break;
    case 13: result_model->setHeaderData(0, Qt::Horizontal, tr("所学专业")); break;
    default: return;
    }

    result_model->setHeaderData(1, Qt::Horizontal, tr("人数"));
    ui->list_result->setModel(result_model);
    ui->list_result->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}
