# SaliCAD
Integrated schematic and pcb layout multiplatform CAD

SaliCAD is a schematic and PCB layout CAD system. It is intended for creating and editing schematic diagrams for electronic devices, automatically creation PCB elements placement, editing PCB elements placement, handle and semiautomatic wiring layout creation and edition. In all modes it support maximum intelligence for minimization handle work.

About 20 years ago I begin creation this system. For 15 year ago at common it was completed on Windows platform with Borland BC5 IDE. It's name was PasCAD. It has help system, animated teach module with audio explanation. Interface only Russian. For next years system progress to support more and more functions. But at now I move to Linux and last 7 year I work on Qt. So, a time ago I choose to rewrite system on Qt with multiplatform support, remote component base repository, JSON file format for interplatform and Internet changeover, multilanguage support and so on.

There are four projects:
src/SaliCAD.pro - main work project SaliCAD
src/Illustrator.pro - test project for debug illustrator which integrated into SaliCAD and used
                      with wizards for preview display generating
src/SaliCaptureEditor.pro - project SaliCAD has integrated system for screen capture and store
                        video guide. SaliCaptureEditor used for append to video titres and
                        its translations.
src/SaliCadServer/SaliCadServer.pro - project for remote repository server. This server used
                                  for creation central repository of component library


Directory structure:
category - library category grouping information
help - Help system html and png files for SaliCAD
  help/guider - Multimedia guider files for SaliCAD help and presentation
install - Installator build script. We use QtInstaller
libFiles - SaliCAD library files
pattern - Layers list pattern for Gerber export and other cases
src - Source
  src/guider - Source. SaliCAD guider player, recorder and editor
  src/library - Source. Library access subsystem. Used in SaliCAD and SaliCadServer
  src/master - Source. Masters for SaliCAD
  src/modes - Source. Modes for graphics editor of SaliCAD
  src/objects - Source. Object hierarchy of SaliCAD
  src/pic - Source. Icons and pictures used in resource of SaliCAD
  src/SaliCadServer - Source. Project for SaliCadServer - remote library manager
  src/windows - Source
