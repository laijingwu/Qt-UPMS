#include "sqlmodel.h"

SQLModel::SQLModel() : m_bIsAdministrator(false)
{
    QString error;
    openDatabase(error);
}

bool SQLModel::verifyPassword(QString username, QString password)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM `user` WHERE `customer` = :username AND `password` = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();
    if (query.size() > 0)
    {
        query.first();
        m_username = query.value("customer").toString();
        m_bIsAdministrator = query.value("privilege").toBool();
        return true;
    }
    else
        return false;
}
