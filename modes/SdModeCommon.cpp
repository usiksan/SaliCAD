/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdModeCommon.h"


void SdModeCommon::activate()
  {
  //Установить панель свойств
//  SetPropBar( GetPropBarId(), 0 );
  //Выполнить действия по умолчанию
  SdMode::activate();
  }



void SdModeCommon::propChanged(SdContext &)
  {
//  //Свойства изменились, нужно стереть с предыдущими свойствами, а затем перерисовать с установленными
//  dc.SetXorMode( wccEnter );          //Режим затирания
//  Hide( dc );                         //Стереть
//  // Получить новые свойства
//  DAutomation( DAC_GETPROP, 0, 0, &GetGlobalProp() );
//  SetPropertais( GetGlobalProp() );   //Изменить свойства режима
//  dc.SetXorMode( wccEnter );          //Режим затирания
//  Show( dc );                         //Нарисовать
  }



void SdModeCommon::movePoint(SdPoint, SdContext &)
  {
//  if( prev != p ) {
//    dc.SetXorMode( wccEnter );
//    Hide( dc );
//    MovePointLow( p );
//    dc.SetXorMode( wccEnter );
//    Show( dc );
//    prev = p;
//    }
  }
