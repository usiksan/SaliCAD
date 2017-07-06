/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeSelect.h"
#include "objects/SdContainerSheetNet.h"

//All prop bars
#include "windows/SdPropBarLinear.h"
#include "windows/SdPropBarPartPin.h"
#include "windows/SdPropBarSymImp.h"
#include "windows/SdPropBarSymPin.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdPropBarWire.h"

#include "windows/SdWCommand.h"

#include <QObject>

SdModeSelect::SdModeSelect(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj )
  {

  }


#if 0


//================== Итераторы =================================================

class DrawCopyIterator : public DBaseIterator<DGraphObjectPic> {
    DLayerOffsetConverter converter; //Контекст для рисования + преобразователь слоев + смещение
  public:
    DrawCopyIterator( DContext &dc, DPoint offset ) : converter( dc, dslSelected, offset ) {}

    bool operator () ( DGraphObjectPic *pic ) { pic->Draw( converter ); return true; }
  };

//Выделение объектов точкой
class SelectPointIterator : public DownIterator {
    DPoint        point;
    DSelectorPic &selector;
  public:
    SelectPointIterator( DPoint p, DSelectorPic &s ) : point(p), selector(s) {}

    bool operation( DBasePic *pic );
  };

bool
SelectPointIterator::operation( DBasePic *pic ) {
  if( !TestLookUpObject(pic) ) return true;
  DGraphObjectPic *graph = dynamic_cast<DGraphObjectPic*>(pic);
  if( graph ) graph->SelectByPoint( point, selector );
  return true;
  }


//Выделение объектов прямоугольником
class SelectRectIterator : public DownIterator {
    DRect         rect;
    DSelectorPic &selector;
  public:
    SelectRectIterator( DRect r, DSelectorPic &s ) : rect(r), selector(s) {}

    bool operation( DBasePic *pic );
  };

bool
SelectRectIterator::operation( DBasePic *pic ) {
  if( !TestLookUpObject(pic) ) return true;
  DGraphObjectPic *graph = dynamic_cast<DGraphObjectPic*>(pic);
  if( graph ) graph->SelectByRect( rect, selector );
  return true;
  }

//Подготовка переноса
class PrepareMoveIterator : public DBaseIterator<DGraphObjectPic> {
    DSelectorPic &selector;
  public:
    PrepareMoveIterator( DSelectorPic &s ) : selector(s) {}

    bool operator () ( DGraphObjectPic *pic );
  };

bool
PrepareMoveIterator::operator () ( DGraphObjectPic *pic ) {
  DWiringPic *wire = dynamic_cast<DWiringPic*>(pic);
  if( wire ) wire->PrepareMove();
  return true;
  }


class PostMoveIterator : public DBaseIterator<DGraphObjectPic> {
    DPoint           grid;
    PDSheetNetTable &table;
  public:
    PostMoveIterator( DPoint g, PDSheetNetTable &t ) : grid(g), table(t) {}

    bool operator () ( DGraphObjectPic *obj );
  };

bool
PostMoveIterator::operator () ( DGraphObjectPic *obj ) {
  DWiringPic *wire = dynamic_cast<DWiringPic*>(obj);
  if( wire ) {
    wire->MoveComplete(grid);
    // Разместить указатель на цепь в таблицу цепей
    DWirePic *w = dynamic_cast<DWirePic*>(obj);
    if( w ) {
      DSheetNetPic *net = w->GetNet();
      if( net ) {
        // Ищем цепь в таблице
        FOR_EACH( table )
          if( table[i] == net ) return true;
        // Цепи нет, добавить в таблицу
        table.Add( net );
        }
      }
    }
  return true;
  }


//================== SelectMode ======================================
DSelectMode::DSelectMode( DViewer *viewer, DGraphTopPic *obj ) :
  DMode( viewer, obj ),
  fragment(),
  state(0),
  shift(false),
  control(false),
  paste(0),
  pastePrj(0),
  netFragment( 20, 20 ) {}


DSelectMode::~DSelectMode() {
  if( GetStep() ) Unselect( false );
  }

//Отменить выделение
void
DSelectMode::Unselect( bool update ) {
  GetUndo()->AppendAction();

  //Очистить таблицу цепей
  netFragment.Clear();

  //Завершить перенос, накопить выделенные цепи
  fragment.ForEach( PostMoveIterator( GetViewer()->GetGrid(), netFragment ) );

  //Объекты становятся невыделенными
  fragment.RemoveAll();

  //Утилизировать цепи
  FOR_EACH( netFragment ) netFragment[i]->Utilise();

  GetUndo()->EndAction( 0 );

  //Обновить свойства и обновить изображение
  if( update ) GetProp();
  }

bool
DSelectMode::KeyDown( int ch, DContext &dc ) {
  switch( ch ) {
    case vkCopy     : Copy(); break;
    case vkPaste    : SetDirty(); Paste(); break;
    case vkCut      : SetDirty(); Copy(); DeleteSelected(); break;
    case vkSelAll   : SelectAll(); break;
    case vkShift    : shift = true; break;
    case vkControl  : control = true; break;
    case vkDelete   : SetDirty(); DeleteSelected(); break;
    case vkDubl     : SetDirty(); Dublicate(); break;
    case vkParam    : SetDirty(); Param(); break;
    case vkNumerate : SetDirty(); Numerate(); break;
    case vkF7 :
      SetDirty();
      //Поворот как группы компонентов
      if( fragment.GetNumber() && GetStep() == smSelPresent ) {
        dc.SetXorMode( wccDSLSelected );
        Hide( dc );  //Стереть
        PMRotateIterator rot( curPoint, DAngle(90000) );
        fragment.ForEach( rot );
        Show( dc );  //Нарисовать
        }
      break;
    default : return false;
    }
  return true;
  }

