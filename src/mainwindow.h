#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include "sqlmodel.h"
#include "usersettingdialog.h"
#include "aboutdialog.h"
#include "nationinformationdialog.h"
#include "positioninformationdialog.h"
#include "educationdegreedialog.h"
#include "politicalstatusdialog.h"
#include "departmentsectiondialog.h"
#include "wageleveldialog.h"
#include "technicalpostdialog.h"
#include "administrativepostdialog.h"
#include "departmentdialog.h"
#include "sectiondialog.h"
#include "auxiliarydialog.h"
#include "appendantdialog.h"
#include "fulltimedialog.h"
#include "humanbrowsedialog.h"
#include "humansearchdialog.h"
#include "humanstatisticsdialog.h"
#include "notesdialog.h"
#include "contactsdialog.h"
#include "meetingdialog.h"

#define EXIT_RESTART 773

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QLabel *statusBar_currentTimeLabel;
    QLabel *statusBar_titleLabel;
    QTimer *statusBar_timer;
    SQLModel *SqlController;

private slots:
    void statusBar_slotTimeUpdate();
    void slotSysReload();
    void slotSysUserSetting();
    void slotAbout();
    void slotSysDbBackup();
    void slotSysDbRecover();
    void slotInfNation();
    void slotInfPosition();
    void slotInfEducation();
    void slotInfPolitical();
    void slotInfDept();
    void slotInfWage();
    void slotInfTechnical();
    void slotInfAdmin();
    void slotOrgDept();
    void slotOrgSection();
    void slotOrgAuxiliary();
    void slotOrgAppendant();
    void slotOrgFulltime();
    void slotHumanBrowse();
    void slotHelpDocument();
    void slotHumanQuery();
    void slotHumanStatistics();
    void slotToolNotes();
    void slotToolContacts();
    void slotToolMeeting();
};

#endif // MAINWINDOW_H
