//---------------------------------------------------------------------------
#ifndef smapiH
#define smapiH
//---------------------------------------------------------------------------
#ifndef MAPI_H
#include <mapi.h>
#endif

extern HINSTANCE    hlibMAPI;

extern LPMAPILOGON lpfnMAPILogon;
extern LPMAPILOGOFF lpfnMAPILogoff;
extern LPMAPISENDMAIL lpfnMAPISendMail;
extern LPMAPISENDDOCUMENTS lpfnMAPISendDocuments;
extern LPMAPIFINDNEXT lpfnMAPIFindNext;
extern LPMAPIREADMAIL lpfnMAPIReadMail;
extern LPMAPISAVEMAIL lpfnMAPISaveMail;
extern LPMAPIDELETEMAIL lpfnMAPIDeleteMail;
extern LPMAPIFREEBUFFER lpfnMAPIFreeBuffer;
extern LPMAPIADDRESS lpfnMAPIAddress;
extern LPMAPIDETAILS lpfnMAPIDetails;
extern LPMAPIRESOLVENAME lpfnMAPIResolveName;

#undef MAPILogon
#undef MAPILogoff
#undef MAPISendMail
#undef MAPISendDocuments
#undef MAPIFindNext
#undef MAPIReadMail
#undef MAPISaveMail
#undef MAPIDeleteMail
#undef MAPIFreeBuffer
#undef MAPIAddress
#undef MAPIDetails
#undef MAPIResolveName
#define MAPILogon           (*lpfnMAPILogon)
#define MAPILogoff          (*lpfnMAPILogoff)
#define MAPISendMail        (*lpfnMAPISendMail)
#define MAPISendDocuments   (*lpfnMAPISendDocuments)
#define MAPIFindNext        (*lpfnMAPIFindNext)
#define MAPIReadMail        (*lpfnMAPIReadMail)
#define MAPISaveMail        (*lpfnMAPISaveMail)
#define MAPIDeleteMail      (*lpfnMAPIDeleteMail)
#define MAPIFreeBuffer      (*lpfnMAPIFreeBuffer)
#define MAPIAddress         (*lpfnMAPIAddress)
#define MAPIDetails         (*lpfnMAPIDetails)
#define MAPIResolveName     (*lpfnMAPIResolveName)

extern BOOL InitSimpleMAPI(void);
extern void DeinitSimpleMAPI(void);
extern bool SendMailMessage(AnsiString from,AnsiString to,AnsiString subj,AnsiString body,bool dialog=true);

#endif
