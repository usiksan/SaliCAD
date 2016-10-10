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

quint64 getLocalId() {
  static quint64 localId = Q_UINT64_C(0x1);
  return localId++;
  }

quint64 getGlobalId() {
  QSettings s;
  quint64 base = s.value( SDK_GLOBAL_ID_MACHINE ).toULongLong();
  quint64 count = s.value( SDK_GLOBAL_ID_COUNT ).toUInt();
  base |= count;
  s.setValue( SDK_GLOBAL_ID_COUNT, (unsigned)count );
  return base;
  }
