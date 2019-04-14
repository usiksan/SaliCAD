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

class SdProject;

class SdDGetBus : public QDialog
  {
    Q_OBJECT

    QStringList   mNets;    //Net list of bus
    bool          mFirst;   //True while bus edit line not changed
  public:
    explicit SdDGetBus( SdProject *prj, QWidget *parent = nullptr);
    ~SdDGetBus() override;

    QStringList busList() const { return mNets; }

  private:
    Ui::SdDGetBus *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;

  private:
    //Net list comma separated string parsed to string list
    bool translation(const QString sour);

    //Extract number position before delimiter.
    int  checkDigit( const QString buf, int index, QChar delim );

    //Display net list syntax error
    bool syntaxError();
  };

#endif // SDDGETBUS_H
