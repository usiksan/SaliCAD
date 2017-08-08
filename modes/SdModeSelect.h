/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Complex mode for selection, edit, move many graph objects
*/
#ifndef SDMODESELECT_H
#define SDMODESELECT_H

#include "SdMode.h"
#include "objects/SdProp.h"
#include "objects/SdSelector.h"
#include "objects/SdProject.h"

class SdModeSelect : public SdMode
  {
    SdPropSelected  mLocalProp;  //Свойства выделенных объектов
    SdSelector      mFragment;   //Набор выделенных объектов
    //PDSheetNetTable netFragment;//Набор выделенных цепей
    SdPoint         mFirst;      //Начальная точка
    SdPoint         mPrevMove;   //Предыдущая точка
    SdPoint         mCurPoint;   //Текущая точка
    int             mState;      //Состояние курсора
    bool            mShift;      //Нажата клавиша Shift
    bool            mControl;    //Нажата клавиша Control

    SdSelector      mPaste;      //Врагмент для вставки (копирования)
    SdProject      *mPastePrj;   //Проект, из которого производится вставка

    enum {
      smNoSelect,      //Нет выделения
      smSelRect,       //Происходит выделение прямоугольником
      smSelPresent,    //Есть выделение
      smCopy,          //Копирование
      smMove,          //Перенос
      smRotateBase,    //Указание базы для поворота
      smRotateDir,     //Указание направления поворота
      smPaste,         //Вставка из карамана
      smLast };

  public:
    SdModeSelect( SdWEditorGraph *editor, SdProjectItem *obj );
    ~SdModeSelect();

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    reset() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint point) override;
    virtual void    clickPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint point) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void wheel(SdPoint) override;
    virtual void    beginDrag(SdPoint point) override;
    virtual void    dragPoint(SdPoint point) override;
    virtual void    stopDrag(SdPoint point) override;
    virtual bool    enableCopy() const override;
    virtual bool    enablePaste(quint64 pasteMask) const override;
    virtual bool getInfo(SdPoint p, QString &info) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual void    keyUp(int key, QChar ch) override;


    void copy();                  //Copy selection to clipboard
    void cut();                   //Cut selection to clipboard
    void paste();                 //Insert from clipboard
    void selectAll();             //Select all [Выделить все]
    void deleteSelected();        //Delete all selected objects [Удалить все выделенные объекты]
    void unselect( bool update);  //Remove selection [Убрать выделение]
  protected:
    int  checkPoint( SdPoint p ); //Check object behind point [Проверить объект под точкой]
    void beginCopy( SdPoint p );  //Begin copy process [Начало копирования]
    void dragCopy( SdPoint p );   //Copy process [Процесс копирования]
    void stopCopy( SdPoint p );   //Copy complete [Завершение копирования]
    void beginMove( SdPoint p );  //Moving prepare
    void dragMove( SdPoint p );   //Moving process
    void stopMove( SdPoint p );   //Moving complete
    void beginRect( SdPoint p );  //Begin selection by rect
    void dragRect( SdPoint p );   //Selection by rect - selection process
    void stopRect( SdPoint p );   //Selection by rect - selection complete, accumulate selected elements
    void enterPaste( SdPoint point ); //Вставка фрагмента
    void cancelPaste();           //Cancel paste [Отмена вставки фрагмента]
    void showRect( SdContext *ctx );
    void insertCopy( SdPoint offset, bool next );//Вставить копии объектов
    void activateMenu();

  };

#if 0
class DSelectMode : public DMode {
  protected:

    virtual CPChar GetStepHelp() const;
    virtual CPChar GetStepThema() const;
    virtual int    GetCursor() const;
    virtual CPChar GetModeHelp() const { return "MSelect"; }
  public:
    DSelectMode( DViewer *viewer, DGraphTopPic *obj );
    ~DSelectMode();

    DSelectorPic* GetSelector() { return &fragment; }

    virtual void Activate();   //Вызывается первой, после того как режим делается текущим
    virtual void Reset();                  //Сбрасывает режим в исходное состояние
    virtual void Draw( DContext& );
    virtual void EnterPoint( DPoint, DContext& );
    virtual void ClickPoint( DPoint, DContext& );   //Двойное нажатие левой кнопки
    virtual void CancelPoint( DPoint, DContext& );
    virtual void MovePoint( DPoint, DContext& );
    virtual void BeginDrag( DPoint, DContext& );
    virtual void DragPoint( DPoint, DContext& );
    virtual void StopDrag( DPoint, DContext& );
    virtual bool KeyDown( int, DContext& );
    virtual bool KeyUp( int, DContext& );
    virtual void PropChanged( DContext& );         //Извещение об изменении свойств
            bool IsSelectPresent() { return fragment.GetNumber() != 0; }
    virtual bool EnableCopy() const { return fragment.GetNumber() != 0; }
    virtual bool EnablePaste( int pasteMask ) const;
  protected:
    virtual DBaseIterator<DGraphObjectPic>* GetCopyIterator( DSelectorPic &tbl, DPoint offset, bool next ) = 0;
    virtual void                            CopyClipboard() = 0;
    virtual bool                            PasteClipboard() = 0;
    virtual void                            ActivateMenu() = 0;
    virtual void                            Dublicate() = 0;
    virtual void                            Param() = 0;
    virtual void                            Numerate() = 0;
  };

class WinSelectMode : public DSelectMode {
    WGraphWindow *window;
  public:
    WinSelectMode( WGraphWindow *viewer, DGraphTopPic *obj ) : DSelectMode( viewer, obj ), window(viewer) {}
    ~WinSelectMode() {}

  protected:
            WGraphWindow*                   GetWindow() const { return window; }
    virtual DBaseIterator<DGraphObjectPic>* GetCopyIterator( DSelectorPic &tbl, DPoint offset, bool next );
    virtual void                            CopyClipboard();
    virtual bool                            PasteClipboard();
    virtual void                            ActivateMenu();
    virtual void                            Dublicate();
    virtual void                            Param();
    virtual void                            Numerate();
  };

#endif
#endif // SDMODESELECT_H
