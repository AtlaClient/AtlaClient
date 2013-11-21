object EventsForm: TEventsForm
  Left = 679
  Top = 22
  Caption = 'Events'
  ClientHeight = 127
  ClientWidth = 467
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShortCut = FormShortCut
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 0
    Top = 0
    Width = 467
    Height = 127
    Align = alClient
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    OnDblClick = MemoDblClick
  end
end
