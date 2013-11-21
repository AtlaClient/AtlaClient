object EditOrderStealForm: TEditOrderStealForm
  Left = 252
  Top = 172
  BorderStyle = bsSizeToolWin
  Caption = 'Edit STEAL Order'
  ClientHeight = 138
  ClientWidth = 350
  Color = clBtnFace
  Constraints.MaxHeight = 162
  Constraints.MinHeight = 162
  Constraints.MinWidth = 221
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    350
    138)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 7
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Steal'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 8
    Top = 31
    Width = 25
    Height = 17
    AutoSize = False
    Caption = 'from'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 0
    Top = 60
    Width = 35
    Height = 13
    Caption = 'Faction'
  end
  object lbAll: TLabel
    Left = 312
    Top = 104
    Width = 3
    Height = 13
    Caption = ' '
  end
  object bnOk: TButton
    Left = 24
    Top = 106
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 5
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 106
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 6
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 129
    Top = 84
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 4
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 84
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 3
  end
  object cbTargets: TComboBox
    Left = 40
    Top = 32
    Width = 308
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    ItemHeight = 13
    TabOrder = 1
    OnChange = cbTargetsChange
  end
  object cbItems: TComboBox
    Left = 40
    Top = 8
    Width = 308
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = cbItemsChange
  end
  object cbFacDiap: TComboBox
    Left = 40
    Top = 56
    Width = 308
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 2
    OnChange = cbFacDiapChange
    OnEnter = cbFacDiapEnter
    OnKeyPress = cbFacDiapKeyPress
  end
end
