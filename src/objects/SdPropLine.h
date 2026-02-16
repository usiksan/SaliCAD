/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDLINEPROP_H
#define SDLINEPROP_H

#include "SdPropInt.h"
#include "SdPropLayer.h"

//Line types
#define dltSolid  0 //Solid line _______
#define dltDotted 1 //Dotted line . . . . .
#define dltDashed 2 //Dashed line __  __  __


//!
//! \brief The SdPropLine struct - Composite property structure for line attributes
//!        Combines width, type, and layer properties with JSON serialization
//!
struct SdPropLine {
  SdPropInt   mWidth; //!< Line width property in logical units
  SdPropInt   mType;  //!< Line type property (solid, dashed, dotted, etc.)
  SdPropLayer mLayer; //!< Layer property where line is drawn

  //!
  //! \brief SdPropLine Default constructor
  //!        Initializes width and type to 0, layer to default
  //!
  SdPropLine() : mWidth(0), mType(0), mLayer() { }

  //!
  //! \brief setLayerUsage Mark the associated layer as used
  //!        Delegates to mLayer.setLayerUsage()
  //!
  void setLayerUsage() const { mLayer.setLayerUsage(); }

  //!
  //! \brief json Write line properties to JSON with specified prefix
  //! \param prefix JSON key prefix for grouped properties
  //! \param js     JSON writer object
  //!
  void json( const QString &prefix, SvJsonWriter &js ) const;


  //!
  //! \brief json Read line properties from JSON with specified prefix
  //! \param prefix JSON key prefix for grouped properties
  //! \param js     JSON reader object
  //!
  void json( const QString &prefix, const SdJsonReader &js );


  //!
  //! \brief json Write line properties to JSON with default "line" prefix
  //! \param js JSON writer object
  //!
  void json( SvJsonWriter &js ) const { json( "line", js ); }


  //!
  //! \brief json Read line properties from JSON with default "line" prefix
  //! \param js JSON reader object
  //!
  void json( const SdJsonReader &js ) { json( "line", js ); }


  //!
  //! \brief swap Swap all line properties with another instance
  //! \param other Other SdPropLine object to swap with
  //!
  void swap( SdPropLine &other );
  };

#endif // SDLINEPROP_H
