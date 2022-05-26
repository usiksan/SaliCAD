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
  SdGraphSymPin  *mPin;       //!< Pin
  QString         mPinNumber; //!< Pin number in part
  SdPoint         mPosition;  //!< Pin position in sheet context
  QString         mNetName;   //!< Net, which pin connected to

  SdSymImpPin();

  //SdSymImpPin &operator = (const SdSymImpPin &pin );
  void         draw( SdContext *dc ) const;
  bool         isCanConnect( SdPoint a, SdPoint b ) const;
  bool         isCanDisconnect( SdPoint a, SdPoint b, const QString wireName ) const;
  void         prepareMove(SdPItemSheet *sheet, SdSelector *selector , SdUndo *undo);
  bool         isConnected() const { return !mNetName.isEmpty(); }

  QString      fromJson( const SdJsonReader &js );


  //!
  //! \brief json Function to write object content into json writer
  //! \param js   Json writer
  //!
  void         json( SdJsonWriter &js ) const;

  //!
  //! \brief json Function to read object content from json reader
  //! \param js   Json reader
  //!
  void         json( const SdJsonReader &js);

  };


typedef QMap<QString,SdSymImpPin> SdSymImpPinTable;

#endif // SDSYMIMPPIN_H
