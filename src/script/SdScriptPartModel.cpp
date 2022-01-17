#include "SdScriptPartModel.h"
#include "objects/SdPItemPart.h"
#include "objects/Sd3dGraphModel.h"

#include <QDebug>

SdScriptPartModel::SdScriptPartModel(SdPItemPart *part) :
  SdScriptValueVariable(),
  mPart(part),
  mModel(nullptr)
  {

  }



void SdScriptPartModel::assign(SdM3dValuePtr src)
  {
  SdScriptVal3dModel model = src->toModel();
  if( model.count() ) {
    qDebug() << "add model" << model.count();
    if( mModel == nullptr || !mPart->isChild(mModel) ) {
      mModel = new Sd3dGraphModel();
      mPart->insertChild( mModel, nullptr );
      }
    mModel->modelAdd( model );
    }
  }
