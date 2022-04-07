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
#include "SdConfig.h"
#include "SdDRowValue.h"
#include "ui_SdDRowValue.h"
#include "SdDHelp.h"

#include <QPushButton>
#include <math.h>

const SdValueModifier sdValueModifierOm[] = {
  { "uOm",  0.000001, 0.001, 0.000001 },
  { "Om",   0.001, 1000.0, 1.0 },
  { "kOm",  1000.0, 1000000.0, 1000.0 },
  { "MOm",  1000000.0, 1000000000.0, 1000000.0 },
  { "GOm",  1000000000.0, 1000000000000.0, 1000000000.0 },
  { nullptr, 0.0, 0.0, 0.0 }
};



const SdValueModifier sdValueModifierFarad[] = {
  { "pF", 0.0000000000001, 0.00000001, 0.000000000001 },
  { "uF", 0.00000001, 0.1, 0.000001 },
  { nullptr, 0.0, 0.0, 0.0 }
};


const SdValueModifier sdValueModifierHenry[] = {
  { "nH",  0.000000001, 0.000001, 0.000000001 },
  { "uH",  0.000001, 0.01, 0.000001 },
  { "mH",  0.01, 1.0, 0.001 },
  { "H",   1.0, 1000.0, 1.0 },
  { nullptr, 0.0, 0.0, 0.0 }
};

const SdValueModifier sdValueModifierAmper[] = {
  { "nA",  0.000000001, 0.000001, 0.000000001 },
  { "uA",  0.000001, 0.01, 0.000001 },
  { "mA",  0.01, 1.0, 0.001 },
  { "A",   1.0, 1000000.0, 1.0 },
  { nullptr, 0.0, 0.0, 0.0 }
};


const SdValueModifier sdValueModifierVolt[] = {
  { "nV",  0.000000001, 0.000001, 0.000000001 },
  { "uV",  0.000001, 0.01, 0.000001 },
  { "mV",  0.01, 1.0, 0.001 },
  { "V",   1.0, 1000000.0, 1.0 },
  { nullptr, 0.0, 0.0, 0.0 }
};


const SdValueModifier sdValueModifierHerz[] = {
  { "Hz",   0.001, 1000.0, 1.0 },
  { "kHz",  1000.0, 1000000.0, 1000.0 },
  { "MHz",  1000000.0, 1000000000.0, 1000000.0 },
  { "GHz",  1000000000.0, 1000000000000.0, 1000000000.0 },
  { nullptr, 0.0, 0.0, 0.0 }
};



//==============================================================================
//---------------------- Ряды стандартных значений -----------------------------
//Standard row values
static
double ve3[]  = { 1.0, 2.2, 4.7, 10.0 },
       ve6[]  = { 1.0, 1.5, 2.2, 3.3, 4.7, 6.8, 10.0 },
       ve12[] = { 1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2, 10.0 },
       ve24[] = { 1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0,
                  3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1, 10.0 },
       ve48[] = { 1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47, 1.54, 1.62, 1.69,
                  1.78, 1.87, 1.96, 2.05, 2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87, 3.01,
                  3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42, 4.64, 4.87, 5.11, 5.36,
                  5.62, 5.90, 6.19, 6.49, 6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53, 10.0 },
       ve96[] = { 1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18, 1.21, 1.24, 1.27, 1.30,
                  1.33, 1.37, 1.40, 1.43, 1.47, 1.50, 1.54, 1.58, 1.62, 1.65, 1.69, 1.74,
                  1.78, 1.82, 1.87, 1.91, 1.96, 2.00, 2.05, 2.10, 2.15, 2.21, 2.26, 2.32,
                  2.37, 2.43, 2.49, 2.55, 2.61, 2.67, 2.74, 2.80, 2.87, 2.94, 3.01, 3.09,
                  3.16, 3.24, 3.32, 3.40, 3.48, 3.57, 3.65, 3.74, 3.83, 3.92, 4.02, 4.12,
                  4.22, 4.32, 4.42, 4.53, 4.64, 4.75, 4.87, 4.99, 5.11, 5.23, 5.36, 5.49,
                  5.62, 5.76, 5.90, 6.04, 6.19, 6.34, 6.49, 6.65, 6.81, 6.98, 7.15, 7.32,
                  7.50, 7.68, 7.87, 8.06, 8.25, 8.45, 8.66, 8.87, 9.09, 9.31, 9.53, 9.76, 10.0 },
       ve192[]= { 1.00, 1.01, 1.02, 1.04, 1.05, 1.06, 1.07, 1.09, 1.10, 1.11, 1.13, 1.14,
                  1.15, 1.17, 1.18, 1.20, 1.21, 1.23, 1.24, 1.26, 1.27, 1.29, 1.30, 1.32,
                  1.33, 1.35, 1.37, 1.38, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 1.50, 1.52,
                  1.54, 1.56, 1.58, 1.60, 1.62, 1.64, 1.65, 1.67, 1.69, 1.72, 1.74, 1.76,
                  1.78, 1.80, 1.82, 1.84, 1.87, 1.89, 1.91, 1.93, 1.96, 1.98, 2.00, 2.03,
                  2.05, 2.08, 2.10, 2.13, 2.15, 2.18, 2.21, 2.23, 2.26, 2.29, 2.32, 2.34,
                  2.37, 2.40, 2.43, 2.46, 2.49, 2.52, 2.55, 2.58, 2.61, 2.64, 2.67, 2.71,
                  2.74, 2.77, 2.80, 2.84, 2.87, 2.91, 2.94, 2.98, 3.01, 3.05, 3.09, 3.12,
                  3.16, 3.20, 3.24, 3.28, 3.32, 3.36, 3.40, 3.44, 3.48, 3.52, 3.57, 3.61,
                  3.65, 3.70, 3.74, 3.79, 3.83, 3.88, 3.92, 3.97, 4.02, 4.07, 4.12, 4.17,
                  4.22, 4.27, 4.32, 4.37, 4.42, 4.48, 4.53, 4.59, 4.64, 4.70, 4.75, 4.81,
                  4.87, 4.93, 4.99, 5.05, 5.11, 5.17, 5.23, 5.30, 5.36, 5.42, 5.49, 5.56,
                  5.62, 5.69, 5.76, 5.83, 5.90, 5.97, 6.04, 6.12, 6.19, 6.26, 6.34, 6.42,
                  6.49, 6.57, 6.65, 6.73, 6.81, 6.90, 6.98, 7.06, 7.15, 7.23, 7.32, 7.41,
                  7.50, 7.59, 7.68, 7.77, 7.87, 7.96, 8.06, 8.16, 8.25, 8.35, 8.45, 8.56,
                  8.66, 8.76, 8.87, 8.98, 9.09, 9.20, 9.31, 9.42, 9.53, 9.65, 9.76, 9.88, 10.0};


