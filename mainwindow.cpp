#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "dialog_statistiques.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPushButton>
#include <QPdfWriter>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_ID->setValidator( new QIntValidator(0, 99999999, this));
    ui->lineEdit_NumTel->setValidator( new QIntValidator(0, 99999999, this));
    ui->comboBox_ID->setModel(S.afficher_id());
    S.write(S.time(),"App started");
    ui->tab_Spornors->setModel (S.afficher());
    connection c;
    bool test=c.CreateConnexion();
    if(test)
    {
        ui->label_db->setText("Database: CONNECTED");

    }
    else
        ui->label_db->setText("Database: FAILED");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pB_Ajouter_clicked()
{
    int ID=ui->lineEdit_ID->text().toInt();
    int NUM_TEL=ui->lineEdit_NumTel->text().toInt();
    QString DESCRIPTION=ui->lineEdit_Description->text();
    sponsoring S(ID,NUM_TEL,DESCRIPTION);
    bool test=S.ajouterS();
    if(test)
    {
        ui->label_Info->setText("Ajout effectué");
        ui->comboBox_ID->setModel(S.afficher_id());
        ui->tab_Spornors->setModel(S.afficher());
        S.write(S.time(),"SPONSORING: Ajout effectué");
        ui->textBrowser_Sponso->setText(S.read());
    }
    else
    {
        ui->label_Info->setText("Ajout non effectué");
    }
}

void MainWindow::on_pB_Modifier_clicked()
{
    int ID=ui->lineEdit_ID->text().toInt();
    int NUM_TEL=ui->lineEdit_NumTel->text().toInt();
    QString DESCRIPTION=ui->lineEdit_Description->text();
    sponsoring S(ID,NUM_TEL,DESCRIPTION);
    bool test=S.modifierS();
    if(test)
    {
        ui->label_Info->setText("Modification effectué");
        ui->tab_Spornors->setModel(S.afficher());
        ui->comboBox_ID->setModel(S.afficher_id());
        S.write(S.time(),"SPONSORING: Modification effectué");
        ui->textBrowser_Sponso->setText(S.read());
    }
    else
    {
        ui->label_Info->setText("Modification non effectué");
    }
}

void MainWindow::on_pB_Supprimer_clicked()
{
    sponsoring S;
    S.setid(ui->comboBox_ID->currentText().toInt());
    bool test=S.supprimerS(S.getid());
    if(test)
    {
        ui->label_Info->setText("Suppression effectué");
        ui->tab_Spornors->setModel(S.afficher());
        ui->comboBox_ID->setModel(S.afficher_id());
        S.write(S.time(),"SPONSORING: Suppression effectué");
        ui->textBrowser_Sponso->setText(S.read());
    }
    else
    {
        ui->label_Info->setText("Suppression non effectué");
    }
}

void MainWindow::on_pB_TirerParNom_clicked()
{
    ui->label_Info->setText("Tri par Num Tel effectué");
    ui->tab_Spornors->setModel(S.tri_numtel());
}

void MainWindow::on_pB_TirerParID_clicked()
{
    ui->label_Info->setText("Tri par ID effectué");
    ui->tab_Spornors->setModel(S.tri_id());
}

void MainWindow::on_pushButton_SupprimerS_clicked()
{
    S.clearh();
    ui->textBrowser_Sponso->setText(S.read());
}

void MainWindow::on_comboBox_ID_currentIndexChanged(int)
{
    int id=ui->comboBox_ID->currentText().toInt();
    QString id_1=QString::number(id);
    QSqlQuery query;
    query.prepare("SELECT * FROM SPONSORING where ID='"+id_1+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->lineEdit_ID->setText(query.value(0).toString()) ;
            ui->lineEdit_NumTel->setText(query.value(1).toString()) ;
            ui->lineEdit_Description->setText(query.value(2).toString()) ;
        }
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                              QObject::tr("ECHEC DE chargement .\n""Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_Fermer_clicked()
{
    close();
}

void MainWindow::on_pB_ExPDF_clicked()
{
    QPdfWriter pdf("C:/Users/alamo/Desktop/GestionDesSponsors/Liste.pdf");

    QPainter painter(&pdf);

    int i = 4000;
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    painter.drawPixmap(QRect(100,400,2000,2000),QPixmap("C:/Users/alamo/Desktop/GestionDesSponsors/Logo.png"));
    painter.drawText(3000,1500,"LISTE DES SPONSORS");
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(2700,200,6300,2600);
    painter.drawRect(0,3000,9600,500);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    painter.drawText(300,3300,"ID");
    painter.drawText(2300,3300,"NUM_TEL");
    painter.drawText(4300,3300,"DESCRIPTION");
    QSqlQuery query;
    query.prepare("<SELECT CAST( GETDATE() AS Date ) ");
    time_t tt;
    struct tm* ti;
    time(&tt);
    ti=localtime(&tt);
    asctime(ti);
    painter.drawText(500,300, asctime(ti));
    query.prepare("select * from SPONSORING");
    query.exec();
    while (query.next())
    {
        painter.drawText(300,i,query.value(0).toString());
        painter.drawText(2300,i,query.value(1).toString());
        painter.drawText(4300,i,query.value(2).toString());
        painter.drawText(6300,i,query.value(3).toString());
        painter.drawText(8000,i,query.value(4).toString());
        painter.drawText(10000,i,query.value(5).toString());
        i = i +500;
    }
    int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes |  QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile("C:/Users/alamo/Desktop/GestionDesSponsors/Liste.pdf"));
        painter.end();
    }
    if (reponse == QMessageBox::No)
    {
        painter.end();
    }
}

void MainWindow::on_pB_Recherche_clicked()
{
    sponsoring S;
    QString text;

    if (ui->rB_ID->isChecked()==true)
    {
        S.clearTable(ui->tab_Spornors);
            int ID=ui->lineEdit_IDRecherche->text().toInt();
            S.chercheID(ui->tab_Spornors,ID);
    }
    if (ui->rB_NumTel->isChecked()==true)
    {
        S.clearTable(ui->tab_Spornors);
            int Numtel=ui->lineEdit_IDRecherche->text().toInt();
            S.chercheNumTel(ui->tab_Spornors,Numtel);
    }
}
void MainWindow::on_pB_Stats_clicked()
{
    Dialog_Statistiques stats;
    stats.setModal(true);
    stats.exec();
}