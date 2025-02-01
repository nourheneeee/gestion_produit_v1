#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set a regular expression validator to allow only numbers
        QRegularExpression regex("^[0-9]+$"); // Regex for numbers only
        QRegularExpressionValidator *validator1 = new QRegularExpressionValidator(regex, this);
        ui->produit_id_p->setValidator(validator1);
        ui->produit_quantite->setValidator(validator1);
        ui->produit_prix->setValidator(validator1);



    // Set a regular expression validator to allow only letters
        QRegularExpression regex1("^[A-Za-z]+$"); // Regex for letters only
        QRegularExpressionValidator *validator2 = new QRegularExpressionValidator(regex1, this);
        ui->produit_nom->setValidator(validator2);
    //set current date
        ui->produit_date->setDate(QDate::currentDate());

        stats_produit();
        ui->tableView_produit->setModel(p.afficher());

        //remplir combo id employe
       remplir_combo_id_employe();
        alerte();



//promotion
int ret = a.connect_arduino(); // Connect to Arduino
switch (ret) {
case 0:
    qDebug() << "Arduino is available and connected to:" << a.getarduino_port_name();
    break;
case 1:
    qDebug() << "Arduino is available but not connected to:" << a.getarduino_port_name();
    break;
case -1:
    qDebug() << "Arduino is not available";
    break;
}


QObject::connect(a.getserial(), SIGNAL(readyRead()), this, SLOT(readSerialData()));
promotion p1;
ui->tableView->setModel(p1.afficher());


//
ui->gestion_promotion->hide();
ui->gestion_produit->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ajouter_produit_clicked()
{

    // Récupération des valeurs depuis l'interface utilisateur
       int id = ui->produit_id_p->text().toInt();  // Assurez-vous d'avoir un champ pour l'ID
       QString nom = ui->produit_nom->text();
       int PRIX = ui->produit_prix->text().toInt();
       int QUANTITE = ui->produit_quantite->text().toInt();
          QDate date = ui->produit_date->date();
          QString disponibilite=ui->produit_disponibilite->currentText();
       int idemploye = ui->produit_idemploye->currentText().toInt();
    QString TYPE=ui->produit_disponibilite->currentText();

    if(p.produitExists(id))
    {

    QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                          QObject::tr("id déja existe"), QMessageBox::Cancel);
    return;
    }
    if(ui->produit_id_p->text().isEmpty() || nom.isEmpty() || ui->produit_prix->text().isEmpty()  )
    {

    QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                          QObject::tr("tu dois remplir tous les champs"), QMessageBox::Cancel);
   return;
    }
    PRODUIT p1(id, nom, PRIX,QUANTITE ,p.getimage(), date,disponibilite, idemploye);

       // Tente d'ajouter le produit  à la base de données
       bool insertionSuccess = p1.ajouter();
       QMessageBox messageBox;

       if (insertionSuccess)
       {
           QMessageBox::information(nullptr, QObject::tr(""),
                                 QObject::tr("Données ajoutées à la base de données avec succès !"), QMessageBox::Cancel);
           // Réinitialisez les champs de saisie
           ui->produit_id_p->clear();
           ui->produit_nom->clear();
           ui->produit_prix->clear();
           ui->produit_quantite->clear();
            ui->tableView_produit->setModel(p.afficher());
            ui->label_image->clear();

            stats_produit();
            addToHistory("Ajout du produit", QString::number(id));
            alerte();

       }
       else
       {
           QMessageBox::critical(nullptr, QObject::tr(""),
                                 QObject::tr("Échec de l'ajout des données dans la base de données."), QMessageBox::Cancel);
       }


}