SdDRowValue::SdDRowValue(SdStringMap *map, const SdValueModifier *list, QWidget *parent) :
  QDialog(parent),
  mValueModifierList(list),
  mMap(map),
  ui(new Ui::SdDRowValue)
  {
  ui->setupUi(this);

  //Fill modifier list
  double minv = value( stdParamValueMin );
  double maxv = value( stdParamValueMax );
  QString strVal = map->value( stdParamValue );
  double val  = value( stdParamValue );
  //Skeep low mod's
  for( mModStart = 0; minv > mValueModifierList[mModStart].mMax && mValueModifierList[mModStart].mModifier; mModStart++ );
  //Display mod's
  for( mModStop = mModStart; maxv > mValueModifierList[mModStop].mMin && mValueModifierList[mModStop].mModifier; mModStop++ )
    ui->mModifiers->addItem( QString(mValueModifierList[mModStop].mModifier) );

  if( !strVal.isEmpty() ) {
    //Select current modifier
    for( int i = mModStart; i < mModStop; i++ )
      if( mValueModifierList[i].mMin <= val && val < mValueModifierList[i].mMax ) {
        ui->mModifiers->setCurrentRow(i - mModStart);
        //Fill values for modifier
        onModifierChanged(i - mModStart);
        break;
        }
    }

  connect( ui->mModifiers, &QListWidget::currentRowChanged, this, &SdDRowValue::onModifierChanged );

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDRowValue.htm"), this );
    } );
  }




SdDRowValue::~SdDRowValue()
  {
  delete ui;
  }




double SdDRowValue::valueToDouble(const QString &val, const SdValueModifier list[], QString *modifier )
  {
  double v = 0;
  QString src = val.simplified();
  for( int i = 0; i < src.length(); i++ )
    if( !src.at(i).isDigit() && src.at(i) != QChar('.') && src.at(i) != QChar(',') ) {
      QString strVal = src.left(i);
      strVal.replace( QChar(','), QChar('.') );
      v = strVal.toDouble();
      break;
      }

  double factor = 1.0;
  //Extract modifier
  QString mod;
  for( int i = 0; i < src.length(); i++ )
    if( src.at(i).isLetter() ) {
      mod = src.mid(i);
      break;
      }
  if( modifier )
    *modifier = mod;
  for( int i = 0; list[i].mModifier; i++ )
    if( mod == QString(list[i].mModifier) )
      factor = list[i].mFactor;
  return v * factor;
  }


struct SdMulter {
    const QString mSymbol;
    double        mMulter;
  };


