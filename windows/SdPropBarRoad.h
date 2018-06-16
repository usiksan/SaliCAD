/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for tracing road
*/
#ifndef SDPROPBARROAD_H
#define SDPROPBARROAD_H

#include "SdPropBarStratum.h"

#define ROAD_WIDTH_PREV_COUNT 10

class SdPropBarRoad : public SdPropBarStratum
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;      //Logical coord per physical
    QComboBox *mWidth;    //Width of linear objects

    QLineEdit *mWireName; //Name of net for road

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;
  public:
    SdPropBarRoad( const QString title );

    void setPropRoad( SdPropRoad *propRoad, double ppm, int enterType );
    void getPropRoad( SdPropRoad *propRoad, int *enterType );

  private:
    void setWidth( double width );
    void setVertexType( int type );
  };

#endif // SDPROPBARROAD_H
