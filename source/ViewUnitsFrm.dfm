object ViewUnitsForm: TViewUnitsForm
  Left = 0
  Top = 128
  Align = alBottom
  BorderStyle = bsNone
  Caption = 'View Units'
  ClientHeight = 172
  ClientWidth = 1016
  Color = clBtnFace
  Constraints.MinHeight = 50
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 1016
    Height = 23
    AutoSize = True
    ButtonHeight = 21
    ButtonWidth = 41
    ShowCaptions = True
    TabOrder = 0
    object labUnits: TLabel
      Left = 0
      Top = 2
      Width = 81
      Height = 21
      AutoSize = False
      Caption = 'labUnits'
      Layout = tlCenter
    end
    object Label1: TLabel
      Left = 81
      Top = 2
      Width = 40
      Height = 21
      AutoSize = False
      Caption = '&Region'
      FocusControl = cbRegDiap
      Layout = tlCenter
    end
    object cbRegDiap: TComboBox
      Left = 121
      Top = 2
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 0
      OnChange = cbRegDiapChange
      Items.Strings = (
        'current only'
        'current and neighbors'
        'all'
        'from RegionList')
    end
    object Label2: TLabel
      Left = 266
      Top = 0
      Width = 47
      Height = 21
      AutoSize = False
      Caption = '  &Faction'
      FocusControl = cbFacDiap
      Layout = tlCenter
    end
    object cbFacDiap: TComboBox
      Left = 313
      Top = 0
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      Sorted = True
      TabOrder = 1
      OnChange = cbFacDiapChange
      OnEnter = cbFacDiapEnter
      OnKeyPress = cbFacDiapKeyPress
    end
    object ToolButton2: TToolButton
      Left = 458
      Top = 0
      Width = 8
      ImageIndex = 0
      Style = tbsSeparator
    end
    object cbExpression: TCheckBox
      Left = 466
      Top = 0
      Width = 71
      Height = 21
      Caption = 'Use expr.:'
      TabOrder = 2
      OnClick = cbExpressionClick
    end
    object edExpr: TEdit
      Left = 537
      Top = 0
      Width = 121
      Height = 21
      TabOrder = 3
      OnExit = edExprExit
      OnKeyPress = edExprKeyPress
    end
    object bnEditExpr: TButton
      Left = 658
      Top = 0
      Width = 15
      Height = 21
      Caption = '...'
      TabOrder = 6
      OnClick = bnEditExprClick
    end
    object ToolButton1: TToolButton
      Left = 673
      Top = 0
      Width = 8
      ImageIndex = 2
      Style = tbsSeparator
    end
    object cbHideObjects: TCheckBox
      Left = 681
      Top = 0
      Width = 86
      Height = 21
      Caption = 'Hide Objects'
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = cbHideObjectsClick
    end
    object tbSetup: TToolButton
      Left = 767
      Top = 0
      Caption = 'Se&tup'
      ImageIndex = 1
      OnClick = bnSetupClick
    end
    object ToolButton3: TToolButton
      Left = 808
      Top = 0
      Width = 8
      ImageIndex = 1
      Style = tbsSeparator
    end
    object cbColumnSets: TComboBox
      Left = 816
      Top = 0
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 0
      TabOrder = 5
      OnClick = cbColumnSetsClick
    end
  end
  object Grid: TStringGrid
    Left = 0
    Top = 21
    Width = 1016
    Height = 151
    Hint = 'DblClick: on header to sort, on unit to goto'
    Align = alClient
    ColCount = 10
    DefaultColWidth = 100
    DefaultRowHeight = 18
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnDblClick = GridDblClick
    OnDrawCell = GridDrawCell
    OnExit = FormHide
    OnMouseDown = GridMouseDown
    OnMouseMove = GridMouseMove
    ExplicitTop = 23
    ExplicitHeight = 149
    ColWidths = (
      100
      46
      100
      54
      134
      100
      100
      100
      100
      100)
  end
end
