#include "usersettingdialog.h"
#include "ui_usersettingdialog.h"

UserSettingDialog::UserSettingDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserSettingDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    user_model = new QSqlTableModel(this, SqlController->getDefaultConnection());
    user_model->setTable("user");
    user_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    user_model->select();
    user_model->setHeaderData(0, Qt::Horizontal, tr("用户名"));
    while (user_model->canFetchMore())
        user_model->fetchMore();

    ui->list_user->setModel(user_model);
    ui->list_user->setColumnWidth(0, 150);
    ui->list_user->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->list_user->setColumnHidden(1, true);
    ui->list_user->setColumnHidden(2, true);

    connect(ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(btn_cancel_clicked()));
    connect(ui->btn_delete, SIGNAL(clicked(bool)), this, SLOT(btn_delete_clicked()));
    connect(ui->btn_add, SIGNAL(clicked(bool)), this, SLOT(btn_add_clicked()));
    connect(ui->list_user->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(list_user_changed(const QModelIndex &)));
    connect(ui->list_user, SIGNAL(clicked(const QModelIndex &)), this, SLOT(list_user_changed(const QModelIndex &)));
    connect(ui->btn_save, SIGNAL(clicked(bool)), this, SLOT(btn_save_clicked()));
    ui->list_user->selectRow(0);
}

UserSettingDialog::~UserSettingDialog()
{
    delete user_model;
    delete ui;
}

void UserSettingDialog::btn_cancel_clicked()
{
    ui->edit_username->clear();
    ui->edit_password->clear();
    ui->edit_password_confirm->clear();
    ui->check_administrator->setChecked(false);
    ui->edit_username->setEnabled(false);
    ui->list_user->clearFocus();
}

void UserSettingDialog::btn_delete_clicked()
{
    if (ui->edit_username->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("请选择一个已经存在的用户！"));
        return;
    }

    if (QMessageBox::warning(this, tr("删除用户"), tr("您是否确定删除当前选中的用户？\n删除用户将会级联删除所有用户的数据，请谨慎操作。"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int nIndexRow = ui->list_user->currentIndex().row(); // 获取选中行
        user_model->removeRow(nIndexRow--);
        user_model->submitAll();
        user_model->select();
        if (nIndexRow < 0) nIndexRow = 0; // 防止越界
        if (ui->list_user->horizontalHeader()->count() > nIndexRow) // 列表判空
            ui->list_user->selectRow(nIndexRow);
        else
        {
            // 空列表 清空输入框
            ui->edit_username->clear();
            ui->edit_password->clear();
            ui->edit_password_confirm->clear();
            ui->check_administrator->setChecked(false);
        }
    }
}

void UserSettingDialog::btn_add_clicked()
{
    ui->edit_username->clear();
    ui->edit_password->clear();
    ui->edit_password_confirm->clear();
    ui->check_administrator->setChecked(false);
    ui->edit_username->setEnabled(true);
}

void UserSettingDialog::list_user_changed(const QModelIndex &current)
{
    ui->edit_username->setText(user_model->index(current.row(), 0).data().toString());
    ui->edit_password->setText(user_model->index(current.row(), 1).data().toString());
    ui->edit_password_confirm->setText(user_model->index(current.row(), 1).data().toString());
    ui->check_administrator->setChecked(user_model->index(current.row(), 2).data().toBool());
}

void UserSettingDialog::btn_save_clicked()
{
    if (ui->edit_username->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("新增用户"), tr("请输入用户名！"));
        ui->edit_username->setFocus();
        return;
    }
    if (ui->edit_password->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("新增用户"), tr("密码不能为空，请输入密码！"));
        ui->edit_password->setFocus();
        return;
    }
    if (ui->edit_password_confirm->text().trimmed() != ui->edit_password->text().trimmed())
    {
        QMessageBox::warning(this, tr("新增用户"), tr("两次输入的密码不一致，请重新输入密码！"));
        ui->edit_password->clear();
        ui->edit_password_confirm->clear();
        ui->edit_password->setFocus();
        return;
    }

    QSqlQuery query = user_model->query();
    query.prepare("SELECT * FROM `user` WHERE `customer` = :username");
    query.bindValue(":username", ui->edit_username->text().trimmed());
    query.exec();

    if (ui->edit_username->isEnabled())
    {
        // 新增用户
        if (query.size() > 0)
        {
            // 存在用户
            QMessageBox::critical(this, tr("新增用户失败"), tr("该用户已存在！"));
            ui->edit_username->clear();
            ui->edit_password->clear();
            ui->edit_password_confirm->clear();
            ui->edit_username->setFocus();
            return;
        }
        int rowNum = user_model->rowCount();
        user_model->insertRow(rowNum);
        user_model->setData(user_model->index(rowNum, 0), ui->edit_username->text().trimmed());
        user_model->setData(user_model->index(rowNum, 1), ui->edit_password->text().trimmed());
        user_model->setData(user_model->index(rowNum, 2), ui->check_administrator->isChecked());
        user_model->submitAll();
        user_model->select();
        QMessageBox::information(this, tr("提示"), tr("用户添加成功！请记住用户名和密码！"));
        ui->list_user->selectRow(rowNum);
    }
    else
    {
        // 修改用户
        if (query.size() == 0)
        {
            // 不存在用户
            QMessageBox::critical(this, tr("修改用户失败"), tr("该用户不存在！"));
            return;
        }
        int nIndexRow = ui->list_user->currentIndex().row(); // 获取选中行
        user_model->setData(user_model->index(nIndexRow, 1), ui->edit_password->text().trimmed());
        user_model->setData(user_model->index(nIndexRow, 2), ui->check_administrator->isChecked());
        user_model->submitAll();
        user_model->select();
        QMessageBox::information(this, tr("提示"), tr("用户修改成功！请记住用户名和密码！"));
        ui->list_user->selectRow(nIndexRow);
    }
}
