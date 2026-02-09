#ifndef SDLIBRARYSERVERDEF_H
#define SDLIBRARYSERVERDEF_H

//Fields of query
#define SDRM_TYPE                 0
#define SDRM_HASH_UID_NAME        1
#define SDRM_CREATE_TIME          2
#define SDRM_CONTENT              3
#define SDRM_LIST                 4

#define SDRM_TYPE_FAIL            0
#define SDRM_TYPE_OK              1 //!< OK Accnowledge
#define SDRM_TYPE_CHECK           2 //!< Check cloud storage is need to sync object or not
#define SDRM_TYPE_GET             3 //!<
#define SDRM_TYPE_REMOVE          4 //!< Remove file from cloud
#define SDRM_TYPE_OBJECT          5
#define SDRM_TYPE_GET_LIST        6 //!< Query list of newest files
#define SDRM_TYPE_LIST            7

#define SD_GLOBAL_STORAGE_PORT 1971
#define SD_PRIVATE_CLOUD_PORT  1972

#endif // SDLIBRARYSERVERDEF_H
