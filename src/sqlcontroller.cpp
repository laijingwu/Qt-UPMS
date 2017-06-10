#include "sqlcontroller.h"

SQLController::SQLController(QString hostname, QString username, QString password, int port, QString name) :
    db_hostname(hostname), db_username(username), db_password(password), db_port(port), db_name(name)
{
    init();
}

SQLController::~SQLController()
{
    if (db.isOpen())
        db.close();
}

void SQLController::init() {
    if (QSqlDatabase::contains(defaultConnectionName))
    {
        db = getDefaultConnection();
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL", defaultConnectionName);
        db.setHostName(db_hostname);
        db.setPort(db_port);
        db.setUserName(db_username);
        db.setPassword(db_password);
        db.setDatabaseName(db_name);
    }
}

bool SQLController::openDatabase(QString &error)
{
    if (!QSqlDatabase::contains(defaultConnectionName))
        init();

    if (db.isOpen()) return true;
    if (!db.open())
    {
        QSqlError err;
        err = db.lastError();
        error = err.text();
        QSqlDatabase::removeDatabase(defaultConnectionName);
        qDebug() << err.text();
        return false;
    }
    return true;
}

QSqlDatabase SQLController::getDefaultConnection()
{
    return QSqlDatabase::database(defaultConnectionName);
}

bool SQLController::backupDatabase()
{
    QFile backupFile(DB_BACKUP_PATH);
    if (backupFile.exists())
        backupFile.remove();

    QProcess poc;
    poc.setStandardOutputFile(DB_BACKUP_PATH);
    poc.start(QString("mysqldump.exe --add-drop-table -u%1 -p%2 %3").arg(db_username, db_password, db_name));
    poc.waitForFinished(-1);
    if (backupFile.size() > 0)
        return true;
    else
        return false;
}

bool SQLController::recoverDatabase()
{
    QFile backupFile(DB_BACKUP_PATH);
    if (!backupFile.exists() || backupFile.size() <= 0)
        return false;

    QProcess poc;
    poc.setStandardInputFile(DB_BACKUP_PATH);
    poc.start(QString("mysqldump.exe -u%1 -p%2 %3").arg(db_username, db_password, db_name));
    poc.waitForFinished(-1);
    return true;
}
