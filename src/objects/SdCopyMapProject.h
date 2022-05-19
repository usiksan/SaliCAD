#ifndef SDCOPYMAPPROJECT_H
#define SDCOPYMAPPROJECT_H

#include "SdCopyMap.h"
#include "SdProject.h"

class SdCopyMapProject : public SdCopyMap
  {
    SdProject   *mProject;    //!< When copy SdProjectItem object then return its local copy fro project
    SdStringMap  mNetCopyMap; //!< When copy schematic fragment named nets copied exactly.
                              //!< But default named nets must be copied with new default names
                              //!< This map stores association inserted net name and new net name
  public:
    SdCopyMapProject( SdProject *prj );

    // SdCopyMap interface
  public:
    virtual SdObjectPtr copy(const SdObjectPtr src, bool next) override;
  };

#endif // SDCOPYMAPPROJECT_H
