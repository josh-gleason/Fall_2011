/** @file  vec.h
 *  @brief Defines the vec2, vec3, and vec4 classses.
 */

#ifndef __JDG_VEC_H__
#define __JDG_VEC_H__

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdio>
#include <cmath>

/**
  * @brief A 2 dimensional vector
  */
struct vec2 {

  GLfloat  x; ///< x dimension
  GLfloat  y; ///< y dimension

  vec2( 
      GLfloat s = GLfloat(0.0) ///< initial value for x and y.
  ) : x(s), y(s) {}

  vec2( GLfloat x,GLfloat y ) : x(x), y(y) {}

  /** @brief Copy Constructor */
  vec2( const vec2& v )
  {
    x=v.x; y=v.y;
  }

  /** @brief modifiable Indexing Operator */
  GLfloat& operator [] ( int i ) { return *(&x + i); }
  /** @brief non-modifying Indexing Operator */
  const GLfloat operator [] ( int i ) const { return *(&x + i); }

  //
  //  --- Arithmetic operators ---
  //

  vec2 operator - () const // unary minus operator
  { return vec2( -x, -y ); }

  vec2 operator + ( const vec2& v ) const
  { return vec2( x + v.x, y + v.y ); }

  vec2 operator - ( const vec2& v ) const
  { return vec2( x - v.x, y - v.y ); }

  vec2 operator * ( const GLfloat s ) const
  { return vec2( s*x, s*y ); }

  vec2 operator * ( const vec2& v ) const
  { return vec2( x*v.x, y*v.y ); }

  friend vec2 operator * ( const GLfloat s, const vec2& v )
  { return v * s; }

  vec2 operator / ( const GLfloat s ) const {
    GLfloat r = GLfloat(1.0) / s;
    return *this * r;
  }

  vec2& operator += ( const vec2& v )
  { x += v.x;  y += v.y;   return *this; }

  vec2& operator -= ( const vec2& v )
  { x -= v.x;  y -= v.y;  return *this; }

  vec2& operator *= ( const GLfloat s )
  { x *= s;  y *= s;   return *this; }

  vec2& operator *= ( const vec2& v )
  { x *= v.x;  y *= v.y; return *this; }

  vec2& operator /= ( const GLfloat s ) {
    GLfloat r = GLfloat(1.0) / s;
    *this *= r;
    return *this;
  }
  
  vec2& operator = ( const vec2& rhs )
  { x = rhs.x; y = rhs.y;   return *this; }

  //
  //  --- Insertion and Extraction Operators ---
  //

  friend std::ostream& operator << ( std::ostream& os, const vec2& v ) {
    return os << "( " << v.x << ", " << v.y <<  " )";
  }

  friend std::istream& operator >> ( std::istream& is, vec2& v )
  { return is >> v.x >> v.y ; }

  //
  //  --- Conversion Operators ---
  //

  operator const GLfloat* () const
  { return static_cast<const GLfloat*>( &x ); }

  operator GLfloat* ()
  { return static_cast<GLfloat*>( &x ); }
};

//
//  Non-class vec2 Methods
//

inline GLfloat dot( const vec2& u, const vec2& v ) {
  return u.x * v.x + u.y * v.y;
}

inline GLfloat length( const vec2& v ) {
  return std::sqrt( dot(v,v) );
}

inline vec2 normalize( const vec2& v ) {
  return v / length(v);
}

/**
  * @brief A 3 dimensional vector
  */
struct vec3 {

  GLfloat  x; ///< x dimension.
  GLfloat  y; ///< y dimension.
  GLfloat  z; ///< z dimension.

  //
  //  --- Constructors and Destructors ---
  //

  vec3(
      GLfloat s = GLfloat(0.0) ///< @brief initial value for x,y and z
  ) : x(s), y(s), z(s) {}

  vec3( GLfloat x, GLfloat y, GLfloat z ) :
    x(x), y(y), z(z) {}

  /** @brief Copy Constructor */
  vec3( const vec3& v ) { x = v.x;  y = v.y;  z = v.z; }

