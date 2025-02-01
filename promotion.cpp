#include "promotion.h"
#include <QSqlQuery>
#include <QSqlError>
promotion::promotion()
{

}
promotion::promotion(QString ID_PROMO, QString NOM_PROMO,QString POURCENTAGE,QString DATE_DEBUT,QString DATE_FIN,QString CODE_PROMO,QString CATEGORIE,QString STATUT)
{
    this->ID_PROMO=ID_PROMO;
    this->NOM_PROMO=NOM_PROMO;
    this->POURCENTAGE=POURCENTAGE;
    this->DATE_DEBUT=DATE_DEBUT;
    this->DATE_FIN=DATE_FIN;
    this->CODE_PROMO=CODE_PROMO;
    this->CATEGORIE=CATEGORIE;
    this->STATUT=STATUT;
}
void promotion::ajouter()
{
    QSqlQuery query;
    query.prepare("insert into PROMOTION (ID_PROMO,NOM_PROMO,POURCENTAGE,DATE_DEBUT,DATE_FIN,CODE_PROMO,CATEGORIE,STATUT) values (:ID_PROMO,:NOM_PROMO,:POURCENTAGE,:DATE_DEBUT,:DATE_FIN,:CODE_PROMO,:CATEGORIE,:STATUT)");//requete bch najouti lel base les attributs
    query.bindValue(":ID_PROMO",ID_PROMO);
    query.bindValue(":NOM_PROMO", NOM_PROMO);
    query.bindValue(":POURCENTAGE", POURCENTAGE);
    query.bindValue(":DATE_DEBUT", DATE_DEBUT);
    query.bindValue(":DATE_FIN", DATE_FIN);
    query.bindValue(":CODE_PROMO", CODE_PROMO);
    query.bindValue(":CATEGORIE", CATEGORIE);
    query.bindValue(":STATUT", STATUT);
    query.exec();
}
QSqlQueryModel * promotion::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();
    model->setQuery("select * from PROMOTION");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PROMO"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_PROMO"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("POURCENTAGE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_DEBUT"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DATE_FIN"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CODE_PROMO"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("CATEGORIE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("STATUT"));
    return model;
}
bool promotion::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PROMOTION WHERE ID_PROMO = :ID_PROMO");
    query.bindValue(":ID_PROMO", id);

    // Execute the query and return true if successful, false otherwise
    return query.exec();
}

void promotion::modifier()
{
    QSqlQuery query;
          query.prepare("update PROMOTION set NOM_PROMO=:NOM_PROMO, POURCENTAGE=:POURCENTAGE, DATE_DEBUT=:DATE_DEBUT, DATE_FIN=:DATE_FIN, CODE_PROMO=:CODE_PROMO, CATEGORIE=:CATEGORIE, STATUT=:STATUT where ID_PROMO=:ID_PROMO");
          query.bindValue(":ID_PROMO",ID_PROMO);
          query.bindValue(":NOM_PROMO", NOM_PROMO);
          query.bindValue(":POURCENTAGE", POURCENTAGE);
          query.bindValue(":DATE_DEBUT", DATE_DEBUT);
          query.bindValue(":DATE_FIN", DATE_FIN);
          query.bindValue(":CODE_PROMO", CODE_PROMO);
          query.bindValue(":CATEGORIE", CATEGORIE);
          query.bindValue(":STATUT", STATUT);
          query.exec();
}
QSqlQueryModel* promotion::tri()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // Modify the query to cast POURCENTAGE to a numeric type for correct sorting
    model->setQuery("SELECT * FROM PROMOTION ORDER BY CAST(POURCENTAGE AS FLOAT) ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PROMO"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM_PROMO"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("POURCENTAGE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_DEBUT"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DATE_FIN"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CODE_PROMO"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("CATEGORIE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("STATUT"));

    return model;
}

