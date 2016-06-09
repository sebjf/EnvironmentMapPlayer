// owl_ctx.h
// OWL v1.3

#ifndef OWL_CTX_H
#define OWL_CTX_H

#ifdef WIN32
#ifdef __DLL
#define OWLAPI __declspec(dllexport)
#else // !__DLL
#define OWLAPI __declspec(dllimport)
#endif // __DLL
#else // ! WIN32
#define OWLAPI
#endif // WIN32

#ifndef CTX
#define CTX OWLContext *ctx,
#define CTXVOID OWLContext *ctx
#endif // CTX

#ifdef __cplusplus
extern "C" {
#endif

struct OWLContext;

OWLAPI OWLContext* owlCreateContext();
OWLAPI void owlReleaseContext(OWLContext **ctx);

OWLAPI int owlSetContext(OWLContext *ctx);

#ifdef __cplusplus
}
#endif

#endif // OWL_CTX_H
