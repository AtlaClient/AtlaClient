object EditOrderAttackForm: TEditOrderAttackForm
  Left = 252
  Top = 172
  BorderStyle = bsSizeToolWin
  Caption = 'Edit ATTACK Order'
  ClientHeight = 280
  ClientWidth = 322
  Color = clBtnFace
  Constraints.MinHeight = 200
  Constraints.MinWidth = 221
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    322
    280)
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 0
    Top = 12
    Width = 31
    Height = 13
    Caption = 'Attack'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 0
    Top = 36
    Width = 35
    Height = 13
    Caption = 'Faction'
  end
  object bnOk: TButton
    Left = 24
    Top = 248
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&OK'
    ModalResult = 1
    TabOrder = 4
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 248
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 129
    Top = 226
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 3
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 226
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 2
  end
  object cbCanTargets: TComboBox
    Left = 40
    Top = 8
    Width = 280
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    ItemHeight = 0
    TabOrder = 0
    OnChange = cbCanTargetsChange
    OnDblClick = bnAddClick
  end
  object cbFacDiap: TComboBox
    Left = 40
    Top = 32
    Width = 280
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    TabOrder = 1
    OnChange = cbFacDiapChange
    OnEnter = cbFacDiapEnter
    OnKeyPress = cbFacDiapKeyPress
  end
  object lbTargets: TListBox
    Left = 8
    Top = 64
    Width = 309
    Height = 121
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    TabOrder = 6
    OnDblClick = bnDeleteClick
  end
  object bnAdd: TButton
    Left = 24
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&Add'
    Default = True
    TabOrder = 7
    OnClick = bnAddClick
  end
  object bnDelete: TButton
    Left = 232
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&Delete'
    TabOrder = 8
    OnClick = bnDeleteClick
  end
  object bnAddAll: TButton
    Left = 120
    Top = 192
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Add A&ll'
    TabOrder = 9
    OnClick = bnAddAllClick
  end
end
