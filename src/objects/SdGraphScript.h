/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object with script. Script used for in schematic calculations.
  It represents math script language like python or subset of C

  Example of script:
  #This is remark. All symbols after # sing - are ignored
  R1 = ref( "Om", "" )
  R2 = ref( "Om", "" )
  Rcom = ref( "Om", "" )
  Rcom = R1 + R2

  This script calculate common resistance of continuosly connected resistors.
  ref function used to declare visibility and editing possibility of variable.
  In schematic such script will be displayed as:

  R1 = 10kOm
  R2 = 10kOm
  Rcom = 20kOm

  Values 10kOm are editable, i.e. they can be edited by text mode. When they changed
  script automatically recalculated and display correct result.

  Object SdGraphScript display only schematic representation of script programm.
  Source code of script accessed throught SdDExpressionEdit dialog.
*/
#ifndef SDGRAPHSCRIPT_H
#define SDGRAPHSCRIPT_H

#include "SdGraphParam.h"
#include "SdGraphScriptRefMap.h"
#include "script/SdScriptProgramm.h"

#define SD_TYPE_SCRIPT "Script"

class SdGraphScript : public SdGraphParam
  {
    //This fields are saved when project save to file
    SdGraphScriptRefMap mRefMap;   //!< Script visual referenced variables
    QString             mScript;   //!< Script source code
    SdPoint             mOrigin;   //!< Origin point of script visual representation
    SdPropText          mProp;     //!< Text properties with which will be displayed script visual representation
    SdRect              mOverRect; //!< Full rectangle which fill script visual representation

    //This fields are remain unsaved and recalculated when script object loaded
    QString             mError;    //!< Error of script parsing. If no error then this string will be empty
    SdScriptProgrammPtr mProgramm; //!< Programm which builded from script parsing
    bool                mNeedLink;
  public:
    //!
    //! \brief SdGraphScript Default constructor
    //!
    SdGraphScript();

    //!
    //! \brief SdGraphScript Creation constructor. Builds graph script object with params
    //! \param script        Script source code
    //! \param org           Script origin position
    //! \param prp           Script visual representation text properties
    //!
    SdGraphScript( const QString &script, SdPoint org, const SdPropText &prp );


    //!
    //! \brief scriptGet Returns current script source code
    //! \return          Current script source code
    //!
    QString scriptGet() const { return mScript; }

    //!
    //! \brief scriptSet Sets new script source code. Perform script parsing and programm building
    //! \param scr       New script source code
    //! \param undo      Undo operation
    //!
    void    scriptSet( const QString &scr, SdUndo *undo );

    //!
    //! \brief parse Perform parsing current source code and build programm
    //!
    void    parse();

    //!
    //! \brief calculate Perform programm execution and new variable values calculation
    //!
    void    calculate();

    //!
    //! \brief tryLink  Used SdCModeScript to link variable to other param object
    //! \param p        Point of cursor
    //! \param overRect Over rectangle which receiv selected variable name over rectangle
    //! \return         Index selected variable or -1 if no variable in point p
    //!
    int     tryLink( SdPoint p, SdRect &overRect ) const;

    void    link(int index, SdContainer *owner, SdGraphParam *ref, const QString paramName );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select( SdSelector *selector ) override;
    virtual void    setLayerUsage() override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    //!
    //! \brief drawExceptText Draws object in 2d space with exception textual component with textId
    //! \param dc             Display context where drawing doing
    //! \param textId         Text id which must not be drawn
    //!
    virtual void    drawExceptText( SdContext *dc, int textId ) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual int     behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;

  private:
    void ajustProp();
  };

#endif // SDGRAPHSCRIPT_H
