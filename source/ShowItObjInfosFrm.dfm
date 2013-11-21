object ShowItObjInfosForm: TShowItObjInfosForm
  Left = 307
  Top = 129
  Caption = 'Item&Object Reports'
  ClientHeight = 289
  ClientWidth = 307
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 133
    Width = 307
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object Panel1: TPanel
    Left = 0
    Top = 249
    Width = 307
    Height = 40
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object bnCancel: TButton
      Left = 72
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 0
      OnClick = bnCancelClick
    end
    object bnRequery: TButton
      Left = 160
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Requery info'
      TabOrder = 1
      OnClick = bnRequeryClick
    end
    object bnRequeryAll: TButton
      Left = 240
      Top = 8
      Width = 67
      Height = 25
      Caption = 'Requery all'
      TabOrder = 2
      OnClick = bnRequeryAllClick
    end
    object cbItems: TCheckBox
      Left = 8
      Top = 12
      Width = 57
      Height = 17
      Caption = 'Items'
      TabOrder = 3
      OnClick = cbItemsClick
    end
  end
  object List: TListBox
    Left = 0
    Top = 0
    Width = 307
    Height = 133
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
    OnClick = ListClick
  end
  object Memo: TMemo
    Left = 0
    Top = 136
    Width = 307
    Height = 113
    Align = alBottom
    ScrollBars = ssVertical
    TabOrder = 2
  end
end
