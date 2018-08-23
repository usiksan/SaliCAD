/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCPartPlace.h"

SdModeCPartPlace::SdModeCPartPlace(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCPartPlace::activate()
    {
    }

void SdModeCPartPlace::drawStatic(SdContext *ctx)
    {
    }

void SdModeCPartPlace::drawDynamic(SdContext *ctx)
    {
    }

int SdModeCPartPlace::getPropBarId() const
    {
    }

void SdModeCPartPlace::propGetFromBar()
    {
    }

void SdModeCPartPlace::propSetToBar()
    {
    }

void SdModeCPartPlace::enterPoint(SdPoint)
    {
    }

void SdModeCPartPlace::cancelPoint(SdPoint)
    {
    }

void SdModeCPartPlace::movePoint(SdPoint)
    {
    }

void SdModeCPartPlace::keyDown(int key, QChar ch)
    {
    }

void SdModeCPartPlace::keyUp(int key, QChar ch)
    {
    }

SdPoint SdModeCPartPlace::enterPrev()
    {
    }

void SdModeCPartPlace::beginDrag(SdPoint)
    {
    }

void SdModeCPartPlace::dragPoint(SdPoint)
    {
    }

void SdModeCPartPlace::stopDrag(SdPoint)
    {
    }

bool SdModeCPartPlace::getInfo(SdPoint p, QString &info)
    {
    }

QString SdModeCPartPlace::getStepHelp() const
    {
    }

QString SdModeCPartPlace::getModeThema() const
    {
    }

QString SdModeCPartPlace::getStepThema() const
    {
    }

int SdModeCPartPlace::getCursor() const
  {
  switch( getStep() ) {
    //Is selection by rect
    //[Происходит выделение прямоугольником]
    case psmBeingSelRect : return CUR_SEL;
    //No selection [Нет выделения]
    case psmNoSelect :
      //CheckPoint(prevMove);
      //Позаботиться о строке состояния
      //GetViewer()->SetMessage( GetStepHelp() );
      return mBehindCursorTable.count() ? CUR_ TAKE_CUR : POINT_CUR;
    default :
    case psmMove : return PLACE_CUR;
    }
  }



int SdModeCPartPlace::getIndex() const
  {
  return MD_PLACE;
  }
