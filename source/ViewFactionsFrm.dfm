object ViewFactionsForm: TViewFactionsForm
  Left = 83
  Top = 46
  Caption = 'Factions'
  ClientHeight = 377
  ClientWidth = 724
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  ShowHint = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 298
    Width = 724
    Height = 79
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object bnSetCurrent: TButton
      Left = 24
      Top = 32
      Width = 75
      Height = 25
      Caption = 'Set &Current'
      TabOrder = 0
      OnClick = bnSetCurrentClick
    end
    object bnSendMail: TButton
      Left = 120
      Top = 32
      Width = 75
      Height = 25
      Caption = 'Send &Mail'
      TabOrder = 1
      OnClick = bnSendMailClick
    end
    object cbFacDeleted: TCheckBox
      Left = 208
      Top = 32
      Width = 57
      Height = 25
      Caption = 'Deleted'
      TabOrder = 2
      OnClick = cbFacDeletedClick
    end
    object bnSendOrders: TButton
      Left = 280
      Top = 32
      Width = 75
      Height = 25
      Caption = 'Send &Orders'
      TabOrder = 3
      OnClick = bnSendOrdersClick
    end
    object bnFindAddress: TButton
      Left = 376
      Top = 32
      Width = 89
      Height = 25
      Caption = 'Address request'
      TabOrder = 4
      OnClick = bnFindAddressClick
    end
    object btAttDef: TButton
      Left = 488
      Top = 32
      Width = 153
      Height = 25
      Caption = 'Default attitude : '
      TabOrder = 5
      OnClick = btAttDefClick
    end
  end
  object Grid: TDrawGrid
    Left = 0
    Top = 0
    Width = 724
    Height = 298
    Hint = 'DblClick on header to sort. DblClick to cell to change.'
    Align = alClient
    ColCount = 6
    FixedCols = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
    TabOrder = 1
    OnDblClick = GridDblClick
    OnDrawCell = GridDrawCell
    OnSelectCell = GridSelectCell
    ColWidths = (
      176
      40
      123
      83
      79
      196)
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object ColorDialog: TColorDialog
    Options = [cdFullOpen, cdSolidColor]
    Left = 88
    Top = 192
  end
end
