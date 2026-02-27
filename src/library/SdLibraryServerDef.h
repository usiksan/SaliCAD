#ifndef SDLIBRARYSERVERDEF_H
#define SDLIBRARYSERVERDEF_H

//Fields of query
#define SDRM_TYPE                    0
#define SDRM_CLOUD_ID                1
#define SDRM_HASH_UID_NAME           2
#define SDRM_CREATE_TIME             3
#define SDRM_CONTENT                 4
#define SDRM_LIST                    5
#define SDRM_AUTHOR_PUBLIC_KEY       6
#define SDRM_AUTHOR_PRIVATE_KEY      7
#define SDRM_AUTHOR_NAME             8
#define SDRM_AUTHOR_EMAIL            9
#define SDRM_AUTH_TOTAL_COUNT       10
#define SDRM_AUTH_COMPL_COUNT       11
#define SDRM_AUTH_TOTAL_COMPL_COUNT 12
#define SDRM_COMPLIANT_MESSAGE      13
#define SDRM_AUTHOR_LIST            14

#define SDRM_TYPE_FAIL               0
#define SDRM_TYPE_OK                 1 //!< OK Accnowledge
#define SDRM_TYPE_CHECK              2 //!< Check cloud storage is need to sync object or not
#define SDRM_TYPE_GET                3 //!< Need to receiv object
#define SDRM_TYPE_OBJECT             5 //!< Object data
#define SDRM_TYPE_GET_LIST           6 //!< Query list of newest files
#define SDRM_TYPE_LIST               7
#define SDRM_TYPE_REGISTER           8 //!< Set or change author global name
#define SDRM_TYPE_INVALID_KEY_PAIR   9 //!< Author public key and author private key not equals recorded in db
#define SDRM_TYPE_INVALID_NAME      10 //!< Assigned name always used by other author
#define SDRM_TYPE_COMPLIANT_LIST    11
#define SDRM_TYPE_GET_AUTHOR_LIST   12
#define SDRM_TYPE_AUTHOR_LIST       13
#define SDRM_TYPE_REGISTER_GET      14


#define SD_GLOBAL_STORAGE_PORT 1971
#define SD_PRIVATE_CLOUD_PORT  1972


//Hash uid generator for name result length
#define SD_HASH_UID_NAME_LEN     27

#endif // SDLIBRARYSERVERDEF_H
