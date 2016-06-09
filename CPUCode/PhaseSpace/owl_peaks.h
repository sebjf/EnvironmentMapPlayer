// owl_peaks.h
// OWL v1.3

#ifndef OWL_PEAKS_H
#define OWL_PEAKS_H

#ifndef CTX
#define CTX
#endif // CTX

#ifdef __cplusplus
extern "C" {
#endif

struct OWLPeak {
  int id;
  int frame;
  int camera;
  int detector;
  int width;
  uint_t flag;
  
  float pos;
  float amp;
};

struct OWLDetectors {
  int id;
  int frame;
  uint_t detectors[8];
};

/* Image header
  short camera;
  short begin;
  short end;
  short flag;
*/

OWLAPI int owlGetPeaks(CTX OWLPeak *peaks, uint_t count);
OWLAPI int owlGetDetectors(CTX OWLDetectors *detectors, uint_t count);

#ifdef __cplusplus
}
#endif

#endif // OWL_PEAKS_H
