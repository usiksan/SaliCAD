/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implement placement mode
*/
#ifndef SDMODECPARTPLACE_H
#define SDMODECPARTPLACE_H

#include "SdModeCommon.h"
#include "objects/SdContainer.h"
#include "objects/SdPropSelected.h"

//==============================================================================
//--------------------- Режимы размещения --------------------------------------
enum {             //Шаги режима размещения
  psmNoSelect,     //Нет выделения
  psmBeingSelRect, //Происходит выделение прямоугольником
  psmMove,         //Перенос одного или нескольких элементов
  psmLast };

enum DPlaceModes {   //Способ выбора следующего компонента для размещения
  dpmSmart,          //Разумный выбор из группы
  dpmNextNumber,     //Следующий номер в классе элементов
  dpmDialog,         //С помощью диалога
  dpmSheet };        //Указанием в схеме

enum DPlaceEndCode { //Коды завершения диалога выбора компонентов
  placeCancel,       //Прекратить режим
  placeSelect,       //Перейти к выбору компонента заданным способом
  placeComp,         //Размещать компонент
  placeGroup,        //Размещать группу компонентов по шаблону
  placeSheet };      //Выбрать элементы для размещения из схемы

//struct DPlaceModeInfo {
//  //int             placeMode;  //Режим размещения
//  DString          prev;       //Предыдущий компонент
//  DName            shem;       //Предыдущая схема
//  DString          patSour;    //Список идентификаторов группы компонентов
//  DString          patDest;    //
//  DCompPlaceTable  table;      //Таблица компонентов
//  DPlatePic       *plate;      //Обрабатываемая плата
//  DWName           sheetName;  //Название схемы, из которой осуществляется выбор

//  DPlaceModeInfo() : table( 30, 30 ), plate(0) { }

//  void FindGroupBySour();
//  bool CheckMatch( int level, int maxLevel, int sour[], int target[] );
//  };


class SdModeCPartPlace : public SdModeCommon
  {
    SdPoint            mFirst;             //Начальная точка
    SdPoint            mPrevMove;          //Предыдущая точка
    SdPropSelected     mLocalProp;         //Свойства выделенных объектов
    SdSelector         mFragment;          //Набор выделенных объектов
    SdObjectPtrList    mBehindCursorTable; //Список объектов под курсором
    int                mBehindCursorIndex; //Индекс объекта среди объектов под курсором
    bool               mShiftKey;
    QString            mSmartName;         //Компонент, подлежащий размещению разумным режимом
    SdGraphPartImp*    mBehindCursorPrt;   //Компонент, подлежащий выделению при нажатии левой кнопки мыши
    bool               mInsertFlag;        //Автораздвижка компонентов
    int                mBigCompPins;       //Количество выводов главного компонента
    int                mBigCompIndex;      //Индекс главного компонента в таблице компонентов
    //DLineProp         lineProp;          //Свойства для выделения прямоугольником
    bool               mBySheet;           //Истина, когда производится выбор из листа
  public:
    SdModeCPartPlace( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void activate() override;
    virtual void drawStatic(SdContext *ctx) override;
    virtual void drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual void    keyUp(int key, QChar ch) override;
    virtual SdPoint enterPrev() override;
    virtual void    beginDrag(SdPoint p) override;
    virtual void    dragPoint(SdPoint p) override;
    virtual void    stopDrag(SdPoint p) override;
    virtual bool getInfo(SdPoint p, QString &info) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    //Remove selection of components
    void unselect();

    //Set properties to component
    void setPropertiesToComponent();

    //Save state of selected objects
    void saveStateOfSelectedObjects( const QString undoTitle );

    //Perform placing
    void place();

    //Check components behind point
    void checkPoint( SdPoint p );

    //Prepare next component to select
    void nextComponent();
  };

#endif // SDMODECPARTPLACE_H
