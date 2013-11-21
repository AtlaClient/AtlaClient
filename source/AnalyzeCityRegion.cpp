//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <vector.h>
#include <map.h>
#include <algorithm>

#include "AnalyzeCityRegion.h"
#include "util.h"
#include "region.h"
#include "markets.h"
#include "eapfile.h"
#include "turn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TAnalyzeCityRegionFrm *AnalyzeCityRegionFrm;
static int LastBaseVer=1;
//---------------------------------------------------------------------------
enum TMarketType{
  mtUnknown=0,
  mtFood,            //(basepop,var/2)-(basepop+2000,var)
  mtNormal,          //(basepop+1000,0)-(basepop+4000,var)
  mtAdvanced,        //(basepop+2000,0)-(basepop+4000,var)
  mtMagic,           //(basepop+2000,0)-(basepop+4000,var)
  mtTrade,           //(basepop+2000,0)-(basepop+4000,var)
};
AnsiString MarketTypeStrs[]=
{
  "Unknown",
  "Food",
  "Normal",
  "Advanced",
  "Magic",
  "Trade",
};
int CalcMarketAmount(TMarketType mt,int basepop,int curpop,int var)
{
  int minpop,maxpop,minamt,maxamt;
  switch(mt)
  {
    case mtFood:
      minpop=basepop;maxpop=basepop+2000;minamt=var/2;maxamt=var;
    break;
    case mtNormal:
      minpop=basepop+1000;maxpop=basepop+4000;minamt=0;maxamt=var;
    break;
    case mtAdvanced:case mtMagic:case mtTrade:
      minpop=basepop+2000;maxpop=basepop+4000;minamt=0;maxamt=var;
    break;
    default:
      return 0;
  }
  int amount;//=0;
  if(curpop<=minpop)
    amount=minamt;
  else if(curpop>=maxpop)
    amount=maxamt;
  else
    amount=minamt+((maxamt-minamt)*(curpop-minpop))/(maxpop-minpop);
  return amount;
}

