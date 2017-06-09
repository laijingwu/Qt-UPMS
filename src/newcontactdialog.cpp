#include "newcontactdialog.h"
#include "ui_newcontactdialog.h"

NewContactDialog::NewContactDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::NewContactDialog)
{
    ui->setupUi(this);
    connect(ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(btn_ok_clicked()));
}

NewContactDialog::~NewContactDialog()
{
    delete ui;
}

void NewContactDialog::closeEvent(QCloseEvent *event)
{
    if (!ui->btn_ok->isEnabled())
    {
        event->accept();
        return;
    }

    if (!ui->edit_name->text().isEmpty())
    {
        if (QMessageBox::question(this, tr("提示"), tr("关闭当前窗口将会失去已输入的内容，您确定要离开该窗口？")) == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
}

void NewContactDialog::btn_ok_clicked()
{
    if (ui->edit_name->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("姓名不能为空！"));
        ui->edit_name->setFocus();
        return;
    }
    accept();
}

void NewContactDialog::setEditorEnabled(bool enabled)
{
    ui->edit_name->setEnabled(enabled);
    ui->combo_sex->setEnabled(enabled);
    ui->edit_fphone->setEnabled(enabled);
    ui->edit_ophone->setEnabled(enabled);
    ui->edit_mphone->setEnabled(enabled);
    ui->edit_short->setEnabled(enabled);
    ui->edit_qq->setEnabled(enabled);
    ui->edit_wechat->setEnabled(enabled);
    ui->edit_email->setEnabled(enabled);
    ui->edit_unit->setEnabled(enabled);
    ui->edit_remark->setEnabled(enabled);
    ui->btn_ok->setEnabled(enabled);
}

void NewContactDialog::setEditorText(QString name, QString sex, QString fphone, QString ophone, QString mphone,
                                     QString mshort, QString qq, QString wechat, QString email, QString unit, QString remark)
{
    ui->edit_name->setText(name);
    ui->combo_sex->setCurrentText(sex);
    ui->edit_fphone->setText(fphone);
    ui->edit_ophone->setText(ophone);
    ui->edit_mphone->setText(mphone);
    ui->edit_short->setText(mshort);
    ui->edit_qq->setText(qq);
    ui->edit_wechat->setText(wechat);
    ui->edit_email->setText(email);
    ui->edit_unit->setText(unit);
    ui->edit_remark->setPlainText(remark);
}

void NewContactDialog::getEditorText(QString &name, QString &sex, QString &fphone, QString &ophone, QString &mphone,
                                     QString &mshort, QString &qq, QString &wechat, QString &email, QString &unit, QString &remark)
{
    name = ui->edit_name->text().trimmed();
    sex = ui->combo_sex->currentText();
    fphone = ui->edit_fphone->text().trimmed();
    ophone = ui->edit_ophone->text().trimmed();
    mphone = ui->edit_mphone->text().trimmed();
    mshort = ui->edit_short->text().trimmed();
    qq = ui->edit_qq->text().trimmed();
    wechat = ui->edit_wechat->text().trimmed();
    email = ui->edit_email->text().trimmed();
    unit = ui->edit_unit->text().trimmed();
    remark = ui->edit_remark->toPlainText().trimmed();
}