bool
DSelectMode::KeyUp( int ch, DContext& ) {
  switch( ch ) {
    case vkShift : shift = false; break;
    case vkControl : control = false; break;
    }
  return false;
  }

void
DSelectMode::DeleteSelected() {
  SetDirty();

  GetUndo()->BeginAction();

  //Произвести удаление объектов
  fragment.DeleteAll();

  GetUndo()->EndAction( "Удаление" );

  //Обновить свойства и обновить изображение
  GetProp();
  }








//Выделение прямоугольником-начало
void
DSelectMode::BeginRect( DPoint point, DContext &dc ) {
  first = point;
  prevMove = point;
  SetStep( smSelRect );
  dc.SetXorMode( wccDSLSelected );
  ShowRect( dc );
  }

//Выделение прямоугольником-процесс выделения
void
DSelectMode::DragRect( DPoint point, DContext &dc ) {
  dc.SetXorMode( wccDSLSelected );  //Режим инвертирования
  ShowRect( dc );   //Стереть
  prevMove = point; //Установить новую координату
  ShowRect( dc );   //Нарисовать
  }

//Выделение прямоугольником-завершение
void
DSelectMode::StopRect( DPoint point ) {
  //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
  if( !shift ) fragment.RemoveAll();
  //Пройтись по объектам и выделить прямоугольником
  GetBase()->ForEach( SelectRectIterator( DRect( first, point ), fragment ) );
  //Обновить свойства и обновить изображение
  GetProp();
  //Определить состояние курсора в текущей точке
  CheckPoint( point );
  }

//Подготовка переноса
void
DSelectMode::BeginMove( DPoint p ) {
  SetDirty();
  GetUndo()->BeginAction();
  //Сохранить состояние объектов до переноса
  fragment.ForEach( DISaveState() );
  //Подготовка переноса
  fragment.ForEach( PrepareMoveIterator( fragment ) );
  first = p;
  prevMove = p;
  SetStep( smMove );
  //Сохранить состояние объектов до переноса
  //fragment.ForEach( DISaveState() );
  GetUndo()->EndAction( "Перенос" );
  //Обновить изображение
  Update();
  }

void
DSelectMode::DragMove( DPoint p, DContext &dc  ) {
  dc.SetXorMode( wccDSLSelected );                   //Установить инвертирование
  ShowSelect( dc );                                  //Стереть изображение
  fragment.ForEach( MoveIterator( p - prevMove ) );  //Перемещение объектов
  prevMove = p;                                      //Установить новую текущую точку
  ShowSelect( dc );                                  //Рисовать изображение
  }

void
DSelectMode::StopMove( DPoint p, DContext &dc ) {
  DragMove( p, dc );
  SetStep( smSelPresent );
  }

void
DSelectMode::BeginCopy( DPoint p ) {
  SetDirty();
  //Поскольку это копирование, то нужно выделить объекты целиком (пользователь мог выделить и частично)
  fragment.ForEach( SelectAllGraphIterator( fragment ) );
  fragment.SetOrigin( p );
  if( paste ) throw CadError( "DSelectMode::BeginCopy:" );
  paste = &fragment;
  first = p;
  prevMove = p;
  SetStep( smCopy );
  //Пусть рисуется заново
  Update();
  }

void
DSelectMode::DragCopy( DPoint p, DContext &dc ) {
  dc.SetXorMode( wccDSLSelected );                     //Установить инвертирование
  ShowPaste( dc );                                     //Стереть изображение
  if( !paste ) throw CadError("DSelectMode::DragCopy");
  prevMove = p;                                        //Установить новую текущую точку
  ShowPaste( dc );                                     //Рисовать изображение
  }

void
DSelectMode::InsertCopy( DPoint offset, bool next ) {
  SetDirty();
  if( !paste ) throw CadError( "DSelectMode::InsertCopy" );
  GetUndo()->BeginAction();
  //Произвести вставку
  DSelectorPic tmpTable;
  DBaseIterator<DGraphObjectPic> *copyIterator = GetCopyIterator( tmpTable, offset, next );
  if( !copyIterator ) throw CadError("DSelectMode::InsertCopy: не создан итератор копирования");
  paste->ForEach( *copyIterator );
  delete copyIterator;
  GetUndo()->EndAction( "Копирование" );
  //Теперь все копии вставлены, а их ссылки находятся в tmpTable
  Unselect( false ); //Старые объекты развыделяем
  tmpTable.ForEachBack( SelectAllGraphIterator(fragment) ); //Выделяем все копии
  }

void
DSelectMode::StopCopy( DPoint p, DContext &dc ) {
  if( !paste ) throw CadError( "DSelectMode::StopCopy" );
  DragCopy( p, dc );
  if( p != first ) {
    //Произвести вставку
    InsertCopy( p - first, false );
    }
  paste = 0;
  SetStep( smSelPresent );
  GetProp();
  }

void
DSelectMode::PropChanged( DContext &dc ) {
  GetUndo()->BeginAction();
  //Сохранить состояние объектов до изменения свойств
  fragment.ForEach( DISaveState() );

  if( fragment.GetNumber() ) SetDirty();
  dc.SetXorMode( wccDSLSelected );                   //Режим инвертирования
  ShowSelect( dc );                                  //Стереть
  //Получить новые свойства
  DAutomation( DAC_GETPROP, 0, 0, &localProp );
  fragment.ForEach( SetPropIterator( localProp ) );  //Установить свойства
  ShowSelect( dc );                                  //Нарисовать
  GetUndo()->EndAction( "Изменение свойств" );
  }

