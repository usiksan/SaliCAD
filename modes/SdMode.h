/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for operation on objects.
*/

#ifndef SDMODE_H
#define SDMODE_H

#include "objects/SdProjectItem.h"
#include "objects/SdContext.h"

#include <QCursor>

class SdWEditorGraph;

class SdMode
  {
  protected:
    SdProjectItem  *mObject; //Object is mode working on
    SdWEditorGraph *mEditor; //Editor window is mode working in
    int             mStep;   //Mode step

//    int    GetStep() const { return step; }
//virtual void   SetStep( int stp );

////Информационные
//DViewer*       GetViewer() const { return viewer; }
//DGraphTopPic*  GetBase() const { return object; }
//PDUndo         GetUndo() const { return undo; }

   //Исполняемые через обозреватель
   void           update();          //Обновить изображение
   //void           UpdateTools() { viewer->UpdateTools(); }//Обновить панель инструментов
   //void           setMessage( const QString msg ) { GetViewer()->SetMessage( msg ); }   //Установить сообщение в строку состояния
   //void           SetPropBar( int id, DProp *prop );      //Установить панель свойств
   //DProp&         GetGlobalProp() { return viewer->GetGlobalProp(); }          //Получить глобальные свойства
   //DEnvir&        GetEnvir() { return viewer->GetEnvir(); }                    //Получить глобальные установки
   //void           SetCursor( int cursorId ) { viewer->SetCursor( cursorId ); } //Установить курсор
   //DPoint         GetGrid() { return viewer->GetGrid(); }                      //Получить сетку
   //void           CancelMode() { GetViewer()->CancelMode(); }                  //Прекращение режима
   //void           SetDirty() { GetViewer()->SetDirty(); }                      //Объявить объект редактированным

   //Сервисные
   //void           AddPic( PDBasePic pic );                     //Добавить объект к контейнеру

  public:
    SdMode( SdWEditorGraph *editor, SdProjectItem *obj );
    virtual ~SdMode();

    virtual void    activate();               //Вызывается первой, после того как режим делается текущим
    virtual void    reset();                  //Сбрасывает режим в исходное состояние

    virtual void    draw( SdContext* );        //Рисует картинку через призму режима
    virtual void    show( SdContext& );        //Рисует временные данные режима
    virtual void    hide( SdContext& );        //Стирает временные данные режима

    virtual void    enterPoint( SdPoint, SdContext& ) {}    //Ввод точки (левая кнопка)
    virtual void    clickPoint( SdPoint, SdContext& ) {}    //Двойное нажатие левой кнопки
    virtual void    cancelPoint( SdPoint, SdContext& ) {}   //Точка прекращения (правая кнопка)
    virtual void    movePoint( SdPoint, SdContext& ) {}     //Перемещение точки
    virtual bool    keyDown( int, SdContext& );             //Нажатие кнопки
    virtual bool    keyUp( int, SdContext&);                //Отпускание клавиши
    virtual void    propChanged( SdContext& ) {}            //Извещение об изменении свойств
    virtual void    enterPrev( SdContext& ) {}              //Повторение ввода предыдущего сеанса (разумный ввод)
    virtual void    beginDrag( SdPoint, SdContext& ) {}     //Начало перетаскивания
    virtual void    dragPoint( SdPoint, SdContext& ) {}     //Перетаскивание
    virtual void    stopDrag( SdPoint, SdContext& ) {}      //Конец перетаскивания
    virtual bool    enableCopy() const;                     //Режим имеет объекты для копирования
    virtual bool    enablePaste( quint64 pasteMask ) const; //Режим разрешает вставку объектов с заданной маской
    virtual bool    getInfo( SdPoint p, QString &info );    //Получить всплывающую информацию в заданной точке

    virtual QString getStepHelp() const = 0;
    virtual QString getModeHelp() const = 0;
    virtual QString getStepThema() const = 0;
    virtual QCursor getCursor() const = 0;
    virtual int     getIndex() const = 0;

  };

#endif // SDMODE_H