void MainWindow::on_modifier_produit_clicked()
{

    // Récupération des valeurs depuis l'interface utilisateur
       int id = ui->produit_id_p->text().toInt();  // Assurez-vous d'avoir un champ pour l'ID
       QString nom = ui->produit_nom->text();
       int PRIX = ui->produit_prix->text().toInt();
       int QUANTITE = ui->produit_quantite->text().toInt();
       QString disponibilite=ui->produit_disponibilite->currentText();
          QDate date = ui->produit_date->date();
       int idemploye = ui->produit_idemploye->currentText().toInt();
    QString TYPE=ui->produit_disponibilite->currentText();

    if(!p.produitExists(id))
    {

    QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                          QObject::tr("id n' existe pas"), QMessageBox::Cancel);
    return;
    }
    if(ui->produit_id_p->text().isEmpty() || nom.isEmpty() || ui->produit_prix->text().isEmpty()  )
    {

    QMessageBox::critical(nullptr, QObject::tr("ERREUR"),
                          QObject::tr("tu dois remplir tous les champs"), QMessageBox::Cancel);
   return;
    }

       // Tente d'ajouter le produit  à la base de données
       bool insertionSuccess = p.modifier(id, nom, PRIX,QUANTITE,p.getimage(), date,disponibilite, idemploye);
       QMessageBox messageBox;

       if (insertionSuccess)
       {
           QMessageBox::information(nullptr, QObject::tr(""),
                                 QObject::tr("Données modifié à la base de données avec succès !"), QMessageBox::Cancel);
           // Réinitialisez les champs de saisie
           ui->produit_id_p->clear();
           ui->produit_nom->clear();
           ui->produit_prix->clear();
           ui->produit_quantite->clear();
            ui->tableView_produit->setModel(p.afficher());

            ui->label_image->clear();
            stats_produit();
            addToHistory("Mise à jour du produit", QString::number(id));
            alerte();

       }
       else
       {
           QMessageBox::critical(nullptr, QObject::tr(""),
                                 QObject::tr("Échec de la modification des données dans la base de données."), QMessageBox::Cancel);
       }

}

void MainWindow::on_delete_produit_clicked()
{
    int id=ui->produit_id_p->text().toInt();
    if (p.produitExists(id)==false) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("id n'existe pas"), QMessageBox::Cancel);
       }
    else
    {
    if(p.produitExists(id)){
        bool test =p.supprimer(id);
        QMessageBox msgBox;
        if(test)
        {
            QMessageBox::information(this, "", "suppression avec succés");
     ui->produit_id_p->clear();
     ui->tableView_produit->setModel(p.afficher());
     stats_produit();
     addToHistory("Suppression du produit",ui->produit_id_p->text());
     alerte();

        } else
        {
          QMessageBox::warning(this,"", "Erreur", "Échec de suppression du produit.");
          ui->produit_id_p->clear();
        }}
   }

    }


void MainWindow::on_insert_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.bmp *.gif)");

    if (!imagePath.isEmpty()) {
        QPixmap image(imagePath);

        if (!image.isNull()) {
            ui->label_image->setPixmap(image.scaled(ui->label_image->size(), Qt::KeepAspectRatio));

            // Convert QPixmap to QByteArray
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");

            p.setimage(imageData);
        } else {
            QMessageBox::warning(this, "Error", "Failed to load image.");
        }
    }
}

void MainWindow::on_afficher_produit_clicked()
{
    ui->tableView_produit->setModel(p.afficher());
}

void MainWindow::on_pdf_clicked()
{
    {
        QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->tableView_produit->model()->rowCount();
        const int columnCount = ui->tableView_produit->model()->columnCount();

        out << "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
            "<title>%1</title>\n"
            "<style>\n"
            "body {\n"
            "    background-color: #ffe6f2;\n"  // Light pink background
            "    color: #330033;\n"              // Dark purple text
            "}\n"
            "table {\n"
            "    width: 100%;\n"
            "    border-collapse: collapse;\n"
            "}\n"
            "th, td {\n"
            "    padding: 10px;\n"
            "    text-align: left;\n"
            "    border-bottom: 2px solid #ff99cc;\n" // Light pink border
            "}\n"
            "th {\n"
            "    background-color: #ffccff;\n" // Pink header background
            "    color: #990066;\n"             // Darker text for headers
            "}\n"
            "tr:nth-child(even) {\n"
            "    background-color: #ffe6f2;\n"  // Light pink for even rows
            "}\n"
            "tr:hover {\n"
            "    background-color: #ffb3d9;\n"  // Highlight color on hover
            "}\n"
            "</style>\n"
            "</head>\n"
            "<body bgcolor=#ffffff link=#5000A0>\n"
            "<center><h1>Liste des produits</h1></center><br/><br/>\n"
            "<table>\n";

        // headers
        out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView_produit->isColumnHidden(column))
            {
                out << QString("<th>%1</th>").arg(ui->tableView_produit->model()->headerData(column, Qt::Horizontal).toString());
            }
        }
        out << "</tr></thead>\n";

        // data table
        for (int row = 0; row < rowCount; row++)
        {
            out << "<tr> <td>" << row + 1 << "</td>";
            for (int column = 0; column < columnCount; column++)
            {
                if (!ui->tableView_produit->isColumnHidden(column))
                {
                    QString data = ui->tableView_produit->model()->data(ui->tableView_produit->model()->index(row, column)).toString().simplified();
                    out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
            }
            out << "</tr>\n";
        }



        QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), ".pdf");
        if (QFileInfo(fileName).suffix().isEmpty())
        {
            fileName.append(".pdf");
        }

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setOutputFileName(fileName);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.print(&printer);
    }
}


