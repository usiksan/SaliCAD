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
#include "SdDGetBus.h"
#include "ui_SdDGetBus.h"
#include <QStringList>
#include <QMessageBox>

static QStringList previousBusList;

SdDGetBus::SdDGetBus(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDGetBus)
  {
  ui->setupUi(this);

  //Fill previous net list
  ui->mBusList->addItems( previousBusList );
  }

SdDGetBus::~SdDGetBus()
  {
  delete ui;
  }



void SdDGetBus::accept()
  {
  QString bus = ui->mBusEdit->text();
  if( translation( bus ) ) {
    if( previousBusList.contains(bus) ) {
      previousBusList.removeOne( bus );
      }
    previousBusList.insert( 0, bus );
    done(1);
    }
  }




bool SdDGetBus::translation( const QString sour )
  {
  //Список цепей вида "цепь|цепь<стартовый_индекс:конечный_индекс>{,...}"
  //Трансляция списка цепей
  if( !sour.isEmpty() ) {
    QStringList netList = sour.split( QChar(','), QString::SkipEmptyParts );
    for( QString buf : netList ) {
      if( buf.contains( QChar('<') )  || buf.contains( QChar('>') )  ) {
        //Индексный формат
        int nptr = buf.indexOf( QChar('<') ); //Ожидается скобка
        if( nptr < 0 )
          return syntaxError();

        int dptr = checkDigit( buf, nptr+1, QChar(':') ); //Проверить первый индекс и индексный разделитель
        if( dptr < )
          return syntaxError();

        int start = buf.mid( nptr + 1, dptr - nptr - 1 ).toInt(); //Выделить начальный индекс
        int tptr = checkDigit( buf, dptr + 1, QChar('>') ); //Проверить второй индекс и скобку
        if( tptr < 0 )
          return syntaxError();

        int stop = buf.mid( dptr + 1, tptr - dptr - 1 ).toInt(); //Выделить конечный индекс

        //Calculate step
        int step = ( start > stop ) ? -1 : 1;

        //Loop for net list creating
        start -= step;
        do {
          start += step;
          //Form net name with index
          QString net;
          if( nptr ) buf.mid( 0, nptr );
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




int SdDGetBus::checkDigit(const QString buf, int index, QChar delim)
  {
  int last = buf.indexOf( delim, index );
  while( index < last )
    if( !buf.at(index++).isDigit() ) return -1;
  return last;
  }



bool SdDGetBus::syntaxError()
  {
  QMessageBox::warning( this, tr("Error"), tr("Wrong net list syntax. Use: netA,netB,net<0:2>") );
  return false;
  }