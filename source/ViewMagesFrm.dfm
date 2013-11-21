object ViewMagesForm: TViewMagesForm
  Left = 195
  Top = 81
  Caption = 'View Mages'
  ClientHeight = 431
  ClientWidth = 717
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  ShowHint = True
  WindowState = wsMaximized
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TDrawGrid
    Left = 0
    Top = 0
    Width = 717
    Height = 431
    Hint = 
      'DblClick: on line header to sort, on order to edit, on unit&regi' +
      'on name to goto, on skill level to study. Right - menu'
    Align = alClient
    ColCount = 4
    DefaultColWidth = 150
    DefaultRowHeight = 20
    RowCount = 20
    FixedRows = 4
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    PopupMenu = PM
    TabOrder = 0
    OnDblClick = GridDblClick
    OnDrawCell = GridDrawCell
    OnMouseDown = GridMouseDown
  end
  object PM: TPopupMenu
    OnPopup = miEventsClick
    Left = 160
    Top = 224
    object miSetColWidth: TMenuItem
      Caption = 'Set Columns &Width'
      OnClick = miSetColWidthClick
    end
    object miLocalOnly: TMenuItem
      Caption = '&Local Only'
      OnClick = miLocalOnlyClick
    end
    object miCurRegOnly: TMenuItem
      Caption = '&Current Region Only'
      OnClick = miCurRegOnlyClick
    end
    object miEvents: TMenuItem
      Caption = 'Events'
      object TMenuItem
      end
    end
    object miRemoveMonthOrder: TMenuItem
      Caption = '&Remove Monthlong Order'
      OnClick = miRemoveMonthOrderClick
    end
  end
  object MainMenu1: TMainMenu
    Left = 224
    Top = 224
    object miPrevTurn: TMenuItem
      Caption = '&Prev Turn'
      OnClick = miPrevTurnClick
    end
    object miNextTurn: TMenuItem
      Caption = '&Next Turn'
      OnClick = miNextTurnClick
    end
  end
end
