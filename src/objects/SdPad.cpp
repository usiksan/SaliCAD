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
#include "Sd3dDraw.h"

#include <QJsonObject>
#include <math.h>

SdPad::SdPad() :
  mCenterX(0),           //Center of circle or rectangle
  mCenterY(0),
  mDiametrWidth(400),    //Diametr of circle or rectangle dimensions
  mHeight(-1),
  mMaskThreshold(100),   //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mStencilThreshold(40), //Stencil threshold. Calculation: stencilSize = size + mMaskThreshold
  mStencilRows(0),       //If rows or colons > 1 then stencil area divided on array of aperture with
  mStencilCols(0),       // width and height
  mStencilWidth(0),
  mStencilHeight(0),
  mHoleDiametr(0),
  mHoleLength(0),
  mSlotAngle(0),
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle(true)
  {

  }




QJsonObject SdPad::write() const
  {
  QJsonObject obj;
  //Center of circle or rectangle
  if( mCenterX ) obj.insert( QStringLiteral("cx"), mCenterX );
  if( mCenterY ) obj.insert( QStringLiteral("cy"), mCenterY );
  //Diametr of circle or rectangle dimensions
  obj.insert( QStringLiteral("dw"), mDiametrWidth );
  obj.insert( QStringLiteral("h"), mIsCircle ? 0 : mHeight );
  //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  obj.insert( QStringLiteral("mt"), mMaskThreshold );
  //Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
  obj.insert( QStringLiteral("st"), mStencilThreshold );
  //If rows or colons > 1 then stencil area divided on array of aperture with
  // width and height
  if( mStencilRows ) obj.insert( QStringLiteral("sr"), mStencilRows );
  if( mStencilCols ) obj.insert( QStringLiteral("sc"), mStencilCols );
  if( mStencilWidth) obj.insert( QStringLiteral("sw"), mStencilWidth );
  if( mStencilHeight ) obj.insert( QStringLiteral("sh"), mStencilHeight );
  if( mHoleDiametr ) obj.insert( QStringLiteral("hole"), mHoleDiametr );
  if( mHoleLength ) obj.insert( QStringLiteral("slotLen"), mHoleLength );
  if( mSlotAngle ) obj.insert( QStringLiteral("slotAng"), mSlotAngle );
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
  //Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
  mStencilThreshold = obj.value( QStringLiteral("st") ).toInt();
  //If rows or colons > 1 then stencil area divided on array of aperture with
  // width and height
  mStencilRows = obj.value( QStringLiteral("sr") ).toInt();
  mStencilCols = obj.value( QStringLiteral("sc") ).toInt();
  mStencilWidth = obj.value( QStringLiteral("sw") ).toInt();
  mStencilHeight = obj.value( QStringLiteral("sh") ).toInt();
  mHoleDiametr = obj.value( QStringLiteral("hole") ).toInt();
  mHoleLength = obj.value( QStringLiteral("slotLen") ).toInt();
  mSlotAngle = obj.value( QStringLiteral("slotAng") ).toInt();
  }





