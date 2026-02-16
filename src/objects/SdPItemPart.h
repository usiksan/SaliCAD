/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics representation part of component
*/
#ifndef SDPITEMPART_H
#define SDPITEMPART_H

#include "SdProjectItem.h"

#include <QMatrix4x4>

#define SD_TYPE_PART "Part"

class Sd3dGraphModel;

class SdPItemPart : public SdProjectItem
  {
  public:
    SdPItemPart();

    //!
    //! \brief matrixMap Apply matrix to all 3d objects of part
    //! \param matrix    Matrix which applied to all 3d objects
    //! \param undo      Undo to which writed undo for this operation. If no
    //!                  undo then no undo for this operation
    //!
    void            matrixMap( QMatrix4x4 matrix, SdUndo *undo );

    //!
    //! \brief model Return first 3d model object. If no model object, then it created and returned
    //! \return      3d model object
    //!
    Sd3dGraphModel *model();

    //!
    //! \brief model Return first 3d model object. If no model object, then return nullptr
    //! \return      3d model object or nullptr if no any 3d model
    //!
    Sd3dGraphModel *modelConst() const;

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual quint64        getClass() const override;

    // SdProjectItem interface
  public:
    virtual SdStratum      stratum() const { return stmTop | stmBottom; }
    virtual QString        getIconName() const override;
    virtual bool           is3dAllowed() const override { return true; }
    virtual quint64        getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMPART_H
