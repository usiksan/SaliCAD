/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  STEP file reader and 3d model creator
*/
#ifndef SDSTEPREADER_H
#define SDSTEPREADER_H

#include "objects/Sd3dPoint.h"

#include <QString>
#include <QMap>
#include <QIODevice>
#include <functional>

class SdStepReader;


struct SdStepParser {
    virtual bool    parse( const QString &line, const SdStepReader &reader ) = 0;

    virtual QString name() const { return QStringLiteral("DefaultName"); }

    bool            matchName( const QString &line ) const { return line.startsWith( name() ); }
  };

using SdStepParserPtr = SdStepParser*;

using SdStepHandlerList = QList<SdStepParserPtr>;


class SdStepReader
  {
    QMap<QString,QString> mStepMap; //! Step file map (#id = contents)
  public:
    SdStepReader();

    bool read( QIODevice *in );

    QString     value( const QString &id ) const { return mStepMap.value(id); }

    QStringList styledItemList() const;


    QStringList parseList( const QString line ) const;

    QStringList parseListFromValue( const QString &id ) const;

    bool        parseParamList( const QString &line, QList<SdStepParserPtr> paramHandlerList ) const;
  private:
    QStringList itemList( const QString itemName ) const;
  };






struct SdStepParserParamList : public SdStepParser {
    QString               mItemName;
    SdStepHandlerList     mParamHandlerList;
    std::function<void()> mAppler;

    SdStepParserParamList( const QString itemName, SdStepHandlerList handlerList, std::function<void()> appler = [] () {} ) :
      mItemName(itemName),
      mParamHandlerList(handlerList),
      mAppler(appler){}

    // SdStepParser interface
  public:
    virtual bool    parse(const QString &line, const SdStepReader &reader ) override;
    virtual QString name() const override { return mItemName; }
  };




struct SdStepParserParamSkeeper : public SdStepParser {

    // SdStepParser interface
  public:
    virtual bool parse(const QString &line, const SdStepReader &reader) override;
  };




struct SdStepParserAsParamSingleId : public SdStepParser {
    SdStepHandlerList mReferenceList;

    SdStepParserAsParamSingleId( SdStepHandlerList referenceList ) : mReferenceList( referenceList ) {}


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line, const SdStepReader &reader) override;
  };




struct SdStepParserAsParamIdList : public SdStepParser {
    SdStepHandlerList mReferenceList;

    SdStepParserAsParamIdList( SdStepHandlerList referenceList ) : mReferenceList( referenceList ) {}

    // SdStepParser interface
  public:
    virtual bool parse(const QString &line, const SdStepReader &reader) override;
  };



struct SdStepParserAsParamString : public SdStepParser {
    QStringList mStrings;
    int         mSelected;

    SdStepParserAsParamString( QStringList strList ) : mStrings(strList) {}


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line, const SdStepReader &reader) override;
  };



struct SdStepParserAsParamDouble : public SdStepParser {
    double mValue;


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line, const SdStepReader &reader) override;
  };


struct SdStepParserColorRgb : public SdStepParser {
    quint32 mColor;


    // SdStepParser interface
  public:
    virtual bool    parse(const QString &line, const SdStepReader &reader ) override;
    virtual QString name() const override;
  };


struct SdStepParserCartesianPoint : public SdStepParser {
    double x,y,z;

    Sd3dPoint to3dPoint() const { return Sd3dPoint( x * 1000.0, y * 1000.0, z * 1000.0 ); }

    // SdStepParser interface
  public:
    virtual bool    parse(const QString &line, const SdStepReader &reader) override;
    virtual QString name() const override;
  };

struct SdStepParserDirection : public SdStepParserCartesianPoint {


    // SdStepParser interface
  public:
    virtual QString name() const override;
  };

#endif // SDSTEPREADER_H
