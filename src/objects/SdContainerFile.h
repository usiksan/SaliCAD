#ifndef SDCONTAINERFILE_H
#define SDCONTAINERFILE_H

#include "SdContainer.h"
#include "library/SdLibraryHeader.h"

class SdContainerFile : public SdContainer
  {
  protected:
    QString                mTitle;       //!< Item title
    QString                mAuthorKey;   //!< Public item author Key (uid)
    QString                mHashUidName; //!< Hash for unical name of object as combination object type, object title and objects author public key
    int                    mCreateTime;  //!< Create time with sec from 2000year
    bool                   mEditEnable;  //!< True if edit enable for this object
    bool                   mIsPublic;    //!< True if object is public
  public:
    bool                   mThereNewer;  //!< In library present newer object
  public:
    SdContainerFile();

    //Information
    //!
    //! \brief hashUidName Builds hash for unical name of object as combination object type, object title and objects author public key
    //! \return            Hash for unical name
    //!
    QString                hashUidName() const { return mHashUidName; }

    //!
    //! \brief hashUidFile Builds hash for file name (without extension) of object as combination hashUidName and object version
    //! \return            Hash as unical file name
    //!
    QString                hashUidFile() const { return hashUidFile( hashUidName(), mCreateTime ); }

    //!
    //! \brief getUidName Builds world unical name of object  as combination object type, object title and objects author public key
    //!                   for previous version of SaliCAD
    //! \return           Worlds unical name of object
    //!
    QString                getUidName() const;

    QString                getExtendTitle() const;
    QString                getAuthorKey() const { return mAuthorKey; }
    QString                authorGlobalName() const { return authorGlobalName(mAuthorKey); }
    int                    getTime() const { return mCreateTime; }
    QString                getTitle() const { return mTitle; }
    virtual void           getHeader( SdLibraryHeader &hdr ) const;
    //Get editEnable flag
    bool                   isEditEnable() const { return mEditEnable; }
    //Check if another author
    bool                   isAnotherAuthor() const;

    //!
    //! \brief titleSet Sets new title for object
    //! \param title    New title for object
    //!
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
    static QString         getDefaultAuthor();

    //!
    //! \brief hashUidName Builds hash code for unical name of object which consists from three parts
    //! \param objectType  Object type
    //! \param objectTitle Object title
    //! \param authorKey   Author for object
    //! \return            hex hash code
    //!
    static QString         hashUidName( const QString &objectType, const QString &objectTitle, const QString &authorKey );

    //!
    //! \brief hashUidFile Builds hash uid code for file name of object on base unical hash code of name and version
    //! \param hashUidName Unical hash code of object name
    //! \param timeVersion Object version (object time creation)
    //! \return            Full file name for object
    //!
    static QString         hashUidFile( const QString &hashUidName, int timeVersion );

    //!
    //! \brief hashUidPath Build file path from hash uid code name and time
    //! \param hashUidName Unical hash code of object name
    //! \param timeVersion Object version (object time creation)
    //! \return            Path to file of object with directory structure
    //!
    static QString         hashUidPath( const QString &hashUidName, int timeVersion );

    //!
    //! \brief authorGlobalName Retrive author global name if registered, otherwise return "anonim" or "yourown"
    //! \param publicAuthorKey  Public author key
    //! \return                 Author global name
    //!
    static QString         authorGlobalName( const QString &publicAuthorKey );

  protected:
    //Set creation time as current
    void                   updateCreationTime();

    //!
    //! \brief updateAuthorAndHash Set author as current and calculate new hash
    //!
    void                   updateAuthorAndHash();

  };

#endif // SDCONTAINERFILE_H
