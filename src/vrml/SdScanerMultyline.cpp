#include "SdScanerMultyline.h"

#include <QFile>

SdScanerMultyline::SdScanerMultyline() :
  mLineIndex(0)
  {

  }

void SdScanerMultyline::sourceSetStringList(QStringList lines)
  {
  mLines = lines;
  mLineIndex = 0;
  }




//!
//! \brief sourceSetString Set source lines as single line delimited eoln symbols
//! \param lines           Source lines as single line delimited eoln symbols
//!
void SdScanerMultyline::sourceSetString(QString lines)
  {
  sourceSetStringList( lines.split(QChar('\n')) );
  }





//!
//! \brief read Read previously opened file contents to scaner
//! \param file Previously opened file
//! \return     true - if reading successfull
//!
bool SdScanerMultyline::read(QIODevice *file)
  {
  sourceSetString( QString::fromUtf8( file->readAll() ) );
  return true;
  }




//!
//! \brief readFile Read file contents to scaner
//! \param path     Full path to file
//! \return         true - if reading successfull
//!
bool SdScanerMultyline::readFile(const QString &path)
  {
  QFile file(path);
  if( file.open( QIODevice::ReadOnly ) ) {
    return read( &file );
    }
  return false;
  }



bool SdScanerMultyline::nextLine()
  {
  if( mLineIndex < mLines.count() ) {
    lineSet( mLines.at(mLineIndex++) );
    return true;
    }
  return false;
  }
