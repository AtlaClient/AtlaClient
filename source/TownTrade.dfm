object fTownTrade: TfTownTrade
  Left = 205
  Top = 103
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Town Trade'
  ClientHeight = 431
  ClientWidth = 278
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 278
    Height = 34
    Align = alTop
    TabOrder = 0
    object Label1: TLabel
      Left = 34
      Top = 11
      Width = 26
      Height = 13
      Caption = 'Item :'
    end
    object cbItems: TComboBox
      Left = 74
      Top = 7
      Width = 169
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = cbItemsChange
    end
  end
  object gb2: TGroupBox
    Left = 0
    Top = 241
    Width = 278
    Height = 190
    Align = alClient
    Caption = ' Wanted '
    TabOrder = 2
    object grWanted: TStringGrid
      Left = 2
      Top = 15
      Width = 274
      Height = 173
      Align = alClient
      ColCount = 3
      DefaultRowHeight = 18
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      TabOrder = 0
      OnClick = grWantedClick
      OnDblClick = grWantedDblClick
      OnSelectCell = grSaleSelectCell
      ColWidths = (
        131
        56
        64)
    end
  end
  object gb1: TGroupBox
    Left = 0
    Top = 34
    Width = 278
    Height = 168
    Align = alTop
    Caption = ' For Sale '
    TabOrder = 1
    object grSale: TStringGrid
      Left = 2
      Top = 15
      Width = 274
      Height = 151
      Align = alClient
      ColCount = 3
      DefaultRowHeight = 18
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      TabOrder = 0
      OnClick = grWantedClick
      OnDblClick = grSaleDblClick
      OnSelectCell = grSaleSelectCell
      ColWidths = (
        131
        56
        64)
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 202
    Width = 278
    Height = 39
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object lb1: TLabel
      Left = 3
      Top = 4
      Width = 272
      Height = 13
      Alignment = taCenter
      AutoSize = False
    end
    object lb2: TLabel
      Left = 3
      Top = 20
      Width = 272
      Height = 13
      Alignment = taCenter
      AutoSize = False
    end
  end
end
