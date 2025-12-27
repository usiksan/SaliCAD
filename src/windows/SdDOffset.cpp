#include "SdDOffset.h"
#include "SdDHelp.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QStack>

static QStringList previousOffsetX;
static QStringList previousOffsetY;



int priority( QChar op )
  {
  if( op == '+' || op == '-' ) return 1;
  if( op == '*' || op == '/' ) return 2;
  if( op == 'q' || op == '-' ) return 3;
  if( op == '~' ) return 4;
  return 0;
  }



double applyOp( double a, double b, QChar op )
  {
  switch( op.toLatin1() ) {
    case '+' : return a + b;
    case '-' : return a - b;
    case '*' : return a * b;
    case '/' :
      if( b == 0.0 ) b = 0.00001;
      return a / b;
    }
  return 0.0;
  }


double applyUnary( double a, QChar op )
  {
  switch( op.toLatin1() ) {
    case '~' : return -a;
    case 'q' : return a * a;
    case 's' : return a >= 0 ? std::sqrt(a) : 0.0;
    }
  return a;
  }


void applyTop( QStack<double> &values, QStack<QChar> &ops )
  {
  QChar op = ops.pop();
  if( op == 'q' || op == 's' || op == '~' ) {
    double a = values.pop();
    values.push( applyUnary( a, op ) );
    }
  else {
    double b = values.pop();
    double a = values.pop();
    values.push( applyOp( a, b, op ) );
    }
  }


double eval( const QString &expr, bool *ok )
  {
  QStack<double> values;
  QStack<QChar>  ops;

  bool mayUnary = true;
  int i = 0;
  while( i < expr.length() ) {
    QChar c(expr.at(i).toLower());
    if( c.isSpace() ) {
      i++;
      continue;
      }

    if( c.isDigit() || c == QChar('.') ) {
      QString num;
      while( i < expr.length() && (expr.at(i).isDigit() || expr.at(i) == QChar('.'))) {
        num += expr.at(i);
        ++i;
        }
      values.push( num.toDouble() );
      mayUnary = false;
      continue;
      }

    if( (c == 'q' || c == 's') && mayUnary ) {
      ops.push( c );
      ++i;
      continue;
      }

    if( c == QChar('(') ) {
      ops.push( c );
      ++i;
      mayUnary = true;
      continue;
      }

    if( c == QChar(')') ) {
      while( !ops.isEmpty() && ops.top() != QChar('(') )
        applyTop( values, ops );
      if( !ops.isEmpty() ) ops.pop();

      if( !ops.isEmpty() && (ops.top() == 'q' || ops.top() == 's') )
        applyTop( values, ops );

      mayUnary = false;
      ++i;
      continue;
      }

    if( QString("+-*/").contains(c) ) {
      QChar op(c);
      if( c == '-' && mayUnary )
        op = QChar('~');
      while( !ops.isEmpty() && priority(ops.top()) >= priority(op) )
        applyTop( values, ops );
      ops.push(op);
      ++i;
      continue;
      }

    ++i;
    }
  while( !ops.isEmpty() )
    applyTop( values, ops );

  if( ok != nullptr ) *ok = !values.isEmpty();
  return values.isEmpty() ? 0.0 : values.top();
  }


SdDOffset::SdDOffset(const QString &key, QWidget *parent)
  : QDialog(parent)
  {
  //Build interface

  QVBoxLayout *vlay = new QVBoxLayout();
  QHBoxLayout *hlay = new QHBoxLayout();
  hlay->addWidget( new QLabel(tr("Offset X:")) );
  hlay->addWidget( mBoxOffsetX = new QComboBox() );
  vlay->addLayout( hlay );

  hlay = new QHBoxLayout();
  hlay->addWidget( new QLabel(tr("Offset Y:")) );
  hlay->addWidget( mBoxOffsetY = new QComboBox() );
  vlay->addLayout( hlay );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox, &QDialogButtonBox::accepted, this, &SdDOffset::accept );
  connect( dialogButtonBox, &QDialogButtonBox::rejected, this, &SdDOffset::reject );
  //Help system
  connect( dialogButtonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdDOffset.htm", this ); });


  setLayout( vlay );

  //Fill boxies
  replaceBoxX( QString{} );
  replaceBoxY( QString{} );

  mBoxOffsetX->setCurrentIndex(0);
  mBoxOffsetY->setCurrentIndex(0);

  mBoxOffsetX->setEditable(true);
  mBoxOffsetY->setEditable(true);

  if( key.isEmpty() )
    setWindowTitle( tr("Enter offset from smart point") );
  else {
    setWindowTitle( tr("Enter offset from previous point") );
    mBoxOffsetX->setCurrentText( key );
    }
  mBoxOffsetX->setFocus();
  }


void SdDOffset::accept()
  {
  QString textX, textY;
  bool mayClose = true;
  QString str = mBoxOffsetX->currentText();
  str.replace( QChar(','), QChar('.') );
  bool ok;
  mOffset.setX( str.toFloat(&ok) );
  if( !ok ) {
    mayClose = false;
    //Пробуем вычислить выражение
    mOffset.setX( eval( str, &ok ) );
    if( ok ) {
      //Выражение помещаем в историю, а в редактор - результат
      textX = QString("%1").arg( mOffset.x(), 0, 'f', 2 );
      }
    }
  replaceBoxX( str );

  str = mBoxOffsetY->currentText();
  str.replace( QChar(','), QChar('.') );
  mOffset.setY( str.toFloat(&ok) );
  if( !ok ) {
    mayClose = false;
    //Пробуем вычислить выражение
    mOffset.setY( eval( str, &ok ) );
    if( ok ) {
      //Выражение помещаем в историю, а в редактор - результат
      textY = QString("%1").arg( mOffset.y(), 0, 'f', 2 );
      }
    }
  replaceBoxY( str );

  if( mayClose ) done(1);
  else {
    if( !textX.isEmpty() )
      mBoxOffsetX->setEditText( textX );
    if( !textY.isEmpty() )
      mBoxOffsetY->setEditText( textY );
    }
  }




void SdDOffset::replaceBoxX(const QString &str)
  {
  if( !str.isEmpty() ) {
    previousOffsetX.removeAll( str );
    previousOffsetX.insert( 0, str );
    }
  else if( previousOffsetX.isEmpty() )
    previousOffsetX.append( QString("0.0") );
  mBoxOffsetX->clear();
  mBoxOffsetX->addItems( previousOffsetX );
  }




void SdDOffset::replaceBoxY(const QString &str)
  {
  if( !str.isEmpty() ) {
    previousOffsetY.removeAll( str );
    previousOffsetY.insert( 0, str );
    }
  else if( previousOffsetY.isEmpty() )
    previousOffsetY.append( QString("0.0") );
  mBoxOffsetY->clear();
  mBoxOffsetY->addItems( previousOffsetY );
  }