void SdPad::draw(SdContext *dcx, SdPoint p, int stratum) const
  {
  //If no stratum then no draw
  if( stratum == 0 )
    return;

  //Draw pad mask
  SdLayer *layer = sdEnvir->mCacheForMask.getVisibleLayer(stratum);
  if( layer != nullptr && layer->isVisible() && mMaskThreshold > 0 ) {
    int w = mDiametrWidth + mMaskThreshold * 2;
    int h = mHeight + mMaskThreshold * 2;
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), w >> 1, layer->color() );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1), p.y() + mCenterY - (h >> 1), w, h), layer->color() );
    }

  //Draw stencil
  layer = sdEnvir->mCacheForStencil.getVisibleLayer(stratum);
  if( layer != nullptr && layer->isVisible() && mStencilThreshold > 0 ) {
    int w = mDiametrWidth - mStencilThreshold * 2;
    int h = mHeight - mStencilThreshold * 2;
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), w >> 1, layer->color() );
    else {
      if( mStencilRows > 1 || mStencilCols > 1 ) {
        //Draw array of apertures
        int rows = mStencilRows > 1 ? mStencilRows : 1;
        int cols = mStencilCols > 1 ? mStencilCols : 1;
        int cellx = mDiametrWidth / cols;
        int celly = mHeight / rows;
        int offx = (cellx - mStencilWidth) / 2;
        int offy = (celly - mStencilHeight) / 2;
        for( int y = 0; y < rows; y++ )
          for( int x = 0; x < cols; x++ )
            dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1) + x * cellx + offx,
                                   p.y() + mCenterY - (h >> 1) + y * celly + offy, mStencilWidth, mStencilHeight), layer->color() );
        }
      else
        //Single apreture
        dcx->fillRect( SdRect( p.x() + mCenterX - (w >> 1), p.y() + mCenterY - (h >> 1), w, h), layer->color() );
      }
    }

  //Draw pad itself
  layer = sdEnvir->mCacheForPad.getVisibleLayer(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    if( mIsCircle ) dcx->circleFill( SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth >> 1, layer->color() );
    else dcx->fillRect( SdRect( p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1), mDiametrWidth, mHeight), layer->color() );
    }

  //Draw hole
  layer = sdEnvir->mCacheForHole.getVisibleLayer(stratum);
  if( layer != nullptr && layer->isVisible() && mHoleDiametr > 0 ) {
    if( mHoleLength > 0 ) {
      //Hole is slot type
      SdPoint a,b;
      slotPoints( a, b );
      a.move(p);
      b.move(p);
      dcx->setPen( mHoleDiametr, layer, dltSolid );
      dcx->line( a, b );
      }
    else
      //Hole is circle type
      dcx->circleFill( p, mHoleDiametr >> 1, layer->color() );
    }

  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//! \param p      Point of center of pad
