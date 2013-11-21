object RegInfoForm: TRegInfoForm
  Left = 255
  Top = 107
  Hint = 'Use Right button to move map window'
  BorderStyle = bsNone
  Caption = 'Region Info'
  ClientHeight = 200
  ClientWidth = 335
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBtnText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  ShowHint = True
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 0
    Top = 0
    Width = 335
    Height = 200
    Hint = 'Use Right button to move window'
    Align = alClient
    BorderStyle = bsNone
    Color = clBtnFace
    HideSelection = False
    Lines.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '20')
    ReadOnly = True
    TabOrder = 0
    WordWrap = False
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    ExplicitWidth = 225
    ExplicitHeight = 161
  end
end
