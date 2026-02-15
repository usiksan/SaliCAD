/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdLayer.h"
#include "SdEnvir.h"

#include <QCoreApplication>

SdLayerDescr sdLayerDescrDefault[] = {
  // ================================================================
  // Common layers (both Schematic and PCB, without LID1_...)
  // ================================================================
  //   mClass       mId                               mColor      mTrace                mStratum
      { dctCommon, LID0_COMMON,                  0x00404040, layerTraceNone,         stmThrough }, // 0   Common objects
      { dctCommon, LID0_INVISIBLE,               0xff007f7f, layerTraceNone,         stmThrough }, // 1   Invisible objects
      { dctCommon, LID0_ERRORS,                  0xff0000ff, layerTraceNone,         stmThrough }, // 2   Errors display
      { dctCommon, LID0_PICTURE,                 0x0000ff00, layerTraceNone,         stmThrough }, // 3   Arbitrary drawing
      { dctCommon, LID0_REMARK,                  0x00ff2000, layerTraceNone,         stmThrough }, // 4   Comment

  // ================================================================
  // Schematic specific layers (without LID1_...)
  // ================================================================
      { dctSchematic, LID0_NET,                  0x007fff7f, layerTraceNone,         stmThrough }, // 5   Circuit graphics
      { dctSchematic, LID0_NET_NAME,             0x007f7f00, layerTraceNone,         stmThrough }, // 6   Net name graphics
      { dctSchematic, LID0_BUS,                  0xff0000ff, layerTraceNone,         stmThrough }, // 7   Bus graphics
      { dctSchematic, LID0_AREA,                 0x007f7fff, layerTraceNone,         stmThrough }, // 8   PCB area in schematic
      { dctSchematic, LID0_FORM,                 0x007f7f7f, layerTraceNone,         stmThrough }, // 9   Drawing forms and frames
      { dctSchematic, LID0_NAVI,                 0x00303030, layerTraceNone,         stmThrough }, // 10  Navigation grid

  // ================================================================
  // PCB specific layers (without LID1_...)
  // ================================================================
      { dctConstruct, LID0_PCB,                  0x007fff00, layerTraceBoundary,     stmThrough }, // 11  PCB contour
      { dctConstruct, LID0_HOLE,                 0x00bf4000, layerTraceHole,         stmThrough }, // 12  Hole
      { dctConstruct, LID0_EXCLUSION,            0x00002080, layerTraceKeepout,      stmThrough }, // 13  Trace exclusion area (through)
      { dctConstruct, LID0_CLEAR,                0x803f3f00, layerTraceNone,         stmThrough }, // 14  Clearance (through)

  // ================================================================
  // Both Schematic and PCB layers (with LID1_... for PCB)
  // ================================================================
      // Schematic only (no LID1)
      { dctSchematic, LID0_COMPONENT,            0x00ff0000, layerTraceNone,         stmThrough }, // 15  Component symbol
      { dctSchematic, LID0_PIN,                  0x0000ff00, layerTraceNone,         stmThrough }, // 16  Pin graphics
      { dctSchematic, LID0_PIN_NAME,             0x000000ff, layerTraceNone,         stmThrough }, // 17  Pin name
      { dctSchematic, LID0_PIN_NUMBER,           0x000020ff, layerTraceNone,         stmThrough }, // 18  Pin number
      { dctSchematic, LID0_IDENT,                0x00ff7f00, layerTraceNone,         stmThrough }, // 19  Identifier
      { dctSchematic, LID0_VALUE,                0x00404000, layerTraceNone,         stmThrough }, // 20  Value

      // PCB with TOP/BOT
      { dctConstruct, LID0_COMPONENT LID1_TOP,   0x00ff0000, layerTraceNone,         stmTop    }, // 21  Component footprint Top
      { dctConstruct, LID0_COMPONENT LID1_BOT,   0x000000ff, layerTraceNone,         stmBottom }, // 22  Component footprint Bottom
      { dctConstruct, LID0_PIN LID1_TOP,         0x0000ff00, layerTraceNone,         stmTop    }, // 23  Pin Top
      { dctConstruct, LID0_PIN LID1_BOT,         0x00008040, layerTraceNone,         stmBottom }, // 24  Pin Bottom
      { dctConstruct, LID0_PIN_NAME LID1_TOP,    0x000000ff, layerTraceNone,         stmTop    }, // 25  Pin name Top
      { dctConstruct, LID0_PIN_NAME LID1_BOT,    0x00002080, layerTraceNone,         stmBottom }, // 26  Pin name Bottom
      { dctConstruct, LID0_PIN_NUMBER LID1_TOP,  0x000020ff, layerTraceNone,         stmTop    }, // 27  Pin number Top
      { dctConstruct, LID0_PIN_NUMBER LID1_BOT,  0x0000a040, layerTraceNone,         stmBottom }, // 28  Pin number Bottom
      { dctConstruct, LID0_IDENT LID1_TOP,       0x00ff7f00, layerTraceNone,         stmTop    }, // 29  Identifier Top
      { dctConstruct, LID0_IDENT LID1_BOT,       0x00207f00, layerTraceNone,         stmBottom }, // 30  Identifier Bottom
      { dctConstruct, LID0_VALUE LID1_TOP,       0x00404000, layerTraceNone,         stmTop    }, // 31  Value Top
      { dctConstruct, LID0_VALUE LID1_BOT,       0x0040407f, layerTraceNone,         stmBottom }, // 32  Value Bottom

  // ================================================================
  // PCB specific layers with LID1_TOP, LID1_BOT
  // ================================================================
      { dctConstruct, LID0_WIRE LID1_TOP,        0x00cf0000, layerTraceRoad,         stmTop    }, // 33  Wire Top
      { dctConstruct, LID0_WIRE LID1_BOT,        0x000000cf, layerTraceRoad,         stmBottom }, // 34  Wire Bottom

      { dctConstruct, LID0_POLYGON LID1_TOP,     0x808f0000, layerTracePolygon,      stmTop    }, // 35  Polygon Top
      { dctConstruct, LID0_POLYGON LID1_BOT,     0x8000008f, layerTracePolygon,      stmBottom }, // 36  Polygon Bottom

      { dctConstruct, LID0_PAD LID1_TOP,         0x00bf0000, layerTracePad,          stmTop    }, // 37  Pad Top
      { dctConstruct, LID0_PAD LID1_BOT,         0x000000bf, layerTracePad,          stmBottom }, // 38  Pad Bottom

      { dctConstruct, LID0_EXCLUSION LID1_TOP,   0x00002080, layerTraceKeepout,      stmTop    }, // 41  Exclusion Top
      { dctConstruct, LID0_EXCLUSION LID1_BOT,   0x00002080, layerTraceKeepout,      stmBottom }, // 42  Exclusion Bottom

      { dctConstruct, LID0_SOLDER_MASK LID1_TOP, 0x803fff00, layerTraceMask,         stmTop    }, // 43  Solder mask Top
      { dctConstruct, LID0_SOLDER_MASK LID1_BOT, 0x8040007f, layerTraceMask,         stmBottom }, // 44  Solder mask Bottom

      { dctConstruct, LID0_STENCIL LID1_TOP,     0x003f8000, layerTraceStencil,      stmTop    }, // 45  Stencil Top
      { dctConstruct, LID0_STENCIL LID1_BOT,     0x003f007f, layerTraceStencil,      stmBottom }, // 46  Stencil Bottom

      { dctConstruct, LID0_STENCIL_REPER LID1_TOP, 0x003f4000, layerTraceNone,       stmTop    }, // 47  Stencil fiducial Top
      { dctConstruct, LID0_STENCIL_REPER LID1_BOT, 0x003f007f, layerTraceNone,       stmBottom }, // 48  Stencil fiducial Bottom

      { dctConstruct, LID0_SILK LID1_TOP,        0x3f3f8000, layerTraceNone,         stmTop    }, // 49  Silkscreen Top
      { dctConstruct, LID0_SILK LID1_BOT,        0x3f3f8000, layerTraceNone,         stmBottom }, // 50  Silkscreen Bottom

      { dctConstruct, LID0_ADHESIV LID1_TOP,     0x3f7f3f00, layerTraceNone,         stmTop    }, // 51  Adhesive Top
      { dctConstruct, LID0_ADHESIV LID1_BOT,     0x3f7f3f00, layerTraceNone,         stmBottom }, // 52  Adhesive Bottom

      { dctConstruct, LID0_DIM LID1_TOP,         0x0000ffff, layerTraceNone,         stmTop    }, // 53  Dimension Top
      { dctConstruct, LID0_DIM LID1_BOT,         0x00ffff00, layerTraceNone,         stmBottom }, // 54  Dimension Bottom

      { dctConstruct, LID0_GUIDE LID1_TOP,       0x00ff7f00, layerTraceNone,         stmTop    }, // 55  Guide lines Top
      { dctConstruct, LID0_GUIDE LID1_BOT,       0x007fff00, layerTraceNone,         stmBottom }, // 56  Guide lines Bottom

      { dctConstruct, LID0_COURTYARD LID1_TOP,   0x3f3f3f00, layerTraceNone,         stmTop    }, // 57  Courtyard Top
      { dctConstruct, LID0_COURTYARD LID1_BOT,   0x3f3f3f00, layerTraceNone,         stmBottom }, // 58  Courtyard Bottom

  // ================================================================
  // Inner layers INT00 - INT07
  // ================================================================
      { dctConstruct, LID0_WIRE    LID1_INT02,   0x000040cf, layerTraceRoad,         stmInt02  }, // 59  Wire Inner 0
      { dctConstruct, LID0_PAD     LID1_INT02,   0x000040cf, layerTracePad,          stmInt02  }, // 60  Pad Inner 0
      { dctConstruct, LID0_POLYGON LID1_INT02,   0x000040cf, layerTracePolygon,      stmInt02  }, // 61  Polygon Inner 0
      { dctConstruct, LID0_EXCLUSION LID1_INT02, 0x00002080, layerTraceKeepout,      stmInt02  }, // 63  Exclusion Inner 0
      { dctConstruct, LID0_HOLE    LID1_INT02,   0x00bf4000, layerTraceNone,         stmInt02  }, // Hole between TOP and INT00

      { dctConstruct, LID0_WIRE    LID1_INT03,   0x004040cf, layerTraceRoad,         stmInt03  }, // 64  Wire Inner 1
      { dctConstruct, LID0_PAD     LID1_INT03,   0x004040cf, layerTracePad,          stmInt03  }, // 65  Pad Inner 1
      { dctConstruct, LID0_POLYGON LID1_INT03,   0x004040cf, layerTracePolygon,      stmInt03  }, // 66  Polygon Inner 1
      { dctConstruct, LID0_EXCLUSION LID1_INT03, 0x00002080, layerTraceKeepout,      stmInt03  }, // 68  Exclusion Inner 1
      { dctConstruct, LID0_HOLE    LID1_INT03,   0x00bf4000, layerTraceNone,         stmInt03  }, // Hole between INT01 and INT02

      { dctConstruct, LID0_WIRE    LID1_INT04,   0x004000cf, layerTraceRoad,         stmInt04  }, // 69  Wire Inner 2
      { dctConstruct, LID0_PAD     LID1_INT04,   0x004000cf, layerTracePad,          stmInt04  }, // 70  Pad Inner 2
      { dctConstruct, LID0_POLYGON LID1_INT04,   0x004000cf, layerTracePolygon,      stmInt04  }, // 71  Polygon Inner 2
      { dctConstruct, LID0_EXCLUSION LID1_INT04, 0x00002080, layerTraceKeepout,      stmInt04  }, // 73  Exclusion Inner 2

      { dctConstruct, LID0_WIRE    LID1_INT05,   0x00cf4040, layerTraceRoad,         stmInt05  }, // 74  Wire Inner 3
      { dctConstruct, LID0_PAD     LID1_INT05,   0x00cf4040, layerTracePad,          stmInt05  }, // 75  Pad Inner 3
      { dctConstruct, LID0_POLYGON LID1_INT05,   0x00cf4040, layerTracePolygon,      stmInt05  }, // 76  Polygon Inner 3
      { dctConstruct, LID0_EXCLUSION LID1_INT05, 0x00002080, layerTraceKeepout,      stmInt05  }, // 78  Exclusion Inner 3
      { dctConstruct, LID0_HOLE    LID1_INT05,   0x00bf4000, layerTraceNone,         stmInt05  }, // Hole between INT03 and INT04

      { dctConstruct, LID0_WIRE    LID1_INT06,   0x00cf4080, layerTraceRoad,         stmInt06  }, // 79  Wire Inner 4
      { dctConstruct, LID0_PAD     LID1_INT06,   0x00cf4080, layerTracePad,          stmInt06  }, // 80  Pad Inner 4
      { dctConstruct, LID0_POLYGON LID1_INT06,   0x00cf4080, layerTracePolygon,      stmInt06  }, // 81  Polygon Inner 4
      { dctConstruct, LID0_EXCLUSION LID1_INT06, 0x00002080, layerTraceKeepout,      stmInt06  }, // 83  Exclusion Inner 4

      { dctConstruct, LID0_WIRE    LID1_INT07,   0x00cf8040, layerTraceRoad,         stmInt07  }, // 84  Wire Inner 5
      { dctConstruct, LID0_PAD     LID1_INT07,   0x00cf8040, layerTracePad,          stmInt07  }, // 85  Pad Inner 5
      { dctConstruct, LID0_POLYGON LID1_INT07,   0x00cf8040, layerTracePolygon,      stmInt07  }, // 86  Polygon Inner 5
      { dctConstruct, LID0_EXCLUSION LID1_INT07, 0x00002080, layerTraceKeepout,      stmInt07  }, // 88  Exclusion Inner 5
      { dctConstruct, LID0_HOLE    LID1_INT07,   0x00bf4000, layerTraceNone,         stmInt07  }, // Hole between INT05 and INT06

      { dctConstruct, LID0_WIRE    LID1_INT08,   0x00cf8080, layerTraceRoad,         stmInt08  }, // 89  Wire Inner 6
      { dctConstruct, LID0_PAD     LID1_INT08,   0x00cf8080, layerTracePad,          stmInt08  }, // 90  Pad Inner 6
      { dctConstruct, LID0_POLYGON LID1_INT08,   0x00cf8080, layerTracePolygon,      stmInt08  }, // 91  Polygon Inner 6
      { dctConstruct, LID0_EXCLUSION LID1_INT08, 0x00002080, layerTraceKeepout,      stmInt08  }, // 93  Exclusion Inner 6

      { dctConstruct, LID0_WIRE    LID1_INT09,   0x00cfc040, layerTraceRoad,         stmInt09  }, // 94  Wire Inner 7
      { dctConstruct, LID0_PAD     LID1_INT09,   0x00cfc040, layerTracePad,          stmInt09  }, // 95  Pad Inner 7
      { dctConstruct, LID0_POLYGON LID1_INT09,   0x00cfc040, layerTracePolygon,      stmInt09  }, // 96  Polygon Inner 7
      { dctConstruct, LID0_EXCLUSION LID1_INT09, 0x00002080, layerTraceKeepout,      stmInt09  }, // 98  Exclusion Inner 7
      { dctConstruct, LID0_HOLE    LID1_INT09,   0x00bf4000, layerTraceNone,         stmInt09  }, // Hole between INT07 and INT08

  // ================================================================
  // Inner layers INT08 - INT27
  // ================================================================
      { dctConstruct, LID0_WIRE    LID1_INT10,   0x00cfc080, layerTraceRoad,         stmInt10  }, // 99  Wire Inner 8
      { dctConstruct, LID0_PAD     LID1_INT10,   0x00cfc080, layerTracePad,          stmInt10  }, // 100 Pad Inner 8
      { dctConstruct, LID0_POLYGON LID1_INT10,   0x00cfc080, layerTracePolygon,      stmInt10  }, // 101 Polygon Inner 8
      { dctConstruct, LID0_EXCLUSION LID1_INT10, 0x00002080, layerTraceKeepout,      stmInt10  }, // 103 Exclusion Inner 8

      { dctConstruct, LID0_WIRE    LID1_INT11,   0x00cfc0c0, layerTraceRoad,         stmInt11  }, // 104 Wire Inner 9
      { dctConstruct, LID0_PAD     LID1_INT11,   0x00cfc0c0, layerTracePad,          stmInt11  }, // 105 Pad Inner 9
      { dctConstruct, LID0_POLYGON LID1_INT11,   0x00cfc0c0, layerTracePolygon,      stmInt11  }, // 106 Polygon Inner 9
      { dctConstruct, LID0_EXCLUSION LID1_INT11, 0x00002080, layerTraceKeepout,      stmInt11  }, // 108 Exclusion Inner 9
      { dctConstruct, LID0_HOLE    LID1_INT11,   0x00bf4000, layerTraceNone,         stmInt11  }, // Hole between INT09 and INT10

      { dctConstruct, LID0_WIRE    LID1_INT12,   0x00cf00c0, layerTraceRoad,         stmInt12  }, // 109 Wire Inner 10
      { dctConstruct, LID0_PAD     LID1_INT12,   0x00cf00c0, layerTracePad,          stmInt12  }, // 110 Pad Inner 10
      { dctConstruct, LID0_POLYGON LID1_INT12,   0x00cf00c0, layerTracePolygon,      stmInt12  }, // 111 Polygon Inner 10
      { dctConstruct, LID0_EXCLUSION LID1_INT12, 0x00002080, layerTraceKeepout,      stmInt12  }, // 113 Exclusion Inner 10

      { dctConstruct, LID0_WIRE    LID1_INT13,   0x00cf40c0, layerTraceRoad,         stmInt13  }, // 114 Wire Inner 11
      { dctConstruct, LID0_PAD     LID1_INT13,   0x00cf40c0, layerTracePad,          stmInt13  }, // 115 Pad Inner 11
      { dctConstruct, LID0_POLYGON LID1_INT13,   0x00cf40c0, layerTracePolygon,      stmInt13  }, // 116 Polygon Inner 11
      { dctConstruct, LID0_EXCLUSION LID1_INT13, 0x00002080, layerTraceKeepout,      stmInt13  }, // 118 Exclusion Inner 11
      { dctConstruct, LID0_HOLE    LID1_INT13,   0x00bf4000, layerTraceNone,         stmInt13  }, // Hole between INT11 and INT12

      { dctConstruct, LID0_WIRE    LID1_INT14,   0x00cf80c0, layerTraceRoad,         stmInt14  }, // 119 Wire Inner 12
      { dctConstruct, LID0_PAD     LID1_INT14,   0x00cf80c0, layerTracePad,          stmInt14  }, // 120 Pad Inner 12
      { dctConstruct, LID0_POLYGON LID1_INT14,   0x00cf80c0, layerTracePolygon,      stmInt14  }, // 121 Polygon Inner 12
      { dctConstruct, LID0_EXCLUSION LID1_INT14, 0x00002080, layerTraceKeepout,      stmInt14  }, // 123 Exclusion Inner 12

      { dctConstruct, LID0_WIRE    LID1_INT15,   0x000080c0, layerTraceRoad,         stmInt15  }, // 124 Wire Inner 13
      { dctConstruct, LID0_PAD     LID1_INT15,   0x000080c0, layerTracePad,          stmInt15  }, // 125 Pad Inner 13
      { dctConstruct, LID0_POLYGON LID1_INT15,   0x000080c0, layerTracePolygon,      stmInt15  }, // 126 Polygon Inner 13
      { dctConstruct, LID0_EXCLUSION LID1_INT15, 0x00002080, layerTraceKeepout,      stmInt15  }, // 128 Exclusion Inner 13
      { dctConstruct, LID0_HOLE    LID1_INT15,   0x00bf4000, layerTraceNone,         stmInt15  }, // Hole between INT13 and INT14

      { dctConstruct, LID0_WIRE    LID1_INT16,   0x004080c0, layerTraceRoad,         stmInt16  }, // 129 Wire Inner 14
      { dctConstruct, LID0_PAD     LID1_INT16,   0x004080c0, layerTracePad,          stmInt16  }, // 130 Pad Inner 14
      { dctConstruct, LID0_POLYGON LID1_INT16,   0x004080c0, layerTracePolygon,      stmInt16  }, // 131 Polygon Inner 14
      { dctConstruct, LID0_EXCLUSION LID1_INT16, 0x00002080, layerTraceKeepout,      stmInt16  }, // 133 Exclusion Inner 14

      { dctConstruct, LID0_WIRE    LID1_INT17,   0x008080c0, layerTraceRoad,         stmInt17  }, // 134 Wire Inner 15
      { dctConstruct, LID0_PAD     LID1_INT17,   0x008080c0, layerTracePad,          stmInt17  }, // 135 Pad Inner 15
      { dctConstruct, LID0_POLYGON LID1_INT17,   0x008080c0, layerTracePolygon,      stmInt17  }, // 136 Polygon Inner 15
      { dctConstruct, LID0_EXCLUSION LID1_INT17, 0x00002080, layerTraceKeepout,      stmInt17  }, // 138 Exclusion Inner 15
      { dctConstruct, LID0_HOLE    LID1_INT17,   0x00bf4000, layerTraceNone,         stmInt17  }, // Hole between INT15 and INT16

      { dctConstruct, LID0_WIRE    LID1_INT18,   0x0080c0c0, layerTraceRoad,         stmInt18  }, // 139 Wire Inner 16
      { dctConstruct, LID0_PAD     LID1_INT18,   0x0080c0c0, layerTracePad,          stmInt18  }, // 140 Pad Inner 16
      { dctConstruct, LID0_POLYGON LID1_INT18,   0x0080c0c0, layerTracePolygon,      stmInt18  }, // 141 Polygon Inner 16
      { dctConstruct, LID0_EXCLUSION LID1_INT18, 0x00002080, layerTraceKeepout,      stmInt18  }, // 143 Exclusion Inner 16

      { dctConstruct, LID0_WIRE    LID1_INT19,   0x0040c0c0, layerTraceRoad,         stmInt19  }, // 144 Wire Inner 17
      { dctConstruct, LID0_PAD     LID1_INT19,   0x0040c0c0, layerTracePad,          stmInt19  }, // 145 Pad Inner 17
      { dctConstruct, LID0_POLYGON LID1_INT19,   0x0040c0c0, layerTracePolygon,      stmInt19  }, // 146 Polygon Inner 17
      { dctConstruct, LID0_EXCLUSION LID1_INT19, 0x00002080, layerTraceKeepout,      stmInt19  }, // 148 Exclusion Inner 17
      { dctConstruct, LID0_HOLE    LID1_INT19,   0x00bf4000, layerTraceNone,         stmInt19  }, // Hole between INT17 and INT18

      { dctConstruct, LID0_WIRE    LID1_INT20,   0x0040c080, layerTraceRoad,         stmInt20  }, // 149 Wire Inner 18
      { dctConstruct, LID0_PAD     LID1_INT20,   0x0040c080, layerTracePad,          stmInt20  }, // 150 Pad Inner 18
      { dctConstruct, LID0_POLYGON LID1_INT20,   0x0040c080, layerTracePolygon,      stmInt20  }, // 151 Polygon Inner 18
      { dctConstruct, LID0_EXCLUSION LID1_INT20, 0x00002080, layerTraceKeepout,      stmInt20  }, // 153 Exclusion Inner 18

      { dctConstruct, LID0_WIRE    LID1_INT21,   0x0040c040, layerTraceRoad,         stmInt21  }, // 154 Wire Inner 19
      { dctConstruct, LID0_PAD     LID1_INT21,   0x0040c040, layerTracePad,          stmInt21  }, // 155 Pad Inner 19
      { dctConstruct, LID0_POLYGON LID1_INT21,   0x0040c040, layerTracePolygon,      stmInt21  }, // 156 Polygon Inner 19
      { dctConstruct, LID0_EXCLUSION LID1_INT21, 0x00002080, layerTraceKeepout,      stmInt21  }, // 158 Exclusion Inner 19
      { dctConstruct, LID0_HOLE    LID1_INT21,   0x00bf4000, layerTraceNone,         stmInt21  }, // Hole between INT19 and INT20

      { dctConstruct, LID0_WIRE    LID1_INT22,   0x00408040, layerTraceRoad,         stmInt22  }, // 159 Wire Inner 20
      { dctConstruct, LID0_PAD     LID1_INT22,   0x00408040, layerTracePad,          stmInt22  }, // 160 Pad Inner 20
      { dctConstruct, LID0_POLYGON LID1_INT22,   0x00408040, layerTracePolygon,      stmInt22  }, // 161 Polygon Inner 20
      { dctConstruct, LID0_EXCLUSION LID1_INT22, 0x00002080, layerTraceKeepout,      stmInt22  }, // 163 Exclusion Inner 20

      { dctConstruct, LID0_WIRE    LID1_INT23,   0x00408080, layerTraceRoad,         stmInt23  }, // 164 Wire Inner 21
      { dctConstruct, LID0_PAD     LID1_INT23,   0x00408080, layerTracePad,          stmInt23  }, // 165 Pad Inner 21
      { dctConstruct, LID0_POLYGON LID1_INT23,   0x00408080, layerTracePolygon,      stmInt23  }, // 166 Polygon Inner 21
      { dctConstruct, LID0_EXCLUSION LID1_INT23, 0x00002080, layerTraceKeepout,      stmInt23  }, // 168 Exclusion Inner 21
      { dctConstruct, LID0_HOLE    LID1_INT23,   0x00bf4000, layerTraceNone,         stmInt23  }, // Hole between INT21 and INT22

      { dctConstruct, LID0_WIRE    LID1_INT24,   0x004080c0, layerTraceRoad,         stmInt24  }, // 169 Wire Inner 22
      { dctConstruct, LID0_PAD     LID1_INT24,   0x004080c0, layerTracePad,          stmInt24  }, // 170 Pad Inner 22
      { dctConstruct, LID0_POLYGON LID1_INT24,   0x004080c0, layerTracePolygon,      stmInt24  }, // 171 Polygon Inner 22
      { dctConstruct, LID0_EXCLUSION LID1_INT24, 0x00002080, layerTraceKeepout,      stmInt24  }, // 173 Exclusion Inner 22

      { dctConstruct, LID0_WIRE    LID1_INT25,   0x004040c0, layerTraceRoad,         stmInt25  }, // 174 Wire Inner 23
      { dctConstruct, LID0_PAD     LID1_INT25,   0x004040c0, layerTracePad,          stmInt25  }, // 175 Pad Inner 23
      { dctConstruct, LID0_POLYGON LID1_INT25,   0x004040c0, layerTracePolygon,      stmInt25  }, // 176 Polygon Inner 23
      { dctConstruct, LID0_EXCLUSION LID1_INT25, 0x00002080, layerTraceKeepout,      stmInt25  }, // 178 Exclusion Inner 23
      { dctConstruct, LID0_HOLE    LID1_INT25,   0x00bf4000, layerTraceNone,         stmInt25  }, // Hole between INT23 and INT24

      { dctConstruct, LID0_WIRE    LID1_INT26,   0x00404080, layerTraceRoad,         stmInt26  }, // 179 Wire Inner 24
      { dctConstruct, LID0_PAD     LID1_INT26,   0x00404080, layerTracePad,          stmInt26  }, // 180 Pad Inner 24
      { dctConstruct, LID0_POLYGON LID1_INT26,   0x00404080, layerTracePolygon,      stmInt26  }, // 181 Polygon Inner 24
      { dctConstruct, LID0_EXCLUSION LID1_INT26, 0x00002080, layerTraceKeepout,      stmInt26  }, // 183 Exclusion Inner 24

      { dctConstruct, LID0_WIRE    LID1_INT27,   0x00404040, layerTraceRoad,         stmInt27  }, // 184 Wire Inner 25
      { dctConstruct, LID0_PAD     LID1_INT27,   0x00404040, layerTracePad,          stmInt27  }, // 185 Pad Inner 25
      { dctConstruct, LID0_POLYGON LID1_INT27,   0x00404040, layerTracePolygon,      stmInt27  }, // 186 Polygon Inner 25
      { dctConstruct, LID0_EXCLUSION LID1_INT27, 0x00002080, layerTraceKeepout,      stmInt27  }, // 188 Exclusion Inner 25
      { dctConstruct, LID0_HOLE    LID1_INT27,   0x00bf4000, layerTraceNone,         stmInt27  }, // Hole between INT25 and INT26

      { dctConstruct, LID0_WIRE    LID1_INT28,   0x00804040, layerTraceRoad,         stmInt28  }, // 189 Wire Inner 26
      { dctConstruct, LID0_PAD     LID1_INT28,   0x00804040, layerTracePad,          stmInt28  }, // 190 Pad Inner 26
      { dctConstruct, LID0_POLYGON LID1_INT28,   0x00804040, layerTracePolygon,      stmInt28  }, // 191 Polygon Inner 26
      { dctConstruct, LID0_EXCLUSION LID1_INT28, 0x00002080, layerTraceKeepout,      stmInt28  }, // 193 Exclusion Inner 26

      { dctConstruct, LID0_WIRE    LID1_INT29,   0x0080f040, layerTraceRoad,         stmInt29  }, // 194 Wire Inner 27
      { dctConstruct, LID0_PAD     LID1_INT29,   0x0080f040, layerTracePad,          stmInt29  }, // 195 Pad Inner 27
      { dctConstruct, LID0_POLYGON LID1_INT29,   0x0080f040, layerTracePolygon,      stmInt29  }, // 196 Polygon Inner 27
      { dctConstruct, LID0_EXCLUSION LID1_INT29, 0x00002080, layerTraceKeepout,      stmInt29  }, // 198 Exclusion Inner 27
      { dctConstruct, LID0_HOLE    LID1_INT29,   0x00bf4000, layerTraceNone,         stmInt29  }, // Hole between INT27 and BOT

      { 0, nullptr, 0, layerTraceNone, 0x00000000 }
  };



