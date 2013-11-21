//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <ctype.h>
#include "exprlang.h"
#include "util.h"
#include "unit.h"
#include "turn.h"
#include "faction.h"
#include "region.h"
#include "markets.h"
#include "items.h"
#include "orders.h"
#include "acplugins.h"
#include "gameparams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
static AnsiString ErrorExprModesStr(int mode)
{
  if(mode==0)
    return " can't be used.";
  AnsiString str=" can be used only in";
  bool first=true;
  if(mode&emRegion)
  {
    str+=" Wedges";
    first=false;
  }
  if(mode&emUnit)
  {
    if(!first)
      str+=" and";
    str+=" AutoWarning order";
  }
  str+=".";
  return str;
}

static ARegion *reg=0;
static AUnit *un=0;
static ExprModes exprmode=ExprModes(0);

void initregfactions(EVariable &var);
void initlocalfactions(EVariable &var);
void initlocalobservation(EVariable &var);
void initregioninfoage(EVariable &var);
void initgatenumber(EVariable &var);
void initconsumetype(EVariable &var);
void initreginfo(EVariable &var);
void initmens(EVariable &var);
void initturnnumber(EVariable &var);
void initcurreginfo(EVariable &var);
void initattitudesfactions(EVariable &var);
void initpresentunits(EVariable &var);
void initunitfaction(EVariable &var);
void initobjectweight(EVariable &var);
void initobjectcapacity(EVariable &var);

EVariable variables[]={
 EVariable("presentfaction",initregfactions,emRegion|emUnit),
 EVariable("guardfaction",initregfactions,emRegion|emUnit),
 EVariable("localfaction",initlocalfactions,emRegion|emUnit),
 EVariable("localobservation",initlocalobservation,emRegion|emUnit),
 EVariable("regioninfoage",initregioninfoage,emRegion|emUnit),
 EVariable("gatenumber",initgatenumber,emRegion|emUnit),
 EVariable("consumetype",initconsumetype,emRegion|emUnit),
 EVariable("level",0,emRegion|emUnit),//¹7
 EVariable("population",initreginfo,emRegion|emUnit),//¹8
 EVariable("money",initreginfo,emRegion|emUnit),
 EVariable("wages",initreginfo,emRegion|emUnit),
 EVariable("maxwages",initreginfo,emRegion|emUnit),
 EVariable("entertain",initreginfo,emRegion|emUnit),//¹12
 EVariable("mens",initmens,emUnit),
 EVariable("turnnumber",initturnnumber,emRegion|emUnit|emBarBreak),
 EVariable("xloc",initreginfo,emRegion|emUnit),//¹15
 EVariable("yloc",initreginfo,emRegion|emUnit),//¹16
 EVariable("zloc",initreginfo,emRegion|emUnit),//¹17
 EVariable("curxloc",initcurreginfo,emRegion|emUnit),//¹18
 EVariable("curyloc",initcurreginfo,emRegion|emUnit),//¹19
 EVariable("curzloc",initcurreginfo,emRegion|emUnit),//¹20
 EVariable("hostilefaction",initattitudesfactions,emRegion|emUnit),//¹21
 EVariable("unfriendlyfaction",initattitudesfactions,emRegion|emUnit),//¹22
 EVariable("neutralfaction",initattitudesfactions,emRegion|emUnit),//¹23
 EVariable("friendlyfaction",initattitudesfactions,emRegion|emUnit),//¹24
 EVariable("allyfaction",initattitudesfactions,emRegion|emUnit),//¹25
 EVariable("presentunits",initpresentunits,emRegion|emUnit),//¹26
 EVariable("unitfaction",initunitfaction,emUnit),//¹27
 EVariable("objectweight",initobjectweight,emUnit),//¹28
 EVariable("objectcapacity",initobjectcapacity,emUnit),//¹29
};
int VarsCount=sizeof(variables)/sizeof(variables[0]);

EData* productamount(char*&buf);
EData* presentproduct(char*&buf);
EData* canpresentadvproduct(char*&buf);
EData* saleamount(char*&buf);
EData* saleprice(char*&buf);
EData* wantedamount(char*&buf);
EData* wantedprice(char*&buf);
EData* localobservation(char*&buf);
EData* presentitem(char*&buf);
EData* presentnonlocalitem(char*&buf);
EData* presentlocalitem(char*&buf);
EData* RND(char*&buf);
EData* attemptedwages(char*&buf);
EData* attemptedtaxes(char*&buf);
EData* attemptedentertains(char*&buf);
EData* hastraders(char*&buf);
EData* PresentInUserInfo(char*&buf);
EData* PresentInRegionDescription(char*&buf);
EData* skilllevel(char*&buf);
EData* skilldays(char*&buf);
EData* itemcount(char*&buf);
EData* unitswithskill(char*&buf);
EData* distance(char*&buf);
EData* hascastorder(char*&buf);
EData* flagvalue(char*&buf);
EData* presentobject(char*&buf);

