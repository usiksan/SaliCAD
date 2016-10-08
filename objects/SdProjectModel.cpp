/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdProjectModel.h"
#include "SdProject.h"
#include "SdProjectItem.h"

SdProjectModel::SdProjectModel(SdProject *project, QObject *parent) :
  QAbstractItemModel(parent),
  mProject(project)
  {

  }

SdProjectModel::~SdProjectModel()
  {
  delete mProject;
  }

QVariant SdProjectModel::data(const QModelIndex &index, int role) const
  {
  if( !index.isValid() )
    return QVariant();

  if( role != Qt::DisplayRole )
    return QVariant();

  switch( index.internalId() ) {
    case 1 : return QVariant( tr("Sheets") );
    case 2 : return QVariant( tr("PCBs and construction") );
    case 3 : return QVariant( tr("Components") );
    case 4 : return QVariant( tr("Symbols") );
    case 5 : return QVariant( tr("Parts") );
    default : {
      SdProjectItem *item = dynamic_cast<SdProjectItem*>( index.internalPointer() );
      if( item ) {
        return QVariant( item->getTitle() );
        }
      }

    }
  return QVariant(); // item->data(index.column());
  }

Qt::ItemFlags SdProjectModel::flags(const QModelIndex &index) const
  {
  if( !index.isValid() )
    return 0;

  return QAbstractItemModel::flags(index);
  }

QModelIndex SdProjectModel::index(int row, int column, const QModelIndex &parent) const
  {
//  if( !hasIndex(row, column, parent) )
//    return QModelIndex();

  if( !parent.isValid() ) {
    if( row < 5 )
      return createIndex( row, column, row + 1 );
    return QModelIndex();
    }
  if( parent.internalId() < 10 )
    return createIndex( row, column, mProject->item( parent.internalId(), row ) );
  return QModelIndex();
  }

QModelIndex SdProjectModel::parent(const QModelIndex &index) const
  {
  if( !index.isValid() )
    return QModelIndex();

  if( index.internalId() < 10 )
    return QModelIndex();

  SdProjectItem *childItem = dynamic_cast<SdProjectItem*>(index.internalPointer());
  if( childItem ) {
    QString tp = childItem->getType();
    if( tp == QString("Sheet") ) return createIndex( 0, 0, 1 );
    if( tp == QString("PCB") ) return createIndex( 1, 0, 2 );
    if( tp == QString("Comp") ) return createIndex( 2, 0, 3 );
    if( tp == QString("Sym") ) return createIndex( 3, 0, 4 );
    if( tp == QString("Part") ) return createIndex( 4, 0, 5 );
    }
  return QModelIndex();
  }

int SdProjectModel::rowCount(const QModelIndex &parent) const
  {
  if( !parent.isValid() ) return 5;
  return mProject->countOf( parent.internalId() );
  }

int SdProjectModel::columnCount(const QModelIndex &parent) const
  {
  return 1;
  }

