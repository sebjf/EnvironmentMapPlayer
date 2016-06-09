// owl_planes.h
// OWL v1.3

#ifndef OWL_PLANES_H
#define OWL_PLANES_H

#ifndef CTX
#define CTX
#endif // CTX

#ifdef __cplusplus
extern "C" {
#endif

struct OWLPlane {
  int id;
  int camera;
  int frame;
  float plane[4];
  float cond;
  uint_t flag;
};

OWLAPI int owlGetPlanes(CTX OWLPlane *planes, uint_t count);

#ifdef __cplusplus
}
#endif

#endif // OWL_PLANES_H
