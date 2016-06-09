// interp.h

#ifndef INTERP_H
#define INTERP_H

#include <vector>

typedef float Real;

#ifndef ASSERT
#define ASSERT(expr)
#endif


//// Vector ////

class Vector {
public:

  Real x, y, z;

  Vector();
  Vector(const Vector &v);
  Vector(Real x, Real y, Real z);
  Vector(const Real *r);

  // specialize for desired types
  template <class T> Vector(const T &t);

  Vector& operator=(Real r);

  operator Real*() { return &x; }
  operator const Real*() const { return &x; }

  Real operator[](int n) const { ASSERT((unsigned int)n < 3); return (&x)[n]; }
  Real& operator[](int n) { ASSERT((unsigned int)n < 3); return (&x)[n]; }

  bool operator==(const Vector &v) const;

  Vector& operator+=(const Vector &v);
  Vector& operator-=(const Vector &v);
  Vector& operator*=(const Vector &v);
  Vector& operator*=(Real s);
};

Vector operator-(const Vector &v);
Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
Vector operator*(const Vector &v, Real s);
Vector operator*(Real s, const Vector &v);

// dot product
Real operator*(const Vector &a, const Vector &b);

Real dot(const Vector &a, const Vector &b);
Vector cross(const Vector &a, const Vector &b);

// normalize a
Vector norm(const Vector &a, Real v=1);

// project b onto a
Vector proj(const Vector &a, const Vector &b);

// compute normal
Vector normal(const Vector &a, const Vector &b, const Vector &c);


inline Vector::Vector() : x(0), y(0), z(0)
{ }

inline Vector::Vector(const Vector &v) : x(v.x), y(v.y), z(v.z)
{ }

inline Vector::Vector(Real x, Real y, Real z) : x(x), y(y), z(z)
{ }

inline Vector::Vector(const Real *r) : x(r[0]), y(r[1]), z(r[2])
{ }

inline Vector& Vector::operator=(Real r)
{ 
  x = y = z = r;
  return *this;
}

inline bool Vector::operator==(const Vector &v) const
{
  return x == v.x && y == v.y && z == v.z;
}

inline Vector& Vector::operator+=(const Vector &v)
{
  x += v.x; y += v.y; z += v.z;
  return *this;
}

inline Vector& Vector::operator-=(const Vector &v)
{
  x -= v.x; y -= v.y; z -= v.z;
  return *this;
}
  
inline Vector& Vector::operator*=(const Vector &v)
{
  x *= v.x; y *= v.y; z *= v.z;
  return *this;
}
  
inline Vector& Vector::operator*=(Real s)
{
  x *= s; y *= s; z *= s;
  return *this;
}

//// standalone operators ////

inline Vector operator-(const Vector &v)
{
  return Vector(-v.x, -v.y, -v.z);
}

