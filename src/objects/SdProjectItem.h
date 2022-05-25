/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base for all project items
*/

#ifndef SDPROJECTITEM_H
#define SDPROJECTITEM_H

#include "SdContainer.h"
#include "SdPoint.h"
#include "SdSelector.h"
#include "library/SdStringMap.h"
#include "library/SdLibraryHeader.h"

#include <QString>
#include <QOpenGLFunctions_2_0>


class SdProject;
class QTreeWidgetItem;
class SdGraphIdent;
class SdGraphValue;
class SdWEditorGraph;

class SdProjectItem : public SdContainer
  {
    QString                mTitle;      //!< Item title
    QString                mAuthor;     //!< Item author (registered program copy name)
    int                    mCreateTime; //!< Create time with sec from 2000year
    bool                   mAuto;       //!< True if item inserted automatic as reference from other item
    bool                   mEditEnable; //!< True if edit enable for this object
  protected:
    SdPoint                mOrigin;     //!< Origin for object
  public:
    QTreeWidgetItem       *mTreeItem;   //!< Correspond visual tree item
    QTreeWidgetItem       *m3dTreeItem; //!< Correspond visual tree item for 3d view (if present)
    bool                   mThereNewer; //!< In library present newer object

    SdProjectItem();

    //Information
    //Unical ident of item accross world
    QString                getUid() const;
    QString                getExtendTitle() const;
    QString                getToolTip() const;
    QString                getAuthor() const { return mAuthor; }
    int                    getTime() const { return mCreateTime; }
    QString                getTitle() const { return mTitle; }
    SdProject             *getProject() const;
    SdUndo                *getUndo() const;
    virtual void           getHeader( SdLibraryHeader &hdr ) const;
    //Set information and editing status
    void                   setTitle(const QString title, const QString undoTitle );
    void                   setUnicalTitle( const QString undoTitle );
    //Get editEnable flag
    bool                   isEditEnable() const { return mEditEnable; }
    //Set editEnable flag. When needed - creation new object perhaps
    SdProjectItem         *setEditEnable(bool edit , const QString undoTitle);
    //Check if another author
    bool                   isAnotherAuthor() const;
    //Get over rect
    SdRect                 getOverRect( quint64 classMask = dctAll );

    //Object visual (graphical) identificator
    SdGraphIdent          *identGet();
    SdGraphIdent          *identCreate();

    //Object visual (graphical) value
    SdGraphValue          *valueGet();
    SdGraphValue          *valueCreate();


    //Origin point
    SdPoint                getOrigin() const { return mOrigin; }
    void                   setOrigin( const SdPoint org, SdUndo *undo );

    //Delete all item objects
    void                   clear();

    //Set layers usage
    virtual void           setLayerUsage();

    //Status creation flag for project.
    void                   setHand() { mAuto = false; }
    bool                   isAuto() const { return mAuto; }
    //Delete object if it is mAuto=true and no more reference to it
    void                   autoDelete( SdUndo *undo );

    virtual QString        getIconName() const = 0;
    virtual bool           is3dAllowed() const { return false; }
    virtual void           draw3d( QOpenGLFunctions_2_0 *f );
    virtual quint64        getAcceptedObjectsMask() const = 0;
    void                   insertObjects(SdPoint offset, SdSelector *sour, SdUndo *undo, SdSelector *dest, bool next );

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void           json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void           json( const SdJsonReader &js ) override;

    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void           cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //Return current registered author
    static  QString        getDefaultAuthor();

  private:
    //Set creation time as current
    void                   updateCreationTime();
    //Set author as current
    void                   updateAuthor();

  protected:
    //Find ident
    SdGraphIdent          *findIdent() const;

    //Find value
    SdGraphValue          *findValue() const;

  };

typedef SdProjectItem *SdProjectItemPtr;

#endif // SDPROJECTITEM_H
