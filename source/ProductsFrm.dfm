object ProductsForm: TProductsForm
  Left = 194
  Top = 107
  Caption = 'Products'
  ClientHeight = 513
  ClientWidth = 775
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanTop: TPanel
    Left = 0
    Top = 0
    Width = 775
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 12
      Width = 23
      Height = 13
      Caption = 'Item:'
    end
    object labTools: TLabel
      Left = 192
      Top = 12
      Width = 40
      Height = 13
      Caption = 'labTools'
    end
    object Label2: TLabel
      Left = 280
      Top = 12
      Width = 26
      Height = 13
      Caption = 'Level'
    end
    object cbItems: TComboBox
      Left = 40
      Top = 8
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 0
      OnChange = cbItemsChange
    end
    object cbLevel: TComboBox
      Left = 328
      Top = 8
      Width = 57
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 1
      OnChange = cbLevelChange
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5')
    end
    object bnToRegionList: TButton
      Left = 416
      Top = 8
      Width = 97
      Height = 25
      Caption = 'To Region List'
      TabOrder = 2
      OnClick = bnToRegionListClick
    end
  end
  object Grid: TStringGrid
    Left = 0
    Top = 41
    Width = 775
    Height = 472
    Align = alClient
    ColCount = 6
    FixedCols = 0
    FixedRows = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
    TabOrder = 0
    OnDblClick = GridDblClick
    ColWidths = (
      145
      64
      64
      64
      64
      64)
  end
end