void MainWindow::on_tableView_produit_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->tableView_produit->model();

        int row = index.row();
        QString id = model->data(model->index(row, 0)).toString();
        QString NOM = model->data(model->index(row, 1)).toString();
        QString prix = model->data(model->index(row, 2)).toString();

        QString quantite = model->data(model->index(row, 3)).toString();
        QByteArray image_byte = model->data(model->index(row, 4)).toByteArray();
        QDate date = model->data(model->index(row, 5)).toDate();
        QString disponibilite = model->data(model->index(row, 6)).toString();
        QString id_employe = model->data(model->index(row,7)).toString();

ui->produit_id_p->setText(id);
ui->produit_nom->setText(NOM);
ui->produit_prix->setText(prix);
ui->produit_quantite->setText(quantite);
if (!image_byte.isEmpty()) {
     QPixmap image;
     image.loadFromData(image_byte);
     ui->label_image->setPixmap(image.scaled(ui->label_image->size(), Qt::KeepAspectRatio));
 } else {
     ui->label_image->clear();
 }
ui->produit_date->setDate(date);
ui->produit_disponibilite->setCurrentText(disponibilite);
ui->produit_idemploye->setCurrentText(id_employe);

ui->tab_produit->setCurrentIndex(0);
}

void MainWindow::on_tri_produit_clicked()
{
    QString choix = ui->combo_choix->currentText();
    QString ordre = ui->combo_choix_ordre->currentText();

    ui->tableView_produit->setModel(p.tri(choix, ordre));
}

void MainWindow::on_chercher_produit_textChanged(const QString &text)
{
    QString choix=ui->combo_choix->currentText();
    ui->tableView_produit->setModel(p.chercher(choix, text));

}
void MainWindow::stats_produit()
{

        QList<QWidget*> childWidgets = ui->label_stat->findChildren<QWidget*>();
            for (QWidget* childWidget : childWidgets) {
                childWidget->deleteLater();

            }
            //the clear didnt work, but my goal is when i second click this button it deleted old chart and renders a new one
            ui->label_stat->clear();
            ui->label_stat->hide();

        int s0, s1;

        s0 = p.countDisponibilite("Disponible");
        s1 = p.countDisponibilite("Non Disponible");




        int total = s0 + s1   ;
        // Calculate percentages
            float x = (total != 0) ? (s0 * 100.0f) / total : 0.0f; //if(total!=0)  x=x0+100.0f /total else x=0.0f
            float x1 = (total != 0) ? (s1 * 100.0f) / total : 0.0f;


            // Adjust the percentages to ensure they sum up to 100%
            float totalPercentage = x + x1;
            if (totalPercentage != 100.0f && total != 0) {
                float correction = 100.0f - totalPercentage;
                x += correction;  // Apply correction to one of the slices (usually the largest one)
            }



    //qDebug() <<  x <<endl;
    // 3 jour  33.00 %
        QString ch1 = QString("Disponible %1%").arg(QString::number(x, 'f', 2));
        QString ch2 = QString("Non Disponible %2%").arg(QString::number(x1, 'f', 2));





        QPieSeries *series=new QPieSeries();
        series->setHoleSize(0.35);

        QPieSlice *slice= series->append(ch1,x);
        slice->setLabelVisible();
        slice->setLabelColor(QColor(Qt::black));
            slice->setBrush(QColor(255, 176, 192));//changer couleur

            QPieSlice *slice1= series->append(ch2,x1);
        slice1->setLabelVisible();
        slice1->setLabelColor(QColor(Qt::black));
        slice1->setBrush(QColor(255, 150, 170));



        QChart *chart=new QChart();
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QBrush backgroundBrush(QColor(187,93,87,0));
        chart->setBackgroundBrush(backgroundBrush);
        QChartView *chartview=new QChartView(chart);
        chartview->setRenderHint(QPainter::Antialiasing);
        chartview->setFixedSize(ui->label_stat->size());
        chartview->setParent(ui->label_stat);
        ui->label_stat->setStyleSheet("background:transparent; color:white; ");
        ui->label_stat->show();


}
void MainWindow::on_stats_clicked()

