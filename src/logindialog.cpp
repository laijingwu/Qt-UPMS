#include "logindialog.h"
#include "ui_logindialog.h"

SQLModel* SQLModel::m_instance = NULL;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::LoginDialog),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    // 读取用户
    QSqlQuery user_query("SELECT customer FROM `user`", SqlController->getDefaultConnection());
    while (user_query.next())
        ui->edit_username->addItem(user_query.value(0).toString());
    ui->edit_username->setCurrentText("");

    connect(ui->btn_login, SIGNAL(clicked(bool)), this, SLOT(btn_login()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::btn_login()
{
    if (ui->edit_username->currentText().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("登录"), tr("用户名不能为空，请输入用户名！"));
        ui->edit_username->setFocus();
        return;
    }
    if (ui->edit_password->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("登录"), tr("密码不能为空，请输入正确的密码！"));
        ui->edit_password->setFocus();
        return;
    }

    qDebug() << "User " << ui->edit_username->currentText().trimmed() << " try to login.";
    if (SqlController->verifyPassword(ui->edit_username->currentText().trimmed(), ui->edit_password->text().trimmed()))
    {
        // 密码正确
        qDebug() << "User " << ui->edit_username->currentText().trimmed() << " login successfully.";
        qDebug() << "Is Administrator? " << SqlController->m_bIsAdministrator;
        accept();
    }
    else
    {
        // 密码错误
        QMessageBox::critical(this, NULL, tr("用户名或密码错误，请重新输入！"));
        ui->edit_password->clear();
        ui->edit_password->setFocus();
        return;
    }
}