SdLayerLevel sdLayerLevel0[] = {
  // Common layers
  { LID0_COMMON,        QT_TRANSLATE_NOOP( "SdLayer", "Common" ),                   layerTraceNone,     dctCommon },
  { LID0_INVISIBLE,     QT_TRANSLATE_NOOP( "SdLayer", "Invisible" ),                layerTraceNone,     dctCommon },
  { LID0_ERRORS,        QT_TRANSLATE_NOOP( "SdLayer", "Errors" ),                   layerTraceNone,     dctCommon },
  { LID0_PICTURE,       QT_TRANSLATE_NOOP( "SdLayer", "Picture" ),                  layerTraceNone,     dctCommon },
  { LID0_REMARK,        QT_TRANSLATE_NOOP( "SdLayer", "Remark" ),                   layerTraceNone,     dctCommon },

  // Schematic specific
  { LID0_NET,           QT_TRANSLATE_NOOP( "SdLayer", "Schematic net" ),            layerTraceNone,     dctSchematic },
  { LID0_NET_NAME,      QT_TRANSLATE_NOOP( "SdLayer", "Schematic net name" ),       layerTraceNone,     dctSchematic },
  { LID0_BUS,           QT_TRANSLATE_NOOP( "SdLayer", "Schematic net bus" ),        layerTraceNone,     dctSchematic },
  { LID0_AREA,          QT_TRANSLATE_NOOP( "SdLayer", "Schematic PCB area contour" ), layerTraceNone,   dctSchematic },
  { LID0_FORM,          QT_TRANSLATE_NOOP( "SdLayer", "Drawing form and frame" ),   layerTraceNone,     dctSchematic },
  { LID0_NAVI,          QT_TRANSLATE_NOOP( "SdLayer", "Navigation grid" ),          layerTraceNone,     dctSchematic },

  // Both schematic and PCB
  { LID0_COMPONENT,     QT_TRANSLATE_NOOP( "SdLayer", "Component" ),                layerTraceNone,     dctCommon },
  { LID0_PIN,           QT_TRANSLATE_NOOP( "SdLayer", "Pin" ),                      layerTraceNone,     dctCommon },
  { LID0_PIN_NAME,      QT_TRANSLATE_NOOP( "SdLayer", "Pin name" ),                 layerTraceNone,     dctCommon },
  { LID0_PIN_NUMBER,    QT_TRANSLATE_NOOP( "SdLayer", "Pin number" ),               layerTraceNone,     dctCommon },
  { LID0_IDENT,         QT_TRANSLATE_NOOP( "SdLayer", "Component ident" ),          layerTraceNone,     dctCommon },
  { LID0_VALUE,         QT_TRANSLATE_NOOP( "SdLayer", "Value" ),                    layerTraceNone,     dctCommon },

  // PCB specific (without LID1)
  { LID0_PCB,           QT_TRANSLATE_NOOP( "SdLayer", "PCB contour" ),              layerTraceBoundary, dctConstruct },
  { LID0_HOLE,          QT_TRANSLATE_NOOP( "SdLayer", "PCB hole" ),                 layerTraceHole,     dctConstruct },
  { LID0_EXCLUSION,     QT_TRANSLATE_NOOP( "SdLayer", "PCB trace exclusion area" ), layerTraceKeepout,  dctConstruct },
  { LID0_CLEAR,         QT_TRANSLATE_NOOP( "SdLayer", "PCB trace clear" ),          layerTraceNone,     dctConstruct },

  // PCB specific (with LID1)
  { LID0_WIRE,          QT_TRANSLATE_NOOP( "SdLayer", "PCB wire" ),                 layerTraceRoad,     dctConstruct },
  { LID0_POLYGON,       QT_TRANSLATE_NOOP( "SdLayer", "PCB polygon" ),              layerTracePolygon,  dctConstruct },
  { LID0_PAD,           QT_TRANSLATE_NOOP( "SdLayer", "PCB pad" ),                  layerTracePad,      dctConstruct },
  { LID0_SOLDER_MASK,   QT_TRANSLATE_NOOP( "SdLayer", "PCB solder mask" ),          layerTraceNone,     dctConstruct },
  { LID0_STENCIL,       QT_TRANSLATE_NOOP( "SdLayer", "PCB stencil aperture" ),     layerTraceNone,     dctConstruct },
  { LID0_STENCIL_REPER, QT_TRANSLATE_NOOP( "SdLayer", "PCB stencil fiducial" ),     layerTraceNone,     dctConstruct },
  { LID0_SILK,          QT_TRANSLATE_NOOP( "SdLayer", "PCB silkscreen" ),           layerTraceNone,     dctConstruct },
  { LID0_ADHESIV,       QT_TRANSLATE_NOOP( "SdLayer", "PCB adhesive" ),             layerTraceNone,     dctConstruct },
  { LID0_DIM,           QT_TRANSLATE_NOOP( "SdLayer", "PCB dimension" ),            layerTraceNone,     dctConstruct },
  { LID0_GUIDE,         QT_TRANSLATE_NOOP( "SdLayer", "PCB construction guide" ),   layerTraceNone,     dctConstruct },
  { LID0_COURTYARD,     QT_TRANSLATE_NOOP( "SdLayer", "PCB component courtyard" ),  layerTraceNone,     dctConstruct },

  { nullptr, nullptr, layerTraceNone, 0 }
};