inline Vector operator+(const Vector &a, const Vector &b)
{
  return Vector(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline Vector operator-(const Vector &a, const Vector &b)
{
  return Vector(a.x-b.x, a.y-b.y, a.z-b.z);
}

inline Vector operator*(const Vector &v, Real s)
{
  return Vector(v.x*s, v.y*s, v.z*s);
}

inline Vector operator/(const Vector &v, Real s)
{
  return v * (1.0 / s);
}

inline Vector operator*(Real s, const Vector &v)
{
  return Vector(v.x*s, v.y*s, v.z*s);
}


//// utils ////

template <> inline Vector::Vector(const OWLMarker &m) : x(m.x), y(m.y), z(m.z) {}

template <class T>
Vector finite_difference(const T &p0, const T &p1, const T &p2)
{
  Vector v0 = (Vector)p1 - (Vector)p0;
  float t0 = p1.frame - p0.frame;
  Vector v1 = (Vector)p2 - (Vector)p1;
  float t1 = p2.frame - p1.frame;

  float t = t0 + t1;
  float s = t1/t;
  return (v0 / t0) * s + (v1 / t1) * (1 - s);
}

template <class T>
int getSlopes(const std::vector<T> &markers, std::vector<T> &fd)
{
  for(int i = 1; i < (int)markers.size()-1; i++)
    {
      Vector v(finite_difference(markers[i-1], markers[i], markers[i+1]));
      fd.push_back(markers[0]);
      fd.back().x = v.x;
      fd.back().y = v.y;
      fd.back().z = v.z;
    }

  return fd.size();
}


//// LERP ////

class LERP {

  Vector p, m;

public:

  LERP() {}

  template <class T>
  int Init(const T &a, const T &b)
  {
    p = a;
    m = (Vector)b - (Vector)a;
    return 1;
  }

  int Calc(int LOD, std::vector<Vector> &out)
  {
    out.clear();
    for(int i = 0; i <= LOD; i++)
      out.push_back(point((Real)i / (Real)LOD));
    return 1;
  }

  Vector point(Real t)
  {
    return (m * t) + p;
  }
};


//// QUADRATIC FIT ////

class QuadraticFit {

  int start, end;

  std::vector<Vector> C;

public:

  QuadraticFit() : start(0), end(0) {}

  template <class T>
  int Init(const T &P)
  {
    if(P.size() < 3) return 0;
    C.resize(3);

    int frameOffset = P[0].frame;
  
    double t0 = 0;
    double t1 = P[1].frame - frameOffset;
    double t2 = P[2].frame - frameOffset;
  
    end = (int)t2;
    start = (int)t0;

    double T2 = -t2*t1 + t1*t0 + t2*t2 - t2*t0;
    double T1 = t2*t1 - t2*t0 - t1*t1 + t1*t0;
    double T0 = t2*t1 - t2*t0 - t1*t0 + t0*t0;

    if((T0 == 0.0) | (T1 == 0.0) | (T2 == 0.0)) return 0;

    T0 = 1.0/T0;  T1 = 1.0/T1;  T2 = 1.0/T2;

    Vector P0 = P[0], P1 = P[1], P2 = P[2];

    // X
    double Px[3] = { P0.x, P1.x, P2.x };
    C[0].x = Px[2]*t1*t0*T2 - Px[1]*t2*t0*T1 + Px[0]*t2*t1*T0;
    C[1].x = -((t1+t0)* Px[2] * T2) + ((t2 + t0)* Px[1] * T1) - ((t2 + t1)* Px[0] * T0);
    C[2].x = Px[2]*T2 - Px[1]*T1 + Px[0]*T0;

    // Y
    double Py[3] = { P0.y, P1.y, P2.y };
    C[0].y = Py[2]*t1*t0*T2 - Py[1]*t2*t0*T1 + Py[0]*t2*t1*T0;
    C[1].y = -((t1+t0)* Py[2] * T2) + ((t2 + t0)* Py[1] * T1) - ((t2 + t1)* Py[0] * T0);
    C[2].y = Py[2]*T2 - Py[1]*T1 + Py[0]*T0;

    // Z
    double Pz[3] = { P0.z, P1.z, P2.z };
    C[0].z = Pz[2]*t1*t0*T2 - Pz[1]*t2*t0*T1 + Pz[0]*t2*t1*T0;
    C[1].z = -((t1+t0)* Pz[2] * T2) + ((t2 + t0)* Pz[1] * T1) - ((t2 + t1)* Pz[0] * T0);
    C[2].z = Pz[2]*T2 - Pz[1]*T1 + Pz[0]*T0;

    return 1;
  }

  int Calc(int LOD, std::vector<Vector> &vertices)
  {
    vertices.clear();
    double increment = (double)(end - start) / (double)LOD;

    for(int k = 0; k <= LOD; k++)
      vertices.push_back(point((double)k * increment + start));

    return 1;
  }

  Vector point(double t)
  {
    return Vector(t*t*C[2].x + t*C[1].x + C[0].x,
                  t*t*C[2].y + t*C[1].y + C[0].y,
                  t*t*C[2].z + t*C[1].z + C[0].z);
  }
};


//// HERMITE SPLINE ////

template <typename VectorType>
class HermiteSpline {

  VectorType p0, p1, m0, m1;

public:
  
  HermiteSpline() {}

  template <class T>
  int Init(const T &points, Real scale0, Real scale1)
  {
    if(points.size() != 4) return 0;

    p0 = points[0];
    p1 = points[3];
    
    // get slopes at endpoints
    m0 = points[1]; m0 -= points[0]; m0 *= scale0;
    m1 = points[3]; m1 -= points[2]; m1 *= scale1;
    
    return 1;
  }

  template <class T>
  int Init(const T &_p0, const T &_p1, const T &_m0, const T &_m1, float s)
  {
    p0 = _p0;
    p1 = _p1;

    m0 = _m0;
    m1 = _m1;

    m0 *= s;
    m1 *= s;

    return 1;
  }

  int Calc(int LOD, std::vector<VectorType> &vertices)
  {
    vertices.clear();
    for(int k = 0; k <= LOD; k++)
      vertices.push_back(point((Real)k / (Real)LOD));
    return 1;  
  }

  VectorType point(Real t)
  {
    VectorType ret;
    VectorType _p0(p0), _p1(p1), _m0(m0), _m1(m1);
    
    // _p0 * t0 + _m0 * t1 + _p1 * t2 + _m1 * t3
    _p0 *= basisFunction(0, t);
    ret += _p0;
    _m0 *= basisFunction(1, t);
    ret += _m0;
    _p1 *= basisFunction(2, t);
    ret += _p1;
    _m1 *= basisFunction(3, t);
    ret += _m1;

    return ret;
  }

protected:

  Real basisFunction(int i, Real t)
  {
    Real t2 = t*t;
    Real t3 = t*t2;
    
    switch(i)
      {
      case 0: return 2*t3 - 3*t2 + 1;
      case 1: return t3 - 2*t2 + t;
      case 2: return -2*t3 + 3*t2;
      case 3: return t3 - t2;
      }
    return 0;
  }
};


#endif // INTERP_H
