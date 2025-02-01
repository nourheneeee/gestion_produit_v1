#ifndef PRODUIT_H
#define PRODUIT_H

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QDate>


class PRODUIT {
public:
     PRODUIT();
    PRODUIT(int id,  QString nom,  int prix, int Quantite,  QByteArray image,  QDate DATE_DE_VALIDITE,  QString DISPONIBILITE,int ID_EMPLOYE);
    // Getters
    int getId() ;
    QString getNom() ;
    int getprix() ;
    QString getdisponibilite() ;
    QByteArray getimage() ;
    QDate getDATE_DE_VALIDITE() ;
    int getQuantite() ;
    int getID_EMPLOYE();
    // Setters
    void setID(int id);
    void setNom( QString nom);

    void setID_EMPLOYE( int ID_EMPLOYE);
    void setprix( int prix);
    void setdisponibilite(QString disponibilite);
    void setimage( QByteArray image);
    void setDATE_DE_VALIDITE( QDate DATE_DE_VALIDITE);
    void setQuantite(int Quantite);


    // CRUD
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id,  QString nom,  int prix, int QUANTITE,  QByteArray image,  QDate DATE_DE_VALIDITE,  QString DISPONIBILITE,int ID_EMPLOYE);
    QSqlQueryModel* afficher() ;
    // Méthodes auxiliaires
    bool produitExists(int id) ;

    QSqlQueryModel* chercher(QString column,QString text);
    QSqlQueryModel* tri(QString column,QString ordre);
    int countDisponibilite(const QString& dispo);


private:
    // Attributs
    int id;
    QString nom;
    int prix;
    QString disponibilite;
     QByteArray image;
    QDate DATE_DE_VALIDITE;
    int Quantite;
    int ID_EMPLOYE;
};



#endif // Produits_H
