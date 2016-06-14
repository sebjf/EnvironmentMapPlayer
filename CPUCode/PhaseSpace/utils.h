// utils.h

#ifndef UTILS_H
#define UTILS_H

#define NOIMP cout <<"****unimplemented: " << __FILE__ << ":" << __LINE__ << endl

template <class T>
void SWAP(T &a, T &b)
{
  T tmp = a;
  a = b;
  b = tmp;
}

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifdef WIN32
#include <winsock.h>
#include <time.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

inline bool set_flags(size_t &flag, size_t bit, int value=1)
{
  // toggle
  if(value == -1) value = 1 - ((flag>>bit) & 1);
  flag &= ~(1 << bit);
  flag |= (value << bit);
  return value;
}

inline bool get_flags(size_t flag, size_t bit)
{
  return (flag & (1 << bit));
}

inline void delay(int msec)
{
#ifdef WIN32
  Sleep(msec);
#else
  // select is more accurate on linux than usleep or nanosleep
  timeval t = {msec/1000, (msec % 1000)*1000};
  select(0, 0, 0, 0, &t);
#endif
}

inline const char *date(const char *f="%b %d %H:%M:%S ")
{
  static char str[128]="";
  time_t t = time(0);
  strftime(str, 128, f, localtime(&t));
  return str;
}

#endif // UTILS_H