{
    stats_produit();
}


void MainWindow::addToHistory(const QString &action, const QString &produitName)
{
    // Chemin du fichier historique
    QString filePath = "C:/Users/msi/Desktop/gestion_pr_produit_promotion/gestion_produit_v1/historique.txt";
    QFile file(filePath);

    // Ouvrir le fichier en mode ajout
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire l'action dans le fichier historique avec la date et l'heure
    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << action;
    if (!produitName.isEmpty()) {
        out << " pour le produit : " << produitName;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}

void MainWindow::saveHistoryToFile()
{
    // Chemin du fichier historique
    QString filePath = "C:/Users/msi/Desktop/gestion_pr_produit_promotion/gestion_produit_v1/historique.txt";
    QFile file(filePath);

    // Ouvrir le fichier en mode lecture
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique pour lecture.";
        return;
    }

    // Lire le contenu du fichier historique
    QTextStream in(&file);
    QString content = in.readAll();

    // Afficher le contenu de l'historique dans la console ou l'UI
    qDebug() << content;

    // Fermer le fichier
    file.close();
}

void MainWindow::remplir_combo_id_employe()
{

    //répétition
   ui->produit_idemploye->clear();
   QSqlQuery query;
   query.prepare("SELECT CIN_E FROM EMPLOYEE");
   if(query.exec())
       {while(query.next())
       {QString CIN_E=query.value(0).toString();
           ui->produit_idemploye->addItem(CIN_E);
       }
       }
}

void MainWindow::alerte()
{

    QSqlQuery query;

    // Prepare the SQL query
    query.prepare("SELECT ID_P, NOM_P  FROM PRODUIT WHERE DISPONIBILITE ='Disponible' AND QUANTITE=0");

    // Execute the query and check for results
    if (query.exec()) {
        QStringList alertMessages; // List to accumulate alert messages
        QString currentDateString = QDate::currentDate().toString("yyyy-MM-dd"); // Format: "2024-11-20"

        while (query.next()) { // Loop through all results
            QString id = query.value(0).toString();
            QString NOM_P = query.value(1).toString();


            // Check if the extracted date equals the current date
                QString message = QString("repture sur le ID: %1 NOM_P: %2 ").arg(id, NOM_P);
                alertMessages.append(message);
            }


        // Check if we found any alerts
        if (!alertMessages.isEmpty()) {
            QString fullMessage = alertMessages.join("\n\n"); // Join all messages with double newlines
            QMessageBox::critical(nullptr, QObject::tr("Alert"), fullMessage, QMessageBox::Cancel);
        } else {
            QMessageBox::information(nullptr, QObject::tr("Info"), "No items found for the specified criteria.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Error"), "Failed to execute query.", QMessageBox::Cancel);
    }
    }






//promotion
void MainWindow::on_historique_clicked()
{

        QString filePath = "C:/Users/msi/Desktop/gestion_pr_produit_promotion/gestion_produit_v1/historique.txt";
        QFile file(filePath);

        // Vérifier si le fichier peut être ouvert en mode lecture
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier d'historique.");
            return;
        }

        // Lire tout le contenu du fichier historique
        QTextStream in(&file);
        QString historyContent = in.readAll();

        // Afficher le contenu dans une boîte de dialogue ou une zone de texte
        QMessageBox::information(this, "Historique des actions", historyContent);

        // Fermer le fichier
        file.close();

}

