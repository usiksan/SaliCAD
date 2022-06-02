/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Common types for script language
*/
#ifndef SDSCRIPTTYPES_H
#define SDSCRIPTTYPES_H

#include "objects/Sd3dRegion.h"
#include "objects/Sd3dModel.h"

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <QList>
#include <QColor>

#define SD_SCRIPT_TYPE_NONE      '_'
//basis
#define SD_SCRIPT_TYPE_BOOL      'b'
#define SD_SCRIPT_TYPE_FLOAT     'd'
#define SD_SCRIPT_TYPE_COLOR     'c'
#define SD_SCRIPT_TYPE_STRING    's'
#define SD_SCRIPT_TYPE_AFLOAT    'a'
//param reference
#define SD_SCRIPT_TYPE_REF       't'
//2d
#define SD_SCRIPT_TYPE_GRAPH     'g'
//3d
#define SD_SCRIPT_TYPE_VERTEX    'v' //Single vertex index
#define SD_SCRIPT_TYPE_FACE      'f' //List of vertex index
#define SD_SCRIPT_TYPE_FACE_LIST 'l' //List of list of vertex index
#define SD_SCRIPT_TYPE_MODEL     'm' //Model, instance of body
#define SD_SCRIPT_TYPE_MATRIX    'x'


//!
//! \brief The SdScriptVal2dGraph class Universal 2d graph object. It represents any 2d object in 3d scene: line, rect and frect, circle, pin, ident and value
//!
class SdScriptVal2dGraph {
    QPoint  mPos;             //!< First position of object. Start point of line, first vertex of rect, point of pin, center of circle
    QPoint  mPosB;            //!< Second position of object if present. End point of line, second vertex of rect, radius of circle, pos of pin number
    QPoint  mPosName;         //!< Position of pin name
    QString mPad;             //!< Pin pad description
    QString mPinNumber;       //!< Pin number for part pin or pin name for symbol pin
    QString mPinNumberAttr;   //!< Pin number text attributes
    QString mPinNameAttr;     //!< Pin name text attributes
  public:
    enum {
      sdm2dLine,
      sdm2dRect,
      sdm2dCircle,
      sdm2dFRect,
      sdm2dPin,
      sdm2dIdent,
      sdm2dValue
    }       mType;

    //====================================================================================
    // Line, Rect and FRect
    //!
    //! \brief pointA Return first point of line or vertex of rect
    //! \return       First point of line or vertex of rect
    //!
    QPoint  pointA() const { return mPos; }

    //!
    //! \brief pointB Return second point of line or vertex of rect
    //! \return       Second point of line or vertex of rect
    //!
    QPoint  pointB() const { return mPosB; }





    //====================================================================================
    // Circle
    //!
    //! \brief circleCenter Return center of circle
    //! \return             Center of circle
    //!
    QPoint  circleCenter() const { return mPos; }

    //!
    //! \brief circleRadius Return radius of circle
    //! \return             Radius of circle
    //!
    int     circleRadius() const { return mPosB.x(); }




    //====================================================================================
    // Pin
    //!
    //! \brief pinPos Return pin position
    //! \return       Pin position
    //!
    QPoint  pinPos() const { return mPos; }

    //!
    //! \brief pinPad Return pad description
    //! \return       Pad description
    //!
    QString pinPad() const { return mPad; }

    //!
    //! \brief pinNumberPos Return pin number position
    //! \return             Pin number position
    //!
    QPoint  pinNumberPos() const { return mPosB; }

    //!
    //! \brief pinNumber Return pin number for part pin
    //! \return          Pin number
    //!
    QString pinNumber() const { return mPinNumber; }

    //!
    //! \brief pinName Return pin name for symbol pin
    //! \return        Pin name
    //!
    QString pinName() const { return mPinNumber; }

    //!
    //! \brief pinNumberAttr Return pin number text attributes
    //! \return              Pin number text attributes
    //!
    QString pinNumberAttr() const { return mPinNumberAttr; }

    //!
    //! \brief pinNamePos Return pin name position
    //! \return           Pin name position
    //!
    QPoint  pinNamePos() const { return mPosName; }

    //!
    //! \brief pinNameAttr Return pin name text attributes
    //! \return            Pin name text attributes
    //!
    QString pinNameAttr() const { return mPinNameAttr; }




    //====================================================================================
    // Ident
    //!
    //! \brief identPos Return ident position
    //! \return         Ident position
    //!
    QPoint  identPos() const { return mPos; }

    //!
    //! \brief identAttr Return ident text attributes
    //! \return          Ident text attributes
    //!
    QString identAttr() const { return mPinNameAttr; }




    //====================================================================================
    // Value
    //!
    //! \brief valuePos Return value position
    //! \return         Value position
    //!
    QPoint  valuePos() const { return mPos; }

    //!
    //! \brief valueAttr Return value text attributes
    //! \return          Value text attributes
    //!
    QString valueAttr() const { return mPinNameAttr; }




    //!
    //! \brief vectorToPoint Convert mm to mcm
    //! \param p             3d point in mm
    //! \return              2s point in mcm
    //!
    //QPoint  vectorToPoint( QVector3D p ) const { p *= 1000.0; return p.toPoint(); }

    //int     scale( float fv ) const { return fv * 1000.0; }

    int     scale( const QList<float> &lst, int index ) const { return (lst.count() > index ? lst.at(index) : 0) * 1000.0; }

    QPoint  toPoint( const QList<float> &lst, int index ) const { return QPoint( scale(lst, index), scale(lst, index+1) ); }

    QPoint  toPoint( const QList<float> &lst ) const { return toPoint( lst, 0 ); }



    //====================================================================================
    // Consturctors for each object
    //Build line
    SdScriptVal2dGraph( const QList<float> &p1, const QList<float> &p2 ) { mType = sdm2dLine; mPos = toPoint(p1); mPosB = toPoint(p2); }

    //Build rect or frect
    SdScriptVal2dGraph( const QList<float> &p1, const QList<float> &p2, bool filled ) { mType = filled ? sdm2dFRect : sdm2dRect; mPos = toPoint(p1); mPosB = toPoint(p2); }

    //Build circle
    SdScriptVal2dGraph( const QList<float> &lst ) { mType = sdm2dCircle; mPos = toPoint( lst ); mPosB.setX( scale( lst, 2 ) ); }

    //Build pin
    SdScriptVal2dGraph( const QList<float> &pinPos, QString pad, const QList<float> &numberPos, QString number, QString numberAttr, const QList<float> &namePos, QString nameAttr ) {
      mType = sdm2dPin;
      mPos = toPoint(pinPos);
      mPad = pad;
      mPosB = toPoint(numberPos);
      mPinNumber = number;
      mPinNumberAttr = numberAttr;
      mPosName = toPoint(namePos);
      mPinNameAttr = nameAttr;
      }

    //Build ident or value place
    SdScriptVal2dGraph( const QList<float> &pos, QString attr, bool isIdent ) {
      mType = isIdent ? sdm2dIdent : sdm2dValue;
      mPos = toPoint(pos);
      mPinNameAttr = attr;
      }
  };


#endif // SDSCRIPTTYPES_H