//Накопить свойства
void
DSelectMode::GetProp() {
  //Объявить маску выбранных объектов
  int mask = 0;
  //Очистить свойства
  localProp.Clear();
  //Накопить свойства
  fragment.ForEach( GetPropIterator( localProp, mask ) );
  //Выставить панель свойств
  if( mask & dctSymPin )                          mask = dpSymPin;
  else if( mask & dctPrtPin )                     mask = dpPrtPin;
  else if( mask & dctSPName || mask & dctPPName ) mask = dpPinName;
  else if( mask & dctSPNumb || mask & dctPPNumb ) mask = dpPinNumber;
  else if( mask & dctWire )                       mask = dpWire;
  else if( mask & dctText )                       mask = dpText;
  else if( mask & dctLines )                      mask = dpLine;
  else if( mask & dctPrtImp )                     mask = dpPart;
  else if( mask & dctSymImp )                     mask = dpSymbol;
  else if( mask & dctVia )                        mask = 0;
  else if( mask & dctWName )                      mask = dpWireName;
  else if( mask & dctIdent )                      mask = dpText;
  else mask = 0;
  if( mask ) SetPropBar( mask, &localProp );
  else DAutomation( DAC_SETPROP, 0, 0, &localProp );
  //Перейти к другому шагу
  SetStep( fragment.GetNumber() ? smSelPresent : smNoSelect );
  //Обновить изображение
  Update();
  UpdateTools();
  }


int
DSelectMode::CheckPoint( DPoint point ) {
  CheckIterator chk( point );
  GetBase()->ForEach( chk );
  state = chk.GetResult();
  if( DEnvir::envir->bShowMessageRemark ) {
    static DString info;
    DIGetInfo iter( point, info, true );
    if( GetBase()->ForEach( iter ) )
      SetMessage( GetStepHelp() );
    else
      SetMessage( info );
    }
  return state;
  }

void
DSelectMode::DrawUnselected( DContext &dc ) {
  //Рисовать постоянную часть

  //Рисовать все невыделенные
  GetBase()->ForEach( DrawIterator( dc, &fragment, GetEnvir().showRatNet ) );
  }

void
DSelectMode::Draw( DContext &dc ) {
  switch( GetStep() ) {
    case smPaste :
    case smCopy  : DrawCopy( dc ); break;
    default      : DrawDefault( dc ); break;
    }
  }

void
DSelectMode::DrawCopy( DContext &dc ) {
  //Рисовать постоянную часть

  //Рисовать все
  GetBase()->ForEach( DrawIterator( dc, 0, GetEnvir().showRatNet ) );

  dc.SetXorMode( wccDSLSelected );
  //Рисовать вставляемые цветом выделения
  ShowPaste( dc );
  }

void
DSelectMode::ShowPaste( DContext &dc ) {
  //Рисовать вставляемые цветом выделения
  if( !paste ) throw CadError( "DSelectMode::DrawCopy: нет копируемого объекта" );
  paste->ForEach( DrawCopyIterator( dc, prevMove - first ) );
  }

void
DSelectMode::ShowSelect( DContext &dc ) {
  //Рисовать выделенные цветом выделения
  fragment.ForEach( DrawSelectIterator( dc ) );
  }

void
DSelectMode::ShowRect( DContext &dc ) {
  DLineProp prp;
  prp.layer = dslSelected;
  prp.width = 0;
  prp.type  = dltDashed;
  dc.Rect( first, prevMove, prp );
  }

void
DSelectMode::DrawDefault( DContext &dc ) {
  //Рисовать постоянную часть

  //Рисовать все невыделенные
  GetBase()->ForEach( DrawIterator( dc, &fragment, GetEnvir().showRatNet ) );

  //Рисовать выделенные
  dc.SetXorMode( wccDSLSelected );
  ShowSelect( dc );
  //Проверить и рисовать прямоугольник выделения
  if( GetStep() == smSelRect ) ShowRect( dc );
  }




void
DSelectMode::Reset() {
  SetStep(0);
  Update();
  UpdateTools();
  }

void
DSelectMode::Activate() {
  SetPropBar( dpNone, &localProp );
  }

void
DSelectMode::SelectAll() {
  //Выделить все объекты
  GetBase()->ForEach( SelectAllIterator( fragment ) );
  //Обновить изображение
  GetProp();
  }


void
DSelectMode::Copy() {
  if( fragment.GetNumber() ) {
    //Имеются выделенные объекты

    //Вычислить и записать базовую точку фрагмента
    GetGraphOverRect over;
    fragment.ForEach( over );
    //Подогнать к текущей сетке
    DPoint a = ((DRect&)over).LeftBottom();
    DPoint grid = GetViewer()->GetGrid();
    a.x /= grid.x;
    a.x *= grid.x;
    a.y /= grid.y;
    a.y *= grid.y;

    fragment.SetOrigin( a );

    //Осуществить запись в карман
    CopyClipboard();
    }
  }

void
DSelectMode::Paste() {
  SetDirty();
  if( IsSelectPresent() ) Unselect( false );
  if( PasteClipboard() ) {
    // paste и pastePrj содержат указатели на объекты и проект для вставки
    if( !paste || !pastePrj ) throw CadError("DSelectMode::Paste: нет указателя на объект или проект для вставки" );
    first = paste->GetOrigin();
    prevMove = first;
    SetStep( smPaste );
    }
  Update();
  }

