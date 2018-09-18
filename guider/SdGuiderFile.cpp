/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGuiderFile.h"
#include <QFile>
#include <QDataStream>
#include <QBuffer>
#include <QPainter>
#include <QDebug>

inline QDataStream &operator << ( QDataStream &os, const SdGuiderTime &time ) {
  os << time.mData << time.mTime << time.mType;
  return os;
  }

inline QDataStream &operator >> ( QDataStream &is, SdGuiderTime &time ) {
  is >> time.mData >> time.mTime >> time.mType;
  return is;
  }


SdGuiderFile::SdGuiderFile()
  {

  }

void SdGuiderFile::addImage(QImage img, int time)
  {
  if( mFile.count() && mBack.size() == img.size() && (time % 100) ) {
    QImage dif( img );
    int w = img.size().width();
    int h = img.size().height();
    bool eq = true;
    for( int y = 0; y < h; y++ )
      for( int x = 0; x < w; x++ )
        if( mBack.pixel(x,y) == dif.pixel(x,y) )
          dif.setPixelColor( x, y, Qt::transparent );
        else eq = false;
    if( !eq ) {
      SdGuiderTime tm;
      QBuffer buf( &tm.mData );
      dif.save( &buf, "PNG" );
      tm.mTime = time;
      tm.mType = SD_GT_DIF_IMAGE;
      mFile.append( tm );
      mBack = img;
      }
    }
  else {
    SdGuiderTime tm;
    QBuffer buf( &tm.mData );
    img.save( &buf, "PNG" );
    tm.mTime = time;
    tm.mType = SD_GT_KEY_IMAGE;
    mFile.append( tm );
    mBack = img;
    }
  }




void SdGuiderFile::addCursor(quint32 buttons, QPoint cursorPos, int time)
  {
  if( mMouseButtons != buttons || mCursorPos != cursorPos ) {
    SdGuiderTime tm;
    {
    QDataStream os( &tm.mData, QIODevice::WriteOnly );
    mMouseButtons = buttons;
    mCursorPos    = cursorPos;
    os << mMouseButtons << mCursorPos;
    }
    tm.mTime = time;
    tm.mType = SD_GT_MOUSE;
    mFile.append( tm );
    }
  }




void SdGuiderFile::addStop(int time)
  {
  SdGuiderTime tm;
  tm.mTime = time;
  tm.mType = SD_GT_STOP;
  mFile.append( tm );
  }




void SdGuiderFile::play(int index)
  {
  switch( mFile.at(index).mType ) {
    case SD_GT_KEY_IMAGE :
      mBack.loadFromData( mFile.at(index).mData, "PNG" );
      break;
    case SD_GT_DIF_IMAGE : {
      QImage dif;
      dif.loadFromData( mFile.at(index).mData, "PNG" );
      QPainter painter( &mBack );
      painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
      painter.drawImage( 0,0, dif );
      }
      break;
    case SD_GT_MOUSE : {
      QDataStream is( mFile.at(index).mData );
      is >> mMouseButtons >> mCursorPos;
      }
      break;
    }
  }


void drawButton( QPainter &painter, int x, int y, bool press ) {
  QRect left( x, y, 10, 20 );
  if( press )
    painter.setBrush( QBrush(Qt::green) );
  else
    painter.setBrush( QBrush(Qt::transparent) );
  painter.drawRect( left );
  }


QPixmap SdGuiderFile::build()
  {
  //Current background
  QPixmap pix( QPixmap::fromImage(mBack) );
  {
  QPainter painter( &pix );
  painter.setPen( Qt::red );
  //qDebug() << mCursorPos;
  //Draw left button
  drawButton( painter, mCursorPos.x() - 15, mCursorPos.y() + 3, mMouseButtons & Qt::LeftButton );
  //Draw middle button
  drawButton( painter, mCursorPos.x() - 5, mCursorPos.y(), mMouseButtons & Qt::MidButton );
  //Draw right button
  drawButton( painter, mCursorPos.x() + 5, mCursorPos.y() + 3, mMouseButtons & Qt::RightButton );
  //painter.drawText( 100, 100, QString("x:%1  y:%2").arg(mCursorPos.x()).arg(mCursorPos.y()) );
  if( !mTitle.isEmpty() ) {
    painter.drawText( mTitlePos, mTitle );
    }
  }
  //pix.save( QString("c:\\work\\t1.png") );
  return pix;
  }




void SdGuiderFile::save(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::WriteOnly) ) {
    QDataStream os( &file );
    os << mFile;
    }
  }




void SdGuiderFile::load(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    QDataStream is( &file );
    is >> mFile;
    }
  }



QString SdGuiderTime::title() const
  {
  QString str( "%1 %2");
  switch( mType ) {
    case SD_GT_KEY_IMAGE :
      return str.arg(mTime).arg( QString("Key image") );
    case SD_GT_DIF_IMAGE :
      return str.arg(mTime).arg( QString("Dif image") );
    case SD_GT_MOUSE :
      return str.arg(mTime).arg( QString("Mouse position") );
    }
  return str.arg(mTime).arg( QString() );
  }
