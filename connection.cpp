#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Project2A");
db.setUserName("root");//inserer nom de l'utilisateur
db.setPassword("root");//inserer mot de passe de cet utilisateur
//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

    return  test;
}
