/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDSELECTOR_H
#define SDSELECTOR_H

#include "SdObject.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdGraph.h"

#include <QSet>
#include <functional>

//Состояние объектов, соответствеено "Невыделено" и "Выделено"
const int
  OUTSIDE_ELEM = 0, //Вне какого-либо объект
  UNSEL_ELEM   = 1, //Невыделенный объект
  SEL_ELEM     = 2; //Выделенный объект

typedef QSet<SdGraphPtr> SdGraphPtrTable;

class SdProject;

class SdSelector : public SdObject
  {
    SdPoint           mOrigin;        //Точка привязки селектора
    SdGraphPtrTable   mTable;         //Таблица выделенных объектов
    bool              mOwner;         //Является ли селектор собственником объектов
  public:
    SdSelector();
    ~SdSelector();

    void                setOrigin( SdPoint sour ) { mOrigin = sour; }
    SdPoint             getOrigin() const { return mOrigin; }
    bool                getOwner() const { return mOwner; }
    SdGraphPtrTable&    getTable() { return mTable; }

    virtual QString     getType() const { return QStringLiteral("Selector"); } //Тип объекта
    virtual quint64     getClass() const { return dctSelector; } //Получить класс объекта

    void                markDeleteAll(); //Удалить входящие в селектор объекты
    void                removeAll(); //Убрать все сслыки на объекты
    void                remove(SdGraph *graph);
    void                insert( SdGraph *graph );
    int                 count() const { return mTable.count(); }
    void                clear();
    void                operator = ( const SdSelector &sour );

    //Clipboard functions
    void                putToClipboard(const SdProject *project , double scale);
    SdProject          *getFromClipboard();

    virtual void        writeObject( QJsonObject &obj ) const; //Запись объекта в файл
    virtual void        readObject( SdObjectMap *map, const QJsonObject obj ); //Чтение объекта из файла

    //Итерация
    void                forEach( quint64 classMask, std::function<bool(SdGraph*)> fun1 );
    SdRect              getOverRect();
    void                draw( SdContext *ctx );

    static bool         isClipboardAvailable();
  };

#endif // SDSELECTOR_H
