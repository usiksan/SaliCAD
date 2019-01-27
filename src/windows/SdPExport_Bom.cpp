/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation report files:
   - bom
   - specification
   - list
   - cvs
   - json

  Common report file generated from pattern. Pattern is text file with next sections:
   - header
   - start page
   - next page
   - trailer

   Every section contains named fields which filled with project parameters
   Every page contains some lines, each of that filled with component list item
   Every line contains named fields which filled with list item pareameters
*/
#include "SdPExport_Bom.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdProject.h"

#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <functional>
#include <algorithm>

//marker lenght is 22 chars
#define BA_PROLOG       "<!--$$$ba_fldoc_prolog"
#define BA_EPILOG       "<!--$$$ba_fldoc_epilog"
#define BA_PAGE1_PROLOG "<!--$$$ba_page1_prolog"
#define BA_PAGE1_EPILOG "<!--$$$ba_page1_epilog"
#define BA_PAGE_PROLOG  "<!--$$$ba_pageN_prolog"
#define BA_PAGE_EPILOG  "<!--$$$ba_pageN_epilog"
#define BA_LINE1        "<!--$$$ba_line_1_start"
#define BA_LINE         "<!--$$$ba_line_N_start"

class BomArticle;

typedef QMap<QString,BomArticle*> BomArticleMap;

class BomArticle {
    QString     mPattern;         //Article pattern
    QStringList mFieldNameList;
    int         mMaxCount;        //Maximum article count
  public:
    BomArticle( int count ) : mMaxCount(count) {}

    //Maximum article count
    int     maxCount() const { return mMaxCount; }

    //Build article
    QString build( std::function<QString( const QString &field)> fun1 );

    static BomArticleMap parse( QString &src );
  private:
    void    setPattern( const QString pattern );
  };



//Build article
QString BomArticle::build(std::function<QString (const QString &)> fun1)
  {
  QString res = mPattern;
  for( const QString &field : mFieldNameList )
    res = res.arg( fun1(field) );
  return res;
  }





BomArticleMap BomArticle::parse(QString &src)
  {
  BomArticleMap map;
  BomArticle *article = new BomArticle(1);
  map.insert( QString(BA_PROLOG), article );
  int start = 0;
  while(1) {
    int i = src.indexOf( QString("<!--$$$"), start );
    if( i < 0 ) {
      article->setPattern( src.mid( start ) );
      break;
      }
    article->setPattern( src.mid( start, i - start ) );
    //Prepare next article
    QString id = src.mid( i, 22 );
    i += 23;
    start = src.indexOf( QString("-->"), i );
    int count = src.mid( i, start - i ).toInt();
    article = new BomArticle(count);
    map.insert( id, article );
    start += 3;
    }

  return map;
  }




void BomArticle::setPattern(const QString pattern)
  {
  mPattern.clear();
  mFieldNameList.clear();
  //Parse pattern, extract all markers and replace it with marker name index
  QMap<QString,int> map;
  int start = 0;
  while(1) {
    int i = pattern.indexOf( QString("<!--$#$"), start );
    if( i < 0 ) {
      mPattern.append( pattern.mid(start) );
      break;
      }
    mPattern.append( pattern.mid(start, i - start) );
    i += 7;
    start = pattern.indexOf( QString("-->"), i );
    QString field = pattern.mid( i, start - i );
    int index;
    if( map.contains(field) ) index = map.value(field);
    else {
      mFieldNameList.append( field );
      index = mFieldNameList.count();
      map.insert( field, index );
      }
    mPattern.append( QChar('%') ).append( QString::number(index) );
    start += 3;
    }
  }








