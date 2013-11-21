object ViewUnitsSetupForm: TViewUnitsSetupForm
  Left = 86
  Top = 169
  BorderStyle = bsDialog
  Caption = 'View Units Setup'
  ClientHeight = 358
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 40
    Height = 13
    Caption = 'Columns'
  end
  object Label2: TLabel
    Left = 160
    Top = 8
    Width = 107
    Height = 13
    Caption = 'Items/Skills for Column'
  end
  object Label4: TLabel
    Left = 496
    Top = 8
    Width = 74
    Height = 13
    Caption = 'Stored Columns'
  end
  object Label5: TLabel
    Left = 648
    Top = 8
    Width = 90
    Height = 13
    Caption = 'Stored ColumnSets'
  end
  object lbColumns: TListBox
    Left = 8
    Top = 24
    Width = 137
    Height = 265
    Hint = 'DlbClick to Rename'
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnClick = lbColumnsClick
    OnDblClick = lbColumnsDblClick
  end
  object bnOk: TButton
    Left = 248
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 1
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 336
    Top = 328
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object lbItems: TListBox
    Left = 160
    Top = 24
    Width = 137
    Height = 297
    ItemHeight = 13
    Sorted = True
    TabOrder = 3
    OnDblClick = bnDelItemClick
  end
  object lbAvailItems: TListBox
    Left = 336
    Top = 24
    Width = 137
    Height = 297
    ItemHeight = 13
    Sorted = True
    TabOrder = 4
    OnDblClick = bnAddItemClick
  end
  object bnAddItem: TButton
    Left = 304
    Top = 64
    Width = 25
    Height = 25
    Caption = '<'
    TabOrder = 5
    OnClick = bnAddItemClick
  end
  object bnDelItem: TButton
    Left = 304
    Top = 112
    Width = 25
    Height = 25
    Caption = '>'
    TabOrder = 6
    OnClick = bnDelItemClick
  end
  object bnAddColumn: TButton
    Left = 8
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Add'
    TabOrder = 7
    OnClick = bnAddColumnClick
  end
  object bnDelColumn: TButton
    Left = 8
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Delete'
    TabOrder = 8
    OnClick = bnDelColumnClick
  end
  object bnMoveUp: TButton
    Left = 80
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Move Up'
    TabOrder = 9
    OnClick = bnMoveUpClick
  end
  object bnMoveDown: TButton
    Left = 80
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Move Down'
    TabOrder = 10
    OnClick = bnMoveDownClick
  end
  object bnClearItems: TButton
    Left = 160
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Clear Items'
    TabOrder = 11
    OnClick = bnClearItemsClick
  end
  object lbStoredColumns: TListBox
    Left = 496
    Top = 24
    Width = 137
    Height = 265
    ItemHeight = 13
    TabOrder = 12
    OnDblClick = bnColsLoadClick
  end
  object lbStoredSets: TListBox
    Left = 648
    Top = 24
    Width = 137
    Height = 265
    ItemHeight = 13
    TabOrder = 13
    OnDblClick = bnSetsLoadClick
  end
  object bnColsSave: TButton
    Left = 496
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Save'
    TabOrder = 14
    OnClick = bnColsSaveClick
  end
  object bnColsLoad: TButton
    Left = 568
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Load'
    TabOrder = 15
    OnClick = bnColsLoadClick
  end
  object bnColsRename: TButton
    Left = 496
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Rename'
    TabOrder = 16
    OnClick = bnColsRenameClick
  end
  object bnColsDelete: TButton
    Left = 568
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Delete'
    TabOrder = 17
    OnClick = bnColsDeleteClick
  end
  object bnSetsSave: TButton
    Left = 648
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Save'
    TabOrder = 18
    OnClick = bnSetsSaveClick
  end
  object bnSetsLoad: TButton
    Left = 720
    Top = 296
    Width = 65
    Height = 25
    Caption = 'Load'
    TabOrder = 19
    OnClick = bnSetsLoadClick
  end
  object bnSetsDelete: TButton
    Left = 720
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Delete'
    TabOrder = 20
    OnClick = bnSetsDeleteClick
  end
  object bnSetsRename: TButton
    Left = 648
    Top = 328
    Width = 65
    Height = 25
    Caption = 'Rename'
    TabOrder = 21
    OnClick = bnSetsRenameClick
  end
  object tabItems: TTabControl
    Left = 336
    Top = 0
    Width = 137
    Height = 25
    MultiLine = True
    TabOrder = 22
    Tabs.Strings = (
      'Items'
      'Skills')
    TabIndex = 0
    OnChange = tabItemsChange
    OnChanging = tabItemsChanging
  end
end
