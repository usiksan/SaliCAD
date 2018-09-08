
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
  }

