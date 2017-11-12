/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdDRegistation.h"
#include "ui_SdDRegistation.h"

#include <QSettings>
#include <QMessageBox>

SdDRegistation::SdDRegistation(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDRegistation)
  {
  ui->setupUi(this);

  //Fill fields
  QSettings s;
  ui->mServerIP->setText( s.value( QStringLiteral(SDK_SERVER_IP), QString("127.0.0.0")).toString() );
  ui->mName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  ui->mMachineKey->setText( s.value( QStringLiteral(SDK_MACHINE_KEY), QString()).toString() );
  ui->mLimit->setText( QString::number(SD_DEFAULT_DELIVERED_LIMIT) );
  ui->mLoaded->setText( QString("0") );

  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mAddMachine, &QPushButton::clicked, this, &SdDRegistation::cmAddMachine );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );
  }



SdDRegistation::~SdDRegistation()
  {
  delete ui;
  }



//Registration new user
void SdDRegistation::cmRegistration()
  {
  if( ui->mMachineKey->text().isEmpty() ) {
    //Registration new user
    }
  else {
    //Check access to existing user
    }
  }




//Add to user new machine
void SdDRegistation::cmAddMachine()
  {
  if( ui->mMachineKey->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Error!"), tr("You must enter existing machine key."));
    }
  else {
    //Check access user
    }
  }




//Close dialog
void SdDRegistation::cmClose()
  {
  if( ui->mMachineKey->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("You not registered. SaliCAD will work in autonom mode. In this mode You can not access global component database. This dialog allowed in later throw Help menu."));
    }
  else {
    QMessageBox::warning( this, tr("Warning!"), tr("This dialog allowed in later throw Help menu."));
    QSettings s;
    s.setValue( QStringLiteral(SDK_SERVER_IP), ui->mServerIP->text() );
    s.setValue( QStringLiteral(SDK_GLOBAL_AUTHOR), ui->mName->text() );
    s.setValue( QStringLiteral(SDK_MACHINE_KEY), ui->mMachineKey->text() );
    }
  done(0);
  }



