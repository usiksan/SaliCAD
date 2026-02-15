#include "SdKiCad.h"
#include "objects/SdPoint.h"
#include "objects/SdPropLine.h"



struct SdKiCadLayerMapDescription
  {
    const char *mKiCadId;
    const char *mLayerId;
  };

static SdKiCadLayerMapDescription kicadLayerMapDescription[] = {
  // ================================================================
  // Copper layers (Медные слои)
  // ================================================================
  { "F.Cu", LID0_TRACE LID1_TOP },           // Верхний медный слой - трассы
  { "B.Cu", LID0_TRACE LID1_BOT },           // Нижний медный слой - трассы
  { "In1.Cu", LID0_TRACE LID1_INT02 },       // Внутренний слой 1 - трассы
  { "In2.Cu", LID0_TRACE LID1_INT03 },       // Внутренний слой 2 - трассы
  { "In3.Cu", LID0_TRACE LID1_INT04 },       // Внутренний слой 3 - трассы
  { "In4.Cu", LID0_TRACE LID1_INT05 },       // Внутренний слой 4 - трассы
  { "In5.Cu", LID0_TRACE LID1_INT06 },       // Внутренний слой 5 - трассы
  { "In6.Cu", LID0_TRACE LID1_INT07 },       // Внутренний слой 6 - трассы
  { "In7.Cu", LID0_TRACE LID1_INT08 },       // Внутренний слой 7 - трассы
  { "In8.Cu", LID0_TRACE LID1_INT09 },       // Внутренний слой 8 - трассы
  { "In9.Cu", LID0_TRACE LID1_INT10 },       // Внутренний слой 9 - трассы
  { "In10.Cu", LID0_TRACE LID1_INT11 },      // Внутренний слой 10 - трассы
  { "In11.Cu", LID0_TRACE LID1_INT12 },      // Внутренний слой 11 - трассы
  { "In12.Cu", LID0_TRACE LID1_INT13 },      // Внутренний слой 12 - трассы
  { "In13.Cu", LID0_TRACE LID1_INT14 },      // Внутренний слой 13 - трассы
  { "In14.Cu", LID0_TRACE LID1_INT15 },      // Внутренний слой 14 - трассы
  { "In15.Cu", LID0_TRACE LID1_INT16 },      // Внутренний слой 15 - трассы
  { "In16.Cu", LID0_TRACE LID1_INT17 },      // Внутренний слой 16 - трассы
  { "In17.Cu", LID0_TRACE LID1_INT18 },      // Внутренний слой 17 - трассы
  { "In18.Cu", LID0_TRACE LID1_INT19 },      // Внутренний слой 18 - трассы
  { "In19.Cu", LID0_TRACE LID1_INT20 },      // Внутренний слой 19 - трассы
  { "In20.Cu", LID0_TRACE LID1_INT21 },      // Внутренний слой 20 - трассы
  { "In21.Cu", LID0_TRACE LID1_INT22 },      // Внутренний слой 21 - трассы
  { "In22.Cu", LID0_TRACE LID1_INT23 },      // Внутренний слой 22 - трассы
  { "In23.Cu", LID0_TRACE LID1_INT24 },      // Внутренний слой 23 - трассы
  { "In24.Cu", LID0_TRACE LID1_INT25 },      // Внутренний слой 24 - трассы
  { "In25.Cu", LID0_TRACE LID1_INT26 },      // Внутренний слой 25 - трассы
  { "In26.Cu", LID0_TRACE LID1_INT27 },      // Внутренний слой 26 - трассы
  { "In27.Cu", LID0_TRACE LID1_INT28 },      // Внутренний слой 27 - трассы
  { "In28.Cu", LID0_TRACE LID1_INT29 },      // Внутренний слой 28 - трассы (макс в вашей системе)
  { "In29.Cu", nullptr },                    // Не используется (ограничение 30 слоев)
  { "In30.Cu", nullptr },                   // Не используется (ограничение 30 слоев)

  // ================================================================
  // Solder Mask / Паяльная маска
  // ================================================================
  { "F.Mask", LID0_SOLDER_MASK LID1_TOP },   // Верхняя паяльная маска
  { "B.Mask", LID0_SOLDER_MASK LID1_BOT },   // Нижняя паяльная маска

  // Внутренние слои не имеют маски в KiCad

  // ================================================================
  // Paste / Паяльная паста (трафарет)
  // ================================================================
  { "F.Paste", LID0_STENCIL LID1_TOP },      // Верхний трафарет
  { "B.Paste", LID0_STENCIL LID1_BOT },      // Нижний трафарет

  // ================================================================
  // Silkscreen / Шелкография
  // ================================================================
  { "F.SilkS", LID0_SILK LID1_TOP },         // Верхняя шелкография
  { "B.SilkS", LID0_SILK LID1_BOT },         // Нижняя шелкография

  // ================================================================
  // Courtyard / Зона компонента
  // ================================================================
  { "F.CrtYd", LID0_COURTYARD LID1_TOP },    // Верхняя зона компонента
  { "B.CrtYd", LID0_COURTYARD LID1_BOT },    // Нижняя зона компонента

  // ================================================================
  // Fabrication / Сборочный чертеж
  // ================================================================
  { "F.Fab", LID0_COMPONENT LID1_TOP },      // Верхний сборочный чертеж (корпус)
  { "B.Fab", LID0_COMPONENT LID1_BOT },      // Нижний сборочный чертеж (корпус)

  // ================================================================
  // Adhesive / Клей
  // ================================================================
  { "F.Adhes", LID0_ADHESIV LID1_TOP },      // Верхний клей
  { "B.Adhes", LID0_ADHESIV LID1_BOT },      // Нижний клей

  // ================================================================
  // Keep-Out / Запрет трассировки
  // ================================================================
  { "F.KeepOut", LID0_EXCLUSION LID1_TOP },  // Верхний запрет трассировки
  { "B.KeepOut", LID0_EXCLUSION LID1_BOT },  // Нижний запрет трассировки

  // Внутренние слои Keep-Out
  { "In1.KeepOut", LID0_EXCLUSION LID1_INT02 },
  { "In2.KeepOut", LID0_EXCLUSION LID1_INT03 },
  { "In3.KeepOut", LID0_EXCLUSION LID1_INT04 },
  { "In4.KeepOut", LID0_EXCLUSION LID1_INT05 },
  { "In5.KeepOut", LID0_EXCLUSION LID1_INT06 },
  { "In6.KeepOut", LID0_EXCLUSION LID1_INT07 },
  { "In7.KeepOut", LID0_EXCLUSION LID1_INT08 },
  { "In8.KeepOut", LID0_EXCLUSION LID1_INT09 },
  { "In9.KeepOut", LID0_EXCLUSION LID1_INT10 },
  { "In10.KeepOut", LID0_EXCLUSION LID1_INT11 },
  { "In11.KeepOut", LID0_EXCLUSION LID1_INT12 },
  { "In12.KeepOut", LID0_EXCLUSION LID1_INT13 },
  { "In13.KeepOut", LID0_EXCLUSION LID1_INT14 },
  { "In14.KeepOut", LID0_EXCLUSION LID1_INT15 },
  { "In15.KeepOut", LID0_EXCLUSION LID1_INT16 },
  { "In16.KeepOut", LID0_EXCLUSION LID1_INT17 },
  { "In17.KeepOut", LID0_EXCLUSION LID1_INT18 },
  { "In18.KeepOut", LID0_EXCLUSION LID1_INT19 },
  { "In19.KeepOut", LID0_EXCLUSION LID1_INT20 },
  { "In20.KeepOut", LID0_EXCLUSION LID1_INT21 },
  { "In21.KeepOut", LID0_EXCLUSION LID1_INT22 },
  { "In22.KeepOut", LID0_EXCLUSION LID1_INT23 },
  { "In23.KeepOut", LID0_EXCLUSION LID1_INT24 },
  { "In24.KeepOut", LID0_EXCLUSION LID1_INT25 },
  { "In25.KeepOut", LID0_EXCLUSION LID1_INT26 },
  { "In26.KeepOut", LID0_EXCLUSION LID1_INT27 },
  { "In27.KeepOut", LID0_EXCLUSION LID1_INT28 },
  { "In28.KeepOut", LID0_EXCLUSION LID1_INT29 },

  // ================================================================
  // Edge Cuts / Контур платы
  // ================================================================
  { "Edge.Cuts", LID0_GUIDE LID1_TOP LID1_BOT },  // Контур платы (специальный случай)

  // ================================================================
  // Margin / Поле
  // ================================================================
  { "Margin", LID0_GUIDE LID1_TOP LID1_BOT },     // Электрическая граница

  // ================================================================
  // User layers / Пользовательские слои
  // ================================================================
  { "Dwgs.User", LID0_DIM LID1_TOP LID1_BOT },    // Чертежи пользователя
  { "Cmts.User", LID0_IDENT LID1_TOP LID1_BOT },  // Комментарии
  { "Eco1.User", LID0_GUIDE LID1_TOP LID1_BOT },  // ECO 1
  { "Eco2.User", LID0_GUIDE LID1_TOP LID1_BOT },  // ECO 2


  // ================================================================
  // Via-specific layers (переходные отверстия)
  // ================================================================
  { "Via", LID0_WIRE LID1_TOP LID1_BOT },  // Сквозные переходные отверстия
  { "Via.In1", LID0_HOLE LID1_INT02 },     // Отверстие между TOP и INT00
  { "Via.In2", LID0_HOLE LID1_INT03 },     // и так далее...
  { "Via.In3", LID0_HOLE LID1_INT04 },
  { "Via.In4", LID0_HOLE LID1_INT05 },
  { "Via.In5", LID0_HOLE LID1_INT06 },
  { "Via.In6", LID0_HOLE LID1_INT07 },
  { "Via.In7", LID0_HOLE LID1_INT08 },
  { "Via.In8", LID0_HOLE LID1_INT09 },
  { "Via.In9", LID0_HOLE LID1_INT10 },
  { "Via.In10", LID0_HOLE LID1_INT11 },
  { "Via.In11", LID0_HOLE LID1_INT12 },
  { "Via.In12", LID0_HOLE LID1_INT13 },
  { "Via.In13", LID0_HOLE LID1_INT14 },
  { "Via.In14", LID0_HOLE LID1_INT15 },
  { "Via.In15", LID0_HOLE LID1_INT16 },
  { "Via.In16", LID0_HOLE LID1_INT17 },
  { "Via.In17", LID0_HOLE LID1_INT18 },
  { "Via.In18", LID0_HOLE LID1_INT19 },
  { "Via.In19", LID0_HOLE LID1_INT20 },
  { "Via.In20", LID0_HOLE LID1_INT21 },
  { "Via.In21", LID0_HOLE LID1_INT22 },
  { "Via.In22", LID0_HOLE LID1_INT23 },
  { "Via.In23", LID0_HOLE LID1_INT24 },
  { "Via.In24", LID0_HOLE LID1_INT25 },
  { "Via.In25", LID0_HOLE LID1_INT26 },
  { "Via.In26", LID0_HOLE LID1_INT26 },
  { "Via.In27", LID0_HOLE LID1_INT27 },
  { "Via.In28", LID0_HOLE LID1_INT28 },

  // Завершающий элемент
  { nullptr, nullptr }
};

