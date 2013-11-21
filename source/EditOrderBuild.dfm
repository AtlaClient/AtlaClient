object EditOrderBuildForm: TEditOrderBuildForm
  Left = 295
  Top = 197
  BorderStyle = bsToolWindow
  Caption = 'Edit BUILD'
  ClientHeight = 154
  ClientWidth = 301
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  DesignSize = (
    301
    154)
  PixelsPerInch = 96
  TextHeight = 13
  object labProd: TLabel
    Left = 8
    Top = 32
    Width = 36
    Height = 13
    Caption = 'labProd'
  end
  object labCost: TLabel
    Left = 8
    Top = 48
    Width = 35
    Height = 13
    Caption = 'labCost'
  end
  object labHave: TLabel
    Left = 8
    Top = 64
    Width = 40
    Height = 13
    Caption = 'labHave'
  end
  object labCap: TLabel
    Left = 176
    Top = 48
    Width = 33
    Height = 13
    Caption = 'labCap'
  end
  object labSail: TLabel
    Left = 176
    Top = 64
    Width = 31
    Height = 13
    Caption = 'labSail'
  end
  object labProt: TLabel
    Left = 176
    Top = 32
    Width = 33
    Height = 13
    Caption = 'labProt'
  end
  object bnOk: TButton
    Left = 56
    Top = 121
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 136
    Top = 121
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object cbRepeating: TCheckBox
    Left = 56
    Top = 97
    Width = 73
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 1
  end
  object cbCommented: TCheckBox
    Left = 136
    Top = 97
    Width = 87
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 2
  end
  object cbBuilding: TComboBox
    Left = 8
    Top = 8
    Width = 289
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
    OnChange = cbBuildingChange
  end
end
