
//B033 bugs
//F001 fatal
//C001 critical
//D044 do



//TODO B003 copy in sheet
//При копировании в sheet нету вставки

//TODO B009 3 undo's on one text properties changes

//TODO B012 when moving symImp and stop, when connecting to sheet wires - abnormal finish

//TODO B014 previous setuped text size don't set. Only after begin editing

//TODO B015 on edit disable update time and set need update link on prototipe

//TODO B016 insertComponent mode: when select component and after select symbol part remain from component

//TODO B019 in trace mode pads shown not at place

//TODO B020 in trace mode show smart start point

//TODO D042 Append posibility to load object to project for edit

//TODO D043 Append posibility to delete object from library and also from remote library

//TODO B028 First line of section in component section list is empty

//TODO B029 Duplicate components in GetObject dialog

//===================================================================================
//Fixed:

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


