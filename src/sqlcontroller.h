#ifndef SQLCONTROLLER_H
#define SQLCONTROLLER_H

#include <QtSql>
#include <QDebug>
#include <QProcess>
#include <QFile>

const QString DBS_HOSTNAME = "localhost";
const QString DBS_USERNAME = "root";
const QString DBS_PASSWORD = "root";
const int DBS_PORT = 3306;
const QString DBS_NAME = "postdb";
const QString DB_BACKUP_PATH = "PostDb_bak.sql";

class SQLController
{
public:
    SQLController(QString hostname = DBS_HOSTNAME, QString username = DBS_USERNAME, QString password = DBS_PASSWORD, int port = DBS_PORT, QString name = DBS_NAME);
    virtual ~SQLController();
    bool openDatabase(QString &error);
    QSqlDatabase getDefaultConnection();
    bool backupDatabase();
    bool recoverDatabase();

protected:
    QSqlDatabase db;

private:
    void init();
    QString db_hostname;
    QString db_username;
    QString db_password;
    int db_port;
    QString db_name;
    const QString defaultConnectionName = "self_mysql_default_connection";
};

#endif // SQLCONTROLLER_H
