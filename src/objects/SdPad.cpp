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
#include "objects/SdConverterText.h"

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
  mAngle(0),
  mRoundRectRatio(0),    // Rounded rectangle ratio in hundredths (0 if not rounded)
  mEllipse(false),       // True for elliptical pad shape
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle(true)
  {

  }









void SdPad::draw(SdContext *dcx, SdPoint p, SdPvStratum stratum) const
  {
  //If no stratum then no draw
  if( stratum == 0 )
    return;

  // Apply transformations in reverse order (last operation = first applied)
  QTransform transform;

  // Step 3 (last operation): Translate to final position p
  // This will be applied first to the local polygon
  transform.translate(p.x(), p.y());

  // Step 2: Rotate around (0,0) if angle is non-zero
  if( mAngle != 0 )
    transform.rotate( double(mAngle) / 1000.0);  // Convert from 0.001 degrees

  // Step 1 (first operation): Offset by mCenter (relative to pad origin)
  // This will be applied last
  transform.translate(mCenterX, mCenterY);

  //Draw pad mask
  SdLayer *layer = SdEnvir::instance()->layerVisibleForMask(stratum);
  if( layer != nullptr && layer->isVisible() && mMaskThreshold > 0 ) {
    if( !mCacheMask.isEmpty() )
      dcx->polygonFill( transform.map(mCacheMask), layer->color() );
    else {
      int w = mDiametrWidth + mMaskThreshold * 2;
      int h = mHeight + mMaskThreshold * 2;
      if( mIsCircle ) dcx->circleFill( transform.map(QPoint{}), w >> 1, layer->color() );
      else dcx->polygonFill( transform.mapToPolygon( QRect( -(w >> 1), -(h >> 1), w, h) ), layer->color() );
      }
    }

  //Draw stencil
  layer = SdEnvir::instance()->layerVisibleForStencil(stratum);
  if( layer != nullptr && layer->isVisible() && mStencilThreshold > 0 ) {
    if( !mCacheStencil.isEmpty() )
      dcx->polygonFill( transform.map(mCacheStencil), layer->color() );
    else {
      int w = mDiametrWidth - mStencilThreshold * 2;
      int h = mHeight - mStencilThreshold * 2;
      if( mIsCircle ) dcx->circleFill( transform.map(QPoint{}), w >> 1, layer->color() );
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
              dcx->polygonFill( transform.mapToPolygon( QRect( -(w >> 1) + x * cellx + offx,
                                     -(h >> 1) + y * celly + offy, mStencilWidth, mStencilHeight) ), layer->color() );
          }
        else
          //Single apreture
          dcx->polygonFill( transform.mapToPolygon( QRect( -(w >> 1), -(h >> 1), w, h) ), layer->color() );
        }
      }
    }

  //Draw pad itself
  layer = SdEnvir::instance()->layerVisibleForPad(stratum);
  if( layer != nullptr && layer->isVisible() ) {
    if( !mCachePad.isEmpty() )
      dcx->polygonFill( transform.map(mCachePad), layer->color() );
    else if( mIsCircle ) dcx->circleFill( transform.map(QPoint{}), mDiametrWidth >> 1, layer->color() );
    else dcx->polygonFill( transform.mapToPolygon( QRect( -(mDiametrWidth >> 1), -(mHeight >> 1), mDiametrWidth, mHeight) ), layer->color() );
    }

  //Draw hole
  layer = SdEnvir::instance()->layerVisibleForHole(stratum);
  if( layer != nullptr && layer->isVisible() && mHoleDiametr > 0 ) {
    if( mHoleLength > 0 ) {
      //Hole is slot type
      SdPoint a,b;
      slotPoints( a, b );
      dcx->setPen( mHoleDiametr, layer, dltSolid );
      dcx->line( transform.map(a), transform.map(b) );
      }
    else
      //Hole is circle type
      dcx->circleFill( transform.map( QPoint{} ), mHoleDiametr >> 1, layer->color() );
    }

  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//! \param p      Point of center of pad
//!
void SdPad::draw3d(QOpenGLFunctions_2_0 *f, SdPoint p) const
  {
  f->glPushMatrix();
  f->glTranslatef( p.xmm(), p.ymm(), 0 );
  mModel.draw3d( f );
  f->glPopMatrix();
  }