class TMarketInfo{
public:
  map<int,int> amounts; //index - turnnumber
  TMarketType mt;
  AnsiString item;
  int basepop;
  int maxamt;
  bool confirmed;
  TMarketInfo():mt(mtUnknown),basepop(0),maxamt(1),confirmed(false){}
  void UpdateType();
  int CalcAmount(int curpop);
  bool operator<(const TMarketInfo &b) const;
};
class TRegionMarketsInfo{
public:
  ALocation loc;
  vector<TMarketInfo> marketinfos;
  vector<int> turnnumbers;
  map<int,int> populations; //index - turnnumber
  int basepop;
  TRegionMarketsInfo():basepop(1){}
  AnsiString GetFileName();
  void Clear();
  void Read();
  void Write();
  void AddTurnNumber(int tn);
  TMarketInfo *GetMarketInfo(AnsiString item);
  void SortMarkets();
};
void TMarketInfo::UpdateType()
{
  if(mt!=mtUnknown)
    return;
  mt=mtUnknown;
  AItemType *itype=ItemTypes->Get(item,4);
  if(!itype)
    return;
  if(itype->type&AItemType::IT_MAGIC)
    mt=mtMagic;
  else if(itype->type&AItemType::IT_ADVANCED)
    mt=mtAdvanced;
  else if(itype->type&AItemType::IT_TRADE)
    mt=mtTrade;
  else if(itype->type&AItemType::IT_FOOD)
    mt=mtFood;
  else
    mt=mtNormal;
}
int TMarketInfo::CalcAmount(int curpop)
{
  return CalcMarketAmount(mt,basepop,curpop,maxamt);
}
bool TMarketInfo::operator<(const TMarketInfo &b) const
{
  if(mt<b.mt)
    return true;
  if(mt>b.mt)
    return false;
  return item<b.item;  
}
AnsiString TRegionMarketsInfo::GetFileName()
{
  AnsiString fname="_";
  fname+=loc.xloc;
  fname+=",";
  fname+=loc.yloc;
  if(!curRegionList)throw Exception("Error: curRegionList not defined");
  ARegionArray *ra=curRegionList->GetRegionArray(loc.zloc);
  if(!ra)throw Exception("Error: level not found");
  if(ra->name.Length()){
    fname+=",";
    fname+=ra->name;
  }
  return fname;
}
void TRegionMarketsInfo::Clear()
{
  marketinfos.clear();
  turnnumbers.clear();
  populations.clear();
  basepop=1;
}
void TRegionMarketsInfo::Read()
{
  Clear();
  AnsiString fname=GetFileName();
  TFile in;
  in.Open(GetGameFileName(fname));
  if(!in.Opened()){return;}
  int BaseVer;
  in>>BaseVer;
  if(BaseVer>LastBaseVer){
    AnsiString s="Error loading region markets. Unknown format";
    throw Exception(s);
  }
  in>>basepop;
  int turncount,var;
  in>>turncount;
  turnnumbers.resize(turncount);
  for(int i=0;i<turncount;i++)
  {
    in>>var;
    turnnumbers[i]=var;
    in>>var;
    populations[turnnumbers[i]]=var;
  }
  int marketcount;
  in>>marketcount;
  marketinfos.resize(marketcount);
  for(int j=0;j<marketcount;j++)
  {
    TMarketInfo &mi=marketinfos[j];
    in>>var;
    mi.mt=(TMarketType)var;
    in>>mi.item;
    in>>mi.basepop;
    in>>mi.maxamt;
    in>>var;
    mi.confirmed=var;
    for(int i=0;i<turncount;i++)
    {
      in>>var;
      mi.amounts[turnnumbers[i]]=var;
    }
  }
}
void TRegionMarketsInfo::Write()
{
  TFile out;
  AnsiString fName=GetFileName();
  out.Open(GetGameFileName(fName),true);
  int BaseVer=LastBaseVer;
  if(!out.Opened()){return;}
  out<<BaseVer;
  out<<basepop;
  int turncount=turnnumbers.size(),var;
  out<<turncount;
  for(int i=0;i<turncount;i++)
  {
    out<<turnnumbers[i];
    out<<populations[turnnumbers[i]];
  }
  int marketcount=marketinfos.size();
  out<<marketcount;
  for(int j=0;j<marketcount;j++)
  {
    TMarketInfo &mi=marketinfos[j];
    out<<(int)mi.mt;
    out<<mi.item;
    out<<mi.basepop;
    out<<mi.maxamt;
    out<<(int)mi.confirmed;
    for(int i=0;i<turncount;i++)
    {
      out<<mi.amounts[turnnumbers[i]];
    }
  }
}
//---------------------------------------------------------------------------
void TRegionMarketsInfo::AddTurnNumber(int tn)
{
  int *end=turnnumbers.end().operator ->();
  int *last=std::lower_bound(turnnumbers.begin().operator ->(),end,tn);
  if(last!=end)
    if(*last==tn)
      return;
  turnnumbers.insert(last,tn);
}
//---------------------------------------------------------------------------
TMarketInfo *TRegionMarketsInfo::GetMarketInfo(AnsiString item)
{
  for(TMarketInfo *mi=marketinfos.begin().operator ->(),*end=marketinfos.end().operator ->();mi<end;mi++)
  {
    if(mi->item==item)
      return mi;
  }
  return 0;
}
//---------------------------------------------------------------------------
void TRegionMarketsInfo::SortMarkets()
{
  stable_sort(marketinfos.begin(),marketinfos.end());
}
//---------------------------------------------------------------------------
TRegionMarketsInfo Info;
//---------------------------------------------------------------------------
__fastcall TAnalyzeCityRegionFrm::TAnalyzeCityRegionFrm(TComponent* Owner)
  : TForm(Owner)
{
  sgMarkets->Cells[0][0]="Item";
  sgMarkets->Cells[1][0]="Type";
  sgMarkets->Cells[2][0]="BasePop";
  sgMarkets->Cells[3][0]="MaxAmt";
  sgMarkets->Cells[4][0]="Commit";
  sgMarket->Cells[0][0]="Turn";
  sgMarket->Cells[0][1]="Pop";
  sgMarket->Cells[0][2]="Amount";
  sgMarket->Cells[0][3]="Calc Amount";
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::bnLoadClick(TObject *Sender)
{
  try{
    Info.Read();
  }catch(Exception&c)
  {
    ShowMessage(c.Message);
  }catch(...){}
  ShowInfo();
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::bnSaveClick(TObject *Sender)
{
  try{
    Info.Write();
  }catch(Exception&c)
  {
    ShowMessage(c.Message);
  }catch(...){}
}
//---------------------------------------------------------------------------
bool TAnalyzeCityRegionFrm::Init()
{
  Info.Clear();
  Info.loc=*curRegion;
  if(!curRegion->townname.Length())
    return false;
  bnLoadClick(0);
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(cbSaveOnClose->Checked)
    bnSaveClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::FormShow(TObject *Sender)
{
  if(!Init())
  {
    cbSaveOnClose->Checked=false;
    PostMessage(Handle,WM_CLOSE,0,0);
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::bnAddTurnClick(TObject *Sender)
{
  int tn=curTurnNumber;
  TMarketInfo *mi;
  bool needresort=false;
  foreach(curRegion->saleds)
  {
    AMarket *mark=*(AMarket**)iter;
    if(!(mark->type->type&AItemType::IT_TRADE))
      continue;
    mi=Info.GetMarketInfo(mark->type->abr);
    if(!mi)
    {
      TMarketInfo mi0;
      mi0.item=mark->type->abr;
      mi0.UpdateType();
      if(mi0.mt==mtUnknown)
        continue;
      mi0.basepop=Info.basepop;
      Info.marketinfos.push_back(mi0);
      mi=Info.GetMarketInfo(mark->type->abr);
      needresort=true;
    }
	if(!mi)
	  return;
    mi->amounts[tn]=mark->amount;
  }
  foreach(curRegion->wanteds)
  {
    AMarket *mark=*(AMarket**)iter;
    if(mark->type->type&AItemType::IT_TRADE)
      continue;
    mi=Info.GetMarketInfo(mark->type->abr);
    if(!mi)
    {
      TMarketInfo mi0;
      mi0.item=mark->type->abr;
      mi0.UpdateType();
      if(mi0.mt==mtUnknown)
        continue;
      mi0.basepop=Info.basepop;
      Info.marketinfos.push_back(mi0);
      mi=Info.GetMarketInfo(mark->type->abr);
      needresort=true;
    }
    if(!mi)
      return;
    mi->amounts[tn]=mark->amount;
  }

  Info.AddTurnNumber(tn);
  Info.populations[tn]=curRegion->population;
  if(needresort)
    Info.SortMarkets();

  ShowInfo();
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::edDefBasePopExit(TObject *Sender)
{
  Info.basepop=atoi(edDefBasePop->Text.c_str());
}
//---------------------------------------------------------------------------
void TAnalyzeCityRegionFrm::ShowInfo()
{
  labLocation->Caption=curRegion->FullName();
  edDefBasePop->Text=Info.basepop;
  int markCount=Info.marketinfos.size();
  AnsiString nullstr,confirmstr="+";
  if(!markCount)
  {
    sgMarkets->RowCount=2;
    for(int i=0;i<sgMarkets->ColCount;i++)
      sgMarkets->Cells[i][1]=nullstr;
    sgMarkets->Enabled=false;
    curItem.SetLength(0);
    ShowMarket();
    return;
  }
  sgMarkets->Enabled=true;
  sgMarkets->RowCount=markCount+1;
  for(int i=0;i<markCount;i++)
  {
    TMarketInfo &mi=Info.marketinfos[i];
    sgMarkets->Cells[0][i+1]=mi.item;
    sgMarkets->Cells[1][i+1]=MarketTypeStrs[mi.mt];
    sgMarkets->Cells[2][i+1]=mi.basepop;
    sgMarkets->Cells[3][i+1]=mi.maxamt;
    sgMarkets->Cells[4][i+1]=mi.confirmed?confirmstr:nullstr;
  }
  ShowMarket();
}
//---------------------------------------------------------------------------
void TAnalyzeCityRegionFrm::ShowMarket()
{
  TMarketInfo *mi=Info.GetMarketInfo(curItem);
  int count=0;
  if(mi)
  {
    count=Info.turnnumbers.size();
  }
  if(!count)
  {
    AnsiString nullstr;
    sgMarket->ColCount=2;
    for(int i=0;i<sgMarket->RowCount;i++)
      sgMarket->Cells[1][i]=nullstr;
    sgMarket->Enabled=false;
    labMarketItem->Caption=nullstr;
    edBasePop->Enabled=false;
    edBasePop->Text=nullstr;
    edMaxAmt->Enabled=false;
    edMaxAmt->Text=nullstr;
    cbConfirmed->Enabled=false;
    cbConfirmed->Checked=false;
    labError->Caption=nullstr;
    return;
  }
  sgMarket->Enabled=true;
  sgMarket->ColCount=count+1;
  int err=0;
  for(int i=0;i<count;i++)
  {
    int tn=Info.turnnumbers[i];
    int pop=Info.populations[tn];
    int amt1=mi->amounts[tn];
    int amt2=mi->CalcAmount(pop);
    err+=abs(amt1-amt2);
    sgMarket->Cells[i+1][0]=tn;
    sgMarket->Cells[i+1][1]=pop;
    sgMarket->Cells[i+1][2]=amt1;
    sgMarket->Cells[i+1][3]=amt2;
  }
  labMarketItem->Caption=mi->item+" ("+MarketTypeStrs[mi->mt]+")";
  edBasePop->Enabled=true;
  edBasePop->Text=mi->basepop;
  edMaxAmt->Enabled=true;
  edMaxAmt->Text=mi->maxamt;
  cbConfirmed->Enabled=true;
  cbConfirmed->Checked=mi->confirmed;
  labError->Caption=err;
  labError->Color=err?clRed:clGreen;
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::sgMarketsDblClick(TObject *Sender)
{
  int ind=sgMarkets->Row-1;
  if(unsigned(ind)>=Info.marketinfos.size())
    return;
  TMarketInfo &mi=Info.marketinfos[ind];
  curItem=mi.item;
  ShowMarket();
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::edBasePopExit(TObject *Sender)
{
  TMarketInfo *mi=Info.GetMarketInfo(curItem);
  if(!mi) return;
  int bp=atoi(edBasePop->Text.c_str());
  if(bp==mi->basepop) return;
  mi->basepop=bp;
  ShowMarket();
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::edMaxAmtExit(TObject *Sender)
{
  TMarketInfo *mi=Info.GetMarketInfo(curItem);
  if(!mi) return;
  int ma=atoi(edMaxAmt->Text.c_str());
  if(ma==mi->maxamt) return;
  mi->maxamt=ma;
  ShowMarket();
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::edBasePopKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==13)
  {
    Key=0;
    edBasePopExit(Sender);
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::edMaxAmtKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==13)
  {
    Key=0;
    edMaxAmtExit(Sender);
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeCityRegionFrm::cbConfirmedClick(TObject *Sender)
{
  TMarketInfo *mi=Info.GetMarketInfo(curItem);
  if(!mi) return;
  mi->confirmed=cbConfirmed->Checked;
}
//---------------------------------------------------------------------------
bool FillConfirmedMarketMaxAmounts(const ALocation &loc,map<AnsiString,int> &amounts, int &basepop)
{
  try{
    TRegionMarketsInfo Info;
    Info.loc=loc;
    Info.Read();
    basepop=Info.basepop;
	for(TMarketInfo *mi=Info.marketinfos.begin().operator ->(),*end=Info.marketinfos.end().operator ->();mi<end;mi++)
    {
      if(!mi->confirmed) continue;
      if(!mi->maxamt) continue;
      amounts[mi->item]=mi->maxamt;
    }
  }catch(...){
    amounts.clear();
    basepop=0;
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
