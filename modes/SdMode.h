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
#include "SdCursorFactory.h"
#include "SdModeIdents.h"

#include <QChar>
#include <QTransform>
#include <QImage>


class SdWEditorGraph;

class SdMode
  {
    int             mStep;       //Mode step
  protected:
    SdProjectItem  *mObject;     //Object is mode working on
    SdWEditorGraph *mEditor;     //Editor window is mode working in
    SdUndo         *mUndo;       //Undo for this project

////Информационные
//DViewer*       GetViewer() const { return viewer; }
//DGraphTopPic*  GetBase() const { return object; }
//PDUndo         GetUndo() const { return undo; }

   //Исполняемые через обозреватель
   void           update();          //Обновить изображение
   void           cancelMode();      //Прекращение режима
   SdPoint        getGrid() const;
   double         getPPM() const;

   //Сервисные
   void           setStep( int stp );
   int            getStep() const { return mStep; }
   void           setDirty();                      //Объявить объект редактированным
   void           setDirtyCashe();

   //Сервисные
   void           addPic(SdObject *obj , QString title);         //Добавить объект к контейнеру

  public:
    SdMode( SdWEditorGraph *editor, SdProjectItem *obj );
    virtual ~SdMode();

            void    restore();                              //Обновляет все, что касается текущего состояния режима

    virtual void    activate();                             //Вызывается первой, после того как режим делается текущим
    virtual void    reset();                                //Сбрасывает режим в исходное состояние

    virtual void    drawStatic( SdContext *ctx );           //Ресует постоянную часть картинки
    virtual void    drawDynamic( SdContext *ctx );          //Рисует переменную часть картинки

    virtual int     getPropBarId() const { return -1; }     //Получить идентификатор панели свойств
    virtual void    propGetFromBar() {}                     //Извещение об изменении свойств
    virtual void    propSetToBar() {}                       //Установить свойства в активный бар

    virtual void    enterPoint( SdPoint ) {}                //Ввод точки (левая кнопка)
    virtual void    clickPoint( SdPoint ) {}                //Двойное нажатие левой кнопки
    virtual void    cancelPoint( SdPoint ) {}               //Точка прекращения (правая кнопка)
    virtual void    movePoint( SdPoint ) {}                 //Перемещение точки
    virtual bool    wheel( SdPoint ) { return false; }      //Вращение колеса мыши
    virtual void    keyDown( int key, QChar ch );           //Нажатие кнопки
    virtual void    keyUp( int key, QChar ch );             //Отпускание клавиши
    virtual void    enterPrev() {}                          //Повторение ввода предыдущего сеанса (разумный ввод)
    virtual void    beginDrag( SdPoint ) {}                 //Начало перетаскивания
    virtual void    dragPoint( SdPoint ) {}                 //Перетаскивание
    virtual void    stopDrag( SdPoint ) {}                  //Конец перетаскивания

    virtual bool    enableCopy() const;                     //Режим имеет объекты для копирования
    virtual bool    enablePaste( quint64 pasteMask ) const; //Режим разрешает вставку объектов с заданной маской

    virtual bool    getInfo( SdPoint p, QString &info );    //Получить всплывающую информацию в заданной точке

    virtual QString getStepHelp() const = 0;                //Краткая помощь по текущему шагу режима
    virtual QString getModeThema() const = 0;               //Help page address Адрес страницы помощи
    virtual QString getStepThema() const = 0;               //Help page address and link for current step Адрес страницы помощи для текущего шага
    virtual int     getCursor() const = 0;                  //Номер курсора для режима
    virtual int     getIndex() const = 0;                   //Номер режима в списке режимов

  };

#endif // SDMODE_H