  /** @brief Copy a vec2 into a vec3 */
  vec3( const vec2& v, const float f=0.0 ) { x = v.x;  y = v.y;  z = f; }

  //
  //  --- Indexing Operator ---
  //

  /** @brief modifiable Indexing Operator */
  GLfloat& operator [] ( int i ) { return *(&x + i); }
  /** @brief non-modifying Indexing Operator */
  const GLfloat operator [] ( int i ) const { return *(&x + i); }

  //
  //  --- (non-modifying) Arithematic Operators ---
  //

  vec3 operator - () const  // unary minus operator
  { return vec3( -x, -y, -z ); }

  vec3 operator + ( const vec3& v ) const
  { return vec3( x + v.x, y + v.y, z + v.z ); }

  vec3 operator - ( const vec3& v ) const
  { return vec3( x - v.x, y - v.y, z - v.z ); }

  vec3 operator * ( const GLfloat s ) const
  { return vec3( s*x, s*y, s*z ); }

  vec3 operator * ( const vec3& v ) const
  { return vec3( x*v.x, y*v.y, z*v.z ); }

  friend vec3 operator * ( const GLfloat s, const vec3& v )
  { return v * s; }

  vec3 operator / ( const GLfloat s ) const {
    GLfloat r = GLfloat(1.0) / s;
    return *this * r;
  }

  //
  //  --- (modifying) Arithematic Operators ---
  //

  vec3& operator += ( const vec3& v )
  { x += v.x;  y += v.y;  z += v.z;  return *this; }

  vec3& operator -= ( const vec3& v )
  { x -= v.x;  y -= v.y;  z -= v.z;  return *this; }

  vec3& operator *= ( const GLfloat s )
  { x *= s;  y *= s;  z *= s;  return *this; }

  vec3& operator *= ( const vec3& v )
  { x *= v.x;  y *= v.y;  z *= v.z;  return *this; }

  vec3& operator /= ( const GLfloat s ) {
    GLfloat r = GLfloat(1.0) / s;
    *this *= r;
    return *this;
  }
  
  vec3& operator = ( const vec3& rhs )
  { x = rhs.x; y = rhs.y; z = rhs.z;   return *this; }

  //
  //  --- Insertion and Extraction Operators ---
  //

  friend std::ostream& operator << ( std::ostream& os, const vec3& v ) {
    return os << "( " << v.x << ", " << v.y << ", " << v.z <<  " )";
  }

  friend std::istream& operator >> ( std::istream& is, vec3& v )
  { return is >> v.x >> v.y >> v.z ; }

  //
  //  --- Conversion Operators ---
  //

  operator const GLfloat* () const
  { return static_cast<const GLfloat*>( &x ); }

  operator GLfloat* ()
  { return static_cast<GLfloat*>( &x ); }
};

//
//  Non-class vec3 Methods
//

inline GLfloat dot( const vec3& u, const vec3& v ) {
  return u.x*v.x + u.y*v.y + u.z*v.z ;
}

inline GLfloat length( const vec3& v ) {
  return std::sqrt( dot(v,v) );
}

inline vec3 normalize( const vec3& v ) {
  return v / length(v);
}

inline vec3 cross(const vec3& a, const vec3& b )
{
  return vec3( a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x );
}

// assumes z coord is 0 for a and b
inline vec3 cross(const vec2& a, const vec2& b )
{
  return vec3( 0.0, 0.0, a.x * b.y - a.y * b.x );
}

/**
  * @brief A 4 dimensional vector.
  */
struct vec4 {

  GLfloat  x; ///< x dimension
  GLfloat  y; ///< y dimension
  GLfloat  z; ///< z dimension
  GLfloat  w; ///< w dimension

  //
  //  --- Constructors and Destructors ---
  //

  vec4( GLfloat s = GLfloat(0.0) ) :
    x(s), y(s), z(s), w(s) {}

