#include "produit.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
PRODUIT::PRODUIT(int id,  QString nom,  int prix, int Quantite,  QByteArray image,  QDate DATE_DE_VALIDITE,  QString DISPONIBILITE,int ID_EMPLOYE) {
 this->id=id;
 this->nom=nom;
 this->prix=prix;
 this->disponibilite=DISPONIBILITE;
 this->image=image;
 this->DATE_DE_VALIDITE=DATE_DE_VALIDITE;
 this->Quantite=Quantite;
    this->ID_EMPLOYE=ID_EMPLOYE;
}

PRODUIT::PRODUIT()
    {
      id=0;
      nom="";
      prix=0;
      disponibilite="";
      DATE_DE_VALIDITE=QDate::currentDate();
      Quantite=0;
      ID_EMPLOYE=0;
}

int PRODUIT::getId()  {
    return id;
}

QString PRODUIT::getNom()  {
    return nom;
}

int PRODUIT::getprix()  {
    return prix;
}

QString PRODUIT::getdisponibilite()  {
    return disponibilite;
}

QByteArray PRODUIT::getimage()  {
    return image;
}

QDate PRODUIT::getDATE_DE_VALIDITE()  {
    return DATE_DE_VALIDITE;
}

int PRODUIT::getQuantite()  {
    return Quantite;
}
int PRODUIT::getID_EMPLOYE()  {
    return ID_EMPLOYE;
}


void PRODUIT::setID( int id) {
    this->id = id;
}
void PRODUIT::setNom( QString nom) {
    this->nom = nom;
}

void PRODUIT::setprix( int prix) {
    this->prix = prix;
}

void PRODUIT::setdisponibilite( QString disponibilite) {
    this->disponibilite = disponibilite;
}

void PRODUIT::setimage(QByteArray image) {
    this->image = image;
}



void PRODUIT::setDATE_DE_VALIDITE( QDate DATE_DE_VALIDITE) {
    this->DATE_DE_VALIDITE = DATE_DE_VALIDITE;
}

void PRODUIT::setQuantite(int QUANTITE)
 {
    this->Quantite= QUANTITE;
}

void PRODUIT::setID_EMPLOYE( int ID_EMPLOYE) {
    this->ID_EMPLOYE = ID_EMPLOYE;
}


bool PRODUIT::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO produit (ID_P, NOM_P, PRIX, QUANTITE, IMAGES, DATE_DE_VALIDITE, DISPONIBILITE,ID_EMPLOYE) "
                  "VALUES (:ID, :NOM, :PRIX, :QUANTITE,:image, :VALIDITE, :DISPONIBILITE,:ID_EMPLOYE)");

    query.bindValue(":ID", id);
    query.bindValue(":NOM", nom);
    query.bindValue(":PRIX", prix);
    query.bindValue(":QUANTITE", Quantite);
    query.bindValue(":image", image);
    query.bindValue(":VALIDITE", DATE_DE_VALIDITE);
    query.bindValue(":DISPONIBILITE", disponibilite);
    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);

    return query.exec();
}


QSqlQueryModel* PRODUIT::afficher()  {
    QSqlQueryModel* model = new QSqlQueryModel();
    // Exécutez la requête
    model->setQuery("SELECT * FROM produit");


    return model;
}

bool PRODUIT::produitExists(int id)  {
    QSqlQuery query;
    query.prepare("SELECT ID_P FROM produit WHERE ID_P = :ID");
    query.bindValue(":ID", id);
    if(query.exec() && query.next())
     {
      int count = query.value(0).toInt();
      return count >0 ;

    }
return false;
}


bool PRODUIT::supprimer(int id)
{
QSqlQuery query;
query.prepare("Delete from produit WHERE ID_P=:ID");
query.bindValue(0,id);
return query.exec();
}


bool PRODUIT::modifier(int id,  QString nom,  int prix, int QUANTITE,  QByteArray image,  QDate DATE_DE_VALIDITE,  QString DISPONIBILITE,int ID_EMPLOYE) {
    QSqlQuery query;
    query.prepare("UPDATE  produit SET  NOM_P=:NOM_P, PRIX=:PRIX, QUANTITE=:QUANTITE, IMAGES=:IMAGES, DATE_DE_VALIDITE=:DATE_DE_VALIDITE, DISPONIBILITE=:DISPONIBILITE,ID_EMPLOYE=:ID_EMPLOYE WHERE ID_P=:ID_P ");
    query.bindValue(":ID_P", id);
    query.bindValue(":NOM_P", nom);
    query.bindValue(":PRIX", prix);
    query.bindValue(":QUANTITE", QUANTITE);
    query.bindValue(":IMAGES", image);
    query.bindValue(":DATE_DE_VALIDITE", DATE_DE_VALIDITE);
    query.bindValue(":DISPONIBILITE", DISPONIBILITE);
    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);
    return query.exec();
}


QSqlQueryModel* PRODUIT::tri(QString column,QString ordre)
{
QSqlQueryModel* model = new QSqlQueryModel();
model->setQuery("SELECT * FROM produit ORDER BY " + column + " "+ordre);

return model;
}



QSqlQueryModel* PRODUIT::chercher(QString column, QString text)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM PRODUIT WHERE " + column + " LIKE '%" + text + "%'");

    return model;
}

int PRODUIT::countDisponibilite(const QString& dispo)
 {
     int count = 0;

     QSqlQuery query;
     query.prepare("SELECT COUNT(DISPONIBILITE) FROM produit WHERE DISPONIBILITE = :dispo");
     query.bindValue(":dispo", dispo);

     if (query.exec() && query.next()) {
         count = query.value(0).toInt();
     }

     return count;
 }
