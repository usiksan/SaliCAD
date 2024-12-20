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
    //! \brief partIsPresent Returns true if contains one or more part variants
    //! \return              true if contains one or more part variants
    //!
    bool             partIsPresent() const;

    //!
    //! \brief partTitleGet Returns part title for visual presentation
    //! \param id           Id of part variant which title need to be returned. If empty then return primary part title
    //! \return             Part title for visual presentation
    //!
    QString          partTitleGet() const;

    //!
    //! \brief partIdGet Returns part id for primary part variant
    //! \return          Part id for primary part variant
    //!
    QString          partIdGet() const;

    //!
    //! \brief partIdSet Setup new part id for primary part variant
    //! \param id        New id for primary part
    //! \param undo
    //!
    void             partIdSet( const QString id, SdUndo *undo );

    //!
    //! \brief partExtractFromFactory Return part descripted part variant
    //! \return                       Part descripted part variant
    //!
    SdPItemPart     *partExtractFromFactory() const;

    //!
    //! \brief partGet Returns part variant for id. If id is empty then return primary part variant
    //! \return
    //!
    SdPartVariant   *partGet() const;

    //!
    //! \brief partRemove Removes part variant from part list
    //! \param undo
    //!
    void             partRemove( SdUndo *undo );
  };

#endif // SDPITEMWITHPART_H
