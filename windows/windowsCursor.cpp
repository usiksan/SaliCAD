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


QCursor loadWindowsCursor( const QString fname ) {
  quint8 buf[4];
  char *cbuf = (char*)buf;
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    file.read( cbuf, 2 ); // reserved. always 0
    file.read( cbuf, 2 ); // image type; we're only interested for .cur (=2)
    int imagetype = buf[0] | (buf[1]<<8);
    if( imagetype!=2 ) {
      // error:  file is not a valid .cur file
      return QCursor();
      }
    file.read( cbuf, 2 );    // number of images
    // we're only interested in the first image
    file.read( cbuf, 1 );    // width  (0 to 255; 0=means 256 width)
    int width = (buf[0]==0 ? 256 : buf[0]);
    file.read( cbuf, 1 );    // height (0 to 255; 0=means 256 height)
    int height = (buf[0]==0 ? 256 : buf[0]);
    file.read( cbuf, 1 );    // number of colors in palette (0 for no palette)
    file.read( cbuf, 1 );    // reserved. should be 0
    file.read( cbuf, 2 );    // hotspot x
    int hotspot_x = buf[0] | (buf[1]<<8);
    file.read( cbuf, 2 );    // hotspot y
    int hotspot_y = buf[0] | (buf[1]<<8);
    file.read( cbuf, 4 );    // image data in bytes
    file.read( cbuf, 4 );    // offset to image data

    // Now we need to verify if image in .cur is BMP or PNG (Vista+)
    // We can't just check 'magic' since if it's BMP, the header will be missing (PNG is whole)
    // So we search for PNG magic; if doesnt exist, we have a BMP!
    // NOTE:  for simplicity we go only for BMP for the moment.
    //        So just check if 'biSize' is 40 (Windows NT & 3.1x or later)

    // BITMAPINFOHEADER
    file.read( cbuf, 4 );    // biSize
    int biSize = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
    if( biSize != 40 ) {
      // error:  file does not contain valid BMP data;
      return QCursor();
      }
    file.read( cbuf, 4 );    // biWidth
    qint32 biWidth = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
    file.read( cbuf, 4 );    // biHeight (if positive => bottom-up, if negative => up-bottom)
    qint32 biHeight = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
    file.read( cbuf, 2 );    // biPlanes
    file.read( cbuf, 2 );    // biBitCount
    int biBitCount = (buf[0]&0xff) | (buf[1]<<8);
    if( biBitCount != 24 && biBitCount != 32 ) {
      // error:  only 24/32 bits supported;
      return QCursor();
      }
    file.read( cbuf, 4 );    // biCompression
    int biCompression = (buf[0]&0xff) | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
    // we want only uncompressed BMP data
    if( biCompression != 0 /*BI_RGB*/ ) {
      // error:  file is compressed; only uncompressed BMP is supported;
      return QCursor();
      }
    file.read( cbuf, 4 );    // biSizeImage
    file.read( cbuf, 4 );    // biXPelsPerMeter
    file.read( cbuf, 4 );    // biYPelsPerMeter
    file.read( cbuf, 4 );    // biClrUsed
    file.read( cbuf, 4 );    // biClrImportant

    // DECODE IMAGE
    bool bottomUp = biHeight > 0;    // true=bottom-up, false=up-bottom
    // there are cases where BMP sizes are NOT the same with cursor; we use the cursor ones
    biWidth = width;
    biHeight = height;

    QImage img(biWidth, biHeight, QImage::Format_ARGB32);

    for(int y = 0; y < biHeight; y++ ) {
      for(int x = 0; x < biWidth; x++ ) {
        // read pixels by number of bits
        switch(biBitCount) {
          // 24-bit
          case 24:
            file.read( cbuf, 3 );
            img.setPixel( x, bottomUp ? biHeight - 1 - y : y, 0xff000000 | static_cast<unsigned>( ((buf[0]<<16) | (buf[1]<<8) | buf[2]) ) );
            break;
          // 32-bit
          case 32:
            file.read( cbuf, 4 );
            img.setPixel( x, bottomUp ? biHeight - 1 - y : y, static_cast<unsigned>( ((buf[3] << 24) | (buf[0]<<16) | (buf[1]<<8) | buf[2]) ) );
            break;
          }
        }
      }

    // read mask
    // mask is 1-bit-per-pixel for describing the cursor bitmap's opaque and transparent pixels.
    // so for 1 pixel we need 1 bit, for etc. 32 pixels width, we need 32*1 bits (or 4 bytes per line)
    // so for etc. 32 pixels height we need 4*32 = 128 bytes or [mask bytes per line * height]

    // now extract all bits from mask bytes into new array (equal to width*height)
    // reverse vector (?)
    // now the bits contains =1 (transparent) and =0 (opaque) which we use on cursor directly
    int mask_bytes_per_line = biWidth / 8;

    // reverse every [mask_bytes_per_line] bytes; (etc. for 4 bytes per line do:  0,1,2,3 => 3,2,1,0) -> you really need to ask Microsoft for this 8)
    for( int y = 0; y < biHeight; y++ ) {
      int py = biHeight - 1 - y;
      int px = biWidth - 1;
      for( int x = 0; x < mask_bytes_per_line; x++ ) {
        file.read( cbuf, 1 );
        for( int b = 0; b < 8; b++ ) {
          if( buf[0] & (1<<b) )
            img.setPixel( px, py, 0xffffff & img.pixel(px,py) );
          px--;
          if( px < 0 ) break;
          }
        }
      }

    QCursor cur( QPixmap::fromImage(img), hotspot_x, hotspot_y );
    return cur;
    }
  return QCursor();
  }


