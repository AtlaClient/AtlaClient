object ViewChangesForm: TViewChangesForm
  Left = 135
  Top = 97
  Caption = 'View Changes'
  ClientHeight = 586
  ClientWidth = 720
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  WindowState = wsMaximized
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object splitHorz: TSplitter
    Left = 0
    Top = 301
    Width = 720
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 282
  end
  object splitVert: TSplitter
    Left = 337
    Top = 25
    Height = 276
    ExplicitHeight = 257
  end
  object sgAdded: TStringGrid
    Left = 0
    Top = 25
    Width = 337
    Height = 276
    Align = alLeft
    ColCount = 1
    DefaultColWidth = 1000
    FixedCols = 0
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    TabOrder = 0
    OnDblClick = sgAddedDblClick
    ExplicitHeight = 257
  end
  object sgDeleted: TStringGrid
    Left = 340
    Top = 25
    Width = 380
    Height = 276
    Align = alClient
    ColCount = 1
    DefaultColWidth = 1000
    FixedCols = 0
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    TabOrder = 1
    OnDblClick = sgDeletedDblClick
    ExplicitHeight = 257
  end
  object sgChanged: TStringGrid
    Left = 0
    Top = 304
    Width = 720
    Height = 282
    Align = alBottom
    ColCount = 1
    DefaultColWidth = 1000
    FixedCols = 0
    RowCount = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goThumbTracking]
    TabOrder = 2
    OnDblClick = sgChangedDblClick
    ExplicitTop = 285
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 720
    Height = 25
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    object Label1: TLabel
      Left = 8
      Top = 4
      Width = 34
      Height = 13
      Caption = '&Region'
      FocusControl = cbRegDiap
    end
    object Label2: TLabel
      Left = 296
      Top = 4
      Width = 35
      Height = 13
      Caption = '&Faction'
      FocusControl = cbFacDiap
    end
    object cbRegDiap: TComboBox
      Left = 48
      Top = 0
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = cbRegDiapChange
      Items.Strings = (
        'base only'
        'base and neighbors'
        'all')
    end
    object cbFacDiap: TComboBox
      Left = 336
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
    object bnSetBaseReg: TButton
      Left = 200
      Top = 0
      Width = 89
      Height = 21
      Caption = 'Set &Base Region'
      TabOrder = 2
      OnClick = bnSetBaseRegClick
    end
  end
  object MainMenu1: TMainMenu
    Left = 64
    Top = 48
    object miChanges: TMenuItem
      Caption = '&Changes'
      object miFaction: TMenuItem
        Caption = '&Faction'
        Checked = True
        OnClick = miFactionClick
      end
      object miItems: TMenuItem
        Caption = '&Items'
        Checked = True
        OnClick = miItemsClick
      end
      object miLocation: TMenuItem
        Caption = '&Location'
        Checked = True
        OnClick = miLocationClick
      end
    end
    object miAdvanced: TMenuItem
      Caption = '&Advanced'
      object miAdded: TMenuItem
        Caption = '&Added'
        OnClick = miAddedClick
      end
    end
  end
end
