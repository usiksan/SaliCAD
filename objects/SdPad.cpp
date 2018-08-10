/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Single pad description. On this description pads created on demand.
*/
#include "SdPad.h"
#include "SdContext.h"
#include "SdPItemPlate.h"
#include "SdUtil.h"
#include "SdEnvir.h"

#include <QJsonObject>

SdPad::SdPad() :
  mCenterX(0),          //Center of circle or rectangle
  mCenterY(0),
  mDiametrWidth(400),     //Diametr of circle or rectangle dimensions
  mHeight(-1),
  mMaskThreshold(100),    //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mStensilThreshold(40), //Stensil threshold. Calculation: stensilSize = size + mMaskThreshold
  mStensilRows(0),      //If rows or colons > 1 then stensil area divided on array of aperture with
  mStensilCols(0),      // width and height
  mStensilWidth(0),
  mStensilHeight(0),
  mHoleDiametr(0),
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle(true)
  {

  }

QJsonObject SdPad::write() const
  {
  QJsonObject obj;
  //Center of circle or rectangle
  obj.insert( QStringLiteral("cx"), mCenterX );
  obj.insert( QStringLiteral("cy"), mCenterY );
  //Diametr of circle or rectangle dimensions
  obj.insert( QStringLiteral("dw"), mDiametrWidth );
  obj.insert( QStringLiteral("h"), mIsCircle ? 0 : mHeight );
  //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  obj.insert( QStringLiteral("mt"), mMaskThreshold );
  //Stensil threshold. Calculation: stensilSize = size - mStensilThreshold
  obj.insert( QStringLiteral("st"), mStensilThreshold );
  //If rows or colons > 1 then stensil area divided on array of aperture with
  // width and height
  obj.insert( QStringLiteral("sr"), mStensilRows );
  obj.insert( QStringLiteral("sc"), mStensilCols );
  obj.insert( QStringLiteral("sw"), mStensilWidth );
  obj.insert( QStringLiteral("sh"), mStensilHeight );
  obj.insert( QStringLiteral("hole"), mHoleDiametr );
  return obj;
  }





void SdPad::read(const QJsonObject obj)
  {
  //Center of circle or rectangle
  mCenterX = obj.value( QStringLiteral("cx") ).toInt();
  mCenterY = obj.value( QStringLiteral("cy") ).toInt();
  //Diametr of circle or rectangle dimensions
  mDiametrWidth = obj.value( QStringLiteral("dw") ).toInt();
  mHeight = obj.value( QStringLiteral("h") ).toInt();
  mIsCircle = mHeight <= 0;
  //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mMaskThreshold = obj.value( QStringLiteral("mt") ).toInt();
  //Stensil threshold. Calculation: stensilSize = size - mStensilThreshold
  mStensilThreshold = obj.value( QStringLiteral("st") ).toInt();
  //If rows or colons > 1 then stensil area divided on array of aperture with
  // width and height
  mStensilRows = obj.value( QStringLiteral("sr") ).toInt();
  mStensilCols = obj.value( QStringLiteral("sc") ).toInt();
  mStensilWidth = obj.value( QStringLiteral("sw") ).toInt();
  mStensilHeight = obj.value( QStringLiteral("sh") ).toInt();
  mHoleDiametr = obj.value( QStringLiteral("hole") ).toInt();
  }



