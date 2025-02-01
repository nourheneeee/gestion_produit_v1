#ifndef PROMOTION_H
#define PROMOTION_H
#include <QString>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts>
#include <QSqlQuery>
#include <QMap>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>




#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QPainter>
class promotion
{
    QString ID_PROMO;
    QString NOM_PROMO;
    QString POURCENTAGE;
    QString DATE_DEBUT;
    QString DATE_FIN;
    QString CODE_PROMO;
    QString CATEGORIE;
    QString STATUT;

public:
    promotion();
    promotion(QString ID_PROMO, QString NOM_PROMO,QString POURCENTAGE,QString DATE_DEBUT,QString DATE_FIN,QString CODE_PROMO,QString CATEGORIE,QString STATUT);
    void ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString id_equipements);
    void modifier();
    QSqlQueryModel* tri();
    bool idExists(int id);

    QChartView* createPieChartByStatut() ;
    void exportPromotionToPDF() ;
    QSqlQueryModel* rechercher(const QString& value);
};

#endif // PROMOTION_H
