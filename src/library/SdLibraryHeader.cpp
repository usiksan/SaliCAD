#include "SdLibraryStorage.h"




//!
//! \brief authorGlobalName Return global author name associated with mAuthorKey (public author key)
//! \return                 Global author name associated with mAuthorKey (public author key)
//!
QString SdLibraryHeader::authorGlobalName() const
  {
  return SdLibraryStorage::instance()->authorGlobalName( mAuthorKey );
  }