static SdKiCadLayerMapDescription kicadLayerMapDescriptionForPads[] = {
  // ================================================================
  // Pad-specific layers (специальные случаи для падов)
  // ================================================================
  { "F.Cu", LID0_PAD LID1_TOP },              // Пады на верхнем слое
  { "B.Cu", LID0_PAD LID1_BOT },              // Пады на нижнем слое
  { "In1.Cu", LID0_PAD LID1_INT02 },          // Пады на внутреннем слое 1
  { "In2.Cu", LID0_PAD LID1_INT03 },          // ... и так далее до In28.Cu
  { "In3.Cu", LID0_PAD LID1_INT04 },
  { "In4.Cu", LID0_PAD LID1_INT05 },
  { "In5.Cu", LID0_PAD LID1_INT06 },
  { "In6.Cu", LID0_PAD LID1_INT07 },
  { "In7.Cu", LID0_PAD LID1_INT08 },
  { "In8.Cu", LID0_PAD LID1_INT09 },
  { "In9.Cu", LID0_PAD LID1_INT10 },
  { "In10.Cu", LID0_PAD LID1_INT11 },
  { "In11.Cu", LID0_PAD LID1_INT12 },
  { "In12.Cu", LID0_PAD LID1_INT13 },
  { "In13.Cu", LID0_PAD LID1_INT14 },
  { "In14.Cu", LID0_PAD LID1_INT15 },
  { "In15.Cu", LID0_PAD LID1_INT16 },
  { "In16.Cu", LID0_PAD LID1_INT17 },
  { "In17.Cu", LID0_PAD LID1_INT18 },
  { "In18.Cu", LID0_PAD LID1_INT19 },
  { "In19.Cu", LID0_PAD LID1_INT20 },
  { "In20.Cu", LID0_PAD LID1_INT21 },
  { "In21.Cu", LID0_PAD LID1_INT22 },
  { "In22.Cu", LID0_PAD LID1_INT23 },
  { "In23.Cu", LID0_PAD LID1_INT24 },
  { "In24.Cu", LID0_PAD LID1_INT25 },
  { "In25.Cu", LID0_PAD LID1_INT26 },
  { "In26.Cu", LID0_PAD LID1_INT27 },
  { "In27.Cu", LID0_PAD LID1_INT28 },
  { "In28.Cu", LID0_PAD LID1_INT29 },

  // Завершающий элемент
  { nullptr, nullptr }
};



