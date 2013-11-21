object EditOrderGiveForm: TEditOrderGiveForm
  Left = 252
  Top = 172
  BorderStyle = bsSizeToolWin
  Caption = 'Edit GIVE Order'
  ClientHeight = 154
  ClientWidth = 392
  Color = clBtnFace
  Constraints.MaxHeight = 178
  Constraints.MinHeight = 178
  Constraints.MinWidth = 221
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    392
    154)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 7
    Width = 22
    Height = 17
    AutoSize = False
    Caption = 'Give'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 16
    Top = 31
    Width = 9
    Height = 17
    AutoSize = False
    Caption = 'to'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 0
    Top = 60
    Width = 35
    Height = 13
    Caption = 'Faction'
  end
  object labCapacity: TLabel
    Left = 240
    Top = 115
    Width = 55
    Height = 13
    Caption = 'labCapacity'
    Visible = False
  end
  object labCapacity2: TLabel
    Left = 240
    Top = 134
    Width = 55
    Height = 13
    Caption = 'labCapacity'
    Visible = False
  end
  object bnOk: TButton
    Left = 24
    Top = 122
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 8
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 122
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 9
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 129
    Top = 100
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 5
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 100
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 4
  end
  object cbTargets: TComboBox
    Left = 40
    Top = 32
    Width = 350
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    ItemHeight = 13
    TabOrder = 2
    OnChange = cbTargetsChange
  end
  object cbItems: TComboBox
    Left = 120
    Top = 8
    Width = 270
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = cbItemsChange
  end
  object edCount: TEdit
    Left = 40
    Top = 8
    Width = 53
    Height = 21
    TabOrder = 1
    OnExit = edCountExit
  end
  object bnAll: TButton
    Left = 96
    Top = 8
    Width = 22
    Height = 21
    Caption = '&All'
    TabOrder = 6
    TabStop = False
    OnClick = bnAllClick
  end
  object cbDiscard: TCheckBox
    Left = 24
    Top = 80
    Width = 81
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Discard item'
    TabOrder = 7
  end
  object cbAll: TCheckBox
    Left = 128
    Top = 80
    Width = 41
    Height = 17
    Caption = 'A&ll'
    TabOrder = 10
    OnClick = cbAllClick
  end
  object cbExcept: TCheckBox
    Left = 224
    Top = 80
    Width = 57
    Height = 17
    Caption = '&Except'
    TabOrder = 11
    OnClick = cbExceptClick
  end
  object cbFacDiap: TComboBox
    Left = 40
    Top = 56
    Width = 350
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 3
    OnChange = cbFacDiapChange
    OnEnter = cbFacDiapEnter
    OnKeyPress = cbFacDiapKeyPress
  end
end
