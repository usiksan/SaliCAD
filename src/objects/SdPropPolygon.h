/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing polygon properties. It contains windows gap, net name which polygon and polygon stratum
*/
#ifndef SDPROPPOLYGON_H
#define SDPROPPOLYGON_H

#include "SdPropInt.h"
#include "SdPropString.h"
#include "SdPropStratum.h"

struct SdPropPolygonState {
    int     mGap;
    QString mNetName;
    int     mStratum;
  };

struct SdPropPolygon
  {
    SdPropInt    mGap;     //Polygon gap [Зазоры окон полигона]
    SdPropString mNetName; //Net name which polygon is
    SdPropStratum    mStratum; //Polygon stratum

    SdPropPolygon() : mGap(100), mNetName(), mStratum(1) { }
    void operator = ( SdPropPolygon const &prop );  //Назначить свойства в соответствии с шаблоном
    void append( SdPropPolygon const &prop );       //Добавить свойства
    bool operator == ( SdPropPolygon const &prop ); //Сравнить свойства
    void clear();                                   //Установить в неопределенное состояние
    bool match( SdPropPolygon const &prop );        //Сравнить на совпадение с эталоном



    //!
    //! \brief json Function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SvJsonReader &js);

    void saveState( SdPropPolygonState *dst );
    void swapState( SdPropPolygonState *src );
  };

#endif // SDPROPPOLYGON_H
