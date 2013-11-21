object fSkillsTree: TfSkillsTree
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Tree of Skills'
  ClientHeight = 440
  ClientWidth = 668
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 225
    Height = 440
    Align = alLeft
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 223
      Height = 28
      Align = alTop
      TabOrder = 0
      object btSave: TSpeedButton
        Left = 166
        Top = 3
        Width = 49
        Height = 22
        Caption = 'Save'
        Enabled = False
        OnClick = btSaveClick
      end
      object btQ: TSpeedButton
        Left = 8
        Top = 3
        Width = 71
        Height = 22
        Caption = 'Requery info'
        Enabled = False
        OnClick = btQClick
      end
      object btQall: TSpeedButton
        Left = 89
        Top = 3
        Width = 71
        Height = 22
        Caption = 'Requery All'
        OnClick = btQallClick
      end
    end
    object TreeS: TTreeView
      Left = 1
      Top = 29
      Width = 223
      Height = 410
      Align = alClient
      Indent = 19
      TabOrder = 1
      OnChange = TreeSChange
      OnChanging = TreeSChanging
      OnExpanding = TreeSExpanding
    end
  end
  object Panel3: TPanel
    Left = 225
    Top = 0
    Width = 443
    Height = 440
    Align = alClient
    TabOrder = 1
    object gbInf: TGroupBox
      Left = 1
      Top = 1
      Width = 441
      Height = 438
      Align = alClient
      Caption = ' Skill info '
      TabOrder = 0
      OnClick = edLevChange
      object Label1: TLabel
        Left = 10
        Top = 19
        Width = 24
        Height = 13
        Caption = 'Skill :'
      end
      object lbAbr: TLabel
        Left = 42
        Top = 19
        Width = 23
        Height = 13
        Caption = 'ABR'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label3: TLabel
        Left = 98
        Top = 19
        Width = 31
        Height = 13
        Caption = 'Type :'
      end
      object Label2: TLabel
        Left = 290
        Top = 19
        Width = 77
        Height = 13
        Caption = 'Cost for Study :'
      end
      object Label4: TLabel
        Left = 10
        Top = 60
        Width = 51
        Height = 13
        Caption = 'Based on :'
      end
      object lbAnd1: TLabel
        Left = 220
        Top = 81
        Width = 18
        Height = 13
        Caption = 'and'
        Visible = False
      end
      object lbAnd2: TLabel
        Left = 220
        Top = 100
        Width = 18
        Height = 13
        Caption = 'and'
        Visible = False
      end
      object Label5: TLabel
        Left = 272
        Top = 43
        Width = 49
        Height = 13
        Caption = 'Specialists'
      end
      object Label6: TLabel
        Left = 10
        Top = 147
        Width = 62
        Height = 13
        Caption = 'Descriptions:'
      end
      object cbTypeS: TComboBox
        Left = 135
        Top = 15
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = 'Foundation'
        OnChange = edLevChange
        Items.Strings = (
          'Foundation'
          'Magic'
          'Simple')
      end
      object edCost: TCSpinEdit
        Left = 373
        Top = 14
        Width = 54
        Height = 22
        TabOrder = 1
        OnChange = edLevChange
      end
      object cbSkill: TComboBox
        Left = 11
        Top = 78
        Width = 141
        Height = 21
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 4
        OnChange = cbSkillChange
      end
      object cbSkill2: TComboBox
        Left = 10
        Top = 100
        Width = 141
        Height = 21
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 6
        Visible = False
        OnChange = cbSkill2Change
      end
      object cbSkill3: TComboBox
        Left = 10
        Top = 124
        Width = 141
        Height = 21
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 8
        Visible = False
        OnChange = cbSkill3Change
      end
      object edLev: TCSpinEdit
        Left = 160
        Top = 76
        Width = 54
        Height = 22
        MaxValue = 5
        MinValue = 1
        TabOrder = 5
        Value = 1
        Visible = False
        OnChange = edLevChange
      end
      object edLev2: TCSpinEdit
        Left = 160
        Top = 99
        Width = 54
        Height = 22
        MaxValue = 5
        MinValue = 1
        TabOrder = 7
        Value = 1
        Visible = False
        OnChange = edLevChange
      end
      object edLev3: TCSpinEdit
        Left = 160
        Top = 124
        Width = 54
        Height = 22
        MaxValue = 5
        MinValue = 1
        TabOrder = 9
        Value = 1
        Visible = False
        OnChange = edLevChange
      end
      object grSpec: TStringGrid
        Left = 272
        Top = 57
        Width = 158
        Height = 101
        ColCount = 2
        DefaultRowHeight = 15
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
        TabOrder = 10
        ColWidths = (
          124
          26)
      end
      object cbCast: TCheckBox
        Left = 10
        Top = 41
        Width = 89
        Height = 17
        Caption = 'Can be Cast'
        TabOrder = 2
        OnClick = edLevChange
      end
      object cbComb: TCheckBox
        Left = 128
        Top = 41
        Width = 97
        Height = 17
        Caption = 'Combat'
        TabOrder = 3
      end
      object Memo1: TMemo
        Left = 2
        Top = 164
        Width = 437
        Height = 272
        Align = alBottom
        ScrollBars = ssVertical
        TabOrder = 11
      end
    end
  end
end
