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


static QImage mouse[8];

SdGuiderFile::SdGuiderFile()
  {
  //By default no speach
  mTiterChanged = [] () { };

  if( mouse[0].isNull() ) {
    mouse[0] = QImage(QString(":/pic/mouse0.png"));
    mouse[1] = QImage(QString(":/pic/mouse1.png"));
    mouse[2] = QImage(QString(":/pic/mouse2.png"));
    mouse[3] = QImage(QString(":/pic/mouse3.png"));
    mouse[4] = QImage(QString(":/pic/mouse4.png"));
    mouse[5] = QImage(QString(":/pic/mouse5.png"));
    mouse[6] = QImage(QString(":/pic/mouse6.png"));
    mouse[7] = QImage(QString(":/pic/mouse7.png"));
    }
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
    case SD_GT_TITER :
      mTiterIndex = index;
      mTiter.read( mFile.at(index).mData );
      mTiterChanged();
      break;
    case SD_GT_TITER_HIDE :
      mTiterIndex = -1;
      mTiterChanged();
      break;
    }
  }







QPixmap SdGuiderFile::build()
  {
  //Current background
  QPixmap pix( QPixmap::fromImage(mBack) );
  {
  QPainter painter( &pix );
  painter.setPen( Qt::red );
  //qDebug() << mCursorPos;
  int mouseIndex = 0;
  if( mMouseButtons & Qt::LeftButton ) mouseIndex |= 1;
  if( mMouseButtons & Qt::MidButton ) mouseIndex |= 2;
  if( mMouseButtons & Qt::RightButton ) mouseIndex |= 4;
  //Draw mouse
  painter.drawImage( QPoint( mCursorPos.x() - 16, mCursorPos.y() - 4 ), mouse[mouseIndex] );
  }
  //pix.save( QString("c:\\work\\t1.png") );
  return pix;
  }




void SdGuiderFile::updateTiter()
  {
  if( mTiterIndex >= 0 ) {
    mFile[mTiterIndex].mData = mTiter.write();
    }
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
    case SD_GT_TITER :
      return str.arg(mTime).arg( QString("!!! Titer show") );
    case SD_GT_TITER_HIDE :
      return str.arg(mTime).arg( QString("!!! Titer hide") );
    case SD_GT_STOP :
      return str.arg(mTime).arg( QString("Stop file") );
    }
  return str.arg(mTime).arg( QString() );
  }




QByteArray SdGuiderTiter::write() const
  {
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mContens;
  return ar;
  }




void SdGuiderTiter::read(const QByteArray &src)
  {
  QDataStream is( src );
  is >> mContens;
  }



void SdGuiderTiter::clear()
  {
  mContens.clear();
  }


