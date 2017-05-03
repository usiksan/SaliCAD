/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common mode for all with enter text
*/
#ifndef SDMODECTEXTUAL_H
#define SDMODECTEXTUAL_H

#include "SdModeCommon.h"
#include "objects/SdPropText.h"

class SdModeCTextual : public SdModeCommon
  {
  protected:
    SdPropText *mPropText;     //Text properties
    QString     mString;       //Work string
    int         mPos;          //Char cursor position
    int         mStartSel;     //Char start select position
    int         mStopSel;      //Char stop select position
    SdRect      mOverRect;     //Text over rect
    SdRect      mSelectRect;   //Selection region
    SdPoint     mCursorP1;     //Cursor position p1. Cursor is vertical line from p1 to p2 at insert position
    SdPoint     mCursorP2;     //Cursor position p2
    bool        mShift;
    bool        mControl;
  public:
    SdModeCTextual( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void keyDown(int key, QChar ch) override;
    virtual void keyUp(int key, QChar ch) override;
    virtual int  getCursor() const override;
    virtual int  getPropBarId() const override;
    virtual void propGetFromBar() override;
    virtual void propSetToBar() override;
    virtual void enterPoint( SdPoint enter ) override;      //Ввод точки (левая кнопка)
    virtual void cancelPoint( SdPoint p ) override;         //Точка прекращения (правая кнопка)
  protected:
    void drawText( SdContext *dc );

    //Move cursor (insert) posiotion
    void movePos( int pos, bool sel );

    //Define selection
    void setSel( int start, int stop );

    //Insert text in current position with selection param
    void insertText( const QString str, bool sel );

    //Clear old text and insert new one with selection param
    void setText( const QString str, bool sel );

    //Delete selection if present
    void delSelected();

    //Return selection presention
    bool isSelectionPresent() const { return mStartSel != mStopSel; }

    //Clipboard operations
    void clipboardCopy();
    void clipboradPaste();
    void clipboardCut();

    virtual void cancelEdit() {}
    virtual void applyEdit() {}

  };

#endif // SDMODECTEXTUAL_H
