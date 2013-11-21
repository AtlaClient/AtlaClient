object AnalyzeCityRegionFrm: TAnalyzeCityRegionFrm
  Left = 194
  Top = 107
  BorderStyle = bsDialog
  Caption = 'Analyze City Region'
  ClientHeight = 513
  ClientWidth = 613
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object labLocation: TLabel
    Left = 8
    Top = 8
    Width = 55
    Height = 13
    Caption = 'labLocation'
  end
  object labMarketItem: TLabel
    Left = 424
    Top = 248
    Width = 67
    Height = 13
    Caption = 'labMarketItem'
  end
  object labError: TLabel
    Left = 568
    Top = 336
    Width = 36
    Height = 13
    Caption = 'labError'
  end
  object Label1: TLabel
    Left = 424
    Top = 80
    Width = 78
    Height = 13
    Caption = 'default BasePop'
  end
  object Label2: TLabel
    Left = 424
    Top = 280
    Width = 43
    Height = 13
    Caption = 'BasePop'
  end
  object Label3: TLabel
    Left = 424
    Top = 312
    Width = 59
    Height = 13
    Caption = 'Max Amount'
  end
  object Label4: TLabel
    Left = 536
    Top = 336
    Width = 30
    Height = 13
    Caption = 'Errors:'
  end
  object bnLoad: TButton
    Left = 424
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Load'
    TabOrder = 7
    OnClick = bnLoadClick
  end
  object cbSaveOnClose: TCheckBox
    Left = 512
    Top = 40
    Width = 97
    Height = 17
    Caption = 'Save on Close'
    Checked = True
    State = cbChecked
    TabOrder = 9
  end
  object bnSave: TButton
    Left = 512
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 8
    OnClick = bnSaveClick
  end
  object bnAddTurn: TButton
    Left = 424
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Add this Turn'
    TabOrder = 0
    OnClick = bnAddTurnClick
  end
  object edDefBasePop: TEdit
    Left = 512
    Top = 72
    Width = 97
    Height = 21
    TabOrder = 1
    OnExit = edDefBasePopExit
  end
  object sgMarkets: TStringGrid
    Left = 8
    Top = 32
    Width = 409
    Height = 329
    Hint = 'DlbClick to select market'
    FixedCols = 0
    RowCount = 15
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    OnDblClick = sgMarketsDblClick
    ColWidths = (
      116
      96
      65
      64
      43)
  end
  object sgMarket: TStringGrid
    Left = 8
    Top = 376
    Width = 601
    Height = 121
    ColCount = 20
    RowCount = 4
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
    TabOrder = 6
    ColWidths = (
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64)
  end
  object edBasePop: TEdit
    Left = 488
    Top = 272
    Width = 121
    Height = 21
    TabOrder = 3
    OnExit = edBasePopExit
    OnKeyPress = edBasePopKeyPress
  end
  object edMaxAmt: TEdit
    Left = 488
    Top = 304
    Width = 121
    Height = 21
    TabOrder = 4
    OnExit = edMaxAmtExit
    OnKeyPress = edMaxAmtKeyPress
  end
  object cbConfirmed: TCheckBox
    Left = 424
    Top = 336
    Width = 97
    Height = 17
    Caption = 'Confirmed'
    TabOrder = 5
    OnClick = cbConfirmedClick
  end
end