bool promotion::idExists(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PROMOTION WHERE ID_PROMO = :ID_PROMO");
    query.bindValue(":ID_PROMO", id);
    query.exec();

    // Check if the query was successful and if a result is found
    if (query.next()) {
        int count = query.value(0).toInt(); // Get the count result
        return count > 0;  // If count > 0, the ID exists
    }

    return false;  // If no result, the ID does not exist
}
#include "promotion.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QSqlQuery>
#include <QMap>
#include <QString>

using namespace QtCharts;

QChartView* promotion::createPieChartByStatut() {
    // Initialize a map to store the counts of each status
    QMap<QString, int> statusCounts;

    // Query to retrieve STATUT and count occurrences
    QSqlQuery query("SELECT STATUT, COUNT(*) as TotalCount FROM PROMOTION GROUP BY STATUT");
    while (query.next()) {
        QString statut = query.value(0).toString(); // Retrieve the STATUT
        int count = query.value(1).toInt();        // Retrieve the count
        statusCounts[statut] = count;              // Store in the map
    }

    // Create a pie series for the chart
    QPieSeries* series = new QPieSeries();

    // Populate the pie series with slices
    for (auto it = statusCounts.begin(); it != statusCounts.end(); ++it) {
        QPieSlice* slice = series->append(it.key(), it.value()); // Add status and count
        slice->setLabel(QString("%1: %2").arg(it.key()).arg(it.value())); // Display STATUT and count
    }

    // Create a chart and add the series
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Promotions par Statut");
    series->setLabelsVisible();

    // Optional: Customize chart animations
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
void promotion::exportPromotionToPDF() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Promotion Data", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        qDebug() << "No file selected. Aborting PDF generation.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM PROMOTION");
    if (!query.exec()) {
        qDebug() << "Error retrieving promotion data:" << query.lastError().text();
        return;
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        qDebug() << "Failed to begin painting on printer.";
        return;
    }

    // Set up the font and formatting for the document
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(12);
    painter.setFont(font);

    // Start creating the content of the PDF
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; margin: 20px; font-size: 12pt; }"
                   "</style></head><body>";

    // Iterate through the promotions and create paragraphs for each one
    while (query.next()) {
        QString idPromo = query.value("ID_PROMO").toString();
        QString nomPromo = query.value("NOM_PROMO").toString();
        QString pourcentage = query.value("POURCENTAGE").toString();
        QString dateDebut = query.value("DATE_DEBUT").toString();
        QString dateFin = query.value("DATE_FIN").toString();
        QString codePromo = query.value("CODE_PROMO").toString();
        QString categorie = query.value("CATEGORIE").toString();
        QString statut = query.value("STATUT").toString();

        html += QString(
            "<p><strong>ID Promotion:</strong> %1<br>"
            "<strong>Nom:</strong> %2<br>"
            "<strong>Pourcentage:</strong> %3<br>"
            "<strong>Date Début:</strong> %4<br>"
            "<strong>Date Fin:</strong> %5<br>"
            "<strong>Code Promo:</strong> %6<br>"
            "<strong>Catégorie:</strong> %7<br>"
            "<strong>Statut:</strong> %8</p>"
        ).arg(idPromo)
         .arg(nomPromo)
         .arg(pourcentage)
         .arg(dateDebut)
         .arg(dateFin)
         .arg(codePromo)
         .arg(categorie)
         .arg(statut);
    }

    html += "</body></html>";

    // Set the HTML content to the document
    QTextDocument document;
    document.setHtml(html);

    // Draw the content on the PDF
    document.drawContents(&painter);
    painter.end();

    qDebug() << "PDF Promotion List generated successfully!";
}

QSqlQueryModel* promotion::rechercher(const QString& value) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Prepare the query to search by ID_PROMO
    query.prepare("SELECT ID_PROMO, NOM_PROMO, POURCENTAGE, DATE_DEBUT, DATE_FIN, CODE_PROMO, CATEGORIE, STATUT "
                  "FROM PROMOTION WHERE ID_PROMO LIKE :value");
    query.bindValue(":value", "%" + value + "%");

    // Execute the query and set the model
    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Promotion"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Promotion"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Pourcentage"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Début"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Fin"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Code Promo"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Catégorie"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return model;
}
