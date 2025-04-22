/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for wires
*/
#ifndef SDPROPBARWIRE_H
#define SDPROPBARWIRE_H

#include "SdPropBar.h"
#include "objects/SdPropLine.h"

#include <QComboBox>

#define WIRE_WIDTH_PREV_COUNT 10
#define WIRE_NAME_PREV_COUNT  10


class SdPropBarWire : public SdPropBar
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;   //Logical coord per physical
    QComboBox *mWidth; //Width of linear objects

    QComboBox *mWireName; //Name of nets for wires

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Line type
    QAction   *mLineSolid;
    QAction   *mLineDotted;
    QAction   *mLineDashed;
  public:
    SdPropBarWire( const QString title );

    void setPropWire( SdPropLine *propLine, double ppm, int enterType, const QString wireName );
    void getPropWire( SdPropLine *propLine, int *enterType, QString *wireName );

  private:
    void setVertexType( int type );
    void setLineType( int type );
  };

#endif // SDPROPBARWIRE_H
