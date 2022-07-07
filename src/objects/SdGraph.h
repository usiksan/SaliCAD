/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for objects with graphical presentation
*/

#ifndef SDGRAPHOBJECT_H
#define SDGRAPHOBJECT_H

#include "SdObject.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdPropSelected.h"

#include <QOpenGLFunctions_2_0>

class SdSelector;
class SdContext;
class SdSnapInfo;
class QWidget;
class SdProjectItem;

class SdGraph : public SdObject
  {
  protected:
    SdSelector *mSelector;
  public:
    SdGraph();

    //Select object
    SdSelector*    getSelector() { return mSelector; }
    bool           isSelected() const { return mSelector != nullptr; }

    //Save object state
    virtual void   saveState( SdUndo *undo );
    virtual void   moveComplete( SdPoint grid, SdUndo *undo );

    //Изменение перемещением
    virtual void   move( SdPoint offset );
    virtual void   rotate( SdPoint center, SdPropAngle angle );
    virtual void   mirror( SdPoint a, SdPoint b );

    //Изменение свойствами
    virtual void   setProp( SdPropSelected &prop );
    virtual void   getProp( SdPropSelected &prop );

    //Изменение обновлением текста
    virtual void   setText(int index, QString sour, SdPropText &prop, QWidget *parent );

    //Варианты выделения
    virtual void   selectByPoint( const SdPoint p, SdSelector *selector );
    virtual void   selectByRect( const SdRect &r, SdSelector *selector );
    virtual void   select( SdSelector *selector );
    virtual void   prepareMove( SdUndo *undo );
    //Layer maintance
    virtual void   setLayerUsage();
    //Check if object is visible
    virtual bool   isVisible() { return false; }
    //Get over rect for visible
    virtual SdRect getOverRect() const = 0;

    //!
    //! \brief draw Draws object in 2d space [Рисование объекта на экране]
    //! \param dc   Display context where drawing doing
    //!
    virtual void   draw( SdContext *dc );

    //!
    //! \brief drawExceptText Draws object in 2d space with exception textual component with textId
    //! \param dc             Display context where drawing doing
    //! \param textId         Text id which must not be drawn
    //!
    virtual void   drawExceptText( SdContext *dc, int textId );

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void   draw3d( QOpenGLFunctions_2_0 *f ) const;

    //!
    //! \brief accumHoles Accum holes description into faceList
    //! \param model      Model which accumulate coord vertexes
    //! \param faceList   Face list for holding holes (single face for single hole)
    //! \param stratum    Stratum for layers
    //! \param map        Map for holes conversion
    //!
    virtual void   accumHoles( Sd3dModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map ) const;

    //Get object state behind cursor
    //!
    //! \brief behindCursor Return object status behind cursor, one of constant ELEM_xxx
    //! \param p            Point of cursor position
    //! \return             One of constant ELEM_xxx (outside any element, on selected element or on unselected element)
    //!
    virtual int    behindCursor( SdPoint p );

    //!
    //! \brief behindText Return text id behind cursor of this element
    //! \param p          Point of cursor position
    //! \param org        Origin point of textual component
    //! \param dest       Text of textual component
    //! \param prop       Properties of textual component
    //! \return           0 - if none textual components behind cursor or >0 (i.e. 1, 2 and so on) if any textual component
    //!
    //! Some elements may be have more than one textual component, for example pin. It has pin name and pin number
    //! So, for enable edit such components separately we return with this function Id for particular component
    virtual int    behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop );

    virtual bool   getInfo( SdPoint p, QString &info, bool extInfo );
    //Find snap point on object
    virtual void   snapPoint( SdSnapInfo *snap );

    friend class SdSelector;
  };

typedef SdGraph *SdGraphPtr;

#endif // SDGRAPHOBJECT_H
