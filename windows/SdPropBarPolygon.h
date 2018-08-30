/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for tracing polygon
*/
#ifndef SDPROPBARPOLYGON_H
#define SDPROPBARPOLYGON_H

#include "SdPropBarStratum.h"

#define POLYGON_GAP_PREV_COUNT 10


class SdPropBarPolygon : public SdPropBarStratum
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;      //Logical coord per physical
    QComboBox *mGap;      //Gap for polygon windows

    QLineEdit *mWireName; //Name of net for polygon

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

  public:
    SdPropBarPolygon( const QString title );

    void setPropPolygon( SdPropPolygon *propPolygon, double ppm, int enterType );
    void getPropPolygon( SdPropPolygon *propPolygon, int *enterType );

  private:
    void setVertexType( int type );
  };

#endif // SDPROPBARPOLYGON_H
