#include "contactsdialog.h"
#include "ui_contactsdialog.h"

ContactsDialog::ContactsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ContactsDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    /***** 通信簿列表 Start *****/
    contacts_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    contacts_model->setTable("contacts");
    contacts_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    contacts_model->select();

    contacts_model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    contacts_model->setHeaderData(2, Qt::Horizontal, tr("性别"));
    contacts_model->setHeaderData(3, Qt::Horizontal, tr("家庭电话"));
    contacts_model->setHeaderData(4, Qt::Horizontal, tr("办公电话"));
    contacts_model->setHeaderData(5, Qt::Horizontal, tr("手机"));
    contacts_model->setHeaderData(6, Qt::Horizontal, tr("短号"));
    contacts_model->setHeaderData(7, Qt::Horizontal, tr("QQ"));
    contacts_model->setHeaderData(8, Qt::Horizontal, tr("微信号"));
    contacts_model->setHeaderData(9, Qt::Horizontal, tr("电子邮箱"));
    contacts_model->setHeaderData(10, Qt::Horizontal, tr("单位"));
    contacts_model->setHeaderData(11, Qt::Horizontal, tr("备注"));
    while (contacts_model->canFetchMore())
        contacts_model->fetchMore();

    ui->list_contacts->setModel(contacts_model);
    ui->list_contacts->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_contacts->setColumnWidth(0, 80);
    ui->list_contacts->setColumnHidden(0, true);

    connect(ui->list_contacts->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_contacts_changed(const QModelIndex &)));
    connect(ui->list_contacts, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_contacts_changed(const QModelIndex &)));
    connect(ui->list_contacts, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(list_contacts_doubleclicked(const QModelIndex &)));
    /***** 通信簿列表 End *****/

    /***** 查询模块 *****/
    ui->combo_condition->addItem(tr("按姓名查询"), "name");
    ui->combo_condition->addItem(tr("按性别查询"), "sex");
    ui->combo_condition->addItem(tr("按手机查询"), "mPhone");
    connect(ui->btn_select, SIGNAL(clicked(bool)), this, SLOT(btn_select_clicked()));
    /***** 查询模块 End *****/

    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->btn_check, SIGNAL(clicked(bool)), this, SLOT(btn_check_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_update, SIGNAL(clicked(bool)), this, SLOT(btn_update_clicked()));

    ui->list_contacts->selectRow(0);
}

ContactsDialog::~ContactsDialog()
{
    delete contacts_model;
    delete ui;
}

void ContactsDialog::btn_add_clicked()
{
    NewContactDialog dialog(this);
    dialog.setEditorEnabled(true);
    if (dialog.exec() == QDialog::Accepted) {
        QString name, sex, fphone, ophone, mphone, mshort, qq, wechat, email, unit, remark;
        dialog.getEditorText(name, sex, fphone, ophone, mphone, mshort, qq, wechat, email, unit, remark);
        int rowNum = contacts_model->rowCount();
        contacts_model->insertRow(rowNum);
        contacts_model->setData(contacts_model->index(rowNum, 0), 0);
        contacts_model->setData(contacts_model->index(rowNum, 1), name);
        contacts_model->setData(contacts_model->index(rowNum, 2), sex);
        contacts_model->setData(contacts_model->index(rowNum, 3), fphone);
        contacts_model->setData(contacts_model->index(rowNum, 4), ophone);
        contacts_model->setData(contacts_model->index(rowNum, 5), mphone);
        contacts_model->setData(contacts_model->index(rowNum, 6), mshort);
        contacts_model->setData(contacts_model->index(rowNum, 7), qq);
        contacts_model->setData(contacts_model->index(rowNum, 8), wechat);
        contacts_model->setData(contacts_model->index(rowNum, 9), email);
        contacts_model->setData(contacts_model->index(rowNum, 10), unit);
        contacts_model->setData(contacts_model->index(rowNum, 11), remark);
        contacts_model->submitAll();
        contacts_model->select();
        QMessageBox::information(this, tr("提示"), tr("通信簿添加成功！"));
        ui->list_contacts->selectRow(rowNum);
    }
}

void ContactsDialog::btn_check_clicked()
{
    emit ui->list_contacts->doubleClicked(ui->list_contacts->currentIndex());
}