EFunction functions[]={
 EFunction("productamount","(\"\")",productamount,emRegion|emUnit|emBarBreak),
 EFunction("presentproduct","(\"\")",presentproduct,emRegion|emUnit),
 EFunction("canpresentadvproduct","(\"\")",canpresentadvproduct,emRegion|emUnit),
 EFunction("saleamount","(\"\")",saleamount,emRegion|emUnit),
 EFunction("saleprice","(\"\")",saleprice,emRegion|emUnit),
 EFunction("wantedamount","(\"\")",wantedamount,emRegion|emUnit),
 EFunction("wantedprice","(\"\")",wantedprice,emRegion|emUnit),
 EFunction("presentitem","(\"\")",presentitem,emRegion|emUnit),
 EFunction("presentlocalitem","(\"\")",presentlocalitem,emRegion|emUnit),
 EFunction("presentnonlocalitem","(\"\")",presentnonlocalitem,emRegion|emUnit),
 EFunction("RND","()",RND,emRegion|emUnit),
 EFunction("attemptedwages","()",attemptedwages,emRegion|emUnit),
 EFunction("attemptedtaxes","()",attemptedtaxes,emRegion|emUnit),
 EFunction("attemptedentertains","()",attemptedentertains,emRegion|emUnit),
 EFunction("hastraders","()",hastraders,emRegion|emUnit),
 EFunction("PresentInUserInfo","(\"\")",PresentInUserInfo,emRegion|emUnit),
 EFunction("PresentInRegionDescription","(\"\")",PresentInRegionDescription,emRegion|emUnit),
 EFunction("skilllevel","(\"\")",skilllevel,emUnit),
 EFunction("skilldays","(\"\")",skilldays,emUnit),
 EFunction("itemcount","(\"\")",itemcount,emUnit),
 EFunction("UnitsWithSkill","(\"\",1)",unitswithskill,emRegion|emUnit),
 EFunction("distance","(,,1)",distance,emRegion|emUnit),
 EFunction("hascastorder","(\"\")",hascastorder,emUnit),
 EFunction("flagvalue","(\"\")",flagvalue,emUnit),
 EFunction("PresentObject","(\"\")",presentobject,emRegion|emUnit),
};
int FuncsCount=sizeof(functions)/sizeof(functions[0]);

int FindMas(ints &mas, int val){
 for(unsigned i=0;i<mas.size();i++){
  if(mas[i]==val) return i;
 }
 return -1;
}
void AddMas(ints &mas, int val){
 if(FindMas(mas,val)!=-1) return;
 mas.push_back(val);
}
static const char*endtoken=" \n,.():;[]&|>=<!{}+-*/?";
AnsiString GetNextExprToken(char*&buf){
 char*s,*old=buf;
 if(*buf==0) return "";
 if(*buf=='\"'){
  old++;
  s=buf+1;
  while(*s!='\"')s++;
  s++;
  buf=s;
  return AnsiString(old,s-old-1);
 }else{
  s=buf+1;
  while(*s&&strchr(endtoken,*s)==0)s++;
  buf=s;
  return AnsiString(old,s-old);
 }
}

EData::EData(){
}
EData::~EData(){
}
AnsiString EData::GetDataTypeName()
{
 throw Exception("Data::GetDataTypeName");
}
int EData::IsEqual(EData * b)
{
 throw Exception("Data::IsEqual");
}
int EData::IsNotEqual(EData* b)
{
 throw Exception("Data::IsNotEqual");
}
int EData::IsLess(EData* b)
{
 throw Exception("Data::IsLess");
}
int EData::IsGreat(EData * b)
{
 throw Exception("Data::IsGreat");
}
int EData::IsNot()
{
 throw Exception("Data::IsNot");
}
int EData::IsAnd(EData* b)
{
 throw Exception("Data::IsAnd");
}
int EData::IsOr(EData * b)
{
 throw Exception("Data::IsOr");
}
int EData::GetValue()
{
 throw Exception("Data::GetValue");
}
EData * EData::CreateCopy()
{
 throw Exception("Data::CreateCopy");
}
EDataInt::EDataInt()
 :value(0)
{
 type=dtInt;
}
EDataInt::~EDataInt()
{
}
AnsiString EDataInt::GetDataTypeName()
{
 return "Int";
}
int EDataInt::IsEqual(EData * b)
{
 if(b->type!=dtInt)
  throw Exception("Int == "+b->GetDataTypeName());
 return value==b->GetValue();
}
int EDataInt::IsNotEqual(EData* b)
{
 if(b->type!=dtInt)
  throw Exception("Int != "+b->GetDataTypeName());
 return value!=b->GetValue();
}
int EDataInt::IsLess(EData* b)
{
 if(b->type!=dtInt)
  throw Exception("Int < "+b->GetDataTypeName());
 return value<b->GetValue();
}
int EDataInt::IsGreat(EData * b)
{
 if(b->type!=dtInt)
  throw Exception("Int > "+b->GetDataTypeName());
 return value>b->GetValue();
}
int EDataInt::IsNot()
{
 return !value;
}
int EDataInt::IsAnd(EData* b)
{
 if(b->type!=dtInt)
  throw Exception("Int && "+b->GetDataTypeName());
 return value && b->GetValue();
}
int EDataInt::IsOr(EData * b)
{
 if(b->type!=dtInt)
  throw Exception("Int || "+b->GetDataTypeName());
 return value || b->GetValue();
}
int EDataInt::GetValue()
{
 return value;
}
EData* EDataInt::CreateCopy()
{
 EDataInt *n=new EDataInt;
 n->value=value;
 return n;
}

