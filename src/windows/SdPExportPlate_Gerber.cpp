/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation gerber file on plate
*/
#include "SdConfig.h"
#include "SdPExportPlate_Gerber.h"
#include "SdDLayers.h"
#include "SdWEditorGraphPlate.h"
#include "objects/SdContext.h"
#include "objects/SdConverterOffset.h"
#include "objects/SdEnvir.h"
#include "SvLib/SvDir.h"

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
#include <QDebug>




//Function for coord printing
QString gerberMM( int x ) {
  return QString("%1.%2").arg(x/1000).arg(x%1000,3,10,QChar('0'));
  }


//===========================================================================================
//              Aperture section

QString gerberApertureCircle( int r ) {
  if( r == 0 ) r = 1;
  return QString("C,") + gerberMM(r*2);
  }

QString gerberApertureRect( int w, int h ) {
  return QString("R,") + gerberMM(w) + QString("X") + gerberMM(h);
  }





//Gerber aperture accumulation context
//With this context when we 'draw' picture we actualy not draw
// Instead, we append unical apertures to aperture map
struct SdGerberApertureContext : public SdContext {
    QMap<QString,int> mApertureMap;

    SdGerberApertureContext();
    ~SdGerberApertureContext() override { delete mPainter; }

    // SdContext interface
    virtual void setPen(int width, QColor color, int lineStyle) override;
    virtual void fillRect(SdRect r) override;
    virtual void circle(SdPoint center, int radius ) override;
    virtual void circleFill(SdPoint center, int radius) override;
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;

    void appendAperture( const QString ap );
  };


SdGerberApertureContext::SdGerberApertureContext() :
  SdContext( SdPoint(), new QPainter() )
  {

  }

void SdGerberApertureContext::setPen(int width, QColor color, int lineStyle)
  {
  Q_UNUSED(color)
  Q_UNUSED(lineStyle)
  appendAperture( gerberApertureCircle(width/2) );
  }

void SdGerberApertureContext::fillRect(SdRect r)
  {
  if( !mTransform.isRotating() )
    appendAperture( gerberApertureRect( r.width(), r.height() )  );
  }




void SdGerberApertureContext::circle(SdPoint center, int radius)
  {
  circleFill( center, radius );
  }



void SdGerberApertureContext::circleFill(SdPoint center, int radius)
  {
  Q_UNUSED(center)
  appendAperture( gerberApertureCircle(radius) );
  }



void SdGerberApertureContext::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {
  Q_UNUSED(points);
  Q_UNUSED(layer);
  //We interested only with circle windows
  for( const SdPolyWindow &win : windows )
    if( win.getRadius() >= 0 )
      appendAperture( gerberApertureCircle(win.getRadius()) );
  }


void SdGerberApertureContext::appendAperture( const QString ap ) {
  if( !mApertureMap.contains(ap) ) {
    //Aperture not contained in map. Append
    int index = mApertureMap.count() + 10;
    mApertureMap.insert( ap, index );
    }
  }




//===========================================================================================
//              Gerber export section
//Gerber export context
//When we draw through this context we creating Gerber primitive structure
class SdGerberContext : public SdContext {
    QMap<QString,int> mApertureMap;
    int               mCurrentAperture;
    QTextStream      &mStream;
    SdPoint           mPos;
  public:
    SdGerberContext( const QMap<QString,int> &apertureMap, QTextStream &os ) :
      SdContext( SdPoint(), new QPainter() ),
      mApertureMap(apertureMap),
      mCurrentAperture(-1),
      mStream(os),
      mPos(2000000000,2000000000) { }

    ~SdGerberContext() override { delete mPainter; }

    // SdContext interface
  public:
    virtual void setPen(int width, QColor color, int lineStyle) override;
    virtual void line(SdPoint a, SdPoint b) override;
    virtual void fillRect(SdRect r) override;
    virtual void arc(SdPoint center, SdPoint start, SdPoint stop) override;
    virtual void circle(SdPoint center, int radius) override;
    virtual void circleFill(SdPoint center, int radius) override;
    virtual void regionFill( const SdPointList &points, const SdPropLine &prop ) override;
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;

