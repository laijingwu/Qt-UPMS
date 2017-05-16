#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QtSql>
#include "sqlcontroller.h"

class SQLModel : public SQLController
{
private:
    SQLModel();
    static SQLModel* m_instance;
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (SQLModel::m_instance)
                delete SQLModel::m_instance;
        }
    };
    static CGarbo Garbo;
public:
    static SQLModel* instance()
    {
        if (m_instance == NULL)
            m_instance = new SQLModel();
        return m_instance;
    }
    QString m_username;
    bool m_bIsAdministrator;
    bool verifyPassword(QString username, QString password);
};

#endif // SQLMODEL_H
