// owl_rpd.h
// OWL v1.3

#ifndef OWL_RPD_H
#define OWL_RPD_H

#ifdef WIN32
#ifdef __DLL
#define OWLAPI __declspec(dllexport)
#else // !__DLL
#define OWLAPI __declspec(dllimport)
#endif // __DLL
#else // ! WIN32
#define OWLAPI
#endif // WIN32

#define OWL_RPD_SAVE 1
#define OWL_RPD_LOAD 2

#ifndef CTX
#define CTX
#define CTXVOID void
#endif // CTX

#ifdef __cplusplus
extern "C" {
#endif

OWLAPI int owlRPDOpen(CTX const char *servername, const char *filename, int mode);
OWLAPI void owlRPDClose(CTXVOID);
OWLAPI int owlRPDSend(CTXVOID);
OWLAPI int owlRPDRecv(CTXVOID);

#ifdef __cplusplus
}
#endif

#endif // OWL_RPD_H
