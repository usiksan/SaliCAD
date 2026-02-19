/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDTEXTPROP_H
#define SDTEXTPROP_H

#include "SdPvInt.h"
#include "SdPvAngle.h"
#include "SdPvLayer.h"
#include "SdPvMulty.h"

//!
//! \defgroup text_alignment Text alignment constants
//! @{
//!

//Horizontal alignment
#define dhjLeft    0   //!< Left horizontal alignment
#define dhjCenter  1   //!< Center horizontal alignment
#define dhjRight   2   //!< Right horizontal alignment

//Vertical alignment
#define dvjTop     0   //!< Top vertical alignment
#define dvjMiddle  1   //!< Middle vertical alignment
#define dvjBottom  2   //!< Bottom vertical alignment

//! @}

//!
//! \brief The SdPropText struct - Text properties for annotations and labels
//!        Stores all visual and positioning attributes for text elements
//!
struct SdPropText
  {
    SdPvLayer mLayer;  //!< Layer of text placement [Слой размещения текста]
    SdPvInt   mSize;   //!< Text size in logical units [Размер текста]
    SdPvAngle mDir;    //!< Text direction/rotation angle [Направление/угол поворота]
    SdPvInt   mFont;   //!< Font identifier [Идентификатор шрифта]
    SdPvInt   mHorz;   //!< Horizontal alignment (see dhjLeft, dhjCenter, dhjRight) [Выравнивание горизонтальное]
    SdPvInt   mVert;   //!< Vertical alignment (see dvjTop, dvjMiddle, dvjBottom) [Выравнивание вертикальное]
    SdPvInt   mMirror; //!< Mirror factor (0 - normal, 1 - mirrored) [Зеркальность]

    //!
    //! \brief json Write text properties to JSON with specified prefix
    //! \param prefix JSON key prefix for grouped properties
    //! \param js     JSON writer object
    //!
    void json( const QString prefix, SvJsonWriter &js ) const;

    //!
    //! \brief json Read text properties from JSON with specified prefix
    //! \param prefix JSON key prefix for grouped properties
    //! \param js     JSON reader object
    //!
    void json( const QString prefix, const SdJsonReader &js );

    //!
    //! \brief swap Swap all text properties with another instance
    //! \param other Other SdPropText object to swap with
    //!
    void swap( SdPropText &other );
  };

using SdPropComposerText = SdPropComposer<SdPropText, &SdPropText::mDir, &SdPropText::mFont, &SdPropText::mHorz,
                                        &SdPropText::mLayer, &SdPropText::mMirror, &SdPropText::mSize, &SdPropText::mVert>;

//Parse src string and find numeric substring. Numeric substring converted into int, int incremented
//and convert to substring. Substring, at end, inserted into src string and return result
//Example: src=PORT18CDF will result PORT19CDF
QString nextText( const QString src );

#endif // SDTEXTPROP_H