void ContactsDialog::btn_delete_clicked()
{
    if (contacts_model->rowCount() <= 0) // 空列表
        return;

    if (QMessageBox::warning(this, tr("删除通信信息"), tr("您是否确定删除当前选中的通信信息？"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_contacts->currentIndex().row(); // 获取选中行
        contacts_model->removeRow(nIndexRow--);
        contacts_model->submitAll();
        contacts_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_contacts->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_contacts->selectRow(nIndexRow);
        else
        {
            ui->btn_check->setEnabled(false);
            ui->btn_delete->setEnabled(false);
            ui->btn_update->setEnabled(false);
        }
    }
}

void ContactsDialog::btn_select_clicked()
{
    if (ui->edit_content->text().trimmed().isEmpty())
        contacts_model->setFilter("1");
    else
        contacts_model->setFilter(QString("%1 LIKE '%%%2%%'").arg(ui->combo_condition->currentData().toString(), ui->edit_content->text().trimmed()));
    contacts_model->select();

    if (contacts_model->rowCount() > 0) // 非空列表
        ui->list_contacts->selectRow(0);
    else
    {
        ui->btn_check->setEnabled(false);
        ui->btn_delete->setEnabled(false);
        ui->btn_update->setEnabled(false);
    }
}

void ContactsDialog::btn_update_clicked()
{
    NewContactDialog dialog(this);
    dialog.setEditorEnabled(true);
    dialog.setEditorText(
                contacts_model->index(ui->list_contacts->currentIndex().row(), 1).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 2).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 3).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 4).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 5).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 6).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 7).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 8).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 9).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 10).data().toString(),
                contacts_model->index(ui->list_contacts->currentIndex().row(), 11).data().toString());
    if (dialog.exec() == QDialog::Accepted) {
        QString name, sex, fphone, ophone, mphone, mshort, qq, wechat, email, unit, remark;
        dialog.getEditorText(name, sex, fphone, ophone, mphone, mshort, qq, wechat, email, unit, remark);

        QSqlQuery query = contacts_model->query();
        query.prepare("SELECT * FROM `contacts` WHERE `number` = :number");
        query.bindValue(":number", contacts_model->index(ui->list_contacts->currentIndex().row(), 0).data().toString());
        query.exec();

        // 修改通信簿
        if (query.size() == 0)
        {
            // 不存在通信记录
            QMessageBox::critical(this, tr("修改通信记录失败"), tr("该通信记录不存在！"));
            return;
        }
        int rowNum = ui->list_contacts->currentIndex().row(); // 获取选中行
        contacts_model->setData(contacts_model->index(rowNum, 1), name);
        contacts_model->setData(contacts_model->index(rowNum, 2), sex);
        contacts_model->setData(contacts_model->index(rowNum, 3), fphone);
        contacts_model->setData(contacts_model->index(rowNum, 4), ophone);
        contacts_model->setData(contacts_model->index(rowNum, 5), mphone);
        contacts_model->setData(contacts_model->index(rowNum, 6), mshort);
        contacts_model->setData(contacts_model->index(rowNum, 7), qq);
        contacts_model->setData(contacts_model->index(rowNum, 8), wechat);
        contacts_model->setData(contacts_model->index(rowNum, 9), email);
        contacts_model->setData(contacts_model->index(rowNum, 10), unit);
        contacts_model->setData(contacts_model->index(rowNum, 11), remark);
        contacts_model->submitAll();
        contacts_model->select();
        QMessageBox::information(this, tr("提示"), tr("通信簿修改成功！"));
        ui->list_contacts->selectRow(rowNum);
    }
}

void ContactsDialog::list_contacts_doubleclicked(const QModelIndex &current)
{
    NewContactDialog dialog(this);
    dialog.setEditorEnabled(false);
    dialog.setEditorText(
                contacts_model->index(current.row(), 1).data().toString(),
                contacts_model->index(current.row(), 2).data().toString(),
                contacts_model->index(current.row(), 3).data().toString(),
                contacts_model->index(current.row(), 4).data().toString(),
                contacts_model->index(current.row(), 5).data().toString(),
                contacts_model->index(current.row(), 6).data().toString(),
                contacts_model->index(current.row(), 7).data().toString(),
                contacts_model->index(current.row(), 8).data().toString(),
                contacts_model->index(current.row(), 9).data().toString(),
                contacts_model->index(current.row(), 10).data().toString(),
                contacts_model->index(current.row(), 11).data().toString());
    dialog.exec();
}

void ContactsDialog::list_contacts_changed(const QModelIndex &current)
{
    if (current.row() >= 0)
    {
        ui->btn_check->setEnabled(true);
        ui->btn_delete->setEnabled(true);
        ui->btn_update->setEnabled(true);
    }
}
