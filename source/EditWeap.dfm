object fEditWeap: TfEditWeap
  Left = 205
  Top = 106
  Caption = 'Edit weapon'
  ClientHeight = 329
  ClientWidth = 657
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    657
    329)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 14
    Width = 145
    Height = 13
    Caption = 'Nedd skill to wield this weapon'
  end
  object lbSk2: TLabel
    Left = 302
    Top = 14
    Width = 9
    Height = 13
    Caption = 'or'
    Visible = False
  end
  object Label2: TLabel
    Left = 8
    Top = 42
    Width = 63
    Height = 13
    Caption = 'Attack bonus'
  end
  object Label3: TLabel
    Left = 152
    Top = 42
    Width = 72
    Height = 13
    Caption = 'Defense bonus'
  end
  object Label4: TLabel
    Left = 296
    Top = 42
    Width = 82
    Height = 13
    Caption = 'Bonus if mounted'
  end
  object Label5: TLabel
    Left = 8
    Top = 69
    Width = 75
    Height = 13
    Caption = 'Number attacks'
  end
  object Label6: TLabel
    Left = 122
    Top = 69
    Width = 15
    Height = 13
    Caption = 'per'
  end
  object Label7: TLabel
    Left = 176
    Top = 69
    Width = 38
    Height = 13
    Caption = 'round(s)'
  end
  object Label8: TLabel
    Left = 250
    Top = 69
    Width = 68
    Height = 13
    Caption = 'Weapon class'
  end
  object Label9: TLabel
    Left = 470
    Top = 69
    Width = 54
    Height = 13
    Caption = 'Attack type'
  end
  object cbSkill: TComboBox
    Left = 166
    Top = 10
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 0
    OnChange = cbSkillChange
  end
  object cbSkill2: TComboBox
    Left = 323
    Top = 10
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 1
    Visible = False
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 89
    Width = 226
    Height = 198
    Caption = ' Weapon type '
    TabOrder = 9
    object cbALWAYSREADY: TCheckBox
      Left = 9
      Top = 15
      Width = 214
      Height = 17
      Caption = 'Ignore the 50% chance to attack'
      TabOrder = 0
    end
    object cbNODEFENSE: TCheckBox
      Left = 9
      Top = 33
      Width = 214
      Height = 17
      Caption = 'No combat defense against this weapon'
      TabOrder = 1
    end
    object cbNOFOOT: TCheckBox
      Left = 9
      Top = 51
      Width = 214
      Height = 17
      Caption = 'Weapon cannot be used on foot'
      TabOrder = 2
    end
    object cbNOMOUNT: TCheckBox
      Left = 9
      Top = 69
      Width = 214
      Height = 17
      Caption = 'Weapon cannot be used mounted'
      TabOrder = 3
    end
    object cbSHORT: TCheckBox
      Left = 9
      Top = 87
      Width = 214
      Height = 17
      Caption = 'Short melee weapon'
      TabOrder = 4
    end
    object cbLONG: TCheckBox
      Left = 9
      Top = 105
      Width = 214
      Height = 17
      Caption = 'Long melee weapon'
      TabOrder = 5
    end
    object cbRANGED: TCheckBox
      Left = 9
      Top = 123
      Width = 214
      Height = 17
      Caption = 'Missile (ranged) weapon'
      TabOrder = 6
    end
    object cbNOATTACKERSKILL: TCheckBox
      Left = 9
      Top = 141
      Width = 214
      Height = 17
      Caption = 'Attacker gets no combat/skill defense'
      TabOrder = 7
    end
    object cbRIDINGBONUS: TCheckBox
      Left = 9
      Top = 159
      Width = 214
      Height = 17
      Caption = 'Unit gets riding bonus on att and def'
      TabOrder = 8
    end
    object cbRIDINGBONUSDEFENSE: TCheckBox
      Left = 9
      Top = 177
      Width = 214
      Height = 17
      Caption = 'Unit gets riding bonus on def only'
      TabOrder = 9
    end
  end
  object edAttBonus: TMaskEdit
    Left = 88
    Top = 38
    Width = 30
    Height = 21
    EditMask = '9;1; '
    MaxLength = 1
    TabOrder = 2
    Text = '0'
  end
  object edDefBonus: TMaskEdit
    Left = 232
    Top = 38
    Width = 30
    Height = 21
    EditMask = '9;1; '
    MaxLength = 1
    TabOrder = 3
    Text = '0'
  end
  object edMountBonus: TMaskEdit
    Left = 392
    Top = 38
    Width = 30
    Height = 21
    EditMask = '9;1; '
    MaxLength = 1
    TabOrder = 4
    Text = '0'
  end
  object edNumAtt: TMaskEdit
    Left = 89
    Top = 65
    Width = 30
    Height = 21
    EditMask = '9;1;0'
    MaxLength = 1
    TabOrder = 5
    Text = '1'
  end
  object edPerRound: TMaskEdit
    Left = 140
    Top = 65
    Width = 30
    Height = 21
    EditMask = '9;1;0'
    MaxLength = 1
    TabOrder = 6
    Text = '1'
  end
  object Memo: TMemo
    Left = 250
    Top = 92
    Width = 384
    Height = 223
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 10
  end
  object cbWeapClass: TComboBox
    Left = 324
    Top = 65
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 7
    Items.Strings = (
      'Slashing'
      'Piercing'
      'Crushing'
      'Cleaving'
      'Armor Piercing')
  end
  object cbAttType: TComboBox
    Left = 529
    Top = 65
    Width = 121
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 8
    Items.Strings = (
      'Combat'
      'Energy'
      'Spirit'
      'Weather'
      'Riding'
      'Ranged')
  end
  object bnCancel: TButton
    Left = 130
    Top = 296
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 12
    OnClick = bnCancelClick
  end
  object bnOk: TButton
    Left = 18
    Top = 296
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 11
    OnClick = bnOkClick
  end
end
