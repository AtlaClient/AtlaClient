object EditOrderPromoteForm: TEditOrderPromoteForm
  Left = 252
  Top = 172
  BorderStyle = bsSizeToolWin
  Caption = 'Edit PROMOTE Order'
  ClientHeight = 116
  ClientWidth = 350
  Color = clBtnFace
  Constraints.MaxHeight = 140
  Constraints.MinHeight = 140
  Constraints.MinWidth = 221
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    350
    116)
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 0
    Top = 7
    Width = 65
    Height = 17
    AutoSize = False
    Caption = 'Promote'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 0
    Top = 36
    Width = 35
    Height = 13
    Caption = 'Faction'
  end
  object labError: TLabel
    Left = 216
    Top = 63
    Width = 36
    Height = 13
    Caption = 'labError'
  end
  object bnOk: TButton
    Left = 24
    Top = 84
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 128
    Top = 84
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
    Top = 62
    Width = 79
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 3
  end
  object cbRepeating: TCheckBox
    Left = 25
    Top = 62
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 2
  end
  object cbTargets: TComboBox
    Left = 64
    Top = 8
    Width = 284
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    ItemHeight = 0
    TabOrder = 0
    OnChange = cbTargetsChange
  end
  object cbFacDiap: TComboBox
    Left = 64
    Top = 32
    Width = 284
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 0
    TabOrder = 1
    OnChange = cbFacDiapChange
    OnEnter = cbFacDiapEnter
    OnKeyPress = cbFacDiapKeyPress
  end
end
