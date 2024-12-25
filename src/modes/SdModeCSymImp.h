/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for symbol implement
*/
#ifndef SDMODECSYMIMP_H
#define SDMODECSYMIMP_H

#include "SdModeCommon.h"
#include "objects/SdProject.h"

class SdPItemSymbol;
class SdPItemVariant;
class SdPItemPart;

class SdModeCSymImp : public SdModeCommon
  {
    //When component insertion
    SdPoint           mOrigin;     //!< Current position of origin of section
    SdPItemSymbol    *mSection;    //!< Component schematic section for insertion
    SdPItemVariant   *mComponent;  //!< Component for insertion
    SdPItemPart      *mPart;       //!< Part of component
    SdStringMap       mParams;     //!< Exact params of component variant

    //When fragment insertion
    SdSelector        mPaste;      //!< Fragment for insertion [Врагмент для вставки (копирования)]
    SdProject        *mPastePrj;   //!< Project from which insertion [Проект, из которого производится вставка]
    SdPoint           mFirst;      //!< Start point [Начальная точка]
    SdPoint           mPrevMove;   //!< Previous point [Предыдущая точка]
    SdPoint           mCurPoint;   //!< Current point [Текущая точка]

  public:
    SdModeCSymImp( SdWEditorGraph *editor, SdProjectItem *obj );
    ~SdModeCSymImp() override;



    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint point) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    keyDown(int key, QChar ch) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    void getSection();
    void clear();

    //!
    //! \brief checkSchematicFragment Check presence of schematic fragment for mComponent,
    //!                               prompt user for select fragment
    //! \return                       true - if user selected fragment insertion
    //!
    bool checkSchematicFragment();
  };

#endif // SDMODECSYMIMP_H
