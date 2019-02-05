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
  mPropSource(propSource),
  mRegionSource(regionSource),
  mWindowsSource(windowsSource)
  {
  mPropSource->saveState( &mPropState );
  mRegion  = *mRegionSource;
  mWindows = *mWindowsSource;
  }




//Undo elementary operation.
void SdUndoRecordPolygon::undo()
  {
  mPropSource->swapState( &mPropState );
  mRegionSource->swap( mRegion );
  mWindowsSource->swap( mWindows );
  }



