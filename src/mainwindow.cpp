#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    SqlController(SQLModel::instance())
{
    ui->setupUi(this);

    statusBar_titleLabel = new QLabel(this);
    statusBar_titleLabel->setText(tr("高校人事信息数据库应用系统") + QString("  [已登录用户: %1]").arg(SqlController->m_username));
    statusBar_titleLabel->setFrameStyle(QFrame::WinPanel|QFrame::Sunken);
    statusBar_titleLabel->setFixedWidth(350);
    statusBar_currentTimeLabel = new QLabel(this);
    statusBar_currentTimeLabel->setFrameStyle(QFrame::WinPanel|QFrame::Sunken);
    statusBar_timer = new QTimer(this);
    connect(statusBar_timer, SIGNAL(timeout()), this, SLOT(statusBar_slotTimeUpdate()));
    statusBar_timer->start(1000);
    ui->statusBar->addPermanentWidget(statusBar_titleLabel);
    ui->statusBar->addPermanentWidget(statusBar_currentTimeLabel);

    connect(ui->actionSysReload, SIGNAL(triggered(bool)), this, SLOT(slotSysReload()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(slotAbout()));
    connect(ui->actionHelpDocument, SIGNAL(triggered(bool)), this, SLOT(slotHelpDocument()));
    connect(ui->actionHumanBrowse, SIGNAL(triggered(bool)), this, SLOT(slotHumanBrowse()));
    connect(ui->actionHumanQuery, SIGNAL(triggered(bool)), this, SLOT(slotHumanQuery()));
    connect(ui->actionHumanStatistics, SIGNAL(triggered(bool)), this, SLOT(slotHumanStatistics()));
    connect(ui->actionToolNotes, SIGNAL(triggered(bool)), this, SLOT(slotToolNotes()));
    connect(ui->actionToolContacts, SIGNAL(triggered(bool)), this, SLOT(slotToolContacts()));
    connect(ui->actionToolMeeting, SIGNAL(triggered(bool)), this, SLOT(slotToolMeeting()));

    if (!SqlController->m_bIsAdministrator) // 非管理员
    {
        ui->actionSysUserSetting->setEnabled(false);
        ui->actionSysDbBackup->setEnabled(false);
        ui->actionSysDbRecover->setEnabled(false);
        ui->actionInfNation->setEnabled(false);
        ui->actionInfPosition->setEnabled(false);
        ui->actionInfEducation->setEnabled(false);
        ui->actionInfPolitical->setEnabled(false);
        ui->actionInfDept->setEnabled(false);
        ui->actionInfWage->setEnabled(false);
        ui->actionInfTechnical->setEnabled(false);
        ui->actionInfAdmin->setEnabled(false);
        ui->actionOrgDept->setEnabled(false);
        ui->actionOrgSection->setEnabled(false);
        ui->actionOrgAuxiliary->setEnabled(false);
        ui->actionOrgAppendant->setEnabled(false);
        ui->actionOrgFulltime->setEnabled(false);
    }
    else
    {
        connect(ui->actionSysUserSetting, SIGNAL(triggered(bool)), this, SLOT(slotSysUserSetting()));
        connect(ui->actionSysDbBackup, SIGNAL(triggered(bool)), this, SLOT(slotSysDbBackup()));
        connect(ui->actionSysDbRecover, SIGNAL(triggered(bool)), this, SLOT(slotSysDbRecover()));
        connect(ui->actionInfNation, SIGNAL(triggered(bool)), this, SLOT(slotInfNation()));
        connect(ui->actionInfPosition, SIGNAL(triggered(bool)), this, SLOT(slotInfPosition()));
        connect(ui->actionInfEducation, SIGNAL(triggered(bool)), this, SLOT(slotInfEducation()));
        connect(ui->actionInfPolitical, SIGNAL(triggered(bool)), this, SLOT(slotInfPolitical()));
        connect(ui->actionInfDept, SIGNAL(triggered(bool)), this, SLOT(slotInfDept()));
        connect(ui->actionInfWage, SIGNAL(triggered(bool)), this, SLOT(slotInfWage()));
        connect(ui->actionInfTechnical, SIGNAL(triggered(bool)), this, SLOT(slotInfTechnical()));
        connect(ui->actionInfAdmin, SIGNAL(triggered(bool)), this, SLOT(slotInfAdmin()));
        connect(ui->actionOrgDept, SIGNAL(triggered(bool)), this, SLOT(slotOrgDept()));
        connect(ui->actionOrgSection, SIGNAL(triggered(bool)), this, SLOT(slotOrgSection()));
        connect(ui->actionOrgAuxiliary, SIGNAL(triggered(bool)), this, SLOT(slotOrgAuxiliary()));
        connect(ui->actionOrgAppendant, SIGNAL(triggered(bool)), this, SLOT(slotOrgAppendant()));
        connect(ui->actionOrgFulltime, SIGNAL(triggered(bool)), this, SLOT(slotOrgFulltime()));
    }
}

MainWindow::~MainWindow()
{
    statusBar_timer->stop();
    delete statusBar_timer;
    delete statusBar_currentTimeLabel;
    delete statusBar_titleLabel;
    delete ui;
}

void MainWindow::statusBar_slotTimeUpdate()
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("yyyy-MM-dd  hh:mm:ss"); // 设置显示的格式
    statusBar_currentTimeLabel->setText(Timestr);
}

