/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for bus enter
*/
#ifndef SDDGETBUS_H
#define SDDGETBUS_H

#include <QDialog>

namespace Ui {
class SdDGetBus;
}

class SdDGetBus : public QDialog
  {
    Q_OBJECT

    QStringList mNets;
  public:
    explicit SdDGetBus(QWidget *parent = nullptr);
    ~SdDGetBus() override;

    QStringList busList() const { return mNets; }

  private:
    Ui::SdDGetBus *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;

  private:
    bool translation(const QString sour);
    int  checkDigit( const QString buf, int index, QChar delim );
    bool syntaxError();
  };

#endif // SDDGETBUS_H
