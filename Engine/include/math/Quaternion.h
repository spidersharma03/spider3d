#ifndef _Quaternion_H
#define _Quaternion_H

#include "spiderDataTypes.h"
#include "coreMath.h"
#include <math.h>

namespace spider3d
{
namespace core
{

//! Quaternion class for representing rotations.
/** It provides cheap combinations and avoids gimbal locks.
Also useful for interpolations. */
class Quaternion
{
	public:

		//! Default Constructor
		Quaternion() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}

		//! Constructor
		Quaternion(sf32 x, sf32 y, sf32 z, sf32 w) : X(x), Y(y), Z(z), W(w) { }

		//! Constructor which converts euler angles (radians) to a Quaternion
		Quaternion(sf32 x, sf32 y, sf32 z);

		//! Constructor which converts euler angles (radians) to a Quaternion
		Quaternion(const Vector3& vec);

		//! Constructor which converts a matrix to a Quaternion
		Quaternion(const Matrix4f& mat);

		//! Equalilty operator
		bool operator==(const Quaternion& other) const;

		//! Assignment operator
		inline Quaternion& operator=(const Quaternion& other);

		//! Matrix assignment operator
		inline Quaternion& operator=(const Matrix4f& other);

		//! Add operator
		Quaternion operator+(const Quaternion& other) const;

		//! Multiplication operator
		Quaternion operator*(const Quaternion& other) const;

		//! Multiplication operator with scalar
		Quaternion operator*(sf32 s) const;

		//! Multiplication operator with scalar
		Quaternion& operator*=(sf32 s);

		//! Multiplication operator
		Vector3 operator*(Vector3& v) const;

		//! Multiplication operator
		Quaternion& operator*=(const Quaternion& other);

		//! Calculates the dot product
		inline sf32 dotProduct(const Quaternion& other) const;

		//! Sets new Quaternion
		inline Quaternion& set(sf32 x, sf32 y, sf32 z, sf32 w);

		//! Sets new Quaternion based on euler angles (radians)
		inline Quaternion& set(sf32 x, sf32 y, sf32 z);

		//! Sets new Quaternion based on euler angles (radians)
		inline Quaternion& set(const core::Vector3& vec);

		//! Normalizes the Quaternion
		inline Quaternion& normalize();

		//! Creates a matrix from this Quaternion
		Matrix4f getMatrix() const;

		//! Creates a matrix from this Quaternion
		void getMatrix( Matrix4f &dest ) const;

		//! Creates a matrix from this Quaternion
		inline void getMatrix_transposed( Matrix4f &dest ) const;

		//! Inverts this Quaternion
		Quaternion& makeInverse();

		//! Set this Quaternion to the result of the interpolation between two Quaternions
		Quaternion& slerp( Quaternion q1, Quaternion q2, sf32 interpolate );

		//! Create Quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The Quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		Quaternion& fromAngleAxis (sf32 angle, const Vector3& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (sf32 &angle, Vector3& axis) const;

		//! Output this Quaternion to an euler angle (radians)
		void toEuler(Vector3& euler) const;

		//! Set Quaternion to identity
		Quaternion& makeIdentity();

		//! Set Quaternion to represent a rotation from one vector to another.
		Quaternion& rotationFromTo(const Vector3& from, const Vector3& to);

		//! Quaternion elements.
		sf32 X; // vectorial (imaginary) part
		sf32 Y;
		sf32 Z;
		sf32 W; // real part
};


// Constructor which converts euler angles to a Quaternion
inline Quaternion::Quaternion(sf32 x, sf32 y, sf32 z)
{
	set(x,y,z);
}


// Constructor which converts euler angles to a Quaternion
inline Quaternion::Quaternion(const Vector3& vec)
{
	set(vec.x,vec.y,vec.z);
}


// Constructor which converts a matrix to a Quaternion
inline Quaternion::Quaternion(const Matrix4f& mat)
{
	(*this) = mat;
}


// equal operator
inline bool Quaternion::operator==(const Quaternion& other) const
{
	return ((X == other.X) &&
		(Y == other.Y) &&
		(Z == other.Z) &&
		(W == other.W));
}


// assignment operator
inline Quaternion& Quaternion::operator=(const Quaternion& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}


// matrix assignment operator
inline Quaternion& Quaternion::operator=(const Matrix4f& m)
{
	const sf32 diag = m.m00 + m.m11 + m.m22 + 1;

	if( diag > 0.0f )
	{
		const sf32 scale = sqrt(diag) * 2.0f; // get scale from diagonal

		// TODO: speed this up
		X = ( m.m21 - m.m12) / scale;
		Y = ( m.m02 - m.m20) / scale;
		Z = ( m.m10 - m.m01) / scale;
		W = 0.25f * scale;
	}
	else
	{
		if ( m.m00 > m.m11 && m.m00 > m.m22)
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			const sf32 scale = sqrt( 1.0f + m.m00 - m.m11 - m.m22) * 2.0f;

			// TODO: speed this up
			X = 0.25f * scale;
			Y = (m.m01 + m.m10) / scale;
			Z = (m.m20 + m.m02) / scale;
			W = (m.m21 - m.m12) / scale;
		}
		else if ( m.m11 > m.m22)
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const sf32 scale = sqrt( 1.0f + m.m11 - m.m00 - m.m22) * 2.0f;

			// TODO: speed this up
			X = (m.m01 + m.m10 ) / scale;
			Y = 0.25f * scale;
			Z = (m.m12 + m.m21 ) / scale;
			W = (m.m02 - m.m20 ) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const sf32 scale = sqrt( 1.0f + m.m22 - m.m00 - m.m11) * 2.0f;

			// TODO: speed this up
			X = (m.m02 + m.m20) / scale;
			Y = (m.m12 + m.m21) / scale;
			Z = 0.25f * scale;
			W = (m.m10 - m.m01) / scale;
		}
	}

	return normalize();
}