void
DSelectMode::CancelPaste() {
  delete paste;
  paste = 0;
  delete pastePrj;
  pastePrj = 0;
  SetStep( smNoSelect );
  }

void
DSelectMode::EnterPaste( DPoint point ) {
  SetDirty();
  GetUndo()->BeginAction();
  //Произвести вставку
  DSelectorPic tmpTable;
  DBaseIterator<DGraphObjectPic> *copyIterator = GetCopyIterator( tmpTable, point - first, false );
  if( !copyIterator ) throw CadError("DSelectMode::EnterPaste: не создан итератор вставки" );
  paste->ForEach( *copyIterator );
  delete copyIterator;
  GetUndo()->EndAction( "Вставка из кармана" );
  //Теперь все копии вставлены, а их ссылки находятся в tmpTable
  Unselect( false ); //Старые объекты развыделяем
  tmpTable.ForEachBack( SelectAllGraphIterator(fragment) ); //Выделяем все копии
  Unselect( true ); //Новые объекты развыделяем
  CancelPaste();
  }



#endif

#ifdef winSelect
/*
Проект "Схемотехника"
Файл MWSelector.cpp
  Содержание
    Реализация функций-членов класса WinSelectMode
                                     WDDublicate
                                     WDParam
                                     ClipStream
                                     WinCopyIterator

  История
    12.09.97 создан
    30.03.2001 модифицирован windows ориентированный режим выбора
    10.06.2004 устранена ошибка по поводу Dirty
*/

#include "WinModes.h"

//==============================================================================
//----------------------- WDDublicate ------------------------------------------
class WDDublicate : public NDialog {
    NNumberControl rowNumber;
    NNumberControl columnNumber;
    NScrollControl rowScroll;
    NScrollControl columnScroll;
    NFloatControl  rowSpace;
    NFloatControl  columnSpace;
  public:
    WDDublicate( int *nrow, int *ncolumn, double *frow, double *fcolumn ) :
      NDialog( DUBL_DLG ),
      rowNumber( IDC_ROWS_NUMBER, this, nrow, 100, 1 ),
      columnNumber( IDC_COLUMNS_NUMBER, this, ncolumn, 100, 1 ),
      rowScroll( IDC_ROWS_UPDN, this, &rowNumber ),
      columnScroll( IDC_COLUMNS_UPDN, this, &columnNumber ),
      rowSpace( IDC_STEPY, this, frow ),
      columnSpace( IDC_STEPX, this, fcolumn ) {}

    virtual CPChar GetHelpStr() const { return "WDDublicate"; }
  };

//==============================================================================
//----------------------- WDDublicate ------------------------------------------
class WDSymParam : public NDialog {
    NEditControl    part;
    NEditControl    param;
    DSelectResultEx res;
    DSymImpPic     *imp;
  public:
    WDSymParam( DSymImpPic *s ) :
      NDialog( PARAM_DLG ),
      part( IDC_PRT, this, 0, 0 ),
      param( IDC_PARAM, this, 0, 0 ),
      imp(s) { res.id1 = dptPrtPic; }

            void   BrowsePart();
    virtual void   WmInit();
    virtual void   CmOk();
    virtual bool   WmCommand( WPARAM, LPARAM );
    virtual CPChar GetHelpStr() const { return "WDSymParam"; }
  };

void
WDSymParam::BrowsePart() {
  //res.Reset();
  if( WDBrowser( &res, &rootHistory ).Execute( this ) && res.object && res.object->GetType() == dptPrtPic )
    part.SetText( res.object->GetName() );
  }

void
WDSymParam::WmInit() {
  NDialog::WmInit();
  //Заполнить поля из символа
  if( imp && imp->GetPrt() )
    part.SetText( imp->GetPrt()->GetName() );
  param.SetText( imp->GetParam() );
  }

void
WDSymParam::CmOk() {
  //Считать из полей

  //Проверить корпус
  NString tmp;
  part.GetText( tmp );
  if( imp && imp->GetPrt() && tmp[0] && tmp != imp->GetPrt()->GetName() && res.object )
    //Провести замену
    imp->ReplacePrt( dynamic_cast<DPrtPic*>(res.object) );

  //Проверить параметры
  param.GetText( tmp );
  imp->GetParam() = tmp;
  EndDialog(1);
  }

bool
WDSymParam::WmCommand( WPARAM wParam, LPARAM lParam ) {
  switch( LOWORD(wParam) ) {
    case IDC_LIST : BrowsePart(); break;
    default : return NDialog::WmCommand( wParam, lParam );
    }
  return true;
  }

//==============================================================================
//----------------------- ClipStream -------------------------------------------
void
ClipStream::WriteObject( DBasePic *object ) {
  DStream::WriteObject( object );
  DGraphObjectPic *graph = dynamic_cast<DGraphObjectPic*>(object);
  if( graph ) WritePtr( graph->GetSelect() );
  }

DBasePic*
ClipStream::ReadObject( DOwnerPic *owner ) {
  DBasePic *object = DStream::ReadObject( owner );
  DGraphObjectPic *graph = dynamic_cast<DGraphObjectPic*>(object);
  if( graph ) {
    DSelectorPic *selector = dynamic_cast<DSelectorPic*>(ReadPtr());
    if( selector ) graph->DoSelect( *selector );
    }
  return object;
  }


