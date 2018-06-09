#include "SdPad.h"
#include "SdContext.h"
#include "SdPItemPlate.h"
#include "SdUtil.h"
#include "SdEnvir.h"

#include <QJsonObject>

SdPad::SdPad()
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
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth >> 1 );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1), mDiametrWidth, mHeight) );
    }

  //Draw pad mask
  layer = sdEnvir->mCacheForMask.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
    int w = mDiametrWidth + mMaskThreshold * 2;
    int h = mHeight + mMaskThreshold * 2;
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), w >> 1 );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1), p.y() + mCenterY - (h >> 1), w, h) );
    }

  //Draw stensil
  layer = sdEnvir->mCacheForStensil.getLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    dcx->setPen( 0, layer, 0 );
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
    dcx->circleFill( p, mHoleDiametr >> 1 );
    }

  }





QPolygon SdPad::polygon(SdPoint p)
  {
  QPolygon pgn;
  if( mIsCircle ) {
    //Return over octagon
    pgn = SdUtil::octagon( p.x() + mCenterX, p.y() + mCenterY, mDiametrWidth >> 1 );
    }
  else {
    //Fill rect
    pgn << SdPoint(p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1))
        << SdPoint(p.x() + mCenterX + (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1))
        << SdPoint(p.x() + mCenterX + (mDiametrWidth >> 1), p.y() + mCenterY + (mHeight >> 1))
        << SdPoint(p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY + (mHeight >> 1));
    }
  return pgn;
  }