//!
void SdPad::draw3d(QOpenGLFunctions_2_0 *f, SdPoint p) const
  {
  //3d drawing mach different for hole or without hole
  if( mHoleDiametr > 0 ) {
    //Hole present
    SdLayer *layer = sdEnvir->mCacheForPad.getVisibleLayer(stmThrough);
    if( layer != nullptr ) {
      QColor padColor = layer->color();
      layer = sdEnvir->mCacheForHole.getVisibleLayer(stmThrough);
      if( layer != nullptr ) {
        QColor holeColor = layer->color();
        if( mIsCircle )
          Sd3dDraw::padCircle( f, SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth >> 1, padColor, p, mHoleDiametr, mHoleLength, holeColor, -0.01 );
        else
          Sd3dDraw::padRect( f, SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth, mHeight, padColor, p, mHoleDiametr, mHoleLength, holeColor, -0.01 );
        }

      }
    }
  else {
    //Smd pad
    SdLayer *layer = sdEnvir->mCacheForPad.getVisibleLayer(stmThrough);
    if( layer != nullptr ) {
      Sd3dDraw::color( f, layer->color() );
      if( mIsCircle )
        Sd3dDraw::circleFill( f, SdPoint( p.x() + mCenterX, p.y() + mCenterY), mDiametrWidth >> 1, -0.01 );
      else {
        SdPoint a(p.x() + mCenterX - (mDiametrWidth >> 1), p.y() + mCenterY - (mHeight >> 1));
        SdPoint b(a.x() + mDiametrWidth, a.y() + mHeight);
        Sd3dDraw::rectFilled( f, a, b, -0.01 );
        }
      }
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




void SdPad::appendWindow(SdPolyWindowList &dest, SdPoint p, int gap, const QTransform &t) const
  {
  if( mIsCircle ) {
    //Append circle pad
    SdPoint c( t.map(QPoint(p.x() + mCenterX,p.y() + mCenterY)) );
    dest.appendCircle( c, (mDiametrWidth >> 1) + gap );
    }
  else {
    //Append as rectangle
    dest.appendRegion( t.map(polygon(p, gap)) );
    }
  }





//Return over pad circle radius
int SdPad::overCircleRadius() const
  {
  if( mIsCircle )
    return ((mDiametrWidth >> 1) + qMax(abs(mCenterX),abs(mCenterY)));
  //Calculate over circle for rectangle
  double w = mDiametrWidth >> 1;
  double h = mHeight >> 1;
  int r = static_cast<int>( sqrt( w*w + h*h ) );
  return r + qMax(abs(mCenterX),abs(mCenterY));
  }





//Create textual pad description
QString SdPad::description() const
  {
  double ppm = sdEnvir->mPrtPPM;
  if( mIsCircle ) {
    if( mHoleDiametr <= 0 )
      //smd
      return QObject::tr("Planar circle diam pad%1 mask%2 stencil%3").arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth-mStencilThreshold,ppm) );
    else
      //through (no stencil)
      return QObject::tr("Throgh circle diam hole%3 pad%1 mask%2").arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHoleDiametr,ppm) );
    }
  else {
    if( mHoleDiametr <= 0 ) {
      //smd
      if( mStencilCols > 1 || mStencilRows > 1 )
        //Array of stencil apertures
        return QObject::tr("Planar rect pad%1x%2 mask%3x%4 stencil ar%5x%6 of%7x%8")
            .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
            .arg( SdUtil::log2physStr(mHeight,ppm))
            .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
            .arg( qMax(mStencilCols,1) )
            .arg( qMax(mStencilRows,1) )
            .arg( SdUtil::log2physStr(mDiametrWidth-mStencilThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight-mStencilThreshold,ppm) );
      else
        //Single stencil aperture
        return QObject::tr("Planar rect pad%1x%2 mask%3x%4 stencil%5x%6")
            .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
            .arg( SdUtil::log2physStr(mHeight,ppm))
            .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mDiametrWidth-mStencilThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight-mStencilThreshold,ppm) );

      }
    else {
      //through (no stencil)
      //Pad with hole
      if( mHoleLength > 0 )
        //Elongated hole
        return QObject::tr("Through rect hole%5x%6a%7 pad%1x%2 mask%3x%4")
            .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
            .arg( SdUtil::log2physStr(mHeight,ppm))
            .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
            .arg( SdUtil::log2physStr(mHoleDiametr,ppm) )
            .arg( SdUtil::log2physStr(mHoleLength,ppm) )
            .arg( SdUtil::log2physStr(mSlotAngle,0.001) );

      //Simple circle hole
      return QObject::tr("Through rect hole%5 pad%1x%2 mask%3x%4")
          .arg( SdUtil::log2physStr(mDiametrWidth,ppm) )
          .arg( SdUtil::log2physStr(mHeight,ppm))
          .arg( SdUtil::log2physStr(mDiametrWidth+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHeight+mMaskThreshold,ppm) )
          .arg( SdUtil::log2physStr(mHoleDiametr,ppm) );
      }
    }
  //return QString();
  }




//Create textual pad name
QString SdPad::name() const
  {
  QString str;
  if( mIsCircle ) str = QChar('c') + SdUtil::um2mm(mDiametrWidth);
  else str = QChar('r') + SdUtil::um2mm(mDiametrWidth) + QChar('x') + SdUtil::um2mm(mHeight);
  if( mCenterX || mCenterY )
    str += QChar('f') + SdUtil::um2mm(mCenterX) + QChar('x') + SdUtil::um2mm(mCenterY);
  if( mHoleDiametr > 0 ) {
    str += QChar('d') + SdUtil::um2mm(mHoleDiametr);
    if( mHoleLength > 0 )
      str += QChar('l') + SdUtil::um2mm(mHoleLength) + QChar('a') + SdUtil::um2mm(mSlotAngle);
    }
  if( mMaskThreshold > 0 )
    str += QChar('m') + SdUtil::um2mm(mMaskThreshold);
  if( mStencilThreshold > 0 ) {
    str += QChar('s') + SdUtil::um2mm(mStencilThreshold);
    if( mStencilCols > 1 || mStencilRows > 1 )
      str += QChar('a') + QString::number(mStencilCols) + QChar('x') + QString::number(mStencilRows) +
             QChar('x') + SdUtil::um2mm(mStencilWidth) + QChar('x') + SdUtil::um2mm(mStencilHeight);
    }

  return str;
  }