void MainWindow::slotSysReload()
{
    if (QMessageBox::question(this, tr("重新登录"), tr("您确定真的要退出系统以重新登录吗？")) == QMessageBox::Yes)
        qApp->exit(EXIT_RESTART);
    else
        return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, tr("退出系统"), tr("您确定真的要退出系统吗？")) == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::slotSysUserSetting()
{
    UserSettingDialog dialog_usersetting(this);
    dialog_usersetting.exec();
}

void MainWindow::slotAbout()
{
    AboutDialog dialog_about(this);
    dialog_about.exec();
}

void MainWindow::slotSysDbBackup()
{
    if (QMessageBox::question(this, tr("数据库备份"), tr("您是否要对本系统的数据库进行备份？")) == QMessageBox::No)
        return;

    if (SqlController->backupDatabase())
        QMessageBox::information(this, tr("数据库备份"), tr("数据库备份成功！"));
    else
        QMessageBox::critical(this, tr("数据库备份"), tr("数据库备份失败！"));
}

void MainWindow::slotSysDbRecover()
{
    if (QMessageBox::question(this, tr("数据库还原"), tr("您确定是否要对进行还原？\n系统的后台数据库将会被此副本覆盖，请谨慎操作！")) == QMessageBox::No)
        return;

    if (SqlController->recoverDatabase())
        QMessageBox::information(this, tr("数据库还原"), tr("数据库还原成功！"));
    else
        QMessageBox::critical(this, tr("数据库还原"), tr("数据库还原失败！"));
}

void MainWindow::slotInfNation()
{
    NationInformationDialog dialog_nationinf(this);
    dialog_nationinf.exec();
}

void MainWindow::slotInfPosition()
{
    PositionInformationDialog dialog_positioninf(this);
    dialog_positioninf.exec();
}

void MainWindow::slotInfEducation()
{
    EducationDegreeDialog dialog_education(this);
    dialog_education.exec();
}

void MainWindow::slotInfPolitical()
{
    PoliticalStatusDialog dialog_political(this);
    dialog_political.exec();
}

void MainWindow::slotInfDept()
{
    DepartmentSectionDialog dialog_department(this);
    dialog_department.exec();
}

void MainWindow::slotInfWage()
{
    WageLevelDialog dialog_wage(this);
    dialog_wage.exec();
}

void MainWindow::slotInfTechnical()
{
    TechnicalPostDialog dialog_technical(this);
    dialog_technical.exec();
}

void MainWindow::slotInfAdmin()
{
    AdministrativePostDialog dialog_admin(this);
    dialog_admin.exec();
}

void MainWindow::slotOrgDept()
{
    DepartmentDialog dialog_department(this);
    dialog_department.exec();
}

void MainWindow::slotOrgSection()
{
    SectionDialog dialog_section(this);
    dialog_section.exec();
}

void MainWindow::slotOrgAuxiliary()
{
    AuxiliaryDialog dialog_auxiliary(this);
    dialog_auxiliary.exec();
}

void MainWindow::slotOrgAppendant()
{
    AppendantDialog dialog_appendant(this);
    dialog_appendant.exec();
}

void MainWindow::slotOrgFulltime()
{
   FulltimeDialog dialog_fulltime(this);
   dialog_fulltime.exec();
}

void MainWindow::slotHumanBrowse()
{
    HumanBrowseDialog dialog_browse(this);
    dialog_browse.exec();
}

void MainWindow::slotHelpDocument()
{
    QProcess *p_helpProcess = new QProcess(this);
    QStringList argument("help.chm");
    p_helpProcess->start("hh.exe", argument);
}

void MainWindow::slotHumanQuery()
{
    HumanSearchDialog dialog_search(this);
    dialog_search.exec();
}

void MainWindow::slotHumanStatistics()
{
    HumanStatisticsDialog dialog_statistics(this);
    dialog_statistics.exec();
}

void MainWindow::slotToolNotes()
{
    NotesDialog dialog_notes(this);
    dialog_notes.exec();
}

void MainWindow::slotToolContacts()
{
    ContactsDialog dialog_contacts(this);
    dialog_contacts.exec();
}

void MainWindow::slotToolMeeting()
{
    MeetingDialog dialog_meeting(this);
    dialog_meeting.exec();
}
