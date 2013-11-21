object ViewGatesForm: TViewGatesForm
  Left = 205
  Top = 107
  Caption = 'View Gates'
  ClientHeight = 491
  ClientWidth = 401
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TStringGrid
    Left = 0
    Top = 0
    Width = 401
    Height = 491
    Hint = 'DblClick: on line to goto'
    Align = alClient
    ColCount = 2
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDblClick = GridDblClick
    ColWidths = (
      64
      313)
  end
end