double SdDRowValue::phisToDouble(const QString &val)
  {
  static SdMulter list[] = {
    { QStringLiteral(u"y"), 1.0e-24 },
    { QStringLiteral(u"и"), 1.0e-24 },
    { QStringLiteral(u"z"), 1.0e-21 },
    { QStringLiteral(u"з"), 1.0e-21 },
    { QStringLiteral(u"a"), 1.0e-18 },
    { QStringLiteral(u"а"), 1.0e-18 },
    { QStringLiteral(u"f"), 1.0e-15 },
    { QStringLiteral(u"ф"), 1.0e-15 },
    { QStringLiteral(u"p"), 0.000000000001 },
    { QStringLiteral(u"п"), 0.000000000001 },
    { QStringLiteral(u"n"), 0.000000001 },
    { QStringLiteral(u"н"), 0.000000001 },
    { QStringLiteral(u"u"), 0.000001 },
    { QStringLiteral(u"мк"), 0.000001 },
    { QStringLiteral(u"m"), 0.001 },
    { QStringLiteral(u"м"), 0.001 },
    { QStringLiteral(u"c"), 0.01 },
    { QStringLiteral(u"с"), 0.01 },
    { QStringLiteral(u"da"), 10.0 },
    { QStringLiteral(u"да"), 10.0 },
    { QStringLiteral(u"d"), 0.1 },
    { QStringLiteral(u"д"), 0.1 },
    { QStringLiteral(u"h"), 100.0 },
    { QStringLiteral(u"г"), 100.0 },
    { QStringLiteral(u"k"), 1000.0 },
    { QStringLiteral(u"к"), 1000.0 },
    { QStringLiteral(u"M"), 1000000.0 },
    { QStringLiteral(u"М"), 1000000.0 },
    { QStringLiteral(u"G"), 1000000000.0 },
    { QStringLiteral(u"Г"), 1000000000.0 },
    { QStringLiteral(u"T"), 1000000000000.0 },
    { QStringLiteral(u"Т"), 1000000000000.0 },
    { QStringLiteral(u"P"), 1.0e15 },
    { QStringLiteral(u"П"), 1.0e15 },
    { QStringLiteral(u"E"), 1.0e18 },
    { QStringLiteral(u"Э"), 1.0e18 },
    { QStringLiteral(u"Z"), 1.0e21 },
    { QStringLiteral(u"З"), 1.0e21 },
    { QStringLiteral(u"Y"), 1.0e24 },
    { QStringLiteral(u"И"), 1.0e24 },
    { QStringLiteral(""), 0 }
  };

  double v = 0;
  QString src = val.simplified();
  for( int i = 0; i < src.length(); i++ )
    if( !src.at(i).isDigit() && src.at(i) != QChar('.') && src.at(i) != QChar(',') ) {
      QString strVal = src.left(i);
      strVal.replace( QChar(','), QChar('.') );
      v = strVal.toDouble();
      break;
      }
    else if( i == src.length() - 1 ) {
      src.replace( QChar(','), QChar('.') );
      v = src.toDouble();
      }

  double factor = 1.0;
  //Extract modifier
  QString mod;
  for( int i = 0; i < src.length(); i++ )
    if( src.at(i).isLetter() ) {
      mod = src.mid(i);
      break;
      }
  for( int i = 0; !list[i].mSymbol.isEmpty(); i++ )
    if( mod.startsWith( list[i].mSymbol ) )
      factor = list[i].mMulter;
  return v * factor;
  }