// multiplication operator
inline Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion tmp;

	tmp.W = (other.W * W) - (other.X * X) - (other.Y * Y) - (other.Z * Z);
	tmp.X = (other.W * X) + (other.X * W) + (other.Y * Z) - (other.Z * Y);
	tmp.Y = (other.W * Y) + (other.Y * W) + (other.Z * X) - (other.X * Z);
	tmp.Z = (other.W * Z) + (other.Z * W) + (other.X * Y) - (other.Y * X);

	return tmp;
}


// multiplication operator
inline Quaternion Quaternion::operator*(sf32 s) const
{
	return Quaternion(s*X, s*Y, s*Z, s*W);
}

// multiplication operator
inline Quaternion& Quaternion::operator*=(sf32 s)
{
	X*=s;
	Y*=s;
	Z*=s;
	W*=s;
	return *this;
}

// multiplication operator
inline Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return (*this = other * (*this));
}

// add operator
inline Quaternion Quaternion::operator+(const Quaternion& b) const
{
	return Quaternion(X+b.X, Y+b.Y, Z+b.Z, W+b.W);
}


// Creates a matrix from this Quaternion
inline Matrix4f Quaternion::getMatrix() const
{
	Matrix4f m;
	getMatrix_transposed(m);
	return m;
}


// Creates a matrix from this Quaternion
inline void Quaternion::getMatrix( Matrix4f &dest ) const
{
	dest.m00 = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
	dest.m10 = 2.0f*X*Y + 2.0f*Z*W;
	dest.m20 = 2.0f*X*Z - 2.0f*Y*W;
	dest.m30 = 0.0f;

	dest.m01 = 2.0f*X*Y - 2.0f*Z*W;
	dest.m11 = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
	dest.m21 = 2.0f*Z*Y + 2.0f*X*W;
	dest.m31 = 0.0f;

	dest.m02 = 2.0f*X*Z + 2.0f*Y*W;
	dest.m12 = 2.0f*Z*Y - 2.0f*X*W;
	dest.m22 = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
	dest.m32 = 0.0f;

	dest.m03 = 0.f;
	dest.m13 = 0.f;
	dest.m23 = 0.f;
	dest.m33 = 1.f;
}

// Creates a matrix from this Quaternion
inline void Quaternion::getMatrix_transposed( Matrix4f &dest ) const
{
	dest.m00 = 1.0f - 2.0f*Y*Y - 2.0f*Z*Z;
	dest.m01 = 2.0f*X*Y + 2.0f*Z*W;
	dest.m02 = 2.0f*X*Z - 2.0f*Y*W;
	dest.m03 = 0.0f;

	dest.m10 = 2.0f*X*Y - 2.0f*Z*W;
	dest.m11 = 1.0f - 2.0f*X*X - 2.0f*Z*Z;
	dest.m12 = 2.0f*Z*Y + 2.0f*X*W;
	dest.m13 = 0.0f;

	dest.m20 = 2.0f*X*Z + 2.0f*Y*W;
	dest.m21 = 2.0f*Z*Y - 2.0f*X*W;
	dest.m22 = 1.0f - 2.0f*X*X - 2.0f*Y*Y;
	dest.m23 = 0.0f;

	dest.m30 = 0.f;
	dest.m31 = 0.f;
	dest.m32 = 0.f;
	dest.m33 = 1.f;
}



// Inverts this Quaternion
inline Quaternion& Quaternion::makeInverse()
{
	X = -X; Y = -Y; Z = -Z;
	return *this;
}

// sets new Quaternion
inline Quaternion& Quaternion::set(sf32 x, sf32 y, sf32 z, sf32 w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
	return *this;
}


// sets new Quaternion based on euler angles
inline Quaternion& Quaternion::set(sf32 x, sf32 y, sf32 z)
{
	double angle;

	angle = x * 0.5;
	const double sr = sin(angle);
	const double cr = cos(angle);

	angle = y * 0.5;
	const double sp = sin(angle);
	const double cp = cos(angle);

	angle = z * 0.5;
	const double sy = sin(angle);
	const double cy = cos(angle);

	const double cpcy = cp * cy;
	const double spcy = sp * cy;
	const double cpsy = cp * sy;
	const double spsy = sp * sy;

	X = (sf32)(sr * cpcy - cr * spsy);
	Y = (sf32)(cr * spcy + sr * cpsy);
	Z = (sf32)(cr * cpsy - sr * spcy);
	W = (sf32)(cr * cpcy + sr * spsy);

	return normalize();
}

