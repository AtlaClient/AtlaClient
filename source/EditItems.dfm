object fEditItems: TfEditItems
  Left = 153
  Top = 124
  ActiveControl = PageControl1
  Caption = 'Item Editor'
  ClientHeight = 367
  ClientWidth = 619
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  DesignSize = (
    619
    367)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 169
    Height = 132
    TabOrder = 0
    object Label1: TLabel
      Left = 5
      Top = 11
      Width = 31
      Height = 17
      AutoSize = False
      Caption = 'Name'
      Layout = tlBottom
    end
    object Label2: TLabel
      Left = 5
      Top = 59
      Width = 33
      Height = 17
      AutoSize = False
      Caption = 'Abr'
      Layout = tlBottom
    end
    object Label4: TLabel
      Left = 5
      Top = 35
      Width = 33
      Height = 17
      AutoSize = False
      Caption = 'Plural'
      Layout = tlBottom
    end
    object Label3: TLabel
      Left = 5
      Top = 85
      Width = 24
      Height = 13
      Caption = 'Type'
    end
    object Label5: TLabel
      Left = 5
      Top = 109
      Width = 34
      Height = 13
      Caption = 'Weight'
    end
    object edName: TEdit
      Left = 44
      Top = 9
      Width = 121
      Height = 21
      TabOrder = 0
    end
    object edAbr: TEdit
      Left = 44
      Top = 57
      Width = 41
      Height = 21
      ReadOnly = True
      TabOrder = 2
    end
    object edNames: TEdit
      Left = 44
      Top = 33
      Width = 121
      Height = 21
      TabOrder = 1
    end
    object cbItType: TComboBox
      Left = 44
      Top = 81
      Width = 123
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 3
      OnChange = cbItTypeChange
      Items.Strings = (
        'Other'
        'Man'
        'Monster'
        'Mount'
        'Weapon'
        'Armor'
        'Unfinished object')
    end
    object edWeight: TCSpinEdit
      Left = 45
      Top = 105
      Width = 39
      Height = 22
      TabOrder = 4
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 132
    Width = 169
    Height = 148
    TabOrder = 1
    object cbNormal: TCheckBox
      Left = 5
      Top = 8
      Width = 65
      Height = 16
      Caption = 'Normal'
      TabOrder = 0
    end
    object cbMagic: TCheckBox
      Left = 5
      Top = 74
      Width = 65
      Height = 16
      Caption = 'Magic'
      TabOrder = 4
    end
    object cbTrade: TCheckBox
      Left = 5
      Top = 40
      Width = 65
      Height = 16
      Caption = 'Trade'
      TabOrder = 2
    end
    object cbAdvanced: TCheckBox
      Left = 5
      Top = 24
      Width = 73
      Height = 16
      Caption = 'Advanced'
      TabOrder = 1
    end
    object cbFood: TCheckBox
      Left = 5
      Top = 57
      Width = 65
      Height = 16
      Hint = 'It is Consumed ite'
      Caption = 'Food'
      TabOrder = 3
    end
    object cbCantGive: TCheckBox
      Left = 5
      Top = 107
      Width = 97
      Height = 17
      Caption = 'Can'#39't Give'
      TabOrder = 6
    end
    object cbCombat: TCheckBox
      Left = 5
      Top = 124
      Width = 97
      Height = 17
      Caption = 'Show in combat'
      TabOrder = 7
    end
    object cbBattle: TCheckBox
      Left = 5
      Top = 90
      Width = 65
      Height = 16
      Hint = 'Battle Item'
      Caption = 'Battle'
      TabOrder = 5
    end
  end
  object PageControl1: TPageControl
    Left = 171
    Top = 0
    Width = 446
    Height = 201
    ActivePage = tbProd
    HotTrack = True
    TabOrder = 2
    object tbProd: TTabSheet
      BorderWidth = 1
      Caption = 'Production'
      object GroupBox3: TGroupBox
        Left = 0
        Top = 0
        Width = 436
        Height = 171
        Align = alClient
        TabOrder = 0
        object Label6: TLabel
          Left = 5
          Top = 10
          Width = 19
          Height = 13
          Caption = 'Skill'
          Layout = tlBottom
        end
        object labLevel: TLabel
          Left = 139
          Top = 10
          Width = 26
          Height = 13
          Caption = 'Level'
          Layout = tlBottom
        end
        object cbSkill: TComboBox
          Left = 5
          Top = 25
          Width = 128
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = cbSkillChange
        end
        object cbLevel: TComboBox
          Left = 139
          Top = 25
          Width = 55
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5')
        end
        object PanelProd: TPanel
          Left = 4
          Top = 50
          Width = 200
          Height = 106
          BevelOuter = bvNone
          TabOrder = 2
          object labHelpFrom: TLabel
            Left = 52
            Top = 77
            Width = 20
            Height = 13
            Caption = 'from'
            Layout = tlBottom
          end
          object labHelpProd: TLabel
            Left = 7
            Top = 56
            Width = 83
            Height = 13
            Caption = 'Production bonus'
            Layout = tlBottom
          end
          object labPer: TLabel
            Left = 54
            Top = 31
            Width = 15
            Height = 13
            Caption = 'per'
          end
          object labManMonth: TLabel
            Left = 126
            Top = 31
            Width = 52
            Height = 13
            Caption = 'man-month'
          end
          object labRate: TLabel
            Left = 7
            Top = 7
            Width = 35
            Height = 13
            Caption = 'Rate of'
          end
          object cbHelpProd: TComboBox
            Left = 77
            Top = 73
            Width = 117
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            TabOrder = 3
          end
          object edHelpMult: TCSpinEdit
            Left = 7
            Top = 73
            Width = 39
            Height = 22
            TabOrder = 2
          end
          object edMonths: TCSpinEdit
            Left = 77
            Top = 26
            Width = 39
            Height = 22
            TabOrder = 1
          end
          object edOut: TCSpinEdit
            Left = 7
            Top = 26
            Width = 39
            Height = 22
            TabOrder = 0
          end
        end
        object PanelInput: TPanel
          Left = 212
          Top = 8
          Width = 161
          Height = 153
          BevelOuter = bvNone
          TabOrder = 3
          object labInput: TLabel
            Left = 8
            Top = 10
            Width = 24
            Height = 13
            Caption = 'Input'
            Layout = tlBottom
          end
          object btAdd1: TSpeedButton
            Tag = 1
            Left = 80
            Top = 129
            Width = 28
            Height = 22
            Caption = 'Add'
            OnClick = btAdd1Click
          end
          object btDel1: TSpeedButton
            Tag = 1
            Left = 112
            Top = 129
            Width = 28
            Height = 22
            Caption = 'Del'
            OnClick = btDel1Click
          end
          object cbOrIn: TCheckBox
            Left = 93
            Top = 8
            Width = 52
            Height = 17
            Caption = 'any of'
            TabOrder = 0
          end
          object grInput: TStringGrid
            Left = 8
            Top = 25
            Width = 134
            Height = 99
            ColCount = 2
            DefaultRowHeight = 18
            FixedCols = 0
            RowCount = 1
            FixedRows = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
            TabOrder = 1
            OnDblClick = grInputDblClick
            ColWidths = (
              103
              26)
          end
        end
      end
    end
    object tbMProd: TTabSheet
      Caption = 'Mag.production'
      ImageIndex = 1
      object GroupBox11: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label27: TLabel
          Left = 5
          Top = 19
          Width = 23
          Height = 13
          Caption = 'Spell'
        end
        object Label28: TLabel
          Left = 217
          Top = 10
          Width = 24
          Height = 13
          Caption = 'Input'
          Layout = tlBottom
        end
        object btAdd2: TSpeedButton
          Tag = 2
          Left = 288
          Top = 129
          Width = 28
          Height = 22
          Caption = 'Add'
          OnClick = btAdd1Click
        end
        object btDel2: TSpeedButton
          Tag = 2
          Left = 320
          Top = 129
          Width = 28
          Height = 22
          Caption = 'Del'
          OnClick = btDel1Click
        end
        object Label29: TLabel
          Left = 5
          Top = 45
          Width = 26
          Height = 13
          Caption = 'Level'
          Layout = tlBottom
        end
        object cbSpellMP: TComboBox
          Left = 48
          Top = 15
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
        object grInputMP: TStringGrid
          Left = 216
          Top = 25
          Width = 134
          Height = 99
          ColCount = 2
          DefaultRowHeight = 18
          FixedCols = 0
          RowCount = 1
          FixedRows = 0
          TabOrder = 3
          OnDblClick = grInputDblClick
          ColWidths = (
            103
            26)
        end
        object cbLevMP: TComboBox
          Left = 48
          Top = 41
          Width = 55
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5')
        end
        object cbAnyMP: TCheckBox
          Left = 301
          Top = 8
          Width = 52
          Height = 17
          Caption = 'any of'
          TabOrder = 2
        end
      end
    end
    object tbArmor: TTabSheet
      Caption = 'Armor'
      ImageIndex = 2
      object GroupBox5: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label7: TLabel
          Left = 5
          Top = 75
          Width = 76
          Height = 13
          Caption = 'slashing attacks'
        end
        object Label8: TLabel
          Left = 5
          Top = 98
          Width = 75
          Height = 13
          Caption = 'piercing attacks'
        end
        object Label9: TLabel
          Left = 5
          Top = 121
          Width = 78
          Height = 13
          Caption = 'crushing attacks'
        end
        object Label10: TLabel
          Left = 5
          Top = 145
          Width = 78
          Height = 13
          Caption = 'cleaving attacks'
        end
        object Label11: TLabel
          Left = 176
          Top = 75
          Width = 104
          Height = 13
          Caption = 'armor-piercing attacks'
        end
        object Label12: TLabel
          Left = 210
          Top = 98
          Width = 70
          Height = 13
          Caption = 'energy attacks'
        end
        object Label13: TLabel
          Left = 221
          Top = 121
          Width = 59
          Height = 13
          Caption = 'spirit attacks'
        end
        object Label14: TLabel
          Left = 204
          Top = 145
          Width = 76
          Height = 13
          Caption = 'weather attacks'
        end
        object Label15: TLabel
          Left = 5
          Top = 40
          Width = 60
          Height = 13
          Caption = 'Protect from '
        end
        object Label16: TLabel
          Left = 5
          Top = 12
          Width = 50
          Height = 13
          Caption = 'Armor type'
        end
        object edFrom: TEdit
          Left = 79
          Top = 36
          Width = 74
          Height = 21
          TabOrder = 2
          Text = '0'
        end
        object cbAT: TComboBox
          Left = 79
          Top = 8
          Width = 73
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          Items.Strings = (
            '1'
            '2'
            '3'
            '4')
        end
        object cbAss: TCheckBox
          Left = 176
          Top = 10
          Width = 125
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Used in assassination'
          TabOrder = 1
        end
        object edAd1: TCSpinEdit
          Left = 87
          Top = 70
          Width = 50
          Height = 22
          TabOrder = 3
        end
        object edAd2: TCSpinEdit
          Left = 87
          Top = 93
          Width = 50
          Height = 22
          TabOrder = 4
        end
        object edAd3: TCSpinEdit
          Left = 87
          Top = 116
          Width = 50
          Height = 22
          TabOrder = 5
        end
        object edAd4: TCSpinEdit
          Left = 87
          Top = 140
          Width = 50
          Height = 22
          TabOrder = 6
        end
        object edAd5: TCSpinEdit
          Left = 286
          Top = 70
          Width = 50
          Height = 22
          TabOrder = 7
        end
        object edAd6: TCSpinEdit
          Left = 286
          Top = 93
          Width = 50
          Height = 22
          TabOrder = 8
        end
        object edAd7: TCSpinEdit
          Left = 286
          Top = 116
          Width = 50
          Height = 22
          TabOrder = 9
        end
        object edAd8: TCSpinEdit
          Left = 286
          Top = 140
          Width = 50
          Height = 22
          TabOrder = 10
        end
      end
    end
    object tbWeapon: TTabSheet
      Caption = 'Weapon'
      ImageIndex = 3
      object GroupBox4: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label17: TLabel
          Left = 7
          Top = 13
          Width = 19
          Height = 13
          Caption = 'Skill'
        end
        object lbSk2: TLabel
          Left = 17
          Top = 35
          Width = 9
          Height = 13
          Caption = 'or'
          Visible = False
        end
        object Label18: TLabel
          Left = 5
          Top = 105
          Width = 45
          Height = 13
          Caption = 'Att bonus'
        end
        object Label19: TLabel
          Left = 104
          Top = 105
          Width = 49
          Height = 13
          Caption = 'Def bonus'
        end
        object Label20: TLabel
          Left = 21
          Top = 152
          Width = 111
          Height = 13
          Caption = 'Bonus against mounted'
        end
        object Label24: TLabel
          Left = 5
          Top = 58
          Width = 68
          Height = 13
          Caption = 'Weapon class'
        end
        object Label25: TLabel
          Left = 5
          Top = 81
          Width = 54
          Height = 13
          Caption = 'Attack type'
        end
        object cbWskil1: TComboBox
          Left = 32
          Top = 8
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = cbWskil1Change
        end
        object cbWskil2: TComboBox
          Left = 32
          Top = 31
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Visible = False
        end
        object GroupBox6: TGroupBox
          Left = 206
          Top = 9
          Width = 226
          Height = 160
          Caption = ' Weapon type '
          TabOrder = 8
          object cbALWAYSREADY: TCheckBox
            Left = 6
            Top = 13
            Width = 214
            Height = 17
            Caption = 'Ignore the 50% chance to attack'
            TabOrder = 0
          end
          object cbNODEFENSE: TCheckBox
            Left = 6
            Top = 27
            Width = 214
            Height = 17
            Caption = 'No combat defense against this weapon'
            TabOrder = 1
          end
          object cbNOFOOT: TCheckBox
            Left = 6
            Top = 41
            Width = 214
            Height = 17
            Caption = 'Weapon cannot be used on foot'
            TabOrder = 2
          end
          object cbNOMOUNT: TCheckBox
            Left = 6
            Top = 55
            Width = 214
            Height = 17
            Caption = 'Weapon cannot be used mounted'
            TabOrder = 3
          end
          object cbSHORT: TCheckBox
            Left = 6
            Top = 69
            Width = 214
            Height = 17
            Caption = 'Short melee weapon'
            TabOrder = 4
          end
          object cbLONG: TCheckBox
            Left = 6
            Top = 83
            Width = 214
            Height = 17
            Caption = 'Long melee weapon'
            TabOrder = 5
          end
          object cbRANGED: TCheckBox
            Left = 6
            Top = 97
            Width = 214
            Height = 17
            Caption = 'Missile (ranged) weapon'
            TabOrder = 6
          end
          object cbNOATTACKERSKILL: TCheckBox
            Left = 6
            Top = 111
            Width = 214
            Height = 17
            Caption = 'Attacker gets no combat/skill defense'
            TabOrder = 7
          end
          object cbRIDINGBONUS: TCheckBox
            Left = 6
            Top = 125
            Width = 214
            Height = 17
            Caption = 'Unit gets riding bonus on att and def'
            TabOrder = 8
          end
          object cbRIDINGBONUSDEFENSE: TCheckBox
            Left = 6
            Top = 139
            Width = 214
            Height = 17
            Caption = 'Unit gets riding bonus on def only'
            TabOrder = 9
          end
        end
        object cbWeapClass: TComboBox
          Left = 77
          Top = 54
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 2
          Items.Strings = (
            'Slashing'
            'Piercing'
            'Crushing'
            'Cleaving'
            'Armor Piercing')
        end
        object cbAttType: TComboBox
          Left = 77
          Top = 77
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 3
          Items.Strings = (
            'Combat'
            'Energy'
            'Spirit'
            'Weather'
            'Riding'
            'Ranged')
        end
        object cbAttPR: TComboBox
          Left = 5
          Top = 125
          Width = 148
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 6
          Items.Strings = (
            'Attacks per round'
            'Rounds per attack'
            'Attacks as the skill'
            'Attacks as the skill / 2')
        end
        object edAttBonus: TCSpinEdit
          Left = 59
          Top = 100
          Width = 39
          Height = 22
          TabOrder = 4
        end
        object edDefBonus: TCSpinEdit
          Left = 159
          Top = 100
          Width = 39
          Height = 22
          TabOrder = 5
        end
        object edMountBonus: TCSpinEdit
          Left = 159
          Top = 147
          Width = 39
          Height = 22
          TabOrder = 7
        end
        object edNumAtt: TCSpinEdit
          Left = 159
          Top = 123
          Width = 39
          Height = 22
          TabOrder = 9
        end
      end
    end
    object tbMonster: TTabSheet
      Caption = 'Monster'
      Enabled = False
      ImageIndex = 5
      object GroupBox9: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label45: TLabel
          Left = 11
          Top = 13
          Width = 36
          Height = 13
          Caption = 'Combat'
        end
        object Label46: TLabel
          Left = 17
          Top = 35
          Width = 30
          Height = 13
          Caption = 'Tactic'
        end
        object Label47: TLabel
          Left = 11
          Top = 57
          Width = 36
          Height = 13
          Caption = 'Attacks'
        end
        object Label48: TLabel
          Left = 133
          Top = 13
          Width = 18
          Height = 13
          Caption = 'Hits'
        end
        object Label49: TLabel
          Left = 119
          Top = 35
          Width = 32
          Height = 13
          Caption = 'Regen'
        end
        object Label50: TLabel
          Left = 299
          Top = 13
          Width = 33
          Height = 13
          Caption = 'Stealth'
        end
        object Label51: TLabel
          Left = 275
          Top = 35
          Width = 57
          Height = 13
          Caption = 'Observation'
        end
        object Label52: TLabel
          Left = 124
          Top = 57
          Width = 27
          Height = 13
          Caption = 'Ability'
        end
        object GroupBox12: TGroupBox
          Left = 3
          Top = 72
          Width = 307
          Height = 97
          Caption = ' Defence '
          TabOrder = 9
          object Label37: TLabel
            Left = 9
            Top = 12
            Width = 76
            Height = 13
            Caption = 'slashing attacks'
          end
          object Label38: TLabel
            Left = 10
            Top = 34
            Width = 75
            Height = 13
            Caption = 'piercing attacks'
          end
          object Label39: TLabel
            Left = 7
            Top = 55
            Width = 78
            Height = 13
            Caption = 'crushing attacks'
          end
          object Label40: TLabel
            Left = 7
            Top = 77
            Width = 78
            Height = 13
            Caption = 'cleaving attacks'
          end
          object Label41: TLabel
            Left = 149
            Top = 12
            Width = 104
            Height = 13
            Caption = 'armor-piercing attacks'
          end
          object Label42: TLabel
            Left = 183
            Top = 34
            Width = 70
            Height = 13
            Caption = 'energy attacks'
          end
          object Label43: TLabel
            Left = 194
            Top = 55
            Width = 59
            Height = 13
            Caption = 'spirit attacks'
          end
          object Label44: TLabel
            Left = 177
            Top = 77
            Width = 76
            Height = 13
            Caption = 'weather attacks'
          end
          object edMonD2: TCSpinEdit
            Left = 91
            Top = 29
            Width = 39
            Height = 22
            TabOrder = 1
          end
          object edMonD1: TCSpinEdit
            Left = 91
            Top = 7
            Width = 39
            Height = 22
            TabOrder = 0
          end
          object edMonD4: TCSpinEdit
            Left = 91
            Top = 72
            Width = 39
            Height = 22
            TabOrder = 3
          end
          object edMonD3: TCSpinEdit
            Left = 91
            Top = 50
            Width = 39
            Height = 22
            TabOrder = 2
          end
          object edMonD8: TCSpinEdit
            Left = 259
            Top = 72
            Width = 39
            Height = 22
            TabOrder = 7
          end
          object edMonD7: TCSpinEdit
            Left = 259
            Top = 50
            Width = 39
            Height = 22
            TabOrder = 6
          end
          object edMonD6: TCSpinEdit
            Left = 259
            Top = 29
            Width = 39
            Height = 22
            TabOrder = 5
          end
          object edMonD5: TCSpinEdit
            Left = 259
            Top = 7
            Width = 39
            Height = 22
            TabOrder = 4
          end
        end
        object edMonAbil: TComboBox
          Left = 157
          Top = 53
          Width = 131
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 7
        end
        object edMonComb: TCSpinEdit
          Left = 52
          Top = 8
          Width = 39
          Height = 22
          TabOrder = 0
        end
        object edMonTact: TCSpinEdit
          Left = 52
          Top = 30
          Width = 39
          Height = 22
          TabOrder = 3
        end
        object edMonAtts: TCSpinEdit
          Left = 52
          Top = 52
          Width = 39
          Height = 22
          TabOrder = 6
        end
        object edMonHits: TCSpinEdit
          Left = 157
          Top = 8
          Width = 39
          Height = 22
          TabOrder = 1
        end
        object edMonObse: TCSpinEdit
          Left = 336
          Top = 30
          Width = 39
          Height = 22
          TabOrder = 5
        end
        object edMonStea: TCSpinEdit
          Left = 336
          Top = 8
          Width = 39
          Height = 22
          TabOrder = 2
        end
        object edMonRegen: TCSpinEdit
          Left = 157
          Top = 30
          Width = 39
          Height = 22
          TabOrder = 4
        end
        object edMonAbilLev: TCSpinEdit
          Left = 293
          Top = 52
          Width = 39
          Height = 22
          TabOrder = 8
        end
      end
    end
    object tbMount: TTabSheet
      Caption = 'Mount'
      ImageIndex = 6
      object GroupBox8: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label31: TLabel
          Left = 97
          Top = 15
          Width = 48
          Height = 13
          Caption = 'Need Skill'
        end
        object Label32: TLabel
          Left = 96
          Top = 41
          Width = 49
          Height = 13
          Caption = 'Min bonus'
        end
        object Label33: TLabel
          Left = 93
          Top = 66
          Width = 52
          Height = 13
          Caption = 'Max bonus'
        end
        object Label34: TLabel
          Left = 64
          Top = 92
          Width = 81
          Height = 13
          Caption = 'Hampered bonus'
        end
        object Label35: TLabel
          Left = 203
          Top = 41
          Width = 106
          Height = 13
          Caption = '(and minimal skill level)'
          Visible = False
        end
        object Label36: TLabel
          Left = 80
          Top = 136
          Width = 65
          Height = 13
          Caption = 'Special effect'
          Visible = False
        end
        object cbMountS: TComboBox
          Left = 153
          Top = 11
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
        end
        object cbMouMonstr: TCheckBox
          Left = 106
          Top = 112
          Width = 60
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Monster'
          TabOrder = 5
        end
        object cbMouSpec: TComboBox
          Left = 153
          Top = 132
          Width = 121
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 6
          Visible = False
        end
        object edMouHamB: TCSpinEdit
          Left = 153
          Top = 87
          Width = 39
          Height = 22
          TabOrder = 4
        end
        object edMouMaxB: TCSpinEdit
          Left = 153
          Top = 61
          Width = 39
          Height = 22
          TabOrder = 3
        end
        object edMouMinB: TCSpinEdit
          Left = 153
          Top = 36
          Width = 39
          Height = 22
          TabOrder = 1
        end
        object CSpinEdit32: TCSpinEdit
          Left = 323
          Top = 36
          Width = 39
          Height = 22
          TabOrder = 2
          Visible = False
        end
      end
    end
    object tbMan: TTabSheet
      Caption = 'Man'
      ImageIndex = 7
      object GroupBox7: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object SpeedButton4: TSpeedButton
          Tag = 3
          Left = 236
          Top = 145
          Width = 28
          Height = 22
          Caption = 'Del'
          OnClick = btDel1Click
        end
        object SpeedButton3: TSpeedButton
          Left = 202
          Top = 146
          Width = 28
          Height = 22
          Caption = 'Add'
          OnClick = SpeedButton3Click
        end
        object Label30: TLabel
          Left = 113
          Top = 19
          Width = 63
          Height = 13
          Caption = 'Default Level'
          Layout = tlBottom
        end
        object grManSL: TStringGrid
          Left = 112
          Top = 41
          Width = 152
          Height = 99
          ColCount = 2
          DefaultRowHeight = 18
          FixedCols = 0
          RowCount = 2
          TabOrder = 2
          ColWidths = (
            103
            25)
        end
        object cbLeader: TCheckBox
          Left = 16
          Top = 16
          Width = 97
          Height = 17
          Caption = 'Leader'
          TabOrder = 0
        end
        object edManDefL: TComboBox
          Left = 204
          Top = 17
          Width = 55
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 1
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5')
        end
      end
    end
    object tbMove: TTabSheet
      Caption = 'Capacity'
      ImageIndex = 7
      object GroupBox10: TGroupBox
        Left = 0
        Top = 0
        Width = 438
        Height = 173
        Align = alClient
        TabOrder = 0
        object Label21: TLabel
          Left = 28
          Top = 19
          Width = 25
          Height = 13
          Caption = 'Walk'
          Layout = tlBottom
        end
        object Label22: TLabel
          Left = 28
          Top = 47
          Width = 30
          Height = 13
          Caption = 'Riding'
          Layout = tlBottom
        end
        object Label23: TLabel
          Left = 28
          Top = 75
          Width = 27
          Height = 13
          Caption = 'Flying'
          Layout = tlBottom
        end
        object Label26: TLabel
          Left = 28
          Top = 103
          Width = 25
          Height = 13
          Caption = 'Swim'
          Layout = tlBottom
        end
        object edSwim: TCSpinEdit
          Left = 67
          Top = 98
          Width = 39
          Height = 22
          TabOrder = 3
        end
        object edFly: TCSpinEdit
          Left = 67
          Top = 70
          Width = 39
          Height = 22
          TabOrder = 2
        end
        object edRide: TCSpinEdit
          Left = 67
          Top = 42
          Width = 39
          Height = 22
          TabOrder = 1
        end
        object edWalk: TCSpinEdit
          Left = 66
          Top = 14
          Width = 39
          Height = 22
          TabOrder = 0
        end
      end
    end
  end
  object Memo: TMemo
    Left = 171
    Top = 205
    Width = 442
    Height = 157
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 3
  end
  object Button1: TButton
    Left = 8
    Top = 294
    Width = 75
    Height = 25
    Caption = 'Save'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 92
    Top = 294
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
end
