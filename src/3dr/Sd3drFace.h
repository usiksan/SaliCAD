/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face represents as list of vertex indexes of polygon in global
  vertex table of model
*/
#ifndef SD3DRFACE_H
#define SD3DRFACE_H

#include <QList>

using Sd3drFace = QList<int>;


using Sd3drFaceList = QList<Sd3drFace>;

#endif // SD3DRFACE_H
