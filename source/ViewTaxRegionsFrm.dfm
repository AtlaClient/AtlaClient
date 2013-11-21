object ViewTaxRegionsForm: TViewTaxRegionsForm
  Left = 195
  Top = 81
  Caption = 'Tax Regions'
  ClientHeight = 514
  ClientWidth = 777
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TDrawGrid
    Left = 0
    Top = 0
    Width = 777
    Height = 514
    Hint = 'DblClick: on region name to goto, on cell to change taxing'
    Align = alClient
    ColCount = 3
    DefaultColWidth = 100
    FixedCols = 2
    RowCount = 3
    FixedRows = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDblClick = GridDblClick
    OnDrawCell = GridDrawCell
  end
end
