//---------------------------------------------------------------------------
#ifndef importH
#define importH
//---------------------------------------------------------------------------
bool ImportAtlaCart(AnsiString fName);
bool ImportCrystalBall(AnsiString fName);

class AUnit;
AnsiString ExportMageStr(AUnit *un);
bool ExportMages(AnsiString fName);
bool ImportMages(AnsiString fName);
#endif
