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
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMRICH_H
