/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCSAUTHORTABLE_H
#define SDCSAUTHORTABLE_H

#include <QString>
#include <QDataStream>
#include <QSet>
#include <QMap>
#include <QReadWriteLock>

struct SdCsAuthor {
    qint32       mRemainObject;  //Object count available for loading
    qint32       mMaxMachines;
    QSet<qint64> mMachineKeys;   //Author machine keys
  };



class SdCsAuthorTable
  {
    QReadWriteLock           mLock;
    QMap<QString,SdCsAuthor> mUserList; //Author table
  public:
    SdCsAuthorTable();

    /*!
       \brief registerAuthor Register new author
       \param author Author name
       \param maxObject Maximum object count available for loading
       \param key New key for first author machine
       \return True if registration successfull or false if its not
     */
    bool registerAuthor( const QString author, int maxMachines, int maxObject, qint64 *key );


    /*!
       \brief registerMachine Register new machine for this author
       \param author Author whose machine registering
       \param key On in key for any registered author's machines.
        On out key for new machine or zero if machine limit reached.
       \return True if new machine registered or false if not.
     */
    bool registerMachine( const QString author, qint64 *key );


    /*!
       \brief login Check for login author with key
       \param author Author whose login
       \param key Author key for login
       \return True if author with this key can login
     */
    bool login( const QString author, qint64 key );


    /*!
       \brief remainObject Return count of remain objects for loading
       \param Author whose remain count return
       \return Remain object count
     */
    int  remainObject( const QString author );


    /*!
       \brief decrementObject Decrement remain object count and return new one
       \param author Author whose remain object need to be decremented
       \return New remain object count after decrement
     */
    int  decrementObject( const QString author );


    /*!
       \brief load Load author table
       \param path Path where author table is reside
     */
    void load( const QString path );

    /*!
       \brief save Save author table
       \param path Path where author table is reside
     */
    void save( const QString path );

  };

extern SdCsAuthorTable sdCsAuthorTable;

#endif // SDCSAUTHORTABLE_H
