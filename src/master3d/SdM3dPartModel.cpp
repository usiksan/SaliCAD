#include "SdM3dPartModel.h"
#include "objects/SdPItemPart.h"
#include "objects/Sd3dGraphModel.h"

#include <QDebug>

SdM3dPartModel::SdM3dPartModel(SdPItemPart *part) :
  SdM3dVariable(),
  mPart(part),
  mModel(nullptr)
  {

  }



void SdM3dPartModel::assign(SdM3dValuePtr src)
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
