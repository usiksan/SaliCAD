/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  STEP file reader and 3d model creator
*/
#ifndef SDSTEPREADER_H
#define SDSTEPREADER_H

#include <QString>
#include <QMap>
#include <QIODevice>

class SdStepReader
  {
    QMap<QString,QString> mStepMap; //! Step file map (#id = contents)
  public:
    SdStepReader();

    bool read( QIODevice *in );

    QString     value( const QString &id ) const { return mStepMap.value(id); }

    QStringList styledItemList() const;


    QStringList parseList( const QString line ) const;

    QStringList parseListFromValue( const QString &id ) const;

  private:
    QStringList itemList( const QString itemName ) const;
  };

#endif // SDSTEPREADER_H
