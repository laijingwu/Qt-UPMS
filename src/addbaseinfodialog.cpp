#include "addbaseinfodialog.h"
#include "ui_addbaseinfodialog.h"

AddBaseInfoDialog::AddBaseInfoDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AddBaseInfoDialog)
{
    ui->setupUi(this);
    connect(ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(btn_ok_clicked()));
}

AddBaseInfoDialog::~AddBaseInfoDialog()
{
    delete ui;
}

void AddBaseInfoDialog::setStaticUi(QString staticBox_label, QString staticLabel)
{
    ui->box_static->setTitle(staticBox_label);
    ui->label_static->setText(staticLabel);
}

void AddBaseInfoDialog::btn_ok_clicked()
{
    if (ui->edit_static->text().trimmed().isEmpty())
    {
        QString label = ui->label_static->text();
        label.chop(1);
        QMessageBox::warning(this, tr("提示"), label + tr("不能为空！"));
        ui->edit_static->setFocus();
        return;
    }
    accept();
}

QString AddBaseInfoDialog::getStaticText()
{
    return ui->edit_static->text().trimmed();
}

QString AddBaseInfoDialog::getDescriptionText()
{
    return ui->edit_description->toPlainText().trimmed();
}

void AddBaseInfoDialog::closeEvent(QCloseEvent *event)
{
    if (!ui->edit_static->text().isEmpty() || !ui->edit_description->toPlainText().isEmpty())
    {
        if (QMessageBox::question(this, tr("提示"), tr("关闭当前窗口将会失去已输入的内容，您确定要离开该窗口？")) == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
}
