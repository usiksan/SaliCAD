/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for bus enter
*/
#include "SdConfig.h"
#include "SdDGetBus.h"
#include "SdStringHistory.h"
#include "ui_SdDGetBus.h"
#include "SdDHelp.h"
#include "objects/SdProject.h"

#include <QStringList>
#include <QMessageBox>
#include <QDebug>



//static QStringList previousBusList;
static SdStringHistory previousBusList;

SdDGetBus::SdDGetBus(SdProject *prj, QWidget *parent) :
  QDialog(parent),
  mFirst(true),
  ui(new Ui::SdDGetBus)
  {
  ui->setupUi(this);

  //Fill previous net list
  ui->mBusList->addItems( previousBusList );

  //On selection bus line
  connect( ui->mBusList, &QListWidget::currentItemChanged, this, [this] () {
    ui->mBusEdit->setText( ui->mBusList->currentItem()->text() );
    mFirst = false;
    } );

  //Setup default bus line - previous bus line
  if( previousBusList.count() ) {
    ui->mBusEdit->setText( previousBusList.first() );
    ui->mBusEdit->selectAll();
    }

  //Accumulate named nets
  if( prj ) {
    QStringList netList = prj->netList();
    for( const QString &net : netList )
      if( !net.startsWith( defNetNamePrefix ) )
        ui->mNamedNetList->addItem( net );

    //On text changed - reset mFirst flag
    connect( ui->mBusEdit, &QLineEdit::textChanged, this, [this] () { mFirst = false; } );

    //On selection changed we append selected net to the bus edit net list
    connect( ui->mNamedNetList, &QListWidget::currentItemChanged, this, [this] ( QListWidgetItem *current, QListWidgetItem *previous ) {
      Q_UNUSED(previous)
      if( current ) {
        //Clear previously entered text
        if( mFirst )
          ui->mBusEdit->clear();
        mFirst = false;
        //If bus edit is empty then we simple add net
        // otherwise we add comma and net
        if( ui->mBusEdit->text().isEmpty() )
          ui->mBusEdit->setText( current->text() );
        else if( ui->mBusEdit->text().endsWith( QChar(',') ) )
          ui->mBusEdit->setText( ui->mBusEdit->text() + current->text() );
        else
          ui->mBusEdit->setText( ui->mBusEdit->text() + QString(",") + current->text() );
        }
      } );
    }

  //When click remove button we remove last net from bus edit net list
  connect( ui->mRemove, &QPushButton::clicked, this, [this] () {
    mFirst = false;
    QString bus = ui->mBusEdit->text();
    //We find last delimiter
    int index = bus.lastIndexOf( QChar(',') );
    if( index >= 0 )
      ui->mBusEdit->setText( bus.left(index) );
    else
      ui->mBusEdit->clear();
    });

  //Help system
  connect( ui->buttonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdDGetBus.htm", this ); });
  }





SdDGetBus::~SdDGetBus()
  {
  delete ui;
  }





void SdDGetBus::accept()
  {
  QString bus = ui->mBusEdit->text();
  if( translation( bus ) ) {
    previousBusList.addString( bus );
    done(1);
    }
  }



//Net list comma separated string parsed to string list
bool SdDGetBus::translation( const QString sour )
  {
  //Net list parsing
  //Список цепей вида "цепь|цепь<стартовый_индекс:конечный_индекс>{,...}"
  //Трансляция списка цепей
  if( !sour.isEmpty() ) {
    QStringList netList = sour.split( QChar(','), Qt::SkipEmptyParts );
    for( const QString &buf : qAsConst(netList) ) {
      if( buf.contains( QChar('<') )  || buf.contains( QChar('>') )  ) {
        //Индексный формат
        int nptr = buf.indexOf( QChar('<') ); //Ожидается скобка
        if( nptr < 0 )
          return syntaxError();

        int dptr = checkDigit( buf, nptr+1, QChar(':') ); //Проверить первый индекс и индексный разделитель
        if( dptr < 0 )
          return syntaxError();

        int start = buf.midRef( nptr + 1, dptr - nptr - 1 ).toInt(); //Выделить начальный индекс
        int tptr = checkDigit( buf, dptr + 1, QChar('>') ); //Проверить второй индекс и скобку
        if( tptr < 0 )
          return syntaxError();

        int stop = buf.midRef( dptr + 1, tptr - dptr - 1 ).toInt(); //Выделить конечный индекс

        //Calculate step
        int step = ( start > stop ) ? -1 : 1;

        //Loop for net list creating
        start -= step;
        do {
          start += step;
          //Form net name with index
          QString net;
          if( nptr )
            net = buf.mid( 0, nptr );
          net.append( QString::number(start) );
          if( tptr+1 < buf.count() ) net.append( buf.mid( tptr+1 ) );
          //Append if less then limit
          if( mNets.count() < maxBusNumber )
            mNets.append( net );
          }
        while( start != stop);
        }
      else {
        //Simple format. Append if less then limit
        if( mNets.count() < maxBusNumber )
          mNets.append( buf );
        }
      }
    return true;
    }
  return false;
  }



//Extract number position before delimiter.
int SdDGetBus::checkDigit(const QString buf, int index, QChar delim)
  {
  int last = buf.indexOf( delim, index );
  while( index < last )
    if( !buf.at(index++).isDigit() ) return -1;
  return last;
  }



//Display net list syntax error
bool SdDGetBus::syntaxError()
  {
  QMessageBox::warning( this, tr("Error"), tr("Wrong net list syntax. Use: netA,netB,net<0:2>") );
  return false;
  }
