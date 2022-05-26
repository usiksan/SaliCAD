#ifndef SDCONTAINERFILE_H
#define SDCONTAINERFILE_H

#include "SdContainer.h"
#include "library/SdLibraryHeader.h"

class SdContainerFile : public SdContainer
  {
  protected:
    QString                mTitle;      //!< Item title
    QString                mAuthor;     //!< Item author (registered program copy name)
    int                    mCreateTime; //!< Create time with sec from 2000year
    bool                   mEditEnable; //!< True if edit enable for this object
  public:
    bool                   mThereNewer; //!< In library present newer object
  public:
    SdContainerFile();

    //Information
    //Unical ident of item accross world
    QString                getUid() const;
    QString                getExtendTitle() const;
    QString                getAuthor() const { return mAuthor; }
    int                    getTime() const { return mCreateTime; }
    QString                getTitle() const { return mTitle; }
    virtual void           getHeader( SdLibraryHeader &hdr ) const;
    //Get editEnable flag
    bool                   isEditEnable() const { return mEditEnable; }
    //Check if another author
    bool                   isAnotherAuthor() const;

    void                   titleSet(const QString title );

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void           json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void           json( const SdJsonReader &js ) override;

    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void           cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;


    //Return current registered author
    static  QString        getDefaultAuthor();

  protected:
    //Set creation time as current
    void                   updateCreationTime();
    //Set author as current
    void                   updateAuthor();

  };

#endif // SDCONTAINERFILE_H