EDataInts::EDataInts()
{
 type=dtInts;
}
EDataInts::~EDataInts()
{
}
AnsiString EDataInts::GetDataTypeName()
{
 return "Ints";
}
int EDataInts::IsEqual(EData * b)
{
 if(b->type==dtInts){
  EDataInts *d=dynamic_cast<EDataInts*>(b);
  if(!d)
   throw Exception("Ints == "+b->GetDataTypeName()+" bad conversion");
  for(unsigned i=0;i<d->values.size();i++){
   if(FindMas(values,d->values[i])>=0) return true;
  }
  return false;
 }
 if(b->type!=dtInt)
  throw Exception("Ints == "+b->GetDataTypeName());
 return FindMas(values,b->GetValue())!=-1;
}
int EDataInts::IsNotEqual(EData* b)
{
 if(b->type==dtInts){
  return !IsEqual(b);
 }
 if(b->type!=dtInt)
  throw Exception("Ints != "+b->GetDataTypeName());
 return FindMas(values,b->GetValue())==-1;
}
int EDataInts::IsLess(EData* b)
{
 if(b->type==dtInts)
  return b->IsGreat(this);
 throw Exception("DataInts::IsLess "+b->GetDataTypeName());
}
int EDataInts::IsGreat(EData * b)
{
 if(b->type!=dtInts)
  throw Exception("Ints > "+b->GetDataTypeName());
 EDataInts *d=dynamic_cast<EDataInts*>(b);
 if(!d)
  throw Exception("Ints > "+b->GetDataTypeName()+" bad conversion");
 for(unsigned i=0;i<d->values.size();i++){
  if(FindMas(values,d->values[i])<0) return false;
 }
 return true;
}
int EDataInts::GetValue()
{
 throw Exception("DataInts::GetValue");
}
EData * EDataInts::CreateCopy()
{
 EDataInts *n=new EDataInts;
 n->values=values;
 return n;
}
bool EDataInts::Contains(int val){
 for(int *i=values.begin().operator ->(),*endi=values.end().operator ->();i<endi;i++){
  if(*i==val)
   return true;
 }
 return false;
}
EDataString::EDataString()
{
 type=dtString;
}
EDataString::~EDataString()
{
}
int EDataString::IsEqual(EData* b)
{
 if(b->type!=dtString)
  throw Exception("String == "+b->GetDataTypeName());
 EDataString *d=dynamic_cast<EDataString*>(b);
 if(!d)
  throw Exception("String == String Converion error");
 return value.AnsiCompareIC(d->value)==0;
}
int EDataString::IsNotEqual(EData* b)
{
 if(b->type!=dtString)
  throw Exception("String != "+b->GetDataTypeName());
 EDataString *d=dynamic_cast<EDataString*>(b);
 if(!d)
  throw Exception("String != String Converion error");
 return value.AnsiCompareIC(d->value)!=0;
}
EData* EDataString::CreateCopy()
{
 EDataString *n=new EDataString;
 n->value=value;
 return n;
}

