/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation files for smt automat.
*/
#include "SdPExportPlate_Assembler.h"

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

//SdPExportPlate_Assembler::SdPExportPlate_Assembler()
//  {

//  }

SdPExportPlate_Assembler::SdPExportPlate_Assembler(SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mEditor(editor),
  mPlate(plate)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("SMT assemble"), tr("Creates smt assemblion automat files for plate"), step, QString(":/pic/gerberExport.png") );


  }
