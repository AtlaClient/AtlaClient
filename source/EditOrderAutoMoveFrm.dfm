object EditOrderAutoMoveForm: TEditOrderAutoMoveForm
  Left = 353
  Top = 183
  BorderStyle = bsToolWindow
  Caption = 'Edit AutoMove Orders'
  ClientHeight = 304
  ClientWidth = 463
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object labMess: TLabel
    Left = 8
    Top = 248
    Width = 449
    Height = 17
    AutoSize = False
    Caption = 'labMes'
  end
  object bnClose: TButton
    Left = 384
    Top = 272
    Width = 75
    Height = 25
    Caption = 'Close'
    ModalResult = 2
    TabOrder = 0
  end
  object lbOrders: TListBox
    Left = 8
    Top = 8
    Width = 449
    Height = 153
    ItemHeight = 13
    TabOrder = 1
    OnClick = lbOrdersClick
    OnDblClick = bnEditClick
  end
  object Panel1: TPanel
    Left = 8
    Top = 168
    Width = 449
    Height = 73
    BevelOuter = bvLowered
    TabOrder = 2
    object labBegPos: TLabel
      Left = 112
      Top = 8
      Width = 48
      Height = 17
      AutoSize = False
      Caption = 'Begin Pos'
      Layout = tlBottom
    end
    object labReg: TLabel
      Left = 112
      Top = 32
      Width = 329
      Height = 33
      AutoSize = False
      Caption = 'labReg'
      WordWrap = True
    end
    object cbCurrent: TCheckBox
      Left = 16
      Top = 8
      Width = 65
      Height = 17
      Caption = 'C&urrent'
      TabOrder = 0
    end
    object cbCommented: TCheckBox
      Left = 16
      Top = 32
      Width = 97
      Height = 17
      Caption = '&Commented'
      TabOrder = 1
      OnClick = cbCommentedClick
    end
    object edX: TEdit
      Left = 168
      Top = 8
      Width = 41
      Height = 21
      TabOrder = 2
    end
    object edY: TEdit
      Left = 216
      Top = 8
      Width = 41
      Height = 21
      TabOrder = 3
    end
    object bnApply: TButton
      Left = 400
      Top = 8
      Width = 43
      Height = 25
      Caption = 'Apply'
      Default = True
      TabOrder = 4
      OnClick = bnApplyClick
    end
    object bnGetPos: TButton
      Left = 312
      Top = 8
      Width = 33
      Height = 25
      Caption = 'Get'
      TabOrder = 5
      OnClick = bnGetPosClick
    end
    object edZ: TEdit
      Left = 264
      Top = 8
      Width = 41
      Height = 21
      TabOrder = 6
    end
  end
  object bnEdit: TButton
    Left = 56
    Top = 272
    Width = 49
    Height = 25
    Caption = '&Edit'
    TabOrder = 3
    OnClick = bnEditClick
  end
  object cbSail: TCheckBox
    Left = 8
    Top = 272
    Width = 41
    Height = 25
    Caption = '&Sail'
    TabOrder = 4
    OnClick = cbSailClick
  end
  object bnAdd: TButton
    Left = 120
    Top = 272
    Width = 49
    Height = 25
    Caption = '&Add'
    TabOrder = 5
    OnClick = bnAddClick
  end
  object bnDelete: TButton
    Left = 184
    Top = 272
    Width = 49
    Height = 25
    Caption = 'Delete'
    TabOrder = 6
    OnClick = bnDeleteClick
  end
  object bnMoveUp: TButton
    Left = 248
    Top = 272
    Width = 49
    Height = 25
    Caption = '&Up'
    TabOrder = 7
    OnClick = bnMoveUpClick
  end
  object bnDown: TButton
    Left = 312
    Top = 272
    Width = 49
    Height = 25
    Caption = '&Down'
    TabOrder = 8
    OnClick = bnDownClick
  end
end