static SdKiCadLayerMapDescription kicadLayerMapDescriptionForPolygon[] = {
  // ================================================================
  // Polygon-specific layers (специальные случаи для полигонов)
  // ================================================================
  { "F.Cu", LID0_POLYGON LID1_TOP },         // Полигоны на верхнем слое
  { "B.Cu", LID0_POLYGON LID1_BOT },         // Полигоны на нижнем слое
  { "In1.Cu", LID0_POLYGON LID1_INT02 },     // Полигоны на внутренних слоях
  { "In2.Cu", LID0_POLYGON LID1_INT03 },
  { "In3.Cu", LID0_POLYGON LID1_INT04 },
  { "In4.Cu", LID0_POLYGON LID1_INT05 },
  { "In5.Cu", LID0_POLYGON LID1_INT06 },
  { "In6.Cu", LID0_POLYGON LID1_INT07 },
  { "In7.Cu", LID0_POLYGON LID1_INT08 },
  { "In8.Cu", LID0_POLYGON LID1_INT09 },
  { "In9.Cu", LID0_POLYGON LID1_INT10 },
  { "In10.Cu", LID0_POLYGON LID1_INT11 },
  { "In11.Cu", LID0_POLYGON LID1_INT12 },
  { "In12.Cu", LID0_POLYGON LID1_INT13 },
  { "In13.Cu", LID0_POLYGON LID1_INT14 },
  { "In14.Cu", LID0_POLYGON LID1_INT15 },
  { "In15.Cu", LID0_POLYGON LID1_INT16 },
  { "In16.Cu", LID0_POLYGON LID1_INT17 },
  { "In17.Cu", LID0_POLYGON LID1_INT18 },
  { "In18.Cu", LID0_POLYGON LID1_INT19 },
  { "In19.Cu", LID0_POLYGON LID1_INT20 },
  { "In20.Cu", LID0_POLYGON LID1_INT21 },
  { "In21.Cu", LID0_POLYGON LID1_INT22 },
  { "In22.Cu", LID0_POLYGON LID1_INT23 },
  { "In23.Cu", LID0_POLYGON LID1_INT24 },
  { "In24.Cu", LID0_POLYGON LID1_INT25 },
  { "In25.Cu", LID0_POLYGON LID1_INT26 },
  { "In26.Cu", LID0_POLYGON LID1_INT27 },
  { "In27.Cu", LID0_POLYGON LID1_INT28 },
  { "In28.Cu", LID0_POLYGON LID1_INT29 },


  // Завершающий элемент
  { nullptr, nullptr }
};



