#include "dialog_perso.h"
#include "ui_dialog_perso.h"
#include "personnel.h"
#include "login_bd.h"
#include "connexion.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPushButton>
#include "dialog_menu.h"

Dialog_perso::Dialog_perso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_perso)
{
    ui->setupUi(this);
    ui->lineEdit_CIN->setValidator( new QIntValidator(0, 99999999, this));
    ui->comboBox_CIN->setModel(P.afficher_cin());
    ui->tableView_Personnel->setModel (P.afficher());
    P.write(P.time(),"App started");
    ui->textBrowser_Perso->setText(P.read());
}

Dialog_perso::~Dialog_perso()
{
    delete ui;
}

void Dialog_perso::on_pB_AjouterP_clicked()
{
    int CIN=ui->lineEdit_CIN->text().toInt();
    QString NOM=ui->lineEdit_Nom->text();
    QString PRENOM=ui->lineEdit_Prenom->text();
    QString ADRESSE=ui->lineEdit_Adresse->text();
    QString EMAIL=ui->lineEdit_Email->text();
    QDate DATE_DE_NAISSANCE=ui->dateEdit_DateDeNaissance->date();
    int CIN_DIRIGEUR = 0;
    personnel P(CIN,NOM,PRENOM,ADRESSE,EMAIL,DATE_DE_NAISSANCE,CIN_DIRIGEUR);
    bool test=P.ajouter();
    if(test)
    {
        ui->label_Info->setText("Ajout Effectué");
        ui->tableView_Personnel->setModel(P.afficher());
        P.write(P.time(),"PERSONNEL: ajout effectué");
        ui->textBrowser_Perso->setText(P.read());
    }
    else
    {
        ui->label_Info->setText("Ajout non effectué");
    }
}

void Dialog_perso::on_pB_SupprimerP_clicked()
{
    personnel P;
    P.setcin(ui->comboBox_CIN->currentText().toInt());
    bool test=P.supprimer(P.getcin());
    if(test)
    {
        ui->label_Info->setText("Suppression effectué");
        ui->tableView_Personnel->setModel(P.afficher());
        ui->comboBox_CIN->setModel(P.afficher_cin());
        P.write(P.time(),"PERSONNEL: Supression effectuée");
        ui->textBrowser_Perso->setText(P.read());
    }
    else
    {
        ui->label_Info->setText("Suppression non effectué");
    }
}

void Dialog_perso::on_pB_ModifierP_clicked()
{
    int CIN=ui->lineEdit_CIN->text().toInt();
    QString NOM=ui->lineEdit_Nom->text();
    QString PRENOM=ui->lineEdit_Prenom->text();
    QString ADRESSE=ui->lineEdit_Adresse->text();
    QString EMAIL=ui->lineEdit_Email->text();
    QDate DATE_DE_NAISSANCE=ui->dateEdit_DateDeNaissance->date();
    int CIN_DIRIGEUR = 0;
    personnel P(CIN,NOM,PRENOM,ADRESSE,EMAIL,DATE_DE_NAISSANCE,CIN_DIRIGEUR);
    bool test=P.modifierP();
    if(test)
    {
        ui->label_Info->setText("Modification effectué");
        ui->tableView_Personnel->setModel(P.afficher());
        ui->comboBox_CIN->setModel(P.afficher_cin());
        P.write(P.time(),"PERSONNEL: Modification effectuée");
        ui->textBrowser_Perso->setText(P.read());
    }
    else
    {
        ui->label_Info->setText("Modification non effectué");
    }
}

void Dialog_perso::on_pushButton_InsererPhotoPerso_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("choose"),"",tr("Image(*.png *.jpeg *.jpg *.bmp *.gif)"));
    if (QString::compare(filename,QString()) !=0)
    {
        QImage image;
        bool valid = image.load(filename);
        if(valid)
        {
            image=image.scaledToWidth(ui->label_imageperso->width(), Qt::SmoothTransformation);
            ui->label_imageperso->setPixmap(QPixmap::fromImage(image));
        }
        else
        {
            //ERROR HANDLING
        }
    }
}

void Dialog_perso::on_pB_TireParCIN_clicked()
{
    ui->label_Info->setText("Tri par CIN effectué");
    ui->tableView_Personnel->setModel(P.tri_cin());
}

void Dialog_perso::on_pB_TirerParNom_clicked()
{
    ui->label_Info->setText("Tri par Nom effectué");
    ui->tableView_Personnel->setModel(P.tri_nom());
}

void Dialog_perso::on_pB_TirerParEmail_clicked()
{
    ui->label_Info->setText("Tri par Email effectué");
    ui->tableView_Personnel->setModel(P.tri_email());
}

void Dialog_perso::on_pushButton_SupprimerP_clicked()
{
    P.clearh();
    ui->textBrowser_Perso->setText(P.read());
}

void Dialog_perso::on_pushButton_Fermer_clicked()
{
    this->close();
    Dialog_menu menu;
    menu.setModal(true);
    menu.exec();
}

void Dialog_perso::on_comboBox_CIN_currentIndexChanged(int)
{
    int cin=ui->comboBox_CIN->currentText().toInt();
    QString cin_1=QString::number(cin);
    QSqlQuery query;
    query.prepare("SELECT * FROM PERSONNEL where CIN='"+cin_1+"'");
    if(query.exec())
    {
        while (query.next())
        {
            ui->lineEdit_CIN->setText(query.value(0).toString()) ;
            ui->lineEdit_Nom->setText(query.value(1).toString()) ;
            ui->lineEdit_Prenom->setText(query.value(2).toString()) ;
            ui->lineEdit_Adresse->setText(query.value(3).toString()) ;
            ui->lineEdit_Email->setText(query.value(4).toString()) ;
            ui->dateEdit_DateDeNaissance->setDate(query.value(5).toDate()) ;
        }
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK "),
                              QObject::tr("ECHEC DE chargement .\n""Click Cancel to exit."), QMessageBox::Cancel);
    }
}