void SdPad::draw(SdContext *dcx, SdPoint p, int stratum) const
  {
  //Draw pad itself
  SdLayer *layer = sdEnvir->mCacheForPad.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
    dcx->setBrush( layer->color() );
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth >> 1 );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1), mDiametrWidth, mHeight) );
    }

  //Draw pad mask
  layer = sdEnvir->mCacheForMask.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
    dcx->setBrush( layer->color() );
    int w = mDiametrWidth + mMaskThreshold * 2;
    int h = mHeight + mMaskThreshold * 2;
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), w >> 1 );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1), p.y() + mCenterY - (h >> 1), w, h) );
    }

  //Draw stensil
  layer = sdEnvir->mCacheForStensil.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
    dcx->setBrush( layer->color() );
    int w = mDiametrWidth - mStensilThreshold * 2;
    int h = mHeight - mStensilThreshold * 2;
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), w >> 1 );
    else {
      if( mStensilRows > 1 || mStensilCols > 1 ) {
        //Draw array of apertures
        int rows = mStensilRows > 1 ? mStensilRows : 1;
        int cols = mStensilCols > 1 ? mStensilCols : 1;
        int cellx = mDiametrWidth / cols;
        int celly = mHeight / rows;
        for( int y = 0; y < rows; y++ )
          for( int x = 0; x < cols; x++ )
            dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1) + x * cellx,
                                   p.y() + mCenterY - (h >> 1) + y * celly, mStensilWidth, mStensilHeight) );
        }
      else
        //Single apreture
        dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1), p.y() + mCenterY - (h >> 1), w, h) );
      }
    }

  //Draw hole
  layer = sdEnvir->mCacheForHole.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
    dcx->setBrush( layer->color() );
    dcx->circleFill( p, mHoleDiametr >> 1 );
    }

  }





QPolygonF SdPad::polygon(SdPoint p, int addon ) const
  {
  if( mIsCircle )
    //Return over octagon
    return SdUtil::octagon( p.x() + mCenterX, p.y() + mCenterY, (mDiametrWidth >> 1) + addon );
  QPolygonF pgn;
  //Fill rect
  pgn << QPointF(p.x() + mCenterX - (mDiametrWidth >> 1) - addon, p.y() + mCenterY - (mHeight >> 1) - addon)
      << QPointF(p.x() + mCenterX + (mDiametrWidth >> 1) + addon, p.y() + mCenterY - (mHeight >> 1) - addon)
      << QPointF(p.x() + mCenterX + (mDiametrWidth >> 1) + addon, p.y() + mCenterY + (mHeight >> 1) + addon)
      << QPointF(p.x() + mCenterX - (mDiametrWidth >> 1) - addon, p.y() + mCenterY + (mHeight >> 1) + addon);
  return pgn;
  }





//Create textual pad description
QString SdPad::description() const
  {
  double ppm = sdEnvir->mPrtPPM;
  if( mIsCircle ) {
    if( mHoleDiametr <= 0 )
      //smd
      return QObject::tr("Planar circle diam pad%1 mask%2 stensil%3").arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth-mStensilThreshold,ppm) );
    else
      //throw (no stensil)
      return QObject::tr("Throw circle diam hole%3 pad%1 mask%2").arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHoleDiametr,ppm) );
    }
  else {
    if( mHoleDiametr <= 0 ) {
      //smd
      if( mStensilCols > 1 || mStensilRows > 1 )
        //Array of stensil apertures
        return QObject::tr("Planar rect pad%1x%2 mask%3x%4 stensil ar%5x%6 of%7x%8")
            .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
            .arg( SdUtil::log2physStr(mHeight,ppm))
            .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
            .arg( qMax(mStensilCols,1) )
            .arg( qMax(mStensilRows,1) )
            .arg( SdUtil::log2physStr(mDiametrWidth-mStensilThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight-mStensilThreshold,ppm) );
      else
        //Single stensil aperture
        return QObject::tr("Planar rect pad%1x%2 mask%3x%4 stensil%5x%6")
            .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
            .arg( SdUtil::log2physStr(mHeight,ppm))
            .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mDiametrWidth-mStensilThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight-mStensilThreshold,ppm) );

      }
    else {
      //throw (no stensil)
      //Single stensil aperture
      return QObject::tr("Throw rect hole%5 pad%1x%2 mask%3x%4")
          .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mHeight,ppm))
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHoleDiametr,ppm) );
      }
    }
  //return QString();
  }



void SdPad::clear()
  {
  mCenterX = 0;          //Center of circle or rectangle
  mCenterY = 0;
  mDiametrWidth = 400;     //Diametr of circle or rectangle dimensions
  mHeight = -1;
  mMaskThreshold = 100;    //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mStensilThreshold = 40; //Stensil threshold. Calculation: stensilSize = size + mMaskThreshold
  mStensilRows = 0;      //If rows or colons > 1 then stensil area divided on array of aperture with
  mStensilCols = 0;      // width and height
  mStensilWidth = 0;
  mStensilHeight = 0;
  mHoleDiametr = 0;
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle = true;
  }