void MainWindow::on_pushButton_5_clicked()
{
    QString Id=ui->lineEdit_15->text();
    QString Nom=ui->lineEdit_16->text();
    QString Pourcentage=ui->lineEdit_17->text();
    QString dateD=ui->dateEdit->date().toString();
    QString dateF=ui->dateEdit_2->date().toString();
    QString Code=ui->lineEdit_19->text();
    QString Category=ui->lineEdit_21->text();
    QString statut=ui->lineEdit_24->text();
    promotion p1(Id,Nom,Pourcentage,dateD,dateF,Code,Category,statut);

    if(Id=="" || Nom=="" || Pourcentage=="" || dateD=="" || dateF=="" || Code=="" || Category=="" || statut=="")
    {
         QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr(" veuillez remplir tous les champs."),QMessageBox::Ok);
    }
    else{
    p1.ajouter();
    ui->tableView->setModel(p1.afficher());
    QFile file("C:/Users/zied/Desktop/fichier.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier.";
        return;
    }
    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

    // Écrire la date et l'heure actuelles suivies du texte dans le fichier
    QTextStream out(&file);
    out << "<font color=\"green\">Le promotion a ete ajoute a " << formattedDateTime << "</font>\n";
    // endl ajoute un saut de ligne
    file.close();

        QMessageBox::information(0,qApp->tr("reussie"),qApp->tr(" la promotion a ete bien ajouteee."),QMessageBox::Ok);
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    QString Id=ui->lineEdit_15->text();
    QString Nom=ui->lineEdit_16->text();
    QString Pourcentage=ui->lineEdit_17->text();
    QString dateD=ui->dateEdit->date().toString();
    QString dateF=ui->dateEdit_2->date().toString();
    QString Code=ui->lineEdit_19->text();
    QString Category=ui->lineEdit_21->text();
    QString statut=ui->lineEdit_24->text();

    promotion p1(Id,Nom,Pourcentage,dateD,dateF,Code,Category,statut);
    if(Id=="" || Nom=="" || Pourcentage=="" || dateD=="" || dateF=="" || Code=="" || Category=="" || statut=="")
    {
         QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr(" veuillez remplir tous les champs."),QMessageBox::Ok);
    }
    else{
    p1.modifier();
        QFile file("C:/Users/zied/Desktop/fichier.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Impossible d'ouvrir le fichier.";
            return;
        }
        // Obtenir la date et l'heure actuelles
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

        // Écrire la date et l'heure actuelles suivies du texte dans le fichier
        QTextStream out(&file);
        out << "<font color=\"blue\">La promotion a ete modifiee a " << formattedDateTime << "</font>\n";
        // endl ajoute un saut de ligne
        file.close();
    ui->tableView->setModel(p1.afficher());
    QMessageBox::information(0,qApp->tr("reussie"),qApp->tr(" la promotion a ete bien ajouteee."),QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    QString Id = ui->lineEdit_15->text();
    promotion p1;

    // Assuming supprimer() returns a bool indicating success or failure
    bool isDeleted = p1.supprimer(Id);

    if (isDeleted) {
        // Show a message box to inform the user
        QMessageBox::information(this, "Success", "The promotion has been successfully deleted.");

        // Open the file and write the log entry
        QFile file("C:/Users/zied/Desktop/fichier.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Unable to open the file.";
            return;
        }

        // Get the current date and time
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

        // Write the log entry
        QTextStream out(&file);
        out << " <font color=\"red\">La promotion a ete supprimee a " << formattedDateTime << "\n";
       // endl adds a newline
        file.close();

        // Refresh the table view
        ui->tableView->setModel(p1.afficher());
    } else {
        // Show an error message box if deletion fails
        QMessageBox::warning(this, "Error", "Failed to delete the promotion. Please check the ID and try again.");
    }
}

void MainWindow::on_histo_clicked()
{

    QDialog *dialog = new QDialog(this);
    dialog->resize(600, 600);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QTextEdit *textEdit = new QTextEdit(dialog);

    // Ouvrir le fichier en lecture seule
    QFile file("C:/Users/zied/Desktop/fichier.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Lire le contenu du fichier ligne par ligne et l'ajouter à QTextEdit
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = "evenement : "+in.readLine();
            textEdit->append(line);
        }
        // Fermer le fichier
        file.close();
    } else {
        // Afficher un message d'erreur si le fichier n'a pas pu être ouvert
        qDebug() << "Erreur lors de l'ouverture du fichier.";
    }
    layout->addWidget(textEdit);
    dialog->exec();
}

void MainWindow::on_pushButton_11_clicked()
{
    promotion p ;
    p.exportPromotionToPDF();

}

