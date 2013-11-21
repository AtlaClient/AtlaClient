object SupportBuildingForm: TSupportBuildingForm
  Left = 353
  Top = 192
  BorderStyle = bsDialog
  Caption = 'Support Building Analyzer'
  ClientHeight = 284
  ClientWidth = 410
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 96
    Top = 259
    Width = 42
    Height = 13
    Caption = 'Products'
  end
  object Label2: TLabel
    Left = 0
    Top = 259
    Width = 42
    Height = 13
    Caption = 'Buildings'
  end
  object sgCalc: TStringGrid
    Left = 0
    Top = 0
    Width = 201
    Height = 249
    ColCount = 2
    DefaultColWidth = 90
    FixedCols = 0
    RowCount = 10
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    TabOrder = 0
  end
  object edProds: TEdit
    Left = 144
    Top = 256
    Width = 41
    Height = 21
    TabOrder = 1
    OnChange = bnCalcClick
  end
  object edBuilds: TEdit
    Left = 48
    Top = 256
    Width = 41
    Height = 21
    TabOrder = 2
    OnChange = bnCalcClick
  end
  object sgReg: TStringGrid
    Left = 208
    Top = 0
    Width = 201
    Height = 249
    Hint = 'DblClict to calculate'
    ColCount = 2
    DefaultColWidth = 90
    FixedCols = 0
    RowCount = 10
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goThumbTracking]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnDblClick = sgRegDblClick
  end
end
