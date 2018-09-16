/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDROWVALUE_H
#define SDDROWVALUE_H

#include "library/SdStringMap.h"

#include <QDialog>

namespace Ui {
  class SdDRowValue;
}


struct SdValueModifier {
    const char *mModifier;
    double      mMin;
    double      mMax;
    double      mFactor;
  };

extern const SdValueModifier sdValueModifierOm[];
extern const SdValueModifier sdValueModifierFarad[];
extern const SdValueModifier sdValueModifierHenry[];

class SdDRowValue : public QDialog
  {
    Q_OBJECT

    const SdValueModifier *mValueModifierList;
    SdStringMap           &mMap;
  public:
    explicit SdDRowValue(SdStringMap &map, const SdValueModifier *list, QWidget *parent = nullptr);
    ~SdDRowValue();

    static double valueToDouble(const QString &val, const SdValueModifier list[] );

  public slots:
    void onModifierChanged( int row );

  private:
    Ui::SdDRowValue *ui;
  };

#endif // SDDROWVALUE_H
