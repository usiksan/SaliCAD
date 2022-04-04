/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Class of object.
  Class is a bit in 64bit word. This allow to union classes, fast check classes.
*/
#ifndef SDCLASS_H
#define SDCLASS_H

#include <QString>

//Классы объектов
#define dctLines          0x00000001ul
#define dctText           0x00000002ul
#define dctSymPin         0x00000004ul
#define dctPartPin        0x00000008ul
#define dctIdent          0x00000010ul
#define dctSymbol         0x00000020ul
#define dctSymImp         0x00000040ul
#define dctPart           0x00000080ul
#define dctPartImp        0x00000100ul
#define dctNetWire        0x00000200ul
#define dctNetName        0x00000400ul
#define dctSheet          0x00000800ul
#define dctPlate          0x00001000ul
#define dctComponent      0x00002000ul
#define dctProject        0x00004000ul
#define dctSection        0x00008000ul
#define dctPartVariant    0x00010000ul
#define dctArea           0x00020000ul
#define dctSelector       0x00040000ul
#define dctPadAssociation 0x00080000ul
#define dctTraceRoad      0x00100000ul
#define dctTracePolygon   0x00200000ul
#define dctTraceVia       0x00400000ul
#define dctRich           0x00800000ul
#define dct3D             0x01000000ul
#define dctValue          0x02000000ul
#define dctVariant        0x04000000ul
#define dctParam          0x08000000ul

#define dctLocal          0x80000000ul //This flag setup for objects which must not be send to global storage


#define dctPicture       (dctLines | dctText)
#define dctProjectItems  (dctSymbol | dctPart | dctSheet | dctPlate | dctComponent | dctRich)
#define dctAll           MAX64_MASK
#define dctSymbolObjects (dctPicture | dctSymPin | dctIdent | dctValue )
#define dctPartObjects   (dctPicture | dctPartPin | dctIdent | dctValue )
#define dctSheetObjects  (dctPicture | dctSymImp | dctNetWire | dctNetName | dctParam)
#define dctPlateObjects  (dctPicture | dctPartImp | dctTraceVia | dctTraceRoad | dctTracePolygon )
#define dctTraced        (dctPartImp | dctTraceVia | dctTraceRoad | dctTracePolygon )

typedef quint64 SdClass;

#endif // SDCLASS_H
