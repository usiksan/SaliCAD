/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for temporary modes.
*/
#ifndef SDMODETEMP_H
#define SDMODETEMP_H

#include "SdMode.h"

class SdModeTemp : public SdMode
  {
    SdMode *mMainMode; //!< Main mode that has been temporarily replaced by this mode. This mode acts as a placeholder/container.

  public:
    //! \brief SdModeTemp Constructor for temporary mode
    //! \param editor     Pointer to the graphical editor
    //! \param obj        Pointer to the project item being edited
    SdModeTemp( SdWEditorGraph *editor, SdProjectItem *obj );


    //! \brief setMainMode Set new main mode to be stored/restored
    //! \param mode        Pointer to the main mode instance
    void    setMainMode( SdMode *mode ) { mMainMode = mode; }


    //! \brief mainMode Get the stored main mode
    //! \return         Pointer to the main mode instance
    SdMode *mainMode() { return mMainMode; }


    // SdMode interface
  public:
    //! \brief drawStatic Draw static (non-interactive) elements of the scene
    //! \param ctx        Drawing context for rendering
    virtual void drawStatic(SdContext *ctx) override;
  };
#endif // SDMODETEMP_H
