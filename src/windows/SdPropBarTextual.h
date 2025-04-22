/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for text and other text based objects
*/
#ifndef SDPROPBARTEXTUAL_H
#define SDPROPBARTEXTUAL_H

#include "SdPropBar.h"
#include "objects/SdPropText.h"

#define FONT_SIZE_PREV_COUNT 10

class SdPropBarTextual : public SdPropBar
  {
    Q_OBJECT

  protected:
    //Font list
    QComboBox *mFont;

    //Size of text
    double     mPPM;   //Logical coord per physical
    QComboBox *mSize;  //Vertical size of one line text

    //Vertical alignment
    QAction   *mVertTop;
    QAction   *mVertMiddle;
    QAction   *mVertBottom;

    //Horizontal alignment
    QAction   *mHorzLeft;
    QAction   *mHorzCenter;
    QAction   *mHorzRight;

    //Text direction
    QAction   *mDir0;
    QAction   *mDir90;
    QAction   *mDir180;
    QAction   *mDir270;

  public:
    SdPropBarTextual( const QString title );

    void setPropText( SdPropText *propText, double ppm );
    void getPropText( SdPropText *propText );

  private:
    void setFont( int index );
    void setSize( double size );
    void setVerticalAlignment( int vert );
    void setHorizontalAlignment( int horz );
    void setDirection( int dir );
  };

#endif // SDPROPBARTEXTUAL_H
