object ShowObjTypesForm: TShowObjTypesForm
  Left = 205
  Top = 107
  Caption = 'ShowObjTypesForm'
  ClientHeight = 173
  ClientWidth = 382
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 133
    Width = 382
    Height = 40
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object bnEdit: TButton
      Left = 72
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Edit'
      Default = True
      TabOrder = 0
      OnClick = bnEditClick
    end
    object bnCancel: TButton
      Left = 153
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
      Left = 241
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
    Width = 382
    Height = 133
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
  end
end
