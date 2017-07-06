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

class SdModeSelect : public SdMode
  {
  public:
    SdModeSelect( SdWEditorGraph *editor, SdProjectItem *obj );
  };

#if 0
class DSelectMode : public DMode {
  protected:
    DProp          localProp;  //Свойства выделенных объектов
    DSelectorPic   fragment;   //Набор выделенных объектов
    PDSheetNetTable netFragment;//Набор выделенных цепей
    DPoint         first;      //Начальная точка
    DPoint         prevMove;   //Предыдущая точка
    DPoint         curPoint;   //Текущая точка
    int            state;      //Состояние курсора
    bool           shift;      //Нажата клавиша Shift
    bool           control;    //Нажата клавиша Control

    DSelectorPic  *paste;      //Врагмент для вставки (копирования)
    DProjectPic   *pastePrj;   //Проект, из которого производится вставка

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
    virtual int  GetIndex() const { return IDM_SELECT; }
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
            void Copy();                 //Копировать в карман
            void Paste();                //Вставить из кармана
            void SelectAll();            //Выделить все
            void Unselect( bool update); //Убрать выделение
            int  CheckPoint( DPoint p ); //Проверить объект под точкой
            void BeginCopy( DPoint p );  //Начало копирования
            void DragCopy( DPoint p, DContext& );   //Процесс копирования
            void StopCopy( DPoint p, DContext& );   //Завершение копирования
            void BeginMove( DPoint p );  //Начало переноса
            void DragMove( DPoint p, DContext& );   //Процесс переноса
            void StopMove( DPoint p, DContext& );   //Завершение переноса
            void BeginRect( DPoint p, DContext& );  //Начало выделения прямоугольником
            void DragRect( DPoint p, DContext& );   //Процесс выделения прямоугольником
            void StopRect( DPoint p );   //Завершение выделения прямоугольником
            void GetProp();              //Получить свойства выделенных объектов
            void DeleteSelected();       //Удалить все выделенные объекты
            void DrawCopy( DContext &dc );   //Рисование копии объектов из paste
            void DrawDefault( DContext &dc );//Рисовать режим по умолчанию
            void EnterPaste( DPoint point ); //Вставка фрагмента
            void CancelPaste();              //Отмена вставки фрагмента
            void MovePaste( DPoint p );      //Перемещение вставляемого фрагмента
            void ShowRect( DContext& );
            void ShowSelect( DContext& );
            void ShowPaste( DContext& );
            void DrawUnselected( DContext& );
            void InsertCopy( DPoint offset, bool next );//Вставить копии объектов
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
