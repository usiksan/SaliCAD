/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPNEWPROJECTITEM_PARTMASTER_H
#define SDPNEWPROJECTITEM_PARTMASTER_H

#include "objects/SdProjectItem.h"
#include "objects/SdProject.h"
#include <QWizardPage>
#include <QLabel>
#include <QLineEdit>


class SdPNewProjectItem_PartMaster : public QWizardPage
  {
    Q_OBJECT

    SdProjectItem *mItem;
    SdProject     *mProject;
    QLineEdit     *mPartX;
    QLineEdit     *mPartY;
    QLineEdit     *mPinCount;

    QLineEdit     *mLeftX;
    QLineEdit     *mLeftCount;
    QLineEdit     *mLeftOffsetY;
    QLineEdit     *mLeftDistanceY;
    QLineEdit     *mLeftPinType;

    QLineEdit     *mBottomY;
    QLineEdit     *mBottomCount;
    QLineEdit     *mBottomOffsetX;
    QLineEdit     *mBottomDistanceX;
    QLineEdit     *mBottomPinType;

    QLineEdit     *mTopY;
    QLineEdit     *mTopCount;
    QLineEdit     *mTopOffsetX;
    QLineEdit     *mTopDistanceX;

    QLineEdit     *mRightX;

  public:
    SdPNewProjectItem_PartMaster( SdProjectItem *item, SdProject *prj, QWidget *parent = nullptr );

    virtual bool validatePage() override;
    virtual int  nextId() const override;
  public slots:
    void onTextChanged( const QString name );

  };

#endif // SDPNEWPROJECTITEM_PARTMASTER_H
