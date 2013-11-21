object WedgeModeSetupForm: TWedgeModeSetupForm
  Left = 276
  Top = 97
  BorderStyle = bsDialog
  Caption = 'Wedge Mode Settings'
  ClientHeight = 205
  ClientWidth = 691
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 280
    Top = 24
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 280
    Top = 48
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 280
    Top = 72
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label4: TLabel
    Left = 280
    Top = 96
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label5: TLabel
    Left = 280
    Top = 120
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label6: TLabel
    Left = 280
    Top = 144
    Width = 30
    Height = 17
    AutoSize = False
    Caption = '1'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    Layout = tlBottom
  end
  object Label11: TLabel
    Left = 3
    Top = 27
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object Label12: TLabel
    Left = 3
    Top = 51
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object Label13: TLabel
    Left = 3
    Top = 75
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object Label14: TLabel
    Left = 3
    Top = 99
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object Label15: TLabel
    Left = 3
    Top = 123
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object Label16: TLabel
    Left = 3
    Top = 147
    Width = 16
    Height = 16
    AutoSize = False
    Color = clBlack
    ParentColor = False
  end
  object LabelReg: TLabel
    Left = 8
    Top = 4
    Width = 297
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = 'LabelReg'
  end
  object Bevel1: TBevel
    Left = 352
    Top = 0
    Width = 9
    Height = 209
    Shape = bsLeftLine
  end
  object Edit1: TEdit
    Left = 24
    Top = 24
    Width = 250
    Height = 21
    TabOrder = 0
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object bnOk: TButton
    Left = 8
    Top = 176
    Width = 41
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 6
    OnClick = bnOkClick
  end
  object Edit2: TEdit
    Tag = 1
    Left = 24
    Top = 48
    Width = 250
    Height = 21
    TabOrder = 1
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object Edit3: TEdit
    Tag = 2
    Left = 24
    Top = 72
    Width = 250
    Height = 21
    TabOrder = 2
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object Edit4: TEdit
    Tag = 3
    Left = 24
    Top = 96
    Width = 250
    Height = 21
    TabOrder = 3
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object Edit5: TEdit
    Tag = 4
    Left = 24
    Top = 120
    Width = 250
    Height = 21
    TabOrder = 4
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object Edit6: TEdit
    Tag = 5
    Left = 24
    Top = 144
    Width = 250
    Height = 21
    TabOrder = 5
    OnEnter = EditEnter
    OnExit = EditExit
    OnKeyPress = EditKeyPress
  end
  object bnCancel: TButton
    Left = 64
    Top = 176
    Width = 49
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
  end
  object bnExpr: TButton
    Left = 192
    Top = 176
    Width = 75
    Height = 25
    Caption = '&Expressions'
    TabOrder = 8
    OnClick = bnExprClick
  end
  object bnVerify: TButton
    Left = 128
    Top = 176
    Width = 49
    Height = 25
    Caption = 'Verify'
    Default = True
    TabOrder = 9
    OnClick = bnVerifyClick
  end
  object bnSet1: TButton
    Left = 312
    Top = 26
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 10
    OnClick = bnSetClick
  end
  object bnGet1: TButton
    Left = 328
    Top = 26
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 11
    OnClick = bnGetClick
  end
  object bnSet2: TButton
    Tag = 1
    Left = 312
    Top = 50
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 12
    OnClick = bnSetClick
  end
  object bnGet2: TButton
    Tag = 1
    Left = 328
    Top = 50
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 13
    OnClick = bnGetClick
  end
  object bnSet3: TButton
    Tag = 2
    Left = 312
    Top = 74
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 14
    OnClick = bnSetClick
  end
  object bnGet3: TButton
    Tag = 2
    Left = 328
    Top = 74
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 15
    OnClick = bnGetClick
  end
  object bnSet4: TButton
    Tag = 3
    Left = 312
    Top = 98
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 16
    OnClick = bnSetClick
  end
  object bnGet4: TButton
    Tag = 3
    Left = 328
    Top = 98
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 17
    OnClick = bnGetClick
  end
  object bnSet5: TButton
    Tag = 4
    Left = 312
    Top = 122
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 18
    OnClick = bnSetClick
  end
  object bnGet5: TButton
    Tag = 4
    Left = 328
    Top = 122
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 19
    OnClick = bnGetClick
  end
  object bnSet6: TButton
    Tag = 5
    Left = 312
    Top = 146
    Width = 17
    Height = 17
    Caption = '<'
    TabOrder = 20
    OnClick = bnSetClick
  end
  object bnGet6: TButton
    Tag = 5
    Left = 328
    Top = 146
    Width = 17
    Height = 17
    Caption = '>'
    TabOrder = 21
    OnClick = bnGetClick
  end
  object Grid: TStringGrid
    Left = 360
    Top = 0
    Width = 329
    Height = 172
    ColCount = 2
    DefaultRowHeight = 23
    FixedCols = 0
    RowCount = 8
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goThumbTracking]
    TabOrder = 22
    ColWidths = (
      166
      140)
  end
  object bnAdd: TButton
    Left = 584
    Top = 176
    Width = 51
    Height = 25
    Caption = 'Add'
    TabOrder = 23
    OnClick = bnAddClick
  end
  object bnDel: TButton
    Left = 640
    Top = 176
    Width = 49
    Height = 25
    Caption = 'Delete'
    TabOrder = 24
    OnClick = bnDelClick
  end
  object bnLoad: TButton
    Left = 440
    Top = 176
    Width = 51
    Height = 25
    Caption = 'Load'
    TabOrder = 25
    OnClick = bnLoadClick
  end
  object Save: TButton
    Left = 376
    Top = 176
    Width = 49
    Height = 25
    Caption = 'Save'
    TabOrder = 26
    OnClick = SaveClick
  end
  object bnInsert: TButton
    Left = 528
    Top = 176
    Width = 49
    Height = 25
    Caption = 'Insert'
    TabOrder = 27
    OnClick = bnInsertClick
  end
  object PopupMenu: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 272
    Top = 176
    object N1: TMenuItem
      Caption = '=='
      OnClick = ExprClick
    end
    object N2: TMenuItem
      Caption = '!='
      OnClick = ExprClick
    end
    object N3: TMenuItem
      Caption = '>'
      OnClick = ExprClick
    end
    object N4: TMenuItem
      Caption = '<'
      OnClick = ExprClick
    end
    object N5: TMenuItem
      Caption = '!'
      OnClick = ExprClick
    end
    object N6: TMenuItem
      Caption = '&&&&'
      OnClick = ExprClick
    end
    object N7: TMenuItem
      Caption = '||'
      OnClick = ExprClick
    end
  end
  object pmLoad: TPopupMenu
    Left = 496
    Top = 176
  end
end
