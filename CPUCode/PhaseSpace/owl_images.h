// owl_images.h
// OWL v1.3

#ifndef OWL_IMAGES_H
#define OWL_IMAGES_H

#ifndef CTX
#define CTX
#endif // CTX

#ifdef __cplusplus
extern "C" {
#endif

struct OWLImage {
  int id;
  int frame;
  int camera;
  int detector;
  int width;
  uint_t flag;
  
  float pos;
  float amp;
};

OWLAPI int owlGetImages(CTX OWLImage *images, uint_t count);

#ifdef __cplusplus
}
#endif

#endif // OWL_IMAGES_H
