#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <algorithm>
#include <cmath>
#include <iostream>


template <class T> struct Vec2 {
	union {
		struct {T u, v;};
		struct {T x, y;};
		T raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(T _u, T _v) : u(_u),v(_v) {}
	template <class U> Vec2(const Vec2<U>& other): u((T) other.u), v((T) other.v) {}
	inline Vec2<T> operator +(const Vec2<T> &V) const { return Vec2<T>(u+V.u, v+V.v); }
	inline Vec2<T> operator -(const Vec2<T> &V) const { return Vec2<T>(u-V.u, v-V.v); }
	inline Vec2<T> operator *(float f)          const { return Vec2<T>(u*f, v*f); }
	inline T&      operator[](std::size_t idx)        { return raw[idx]; }
	inline const T& operator[](std::size_t idx) const { return raw[idx]; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<T>& v);
	template <class > friend void swap(Vec2& a, Vec2& b) { using std::swap; swap(a.u, b.u); swap(a.v, b.v); }
	inline bool operator ==(const Vec2<T> &V) { return u == V.u && v == V.v; }
	inline bool operator !=(const Vec2<T> &V) { return !(*this == V); }
};

template <class T> struct Vec3 {
	union {
		struct {T x, y, z;};
		struct {T ivert, iuv, inorm;};
		T raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T _x, T _y, T _z) : x(_x),y(_y),z(_z) {}
	template <class U> Vec3(U _x, U _y, U _z) : x((T) _x), y((T) _y), z((T) _z) {}
	template <class U> Vec3(const Vec3<U>& other): x((T) other.x), y((T) other.y), z((T) other.z) {}
	template <class U> Vec3(const Vec2<U>& other): x((T) other.u), y((T) other.v), z(T(0)) {}
	inline Vec3<T> operator ^(const Vec3<T> &v) const { return Vec3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<T> operator +(const Vec3<T> &v) const { return Vec3<T>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<T> operator -(const Vec3<T> &v) const { return Vec3<T>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<T> operator *(float f)          const { return Vec3<T>(x*f, y*f, z*f); }
	inline T       operator *(const Vec3<T> &v) const { return x*v.x + y*v.y + z*v.z; }
	inline T&      operator[](std::size_t idx)        { return raw[idx]; }
	inline const T& operator[](std::size_t idx) const { return raw[idx]; }
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3<T> & normalize(T l = 1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<T>& v);
	template <class > friend void swap(Vec3& a, Vec3& b) { using std::swap; swap(a.x, b.x); swap(a.y, b.y); swap(a.z, b.z); }
	inline bool operator ==(const Vec3<T> &V) { return x == V.x && y == V.y && z == V.z; }
	inline bool operator !=(const Vec3<T> &V) { return !(*this == V); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <class T> std::ostream& operator<<(std::ostream& s, const Vec2<T>& v) {
	s << "(" << v.x << ", " << v.y << ")";
	return s;
}

template <class T> std::ostream& operator<<(std::ostream& s, const Vec3<T>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return s;
}

#endif //__GEOMETRY_H__