//Create pad on name base
void SdPad::parse(const QString nm)
  {
  clear();
  if( nm.length() ) {
    //Skeep start symbols
    //We find 'c' or 'r' with susequent digit
    int index = 1;
    while( index < nm.length() ) {
      if( (nm.at(index-1) == QChar('c') || nm.at(index-1) == QChar('r')) && nm.at(index).isDigit() )
        break;
      index++;
      }
    //We do not find available symbols
    if( index >= nm.length() )
      return;

    //We find signature
    if( nm.at(index-1) == QChar('c') ) {
      mDiametrWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
      mHeight = -1;
      mIsCircle = true;
      }
    else if( nm.at(index-1) == QChar('r') ) {
      mDiametrWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
      index++;
      mHeight = SdUtil::extractLogFromString( nm, index, 0.001 );
      mIsCircle = false;
      }
    else return;

    if( nm.length() > index && nm.at(index) == QChar('f') ) {
      index++;
      mCenterX = SdUtil::extractLogFromString( nm, index, 0.001 );
      index++;
      mCenterY = SdUtil::extractLogFromString( nm, index, 0.001 );
      }
    else mCenterX = mCenterY = 0;


    if( nm.length() > index && nm.at(index) == QChar('d') ) {
      index++;
      mHoleDiametr = SdUtil::extractLogFromString( nm, index, 0.001 );

      if( nm.length() > index && nm.at(index) == QChar('l') ) {
        index++;
        mHoleLength = SdUtil::extractLogFromString( nm, index, 0.001 );
        if( nm.length() > index && nm.at(index) == QChar('a') ) {
          index++;
          mSlotAngle = SdUtil::extractLogFromString( nm, index, 0.001 );
          }
        else mSlotAngle = 0;
        }
      else mHoleLength = mSlotAngle = 0;
      }
    else mHoleDiametr = mHoleLength = mSlotAngle = 0;


    if( nm.length() > index && nm.at(index) == QChar('m') ) {
      index++;
      mMaskThreshold = SdUtil::extractLogFromString( nm, index, 0.001 );
      }
    else mMaskThreshold = 0;

    if( nm.length() > index && nm.at(index) == QChar('s') ) {
      index++;
      mStencilThreshold = SdUtil::extractLogFromString( nm, index, 0.001 );
      if( nm.length() > index && nm.at(index) == QChar('a') ) {
        index++;
        mStencilCols = SdUtil::extractLogFromString( nm, index, 1 );
        index++;
        mStencilRows = SdUtil::extractLogFromString( nm, index, 1 );
        index++;
        mStencilWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
        index++;
        mStencilHeight = SdUtil::extractLogFromString( nm, index, 0.001 );
        }
      }
    else mStencilThreshold = 0;

    }
  }



void SdPad::clear()
  {
  mCenterX = 0;            //Center of circle or rectangle
  mCenterY = 0;
  mDiametrWidth = 400;     //Diametr of circle or rectangle dimensions
  mHeight = -1;
  mMaskThreshold = 100;    //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mStencilThreshold = 40;  //Stencil threshold. Calculation: stencilSize = size + mMaskThreshold
  mStencilRows = 0;        //If rows or colons > 1 then stencil area divided on array of aperture with
  mStencilCols = 0;        // width and height
  mStencilWidth = 0;
  mStencilHeight = 0;
  mHoleDiametr = 0;
  mHoleLength = 0;
  mSlotAngle = 0;
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle = true;
  }




//Calculate points of slot hole
void SdPad::slotPoints(SdPoint &a, SdPoint &b) const
  {
  a.set(-mHoleLength/2,0);
  b.set(mHoleLength/2,0);
  a.rotate( SdPoint(), mSlotAngle );
  b.rotate( SdPoint(), mSlotAngle );
  }


