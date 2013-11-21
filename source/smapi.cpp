//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "smapi.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
HANDLE hInst;
HINSTANCE hlibMAPI = 0;

LPMAPILOGON lpfnMAPILogon = NULL;
LPMAPILOGOFF lpfnMAPILogoff = NULL;
LPMAPISENDMAIL lpfnMAPISendMail = NULL;
LPMAPISENDDOCUMENTS lpfnMAPISendDocuments = NULL;
LPMAPIFINDNEXT lpfnMAPIFindNext = NULL;
LPMAPIREADMAIL lpfnMAPIReadMail = NULL;
LPMAPISAVEMAIL lpfnMAPISaveMail = NULL;
LPMAPIDELETEMAIL lpfnMAPIDeleteMail = NULL;
LPMAPIFREEBUFFER lpfnMAPIFreeBuffer = NULL;
LPMAPIADDRESS lpfnMAPIAddress = NULL;
LPMAPIDETAILS lpfnMAPIDetails = NULL;
LPMAPIRESOLVENAME lpfnMAPIResolveName = NULL;

#ifdef _WIN32
#define szMAPIDLL       "MAPI32.DLL"
#else
#define szMAPIDLL       "MAPI.DLL"
#endif

/*
 -  InitSimpleMAPI
 -
 *  Purpose:
 *      Loads the DLL containing the simple MAPI functions and sets
 *      up a pointer to each. Wrappers for the  function pointers
 *      are declared in SMAPI.H.
 *
 *  Returns:
 *      TRUE if sucessful, else FALSE
 *
 *  Side effects:
 *      Loads a DLL and sets up function pointers
 */
BOOL InitSimpleMAPI (void)
{
    UINT fuError;

    fuError = SetErrorMode(SEM_NOOPENFILEERRORBOX);
    hlibMAPI = LoadLibrary(szMAPIDLL);
    SetErrorMode(fuError);

#ifdef _WIN32
    if (!hlibMAPI)
#else
    if (hlibMAPI < 32)
#endif
    return (FALSE);

    if ((lpfnMAPILogon = (LPMAPILOGON) GetProcAddress (hlibMAPI, "MAPILogon"))==0)
    return (FALSE);
    if ((lpfnMAPILogoff = (LPMAPILOGOFF) GetProcAddress (hlibMAPI, "MAPILogoff"))==0)
    return (FALSE);
    if ((lpfnMAPISendMail = (LPMAPISENDMAIL) GetProcAddress (hlibMAPI, "MAPISendMail"))==0)
    return (FALSE);
    if ((lpfnMAPISendDocuments = (LPMAPISENDDOCUMENTS) GetProcAddress (hlibMAPI, "MAPISendDocuments"))==0)
    return (FALSE);
    if ((lpfnMAPIFindNext = (LPMAPIFINDNEXT) GetProcAddress (hlibMAPI, "MAPIFindNext"))==0)
    return (FALSE);
    if ((lpfnMAPIReadMail = (LPMAPIREADMAIL) GetProcAddress (hlibMAPI, "MAPIReadMail"))==0)
    return (FALSE);
    if ((lpfnMAPISaveMail = (LPMAPISAVEMAIL) GetProcAddress (hlibMAPI, "MAPISaveMail"))==0)
    return (FALSE);
    if ((lpfnMAPIDeleteMail = (LPMAPIDELETEMAIL) GetProcAddress (hlibMAPI, "MAPIDeleteMail"))==0)
    return (FALSE);
    if ((lpfnMAPIFreeBuffer = (LPMAPIFREEBUFFER) GetProcAddress (hlibMAPI, "MAPIFreeBuffer"))==0)
    return (FALSE);
    if ((lpfnMAPIAddress = (LPMAPIADDRESS) GetProcAddress (hlibMAPI, "MAPIAddress"))==0)
    return (FALSE);
    if ((lpfnMAPIDetails = (LPMAPIDETAILS) GetProcAddress (hlibMAPI, "MAPIDetails"))==0)
    return (FALSE);
    if ((lpfnMAPIResolveName = (LPMAPIRESOLVENAME) GetProcAddress (hlibMAPI, "MAPIResolveName"))==0)
    return (FALSE);

    return (TRUE);
}
void DeinitSimpleMAPI ()
{
    if (hlibMAPI)
    {
    FreeLibrary (hlibMAPI);
    hlibMAPI = 0;
    }
}
bool SendMailMessage(AnsiString from,AnsiString to,AnsiString subj,AnsiString body,bool dialog)
{
  if(!hlibMAPI)
    return false;
  MapiRecipDesc sender;
  memset(&sender,0,sizeof(sender));
  sender.ulRecipClass=MAPI_ORIG;
  sender.lpszAddress=from.c_str();

  MapiRecipDesc recipients[1];
  memset(recipients,0,sizeof(recipients));
  recipients[0].ulRecipClass=MAPI_TO;
  recipients[0].lpszAddress=to.c_str();

  MapiMessage mes;
  memset(&mes,0,sizeof(mes));
  mes.lpszSubject=subj.c_str();
  mes.lpszNoteText=body.c_str();
  mes.lpOriginator=&sender;
  mes.nRecipCount=1;
  mes.lpRecips=recipients;

  ULONG res=MAPISendMail(NULL,(ULONG)Application->Handle,&mes
    ,dialog?MAPI_DIALOG:0,0L);
  return res==SUCCESS_SUCCESS;
}

