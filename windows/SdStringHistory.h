/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Represent history of strings.
  Support history limitation, recently used
*/
#ifndef SDSTRINGHISTORY_H
#define SDSTRINGHISTORY_H

#include <QStringList>
#include <QComboBox>

class SdStringHistory : public QStringList
  {
    int         mPrecision; //Double to string precision
    int         mLimit;     //History limit
  public:
    SdStringHistory( int limit = 30, int precision = 3 );

    //Append string to history
    //Return index which index must removed from history
    int addString( const QString str );

    //Append string representation of double to history
    //This function normalize string to double
    //Return index which index must removed from history
    int addDoubleString( const QString str );

    //Append double.
    //Converted double to string representation and append as string
    //Return index which index must removed from history
    int addDouble( double val );

    //Entered or selected new current string in combo box
    //With this fucntion we reorder stringHistory itself and comboBox
    void reorderComboBoxString( QComboBox *box );


    //Entered or selected new current string represented double in combo box
    //With this fucntion we reorder stringHistory itself and comboBox
    void reorderComboBoxDoubleString( QComboBox *box );
  };

#endif // SDSTRINGHISTORY_H
