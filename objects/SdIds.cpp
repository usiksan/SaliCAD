/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Local and global id mechanism
  Local id's generated on session base and can repeate beatween sessions.
  Global id's generated on machine id base and never global repeate.
*/
#include "SdConfig.h"
#include "SdIds.h"
#include <QSettings>


int getGlobalId() {
  QSettings s;
  int count = s.value( SDK_GLOBAL_ID_COUNT ).toInt();
  count++;
  s.setValue( SDK_GLOBAL_ID_COUNT, count );
  return count;
  }
