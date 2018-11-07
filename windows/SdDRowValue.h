/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Row of values support.
  Dialog for selecting value from row.
*/
#ifndef SDDROWVALUE_H
#define SDDROWVALUE_H

#include "library/SdStringMap.h"

#include <QDialog>

namespace Ui {
  class SdDRowValue;
}


//Modifier represents modifier to base value. For example, base value is 'Om', then 'k' is modifier as 'kOm' - x1000 Om
struct SdValueModifier {
    const char *mModifier; //Name of modifier, for example 'k'
    double      mMin;      //Minimal value for modifier, for example 1000
    double      mMax;      //Maximal value for modifier, for example 1000000
    double      mFactor;   //Factor of modifier, for example 1000.0
  };

extern const SdValueModifier sdValueModifierOm[];
extern const SdValueModifier sdValueModifierFarad[];
extern const SdValueModifier sdValueModifierHenry[];

class SdDRowValue : public QDialog
  {
    Q_OBJECT

    const SdValueModifier *mValueModifierList; //Modifier list
    SdStringMap           *mMap;               //Source param map
    int                    mModStart;
    int                    mModStop;
  public:
    explicit SdDRowValue(SdStringMap *map, const SdValueModifier *list, QWidget *parent = nullptr);
    ~SdDRowValue() override;

    static double valueToDouble(const QString &val, const SdValueModifier list[] , QString *modifier);
    static bool   isPhis( const QString &val );
    static double phisToDouble( const QString &val );

  public slots:
    void onModifierChanged( int row );

  private:
    Ui::SdDRowValue *ui;

    double value( const QString &key );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDROWVALUE_H
