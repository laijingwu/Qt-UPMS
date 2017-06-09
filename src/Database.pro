#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T16:57:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Database
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    sqlcontroller.cpp \
    usersettingdialog.cpp \
    aboutdialog.cpp \
    addbaseinfodialog.cpp \
    nationinformationdialog.cpp \
    sqlmodel.cpp \
    positioninformationdialog.cpp \
    educationdegreedialog.cpp \
    politicalstatusdialog.cpp \
    departmentsectiondialog.cpp \
    wageleveldialog.cpp \
    technicalpostdialog.cpp \
    administrativepostdialog.cpp \
    departmentdialog.cpp \
    sectiondialog.cpp \
    auxiliarydialog.cpp \
    appendantdialog.cpp \
    fulltimedialog.cpp \
    humanbrowsedialog.cpp \
    humansearchdialog.cpp \
    humanstatisticsdialog.cpp \
    notesdialog.cpp \
    contactsdialog.cpp \
    newcontactdialog.cpp \
    meetingdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    sqlcontroller.h \
    usersettingdialog.h \
    aboutdialog.h \
    addbaseinfodialog.h \
    nationinformationdialog.h \
    sqlmodel.h \
    positioninformationdialog.h \
    educationdegreedialog.h \
    politicalstatusdialog.h \
    departmentsectiondialog.h \
    wageleveldialog.h \
    technicalpostdialog.h \
    administrativepostdialog.h \
    departmentdialog.h \
    sectiondialog.h \
    auxiliarydialog.h \
    appendantdialog.h \
    fulltimedialog.h \
    humanbrowsedialog.h \
    humansearchdialog.h \
    humanstatisticsdialog.h \
    notesdialog.h \
    contactsdialog.h \
    newcontactdialog.h \
    meetingdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    usersettingdialog.ui \
    aboutdialog.ui \
    addbaseinfodialog.ui \
    nationinformationdialog.ui \
    positioninformationdialog.ui \
    educationdegreedialog.ui \
    politicalstatusdialog.ui \
    departmentsectiondialog.ui \
    wageleveldialog.ui \
    technicalpostdialog.ui \
    administrativepostdialog.ui \
    departmentdialog.ui \
    sectiondialog.ui \
    auxiliarydialog.ui \
    appendantdialog.ui \
    fulltimedialog.ui \
    humanbrowsedialog.ui \
    humansearchdialog.ui \
    humanstatisticsdialog.ui \
    notesdialog.ui \
    contactsdialog.ui \
    newcontactdialog.ui \
    meetingdialog.ui

RESOURCES += \
    res.qrc

RC_FILE += Database.rc
