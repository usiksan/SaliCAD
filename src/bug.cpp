
//B079 bugs
//F001 fatal
//C001 critical
//D094 do




//TODO B046 When renumeration in browse mode no rebuild component list

//TODO B047 Append layer list to all properties bars

//TODO D060 Append object replacing capability and autoreplacing

//TODO D064 Helper graph objects for other graph objects positioning

//TODO D067 Append query to upgrade when loading project if there are upgrading

//TODO D068 Append query to upgrade when newer objects received from remote and possibility to off than query

//TODO D078 Append automatic upgrade files from central repository

//TODO D082 Append navigation to help window on right side

//TODO D083 Append library & repository command to instruments

//TODO B067 Mark as remark all uncompleted links in help files

//TODO D084 Append record indicator to main window

//TODO D091 Append mode for delete trace objects with F6 to switch object behind cursor

//TODO D092 Append mode to fix component and make untouchable for moving and selecting

//TODO B071 Some time do not fit to view (in graph view) and picture too small

//TODO B073 In 3d editor object not fit when view starts

//TODO B074 In 3d masters object not drawn in preview

//===================================================================================
//There are no need:

//B069 Not changed vertex mode in polygon

//D059 Append file generation on every object, category selector on file hierarchy base


//===================================================================================
//Fixed:

//B076 When undo 3d operation then open 2d editor

//B078 Appending 3d object not set project as dirty

//B075 When mode activated in 3d mode button not checked

//B077 Add deactivate 3d mode by right button

//D093 Simplify 3d open from project tree widget, also in locked mode

//D085 Append rotation command to select menu

//B072 In part master for double side vertical not created first pin marker

//B073 In category path must be trailing / else no categories readed

//D086 Append automatic form selection when sheet creation

//D093 When append default component copy to it category from symbol

//D052 mask components by side

//D088 Append backspace key to region and polygon modes

//D089 Append category selection to component editor

//D090 In symbol and part editors text must be display only as field and never context

//B070 Check empty path when gerber packet export

//B068 Copy id and value not allowed

//D081 Append link and sync with repository indication to main window (with tool tip)

//B087 Prevent open same file more then one time

//B065 When upgrading component don't changed ident (and perhaps - value)

//Remove inheritance object, instead of it we use variant table

//Set focus to plate editor window when component selected by id

//When delete schematic sheet and plate we remove all contents for link removing

//Enter bus net from named net list when first selection must delete previous net list

//Append to ic symbol master keyboard commands

//D079 Append connection marker to registration dialog

//D080 Append registration marker to registration dialog

//D044 Append possibilities to ucom set of pins by select them with rect

//D063 Append project component list (bom) and report generator

//B067 When activate part place bar grid align state not updated

//B055 No road connection to middle of existing segment

//D070 Save state of polygon

//B066 When selected by rect and rect over wire segment on middle, then no selection

//D076 Append information with unconnected nets and rule conflict count

//D075 When activated net name property in net properties bar select current net name

//D074 When activated bus dialog select previous net list

//D071 When change param to project dirtyProject do not changed

//D054 append arc implementation to Gerber export

//D046 When try to select pin type from default pad association table and no default table offer to enter one

//D073 Remove default pad association from environment

//D074 Replace pad selection for vias with pad building by pad master

//D072 Text doc not saved until closed

//B050 snap point for polygon

//D069 Append to enter polygon mode smart to retrieve polygon net name

//D066 Append tool tips in project tree view with status description

//B064 Header list contains many objects with same time

//D050 Append component rotation by keyboard

//D062 Append text item

//D005 text doc

//D061 Append project preview editor

//D065 Append indication edit state to icon

//B009 3 undo's on one text properties changes

//B016 insertComponent mode: when select component and after select symbol part remain from component

//B062 When multiple deleting object from project undo restore it with one click

//B063 For selected object properties layer must be common

//D043 Append possibility to delete object from library and also from remote library

//B061 When deactivate select mode all must be unselected

//B036 When duplicate object to edit state it not positioned in project window tree

//B038 When undo sheet insert editor not removed

//B059 When change active project undo actions not updated

//B060 When select tab in project tree not selected appropriate item

