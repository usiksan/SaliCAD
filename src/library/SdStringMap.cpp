/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdStringMap.h"



//Union two string maps. All keys from source map append and replace keys in dest map.
void sdStringMapUnion(const SdStringMap &src, SdStringMap &dst)
  {
  for( auto i = src.cbegin(); i != src.cend(); i++ )
    dst.insert( i.key(), i.value() );
  }
