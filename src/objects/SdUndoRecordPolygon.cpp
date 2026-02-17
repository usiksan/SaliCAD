/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for polygon.
*/
#include "SdUndoRecordPolygon.h"


SdUndoRecordPolygon::SdUndoRecordPolygon(SdPropPolygon *propSource, SdPointList *regionSource, SdPolyWindowList *windowsSource) :
  SdUndoRecord(),
  mPropState(*propSource),
  mPropSource(propSource),
  mRegion(*regionSource),
  mRegionSource(regionSource),
  mWindows(*windowsSource),
  mWindowsSource(windowsSource)
  {
  }




//Undo elementary operation.
void SdUndoRecordPolygon::undo()
  {
  mPropSource->swap( mPropState );
  mRegionSource->swap( mRegion );
  mWindowsSource->swap( mWindows );
  }



