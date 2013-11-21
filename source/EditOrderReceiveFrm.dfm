object EditOrderReceiveForm: TEditOrderReceiveForm
  Left = 253
  Top = 328
  BorderStyle = bsSizeToolWin
  Caption = 'Add RECEIVE order'
  ClientHeight = 131
  ClientWidth = 364
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnPaint = FormPaint
  DesignSize = (
    364
    131)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 12
    Width = 40
    Height = 13
    Caption = 'Receive'
    Layout = tlBottom
  end
  object Label2: TLabel
    Left = 16
    Top = 36
    Width = 20
    Height = 13
    Caption = 'from'
    Layout = tlBottom
  end
  object Label3: TLabel
    Left = 8
    Top = 56
    Width = 44
    Height = 13
    Caption = 'Capacity:'
  end
  object labCapacity: TLabel
    Left = 64
    Top = 56
    Width = 55
    Height = 13
    Caption = 'labCapacity'
  end
  object lbNeed: TLabel
    Left = 8
    Top = 76
    Width = 34
    Height = 13
    Caption = 'lbNeed'
  end
  object bnOk: TButton
    Left = 88
    Top = 100
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = bnOkClick
  end
  object bnCancel: TButton
    Left = 280
    Top = 100
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
    OnClick = bnCancelClick
  end
  object cbTargets: TComboBox
    Left = 56
    Top = 32
    Width = 307
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    ItemHeight = 13
    TabOrder = 1
    OnChange = cbTargetsChange
  end
  object cbItems: TComboBox
    Left = 136
    Top = 8
    Width = 227
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = cbItemsChange
  end
  object edCount: TEdit
    Left = 56
    Top = 8
    Width = 53
    Height = 21
    TabOrder = 2
    Text = '0'
  end
  object bnAll: TButton
    Left = 112
    Top = 8
    Width = 22
    Height = 21
    Caption = '&All'
    TabOrder = 5
    TabStop = False
    OnClick = bnAllClick
  end
  object bnAdd: TButton
    Left = 176
    Top = 100
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'A&dd'
    TabOrder = 6
    OnClick = bnOkClick
  end
  object cbRepeating: TCheckBox
    Left = 7
    Top = 102
    Width = 71
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = '&Repeating'
    TabOrder = 7
  end
end
