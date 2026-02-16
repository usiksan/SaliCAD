/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for lines and other linear objects
*/
#ifndef SDPROPBARLINEAR_H
#define SDPROPBARLINEAR_H

#include "SdPropBar.h"
#include "objects/SdPropLine.h"

#include <QComboBox>

#define LINEAR_WIDTH_PREV_COUNT 10

class SdPropBarLinear : public SdPropBar
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;   //Logical coord per physical
    QComboBox *mWidth; //Width of linear objects

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Line type
    QAction   *mLineSolid;
    QAction   *mLineDotted;
    QAction   *mLineDashed;
  public:
    SdPropBarLinear( const QString title );

    void setPropLine( SdProjectItem *pitem, SdPropLine *propLine, double ppm, int enterType );
    void getPropLine( SdPropLine *propLine, int *enterType );

  private:
    void setVertexType( int type );
    void setLineType( int type );
  };

#endif // SDPROPBARLINEAR_H