// sets new Quaternion based on euler angles
inline Quaternion& Quaternion::set(const Vector3& vec)
{
	return set(vec.x, vec.y, vec.z);
}

// normalizes the Quaternion
inline Quaternion& Quaternion::normalize()
{
	sf32 n = X*X + Y*Y + Z*Z + W*W;

	if (n == 1)
		return *this;

	n = 1.0f / sqrtf(n);
	return (*this *=  n );
}


// set this Quaternion to the result of the interpolation between two Quaternions
inline Quaternion& Quaternion::slerp(Quaternion q1, Quaternion q2, sf32 time)
{
	sf32 angle = q1.dotProduct(q2);

	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	sf32 scale;
	sf32 invscale;

	if ((angle + 1.0f) > 0.05f)
	{
		if ((1.0f - angle) >= 0.05f) // spherical interpolation
		{
			const sf32 theta = acosf(angle);
			const sf32 invsintheta = 1.0f/(sinf(theta));
			scale = sinf(theta * (1.0f-time)) * invsintheta;
			invscale = sinf(theta * time) * invsintheta;
		}
		else // linear interploation
		{
			scale = 1.0f - time;
			invscale = time;
		}
	}
	else
	{
		q2.set(-q1.Y, q1.X, -q1.W, q1.Z);
		scale = (sf32)sinf(M_PI * (0.5f - time));
		invscale = sinf(M_PI * time);
	}

	return (*this = (q1*scale) + (q2*invscale));
}


// calculates the dot product
inline sf32 Quaternion::dotProduct(const Quaternion& q2) const
{
	return (X * q2.X) + (Y * q2.Y) + (Z * q2.Z) + (W * q2.W);
}


//! axis must be unit length
//! angle in radians
inline Quaternion& Quaternion::fromAngleAxis(sf32 angle, const Vector3& axis)
{
	const sf32 fHalfAngle = 0.5f*angle;
	const sf32 fSin = sinf(fHalfAngle);
	W = cosf(fHalfAngle);
	X = fSin*axis.x;
	Y = fSin*axis.y;
	Z = fSin*axis.z;
	return *this;
}


inline void Quaternion::toAngleAxis(sf32 &angle, Vector3 &axis) const
{
	const sf32 scale = sqrtf(X*X + Y*Y + Z*Z);

	if ( fabs(scale) < 1e-10f || W > 1.0f || W < -1.0f)
	{
		angle = 0.0f;
		axis.x = 0.0f;
		axis.y = 1.0f;
		axis.x = 0.0f;
	}
	else
	{
		const sf32 invscale = 1.0f/scale;
		angle = 2.0f * acosf(W);
		axis.x = X * invscale;
		axis.y = Y * invscale;
		axis.z = Z * invscale;
	}
}

inline void Quaternion::toEuler(Vector3& euler) const
{
	const double sqw = W*W;
	const double sqx = X*X;
	const double sqy = Y*Y;
	const double sqz = Z*Z;

	// heading = rotation about z-axis
	euler.z = (sf32) (atan2(2.0 * (X*Y +Z*W),(sqx - sqy - sqz + sqw)));

	// bank = rotation about x-axis
	euler.x = (sf32) (atan2(2.0 * (Y*Z +X*W),(-sqx - sqy + sqz + sqw)));

	// attitude = rotation about y-axis
	sf32 d = -2.0f * (X*Z - Y*W) > 1.0f ;
	if ( d > 1.0f ) 
		d = 1.0f;
	if ( d < -1.0f ) 
		d = -1.0f;
	//euler.y = asinf( clamp(-2.0f * (X*Z - Y*W), -1.0f, 1.0f) );
	euler.y = asinf( d );
}


inline Vector3 Quaternion::operator* (Vector3& v) const
{
	// nVidia SDK implementation
    
	Vector3 uv, uuv;
	Vector3 qvec(X, Y, Z);
	uv = qvec.cross(v);
	uuv = qvec.cross(uv);
	uv.scale(2.0f * W);
	uuv.scale(2.0f);
    Vector3 result;
	result.add(v);
	result.add(uv);
	result.add(uuv);
	return result;
}

// set Quaternion to identity
inline core::Quaternion& Quaternion::makeIdentity()
{
	W = 1.f;
	X = 0.f;
	Y = 0.f;
	Z = 0.f;
	return *this;
}

inline core::Quaternion& Quaternion::rotationFromTo(const Vector3& from, const Vector3& to)
{
	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	Vector3 v0, v1;
	v0 = from;
	v1 = to;
	v0.normalize();
	v1.normalize();

	const sf32 d = v0.dot(v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return makeIdentity();
	}

	const sf32 s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
	const sf32 invs = 1.f / s;
	Vector3 c;
	c = v0.cross(v1);
	c.scale(invs);

	X = c.x;
	Y = c.y;
	Z = c.z;
	W = s * 0.5f;

	return *this;
}


} 
}

#endif


