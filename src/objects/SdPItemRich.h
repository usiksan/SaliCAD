/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rich and plain text representation
*/
#ifndef SDPITEMRICH_H
#define SDPITEMRICH_H

#include "SdProjectItem.h"

#define SD_TYPE_RICH "Rich"

class QTextEdit;

class SdPItemRich : public SdProjectItem
  {
    //Text contents
    QString    mContents;

    //Do not saved
    //Editor which edit contents. Used to retrive string from editor when write object
    QTextEdit *mTextEditor;
  public:
    SdPItemRich();

    //Set or reset visual editor
    void setEditor( QTextEdit *ed ) { mTextEditor = ed; }

    //Get current contents
    QString contents() const { return mContents; }
    //Set new contents
    void    setContents( const QString &str ) { mContents = str; }

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMRICH_H
