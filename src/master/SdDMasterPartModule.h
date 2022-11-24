#ifndef SDDMASTERPARTMODULE_H
#define SDDMASTERPARTMODULE_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartModule;
}

class SdDMasterPartModule : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartModule(SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartModule() override;

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  protected:
    virtual void changeEvent(QEvent *e) override;

  private:
    Ui::SdDMasterPartModule *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    //When accept we build part with current params
    virtual void accept() override;
  };

#endif // SDDMASTERPARTMODULE_H