QString kicadLayerMap( const QString kicadLayerName )
  {
  static QMap<QString,QString> mLayerMap;
  if( mLayerMap.isEmpty() ) {
    //Fill layer map
    for( int i = 0; kicadLayerMapDescription[i].mKiCadId != nullptr; i++ )
      mLayerMap.insert( kicadLayerMapDescription[i].mKiCadId, kicadLayerMapDescription[i].mLayerId );
    }
  return mLayerMap.value( kicadLayerName, LID0_INVISIBLE );
  }


void kicadFillPropLine( SdPropLine &prop, const SdKiCadStroke &stroke, const SdKiCadString &layer )
  {
  if( stroke.isValid() ) {
    //Setup width
    prop.mWidth = stroke.mWidth.mValue;
    //Convert kiCad line type into our
    switch( stroke.mType.mSelector ) {
      case SdKiCadStrokeType::typeDashDot :
      case SdKiCadStrokeType::typeDotted : prop.mType = dltDotted; break;
      case SdKiCadStrokeType::typeDashed : prop.mType = dltDashed; break;
      default:
      case SdKiCadStrokeType::typeSolid : prop.mType = dltSolid; break;
      }
    }
  else {
    prop.mWidth = 0;
    prop.mType  = dltSolid;
    }
  //Convert layer
  prop.mLayer.set( kicadLayerMap(layer.mValue) );
  }