void MainWindow::on_pushButton_4_clicked()
{
    ui->gestion_produit->hide();
    ui->gestion_promotion->show();
}

void MainWindow::on_pushButton_clicked()
{
    ui->gestion_promotion->hide();
    ui->gestion_produit->show();
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->gestion_promotion->hide();
    ui->gestion_produit->show();
}

void MainWindow::on_pushButton_13_clicked()
{
    ui->gestion_produit->hide();
    ui->gestion_promotion->show();
}

void MainWindow::on_pushButton_9_clicked()
{promotion p;

    ui->tableView->setModel(p.afficher());

}

void MainWindow::on_pushButton_7_clicked()
{
    promotion p;
    ui->tableView->setModel(p.tri());

}

void MainWindow::on_stats_2_clicked()
{
    {
        QLayoutItem* item;
        while ((item = ui->verticalLayout->layout()->takeAt(0)) != nullptr) {
              delete item->widget();
             delete item;
         }





       QChartView *chartView = nullptr;

promotion p;
         // Create the pie chart based on 'etat'
           chartView = p.createPieChartByStatut();



       // Add the chart view to the layout if it was successfully created
       if (chartView != nullptr) {
           ui->verticalLayout->layout()->addWidget(chartView);
       }

    }
}

void MainWindow::on_lineEdit_25_textChanged(const QString &arg1)
{

promotion p;
    if (arg1.isEmpty() && ui->lineEdit_25->hasFocus() && !ui->lineEdit_25->hasSelectedText()) {
        ui->tableView->setModel(p.afficher());
        ui->lineEdit_25->setToolTip("Entrez un critère de recherche");
        return; // Exit early if there's no input
    }
    // Call the rechercher function with the selected option and input value
    ui->tableView->setModel(p.rechercher(arg1));
    ui->tableView->clearSelection();
}


void MainWindow::updateLabelFromArduino(const QString &data)
{
    // Assuming you have a QLabel in your UI to display Arduino data
    ui->label->setText(data);  // Replace 'arduinoDataLabel' with the actual label name in your UI
}
void MainWindow::readSerialData()
{
    // Check if data is available from Arduino
    if (a.getserial()->bytesAvailable() > 0) {
        QByteArray data = a.read_from_arduino();  // Read available data
        QString strData = QString(data);         // Convert QByteArray to QString

        qDebug() << "Received data from Arduino: " << strData;  // Debugging

        ui->label->setText("code promo : " + strData);
        a.code=a.code+strData;
        // Check if the data contains '#'
        if (strData.contains("#")) {
            // Remove the '#' character
            strData.remove("#");
            a.code.remove("#");
            qDebug() << "Processed data: " << strData;  // Debugging

            // Process the ID (e.g., search promotion)
            processMessage(a.code);



            QByteArray byteArray;
            byteArray.append("E");
            a.write_to_arduino(byteArray);
            a.code.clear();
        }
    } else {
        qDebug() << "No data available in the serial buffer.";
        QByteArray byteArray;
        byteArray.append("N");
        a.write_to_arduino(byteArray);
        a.code.clear();

    }
}




void MainWindow::processMessage(const QString &message)
{
    // Process the received message (e.g., search for promotion by ID)
    qDebug() << "Processing message: " << message;

    // Example: If the message is an ID, search for the promotion
    QString id = message.trimmed();
    searchPromotion(id);
}


void MainWindow::searchPromotion(const QString &id)
{
    // Perform the database search with the received ID (example using QSqlQuery)
    QSqlQuery query;
    query.prepare("SELECT * FROM promotion WHERE id_promo = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        // Assuming the promotion details are in the query result
        QString promoName = query.value("STATUT").toString();
        //QString promoDiscount = query.value("discount").toString();
        // Update the UI with the promo details
        ui->lab->setText("Promotion: " + promoName);
      //  ui->promoDiscountLabel->setText("Discount: " + promoDiscount);

                           QByteArray byteArray;
                           byteArray.append("E"); // Create a QByteArray for the character
                           a.write_to_arduino(byteArray); // Send the character to Arduino

    } else {
        QMessageBox::warning(this, "Error", "Promotion ID not found.");
        QByteArray byteArray;
        byteArray.append("N"); // Create a QByteArray for the character
        a.write_to_arduino(byteArray); // Send the character to Arduino
    }
}
