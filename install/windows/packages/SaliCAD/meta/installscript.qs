
  function Component()
  {
      // default constructor
  }

  Component.prototype.createOperations = function()
  {
      // call default implementation to actually install
      component.createOperations();

      if (systemInfo.kernelType === "linux") {
          //Create desktop entry for SaliCAD
          component.addOperation("CreateDesktopEntry", "salicad.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SaliCAD\nExec=@TargetDir@/SaliCAD.sh");
          //Create desktop entry for delete salicad
          component.addOperation("CreateDesktopEntry", "salicaddel.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SaliCAD Uninstall\nExec=@TargetDir@/maintenancetool");
      }
      
      if( systemInfo.kernelType === "winnt" ) {
        //Create file association
        component.addOperation("RegisterFileType", "salicad", "@TargetDir@/SaliCAD.exe '%1'", "SaliCAD project file" );
        //Create menu
        component.addOperation("CreateShortcut", "@TargetDir@/SaliCAD.exe", "@StartMenuDir@/SaliCAD.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/SaliCAD.exe", "description=Start SaliCAD schematic and pcb layout CAD" );
        component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/RemoveSaliCAD.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/maintenancetool.exe", "description=Remove SaliCAD from computer" );
        }


  }

