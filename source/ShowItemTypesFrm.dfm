object ShowItemTypesForm: TShowItemTypesForm
  Left = 191
  Top = 107
  Caption = 'Item Types'
  ClientHeight = 462
  ClientWidth = 427
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 422
    Width = 427
    Height = 40
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 134
    ExplicitWidth = 262
    object bnEdit: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Edit'
      Default = True
      TabOrder = 0
      OnClick = bnEditClick
    end
    object bnCancel: TButton
      Left = 96
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 1
      OnClick = bnCancelClick
    end
    object bnRequery: TButton
      Left = 184
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Query info'
      TabOrder = 2
      OnClick = bnRequeryClick
    end
  end
  object List: TListBox
    Left = 0
    Top = 0
    Width = 427
    Height = 422
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
    OnDblClick = bnEditClick
    ExplicitWidth = 262
    ExplicitHeight = 134
  end
end
