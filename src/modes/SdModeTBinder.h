/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Temporary binder mode to bind to some point
*/
#ifndef SDMODETBINDER_H
#define SDMODETBINDER_H

#include "SdModeTemp.h"

class SdModeTBinder : public SdModeTemp
  {
    SdPoint        mSmartPoint; //!< Smart input point - calculated optimal point for object placement
    SdSnapMask     mSmartType;  //!< Smart point type - defines which snapping options are active

  public:
    //!
    //! \brief SdModeTBinder Constructor for temporary binder mode
    //! \param editor        Pointer to the graphical editor
    //! \param obj           Pointer to the project item being edited
    //!
    SdModeTBinder( SdWEditorGraph *editor, SdProjectItem *obj );

    //! \brief drawDynamic Draw dynamic (interactive) part of the scene during editing
    //! \param ctx         Drawing context for rendering
    virtual void    drawDynamic( SdContext *ctx );

    //! \brief enterPoint Process point input (left mouse button click)
    //! \param p          Input point coordinates
    virtual void    enterPoint( SdPoint p );

    //! \brief cancelPoint Process point rejection/cancellation (right mouse button click)
    //! \param p           Point coordinates at cancellation moment
    virtual void    cancelPoint( SdPoint p );

    //! \brief movePoint Process point movement/mouse move
    //! \param p         Current point coordinates during movement
    virtual void    movePoint( SdPoint p );

    //! \brief keyDown Process keyboard key press
    //! \param key     Key code
    //! \param ch      Character representation of the key
    virtual void    keyDown( int key, QChar ch );

    //! \brief getStepHelp Get brief help text for current editing step
    //! \return            Help text string
    virtual QString getStepHelp() const;

    //! \brief getModeThema Get help page address for the mode
    //! \return             Help page URL or identifier
    virtual QString getModeThema() const;

    //! \brief getStepThema Get help page address and link for current editing step
    //! \return             Help page URL or identifier for current step
    virtual QString getStepThema() const;

    //! \brief getCursor Get cursor number/type for this mode
    //! \return          Cursor identifier
    virtual int     getCursor() const;

    //! \brief getIndex Get mode index in the mode list
    //! \return         Mode index number
    virtual int     getIndex() const;
  };


#endif // SDMODETBINDER_H