//==============================================================================
//----------------------- WinCopyIterator --------------------------------------
DSheetNetPic*
WinCopyIterator::CreateNewNet( DSheetNetPic *net ) {
  //Получить лист, в который входит цепь
  DProjectPic *prj = owner.GetProject();
  if( !prj ) throw CadError("WinCopyIterator::CreateNewNet");
  DName name;
  prj->GetUnusedNetName( name );
  owner.Insert( net = new DSheetNetPic(name) );
  return net;
  }

DSheetNetPic*
WinCopyIterator::OldNames( DOwnerPic *own ) {
  //Всем цепям оставить старые имена
  NetAssotiation assoc;
  assoc.sour = dynamic_cast<DSheetNetPic*>(own);
  if( !assoc.sour ) throw CadError("WinCopyIterator::OldNames");
  //Устранение ошибки при межпроектном копировании
  DSheetPic *sheet = dynamic_cast<DSheetPic*>(&owner);
  Assert( !sheet );
  assoc.dest = sheet->CreateNet( assoc.sour->GetName() );
  vector.Add( assoc );
  return assoc.dest;
  }

DSheetNetPic*
WinCopyIterator::DefNames( DOwnerPic *own ) {
  //Новые имена только цепям по умолчанию
  DSheetNetPic *net = dynamic_cast<DSheetNetPic*>(own);
  if( !net ) throw CadError("WinCopyIterator::DefNames");
  NetAssotiation assoc;
  if( strncmp( net->GetName(), szDefWire, 2 ) ) {
    assoc.sour = net;
    //Устранение ошибки при межпроектном копировании
    DSheetPic *sheet = dynamic_cast<DSheetPic*>(&owner);
    Assert( !sheet );
    assoc.dest = sheet->CreateNet( net->GetName() );
    }
  else {
    assoc.sour = net;
    assoc.dest = CreateNewNet( net );
    }
  vector.Add( assoc );
  return assoc.dest;
  }

DSheetNetPic*
WinCopyIterator::NewNames( DOwnerPic *own ) {
  //Всем новые имена
  DSheetNetPic *net = dynamic_cast<DSheetNetPic*>(own);
  if( !net ) throw CadError("WinCopyIterator::NewNames");
  NetAssotiation assoc;
  assoc.sour = net;
  assoc.dest = CreateNewNet( net );
  vector.Add( assoc );
  return assoc.dest;
  }



bool
WinCopyIterator::operator () ( DGraphObjectPic *object ) {
  DOwnerPic *own = object->GetParent();
  if( own->GetType() == dptSheetNetPic ) {
    //Копируемый объект принадлежит цепи, необходимо применить механизм переименования цепей

    //Ищем соответствующую цепь
    int i;
    for( i = 0; i < vector.GetNumber(); ++i )
      if( vector[i].sour == own ) break;
    DSheetNetPic *dest;
    if( i == vector.GetNumber() ) {
      //Цепь не найдена, добавить
      if( !unionSet ) {
        //Режим объединения еще не определен, определить
        if( !WFrame::pUser->envir.netUnionOn )
          WDNetUnion( WFrame::pUser->envir ).Execute( window );
        //Теперь режим объединения определен
        unionMode = WFrame::pUser->envir.netUnionMode;
        unionSet = true;
        }
      //Варианты добавления цепей
      switch( unionMode ) {
        case 0 : dest = OldNames(own); break;    //Всем оставить старые имена
        case 1 : dest = DefNames(own); break;    //Новые имена присвоить только цепям XN
        case 2 : dest = NewNames(own); break;    //Всем присвоить новые имена
        }
      }
    else dest = vector[i].dest;

    //Цепь подготовлена, добавить сегмент
    if( !dest ) CadError("WinCopyIterator::operator (): нет dest");
    DGraphObjectPic *tmp = dynamic_cast<DGraphObjectPic*>(object->Copy());
    if( !tmp ) throw CadError("WinCopyIterator::operator (): объект не того типа или не скопировался");
    //Выделение объекта
    tmp->Select( table );
    //Перемещение объекта
    tmp->Move( offset );
    //Вставка объекта
    dest->Insert( tmp );
    }
  else {
    switch( object->GetType() ) {
      case dptIdentPic :    //Идентификатор
      case dptSheetNetPic : //Цепь в схеме
      case dptTrasePic :    //Сегмент дорожки
      case dptPlateNetPic : //Цепь в плате
      case dptSymbolPic :   //Символ (схемное изображение компоненты)
      case dptAliasPic :    //Компонент (связывает воедино символ, корпус и параметры)
      case dptPrtPic :      //Корпус (изображение корпуса компоненты)
      case dptSheetPic :    //Лист схемы
      case dptPlatePic :    //Печатная плата
      case dptListPic :     //Перечень элементов
      case dptNetListPic :  //Текстовый перечень цепей
      case dptProjectPic :  //Проект
      case dptPolygonPic :  //Заполненная область
      case dptViaPic :      //Переходное отверстие
      case dptAreaPic :     //Область платы на схеме
      case dptOLEPic :      //Объект OLE
      case dptTextDocPic :  //Текстовые документы
      case dptFieldPic :    //Объект-текстовое поле
      case dptSelectorPic : //Объект-выделитель
        return true; //Копирование не производится
      }
    DGraphObjectPic *tmp = dynamic_cast<DGraphObjectPic*>(next ? object->NextCopy() : object->Copy());
    if( !tmp ) throw CadError("WinCopyIterator::operator (): объект не того типа или не скопировался 2");
    //Выделение объекта
    tmp->Select( table );
    //Перемещение объекта
    tmp->Move( offset );
    //Вставка объекта
    owner.Insert( tmp );
    }
  return true;
  }