  vec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w ) :
    x(x), y(y), z(z), w(w) {}

  /** @brief Copy Constructor */
  vec4( const vec4& v ) { x = v.x;  y = v.y;  z = v.z;  w = v.w; }

  /** @brief Copy a vec3 into a vec4 */
  vec4( const vec3& v, const float w = 1.0 ) : w(w)
  { x = v.x;  y = v.y;  z = v.z; }

  /** @brief Copy a vec2 into a vec4 */
  vec4( const vec2& v, const float z, const float w ) : z(z), w(w)
  { x = v.x;  y = v.y; }

  //
  //  --- Indexing Operator ---
  //

  GLfloat& operator [] ( int i ) { return *(&x + i); }
  const GLfloat operator [] ( int i ) const { return *(&x + i); }

  //
  //  --- (non-modifying) Arithematic Operators ---
  //

  vec4 operator - () const  // unary minus operator
  { return vec4( -x, -y, -z, -w ); }

  vec4 operator + ( const vec4& v ) const
  { return vec4( x + v.x, y + v.y, z + v.z, w + v.w ); }

  vec4 operator - ( const vec4& v ) const
  { return vec4( x - v.x, y - v.y, z - v.z, w - v.w ); }

  vec4 operator * ( const GLfloat s ) const
  { return vec4( s*x, s*y, s*z, s*w ); }

  vec4 operator * ( const vec4& v ) const
  { return vec4( x*v.x, y*v.y, z*v.z, w*v.z ); }

  friend vec4 operator * ( const GLfloat s, const vec4& v )
  { return v * s; }

  vec4 operator / ( const GLfloat s ) const {
    GLfloat r = GLfloat(1.0) / s;
    return *this * r;
  }

  //
  //  --- (modifying) Arithematic Operators ---
  //

  vec4& operator += ( const vec4& v )
  { x += v.x;  y += v.y;  z += v.z;  w += v.w;  return *this; }

  vec4& operator -= ( const vec4& v )
  { x -= v.x;  y -= v.y;  z -= v.z;  w -= v.w;  return *this; }

  vec4& operator *= ( const GLfloat s )
  { x *= s;  y *= s;  z *= s;  w *= s;  return *this; }

  vec4& operator *= ( const vec4& v )
  { x *= v.x, y *= v.y, z *= v.z, w *= v.w;  return *this; }

  vec4& operator /= ( const GLfloat s ) {
    GLfloat r = GLfloat(1.0) / s;
    *this *= r;

    return *this;
  }
  
  vec4& operator = ( const vec4& rhs )
  { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;   return *this; }

  //
  //  --- Insertion and Extraction Operators ---
  //

  friend std::ostream& operator << ( std::ostream& os, const vec4& v ) {
    return os << "( " << v.x << ", " << v.y
      << ", " << v.z << ", " << v.w << " )";
  }

  friend std::istream& operator >> ( std::istream& is, vec4& v )
  { return is >> v.x >> v.y >> v.z >> v.w; }

  //
  //  --- Conversion Operators ---
  //

  operator const GLfloat* () const
  { return static_cast<const GLfloat*>( &x ); }

  operator GLfloat* ()
  { return static_cast<GLfloat*>( &x ); }
};

//
//  Non-class vec4 Methods
//

inline GLfloat dot( const vec4& u, const vec4& v ) {
  return u.x*v.x + u.y*v.y + u.z*v.z + u.w+v.w;
}

inline GLfloat length( const vec4& v ) {
  return std::sqrt( dot(v,v) );
}

inline vec4 normalize( const vec4& v ) {
  return v / length(v);
}

inline vec3 cross(const vec4& a, const vec4& b )
{
  return vec3( a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x );
}

//----------------------------------------------------------------------------

#define Error( str ) do { std::cerr << "[" __FILE__ ":" << __LINE__ << "] " \
  << str << std::endl; } while(0)


/** @deprecated Use vector subtraction instead */
inline vec4 minus(const vec4& a, const vec4&  b )
{
  Error( "replace with vector subtraction" );
  return vec4(a[0]-b[0], a[1]-b[1], a[2]-b[2], 0.0);
}

/** @deprecated Use vector insertion operator instead */
inline void printv(const vec4& a )
{
  Error( "replace with vector insertion operator" );
  printf("%f %f %f %f \n\n", a[0], a[1], a[2], a[3]);
}

#endif // __JDG_VEC_H__
