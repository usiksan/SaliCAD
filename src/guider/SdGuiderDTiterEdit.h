#ifndef SDGUIDERDTITEREDIT_H
#define SDGUIDERDTITEREDIT_H

#include "SdGuiderFile.h"
#include <QDialog>

namespace Ui {
  class SdGuiderDTiterEdit;
  }

class SdGuiderDTiterEdit : public QDialog
  {
    Q_OBJECT

    SdGuiderTiter *mTiter;
  public:
    explicit SdGuiderDTiterEdit( SdGuiderTiter *titer, QWidget *parent = nullptr);
    ~SdGuiderDTiterEdit();

  public slots:
    void onChanged( int row, int column );
    void onAppend();
    void onDelete();

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdGuiderDTiterEdit *ui;
  };

#endif // SDGUIDERDTITEREDIT_H
