/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project item object which can contains part link object (part variant)
*/
#ifndef SDPITEMWITHPART_H
#define SDPITEMWITHPART_H

#include "SdProjectItem.h"
#include "SdPartVariant.h"

class SdPItemWithPart : public SdProjectItem
  {
  public:
    SdPItemWithPart();

    //Part
    //!
    //! \brief isPartPresent Returns true if contains one or more part variants
    //! \return              true if contains one or more part variants
    //!
    bool             isPartPresent() const;

    //!
    //! \brief getPartTitle Returns part title for visual presentation
    //! \param id           Id of part variant which title need to be returned. If empty then return primary part title
    //! \return             Part title for visual presentation
    //!
    QString          getPartTitle( const QString id = QString{} ) const;

    //!
    //! \brief getPartId Returns part id for primary part variant
    //! \return          Part id for primary part variant
    //!
    QString          getPartId() const;

    //!
    //! \brief getPartIdList Returns part ids for all parts variants
    //! \return              Part ids for all parts variants
    //!
    QStringList      getPartIdList() const;

    //!
    //! \brief setPartId Setup new part id for primary part variant
    //! \param id        New id for primary part
    //! \param undo
    //!
    void             setPartId( const QString id, SdUndo *undo );

    //!
    //! \brief extractPartFromFactory Return part descripted part variant
    //! \param id                     Id of part which need to returned. If empty return primary part
    //! \return                       Part descripted part variant
    //!
    SdPItemPart     *extractPartFromFactory( const QString id = QString{} ) const;

    //!
    //! \brief getPart Returns part variant for id. If id is empty then return primary part variant
    //! \param id      Id of part which variant need to returned. If empty return primary part variant
    //! \return
    //!
    SdPartVariant   *getPart( const QString id = QString{} ) const;

    //!
    //! \brief removePart Removes part variant from part list
    //! \param id         Id of part which variant need to be removed
    //! \param undo
    //!
    void             removePart( const QString id, SdUndo *undo );
  };

#endif // SDPITEMWITHPART_H
