object BattleSimForm: TBattleSimForm
  Left = 275
  Top = 163
  Caption = 'p'
  ClientHeight = 628
  ClientWidth = 860
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 860
    Height = 289
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 252
      Top = 34
      Height = 255
      ExplicitLeft = 280
      ExplicitTop = 120
      ExplicitHeight = 100
    end
    object lbAtts: TListBox
      Left = 0
      Top = 34
      Width = 252
      Height = 255
      Align = alLeft
      ItemHeight = 13
      TabOrder = 0
      OnClick = lbAttsDefsClick
    end
    object Panel2: TPanel
      Left = 0
      Top = 0
      Width = 860
      Height = 34
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object labMountType: TLabel
        Left = 104
        Top = 5
        Width = 121
        Height = 17
        AutoSize = False
        Caption = 'labMountType'
        Layout = tlBottom
      end
      object cbRegType: TComboBox
        Left = 4
        Top = 5
        Width = 97
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = cbRegTypeChange
      end
      object bnRun: TButton
        Left = 208
        Top = 5
        Width = 49
        Height = 25
        Caption = 'Run'
        Default = True
        TabOrder = 1
        OnClick = bnRunClick
      end
      object bnAddAtt: TButton
        Left = 264
        Top = 5
        Width = 75
        Height = 25
        Caption = 'Add Attacker'
        TabOrder = 2
        OnClick = bnAddAttClick
      end
      object bnDelete: TButton
        Left = 344
        Top = 5
        Width = 41
        Height = 25
        Caption = 'Delete'
        TabOrder = 3
        OnClick = bnDeleteClick
      end
      object bnClearAtt: TButton
        Left = 392
        Top = 5
        Width = 81
        Height = 25
        Caption = 'Clear Attackers'
        TabOrder = 4
        OnClick = bnClearAttClick
      end
      object bnSwap: TButton
        Left = 480
        Top = 5
        Width = 41
        Height = 25
        Caption = 'Swap'
        TabOrder = 5
        OnClick = bnSwapClick
      end
      object bnSwapAll: TButton
        Left = 528
        Top = 5
        Width = 49
        Height = 25
        Caption = 'Swap All'
        TabOrder = 6
        OnClick = bnSwapAllClick
      end
      object bnAddDef: TButton
        Left = 584
        Top = 5
        Width = 75
        Height = 25
        Caption = 'Add Defender'
        TabOrder = 7
        OnClick = bnAddDefClick
      end
      object bnSave: TButton
        Left = 664
        Top = 5
        Width = 57
        Height = 25
        Caption = 'Save'
        TabOrder = 8
        OnClick = bnSaveClick
      end
      object bnLoad: TButton
        Left = 728
        Top = 5
        Width = 57
        Height = 25
        Caption = 'Load'
        TabOrder = 9
        OnClick = bnLoadClick
      end
    end
    object Panel3: TPanel
      Left = 255
      Top = 34
      Width = 605
      Height = 255
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 392
        Height = 255
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object labCurUnit: TLabel
          Left = 7
          Top = 6
          Width = 385
          Height = 25
          AutoSize = False
          Caption = 'labCurUnit'
          WordWrap = True
        end
        object labAnalRes: TLabel
          Left = 255
          Top = 196
          Width = 137
          Height = 43
          AutoSize = False
          Caption = 'labAnalRes'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -8
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object Label1: TLabel
          Left = 175
          Top = 166
          Width = 73
          Height = 17
          AutoSize = False
          Caption = 'Combat Spell'
          Layout = tlBottom
        end
        object Label2: TLabel
          Left = 295
          Top = 129
          Width = 57
          Height = 13
          Caption = 'MaxRounds'
        end
        object lbItems: TListBox
          Left = 7
          Top = 38
          Width = 161
          Height = 80
          ItemHeight = 13
          TabOrder = 0
          OnClick = lbItemsClick
        end
        object lbSkills: TListBox
          Left = 7
          Top = 126
          Width = 161
          Height = 113
          ItemHeight = 13
          TabOrder = 1
          OnClick = lbSkillsClick
        end
        object cbItems: TComboBox
          Left = 175
          Top = 38
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 2
        end
        object cbSkills: TComboBox
          Left = 175
          Top = 118
          Width = 113
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 3
        end
        object edItem: TEdit
          Left = 175
          Top = 62
          Width = 49
          Height = 21
          TabOrder = 4
        end
        object edSkill: TEdit
          Left = 175
          Top = 142
          Width = 49
          Height = 21
          TabOrder = 5
        end
        object bnSetItem: TButton
          Left = 231
          Top = 62
          Width = 57
          Height = 21
          Caption = 'Set'
          TabOrder = 6
          OnClick = bnSetItemClick
        end
        object bnSetSkill: TButton
          Left = 231
          Top = 142
          Width = 57
          Height = 21
          Caption = 'Set'
          TabOrder = 7
          OnClick = bnSetSkillClick
        end
        object cbBehind: TCheckBox
          Left = 175
          Top = 94
          Width = 97
          Height = 17
          Caption = 'Behind'
          TabOrder = 8
          OnClick = cbBehindClick
        end
        object bnView: TButton
          Left = 303
          Top = 94
          Width = 75
          Height = 25
          Caption = 'View'
          TabOrder = 9
          OnClick = bnViewClick
        end
        object bnAnalyze: TButton
          Left = 175
          Top = 198
          Width = 75
          Height = 25
          Caption = 'Analyze'
          TabOrder = 10
          OnClick = bnAnalyzeClick
        end
        object cbCombatSpell: TComboBox
          Left = 247
          Top = 166
          Width = 137
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 11
          OnClick = cbCombatSpellClick
        end
        object bnSoldiers: TButton
          Left = 303
          Top = 62
          Width = 75
          Height = 25
          Caption = 'View Soldiers'
          TabOrder = 12
          OnClick = bnSoldiersClick
        end
        object cbShortMode: TCheckBox
          Left = 175
          Top = 224
          Width = 81
          Height = 17
          Caption = 'Short Mode'
          TabOrder = 13
          OnClick = cbShortModeClick
        end
        object edMaxRounds: TEdit
          Left = 359
          Top = 126
          Width = 25
          Height = 21
          Hint = 'Empty - no limits, 0 - only free round'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 14
        end
      end
      object lbDefs: TListBox
        Left = 392
        Top = 0
        Width = 213
        Height = 255
        Align = alClient
        ItemHeight = 13
        TabOrder = 1
        OnClick = lbAttsDefsClick
      end
    end
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 320
    Top = 88
  end
  object SaveDialog: TSaveDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 352
    Top = 88
  end
end