//==============================================================================
//----------------------- WinSelectMode ----------------------------------------
DBaseIterator<DGraphObjectPic>*
WinSelectMode::GetCopyIterator( DSelectorPic &tbl, DPoint offset, bool next ) {
  return new WinCopyIterator( tbl, *GetBase(), GetWindow(), offset, GetUndo(), next );
  }

void
WinSelectMode::CopyClipboard() {
  try {
    //Создать файл в памяти
    NWHandleFile file;

    //Определить класс входящих объектов
    int i = fragment.GetObjectClass();

    //Записать класс входящих объектов
    file.Write( &i, sizeof(int) );

    //Образовать поток вывода
    ClipStream os( file );

    //Записать проект
    DProjectPic *prj = GetBase()->GetProject();
    if( !prj ) throw CadError("WinSelectMode::CopyClipboard");
    bool ed = prj->IsEdited();
    prj->Save( VERSION, os );
    if( ed ) prj->SetDirty();

    //Записать фрагмент
    os.WriteObject( &fragment );

    //Записать в буфер обмена
    file.CopyToClipboard( GetWindow()->GetHWND(), WFrame::pFrame->GetCbfPasCAD() );

    //Записываем в формате битовой картинки
    NClientContext cc( *window );
    HDC hdc = CreateCompatibleDC( cc );
    DRect r( fragment.GetOverRect() );    //Охватывающий прямоугольник
    r.a.x -= 10;  //Расширить, чтобы вошли пограничные объекты
    r.a.y -= 10;
    r.b.x += 10;
    r.b.y += 10;
    NPoint s;                             //Размер битовой карты в пикселах
    s.x = r.Width() / window->GetScale() + 10; //Вычисление размера
    s.y = r.High()  / window->GetScale() + 10;
    HBITMAP bmp = CreateCompatibleBitmap( cc.GetHDC(), s.x, s.y );
    if( bmp ) {
      HBITMAP old = SelectObject( hdc, bmp );
      NRect rect;
      rect.right  = s.x;
      rect.bottom = s.y;
      //Закрасить белым
      FillRect( hdc, &rect, GetStockObject(WHITE_BRUSH) );
      NContext dest( hdc );
      WContext wc( &cc, dest, NPoint(100,100), window->GetScale(), r.Center(), s );
      //Рисовать выделенные цветом выделения
      fragment.ForEach( DrawGraphIterator( wc ) );
      dest.ClearContext();
      SelectObject( hdc, old );
      }
    DeleteDC( hdc );
    //Поместить карту в карман
    if( bmp ) NClipboard( *window ).SetData( CF_BITMAP, bmp );
    }
  catch( NCommon::FileError ) {
    //Ошибка ввода/вывода
    GetWindow()->ErrorBox( "Ошибка ввода/вывода" );
    }
  catch( NWinError err ) {
    //Ошибка Windows
    if( err.GetId() ) {
      char buf[1000];
      wsprintf( buf, "%s : %d", err.GetErrMsg(), err.GetId() );
      GetWindow()->ErrorBox( buf );
      }
    else if( err.GetErrMsg() ) GetWindow()->ErrorBox( err.GetErrMsg() );
    else GetWindow()->ErrorBox( "Ошибка в программе Билла Гейтса. Обратитесь к автору (Биллу)!" );
    }
  catch( ... ) {
    //Общая ошибка
    GetWindow()->ErrorBox( "Непонятная ошибка!.." );
    }
  }

bool
WinSelectMode::PasteClipboard() {
  if( IsClipboardFormatAvailable( WFrame::pFrame->GetCbfPasCAD() ) ) {
    //Формат данных PasCAD доступен в буфере обмена

    try {
      //Образуем файл в памяти
      NWHandleFile file;

      //Читаем из буфера обмена
      file.PasteFromClipboard( GetWindow()->GetHWND(), WFrame::pFrame->GetCbfPasCAD() );

      //Образовать поток ввода
      ClipStream is(file);

      //Считать класс записанных объектов и сравнить с воспринимаемым
      int i;
      is.Read( &i, sizeof(i) );
      if( i & ~(GetBase()->GetSubObjectMask()) )
        GetWindow()->ErrorBox( "Данные из буфера обмена не подходят для данного объекта" );
      else {
        //Все хорошо, вставляем

        //Создать и читать проект
        pastePrj = new DProjectPic( "", false );
        LoadProjectFile( GetWindow(), pastePrj, is, "Буфер обмена" );

        //Создать и читать фрагмент
        paste = dynamic_cast<DSelectorPic*>( is.ReadObject(0) );
        return true;
        }
      }
    catch( NCommon::FileError ) {
      //Ошибка ввода/вывода
      GetWindow()->ErrorBox( "Ошибка ввода/вывода" );
      }
    catch( NWinError err ) {
      //Ошибка Windows
      if( err.GetId() ) {
        char buf[1000];
        wsprintf( buf, "%s : %d", err.GetErrMsg(), err.GetId() );
        GetWindow()->ErrorBox( buf );
        }
      else if( err.GetErrMsg() ) GetWindow()->ErrorBox( err.GetErrMsg() );
      else GetWindow()->ErrorBox( "Ошибка в программе Билла Гейтса. Обратитесь к автору (Биллу)!" );
      }
    catch( ... ) {
      //Общая ошибка
      GetWindow()->ErrorBox( "Непонятная ошибка!.." );
      }
    }
  return false;
  }

