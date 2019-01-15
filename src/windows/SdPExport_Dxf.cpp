/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation dxf file from current object
*/
#include "SdPExport_Dxf.h"
#include "objects/SdContext.h"

//Dxf context
//With this context when we 'draw' picture we actualy not draw
// Instead, we append items to dxf file
struct SdDxfContext : public SdContext {
    QString mBuffer;

    SdDxfContext();
    ~SdDxfContext() override { delete mPainter; }

    // SdContext interface
    virtual void setPen(int width, QColor color, int lineStyle) override;
    virtual void fillRect(SdRect r) override;
    virtual void circle(SdPoint center, int radius ) override;
    virtual void circleFill(SdPoint center, int radius) override;
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;


  };




SdPExport_Dxf::SdPExport_Dxf(SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mProjectItem(item)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("DXF file"), tr("Creates DXF file from current object"), step, QString(":/pic/gerberExport.png") );

  }