QString SdDRowValue::doubleToPhis(double val, const QString &modifier, const QString &row)
  {
  const SdValueModifier *mod = nullptr;
  if( modifier == QStringLiteral("Om") )     mod = sdValueModifierOm;
  else if( modifier == QStringLiteral("F") ) mod = sdValueModifierFarad;
  else if( modifier == QStringLiteral("H") ) mod = sdValueModifierHenry;
  else if( modifier == QStringLiteral("A") ) mod = sdValueModifierAmper;
  else if( modifier == QStringLiteral("V") ) mod = sdValueModifierVolt;
  else if( modifier == QStringLiteral("Hz") ) mod = sdValueModifierHerz;

  QString suffics(modifier);
  double divider = 1.0;
  double multer  = 1.0;

  //Find in which row hit val
  if( mod != nullptr ) {
    while( mod->mModifier != nullptr ) {
      if( mod->isHit(val) ) {
        suffics = QString(mod->mModifier);
        divider  = mod->mFactor;
        multer   = mod->mMin;
        break;
        }
      mod++;
      }
    }

  if( row.isEmpty() ) {
    //Build simple value without row ajust
    val /= divider;
    return QStringLiteral("%1 ").arg(val, 0, 'f', 2 ) + suffics;
    }

  //Ajust to row

  //Row values ptr
  double *rowv = ve3;

  //Row values count
  int     rowc = 3;

  //Row values display precision
  int     prec = 1;

  if( row == QStringLiteral("E6") ) {
    rowv = ve6;
    rowc = 6;
    prec = 1;
    }
  else if( row == QStringLiteral("E12") ) {
    rowv = ve12;
    rowc = 12;
    prec = 1;
    }
  else if( row == QStringLiteral("E24") ) {
    rowv = ve24;
    rowc = 24;
    prec = 1;
    }
  else if( row == QStringLiteral("E48") ) {
    rowv = ve48;
    rowc = 48;
    prec = 2;
    }
  else if( row == QStringLiteral("E96") ) {
    rowv = ve96;
    rowc = 96;
    prec = 2;
    }
  else if( row == QStringLiteral("E192") ) {
    rowv = ve192;
    rowc = 192;
    prec = 2;
    }
  //Build resultat
  while( rowv[0] * multer <= val ) {
    for( int i = 0; i < rowc; i++ ) {
      if( rowv[i] * multer <= val && val < rowv[i+1] * multer ) {
        //Interval found, detect nearest
        double error = qAbs( rowv[i] * multer - val );
        if( error < qAbs( rowv[i+1] * multer - val ) )
          //Lower value is nearest
          //Replace to lower value
          val = rowv[i] * multer;
        else
          //Upper value is nearest
          //Replace to upper value
          val = rowv[i+1] * multer;
        //Build value
        val /= divider;
        return QStringLiteral("%1 ").arg(val, 0, 'f', prec ) + suffics;
        }
      }
    multer *= 10.0;
    if( prec ) prec--;
    }

  //Impossible row ajusting
  //Build value
  val /= divider;
  return QStringLiteral("%1 ").arg(val, 0, 'f', 2 ) + suffics;
  }




void SdDRowValue::onModifierChanged(int row)
  {
  row += mModStart;
  ui->mValues->clear();
  double minRow = mValueModifierList[row].mMin;
  double maxRow = mValueModifierList[row].mMax;
  double factorRow = mValueModifierList[row].mFactor;

  //Row values ptr
  double *rowv = ve3;

  //Row values count
  int     rowc = 3;

  //Row values display precision
  int     prec = 1;

  double minv = value( stdParamValueMin );
  double maxv = value( stdParamValueMax );
  double curv = value( stdParamValue );
  double delta = curv / 100.0;
  QString rowe = mMap->value( stdParamValueRow );
  if( rowe == QStringLiteral("E6") ) {
    rowv = ve6;
    rowc = 6;
    prec = 1;
    }
  else if( rowe == QStringLiteral("E12") ) {
    rowv = ve12;
    rowc = 12;
    prec = 1;
    }
  else if( rowe == QStringLiteral("E24") ) {
    rowv = ve24;
    rowc = 24;
    prec = 1;
    }
  else if( rowe == QStringLiteral("E48") ) {
    rowv = ve48;
    rowc = 48;
    prec = 2;
    }
  else if( rowe == QStringLiteral("E96") ) {
    rowv = ve96;
    rowc = 96;
    prec = 2;
    }
  else if( rowe == QStringLiteral("E192") ) {
    rowv = ve192;
    rowc = 192;
    prec = 2;
    }
  double multerNorm = minRow / factorRow;
  double multer     = factorRow * multerNorm;
  if( multerNorm < 0.002 ) prec += 3;
  else if( multerNorm < 0.02 ) prec += 2;
  else if( multerNorm < 0.2 ) prec += 1;
  bool stop = false;
  QString modifier( mValueModifierList[row].mModifier );
  while( !stop ) {
    for( int i = 0; i < rowc; i++ ) {
      double val = rowv[i] * multer;
      if( val > maxv || val > maxRow ) {
        stop = true;
        break;
        }
      if( val >= minv && val >= minRow ) {
        if( i == 0 && prec )
          ui->mValues->addItem( QString( "%1 %2" ).arg(rowv[i] * multerNorm, 0, 'f', prec - 1 ).arg(modifier) );
        else
          ui->mValues->addItem( QString( "%1 %2" ).arg(rowv[i] * multerNorm, 0, 'f', prec ).arg(modifier) );
        if( fabs(val - curv) < delta )
          ui->mValues->setCurrentRow( ui->mValues->count() - 1 );
        }
      }
    multerNorm *= 10.0;
    multer *= 10.0;
    if( prec ) prec--;
    }
  }




double SdDRowValue::value(const QString &key)
  {
  return valueToDouble( mMap->value(key), mValueModifierList, nullptr );
  }



void SdDRowValue::accept()
  {
  mMap->insert( stdParamValue, ui->mValues->currentItem()->text() );
  QDialog::accept();
  }