SdPExport_Bom::SdPExport_Bom(SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mProjectItem(item)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("Reports"), tr("Creates report files:\n- BOM\n- specification\n- list of components\n- cvs\n- json"), step, QString(":/pic/gerberExport.png") );


  //Build interface
  QVBoxLayout *lay = new QVBoxLayout();

  //Title
  lay->addWidget( new QLabel( tr("Reports")) );

  //If checked info collected only from current item, otherwise - from all project
  lay->addWidget( mCurrentItem = new QCheckBox(tr("If checked info collected only from current item, otherwise - from all project")) );
  if( item->getClass() != dctPlate )
    mCurrentItem->setEnabled(false);

  //Two part:
  // - on left - bom creation
  // - on right - registry creation
  QVBoxLayout *left = new QVBoxLayout();
    left->addWidget( new QLabel( tr("BOM or specification generation")) );
    //If checked then generate BOM or specification
    left->addWidget( mGenBom = new QCheckBox( tr("Generate BOM or specification")) );
    left->addWidget( new QLabel(tr("Bom/specification patterns:")) );
    left->addWidget( mBomPatternList = new QListWidget() );
    left->addWidget( mBom = new QPushButton( tr("Generate") ) );
    connect( mBom, &QPushButton::click, this, &SdPExport_Bom::genBom );
    //At start we don't generate BOM, then we disable list and button
    mBomPatternList->setEnabled(false);
    mBom->setEnabled(false);
    //Disable-enable list and button on mGenBom checkBox toggled
    connect( mGenBom, &QCheckBox::toggled, mBomPatternList, &QListWidget::setEnabled );
    connect( mGenBom, &QCheckBox::toggled, mBom, &QPushButton::setEnabled );

    //BOM pattern list
    QDir patternDir( sdEnvir->mPatternPath );
    QFileInfoList lst = patternDir.entryInfoList( QStringList({QString("bom*.*")}), QDir::Files );
    for( const QFileInfo &info : lst ) {
      mBomPatternList->addItem( info.fileName() );
      }


  QVBoxLayout *right = new QVBoxLayout();
    right->addWidget( new QLabel( tr("Component registry generation")) );
    //If checked then generate component registry
    right->addWidget( mGenRegistry = new QCheckBox( tr("Generate component registry")) );
    //If checked then components in registry grouped if consistently the same
    right->addWidget( mGenRegistryGroup = new QCheckBox( tr("Group consistently the same components")) );
    right->addWidget( new QLabel(tr("Registry patterns:")) );
    right->addWidget( mRegistryPattern = new QListWidget() );
    right->addWidget( mRegistry = new QPushButton( tr("Generate") ) );
    connect( mRegistry, &QPushButton::click, this, &SdPExport_Bom::genRegistry );
    //At start we don't generate registry, then we disable list and button
    mGenRegistryGroup->setEnabled(false);
    mRegistryPattern->setEnabled(false);
    mRegistry->setEnabled(false);
    //Disable-enable list and button on mGenRegistry checkBox toggled
    connect( mGenRegistry, &QCheckBox::toggled, mGenRegistryGroup, &QCheckBox::setEnabled );
    connect( mGenRegistry, &QCheckBox::toggled, mRegistryPattern, &QListWidget::setEnabled );
    connect( mGenRegistry, &QCheckBox::toggled, mRegistry, &QPushButton::setEnabled );

    //Registry pattern list
    lst = patternDir.entryInfoList( QStringList({QString("registry*.*")}), QDir::Files );
    for( const QFileInfo &info : lst ) {
      mRegistryPattern->addItem( info.fileName() );
      }


  QHBoxLayout *hbox  = new QHBoxLayout();
  hbox->addLayout( left );
  hbox->addLayout( right );

  lay->addLayout( hbox );
  setLayout( lay );
  }








//Generate BOM
void SdPExport_Bom::genBom()
  {
  //Check if pattern selected in pattern list
  QListWidgetItem *item = mBomPatternList->currentItem();
  if( item ) {
    //Accumulate list
    QList<QJsonObject> ar = accumulate();
    //Sort by bom
    std::stable_sort( ar.begin(), ar.end(), [] ( const QJsonObject &obj1, const QJsonObject &obj2 ) -> bool {
      return obj1.value( QString("bom") ).toString() < obj2.value( QString("bom") ).toString();
      });

    }
  else QMessageBox::warning( this, tr("Warning!"), tr("You not selected pattern to generation. Select pattern in list and repeate.") );
  }




//Generate Registry
void SdPExport_Bom::genRegistry()
  {
  //Check if pattern selected in pattern list
  QListWidgetItem *item = mRegistryPattern->currentItem();
  if( item ) {
    //Accumulate list
    }
  else QMessageBox::warning( this, tr("Warning!"), tr("You not selected pattern to generation. Select pattern in list and repeate.") );
  }




bool SdPExport_Bom::validatePage()
  {
  return true;
  }




QList<QJsonObject> SdPExport_Bom::accumulate()
  {
  QList<QJsonObject> ar;
  if( mCurrentItem->isChecked() ) {
    //Accumulate only from current object
    mProjectItem->forEach( dctPartImp, [&ar] (SdObject *obj) -> bool {
      SdPtr<SdGraphPartImp> part(obj);
      if( part.isValid() ) {
        ar.append( part->paramTableObject() );
        }
      return true;
      });
    }
  else {
    //Accumulate from project
    SdProject *project = mProjectItem->getProject();
    //Scan all plates
    project->forEach( dctPlate, [&ar] (SdObject *obj) -> bool {
      SdPtr<SdPItemPlate> plate(obj);
      if( plate.isValid() ) {
        //Scan plate for all part implements
        plate->forEach( dctPartImp, [&ar] (SdObject *obj) -> bool {
          //For each part implement append param table object
          SdPtr<SdGraphPartImp> part(obj);
          if( part.isValid() ) {
            ar.append( part->paramTableObject() );
            }
          return true;
          });
        }
      return true;
      });
    }
  return ar;
  }


