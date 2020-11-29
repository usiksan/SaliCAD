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

class SdStepReader;


struct SdStepParser {
    SdStepReader *mReader;

    SdStepParser( SdStepReader *reader ) : mReader(reader) { }

    virtual bool    parse( const QString &line ) = 0;

    virtual QString name() const = 0;
  };

using SdStepParserPtr = SdStepParser*;


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


struct SdStepParserStyledItem : public SdStepParser {


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line) override;
    virtual QString name() const override;
  };


struct SdStepParserColorRgb : public SdStepParser {
    quint32 mColor;


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line) override;
    virtual QString name() const override;
  };


struct SdStepParserFillAreaStyleColor : public SdStepParser {


    // SdStepParser interface
  public:
    virtual bool parse(const QString &line) override;
    virtual QString name() const override;
  };


struct SdStepParser3dFace : public SdStepParser {
    int mFaceMask;
    quint32 mFaceColor;

    bool faceMask( const QString &param );
    bool faceColor( const QString &colorId );
    bool style( const QString &id );
  };

//struct SdStepParser3dFace {
//    QList<Sd3dPoint> mRegion;     //! Face region
//    Sd3dPoint        mNormal;     //! Face normal vector for foregraund and background side detection
//    quint32          mFaceColor;  //! Face color

//  };

#endif // SDSTEPREADER_H
