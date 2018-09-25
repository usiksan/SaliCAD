/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  The SdCsAuthor struct represents one user at global space
*/
#ifndef SDCSAUTHORTABLE_H
#define SDCSAUTHORTABLE_H

#include <QString>
#include <QDataStream>
#include <QSet>
#include <QMap>
#include <QReadWriteLock>


/*!
   \brief The SdCsAuthor struct represents one user at global space

   To perform global object storage we need to separate users one to another. This
   need for to trust some users and fix another in black list. One user may be in fact
   a group of users. Also user can have many computers. All computers of one user
   have individual key. So, we register individual user and all machines of this user.

   To register new machine user must enter key of existing machine.
 */
struct SdCsAuthor {
    QString       mEmail;         //User email for key restoring
    qint32        mRemainObject;  //Object count available for loading
    qint32        mMaxMachines;   //Max available machines count for this user
    QSet<quint64> mMachineKeys;   //Author machine keys
  };

inline QDataStream &operator << ( QDataStream &os, const SdCsAuthor &author ) {
  os << author.mEmail << author.mRemainObject << author.mMaxMachines << author.mMachineKeys;
  return os;
  }

inline QDataStream &operator >> ( QDataStream &is, SdCsAuthor &author ) {
  is >> author.mEmail >> author.mRemainObject >> author.mMaxMachines >> author.mMachineKeys;
  return is;
  }




/*!
   \brief The SdCsAuthorTable class represents list of registered users
 */
class SdCsAuthorTable
  {
    QReadWriteLock           mLock;     //Lock semaphor
    QMap<QString,SdCsAuthor> mUserList; //Author table
    bool                     mDirty;    //If true then save needed
    QString                  mPath;
  public:
    SdCsAuthorTable();

    bool isDirty() const { return mDirty; }

    /*!
       \brief registerAuthor Register new author
       \param author Author name
       \param email Author email to restore machine key
       \param maxObject Maximum object count available for loading
       \param key New key for first author machine
       \return True if registration successfull or false if its not
     */
    bool registerAuthor(const QString author, const QString email, int maxMachines, int maxObject, quint64 *key );


    /*!
       \brief registerMachine Register new machine for this author
       \param author Author whose machine registering
       \param key On in key for any registered author's machines.
        On out key for new machine or zero if machine limit reached.
       \return True if new machine registered or false if not.
     */
    bool registerMachine( const QString author, quint64 *key );


    /*!
       \brief login Check for login author with key
       \param author Author whose login
       \param key Author key for login
       \return True if author with this key can login
     */
    bool login( const QString author, quint64 key );


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
     */
    void save();

  private:
    bool registerMachinePrivate(const QString author, quint64 *key );
  };

extern SdCsAuthorTable sdCsAuthorTable;

#endif // SDCSAUTHORTABLE_H