SdLayerLevel sdLayerLevel1[] = {
  { "",                 QT_TRANSLATE_NOOP( "SdLayer", "---" ),            layerTraceNone,stmThrough },
  { LID1_TOP,           QT_TRANSLATE_NOOP( "SdLayer", "on top" ),         layerTraceNone,stmTop },
  { LID1_BOT,           QT_TRANSLATE_NOOP( "SdLayer", "on bottom" ),      layerTraceNone,stmBottom },
  { LID1_INT02,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 2" ),  layerTraceNone,stmInt02 },
  { LID1_INT03,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 3" ),  layerTraceNone,stmInt03 },
  { LID1_INT04,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 4" ),  layerTraceNone,stmInt04 },
  { LID1_INT05,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 5" ),  layerTraceNone,stmInt05 },
  { LID1_INT06,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 6" ),  layerTraceNone,stmInt06 },
  { LID1_INT07,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 7" ),  layerTraceNone,stmInt07 },
  { LID1_INT08,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 8" ),  layerTraceNone,stmInt08 },
  { LID1_INT09,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 9" ),  layerTraceNone,stmInt09 },
  { LID1_INT10,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 10" ), layerTraceNone,stmInt10 },
  { LID1_INT11,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 11" ), layerTraceNone,stmInt11 },
  { LID1_INT12,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 12" ), layerTraceNone,stmInt12 },
  { LID1_INT13,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 13" ), layerTraceNone,stmInt13 },
  { LID1_INT14,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 14" ), layerTraceNone,stmInt14 },
  { LID1_INT15,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 15" ), layerTraceNone,stmInt15 },
  { LID1_INT16,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 16" ), layerTraceNone,stmInt16 },
  { LID1_INT17,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 17" ), layerTraceNone,stmInt17 },
  { LID1_INT18,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 18" ), layerTraceNone,stmInt18 },
  { LID1_INT19,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 19" ), layerTraceNone,stmInt19 },
  { LID1_INT20,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 20" ), layerTraceNone,stmInt20 },
  { LID1_INT21,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 21" ), layerTraceNone,stmInt21 },
  { LID1_INT22,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 22" ), layerTraceNone,stmInt22 },
  { LID1_INT23,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 23" ), layerTraceNone,stmInt23 },
  { LID1_INT24,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 24" ), layerTraceNone,stmInt24 },
  { LID1_INT25,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 25" ), layerTraceNone,stmInt25 },
  { LID1_INT26,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 26" ), layerTraceNone,stmInt26 },
  { LID1_INT27,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 27" ), layerTraceNone,stmInt27 },
  { LID1_INT28,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 28" ), layerTraceNone,stmInt28 },
  { LID1_INT29,         QT_TRANSLATE_NOOP( "SdLayer", "on internal 29" ), layerTraceNone,stmInt29 },

  { LID1_CUSTOM1,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 1" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM2,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 2" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM3,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 3" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM4,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 4" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM5,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 5" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM6,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 6" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM7,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 7" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM8,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 8" ),    layerTraceNone,stmThrough },
  { LID1_CUSTOM9,       QT_TRANSLATE_NOOP( "SdLayer", "on custom 9" ),    layerTraceNone,stmThrough },

  { nullptr, nullptr, layerTraceNone,0 }
};



SdLayer::SdLayer(QString layerId, QString layerName, QString layerEnglishName, SdLayerTrace layerTrace, int layerClass, int layerStratum, unsigned layerColor, int layerIndex ) :
  mId(layerId),                //Идентификатор слоя
  mName(layerName),            //Имя слоя
  mEnglish(layerEnglishName),
  mState(layerStateEdit), //Состояние
  mTrace(layerTrace), //Разрешение трассировки на данном слое
  mClass(layerClass),
  mStratum(layerStratum),      //Позиция слоя при трассировке (верх, низ, внутри)
  mColor(layerColor),          //Цвет
  mIndex(layerIndex),
  mPair(nullptr),               //Парный слой
  mUsage(false)          //Флаг использования
  {
  //Невидимый слой выключить
  if( mId.startsWith(LID0_INVISIBLE) )
    mState = layerStateOff;
  //По умолчанию пара - этот-же слой
  //By default, pair - is same layer
  mPair = this;
  }






//Установить новую пару для слоя
void SdLayer::pairSet(SdLayer *p)
  {
  //Reset existing pair
  pairReset();
  //Set new one
  mPair = p;
  p->mPair = this;
  }




//Reset pair for layer
void SdLayer::pairReset()
  {
  //Check if pair exist
  if( mPair ) {
    //If it's - then reset existing pair
    mPair->mPair = mPair;
    mPair = this;
    }
  }




//Сохранить слой
void SdLayer::write(QDataStream &os)
  {
  os << mId
     << mName
     << mEnglish
     << static_cast<int>(mState)
     << static_cast<int>(mTrace)
     << mClass
     << mStratum
     << mColor
     << mIndex
     << mPair->mId;
  }




//Загрузить слой
void SdLayer::read(QDataStream &is)
  {
  int state, trace;
  QString pair;
  is >> mId
     >> mName
     >> mEnglish
     >> state
     >> trace
     >> mClass
     >> mStratum
     >> mColor
     >> mIndex
     >> pair;
  mState = static_cast<SdLayerState>(state);
  mTrace = static_cast<SdLayerTrace>(trace);
  mPair = SdEnvir::instance()->layerGet( pair );
  }






QPair<QString, QString> SdLayer::layerIdToName(const QString layerId, int &layerIndex, SdLayerDescr *descr)
  {
  //Build layer name
  QStringList list = layerId.split( QChar('.') );
  QString lid0 = list.at(0);
  QString lid1;

  if( list.count() > 1 )
    lid1 = QString(".") + list.at(1);

  return layerIdToName( lid0, lid1, layerIndex, descr );
  }




QPair<QString, QString> SdLayer::layerIdToName(const QString &lid0, const QString &lid1, int &layerIndex, SdLayerDescr *descr)
  {
  //Default values for descr
  if( descr != nullptr ) {
    descr->mClass = dctCommon;
    descr->mColor = 0x00743290;
    descr->mTrace = layerTraceNone;
    descr->mStratum = stmThrough;
    }
  const char *trans0 = nullptr;
  //Find for lid0
  for( int i = 0; sdLayerLevel0[i].mLid != nullptr; i++ )
    if( lid0 == QString(sdLayerLevel0[i].mLid) ) {
      trans0 = sdLayerLevel0[i].mTranslate;
      if( descr != nullptr ) {
        descr->mClass = sdLayerLevel0[i].classId();
        descr->mTrace = sdLayerLevel0[i].mTrace;
        }
      layerIndex = i;
      break;
      }

  if( trans0 == nullptr )
    trans0 = "Internal error";

  QString englishName(trans0);
  QString name( QCoreApplication::translate( "SdLayer", trans0 ) );

  if( !lid1.isEmpty() ) {
    const char *trans1 = nullptr;
    for( int i = 1; sdLayerLevel1[i].mLid != nullptr; i++ )
      if( lid1 == QString(sdLayerLevel1[i].mLid) ) {
        trans1 = sdLayerLevel1[i].mTranslate;
        if( descr != nullptr ) {
          descr->mStratum = sdLayerLevel1[i].mStratum;
          }
        layerIndex += i * 200;
        break;
        }
    if( trans1 == nullptr )
      trans1 = "Internal error";

    //English name
    englishName.append( QString(" ") );
    englishName.append( QString(trans1) );

    //Translated name
    name.append( QString(" ") );
    name.append( QCoreApplication::translate( "SdLayer", trans1 ) );
    }

  return QPair<QString,QString>( englishName, name );
  }

