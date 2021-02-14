/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation 3d masters.

  Show list for object type, user select one master and dialog
  execute selected master.
*/
#include "SdPNewProjectItem_3dMaster.h"
#include "SdPNewProjectItem.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdPItemRich.h"

SdPNewProjectItem_3dMaster::SdPNewProjectItem_3dMaster(SdProjectItemPtr *item, SdProject *prj, QWidget *parent) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj)
  {

  }




bool SdPNewProjectItem_3dMaster::validatePage()
  {
  int index = mMasterType->currentRow();
  if( index >= 0 )
    return true;
  return false;
  }




int SdPNewProjectItem_3dMaster::nextId() const
  {
  return SDP_NPI_NAME;
  }




void SdPNewProjectItem_3dMaster::initializePage()
  {
  //Fill programm list
  QStringList idList;
  SdObjectFactory::forEachHeader( [&idList] ( SdLibraryHeader &hdr ) -> bool {
    if( hdr.mClass == dctRich && hdr.mParamTable.contains( stdParam3dModelProgramm ) ) {
      idList.append( hdr.uid() );
      }
    if( idList.count() > 100 )
      return true;
    return false;
    });

  for( auto id : qAsConst(idList) ) {
    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, this ) );
    if( rich != nullptr ) {
      mMasterType->addItem( rich->getTitle() );
      mDescriptions.append( rich->paramGet( stdParam3dModelProgramm ) );
      mProgramms.append( rich->contents() );
      }
    }

  }