  private:
    //Select current aperture
    void selectAperture( const QString apertureName );

    //Draw polygon with current settings
    void polygonInt(QPolygon pgn );
  };



void SdGerberContext::setPen(int width, QColor color, int lineStyle)
  {
  Q_UNUSED(color)
  Q_UNUSED(lineStyle)
  selectAperture( gerberApertureCircle(width/2) );
  }



void SdGerberContext::line(SdPoint a, SdPoint b)
  {
  //Line drawing
  //Линия интерпретируется как дорожка
  a = mTransform.map(a);
  b = mTransform.map(b);
  if( mPos != a ) mStream << "G01X" << a.x() << "Y" << a.y() << "D02*\n";
  mStream << "G01X" << b.x() << "Y" << b.y() << "D01*\n";
  mPos = b;
  }



void SdGerberContext::fillRect(SdRect r)
  {
  //Filled rectange interpretate as pad and make up as flash
  // only for nonrotated rect's. For rotated rect's we use polygon
  //Прямоугольник интерпретируется как площадки и делаются вспышками
  if( mTransform.isRotating() ) {
    //Rect is rotated. Use polygon
    polygonInt( mTransform.mapToPolygon(r) );
    }
  else {
    //Rect not rotated. Use flash
    selectAperture( gerberApertureRect( r.width(), r.height() )  );
    QPoint center = mTransform.mapRect( r ).center();
    mStream << "G55X" << (center.x()) << "Y" << (center.y()) << "D03*\n";
    }
  }




void SdGerberContext::arc(SdPoint center, SdPoint start, SdPoint stop)
  {
  //We always roll arc from start to stop clockwise
  //Select multiquadrant mode
  mStream << "G75*\n";
  //Move to start point
  if( mPos != start ) mStream << "G01X" << start.x() << "Y" << start.y() << "D02*\n";
  mStream << "G02X" << stop.x() << "Y" << stop.y() << "I" << (center.x() - start.x()) << "J" << (center.y() - start.y()) << "D01*\nG01*";
  mPos = stop;
  }




void SdGerberContext::circle(SdPoint center, int radius)
  {
  //Interpertate circle as filled circle
  circleFill( center, radius );
  }




void SdGerberContext::circleFill(SdPoint cx, int radius)
  {
  //Filled circle interpretate as pad and make up as flash
  selectAperture( gerberApertureCircle(radius) );
  QPoint center = mTransform.map( cx );
  mStream << "G55X" << (center.x()) << "Y" << (center.y()) << "D03*\n";
  }




void SdGerberContext::regionFill(const SdPointList &points, const SdPropLine &prop)
  {
  if( (mSelector || prop.mLayer.layer(mPairLayer)->isVisible()) && (points.count() > 2) ) {
    //Define posititve Gerber layer [Определить позитивный слой]
    mStream << "%LPD*%\n";
    mCurrentAperture = -1;
    mPos.set(2000000000,2000000000);

    //Draw polygon
    polygonInt( mTransform.map(points) );
    }
  }




void SdGerberContext::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {
  Q_UNUSED(layer)
  if( points.count() > 2 ) {
    //Define posititve Gerber layer [Определить позитивный слой]
    mStream << "%LPD*%\n";
    mCurrentAperture = -1;
    mPos.set(2000000000,2000000000);

    //Draw polygon
    polygonInt( mTransform.map(points) );

    //Polygon windows (only if there)
    if( windows.count() ) {
      mStream << "%LPC*%\n";
      for( const SdPolyWindow &win : windows )
        if( win.getRadius() >= 0 ) {
          //Circle window. Use flash
          circleFill( win.getP1(), win.getRadius() );
          }
        else {
          //Use polygon window
          polygonInt( mTransform.map( win.polygon().toPolygon() ) );
          }
      }
    }
  }




