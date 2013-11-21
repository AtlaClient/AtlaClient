object EditSkillTypeForm: TEditSkillTypeForm
  Left = 203
  Top = 161
  Caption = 'Skill Editor'
  ClientHeight = 238
  ClientWidth = 402
  Color = clBtnFace
  Constraints.MaxHeight = 265
  Constraints.MinHeight = 265
  Constraints.MinWidth = 410
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnShow = FormShow
  DesignSize = (
    402
    238)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 112
    Width = 31
    Height = 17
    AutoSize = False
    Caption = 'Name:'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 0
    Top = 136
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Abr:'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 80
    Top = 136
    Width = 25
    Height = 17
    AutoSize = False
    Caption = 'Cost:'
    Layout = tlBottom
  end
  object Label5: TLabel
    Left = 160
    Top = 112
    Width = 58
    Height = 13
    Alignment = taCenter
    Caption = 'Depend skill'
  end
  object Label6: TLabel
    Left = 336
    Top = 112
    Width = 22
    Height = 13
    Alignment = taCenter
    Caption = 'level'
  end
  object Table: TStringGrid
    Left = 0
    Top = 8
    Width = 401
    Height = 97
    Anchors = [akLeft, akTop, akRight]
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    TabOrder = 0
    OnDblClick = TableDblClick
  end
  object edName: TEdit
    Left = 32
    Top = 112
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object edAbr: TEdit
    Left = 32
    Top = 136
    Width = 41
    Height = 21
    TabOrder = 2
  end
  object edCost: TEdit
    Left = 104
    Top = 136
    Width = 49
    Height = 21
    TabOrder = 3
  end
  object cbMagic: TCheckBox
    Left = 8
    Top = 160
    Width = 57
    Height = 17
    Caption = 'Magic'
    TabOrder = 4
  end
  object cbCombat: TCheckBox
    Left = 80
    Top = 160
    Width = 57
    Height = 17
    Caption = 'Combat'
    TabOrder = 5
  end
  object cbCast: TCheckBox
    Left = 8
    Top = 184
    Width = 57
    Height = 17
    Caption = 'Cast'
    TabOrder = 6
  end
  object cbFoundation: TCheckBox
    Left = 80
    Top = 184
    Width = 73
    Height = 17
    Caption = 'Foundation'
    TabOrder = 7
  end
  object cbDep1: TComboBox
    Left = 160
    Top = 128
    Width = 169
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 8
    OnChange = cbDep1Change
  end
  object cbDep2: TComboBox
    Left = 160
    Top = 152
    Width = 169
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 10
    Visible = False
    OnChange = cbDep2Change
  end
  object cbDep3: TComboBox
    Left = 160
    Top = 176
    Width = 169
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 12
    Visible = False
    OnChange = cbDep3Change
  end
  object bnOk: TButton
    Left = 65
    Top = 208
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 14
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 265
    Top = 208
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 15
  end
  object cbLev1: TComboBox
    Left = 336
    Top = 128
    Width = 57
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 9
    Visible = False
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5')
  end
  object cbLev2: TComboBox
    Left = 336
    Top = 152
    Width = 57
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 11
    Visible = False
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5')
  end
  object cbLev3: TComboBox
    Left = 336
    Top = 176
    Width = 57
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 13
    Visible = False
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5')
  end
end