//D053 Append to context menu of project "copy selected" command

//D058 Append copy-paste for project objects

//B058 Remove cursor viewing in view editor

//B056 When help window editor appended to tab it not top

//B057 Disable selectValue button when edit disable

//D057 Append undo rules

//B026 When cancel close programm

//D055 append circle implementation to Gerber export

//B045 When cancel mode part pin current step remains after cancel but must be reset

//B051 Over rect for part implement calculated with ident, but must be without

//B040 After smart net insertion no segment utilisation performed

//B041 In active text mode +-* buttons not supported

//B035 When undo in plate we need to rebuild rat net

//B054 When enter road we setup road width, but when enter first point with restored from internal
//Road width restored from road rules.

//D057 Append to road properties bar cursor grid align selection

//B053 When selected by rect then diagonal wrong roads selected

//B019 in trace mode pads shown not at place

//B020 in trace mode show smart start point

//B052 Not available to enter custom file name, only select

//D037 snap point for roads

//B039 Can't delete section when press Delete button

//B042 Wrong renumeration with multisection component with sections in separate sheets

//B043 Wrong size of scroll bar when zoomed

//B044 Editor options not applied

//B035 When close project incorect deleting from comboBox

//B037 Reverse renumeration sorting algorithm

//D048 When deleting implements perform check reference for symbol, part and component

//D010 snap partImp

//B034 When copy mirrored symImp sometimes changes to nonmirrored

//D045 When press lamp icon in layer table switch layer state on-off

//B028 First line of section in component section list is empty

//B014 previous setuped text size don't set. Only after begin editing

//B003 copy in sheet
//При копировании в sheet нету вставки

//B015 on edit disable update time and set need update link on prototipe

//D042 Append posibility to load object to project for edit

//B029 Duplicate components in GetObject dialog

//B035 When copy sheet wires need copy nextWires for default wires

//B012 when moving symImp and stop, when connecting to sheet wires - abnormal finish

//B027 When copy param into component nothing done

//B023 When copy component in schematic link with part not correct (rat net not builded)

//B030 Wrong components in list of GetObject dialog

//B032 When partImp rotated overRect not rebuild

//B031 Error in comboBox of partImp rotation

//B033 After closing pad editor need redraw

//B022 With press close icon graph editor do not closing

//D009 snap region

//D040 show close point on region as smart point

//B024 Not connected pin locks display pads

//D014 add snap to area

//B025 When moving cursor in select mode do not displaing bom behind cursor component

//D023 getInfo in select mode

//D024 getInfo partImp

//B011 when inserting sheet wires - they not connected to symImp

//D013 make BOM

//B021 after disable edit in project tree need update time

//D041 change id only name and author (without creation time).
//Creation time save in database. It allow update objects with newly created.
//When name test - use also data base objects (simple test object id)

//B018 when handle enter coord wrong axiz

//D027 partPropBar in select mode

//D026 partPropBar in select mode

//B007 arrows key to move cursor
//Использовать кнопки со стрелками для перемещения курсора

//D020 draw cursor

//D039 bottom line in illustrator invisible

//D038 limit simultaneously opened editors

//B017 When rename component name not changed in graph editor

//D029 roadPropBar in select mode

//D030 roadPropBar in select mode

//D034 draw pad

//D036 read pad association

//D035 write pad association

//D019 Set current pin type

//D002 when change name of wire

//B005 dont work wheel functions
//Не работает функция колеса

//D032 fill new hierarhy and translation list

//D025 load object from server

//D018 Check if name unical for this author

//D012 set edit status

//B013 smart mode for sheet wire start at 0,0


//B001 selection problem
//При выделении внутри прямоугольника прямоугольником выделяется прямоугольник, а не должен

//B010 when selecting line width dont setup
//При выборе линии ширина в панели не соответствует

//D028 read pad assotiation

//D001 write pad assotiation

//B008 Problem with double validation: convert use point and validate comma (for russian local).

//D021 undo deleteObject

//B006 when creating editor - dont work print (not enabled)

//B004 dont work space for smart insert
//Не работает пробел для выбора по разумному режиму

//B002 print window
//При печати края не пропечатываются. Увеличивать окно печати.


