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
#include "SdSysColors.h"
#include <QSettings>

QColor SdSysColors::mSysColors[scLast];


QColor SdSysColors::getSysColor(int colorId)
  {
  if( colorId == scUnvisible )
    return QColor(0,0,0,0);
  //Если вне диапазона, то вернуть черный цвет
  if( colorId < 0 || colorId >= scLast )
    return QColor(0,0,0);
  return mSysColors[colorId];
  }

static QColor loadColor( int id, QColor def ) {
  QSettings s;
  return QColor( QRgb( s.value( QString(SDK_COLOR).arg(id), (unsigned)(def.rgba()) ).toUInt() ) );
  }

void SdSysColors::loadColors()
  {
  loadColor( scLocked, QColor(200,200,0) );      //Цвет заблокированных элементов
  loadColor( scSelected, QColor(150,150,150) );  //Цвет выбранных элементов
  loadColor( scEnter, QColor(65,65,65) );        //Цвет ввода
  loadColor( scSmart, QColor(36,27,99) );        //Цвет разумного режима
  loadColor( scGraphBack, QColor(255,255,255) ); //Цвет фона графического редактора
  loadColor( scRatNet, QColor(30,30,30) );       //Цвет резинок
  loadColor( scCursor, QColor(0,0,0) );          //Цвет курсора
  loadColor( scTraseNet, QColor(0,0,0) );        //Цвет трассируемой цепи
  loadColor( scGrid, QColor(100,100,100) );      //Цвет сетки

  }