EData *ExprLast(char *&buf);
EData *Expr1(char*&buf){
 AnsiString str;
 while(*buf==' ') buf++;
 switch(*buf){
  case 0:
    throw Exception("Not ended expression");
  case '0':case '1':case '2':case '3':case '4':
  case '5':case '6':case '7':case '8':case '9':{
   EDataInt *expr=new EDataInt;
   expr->value=atoi(buf);
   GetNextExprToken(buf);
   return expr;
  }
  case '\"':{
   EDataString *e=new EDataString;
   e->value=GetNextExprToken(buf);
   return e;
  }
  case '!':{
   buf++;
   EData *e=Expr1(buf);
   if(!e)
    throw Exception("\'Not\' error");
   EDataInt *expr=new EDataInt;
   expr->value=e->IsNot();
   delete e;
   return expr;
  }
  case '-':{
   buf++;
   EData *e=Expr1(buf);
   if(!e)
    throw Exception("Unary \'-\' error");
   if(e->type!=dtInt)
    throw Exception("Unary \'-\' error: not int");
   EDataInt *expr=new EDataInt;
   expr->value=-e->GetValue();
   delete e;
   return expr;
  }

  case '(':{
   buf++;
   EData *e=ExprLast(buf);
   if(!e)
    throw Exception("Brackes error");
   EDataInt *expr=new EDataInt;
   if(*buf!=')')
    throw Exception("Brackes error: can't find another brakes");
   buf++;
   expr->value=e->GetValue();
   delete e;
   return expr;
  }
  case '{':{
   buf++;
   EData *e=ExprLast(buf);
   if(!e)
    throw Exception("{ error");
   if(e->type!=dtInt)
    throw Exception("{ error: bad type "+e->GetDataTypeName());
   EDataInts *mas=new EDataInts;
   AddMas(mas->values,e->GetValue());
   EDataTypes type=e->type;
   delete e;
   while(*buf!='}'){
    if(*buf!=',')
     throw Exception("{ error: } or , not found");
    buf++;
    e=ExprLast(buf);
    if(e->type!=type)
     throw Exception("{ error: type changing");
    AddMas(mas->values,e->GetValue());
    delete e;
   }
   buf++;
   return mas;
  }
  default:{
   str=GetNextExprToken(buf);
   int i;
   if(*buf!='('){
    for(i=0;i<VarsCount;i++){
     if(variables[i].name.AnsiCompareIC(str)==0) break;
    }
    if(i!=VarsCount){
     if((variables[i].mode&exprmode)==0)
       throw Exception("Variable "+str+ErrorExprModesStr(variables[i].mode));
     EData *e1=variables[i].data;
     if(!e1&&variables[i].Init){
      variables[i].Init(variables[i]);
      e1=variables[i].data;
     }
     if(!e1)
      throw Exception("Variable "+str+" can't initialing");
     return e1->CreateCopy();
    }else
     throw Exception("Unknown variable "+str);
   }
   buf++;
   for(i=0;i<FuncsCount;i++){
    if(functions[i].name.AnsiCompareIC(str)==0) break;
   }
   if(i==FuncsCount)
   {
    EData *e=Plugins.RunExpression(str,exprmode,buf);
    if(!e)
      throw Exception("Unknown function "+str);
    if(*buf!=')')
     throw Exception("Error call plugin function "+str);
    buf++;
    return e;
   }
   if((functions[i].mode&exprmode)==0)
     throw Exception("Function "+str+ErrorExprModesStr(functions[i].mode));
   EData *e=functions[i].func(buf);
   if(*buf!=')'||e==0)
    throw Exception("Error call function "+str);
   buf++;
   return e;
  }
 }
// return 0;
}
EData *Expr1_1(char*&buf){
 EData *left=Expr1(buf);
 char c;
 while((c=*buf)=='*'||c=='/'){
  buf++;
  if(left->type!=dtInt)
   throw Exception("\'*\' or '/\' error: left not int");
  EData *right=Expr1(buf);
  if(right->type!=dtInt)
   throw Exception("\'*\' or '/\' error: right not int");
  int val=((EDataInt*)right)->value;
  delete right;
  switch(c){
   case '*':
    ((EDataInt*)left)->value*=val;
   break;
   case '/':
    if(!val)
     throw Exception("\'/\' error: divide by zero");
    ((EDataInt*)left)->value/=val;
   break;
  }
 }
 return left;
}
EData *Expr1_2(char*&buf){
 EData *left=Expr1_1(buf);
 char c;
 while((c=*buf)=='+'||c=='-'){
  buf++;
  if(left->type!=dtInt)
   throw Exception("\'-\' or '+\' error: left not int");
  EData *right=Expr1_1(buf);
  if(right->type!=dtInt)
   throw Exception("\'-\' or '+\' error: right not int");
  int val=((EDataInt*)right)->value;
  delete right;
  switch(c){
   case '+':
    ((EDataInt*)left)->value+=val;
   break;
   case '-':
    ((EDataInt*)left)->value-=val;
   break;
  }
 }
 return left;
}
EData *Expr2(char*&buf){
 EData *left=Expr1_2(buf);
 int usl=0;
 if(buf[0]=='='&&buf[1]=='='){
  buf+=2;
  usl=1;
 }else if(buf[0]=='!'&&buf[1]=='='){
  buf+=2;
  usl=2;
 }else if(buf[0]=='>'){
  buf++;
  if(buf[0]=='='){
   buf++;
   usl=5;
  }else
   usl=3;
 }else if(buf[0]=='<'){
  buf++;
  if(buf[0]=='='){
   buf++;
   usl=6;
  }else
   usl=4;
 }
 if(usl==0) return left;
 EData *right=Expr1_2(buf);
 EDataInt *expr=new EDataInt;
 switch(usl){
  case 1:
   expr->value=left->IsEqual(right);
  break;
  case 2:
   expr->value=left->IsNotEqual(right);
  break;
  case 3:
   expr->value=left->IsGreat(right);
  break;
  case 4:
   expr->value=left->IsLess(right);
  break;
  case 5:
   expr->value=!left->IsLess(right);
  break;
  case 6:
   expr->value=!left->IsGreat(right);
  break;
 }
 delete left;
 delete right;
 return expr;
}
EData *Expr3(char*&buf){
 EData *left=Expr2(buf);
 if(buf[0]!='&'||buf[1]!='&'){
  return left;
 }
 EDataInt *expr=new EDataInt;
 expr->value=left->GetValue();
 delete left;
 while(buf[0]=='&'&&buf[1]=='&'){
  buf+=2;
  EData *right=Expr2(buf);
  expr->value=expr->IsAnd(right);
  delete right;
 }
 return expr;
}
EData *Expr4(char*&buf){
 EData *left=Expr3(buf);
 if(buf[0]!='|'||buf[1]!='|'){
  return left;
 }
 EDataInt *expr=new EDataInt;
 expr->value=left->GetValue();
 delete left;
 while(buf[0]=='|'&&buf[1]=='|'){
  buf+=2;
  EData *right=Expr3(buf);
  expr->value=expr->IsOr(right);
  delete right;
 }
 return expr;
}
EData *Expr5(char*&buf){
 EData *left=Expr4(buf);
 if(*buf!='?')
  return left;
 buf++;
 bool first=left->GetValue();
 delete left;
 EData *e1=Expr4(buf);
 if(*buf!=':')
   throw Exception("?: error : not found");
 buf++;
 EData *e2=Expr4(buf);
 if(first){
   delete e2;
   return e1;
 }else{
   delete e1;
   return e2;
 }
}

