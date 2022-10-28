#include "fournisseur.h"
#include <QtDebug>
#include <QObject>
#include <QMainWindow>
#include <QDateTime>
#include <QDate>
#include <QFile>
#include <QMessageBox>

fournisseur::fournisseur()
{
    ID=0;
    NOM=" ";
    PRENOM=" ";
    EMAIL=" ";
    TYPE_MATERIEL=" ";
}
fournisseur::fournisseur(int ID, QString NOM, QString PRENOM ,QString EMAIL ,QString TYPE_MATERIEL)
{
    this->ID=ID;
    this->NOM=NOM;
    this->PRENOM=PRENOM;
    this->EMAIL=EMAIL;
    this->TYPE_MATERIEL=TYPE_MATERIEL;
}
/**************************************/
int fournisseur::getid()
{
    return ID;
}
QString fournisseur::getnom()
{
    return NOM;
}
QString fournisseur::getprenom()
{
    return PRENOM;
}
QString fournisseur::getemail()
{
    return EMAIL;
}
QString fournisseur::gettypem()
{
    return TYPE_MATERIEL;
}
/**************************************/
void fournisseur::setid(int ID)
{
    this->ID=ID;
}
void fournisseur::setnom(QString NOM)
{
    this->NOM=NOM;
}
void fournisseur::setprenom(QString PRENOM)
{
    this->PRENOM=PRENOM;
}
void fournisseur::setemail(QString EMAIL)
{
    this->EMAIL=EMAIL;
}
void fournisseur::settypem(QString TYPE_MATERIEL)
{
    this->TYPE_MATERIEL=TYPE_MATERIEL;
}
/**************************************/
bool fournisseur::AjouterF()
{
    QSqlQuery query;
    QString id_string=QString::number(ID);
         query.prepare("INSERT INTO FOURNISSUER(ID,NOM,PRENOM,EMAIL,CIN_DIRIGEUR)" "VALUES (:ID,:NOM,:PRENOM,:EMAIL,:TYPE_MATERIEL)");
         query.bindValue(":ID", ID);
         query.bindValue(":NOM", NOM);
         query.bindValue(":PRENOM", PRENOM);
         query.bindValue(":EMAIL", EMAIL);
         query.bindValue(":TYPE_MATERIEL", TYPE_MATERIEL);
    return query.exec();
}
bool fournisseur::SupprimerF(int ID)
{
    QSqlQuery query;
         query.prepare("DELETE FROM FOURNISSUER where ID= :ID");
         query.bindValue(0, ID);
    return query.exec();
}
QSqlQueryModel* fournisseur::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();
          model->setQuery("SELECT* FROM FOURNISSEUR");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
          model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE_MATERIEL"));
    return  model;
}
bool fournisseur::ModifierF()
{
    QSqlQuery query;
        query.prepare("UPDATE FOURNISSEUR SET NOM=:NOM, PRENOM=:PRENOM, EMAIL=:EMAIL, TYPE_MATERIEL=:TYPE_MATERIEL WHERE ID=:ID ");
        query.bindValue(":NOM", NOM);
        query.bindValue(":PRENOM", PRENOM);
        query.bindValue(":EMAIL", EMAIL);
        query.bindValue(":TYPE_MATERIEL", TYPE_MATERIEL);
        query.bindValue(":ID", ID);
    return query.exec();
}
QSqlQueryModel* fournisseur::afficher_id()
{
    QSqlQueryModel* model=new QSqlQueryModel();
        model->setQuery("SELECT ID from FOURNISSEUR");
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    return model;
}
QSqlQueryModel * fournisseur::tri_nom()
{
    QSqlQueryModel * model= new QSqlQueryModel();
        model->setQuery("SELECT * from FOURNISSEUR order by ID");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE_MATERIEL"));
    return model;
}
QSqlQueryModel * fournisseur::tri_typem()
{
    QSqlQueryModel * model= new QSqlQueryModel();
        model->setQuery("SELECT * from FOURNISSEUR order by TYPE_MATERIEL");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE_MATERIEL"));
    return model;
}
QString fournisseur::read()
{
    QFile file("C:/Users/alamo/Desktop/ProjetCPP/historyfournisseur.txt");
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"info",file.errorString());
    QTextStream in(&file);
    return  in.readAll();
}

void fournisseur::write(QString time, QString txt)
{
    QFile file("C:/Users/alamo/Desktop/ProjetCPP/historyfournisseur.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<time<<" "<<txt<<endl;
        file.close();
    }
}

QString fournisseur::time()
{
    QDateTime time=time.currentDateTime();
    return  time.toString();
}

void fournisseur::clearh()
{
    QFile file("C:/Users/alamo/Desktop/ProjetCPP/historyfournisseur.txt");
    file.open(QFile::WriteOnly|QFile::Truncate);
}