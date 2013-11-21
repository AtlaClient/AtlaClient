object EditOrderMovingForm: TEditOrderMovingForm
  Left = 439
  Top = 143
  BorderStyle = bsToolWindow
  Caption = 'Edit Moving Order'
  ClientHeight = 374
  ClientWidth = 432
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  DesignSize = (
    432
    374)
  PixelsPerInch = 96
  TextHeight = 13
  object labLoc: TLabel
    Left = 8
    Top = 277
    Width = 417
    Height = 39
    Anchors = [akLeft, akBottom]
    AutoSize = False
    Layout = tlCenter
    WordWrap = True
  end
  object Label1: TLabel
    Left = 104
    Top = 8
    Width = 321
    Height = 17
    Alignment = taCenter
    AutoSize = False
    Caption = 'Available moves'
  end
  object Label2: TLabel
    Left = 8
    Top = 248
    Width = 57
    Height = 17
    AutoSize = False
    Caption = 'Custom'
    Layout = tlBottom
  end
  object bnOk: TButton
    Left = 49
    Top = 345
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 7
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 201
    Top = 345
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 8
    OnClick = bnCancelClick
  end
  object cbCommented: TCheckBox
    Left = 186
    Top = 322
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Commented'
    TabOrder = 1
    OnClick = cbCommentedClick
  end
  object cbRepeating: TCheckBox
    Left = 42
    Top = 322
    Width = 97
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 0
  end
  object lbDirs: TListBox
    Left = 8
    Top = 8
    Width = 89
    Height = 201
    ItemHeight = 13
    TabOrder = 2
    OnDblClick = bnDeleteClick
  end
  object bnDelete: TButton
    Left = 8
    Top = 216
    Width = 65
    Height = 25
    Caption = 'Delete Last'
    TabOrder = 3
    OnClick = bnDeleteClick
  end
  object lbAvDirs: TListBox
    Left = 104
    Top = 24
    Width = 321
    Height = 185
    Hint = 'Press 1,2,3,7,8,9 to move, BackSpace to Delete Last'
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    OnDblClick = lbAvDirsDblClick
  end
  object edCustom: TEdit
    Left = 64
    Top = 248
    Width = 361
    Height = 21
    TabOrder = 5
  end
  object cbType: TComboBox
    Left = 80
    Top = 216
    Width = 81
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 6
    OnChange = cbTypeChange
    Items.Strings = (
      'MOVE'
      'ADVANCE'
      'SAIL')
  end
  object cbUpdateMap: TCheckBox
    Left = 320
    Top = 322
    Width = 97
    Height = 17
    Caption = 'Update Map'
    Checked = True
    State = cbChecked
    TabOrder = 9
  end
  object ActionList1: TActionList
    Left = 224
    Top = 216
    object Action1: TAction
      Caption = 'Action1'
      Enabled = False
      ShortCut = 112
      OnExecute = Action1Execute
    end
  end
end
