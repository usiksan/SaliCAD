/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include <QFile>
#include <QPixmap>
#include <QCursor>
#include <QImage>
#include <QBitArray>
#include <QDataStream>

static QBitArray readBitmap(int width, int height, QDataStream &in) {
  QBitArray bitmap(width * height);
  quint8 byte;
  quint32 word = 0;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if( (j % 32) == 0 ) {
        word = 0;
        for( int k = 0; k < 4; ++k ) {
          in >> byte;
          word = (word << 8) | byte;
          }
        }
      bitmap.setBit(((height - i - 1) * width) + j, word & 0x80000000);
      word <<= 1;
      }
    }
  return bitmap;
  }

QCursor loadWindowsCursor( const QString fname ) {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    quint16 reserved;
    quint16 type;
    quint16 count;
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    in >> reserved >> type >> count;
    in.skipRawData(4);
    qint16 hotspot_x;
    qint16 hotspot_y;
    in >> hotspot_x >> hotspot_y;
    in.skipRawData(8);
    if( in.status() != QDataStream::Ok || reserved != 0 || type != 2 || count == 0 ) {
      return QCursor();
      }
    qint32 size;
    qint32 width;
    qint32 height;
    qint16 numPlanes;
    qint16 bitsPerPixel;
    qint32 compression;
    in >> size;
    if( size != 40 ) {
      return QCursor();
      }
    in >> width >> height >> numPlanes >> bitsPerPixel >> compression;
    height /= 2;
    if( numPlanes != 1 || bitsPerPixel != 1 || compression != 0 ) {
      return QCursor();
      }
    in.skipRawData( (size - 20) + 8 );
    QBitArray xorBitmap = readBitmap(width, height, in);
    QBitArray andBitmap = readBitmap(width, height, in);
    if( in.status() != QDataStream::Ok ) {
      return QCursor();
      }
    QImage img(width, height, QImage::Format_ARGB32);
    for( int i = 0; i < int(height); ++i) {
      for( int j = 0; j < int(width); ++j) {
        QRgb color;
        int bit = (i * width) + j;
        if( andBitmap.testBit(bit) ) {
          if (xorBitmap.testBit(bit) ) color = 0xff000000;
          else                         color = 0x00FFFFFF;
          }
        else {
          if( xorBitmap.testBit(bit) ) color = 0xFFFFFFFF;
          else                         color = 0xFF000000;
          }
        img.setPixel( j, i, color );
        }
      }
    //Save cursor for help perpose
//    static int cursorIndex = 0;
//    img.save( QString("cursor%1.png").arg(cursorIndex++) );
    QCursor cur( QPixmap::fromImage(img), hotspot_x, hotspot_y );
    return cur;
    }

  return QCursor();
  }


