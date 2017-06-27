/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Converter form template matrix to convert physical coords into pixels
*/
#ifndef SDCONVERTER_H
#define SDCONVERTER_H

#include <QTransform>

class SdContext;
class SdLayer;

class SdConverter
  {
  protected:
    SdConverter *mNext;      //Next linked converter
    SdContext   *mContext;   //Context is working on
    bool         mPairLayer; //True when need paired layer
  public:
    SdConverter();
    virtual ~SdConverter();

    //Convert layer flag
    bool               getFullPairLayer();

    //Calculate and return full transformation matrix
    QTransform         getFullMatrix();

    //Return local conversion transformation matrix
    virtual QTransform getMatrix() { return QTransform(); }

    //Return scale factor
    virtual double     getScale() const;

    virtual bool       getMirror() const;

    friend class SdContext;
  };

#endif // SDCONVERTER_H
