/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin for symbol implementation
*/
#ifndef SDSYMIMPPIN_H
#define SDSYMIMPPIN_H

#include "SdGraph.h"
#include "SdPoint.h"

#include <QString>
#include <QMap>


class SdGraphSymPin;
class SdPItemSheet;

//Pin for symbol implementation
struct SdSymImpPin {
  SdGraphSymPin  *mPin;       //Pin
  QString         mPinNumber; //Pin number in part
  SdPoint         mPosition;  //Pin position in sheet context
  QString         mWireName;  //Net, which pin connected to

  SdSymImpPin();

  void        operator = (const SdSymImpPin &pin );
  void        draw( SdContext *dc ) const;
  bool        isCanConnect( SdPoint a, SdPoint b ) const;
  bool        isCanDisconnect( SdPoint a, SdPoint b, const QString wireName ) const;
  void        prepareMove( SdPItemSheet *sheet, SdSelector *selector );
  bool        isConnected() const { return !mWireName.isEmpty(); }

  QJsonObject toJson(const QString pinName) const;
  QString     fromJson( SdObjectMap *map, const QJsonObject obj );

  };


typedef QMap<QString,SdSymImpPin> SdSymImpPinTable;

#endif // SDSYMIMPPIN_H
