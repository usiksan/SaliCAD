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
#include "library/SvDir.h"
#include "objects/SdUtil.h"

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


//======================================================================================================================
// BomArticle - one article from source pattern
// BomArticleMap - all articles pattern from source pattern

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

    //Parse source string
    static BomArticleMap parse(QString src );

    //Parse pattern file
    static BomArticleMap parsePatternFile( const QString fileName );
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





BomArticleMap BomArticle::parse(QString src)
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






//Parse pattern file
BomArticleMap BomArticle::parsePatternFile(const QString fileName)
  {
  QFile file(fileName);
  if( file.open(QIODevice::ReadOnly) )
    return parse( QString::fromUtf8( file.readAll() ) );

  //When error return empty map
  return BomArticleMap();
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







//======================================================================================================================
// export bom dialog

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
    connect( mBom, &QPushButton::clicked, this, &SdPExport_Bom::genBom );
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
    connect( mRegistry, &QPushButton::clicked, this, &SdPExport_Bom::genRegistry );
    //At start we don't generate registry, then we disable list and button
    mGenRegistryGroup->setEnabled(false);
    mGenRegistryGroup->setVisible(false);
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







//Form itemId list
static QString buildItemIdList( QList<int> &logNumberList, const QString prefix )
  {
  if( logNumberList.count() == 1 )
    return QString("%1%2").arg( prefix ).arg( logNumberList.at(0) );
  else if( logNumberList.count() == 0 )
    return QString();

  //Sort logNumberList with ascending order
  std::stable_sort( logNumberList.begin(), logNumberList.end(), [] ( const int val1, const int val2 ) -> bool { return val1 < val2; } );
  QString res;
  int index = logNumberList.at(0);
  res.append( prefix ).append( QString::number(index) );
  int count = 1;
  for( int i = 1; i < logNumberList.count(); i++ )
    if( index + 1 == logNumberList.at(i) ) {
      index++;
      count++;
      }
    else {
      if( count > 2 )
        res.append( QChar('-') ).append( prefix ).append( QString::number(index) );
      else if( count == 2 )
        res.append( QChar(',') ).append( prefix ).append( QString::number(index) );
      index = logNumberList.at(i);
      res.append( QChar(',') ).append( prefix ).append( QString::number(index) );
      }
  if( count > 2 )
    res.append( QChar('-') ).append( prefix ).append( QString::number(index) );
  else if( count == 2 )
    res.append( QChar(',') ).append( prefix ).append( QString::number(index) );
  return res;
  }




//Generate BOM
void SdPExport_Bom::genBom()
  {
  //Check if pattern selected in pattern list
  QListWidgetItem *pattern = mBomPatternList->currentItem();
  if( pattern ) {
    //Accumulate list
    QList<QJsonObject> ar = accumulate();

    //Sort by bom
    std::stable_sort( ar.begin(), ar.end(), [] ( const QJsonObject &obj1, const QJsonObject &obj2 ) -> bool {
      return obj1.value( stdParamBom ).toString() < obj2.value( stdParamBom ).toString();
      });

    //Form result bom
    QList<QJsonObject> res;
    QJsonObject bomItem;
    QList<int> logNumberList;
    for( const QJsonObject &comp : ar ) {
      if( bomItem.isEmpty() ) {
        //bomItem not assigned yet, assign comp to it
        bomItem = comp;
        logNumberList.clear();
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      else if( bomItem.value( stdParamBom ).toString() == comp.value( stdParamBom ).toString() ) {
        //Next item in bomItem series
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      else {
        //Complete with bomItem previous series, append it to result list
        bomItem.insert( stdParamCompCount, QString::number( logNumberList.count() ) );
        bomItem.insert( stdParamItemIdList, buildItemIdList( logNumberList, bomItem.value( stdParamPrefix ).toString() ) );
        //
        res.append( bomItem );

        //Assign new comp
        bomItem = comp;
        logNumberList.clear();
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      }

    //Append last item
    bomItem.insert( stdParamCompCount, QString::number( logNumberList.count() ) );
    bomItem.insert( stdParamItemIdList, buildItemIdList( logNumberList, bomItem.value( stdParamPrefix ).toString() ) );
    res.append( bomItem );

    SvDir patternDir( sdEnvir->mPatternPath );
    BomArticleMap map = BomArticle::parsePatternFile( patternDir.slashedPath() + pattern->text() );

    if( map.isEmpty() )
      QMessageBox::warning( this, tr("Error!"), tr("Can't read pattern file.") );
    else {
      //Destignation text
      QString dest;
      //Total line count
      int lineCount = res.count();
      //Document prolog and epilog
      BomArticle *prolog = map.value( QString(BA_PROLOG), nullptr );
      BomArticle *epilog = map.value( QString(BA_EPILOG), nullptr );
      //First page prolog and epilog
      BomArticle *page1Prolog = map.value( QString(BA_PAGE1_PROLOG), nullptr );
      BomArticle *page1Epilog = map.value( QString(BA_PAGE1_EPILOG), nullptr );
      //First page table line
      BomArticle *line1 = map.value( QString(BA_LINE1), nullptr );

      //Next page prolog and epilog
      BomArticle *pageProlog = map.value( QString(BA_PAGE_PROLOG), nullptr );
      BomArticle *pageEpilog = map.value( QString(BA_PAGE_EPILOG), nullptr );
      //Next page table line
      BomArticle *line = map.value( QString(BA_LINE), nullptr );

      //Calculate total page count
      if( line1 && page1Prolog ) {
        int page1Count = SdUtil::iLimit( (lineCount + line1->maxCount() - 1) / line1->maxCount(), 1, page1Prolog->maxCount() );
        lineCount -= line1->maxCount() * page1Count;
        mTotalPageCount = page1Count;
        }
      else mTotalPageCount = 0;

      if( line && pageProlog && lineCount > 0 ) {
        int pageCount = SdUtil::iLimit( (lineCount + line->maxCount() - 1) / line->maxCount(), 1, pageProlog->maxCount() );
        mTotalPageCount += pageCount;
        }

      //Total line count
      lineCount = res.count();
      //Perform generation
      if( prolog )
        dest = prolog->build( [this] (const QString &key) ->QString { return globalParam(key); } );

      mPageIndex = 1;
      mLineIndex = 0;
      if( page1Prolog && line1 ) {
        for( int i = 0; i < page1Prolog->maxCount() && mLineIndex < lineCount; i++ ) {

          dest.append( page1Prolog->build( [this] (const QString &key) -> QString { return globalParam(key); }));

          for( int lineIndex = 0; lineIndex < line1->maxCount() && mLineIndex < lineCount; lineIndex++ ) {
            dest.append( line1->build( [this,res] (const QString &key) -> QString {
              return res.at(mLineIndex).value(key).toString();
              }));
            mLineIndex++;
            }

          if( page1Epilog )
            dest.append( page1Epilog->build( [this] (const QString &key) -> QString { return globalParam(key); } ));

          mPageIndex++;
          }
        }

      if( pageProlog && line ) {
        for( int i = 0; i < pageProlog->maxCount() && mLineIndex < lineCount; i++ ) {

          dest.append( pageProlog->build( [this] (const QString &key) -> QString { return globalParam(key); }));

          for( int lineIndex = 0; lineIndex < line->maxCount() && mLineIndex < lineCount; lineIndex++ ) {
            dest.append( line->build( [this,res] (const QString &key) -> QString {
              return res.at(mLineIndex).value(key).toString();
              }));
            mLineIndex++;
            }

          if( pageEpilog )
            dest.append( pageEpilog->build( [this] (const QString &key) -> QString { return globalParam(key); } ));

          mPageIndex++;
          }

        }

      if( epilog )
        dest.append( epilog->build( [this] (const QString &key) ->QString { return globalParam(key); } ) );

      qDeleteAll( map );

      //Generation completed
      QString destFileName = QFileDialog::getSaveFileName( this, tr("Enter file name to save report") );
      if( !destFileName.isEmpty() ) {
        QFile destFile(destFileName);
        if( destFile.open( QIODevice::WriteOnly ) ) {
          destFile.write( dest.toUtf8() );
          destFile.close();
          QMessageBox::warning( this, tr("Info!"), tr("Report successfully created.") );
          }
        }
      }
    }
  else QMessageBox::warning( this, tr("Warning!"), tr("You not selected pattern to generation. Select pattern in list and repeate.") );
  }




//Generate Registry
void SdPExport_Bom::genRegistry()
  {
  //Check if pattern selected in pattern list
  QListWidgetItem *pattern = mRegistryPattern->currentItem();
  if( pattern ) {
    //Accumulate list
    QList<QJsonObject> ar = accumulate();

    //Sort by prefix, logNumber
    std::stable_sort( ar.begin(), ar.end(), [] ( const QJsonObject &obj1, const QJsonObject &obj2 ) -> bool {
      if( obj1.value( stdParamPrefix ).toString() == obj2.value( stdParamPrefix ).toString() )
        return obj1.value( stdParamLogNumber ).toInt() < obj2.value( stdParamLogNumber ).toInt();
      return obj1.value( stdParamPrefix ).toString() < obj2.value( stdParamPrefix ).toString();
      });


    //Form result registry
    QList<QJsonObject> res;
    QJsonObject bomItem;
    QJsonObject emptyItem;
    QList<int> logNumberList;
    QString currentPrefix;
    for( const QJsonObject &comp : ar ) {
      if( bomItem.isEmpty() ) {
        //bomItem not assigned yet, assign comp to it
        bomItem = comp;
        logNumberList.clear();
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      else if( bomItem.value( stdParamBom ).toString() == comp.value( stdParamBom ).toString() && ((logNumberList.last() + 1) == comp.value( stdParamLogNumber ).toInt()) ) {
        //Next item in bomItem series
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      else {
        //Complete with bomItem previous series, append it to result list
        bomItem.insert( stdParamCompCount, QString::number( logNumberList.count() ) );
        bomItem.insert( stdParamItemIdList, buildItemIdList( logNumberList, bomItem.value( stdParamPrefix ).toString() ) );
        //
        if( currentPrefix != bomItem.value( stdParamPrefix ).toString() ) {
          //Insert two empty strings
          res.append( emptyItem );
          res.append( emptyItem );
          currentPrefix = bomItem.value( stdParamPrefix ).toString();
          }
        res.append( bomItem );

        //Assign new comp
        bomItem = comp;
        logNumberList.clear();
        logNumberList.append( comp.value( stdParamLogNumber ).toInt() );
        }
      }

    //Append last item
    bomItem.insert( stdParamCompCount, QString::number( logNumberList.count() ) );
    bomItem.insert( stdParamItemIdList, buildItemIdList( logNumberList, bomItem.value( stdParamPrefix ).toString() ) );
    if( currentPrefix != bomItem.value( stdParamPrefix ).toString() ) {
      //Insert two empty strings
      res.append( emptyItem );
      res.append( emptyItem );
      currentPrefix = bomItem.value( stdParamPrefix ).toString();
      }
    res.append( bomItem );


    SvDir patternDir( sdEnvir->mPatternPath );
    BomArticleMap map = BomArticle::parsePatternFile( patternDir.slashedPath() + pattern->text() );

    if( map.isEmpty() )
      QMessageBox::warning( this, tr("Error!"), tr("Can't read pattern file.") );
    else {
      //Destignation text
      QString dest;
      //Total line count
      int lineCount = res.count();
      //Document prolog and epilog
      BomArticle *prolog = map.value( QString(BA_PROLOG), nullptr );
      BomArticle *epilog = map.value( QString(BA_EPILOG), nullptr );
      //First page prolog and epilog
      BomArticle *page1Prolog = map.value( QString(BA_PAGE1_PROLOG), nullptr );
      BomArticle *page1Epilog = map.value( QString(BA_PAGE1_EPILOG), nullptr );
      //First page table line
      BomArticle *line1 = map.value( QString(BA_LINE1), nullptr );

      //Next page prolog and epilog
      BomArticle *pageProlog = map.value( QString(BA_PAGE_PROLOG), nullptr );
      BomArticle *pageEpilog = map.value( QString(BA_PAGE_EPILOG), nullptr );
      //Next page table line
      BomArticle *line = map.value( QString(BA_LINE), nullptr );

      //Calculate total page count
      if( line1 && page1Prolog ) {
        int page1Count = SdUtil::iLimit( (lineCount + line1->maxCount() - 1) / line1->maxCount(), 1, page1Prolog->maxCount() );
        lineCount -= line1->maxCount() * page1Count;
        mTotalPageCount = page1Count;
        }
      else mTotalPageCount = 0;

      if( line && pageProlog && lineCount > 0 ) {
        int pageCount = SdUtil::iLimit( (lineCount + line->maxCount() - 1) / line->maxCount(), 1, pageProlog->maxCount() );
        mTotalPageCount += pageCount;
        }

      //Total line count
      lineCount = res.count();
      //Perform generation
      if( prolog )
        dest = prolog->build( [this] (const QString &key) ->QString { return globalParam(key); } );

      mPageIndex = 1;
      mLineIndex = 0;
      if( page1Prolog && line1 ) {
        for( int i = 0; i < page1Prolog->maxCount() && mLineIndex < lineCount; i++ ) {

          dest.append( page1Prolog->build( [this] (const QString &key) -> QString { return globalParam(key); }));

          for( int lineIndex = 0; lineIndex < line1->maxCount() && mLineIndex < lineCount; lineIndex++ ) {
            dest.append( line1->build( [this,res] (const QString &key) -> QString {
              return res.at(mLineIndex).value(key).toString();
              }));
            mLineIndex++;
            }

          if( page1Epilog )
            dest.append( page1Epilog->build( [this] (const QString &key) -> QString { return globalParam(key); } ));

          mPageIndex++;
          }
        }

      if( pageProlog && line ) {
        for( int i = 0; i < pageProlog->maxCount() && mLineIndex < lineCount; i++ ) {

          dest.append( pageProlog->build( [this] (const QString &key) -> QString { return globalParam(key); }));

          for( int lineIndex = 0; lineIndex < line->maxCount() && mLineIndex < lineCount; lineIndex++ ) {
            dest.append( line->build( [this,res] (const QString &key) -> QString {
              return res.at(mLineIndex).value(key).toString();
              }));
            mLineIndex++;
            }

          if( pageEpilog )
            dest.append( pageEpilog->build( [this] (const QString &key) -> QString { return globalParam(key); } ));

          mPageIndex++;
          }

        }

      if( epilog )
        dest.append( epilog->build( [this] (const QString &key) ->QString { return globalParam(key); } ) );

      qDeleteAll( map );

      //Generation completed
      QString destFileName = QFileDialog::getSaveFileName( this, tr("Enter file name to save report") );
      if( !destFileName.isEmpty() ) {
        QFile destFile(destFileName);
        if( destFile.open( QIODevice::WriteOnly ) ) {
          destFile.write( dest.toUtf8() );
          destFile.close();
          QMessageBox::warning( this, tr("Info!"), tr("Report successfully created.") );
          }
        }
      }
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




//Retrive global param
QString SdPExport_Bom::globalParam(const QString paramName) const
  {
  if( paramName == stdParamTotalPageCount )
    return QString::number(mTotalPageCount);
  if( paramName == stdParamPageIndex )
    return QString::number(mPageIndex);
  SdProject *project = mProjectItem->getProject();
  return project->paramGet( paramName );
  }