//!
//! \brief polygon Returns polygon for pad with optional size adjustment and rotation
//! \param p       Final position of pad center
//! \param addon   Additional size to add to pad (positive for enlargement, negative for reduction)
//! \return        Polygon representing pad shape with offset and rotation applied
//!
QPolygonF SdPad::polygon(SdPoint p, int addon ) const
  {
  QPolygonF localPoly;  // Polygon centered at (0,0)

  // Build local polygon centered at origin
  if( !mCachePad.isEmpty() ) {
    // For cached shapes (ellipse or rounded rectangle), generate polygon
    // with adjusted dimensions, centered at (0,0)
    int newWidth = mDiametrWidth + addon * 2;
    int newHeight = mHeight + addon * 2;
    QRect localRect( -newWidth/2, -newHeight/2, newWidth, newHeight );

    if( mEllipse ) {
      localPoly = SdUtil::ellipse(localRect,40);
      }
    else if( !mIsCircle && mRoundRectRatio > 0 ) {
      localPoly = SdUtil::roundrect(localRect, mRoundRectRatio, 40);
      }
    }
  else if( mIsCircle ) {
    // Circle: generate octagon centered at origin
    int radius = (mDiametrWidth >> 1) + addon;
    localPoly = SdUtil::octagon(0, 0, radius);
    }
  else {
    // Simple rectangle centered at origin
    int w = mDiametrWidth + addon * 2;
    int h = mHeight + addon * 2;
    localPoly << QPointF(-w/2, -h/2)
              << QPointF( w/2, -h/2)
              << QPointF( w/2,  h/2)
              << QPointF(-w/2,  h/2);
    }

  // Apply transformations in reverse order (last operation = first applied)
  QTransform transform;

  // Step 3 (last operation): Translate to final position p
  // This will be applied first to the local polygon
  transform.translate(p.x(), p.y());

  // Step 2: Rotate around (0,0) if angle is non-zero
  if( mAngle != 0 ) {
    transform.rotate(mAngle / 1000.0);  // Convert from 0.001 degrees
    }

  // Step 1 (first operation): Offset by mCenter (relative to pad origin)
  // This will be applied last
  transform.translate(mCenterX, mCenterY);

  // The actual transformation order when applied to a point P is:
  // Result = translate(p) * rotate * translate(mCenter) * P
  //
  // Which means:
  // 1. First translate by mCenter (relative to pad origin)
  // 2. Then rotate around (0,0)
  // 3. Finally translate to final position p

  return transform.map(localPoly);
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




//!
//! \brief appendPadHoles Accumulates holes description into faceList
//! \param p              Position of pad
//! \param model          Model which accumulates coordinate vertexes
//! \param faceList       Face list for holding holes (single face for single hole)
//! \param stratum        Stratum for layers (unused)
//! \param map            Base map for holes conversion (applied after local transforms)
//!
void SdPad::appendPadHoles(SdPoint p, Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map) const
  {
  Q_UNUSED(stratum)

  if( !isThrough() )
    return;

  // Build local transformation matrix (applied before map)
  QMatrix4x4 localMat;

  // Step 1: Offset by center (relative to pad origin)
  if( mCenterX != 0 || mCenterY != 0 )
    localMat.translate( QVector3D(QPointF(mCenterX,mCenterY) / 1000.0)  );

  // Step 2: Rotate around Z axis if angle is non-zero
  if( mAngle != 0 )
    localMat.rotate( mAngle / 1000.0, 0, 0, 1 );  // Convert from 0.001 degrees

  // Step 3: Translate to final position p
  localMat.translate( QVector3D(QPointF(p) / 1000.0) );

  // Combine with incoming map: map * localMat (local applied first, then map)
  QMatrix4x4 combined = map * localMat;

  if( mHoleLength > 0 ) {
    // Slot hole (elongated hole) - generate as rounded rectangle
    // Use ratio = 500 (0.5) to create fully rounded ends (like a slot)
    int holeWidth = mHoleDiametr;
    int holeHeight = mHoleDiametr + mHoleLength;  // Length adds to diameter

    // Create hole rectangle centered at origin
    QRect holeRect( -holeWidth/2, -holeHeight/2, holeWidth, holeHeight );

    // Generate rounded rectangle polygon with 0.5 ratio (fully rounded ends)
    QPolygonF holePoly = SdUtil::roundrect(holeRect, 500, 40);  // 500 = 0.5 in thousandths

    // Convert to 3D face with combined transformation
    faceList.append( model.faceFromUmPolygon(holePoly, combined) );
    }
  else {
    // Round hole
    faceList.append( model.faceCircleSide( static_cast<float>(mDiametrWidth >> 1) / 1000.0, 32, combined ) );
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





//!
//! \brief description Creates human-readable textual pad description
//! \return            Localized string describing pad parameters in mm units
//!
QString SdPad::description() const
  {
  QString str;

  // Basic shape
  if( mIsCircle ) {
    str = QObject::tr("Circle diam %1mm").arg(SdUtil::um2mm(mDiametrWidth));
    }
  else if( mEllipse ) {
    str = QObject::tr("Ellipse %1*%2mm").arg(SdUtil::um2mm(mDiametrWidth),
                                              SdUtil::um2mm(mHeight));
    }
  else {
    if( mRoundRectRatio > 0 )
      str = QObject::tr("Rounded rect %1*%2mm ratio %3mm").arg(SdUtil::um2mm(mDiametrWidth),
                                                           SdUtil::um2mm(mHeight),
                                                           SdUtil::um2mm(mRoundRectRatio));
    else
      str = QObject::tr("Rect %1*%2mm").arg(SdUtil::um2mm(mDiametrWidth),
                                             SdUtil::um2mm(mHeight));
    }

  // Offset from center
  if( mCenterX || mCenterY )
    str += QObject::tr(" off %1*%2mm").arg(SdUtil::um2mm(mCenterX),
                                           SdUtil::um2mm(mCenterY));

  // Rotation angle
  if( mAngle != 0 )
    str += QObject::tr(" rotate %1°").arg( SdUtil::um2mm(mAngle) );

  // Hole definition
  if( mHoleDiametr > 0 ) {
    if( mHoleLength > 0 )
      str += QObject::tr(" slot %1*%2mm").arg(SdUtil::um2mm(mHoleDiametr),
                                              SdUtil::um2mm(mHoleLength));
    else
      str += QObject::tr(" hole %1mm").arg(SdUtil::um2mm(mHoleDiametr));
    }

  // Mask threshold
  if( mMaskThreshold > 0 ) {
    if( mMaskThreshold == 1 )
      str += QObject::tr(" mask=pad");
    else if( mIsCircle )
      str += QObject::tr(" mask %1mm").arg(SdUtil::um2mm(mDiametrWidth + mMaskThreshold));
    else
      str += QObject::tr(" mask %1*%2mm").arg(SdUtil::um2mm(mDiametrWidth + mMaskThreshold),
                                              SdUtil::um2mm(mHeight + mMaskThreshold));
    }

  // Stencil definition
  if( mStencilThreshold > 0 ) {
    if( mStencilThreshold == 1 )
      str += QObject::tr(" stencil=pad");
    else if( mStencilCols > 1 || mStencilRows > 1 ) {
      str += QObject::tr(" stencil arr %1*%2 of %3*%4mm")
          .arg(qMax(mStencilCols, 1))
          .arg(qMax(mStencilRows, 1))
          .arg(SdUtil::um2mm(mStencilWidth), SdUtil::um2mm(mStencilHeight));
      }
    else if( mIsCircle )
      str += QObject::tr(" stencil %1mm").arg(SdUtil::um2mm(mDiametrWidth - mStencilThreshold));
    else
      str += QObject::tr(" stencil %1*%2mm").arg(SdUtil::um2mm(mDiametrWidth - mStencilThreshold),
                                                 SdUtil::um2mm(mHeight - mStencilThreshold));
    }

  return str;
  }



//!
//! \brief name Creates textual pad name for serialization
//! \return     Pad name string with format:
//!             c<diameter> - circle
//!             r<width>x<height> - rectangle
//!             r<width>x<height>u<round> - rounded rectangle (round in thousandths)
//!             e<width>x<height> - ellipse
//!             followed by optional modifiers:
//!             f<dx>x<dy> - offset from center
//!             a<angle> - rotation angle for pad and hole (0.001 degrees)
//!             d<diameter> - hole diameter
//!             l<length> - slot length (if hole is slot)
//!             m<threshold> - mask threshold (m0 for exact match, positive for enlargement)
//!             s<threshold> - stencil threshold (s0 for exact match, positive for reduction)
//!             a<cols>x<rows>x<width>x<height> - stencil array
//!
QString SdPad::name() const
  {
  QString str;

  // Basic shape
  if( mIsCircle ) {
    str = QChar('c') + SdUtil::um2mm(mDiametrWidth);
    }
  else if( mEllipse ) {
    str = QChar('e') + SdUtil::um2mm(mDiametrWidth) + QChar('x') + SdUtil::um2mm(mHeight);
    }
  else {
    str = QChar('r') + SdUtil::um2mm(mDiametrWidth) + QChar('x') + SdUtil::um2mm(mHeight);
    if( mRoundRectRatio > 0 )
      str += QChar('u') + SdUtil::um2mm(mRoundRectRatio);
    }

  // Offset from center
  if( mCenterX || mCenterY )
    str += QChar('f') + SdUtil::um2mm(mCenterX) + QChar('x') + SdUtil::um2mm(mCenterY);

  // Rotation angle (applies to both pad and hole)
  if( mAngle != 0 )
    str += QChar('a') + SdUtil::um2mm(mAngle);

  // Hole definition
  if( mHoleDiametr > 0 ) {
    str += QChar('d') + SdUtil::um2mm(mHoleDiametr);
    if( mHoleLength > 0 )
      str += QChar('l') + SdUtil::um2mm(mHoleLength);
    }

  // Mask threshold - special case for exact match (1 micron = practically zero)
  if( mMaskThreshold > 0 ) {
    if( mMaskThreshold == 1 )
      str += QStringLiteral("m0");
    else
      str += QChar('m') + SdUtil::um2mm(mMaskThreshold);
    }

  // Stencil threshold - special case for exact match (1 micron = practically zero)
  if( mStencilThreshold > 0 ) {
    if( mStencilThreshold == 1 )
      str += QStringLiteral("s0");
    else
      str += QChar('s') + SdUtil::um2mm(mStencilThreshold);

    // Stencil array definition
    if( mStencilCols > 1 || mStencilRows > 1 )
      str += QChar('a') + QString::number(mStencilCols) + QChar('x') + QString::number(mStencilRows) +
             QChar('x') + SdUtil::um2mm(mStencilWidth) + QChar('x') + SdUtil::um2mm(mStencilHeight);
    }

  return str;
  }



//!
//! \brief parse Creates pad from name string
//! \param nm    Name string to parse with format:
//!              c<diameter> - circle
//!              r<width>x<height> - rectangle
//!              r<width>x<height>u<round> - rounded rectangle
//!              e<width>x<height> - ellipse
//!              followed by optional modifiers:
//!              f<dx>x<dy> - offset from center
//!              a<angle> - rotation angle
//!              d<diameter> - hole diameter
//!              l<length> - slot length
//!              m<threshold> - mask threshold (m0 for exact match)
//!              s<threshold> - stencil threshold (s0 for exact match)
//!              a<cols>x<rows>x<width>x<height> - stencil array
//!
void SdPad::parse(const QString nm)
  {
  clear();

  if( nm.isEmpty() )
    return;

  // Skip start symbols and find shape prefix with following digit
  int index = 1;
  while( index < nm.length() ) {
    QChar ch = nm.at(index-1);
    if( (ch == QChar('c') || ch == QChar('r') || ch == QChar('e')) && nm.at(index).isDigit() )
      break;
    index++;
    }

  // No valid shape prefix found
  if( index >= nm.length() )
    return;

  // Parse basic shape
  QChar shapePrefix = nm.at(index-1);

  if( shapePrefix == QChar('c') ) {
    // Circle
    mDiametrWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
    mHeight = -1;
    mIsCircle = true;
    mEllipse = false;
    mRoundRectRatio = 0;
    }
  else if( shapePrefix == QChar('e') ) {
    // Ellipse
    mDiametrWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
    index++;
    mHeight = SdUtil::extractLogFromString( nm, index, 0.001 );
    mIsCircle = false;
    mEllipse = true;
    mRoundRectRatio = 0;
    }
  else if( shapePrefix == QChar('r') ) {
    // Rectangle (may be rounded)
    mDiametrWidth = SdUtil::extractLogFromString( nm, index, 0.001 );
    index++;
    mHeight = SdUtil::extractLogFromString( nm, index, 0.001 );
    mIsCircle = false;
    mEllipse = false;
    mRoundRectRatio = 0;

    // Check for rounded rectangle suffix
    if( nm.length() > index && nm.at(index) == QChar('u') ) {
      index++;
      mRoundRectRatio = SdUtil::extractLogFromString( nm, index, 0.001 );
      }
    }
  else
    return;

  // Parse offset from center
  if( nm.length() > index && nm.at(index) == QChar('f') ) {
    index++;
    mCenterX = SdUtil::extractLogFromString( nm, index, 0.001 );
    index++;
    mCenterY = SdUtil::extractLogFromString( nm, index, 0.001 );
    }
  else {
    mCenterX = 0;
    mCenterY = 0;
    }

  // Parse rotation angle (applies to entire pad)
  if( nm.length() > index && nm.at(index) == QChar('a') ) {
    index++;
    mAngle = SdUtil::extractLogFromString( nm, index, 0.001 );
    }
  else {
    mAngle = 0;
    }

  // Parse hole definition
  if( nm.length() > index && nm.at(index) == QChar('d') ) {
    index++;
    mHoleDiametr = SdUtil::extractLogFromString( nm, index, 0.001 );

    // Check for slot hole
    if( nm.length() > index && nm.at(index) == QChar('l') ) {
      index++;
      mHoleLength = SdUtil::extractLogFromString( nm, index, 0.001 );
      }
    else {
      mHoleLength = 0;
      }
    }
  else {
    mHoleDiametr = 0;
    mHoleLength = 0;
    }

  // Parse mask threshold (special case: m0 = 1 micron)
  if( nm.length() > index && nm.at(index) == QChar('m') ) {
    index++;
    mMaskThreshold = SdUtil::extractLogFromString( nm, index, 0.001 );

    // Special case: m0 = exact mask match
    if( mMaskThreshold == 0 )
      mMaskThreshold = 1;  // 1 micron = practically zero
    }
  else {
    mMaskThreshold = 0;
    }

  // Parse stencil definition
  if( nm.length() > index && nm.at(index) == QChar('s') ) {
    index++;
    mStencilThreshold = SdUtil::extractLogFromString( nm, index, 0.001 );

    // Special case: s0 = exact stencil match
    if( mStencilThreshold == 0 )
      mStencilThreshold = 1;  // 1 micron = practically zero

    // Check for stencil array
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
    else {
      mStencilCols = 0;
      mStencilRows = 0;
      mStencilWidth = 0;
      mStencilHeight = 0;
      }
    }
  else {
    mStencilThreshold = 0;
    mStencilCols = 0;
    mStencilRows = 0;
    mStencilWidth = 0;
    mStencilHeight = 0;
    }

  //Rebuild caches for new settings
  rebuildCache();

  // Note: 3D model generation will be handled separately

  mModel.clear();
  //Build 3d model for pad
  //3d drawing mach different for hole or without hole
  float radius = mDiametrWidth >> 1;
  radius /= 1000.0;

  // Build local transformation matrix
  QMatrix4x4 localMat;

  // Step 1: Offset by center (relative to pad origin)
  if( mCenterX != 0 || mCenterY != 0 )
    localMat.translate( QVector3D(QPointF(mCenterX,mCenterY) / 1000.0)  );

  // Step 2: Rotate around Z axis if angle is non-zero
  if( mAngle != 0 )
    localMat.rotate( mAngle / 1000.0, 0, 0, 1 );  // Convert from 0.001 degrees

  Sd3drBody body;
  if( mHoleDiametr > 0 ) {
    //Hole present
    Sd3drFace hole3d = mModel.faceCircleSide( static_cast<float>(mHoleDiametr) / 2000.0, 40, localMat );
    Sd3drFace pad3d;
    if( mIsCircle )
      pad3d = mModel.faceCircleSide( radius, 40, localMat );
    else
      pad3d = mModel.faceRectangleSide( static_cast<float>(mDiametrWidth) / 1000.0, static_cast<float>(mHeight) / 1000.0, 40, localMat );
    //Top pad
    body.faceAppend( mModel.faceListWall( pad3d, hole3d, true ) );
    //Bottom pad
    QMatrix4x4 shift;
    shift.translate( 0, 0, -1.45 );
    Sd3drFace botHole3d = mModel.faceDuplicate( hole3d, shift );
    Sd3drFace botPad3d  = mModel.faceDuplicate( pad3d, shift );
    body.faceAppend( mModel.faceListWall( botPad3d, botHole3d, true ) );
    //Hole
    body.faceAppend( mModel.faceListWall( hole3d, botHole3d, true ) );
    }
  else {
    //Smd pad
    Sd3drFace pad3d;
    if( mIsCircle )
      pad3d = mModel.faceCircle( radius, 10, localMat );
    else
      pad3d = mModel.faceRectangle( static_cast<float>(mDiametrWidth) / 1000.0, static_cast<float>(mHeight) / 1000.0, localMat );
    body.faceAppend( pad3d );
    }
  body.colorSet( SdEnvir::instance()->getSysColor( sc3dPadTop ) );
  Sd3drInstance inst;
  inst.add( body );
  inst.addCopy( QMatrix4x4() );
  mModel.instanceAppend( inst );
  }





//!
//! \brief clear Resets pad to default state
//!
void SdPad::clear()
  {
  mCenterX = 0;            // Center of circle or rectangle X coordinate
  mCenterY = 0;            // Center of circle or rectangle Y coordinate
  mDiametrWidth = 400;     // Diameter of circle or rectangle width
  mHeight = -1;            // Rectangle height (if -1 then circle)
  mMaskThreshold = 100;    // Mask threshold. Calculation: maskSize = size + mMaskThreshold
  mStencilThreshold = 40;  // Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
  mStencilRows = 0;        // Number of rows for stencil array
  mStencilCols = 0;        // Number of columns for stencil array
  mStencilWidth = 0;       // Width of individual stencil aperture
  mStencilHeight = 0;      // Height of individual stencil aperture
  mHoleDiametr = 0;        // Hole diameter (if zero then no hole)
  mHoleLength = 0;         // Length for slot holes (if zero then circular hole)
  mAngle = 0;              // Pad and hole rotation angle in 0.001 degrees
  mRoundRectRatio = 0;     // Rounded rectangle ratio in 0.001 units (0 if not rounded)
  mEllipse = false;        // True for elliptical pad shape
  //Not saved. Circle determine by mHeight = 0.
  mIsCircle = true;        // True for circular pad (legacy)

  //Clear caches
  mCachePad.clear();
  mCacheMask.clear();
  mCacheStencil.clear();

  mModel.clear();          // Clear 3D model
  }




//Calculate points of slot hole
void SdPad::slotPoints(SdPoint &a, SdPoint &b) const
  {
  a.set(-mHoleLength/2,0);
  b.set(mHoleLength/2,0);
  }




//!
//! \brief rebuildCache Rebuilds cached polygons for pad, mask and stencil
//!                     Cache is built only for new shapes (ellipse, rounded rect)
//!                     For legacy shapes or when mask/stencil absent, caches remain empty
//!
void SdPad::rebuildCache()
  {
  // Clear all caches first
  mCachePad.clear();
  mCacheMask.clear();
  mCacheStencil.clear();

  // Build caches only for new shapes
  if( mEllipse ) {
    // Elliptical pad
    QRect padRect( -mDiametrWidth/2, -mHeight/2, mDiametrWidth, mHeight );
    mCachePad = SdUtil::ellipse(padRect, 40);

    // Mask for ellipse
    if( mMaskThreshold > 0 ) {
      int maskWidth = mDiametrWidth + mMaskThreshold * 2;
      int maskHeight = mHeight + mMaskThreshold * 2;
      QRect maskRect( -maskWidth/2, -maskHeight/2, maskWidth, maskHeight );
      mCacheMask = SdUtil::ellipse(maskRect, 40);
      }

    // Stencil for ellipse
    if( mStencilThreshold > 0 ) {
      int stencilWidth = mDiametrWidth - mStencilThreshold * 2;
      int stencilHeight = mHeight - mStencilThreshold * 2;
      if( stencilWidth > 0 && stencilHeight > 0 ) {
        QRect stencilRect( -stencilWidth/2, -stencilHeight/2, stencilWidth, stencilHeight );
        mCacheStencil = SdUtil::ellipse(stencilRect, 40 );
        }
      }
    }
  else if( !mIsCircle && mRoundRectRatio > 0 ) {
    // Rounded rectangle pad
    QRect padRect( -mDiametrWidth/2, -mHeight/2, mDiametrWidth, mHeight );
    mCachePad = SdUtil::roundrect(padRect, mRoundRectRatio, 40);

    // Mask for rounded rectangle
    if( mMaskThreshold > 0 ) {
      int maskWidth = mDiametrWidth + mMaskThreshold * 2;
      int maskHeight = mHeight + mMaskThreshold * 2;
      QRect maskRect( -maskWidth/2, -maskHeight/2, maskWidth, maskHeight );
      mCacheMask = SdUtil::roundrect(maskRect, mRoundRectRatio, 40);
      }

    // Stencil for rounded rectangle
    if( mStencilThreshold > 0 ) {
      int stencilWidth = mDiametrWidth - mStencilThreshold * 2;
      int stencilHeight = mHeight - mStencilThreshold * 2;
      if( stencilWidth > 0 && stencilHeight > 0 ) {
        QRect stencilRect( -stencilWidth/2, -stencilHeight/2, stencilWidth, stencilHeight );
        mCacheStencil = SdUtil::roundrect(stencilRect, mRoundRectRatio, 40);
        }
      }
    }
  // For circle and simple rectangle, all caches remain empty (legacy handling)

  // Note: Rotation is not applied in cache - it will be applied during drawing/export
  // using the cached polygon as base
  }



void SdPad::json(SvJsonWriter &js) const
  {
  //Center of circle or rectangle
  if( mCenterX ) js.jsonInt( QStringLiteral("cx"), mCenterX );
  if( mCenterY ) js.jsonInt( QStringLiteral("cy"), mCenterY );
  //Diametr of circle or rectangle dimensions
  js.jsonInt( QStringLiteral("dw"), mDiametrWidth );
  js.jsonInt( QStringLiteral("h"), mIsCircle ? 0 : mHeight );
  //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  js.jsonInt( QStringLiteral("mt"), mMaskThreshold );
  //Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
  js.jsonInt( QStringLiteral("st"), mStencilThreshold );
  //If rows or colons > 1 then stencil area divided on array of aperture with
  // width and height
  if( mStencilRows ) js.jsonInt( QStringLiteral("sr"), mStencilRows );
  if( mStencilCols ) js.jsonInt( QStringLiteral("sc"), mStencilCols );
  if( mStencilWidth) js.jsonInt( QStringLiteral("sw"), mStencilWidth );
  if( mStencilHeight ) js.jsonInt( QStringLiteral("sh"), mStencilHeight );
  if( mHoleDiametr ) js.jsonInt( QStringLiteral("hole"), mHoleDiametr );
  if( mHoleLength ) js.jsonInt( QStringLiteral("slotLen"), mHoleLength );
  if( mAngle ) js.jsonInt( QStringLiteral("slotAng"), mAngle );
  }



void SdPad::json(const SvJsonReader &js)
  {
  //Center of circle or rectangle
  js.jsonInt( QStringLiteral("cx"), mCenterX, 0 );
  js.jsonInt( QStringLiteral("cy"), mCenterY, 0 );
  //Diametr of circle or rectangle dimensions
  js.jsonInt( QStringLiteral("dw"), mDiametrWidth );
  js.jsonInt( QStringLiteral("h"), mHeight );
  //Mask threshold. Calculation: maskSize = size + mMaskThreshold
  js.jsonInt( QStringLiteral("mt"), mMaskThreshold );
  //Stencil threshold. Calculation: stencilSize = size - mStencilThreshold
  js.jsonInt( QStringLiteral("st"), mStencilThreshold );
  //If rows or colons > 1 then stencil area divided on array of aperture with
  // width and height
  js.jsonInt( QStringLiteral("sr"), mStencilRows, 0 );
  js.jsonInt( QStringLiteral("sc"), mStencilCols, 0 );
  js.jsonInt( QStringLiteral("sw"), mStencilWidth, 0 );
  js.jsonInt( QStringLiteral("sh"), mStencilHeight, 0 );
  js.jsonInt( QStringLiteral("hole"), mHoleDiametr, 0 );
  js.jsonInt( QStringLiteral("slotLen"), mHoleLength, 0 );
  js.jsonInt( QStringLiteral("slotAng"), mAngle, 0 );
  }