void
WinSelectMode::ActivateMenu() {
  //Убить таймер
  window->KillTimer(1);
  //Активировать и выполнить команду из меню
  NMenu *pMenu = window->GetSelectMenu();
  NPoint cursor;
  GetCursorPos( &cursor );
  pMenu->GetSubMenu( 0 ).TrackPopupMenu( TPM_LEFTALIGN|TPM_LEFTBUTTON, cursor, *window );
  }

void
WinSelectMode::Dublicate() {
  static int rows = 1, columns = 2;
  static double fx, fy;
  if( fragment.GetNumber() && WDDublicate( &rows, &columns, &fy, &fx ).Execute( window ) ) {
    //Выполнить размножение

    //Поскольку это копирование, то нужно выделить объекты целиком (пользователь мог выделить и частично)
    fragment.ForEach( SelectAllGraphIterator( fragment ) );
    fragment.SetOrigin( fragment.GetOverRect().LeftBottom() );
    if( paste ) throw CadError( "DSelectMode::Dublicate" );
    paste = &fragment;

    DCoord dx = fx * window->GetPPM();
    DCoord dy = 0;
    DPoint px( fragment.GetOrigin() );
    for( int r = 0; r < rows; ++r ) {
      for( int c = r ? 0 : 1; c < columns; ++c ) {
        InsertCopy( DPoint( dx, dy ), true );
        fragment.SetOrigin( fragment.GetOverRect().LeftBottom() );
        dx = fx * window->GetPPM();
        dy = 0;
        }
      //Смещение для следующего ряда
      px.y += fy * window->GetPPM();
      dx = px.x - fragment.GetOrigin().x;
      dy = px.y - fragment.GetOrigin().y;
      }
    paste = 0;
    }
  }

void
WinSelectMode::Param() {
  if( fragment.GetNumber() && fragment[0]->GetType() == dptSymImpPic )
    WDSymParam( dynamic_cast<DSymImpPic*>(fragment[0]) ).Execute( window );
  }

void
WinSelectMode::Numerate() {
  if( fragment.GetNumber() && fragment[0]->GetType() == dptSymImpPic ) {
    DIdent ident;
    int logNumber;
    int logSection;
    DBasePtr<DSymImpPic> sym( fragment[0], "WinSelectMode::Numerate" );
    sym->GetIdentInfo( ident, logNumber, logSection );
    if( WDHandRenum( ident.GetBuffer(), &logNumber, &logSection ).Execute( window ) )
      sym->SetIdentInfo( ident, logNumber, logSection );
    }
  }



#endif


void SdModeSelect::activate()
  {
  }

void SdModeSelect::reset()
  {
  }

void SdModeSelect::drawStatic(SdContext *ctx)
  {
  }

void SdModeSelect::drawDynamic(SdContext *ctx)
  {
  }




int SdModeSelect::getPropBarId() const
  {
  //On base mPropObject return prop bar id
  switch( mPropObject ) {
    default :
    case dctArea          :
    case dctLines         : return PB_LINEAR;
    case dctIdent         :
    case dctWireName      :
    case dctText          : return PB_TEXT;
    case dctSymPin        : return PB_SYM_PIN;
    case dctSymImp        : return PB_SYM_IMP;
    case dctPartPin       : return PB_PART_PIN;
    //case dctRoadPin       :
    case dctPartImp       : return PB_PART_IMP;
    case dctWire          : return PB_WIRE;
    //case dctList          :
    //case dctPoligon       :
    //case dctVia           :
    //case dctSize          :
    //case dctSizeProp      :
    }
  }



void SdModeSelect::propGetFromBar()
  {
  mUndo->begin( QObject::tr("Properties changed") );
  //Сохранить состояние объектов до изменения свойств
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->saveState( mUndo );
    return true;
    });

  if( mFragment.count() ) setDirty();
  //Получить новые свойства
  SdPropBarLinear  *barLinear  = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
  SdPropBarPartPin *barPartPin = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
  SdPropBarSymImp  *barSymImp  = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
  SdPropBarSymPin  *barSymPin  = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
  SdPropBarTextual *barTextual = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
  SdPropBarWire    *barWire    = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));

  mLocalProp.Clear();
  switch( mPropObject ) {
    default :
    case dctArea          :
    case dctLines         :
      barLinear->getPropLine( &(mLocalProp.mLineProp), &(mLocalProp.mLineEnterType) );
      break;
    case dctIdent         :
      barTextual->getPropText( &(mLocalProp.mSymIdentProp) );
      break;
    case dctWireName      :
      barTextual->getPropText( &(mLocalProp.mWireNameProp) );
      break;
    case dctText          :
      barTextual->getPropText( &(mLocalProp.mTextProp) );
      break;
    case dctSymPin        :
      barSymPin->getPropSymPin( &(mLocalProp.mSymPinProp) );
      break;
    case dctSymImp        : return PB_SYM_IMP;
    case dctPartPin       : return PB_PART_PIN;
    //case dctRoadPin       :
    case dctPartImp       : return PB_PART_IMP;
    case dctWire          : return PB_WIRE;
    //case dctList          :
    //case dctPoligon       :
    //case dctVia           :
    //case dctSize          :
    //case dctSizeProp      :
    }
  //Setup new properties
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->setProp( mLocalProp );
    return true;
    });
  }




void SdModeSelect::propSetToBar()
  {
  }




