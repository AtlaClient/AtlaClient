object EditObjTypeForm: TEditObjTypeForm
  Left = 205
  Top = 107
  ActiveControl = edName
  Caption = 'Object Editor'
  ClientHeight = 365
  ClientWidth = 505
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    505
    365)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 31
    Height = 17
    AutoSize = False
    Caption = 'Name'
    Layout = tlBottom
  end
  object Label5: TLabel
    Left = 215
    Top = 8
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Skill'
    Layout = tlBottom
  end
  object labInput: TLabel
    Left = 215
    Top = 35
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Input'
    Layout = tlBottom
  end
  object labNumber: TLabel
    Left = 215
    Top = 64
    Width = 41
    Height = 17
    AutoSize = False
    Caption = 'Number'
    Layout = tlBottom
  end
  object labHelpProd: TLabel
    Left = 215
    Top = 95
    Width = 97
    Height = 17
    AutoSize = False
    Caption = 'Production bonus +'
    Layout = tlBottom
  end
  object labHelpFrom: TLabel
    Left = 347
    Top = 95
    Width = 29
    Height = 17
    AutoSize = False
    Caption = 'from'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 8
    Top = 37
    Width = 34
    Height = 13
    Caption = 'Protect'
  end
  object Label3: TLabel
    Left = 8
    Top = 66
    Width = 41
    Height = 13
    Caption = 'Capacity'
  end
  object lbSailors: TLabel
    Left = 8
    Top = 153
    Width = 31
    Height = 13
    Caption = 'Sailors'
    Visible = False
  end
  object Label6: TLabel
    Left = 215
    Top = 153
    Width = 26
    Height = 13
    Caption = 'Road'
  end
  object labLevel: TLabel
    Left = 372
    Top = 8
    Width = 33
    Height = 17
    AutoSize = False
    Caption = 'Level'
    Layout = tlBottom
  end
  object Label7: TLabel
    Left = 215
    Top = 126
    Width = 99
    Height = 13
    Caption = 'Increases production'
  end
  object Memo: TMemo
    Left = 10
    Top = 176
    Width = 484
    Height = 145
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 14
  end
  object edName: TEdit
    Left = 63
    Top = 6
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object cbSkill: TComboBox
    Left = 247
    Top = 6
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 6
    OnChange = cbSkillChange
  end
  object cbLevel: TComboBox
    Left = 407
    Top = 6
    Width = 57
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 7
    Visible = False
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5')
  end
  object cbInput: TComboBox
    Left = 247
    Top = 33
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 8
    OnChange = cbInputChange
  end
  object edNumber: TEdit
    Left = 263
    Top = 62
    Width = 49
    Height = 21
    TabOrder = 9
  end
  object edHelpMult: TEdit
    Left = 311
    Top = 93
    Width = 33
    Height = 21
    TabOrder = 10
  end
  object cbHelpProd: TComboBox
    Left = 375
    Top = 93
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 11
  end
  object cbEnter: TCheckBox
    Left = 8
    Top = 95
    Width = 67
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Can enter'
    TabOrder = 3
  end
  object cbShip: TCheckBox
    Left = 8
    Top = 124
    Width = 69
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Ship'
    TabOrder = 4
    OnClick = cbShipClick
  end
  object edProtect: TMaskEdit
    Left = 63
    Top = 33
    Width = 59
    Height = 21
    EditMask = '0999;0; '
    MaxLength = 4
    TabOrder = 1
  end
  object edCapacity: TMaskEdit
    Left = 63
    Top = 62
    Width = 59
    Height = 21
    EditMask = '0999;0; '
    MaxLength = 4
    TabOrder = 2
  end
  object edSailors: TMaskEdit
    Left = 63
    Top = 149
    Width = 59
    Height = 21
    EditMask = '0999;0; '
    MaxLength = 4
    TabOrder = 5
  end
  object cbRoad: TComboBox
    Left = 262
    Top = 149
    Width = 87
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 13
    Items.Strings = (
      'no'
      'North'
      'Northeast'
      'Southeast'
      'South'
      'Southwest'
      'Northwest')
  end
  object bnOk: TButton
    Left = 119
    Top = 332
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 15
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 311
    Top = 331
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 16
  end
  object cbIncProd: TComboBox
    Left = 323
    Top = 122
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 12
    OnChange = cbInputChange
  end
end
