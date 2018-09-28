/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  View and edit variant param table
*/
#ifndef SDDVARIANTTABLE_H
#define SDDVARIANTTABLE_H

#include "objects/SdPItemVariant.h"

#include <QDialog>

namespace Ui {
  class SdDVariantTable;
  }

class SdDVariantTable : public QDialog
  {
    Q_OBJECT

    SdPItemVariant *mVariant;           //Object with variant table
    SdStringMap     mDefFields;         //Default fields table
    qint32          mVariantFieldCount; //Variant table fields count
    QStringList     mVariantTable;      //Variant table itself
  public:
    explicit SdDVariantTable( SdPItemVariant *var, bool editEna, QWidget *parent = nullptr);
    ~SdDVariantTable() override;

  public slots:
    //Append field to default fields table
//    void defFieldAppend();

    //Delete field from default fields table
//    void defFieldDelete();

    //Append field to variant table
    void variantFieldAppend();

    //Delete field from variant table
    void variantFieldDelete();

    //Insert row to variant table
    void rowInsert();

    //Delete row from variant table
    void rowDelete();

    //When edited def parameter
    void defChanged( int row, int column );
  protected:
    virtual void changeEvent(QEvent *e) override;

  private:
    Ui::SdDVariantTable *ui;

    //Build visual default param table
    void buildDefTable();

    //Build visual variant table
    void buildVariantTable();

    //Append field at position pos with name
    void variantFieldAppendInt(const QString name );

    //Check if field present in variant table
    bool isVariantField( const QString field );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDVARIANTTABLE_H
