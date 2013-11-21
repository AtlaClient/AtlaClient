object RegionListForm: TRegionListForm
  Left = 484
  Top = 219
  BorderStyle = bsSizeToolWin
  Caption = 'Region List'
  ClientHeight = 276
  ClientWidth = 217
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object List: TListBox
    Left = 0
    Top = 0
    Width = 217
    Height = 276
    Align = alClient
    ItemHeight = 13
    PopupMenu = PopupMenu
    TabOrder = 0
    OnDblClick = ListDblClick
  end
  object PopupMenu: TPopupMenu
    Left = 96
    Top = 64
    object miSaveList: TMenuItem
      Caption = '&Save List'
      OnClick = miSaveListClick
    end
    object miLoadList: TMenuItem
      Caption = '&Load List'
      OnClick = miLoadListClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object miAddCurrent: TMenuItem
      Caption = 'Add current'
      ShortCut = 45
      OnClick = miAddCurrentClick
    end
    object miAddByExpression: TMenuItem
      Caption = 'Add by &Expression'
      OnClick = miAddByExpressionClick
    end
    object miDelete: TMenuItem
      Caption = 'Delete'
      ShortCut = 46
      OnClick = miDeleteClick
    end
    object miClearList: TMenuItem
      Caption = 'Clear List'
      OnClick = miClearListClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object miSaveMap: TMenuItem
      Caption = 'Save Map'
      OnClick = miSaveMapClick
    end
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 24
    Top = 8
  end
  object SaveDialog: TSaveDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 64
    Top = 8
  end
end
