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
#include "SdPExportPlate_Gerber.h"
#include "SdDLayers.h"
#include "SdWEditorGraphPlate.h"
#include "objects/SdContext.h"
#include "objects/SdConverterOffset.h"

#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>




//Function for coord printing
QString gerberMM( int x ) {
  return QString("%1.%2").arg(x/1000).arg(x%1000,3,10,QChar('0'));
  }


//===========================================================================================
//              Aperture section

QString gerberApertureCircle( int r ) {
  return QString("C,") + gerberMM(r);
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
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;

  private:
    //Select current aperture
    void selectAperture( const QString apertureName );

    //Draw polygon with current settings
    void polygonInt(QPolygon pgn );
  };



void SdGerberContext::setPen(int width, QColor color, int lineStyle)
  {
  Q_UNUSED(color);
  Q_UNUSED(lineStyle);
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
  //TODO D054 append arc implementation to Gerber export
  }




void SdGerberContext::circle(SdPoint center, int radius)
  {
  //TODO D055 append circle implementation to Gerber export
  }




void SdGerberContext::circleFill(SdPoint cx, int radius)
  {
  //Filled circle interpretate as pad and make up as flash
  selectAperture( gerberApertureCircle(radius) );
  QPoint center = mTransform.map( cx );
  mStream << "G55X" << (center.x()) << "Y" << (center.y()) << "D03*\n";
  }




void SdGerberContext::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {
  Q_UNUSED(layer);
  if( points.count() > 2 ) {
    //Определить позитивный слой
    mStream << "G02*\n%LPD*%\n";
    mCurrentAperture = -1;
    mPos.set(2000000000,2000000000);

    //First polygon point [Начальная точка полигона]
    polygonInt( mTransform.map(points) );

    //Polygon windows (only if there)
    if( windows.count() ) {
      mStream << "G02*\n%LPC*%\n";
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
    mStream << "G02*\n"
            << "G54D" << ap << "*\n";
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
  mPlate(plate)
  {
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

    hbox->addLayout( vbox );

    //Right side - group file creation section
    vbox = new QVBoxLayout();
      //Title
      vbox->addWidget( new QLabel( tr("Multiple file creation") ) );

    hbox->addLayout( vbox );

  setLayout( hbox );
  }




//On press file select button. We show file select dialog.
void SdPExportPlate_Gerber::onFileSelect()
  {
  //TODO B052 Not available to enter custom file name, only select
  QString str = QFileDialog::getOpenFileName( this, tr("Enter or select gerber file name") );
  if( !str.isEmpty() )
    mFile->setText( str );
  }




void SdPExportPlate_Gerber::onGenerate()
  {
  //Retrive output file name
  QString fileName = mFile->text();
  //Attempt to file create
  QFile file(fileName);
  if( file.open(QIODevice::WriteOnly) ) {
    QTextStream os( &file );
    QFileInfo info(file);
    bool polarPos = true;
    //Header
    //Заголовок
    os << "%ASAXBY*\n"     //Выбор осей
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
    os << "G02*\n%LPD*%\n";

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
