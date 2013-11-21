//---------------------------------------------------------------------------
#ifndef exprlangH
#define exprlangH
//---------------------------------------------------------------------------
#include <vector.h>
enum ExprModes{
  emRegion=0x0001,
  emUnit=0x0002,

  emBarBreak=0x1000,
};

enum EDataTypes{
 dtInt=0,
 dtInts,
 dtString,


 dtMaxDT
};
typedef vector<int> ints;
class EData{
public:
    EDataTypes type;
    EData();
    virtual ~EData();
    virtual AnsiString GetDataTypeName();
    virtual int IsEqual(EData * b);
    virtual int IsNotEqual(EData* b);
    virtual int IsLess(EData* b);
    virtual int IsGreat(EData * b);
    virtual int IsNot();
    virtual int IsAnd(EData * b);
    virtual int IsOr(EData* b);
    virtual int GetValue();
    virtual EData * CreateCopy();
};
class EDataInt:public EData{
public:
    int value;

    EDataInt();
    virtual ~EDataInt();
    virtual AnsiString GetDataTypeName();
    virtual int IsEqual(EData * b);
    virtual int IsNotEqual(EData* b);
    virtual int IsLess(EData* b);
    virtual int IsGreat(EData * b);
    virtual int IsNot();
    virtual int IsAnd(EData * b);
    virtual int IsOr(EData* b);
    virtual int GetValue();
    virtual EData* CreateCopy();
};
class EDataInts:public EData{
public:
    ints values;

    EDataInts();
    virtual ~EDataInts();
    virtual AnsiString GetDataTypeName();
    virtual int IsEqual(EData * b);
    virtual int IsNotEqual(EData* b);
    virtual int IsLess(EData* b);
    virtual int IsGreat(EData * b);
    virtual int GetValue();
    virtual EData * CreateCopy();

    bool Contains(int val);

};
class EDataString:public EData{
public:
    AnsiString value;

    EDataString();
    virtual ~EDataString();
    virtual int IsEqual(EData* b);
    virtual int IsNotEqual(EData* b);
    virtual EData* CreateCopy();
};

class ARegion;
class AUnit;
void ExprSetRegionMode(ARegion *r);
void ExprSetUnitMode(AUnit *u);
int ExprProcessExpr(AnsiString expr);

class EVariable;
typedef void (*initfunc)(EVariable &var);
class EVariable{
public:
    AnsiString name;
    EData *data;
    initfunc Init;
    int mode;
    EVariable(AnsiString n, initfunc in, int m):
     name(n),data(0),Init(in),mode(m){}
    void ClearData(){delete data;data=0;}
};
extern EVariable variables[];
extern int VarsCount;

typedef EData* (*funcfunc)(char*&);
class EFunction{
public:
    AnsiString name,params;
    funcfunc func;
    int mode;
    EFunction(AnsiString n,AnsiString p, funcfunc f,int m):
     name(n),params(p),func(f),mode(m){}
};
extern EFunction functions[];
extern int FuncsCount;

void ExprCreateMenu(TMenuItem * root,int exprmode,TNotifyEvent OnClick);

#endif