EData *ExprLast(char *&buf){
 return Expr5(buf);
}
void ClearVariables()
{
  for(int i=0;i<VarsCount;i++){
    variables[i].ClearData();
  }
}
void ExprSetRegionMode(ARegion *r){
  exprmode=emRegion;
  PluginRegion=reg=r;
  PluginUnit=un=0;
  ClearVariables();
}
void ExprSetUnitMode(AUnit *u){
  exprmode=emUnit;
  PluginUnit=un=u;
  if(u) reg=u->baseobj->basereg; else reg=0;
  PluginRegion=reg;
  ClearVariables();
}
int ExprProcessExpr(AnsiString expr){
 bool instring=false;
 for(int i=1;i<=expr.Length();i++){
  if(expr[i]=='\"')
   instring=!instring;
  if(instring) continue;
  if(expr[i]!=' ') continue;
  expr.Delete(i,1);
  i--;
 }
 char *s=expr.c_str();
 if(!*s) return false;
 EData *e=ExprLast(s);
 if(!e)
  throw Exception("ProcessExpr Error");
 if(*s)
  throw Exception("ProcessExpr Error: Bad end");
 int ret=e->GetValue();
 delete e;
 return ret;
}

void initregfactions(EVariable &var){
 if(!reg) return;
 EDataInts *presfac=new EDataInts;
 EDataInts *guardfac=new EDataInts;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   int facnum=/*0;
   if(un->faction) facnum=*/un->faction->number;
   AddMas(presfac->values,facnum);
   if(un->guard==GUARD_GUARD) AddMas(guardfac->values,facnum);
  }
 }
 variables[0].data=presfac;
 variables[1].data=guardfac;
}
void initlocalfactions(EVariable &var){
 if(!reg) return;
 EDataInts *locfac=new EDataInts;
 AFactions *facs=reg->turn->factions;
 foreach(facs){
  AFaction *fac=*(AFaction**)iter;
  if(!fac->local) continue;
  AddMas(locfac->values,fac->number);
 }
 var.data=locfac;
}
void initlocalobservation(EVariable &var){
 if(!reg) return;
 EDataInt *locobs=new EDataInt;
 int maxobs=0,obs;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
//   if(!un->faction) continue;
   if(!un->faction->local) continue;
   obs=un->GetBegObservation();
   if(obs>maxobs) maxobs=obs;
  }
 }
 locobs->value=maxobs;
 var.data=locobs;
}
void initregioninfoage(EVariable &var){
 if(!reg) return;
 EDataInt *regia=new EDataInt;
 if(reg->hasInfo)
  regia->value=curTurnNumber-reg->oldinfoturnnum;
 else
  regia->value=-1;
 var.data=regia;
}
void initgatenumber(EVariable &var){
 if(!reg) return;
 EDataInt *gate=new EDataInt;
 gate->value=reg->gate;
 var.data=gate;
}
void initconsumetype(EVariable &var){
 if(!reg) return;
 int ct=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!un->faction->local) continue;
   if(un->endflags&FLAG_CONSUMING_UNIT) ct|=1;
   if(un->endflags&FLAG_CONSUMING_FACTION) ct|=2;
  }
 }

 EDataInt *ctype=new EDataInt;
 ctype->value=ct;
 var.data=ctype;
}
void initreginfo(EVariable &var){
 if(!reg) return;
 EDataInt *data;
 data=new EDataInt;
 data->value=reg->population;
 variables[8].data=data;
 data=new EDataInt;
 data->value=reg->money ;
 variables[9].data=data;
 data=new EDataInt;
 data->value=reg->wages;
 variables[10].data=data;
 data=new EDataInt;
 data->value=reg->maxwages;
 variables[11].data=data;
 data=new EDataInt;
 data->value=reg->entertainment;
 variables[12].data=data;
 data=new EDataInt;
 data->value=reg->xloc;
 variables[15].data=data;
 data=new EDataInt;
 data->value=reg->yloc;
 variables[16].data=data;
 data=new EDataInt;
 data->value=reg->zloc;
 variables[17].data=data;
}
//---------------------------------------------------------------------------
void initmens(EVariable &var)
{
 if(!un) return;
 EDataInt *e=new EDataInt;
 e->value=un->GetBegMen();
 var.data=e;
}
//---------------------------------------------------------------------------
void initturnnumber(EVariable &var)
{
 EDataInt *e=new EDataInt;
 e->value=curTurn->num;
 var.data=e;
}
//---------------------------------------------------------------------------
void initcurreginfo(EVariable &var){
 if(!curRegion) return;
 EDataInt *data;
 data=new EDataInt;
 data->value=curRegion->xloc;
 variables[18].data=data;
 data=new EDataInt;
 data->value=curRegion->yloc;
 variables[19].data=data;
 data=new EDataInt;
 data->value=curRegion->zloc;
 variables[20].data=data;
}
//---------------------------------------------------------------------------
void initattitudesfactions(EVariable &var){
 EDataInts *facs[5];
 for(int i=0;i<5;i++)
  facs[i]=new EDataInts;
 if(curFaction&&curFaction->local)
 {
  foreach(curFaction){
   Attitude *att=*(Attitude**)iter;
   AddMas(facs[att->attitude]->values,att->facnum);
  }
 }
 for(int i=0;i<5;i++)
  variables[i+21].data=facs[i];
}
//---------------------------------------------------------------------------
void initpresentunits(EVariable &var){
 EDataInts *uns=new EDataInts;
 foreach(reg)
 {
   AObject *obj=*(AObject**)iter;
   foreach(obj->units)
   {
     AUnit *un=*(AUnit**)iter;
     AddMas(uns->values,un->num);
   }
 }
 var.data=uns;
}
//---------------------------------------------------------------------------
void initunitfaction(EVariable &var){
  EDataInt *e=new EDataInt;
  e->value=un->faction->number;
  var.data=e;
}
//---------------------------------------------------------------------------
void initobjectweight(EVariable &var){
  int cap2=0;
  AObject *baseobj=un->endobj;
  ARegion *basereg=baseobj->basereg;
  foreach(basereg)
  {
    AObject *obj=*(AObject**)iter;
    foreach(obj->units){
      AUnit *un=*(AUnit**)iter;
      if(un->endobj!=baseobj) continue;
      cap2+=un->weight;
    }
  }
  EDataInt *e=new EDataInt;
  e->value=cap2;
  var.data=e;
}
//---------------------------------------------------------------------------
void initobjectcapacity(EVariable &var){
  EDataInt *e=new EDataInt;
  e->value=un->endobj->Capacity();
  var.data=e;
}
//---------------------------------------------------------------------------
EData* productamount(char*&buf){
 if(*buf!='\"')
  throw Exception("ProductAmount: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("ProductAmount: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg->products){
  AMarket *prod=*(AMarket**)iter;
  if(prod->type->abr!=str) continue;
  e->value=prod->amount;
  break;
 }
 return e;
}
EData* presentproduct(char*&buf){
 if(*buf!='\"')
  throw Exception("PresentProduct: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("PresentProduct: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg->products){
  AMarket *prod=*(AMarket**)iter;
  if(prod->type->abr!=str) continue;
  e->value=1;
  break;
 }
 return e;
}
EData* canpresentadvproduct(char*&buf){
 if(*buf!='\"')
  throw Exception("CanPresentAdvProduct: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("CanPresentAdvProduct: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 ATerrainType *ttype=TerrainTypes->Get(reg->type);
 if(ttype)
 {
   for(int i=0;i<ttype->advlist->Count;i++)
   {
     if(ttype->advlist->Strings[i]!=str) continue;
     e->value=1;
     break;
   }
 }
 return e;
}
EData* saleamount(char*&buf){
 if(*buf!='\"')
  throw Exception("saleamount: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("saleamount: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg->saleds){
  AMarket *mark=*(AMarket**)iter;
  if(mark->type->abr!=str) continue;
  e->value=mark->amount;
  break;
 }
 return e;
}
EData* saleprice(char*&buf){
 if(*buf!='\"')
  throw Exception("saleprice: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("saleprice: "+str+" is not item");
 str=itype->abr;
 e->value=-1;
 foreach(reg->saleds){
  AMarket *mark=*(AMarket**)iter;
  if(mark->type->abr!=str) continue;
  e->value=mark->price;
  break;
 }
 return e;
}
EData* wantedamount(char*&buf){
 if(*buf!='\"')
  throw Exception("wantedamount: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("wantedamount: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg->wanteds){
  AMarket *mark=*(AMarket**)iter;
  if(mark->type->abr!=str) continue;
  e->value=mark->amount;
  break;
 }
 return e;
}
EData* wantedprice(char*&buf){
 if(*buf!='\"')
  throw Exception("wantedprice: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("wantedprice: "+str+" is not item");
 str=itype->abr;
 e->value=-1;
 foreach(reg->wanteds){
  AMarket *mark=*(AMarket**)iter;
  if(mark->type->abr!=str) continue;
  e->value=mark->price;
  break;
 }
 return e;
}
EData* presentitem(char*&buf){
 if(*buf!='\"')
  throw Exception("presentitem: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("presentitem: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   e->value+=un->items->GetNum(str);
  }
 }
 return e;
}
EData* presentlocalitem(char*&buf){
 if(*buf!='\"')
  throw Exception("presentlocalitem: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("presentlocalitem: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!un->faction->local) continue;
   e->value+=un->items->GetNum(str);
  }
 }
 return e;
}
EData* presentnonlocalitem(char*&buf){
 if(*buf!='\"')
  throw Exception("presentnonlocalitem: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AItemType *itype=ItemTypes->Get(str);
 if(!itype)
  throw Exception("presentnonlocalitem: "+str+" is not item");
 str=itype->abr;
 e->value=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(un->faction->local) continue;
   e->value+=un->items->GetNum(str);
  }
 }
 return e;
}
EData* RND(char*&buf){
 if(*buf==')')
  throw Exception("rnd: int needed");
 EData *e=ExprLast(buf);
 if(e->type!=dtInt)
  throw Exception("rnd: int needed");
 EDataInt *res=(EDataInt*) e;
 res->value=getrandom(res->value);
 return res;
}
EData* attemptedwages(char*&buf){
 if(*buf==')')
  throw Exception("attemptedwages: ints needed");
 EData *e=ExprLast(buf);
 if(e->type!=dtInts)
  throw Exception("attemptedwages: ints needed");
 EDataInts *_e=(EDataInts*)e;
 int count=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!_e->Contains(un->endfaction->number)) continue;
   AOrder *ord=un->orders->Find(O_WORK,2);
   if(!ord)continue;
   count+=un->GetMen();
  }
 }
 count*=reg->wages;
 delete e;
 EDataInt *res=new EDataInt;
 res->value=count;
 return res;
}
EData* attemptedtaxes(char*&buf){
 if(*buf==')')
  throw Exception("attemptedtaxes: ints needed");
 EData *e=ExprLast(buf);
 if(e->type!=dtInts)
  throw Exception("attemptedtaxes: ints needed");
 EDataInts *_e=(EDataInts*)e;
 int count=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!_e->Contains(un->endfaction->number)) continue;
   if(!un->HasTaxOrder()) continue;
   count+=TaxEffect*un->Taxers();
  }
 }
 delete e;
 EDataInt *res=new EDataInt;
 res->value=count;
 return res;
}
EData* attemptedentertains(char*&buf){
 if(*buf==')')
  throw Exception("attemptedentertains: ints needed");
 EData *e=ExprLast(buf);
 if(e->type!=dtInts)
  throw Exception("attemptedentertains: ints needed");
 EDataInts *_e=(EDataInts*)e;
 int count=0;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!_e->Contains(un->endfaction->number)) continue;
   AOrder *ord=un->orders->Find(O_ENTERTAIN,2);
   if(!ord)continue;
   count+=EntertainEffect*un->GetSkill(EnteSkill)*un->GetMen();
  }
 }
 delete e;
 EDataInt *res=new EDataInt;
 res->value=count;
 return res;
}
EData* hastraders(char*&buf){
 if(*buf==')')
  throw Exception("hastraders: ints needed");
 EData *e=ExprLast(buf);
 if(e->type!=dtInts)
  throw Exception("hastraders: ints needed");
 EDataInts *_e=(EDataInts*)e;
 bool found=false;
 foreach(reg){
  AObject *obj=*(AObject**)iter;
  foreach(obj->units){
   AUnit *un=*(AUnit**)iter;
   if(!_e->Contains(un->endfaction->number)) continue;
   if(!un->HasTradeOrder()) continue;
   found=true;
   break;
  }
  if(found)
   break;
 }
 delete e;
 EDataInt *res=new EDataInt;
 res->value=found;
 return res;
}
EData* PresentInUserInfo(char*&buf){
 if(*buf!='\"')
  throw Exception("PresentInUserInfo: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AnsiString ui=reg->turn->RegUserInfos[reg];
 e->value=ui.Pos(str)!=0;
 return e;
}
//---------------------------------------------------------------------------
EData* PresentInRegionDescription(char*&buf){
 if(*buf!='\"')
  throw Exception("PresentInRegionDescription: string needed");
 AnsiString str=GetNextExprToken(buf);
 EDataInt *e=new EDataInt;
 AnsiString ui=reg->descr;
 e->value=ui.Pos(str)!=0;
 return e;
}
//---------------------------------------------------------------------------
EData* skilllevel(char*&buf)
{
  if(*buf!='\"')
    throw Exception("skilllevel: string needed");
  AnsiString str=GetNextExprToken(buf);
  ASkillType *stype=SkillTypes->Get(str);
  if(!stype)
    throw Exception("skilllevel: "+str+" is not skill");
  str=stype->abr;
  EDataInt *e=new EDataInt;
  e->value=un->GetBegSkill(str);
  return e;
}
//---------------------------------------------------------------------------
EData* skilldays(char*&buf)
{
  if(*buf!='\"')
    throw Exception("skilldays: string needed");
  AnsiString str=GetNextExprToken(buf);
  ASkillType *stype=SkillTypes->Get(str);
  if(!stype)
    throw Exception("skilldays: "+str+" is not skill");
  str=stype->abr;
  EDataInt *e=new EDataInt;
  e->value=un->GetBegSkillDays(str);
  return e;
}
//---------------------------------------------------------------------------
EData* itemcount(char*&buf)
{
  if(*buf!='\"')
    throw Exception("itemcount: string needed");
  AnsiString str=GetNextExprToken(buf);
  AItemType *itype=ItemTypes->Get(str);
  if(!itype)
    throw Exception("itemcount: "+str+" is not item");
  str=itype->abr;
  EDataInt *e=new EDataInt;
  e->value=un->items->GetNum(str);
  return e;
}
//---------------------------------------------------------------------------
EData* unitswithskill(char*&buf){
  if(*buf!='\"')
   throw Exception("unitswithskill: string needed");
  AnsiString str=GetNextExprToken(buf);
  ASkillType *stype=SkillTypes->Get(str);
  if(!stype)
    throw Exception("unitswithskill: "+str+" is not skill");
  str=stype->abr;
  if(*buf!=',')
    throw Exception("unitswithskill: ,level is not found");
  buf++;
  EData *_e=ExprLast(buf);
  if(_e->type!=dtInt)
    throw Exception("unitswithskill: level must be integer");
  EDataInt *e=(EDataInt*)_e;
  int lev=e->value;
  e->value=0;
  foreach(reg){
    AObject *obj=*(AObject**)iter;
    foreach(obj->units){
      AUnit *un=*(AUnit**)iter;
      if(!un->faction->local) continue;
      if(un->GetBegSkill(str)<lev) continue;
      e->value+=un->GetBegMen();
    }
  }
  return e;
}
//---------------------------------------------------------------------------
EData* distance(char*&buf){
  int xloc,yloc,zloc;
  EData *_e=ExprLast(buf);
  if(_e->type!=dtInt)
    throw Exception("distance: xloc must be integer");
  xloc=((EDataInt*)_e)->value;
  delete _e;
  if(*buf!=',')
    throw Exception("distance: ,yloc is not found");
  buf++;
  _e=ExprLast(buf);
  if(_e->type!=dtInt)
    throw Exception("distance: yloc must be integer");
  yloc=((EDataInt*)_e)->value;
  delete _e;
  if(*buf!=',')
    throw Exception("distance: ,zloc is not found");
  buf++;
  _e=ExprLast(buf);
  if(_e->type!=dtInt)
    throw Exception("distance: zloc must be integer");
  zloc=((EDataInt*)_e)->value;
  delete _e;
  EDataInt *e=new EDataInt;
  e->value=reg->GetDistance(xloc,yloc,zloc);
  return e;
}
//---------------------------------------------------------------------------
EData* hascastorder(char*&buf){
 bool has=false;
 AnsiString skill;
 if(*buf=='\"')
   skill=GetNextExprToken(buf);
 AOrderCast *ord=dynamic_cast<AOrderCast*>(un->orders->Find(O_CAST,2));
 if(ord){
   if(!skill.Length())
     has=true;
   else{
     ASkillType *stype=SkillTypes->Get(skill);
     if(!stype||!(stype->flags&ASkillType::CAST))
       throw Exception("hascastorder: "+skill+" is not castable skill");
     if(ord->abr==stype->abr)
       has=true;
   }
 }
 EDataInt *e=new EDataInt;
 e->value=has;
 return e;
}
//---------------------------------------------------------------------------
EData* flagvalue(char*&buf)
{
  int val=0;
  AnsiString flagstr;
  if(*buf!='\"')
    throw Exception("flagvalue: flag name needed");
  flagstr=GetNextExprToken(buf).UpperCase();
  if(flagstr=="AVOID")
    val=un->endguard==GUARD_AVOID;
  else if(flagstr=="GUARD")
    val=un->endguard==GUARD_GUARD;
  else if(flagstr=="REVEAL")
    val=un->endreveal;
  else if(flagstr=="CONSUME")
    val=(un->endflags&FLAG_CONSUMING_FACTION?2:un->endflags&FLAG_CONSUMING_UNIT?1:0);
  else if(flagstr=="BEHIND")
    val=un->endflags&FLAG_BEHIND;
  else if(flagstr=="NOCROSS")
    val=un->endflags&FLAG_NOCROSS_WATER;
  else if(flagstr=="AUTOTAX")
    val=un->endflags&FLAG_AUTOTAX;
  else if(flagstr=="HOLD")
    val=un->endflags&FLAG_HOLDING;
  else if(flagstr=="NOAID")
    val=un->endflags&FLAG_NOAID;
  else if(flagstr=="SHARING")
    val=un->endflags&FLAG_SHARING;
  else if(flagstr=="SPOILS")
  {
    if(un->endflags&FLAG_NOSPOILS)
      val=0;
    else if(un->endflags&FLAG_FLYSPOILS)
      val=1;
    else if(un->endflags&FLAG_RIDESPOILS)
      val=2;
    else if(un->endflags&FLAG_WALKSPOILS)
      val=3;
    else
      val=4;
  }else
    throw Exception("flagvalue: unknown flag name "+flagstr);
  EDataInt *e=new EDataInt;
  e->value=val;
  return e;
}
//---------------------------------------------------------------------------
EData* presentobject(char*&buf){
  int count=0;
  AnsiString type;
  if(*buf!='\"')
    throw Exception("presentobject: object typename needed");
  type=GetNextExprToken(buf);
  foreach(reg)
  {
    AObject *obj=*(AObject**)iter;
    if(obj->type.AnsiCompareIC(type)) continue;
    count++;
  }
  EDataInt *e=new EDataInt;
  e->value=count;
  return e;
}
//---------------------------------------------------------------------------
void ExprCreateMenu(TMenuItem * root,int exprmode,TNotifyEvent OnClick)
{
 TMenuItem *mi;
 for(int i=0;i<VarsCount;i++){
  if(!variables[i].Init) continue;
  if((variables[i].mode&exprmode)==0) continue;
  mi=new TMenuItem(root);
  mi->Caption=variables[i].name;
  mi->OnClick=OnClick;
  if(variables[i].mode&emBarBreak)
   mi->Break=mbBarBreak;
  root->Add(mi);
 }
 for(int i=0;i<FuncsCount;i++){
  if((functions[i].mode&exprmode)==0) continue;
  mi=new TMenuItem(root);
  mi->Caption=functions[i].name+functions[i].params;
  mi->OnClick=OnClick;
  if(functions[i].mode&emBarBreak)
   mi->Break=mbBarBreak;
  root->Add(mi);
 }
 Plugins.CreateFunctionMenu(root,exprmode,OnClick);
}
//---------------------------------------------------------------------------

