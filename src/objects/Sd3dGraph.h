/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base object for 3d graphics
*/
#ifndef SD3DGRAPH_H
#define SD3DGRAPH_H

#include "SdGraph.h"

#include <QMatrix3x2>


class Sd3dGraph : public SdGraph
  {
  public:
    Sd3dGraph();

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    virtual void    volumeAdd( QMatrix2x3 &volume ) const { Q_UNUSED(volume) }

    // SdObject interface
  public:
    virtual SdClass getClass() const override;
  };

#endif // SD3DGRAPH_H