void SdModeSelect::enterPoint( SdPoint point )
  {
  //GetBase()->GetProject()->Dirty();
  switch( getStep() ) {
    case smPaste : enterPaste( point ); break;
    default :
      //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
      if( !(checkPoint(point) & SEL_ELEM) ) {
        if( !mShift ) unselect( false );
        //Пройтись по объектам и выделить точкой
        mObject->forEach( dctAll, [point,this] (SdObject *obj) ->bool {
          SdGraph *graph = dynamic_cast<SdGraph*>(obj);
          if( graph != nullptr )
            graph->selectByPoint( point, &mFragment );
          return true;
          });
        //Обновить свойства и обновить изображение
        propSetToBar();
        }
    }
  //Определить состояние курсора в текущей точке
  checkPoint( point );
  }





void SdModeSelect::clickPoint( SdPoint point )
  {
  if( getStep() != smPaste ) {
    //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
    if( !mShift ) unselect( false );
    //Пройтись по объектам и выделить точкой
    mObject->forEach( dctAll, [point,this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr ) {
        graph->selectByPoint( point, &mFragment );
        }
      SdContainerSheetNet *net = dynamic_cast<SdContainerSheetNet*>(obj);
      if( net != nullptr ) {
        //For nets full net selection
        net->forEach( dctAll, [this] (SdObject *subObj) -> bool {
          SdGraph *subGraph = dynamic_cast<SdGraph*>(subObj);
          if( subGraph != nullptr )
            subGraph->select( &mFragment );
          return true;
          });
        }
      return true;
      });
    //Обновить свойства и обновить изображение
    propSetToBar();
    //Определить состояние курсора в текущей точке
    checkPoint( point );
    }
  }



void SdModeSelect::cancelPoint( SdPoint point )
  {
  switch( getStep() ) {
    case smPaste : cancelPaste(); break;
    default :
      if( checkPoint(point) ) {
        activateMenu();
        return;
        }
      if( isSelectPresent() ) unselect( true );
      else cancelMode();
    }
  setStep( smNoSelect );
  //Определить состояние курсора в текущей точке
  checkPoint( point );
  }




void SdModeSelect::movePoint( SdPoint p )
  {
  mCurPoint = p;
  switch( getStep() ) {
    case smPaste : dragCopy( p ); break;
    default      : checkPoint( p ); //Определить состояние курсора в текущей точке
    }
  }



void SdModeSelect::wheel(SdPoint)
  {
  }

void SdModeSelect::beginDrag( SdPoint point )
  {
  if( checkPoint(point) ) {
    //Над объектом
    if( mControl ) beginCopy( point );
    else beginMove( point );
    }
  else beginRect( point );
  }



void SdModeSelect::dragPoint( SdPoint point )
  {
  switch( getStep() ) {
    case smCopy    : dragCopy( point ); break;
    case smMove    : dragMove( point ); break;
    case smSelRect : dragRect( point ); break;
    }
  }



void SdModeSelect::stopDrag(SdPoint point)
  {
  dragPoint( point );
  switch( getStep() ) {
    case smCopy    : stopCopy( point ); break;
    case smMove    : stopMove( point ); break;
    case smSelRect : stopRect( point ); break;
    }
  }



bool SdModeSelect::enableCopy() const
  {
  }



bool SdModeSelect::enablePaste(quint64 pasteMask) const
  {
  return (mObject->getAcceptedObjectsMask() & pasteMask) != 0l;
  }



bool SdModeSelect::getInfo(SdPoint p, QString &info)
  {
  }




QString SdModeSelect::getStepHelp() const
  {
  switch( getStep() ) {
    default :
    case smNoSelect   : return QObject::tr( "No selected elements" );
    //Происходит выделение прямоугольником
    case smSelRect    : return QObject::tr( "Enter second corner for selection objects group" );
    //Есть выделение
    case smSelPresent : return QObject::tr( "There selected elements. You may them move, copy and edit");
    //Копирование
    case smCopy       : return QObject::tr( "Enter position for new elements" );
    //Перенос
    case smMove       : return QObject::tr( "Enter new position for selected elements" );
    //Указание базы для поворота
    case smRotateBase : return QObject::tr( "Enter center point for elements group rotation");
    //Указание направления поворота
    case smRotateDir  : return QObject::tr( "Enter point to define angle of rotation" );
    //Вставка из карамана
    case smPaste      : return QObject::tr( "Enter position for pasted elements" );
    }
  }




QString SdModeSelect::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeSelect.htm" );
  }




QString SdModeSelect::getStepThema() const
  {
  switch( getStep() ) {
    default :
    case smNoSelect   : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectNone" );
    //Происходит выделение прямоугольником
    case smSelRect    : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectSecondCorner" );
    //Есть выделение
    case smSelPresent : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPresent" );
    //Копирование
    case smCopy       : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectInsertPoint" );
    //Перенос
    case smMove       : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPlacePoint" );
    //Указание базы для поворота
    case smRotateBase : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectRotateCenter" );
    //Указание направления поворота
    case smRotateDir  : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectRotateAngle" );
    //Вставка из карамана
    case smPaste      : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPaste" );
    }
  }



int SdModeSelect::getCursor() const
  {
  switch( getStep() ) {
    case smNoSelect :
    case smSelPresent :
      if( mState & SEL_ELEM ) return mControl ? CUR_TAKE_COPY : CUR_TAKE;
      else if( mState & UNSEL_ELEM ) return CUR_HAND;
      return CUR_POINT;
    case smSelRect    : return CUR_SEL;
    case smCopy       : return CUR_COPY;
    case smMove       : return CUR_MOVE;
    case smRotateBase : return CUR_ROTATE;
    case smRotateDir  : return CUR_ROTATE;
    case smPaste      : return CUR_ARROW;
    }
  return CUR_ARROW;
  }




int SdModeSelect::getIndex() const
  {
  return MD_SELECT;
  }
