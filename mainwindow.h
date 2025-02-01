#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "produit.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QTextStream>
#include <QFile>
#include <QPrinter>
#include <QTextDocument>
#include <QChartView>
#include <QChart>
#include <QPieSeries>
#include <QPieSlice>
#include <QDebug>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <iostream>
#include <fstream>
#include "arduino.h"
#include "promotion.h"

#include <QDateTime>
using namespace  QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_produit_clicked();


    void on_modifier_produit_clicked();

    void on_delete_produit_clicked();


    void on_insert_clicked();

    void on_afficher_produit_clicked();

    void on_pdf_clicked();


    void on_tableView_produit_clicked(const QModelIndex &index);

    void on_tri_produit_clicked();

    void on_chercher_produit_textChanged(const QString &text);

    void on_stats_clicked();
    void stats_produit();
    void addToHistory(const QString &action, const QString &produitName);
    void saveHistoryToFile();
    void remplir_combo_id_employe();
    void alerte();

    void on_historique_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_histo_clicked();

    void on_pushButton_11_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_stats_2_clicked();

    void on_lineEdit_25_textChanged(const QString &arg1);

    void searchPromotion(const QString &id);
    void processMessage(const QString &message);
    void readSerialData();
    void updateLabelFromArduino(const QString &data);

private:
    Ui::MainWindow *ui;
    PRODUIT p;
   Arduino a;
};
#endif // MAINWINDOW_H
