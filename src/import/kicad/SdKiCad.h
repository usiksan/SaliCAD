#ifndef SDKICAD_H
#define SDKICAD_H

#include "SdScanerKiCad.h"


template<typename Class, auto Member>
void sdKiCad(SdScanerKiCad* scaner, Class* pad) {
  std::invoke(Member, pad).scan(scaner);
  }




struct SdKiCadValidable
  {
    bool mPresent = false; //!< Flag, true if this field present in file

    bool isValid() const { return mPresent; }
  };





struct SdKiCadDouble : public SdKiCadValidable
  {
    double mValue   = 0;     //!< Value

    void scan( SdScanerKiCad *scaner )
      {
      mValue = scaner->tokenNeedValueDouble();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadCoord : public SdKiCadValidable
  {
    int  mValue   = 0;     //!< Value

    void scan( SdScanerKiCad *scaner )
      {
      mValue = scaner->tokenNeedCoord();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadPoint : public SdKiCadValidable
  {
    QPointF mPoint;           //!< 2d point

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedValueDouble() );
      mPoint.setY( scaner->tokenNeedValueDouble() );
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadSdPoint : public SdKiCadValidable
  {
    QPoint mPoint; //!< 2d point

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedCoord() );
      mPoint.setY( scaner->tokenNeedCoord() );
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadPointWithAngle : public SdKiCadValidable
  {
    QPointF mPoint;           //!< 2d point
    double  mAngle   = 0;     //!< Angle of rotation

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedValueDouble() );
      mPoint.setY( scaner->tokenNeedValueDouble() );
      mAngle = scaner->tokenNeedValueDouble();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };





struct SdKiCadSdPointWithAngle : public SdKiCadValidable
  {
    QPoint mPoint;           //!< 2d point
    int    mAngle   = 0;     //!< Angle of rotation

    void scan( SdScanerKiCad *scaner )
      {
      mPoint.setX( scaner->tokenNeedCoord() );
      mPoint.setY( scaner->tokenNeedCoord() );
      mAngle = scaner->tokenNeedAngle();
      scaner->tokenNeedClose();
      mPresent = true;
      }
  };




struct SdKiCadString : public SdKiCadValidable
  {
    QString mValue;           //!< String value

    void scan( SdScanerKiCad *scaner )
      {
      mValue = scaner->tokenNeedValue('s');
      scaner->tokenNeed( ')' );
      mPresent = true;
      }

    QString layer() const;
  };




struct SdKiCadStringList : public SdKiCadValidable
  {
    QStringList mList;            //!< List of strings

    void scan( SdScanerKiCad *scaner )
      {
      while( scaner->token() != ')' )
        mList.append( scaner->tokenNeedValue('s') );
      scaner->tokenNeed( ')' );
      mPresent = true;
      }
  };




template <typename SdKiCad>
struct SdKiCadMap : public SdKiCadValidable
  {
    int  mSelector = -1;

    void scan( SdScanerKiCad *scaner )
      {
      QString str = scaner->tokenNeedValue('n');
      scaner->tokenNeed( ')' );
      mSelector =  SdKiCad::mTokenMap.value(str, mSelector);
      mPresent = true;
      }
  };





struct SdKiCadYesNo : public SdKiCadMap<SdKiCadYesNo>
  {
    inline static const QMap<QString,int> mTokenMap = {
      { "yes", 1 },
      { "no",  0 }
    };

    bool isYes() const { return mSelector == 1; }
  };


/*
struct SdKiCadMap
  {
    const QMap<QString,int> &mFindMap;
    int                      mSelector;
    bool                     mPresent = false; //!< Flag, true if this field present in file

    SdKiCadMap( const QMap<QString,int> &map, int fail = -1 ) : mFindMap(map), mSelector(fail) {}

    void scan( SdScanerKiCad *scaner )
      {
      QString str = scaner->tokenNeedValue('n');
      scaner->tokenNeed( ')' );
      mSelector = mFindMap.value(str, mSelector);
      mPresent = true;
      }
  };

*/



struct SdKiCadStrokeType : public SdKiCadMap<SdKiCadStrokeType>
  {
    enum {
      typeNone,
      typeSolid,
      typeDashed,
      typeDotted,
      typeDashDot
    };

    inline static const QMap<QString,int> mTokenMap = {
      { "none",     typeNone },
      { "solid",    typeSolid },
      { "dashed",   typeDashed },
      { "dotted",   typeDotted },
      { "dash_dot", typeDashDot }
    };
  };





template <typename SdKiCad >
struct SdKiCadGrouped
  {
    void scan(  SdScanerKiCad *scaner  )
      {
      scaner->parse( SdKiCad::tokenMap, static_cast<SdKiCad*>(this) );
      }
  };





struct SdKiCadStroke : public SdKiCadGrouped<SdKiCadStroke>
  {
    SdKiCadCoord      mWidth;
    SdKiCadStrokeType mType;

    inline static SdKiCadTokenMap<SdKiCadStroke> tokenMap = {
      { "width", sdKiCad<SdKiCadStroke,&SdKiCadStroke::mWidth> },
      { "type",  sdKiCad<SdKiCadStroke,&SdKiCadStroke::mType> }
    };

    bool isValid() const { return mWidth.isValid() && mType.isValid(); }
  };




struct SdKiCadLine : public SdKiCadGrouped<SdKiCadLine>
  {
    SdKiCadSdPoint mStart;
    SdKiCadSdPoint mEnd;
    SdKiCadStroke  mStroke;
    SdKiCadString  mLayer;

    inline static SdKiCadTokenMap<SdKiCadLine> tokenMap = {
      { "start",  sdKiCad<SdKiCadLine,&SdKiCadLine::mStart> },
      { "end",    sdKiCad<SdKiCadLine,&SdKiCadLine::mEnd> },
      { "stroke", sdKiCad<SdKiCadLine,&SdKiCadLine::mStroke> },
      { "layer",  sdKiCad<SdKiCadLine,&SdKiCadLine::mLayer> }
    };

    bool isValid() const { return mStart.isValid() && mEnd.isValid() && mStroke.isValid() && mLayer.isValid(); }
  };



struct SdKiCadCircle : public SdKiCadGrouped<SdKiCadCircle>
  {
    SdKiCadSdPoint mCenter;
    SdKiCadSdPoint mEnd;
    SdKiCadStroke  mStroke;
    SdKiCadString  mLayer;
    SdKiCadYesNo   mFilled;

    inline static SdKiCadTokenMap<SdKiCadCircle> tokenMap = {
      { "center", sdKiCad<SdKiCadCircle,&SdKiCadCircle::mCenter> },
      { "end",    sdKiCad<SdKiCadCircle,&SdKiCadCircle::mEnd> },
      { "stroke", sdKiCad<SdKiCadCircle,&SdKiCadCircle::mStroke> },
      { "layer",  sdKiCad<SdKiCadCircle,&SdKiCadCircle::mLayer> },
      { "fill",   sdKiCad<SdKiCadCircle,&SdKiCadCircle::mFilled> }
    };

    bool isValid() const { return mCenter.isValid() && mEnd.isValid() && mLayer.isValid() && (mFilled.isYes() || mStroke.isValid()); }
  };



struct SdKiCadZoneConnection : public SdKiCadMap<SdKiCadZoneConnection>
  {
    enum {
      padNone,
      padThermal,
      padSolid
    };

    inline static const QMap<QString,int> mTokenMap = {
        { "padNone",    padNone },
        { "padThermal", padThermal },
        { "padSolid",   padSolid }
      };
  };




struct SdKiCadPad : public SdKiCadGrouped<SdKiCadPad>
  {
    SdKiCadSdPointWithAngle mPosWithRotation;
    SdKiCadSdPoint          mSize;
    SdKiCadCoord            mDrill;
    SdKiCadStringList       mLayerList;
    SdKiCadDouble           mRoundRectRatio;
    SdKiCadCoord            mSolderMaskMargin;
    SdKiCadCoord            mSolderPasteMargin;
    SdKiCadDouble           mSolderPasteRatio;
    SdKiCadZoneConnection   mZoneConnection;

    inline static const SdKiCadTokenMap<SdKiCadPad> tokenMap = {
      { "at",                  sdKiCad<SdKiCadPad,&SdKiCadPad::mPosWithRotation> },
      { "size",                sdKiCad<SdKiCadPad,&SdKiCadPad::mSize> },
      { "drill",               sdKiCad<SdKiCadPad,&SdKiCadPad::mDrill> },
      { "layers",              sdKiCad<SdKiCadPad,&SdKiCadPad::mLayerList> },
      { "roundrect_rratio",    sdKiCad<SdKiCadPad,&SdKiCadPad::mRoundRectRatio> },
      { "solder_mask_margin",  sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderMaskMargin> },
      { "solder_paste_margin", sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderPasteMargin> },
      { "solder_paste_ratio",  sdKiCad<SdKiCadPad,&SdKiCadPad::mSolderPasteRatio> },
      { "zone_connection",     sdKiCad<SdKiCadPad,&SdKiCadPad::mZoneConnection> }
    };

  };


struct SdKiCadFont : public SdKiCadGrouped<SdKiCadFont>
  {
    SdKiCadSdPoint mSize;
    SdKiCadCoord   mThickness;

    inline static const SdKiCadTokenMap<SdKiCadFont> tokenMap = {
      { "size",      sdKiCad<SdKiCadFont,&SdKiCadFont::mSize> },
      { "thickness", sdKiCad<SdKiCadFont,&SdKiCadFont::mThickness> }
    };
  };


/*
struct SdKiCadTextEffects : public SdKiCadGrouped<SdKiCadTextEffects>
  {

  };


struct SdKiCadTextProp : public SdKiCadGrouped<SdKiCadTextProp>
  {
    SdKiCadSdPoint          mPos;
    SdKiCadString  mLayer;

  };

*/


void kicadFootprint( SdScanerKiCad *scaner, SdProject *project );
void kicadSymbolLib( SdScanerKiCad *scaner, SdProject *project );
void kicadPropertyPart( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadGraphLine( SdScanerKiCad *scaner, SdPItemPart *part );
void kicadGraphCircle( SdScanerKiCad *scaner, SdPItemPart *part );

void kicadFillPropLine( SdPropLine &prop, const SdKiCadStroke &stroke, const SdKiCadString &layer );
QString kicadLayerMap( const QString kicadLayerName );

#endif // SDKICAD_H