void SdGerberContext::selectAperture(const QString apertureName)
  {
  int ap = mApertureMap.value( apertureName );
  if( ap != mCurrentAperture ) {
    //Selected new aperture
    mCurrentAperture = ap;
    mStream << "G54D" << ap << "*\n";
    }
  }




void SdGerberContext::polygonInt(QPolygon pgn)
  {
  mStream << "G36*\n";
  QPoint a = pgn.at(0);
  mStream << "X" << a.x() << "Y" << a.y() << "D02*\n";
  for( int i = 1; i < pgn.count(); i++ ) {
    a = pgn.at(i);
    //Next polygon point [Очередная точка полигона]
    mStream << "X" << a.x() << "Y" << a.y() << "D01*\n";
    }
  //Close polygon
  mStream << "G37*\n";
  }









//===========================================================================================
//              Dialog section


SdPExportPlate_Gerber::SdPExportPlate_Gerber(SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mEditor(editor),
  mPlate(plate),
  mGenerated(false) //True if at least one time generation was pressed
  {
  setMinimumWidth(800);
  list->addMaster( tr("Gerber"), tr("Creates gerber files for plate"), step, QString(":/pic/gerberExport.png") );

  //Build interface
  QHBoxLayout *hbox  = new QHBoxLayout();
    //Left side - single file creation section
    QVBoxLayout *vbox = new QVBoxLayout();
      //Title
      vbox->addWidget( new QLabel( tr("Signle file creation") ) );
      //Title to gerber file
      vbox->addWidget( new QLabel( tr("Gerber file name:") ) );
      //Gerber file name and selection
      QHBoxLayout *fbox = new QHBoxLayout();
        mFile = new QLineEdit();
        fbox->addWidget( mFile );
        QPushButton *but = new QPushButton( tr("Select...") );
        fbox->addWidget( but );
        connect( but, &QPushButton::clicked, this, &SdPExportPlate_Gerber::onFileSelect );
      vbox->addLayout( fbox );

      //Layers selection
      fbox = new QHBoxLayout();
        fbox->addWidget( new QLabel(tr("Layers for gerber:")) );
        but = new QPushButton( tr("Layers...") );
        fbox->addWidget( but );
        connect( but, &QPushButton::clicked, this, &SdPExportPlate_Gerber::onLayers );
      vbox->addLayout( fbox );

      //Gerber generation
      fbox = new QHBoxLayout();
        fbox->addWidget( new QLabel(tr("Generate single gerber:")) );
        but = new QPushButton( tr("Generate") );
        fbox->addWidget( but );
        connect( but, &QPushButton::clicked, this, &SdPExportPlate_Gerber::onGenerate );
      vbox->addLayout( fbox );

      vbox->addStretch();

    hbox->addLayout( vbox );

    //Right side - group file creation section
    vbox = new QVBoxLayout();
      //Title
      vbox->addWidget( new QLabel( tr("Multiple file creation") ) );
      //Title to gerber file
      vbox->addWidget( new QLabel( tr("Gerber files path:") ) );
      //Gerber file path and selection
      fbox = new QHBoxLayout();
        mGroupPath = new QLineEdit();
        fbox->addWidget( mGroupPath );
        but = new QPushButton( tr("Select...") );
        fbox->addWidget( but );
        connect( but, &QPushButton::clicked, this, &SdPExportPlate_Gerber::onGroupPathSelect );
      vbox->addLayout( fbox );

      //Table for packet generation
      mGroup = new QTableWidget();
      vbox->addWidget( mGroup );

      //Button to group generation
      fbox = new QHBoxLayout();
        fbox->addWidget( new QLabel(tr("Generate gerber group:")) );
        but = new QPushButton( tr("Generate") );
        fbox->addWidget( but );
        connect( but, &QPushButton::clicked, this, &SdPExportPlate_Gerber::onGroupGenerate );
      vbox->addLayout( fbox );

    hbox->addLayout( vbox );

  setLayout( hbox );

  //Fill group table
  //We scan pattern directory for existing layers list files
  //For each file we create one line in table
  mGroup->setColumnCount(3);
  mGroup->setColumnWidth( 0, 80 );
  mGroup->setColumnWidth( 1, 130 );
  mGroup->setColumnWidth( 2, 130 );

  //Set column header
  QStringList header;
  header << tr("Generate") << tr("File name") << tr("Layer list");
  mGroup->setHorizontalHeaderLabels( header );

  //Hide row header
  mGroup->verticalHeader()->hide();

  //Directory with layers list files
  QDir pat( sdEnvir->mPatternPath );
  //Get file list of existing layers list files
  QStringList filtr;
  filtr << "*" SD_LAYER_LIST_EXTENSION;
  QFileInfoList patList = pat.entryInfoList( filtr );
  //Setup row for each file
  mGroup->setRowCount( patList.count() );
  int row = 0;
  for( const QFileInfo &info : patList ) {
    mGroup->setRowHeight( row, 25 );
    //By default we generate none
    mGroup->setItem( row, 0, new QTableWidgetItem(tr("No")) );
    mGroup->item( row,0 )->setFlags(Qt::ItemIsEnabled);
    //mGroup->item( row,0 )->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    //Construct file name
    mGroup->setItem( row, 1, new QTableWidgetItem( info.completeBaseName() + QStringLiteral(SD_GERBER_EXTENSION)) );
    //Layers list file name
    mGroup->setItem( row, 2, new QTableWidgetItem( info.fileName() ) );
    mGroup->item( row,2 )->setFlags(Qt::ItemIsEnabled);
    //Next row
    row++;
    }
  connect( mGroup,  &QTableWidget::cellClicked, this, &SdPExportPlate_Gerber::onCellClicked );

  }




//On press file select button. We show file select dialog.
void SdPExportPlate_Gerber::onFileSelect()
  {
  QString str = QFileDialog::getSaveFileName( this, tr("Enter or select gerber file name") );
  if( !str.isEmpty() )
    mFile->setText( str );
  }




void SdPExportPlate_Gerber::onGenerate()
  {
  //Retrive output file name and generate
  QString fn = mFile->text();
  if( fn.isEmpty() )
    QMessageBox::warning( this, tr("Error"), tr("File name field is empty. Enter file name gerber generate to.") );
  else
    generation( fn );
  }






//On press button "Layers...". We opern layers dialog
void SdPExportPlate_Gerber::onLayers()
  {
  //Create and execute layers dialog
  SdDLayers dlg( mPlate->getProject(), this );
  dlg.exec();

  //Update editor
  mEditor->dirtyCashe();
  mEditor->update();
  }





//On press button "group generate". We generate Gerber to goupt path for each selected pattern
void SdPExportPlate_Gerber::onGroupGenerate()
  {
  QString gn = mGroupPath->text();
  if( gn.isEmpty() ) {
    QMessageBox::warning( this, tr("Error"), tr("Group path field is empty. Enter path to witch files will be generate.") );
    return;
    }
  //For each row of file table we check flag of generation
  // if setup then generate according gerber
  SvDir pat( gn );
  for( int row = 0; row < mGroup->rowCount(); row++ )
    if( mGroup->item(row,0)->text() == tr("Yes") ) {
      //Load layer list
      SvDir dir( sdEnvir->mPatternPath );
      SdDLayers::loadLayerList( dir.slashedPath() + mGroup->item(row,2)->text() );

      //Update editor
      mEditor->dirtyCashe();
      mEditor->update();

      //Generate Gerber
      generation( pat.slashedPath() + mGroup->item(row,1)->text() );
      }
  }




//On press file group select button. We show directory select dialog
void SdPExportPlate_Gerber::onGroupPathSelect()
  {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if( !dir.isEmpty() )
    mGroupPath->setText( dir );
  }





//On cell clicked
void SdPExportPlate_Gerber::onCellClicked(int row, int column)
  {
  if( column == 0 ) {
    //Switch state of "generate" column
    if( mGroup->item(row,0)->text() == tr("Yes") )
      mGroup->item(row,0)->setText( tr("No") );
    else
      mGroup->item(row,0)->setText( tr("Yes") );
    }
  }




//Generate gerber for current view to file
void SdPExportPlate_Gerber::generation(const QString fileName)
  {
  //Mark that at least one file generated
  mGenerated = true;
  //Attempt to file create
  QFile file(fileName);
  if( file.open(QIODevice::WriteOnly) ) {
    QTextStream os( &file );
    QFileInfo info(file);
    bool polarPos = true;
    //Header
    //Заголовок
    os << "G04 SaliCAD Gerber export. www.salilab.com *\n"
          "%ASAXBY*\n"     //Выбор осей
          "FSLAX33Y33*\n"  //Формат, опущены лидирующие нули, абсолютные данные, 3 целых 3 дробных
          "MIA0B0*\n"      //Зеркальность 0=нет, 1=есть
          "MOMM*\n"        //Режим миллиметров
          "OFA0B0*\n"      //Смещение 0
          "SFA1.0B1.0*%\n" //Масштаб 1:1

          "%IJALBL*\n"     //Выравнивание картинки
          "IN"
       << info.completeBaseName()
       << "*\n"            //Название картинки
          "IOA0B0*%\n"     //Смещение
          "%IP"
       << (polarPos ? "POS*\n" : "NEG*\n") //Позитив
       << "IR0*%\n";        //Поворот

    //Apertures accum
    SdGerberApertureContext app;
    mPlate->forEach( dctAll, [&app] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph(obj);
      if( graph.isValid() )
        graph->draw( &app );
      return true;
      } );

    //Print apertures list
    os << "G04 Define Apertures*\n";
    for( auto iter = app.mApertureMap.cbegin(); iter != app.mApertureMap.cend(); iter++ )
      os << "%ADD" << iter.value() << iter.key() << "*%\n";

    //Build piture [Сформировать картинку]
    //Calculate over rect [Подсчитать охватывающий прямоугольник]
    SdRect over = mPlate->getOverRect();
    qDebug() << "Gerber over" << over;

    //Define context [Образовать контекст]
    SdGerberContext gc( app.mApertureMap, os );

    //Offset of picture [Образовать смещение]
    SdConverterOffset offset( over.getBottomLeft().complement() );
    gc.setConverter( &offset );

    //At first, draw polygons [Рисуем вначале полигоны]
    mPlate->forEach( dctTracePolygon, [&gc] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph(obj);
      if( graph.isValid() )
        graph->draw( &gc );
      return true;
      });

    //Positiv layer [Определить позитивный слой]
    os << "%LPD*%\n";

    //At now, draw all except polygons [Теперь рисуем все кроме полигонов]
    mPlate->forEach( dctAll & ~dctTracePolygon, [&gc] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph(obj);
      if( graph.isValid() )
        graph->draw( &gc );
      return true;
      });


    //File finish [Завершить файл]
    os << "M02*";
    os.flush();
    QMessageBox::information( this, tr("Info!"), tr("Gerber file \'%1\' successfully created.").arg(fileName) );
    }
  else
    QMessageBox::warning( this, tr("Error!"), tr("Can't create Gerber file \'%1\'").arg(fileName) );
  }




//Test if can finished
bool SdPExportPlate_Gerber::validatePage()
  {
  //If at least one file generated we can finish
  if( !mGenerated )
    QMessageBox::warning( this, tr("Warning!"), tr("Press generate button to generate one or more gerber files or press Cancel") );
  return mGenerated;
  }


