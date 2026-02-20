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
#include "objects/SdPropRoad.h"
#include "objects/SdPropVia.h"

#define ROAD_WIDTH_PREV_COUNT 10

class SdPropBarRoad : public SdPropBarStratum
  {
    Q_OBJECT

  protected:
    //Width of line
    double     mPPM;               //Logical coord per physical
    QComboBox *mWidth;             //Width of linear objects

    QLineEdit *mWireName;          //Name of net for road

    //Cursor align to greed or not
    QAction   *mAlignToGrid;

    //Enable-disable automatic road loop detection and removing
    QAction   *mLoopDetection;

    //Vertex type of two lines
    QAction   *mEnterOrtho;
    QAction   *mEnter45degree;
    QAction   *mEnterAnyDegree;

    //Vias
    QAction     *mViaThrough;      //Via through or blind
    QComboBox   *mViaPadType;      //Via pad type


  public:
    SdPropBarRoad(const QString title, bool asRoad = true );

    void setPropRoad( const SdPropRoad &propRoad, const SdPropVia &propVia, double ppm, int enterType );
    void getPropRoad( SdPropRoad &propRoad, SdPropVia &propVia, int *enterType );

    void setPropRoad( const SdPropComposerRoad &propRoad, const SdPropComposerVia &propVia, double ppm, int enterType );
    void getPropRoad( SdPropComposerRoad &propRoad, SdPropComposerVia &propVia, int *enterType );

    void setPropVia( SdPropVia *propVia );
    void getPropVia( SdPropVia *propVia );

    void setViaRule( int viaRule );
    int  getViaRule();

  private:
    void setVertexType( int type );
  };

#endif // SDPROPBARROAD_H
