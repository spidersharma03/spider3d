#include "math/coreMath.h"
#include "math/Plane.h"
#include<math.h>
#include<stdio.h>

namespace spider3d
{

	namespace core
	{
		Matrix4f Matrix4f::Identity;

		Tuple3::Tuple3():x(0),y(0),z(0)
		{
		}

		Tuple3::Tuple3(sf32 x , sf32 y , sf32 z):x(x),y(y),z(z)
		{
		}

		Tuple3::Tuple3(const Tuple3& t):x(t.x),y(t.y),z(t.z)
		{
		}

		void Tuple3::set(sf32 x ,sf32 y ,sf32 z )
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Tuple3::set(const Tuple3& other )
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		void Tuple3::interpolate(Tuple3& start, Tuple3& end, float t)
		{
			x = start.x * ( 1 - t ) + end.x * t;
			y = start.y * ( 1 - t ) + end.y * t;
			z = start.z * ( 1 - t ) + end.z * t;
		}

		/*Tuple3& Tuple3::operator= ( Tuple3& other )
		{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
		}*/

		/*Tuple3& Tuple3::operator= ( sf32 other )
		{
		x = other;
		y = other;
		z = other;
		return *this;
		}*/

		/*Tuple3& Tuple3::operator*= (sf32 val )
		{
		x *= val;
		y *= val;
		z *= val;
		return  *this;
		}

		Tuple3& Tuple3::operator+= (sf32 val )
		{
		x += val;
		y += val;
		z += val;
		return  *this;
		}

		Tuple3& Tuple3::operator-= (sf32 val )
		{
		x -= val;
		y -= val;
		z -= val;
		return  *this;
		}

		Tuple3& Tuple3::operator* (sf32 val )
		{
		Tuple3 other;
		other *= val;
		return other;
		}

		Tuple3& Tuple3::operator+ (sf32 val )
		{
		Tuple3 other;
		other += val;
		return other;
		}

		Tuple3& Tuple3::operator- (sf32 val )
		{
		Tuple3 other;
		other -= val;
		return other;
		}

		Tuple3& Tuple3::operator+= (Tuple3& t )
		{
		x += t.x;
		y += t.y;
		z += t.z;
		return *this;
		}

		Tuple3& Tuple3::operator-= (Tuple3& t )
		{
		x -= t.x;
		y -= t.y;
		z -= t.z;
		return *this;
		}

		Tuple3& Tuple3::operator+ (Tuple3& t )
		{
		Tuple3 other;
		other += t;
		return other;
		}

		Tuple3& Tuple3::operator- (Tuple3& t )
		{
		Tuple3 other;
		other -= t;
		return other;
		}*/

		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		Point3::Point3():Tuple3()
		{
		}

		Point3::Point3(const Point3& point):Tuple3(point)
		{
		}


		Point3::Point3(sf32 x, sf32 y ,sf32 z):Tuple3(x,y,z)
		{
		}

		Point3 Point3::operator* ( sf32 val )
		{
			Point3 p;
			p *= val;
			return p;
		}

		void Point3::operator*= ( sf32 val )
		{
			x *= val;
			y *= val;
			z *= val;
		}

		
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////


		/*Color3& Color3::operator= ( Tuple3& other )
		{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
		}*/


		Vector3::Vector3():Tuple3()
		{
		}

		Vector3::Vector3(sf32 x, sf32 y ,sf32 z):Tuple3(x,y,z)
		{
		}

		
		Vector3& Vector3::operator= ( Tuple3& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3& Vector3::operator*= ( Tuple3& other )
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vector3 Vector3::operator* ( Tuple3& other )
		{
			Vector3 v;
			v *= other;
			return v;
		}

		Vector3& Vector3::operator-= ( Tuple3& other )
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vector3 Vector3::operator- ( Tuple3& other )
		{
			Vector3 v;
			v -= other;
			return v;
		}

		Vector3& Vector3::operator+= ( Tuple3& other )
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3 Vector3::operator+ ( Tuple3& other )
		{
			Vector3 v;
			v += other;
			return v;
		}

		Vector3& Vector3::operator*= ( sf32 val )
		{
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}

		Vector3 Vector3::operator* ( sf32 val )
		{
			Vector3 v;
			v *= val;
			return v;
		}

		Vector3& Vector3::operator-= ( sf32 val )
		{
			x -= val;
			y -= val;
			z -= val;
			return *this;
		}

		Vector3 Vector3::operator- ( sf32 val )
		{
			Vector3 v;
			v -= val;
			return v;
		}

		Vector3& Vector3::operator+= ( sf32 val )
		{
			x += val;
			y += val;
			z += val;
			return *this;
		}

		Vector3 Vector3::operator+ ( sf32 val )
		{
			Vector3 v;
			v += val;
			return v;
		}

		void Vector3::add(Tuple3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}

		void Vector3::add(Tuple3& v1 , Tuple3& v2)
		{
			x = v1.x + v2.x;
			y = v1.y + v2.y;
			z = v1.z + v2.z;
		}

		void Vector3::sub(Tuple3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		void Vector3::sub(Tuple3& v1 , Tuple3& v2)
		{
			x = v1.x - v2.x;
			y = v1.y - v2.y;
			z = v1.z - v2.z;
		}

		void Vector3::scale(sf32 s)
		{
			x *= s;
			y *= s;
			z *= s;
		}


		void Vector3::scale(Tuple3& s)
		{
			x *= s.x;
			y *= s.y;
			z *= s.z;
		}

		sf32 Vector3::dot(Tuple3& other)
		{
			return ( x*other.x + y*other.y + z*other.z ); 
		}

		Vector3 Vector3::cross(const Tuple3& other)
		{
			Vector3 v;
			v.x = y * other.z - z * other.y;
			v.y = -(x * other.z - z * other.x);
			v.z = x * other.y - y * other.x;
			return v;
		}

		Vector3 Vector3::cross(const Tuple3& v1 , const Tuple3& v2)
		{
			Vector3 v;
			v.x = v1.y * v2.z - v1.z * v2.y;
			v.y = -(v1.x * v2.z - v1.z * v2.x);
			v.z = v1.x * v2.y - v1.y * v2.x;
			return v;
		}

		sf32 Vector3::length()
		{
			return (sf32)sqrtf ( (this->dot(*this) ) );
		}

		sf32 Vector3::lengthSquared()
		{
			return (sf32)( (this->dot(*this) ) );
		}

		void Vector3::normalize()
		{
			sf32 len = length();
			if ( len > 1e-08 )
				scale(1.0f/len);
		}

		Vector3::Vector3(const Vector3& v):Tuple3(v)
		{
		}


		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////
		void Matrix3f::set(sf32* data)
		{
			m00 = data[0]; m01 = data[1] ; m02 = data[2];
			m10 = data[3]; m11 = data[4] ; m12 = data[5];
			m20 = data[6]; m21 = data[7] ; m22 = data[8];
		}

		void Matrix3f::set(Matrix3f& matrix)
		{
			m00 = matrix.m00 ; m01 = matrix.m01 ; m02 = matrix.m02;
			m10 = matrix.m10 ; m11 = matrix.m11 ; m12 = matrix.m12;
			m20 = matrix.m20 ; m21 = matrix.m21 ; m22 = matrix.m22;
		}


		void Matrix3f::multiply(Matrix3f& inMatrix)
		{
			Matrix3f mTemp;

			mTemp.m00 = m00 * inMatrix.m00 + m01 * inMatrix.m10 + m02 * inMatrix.m12; 
			mTemp.m01 = m00 * inMatrix.m01 + m01 * inMatrix.m11 + m02 * inMatrix.m21; 
			mTemp.m02 = m00 * inMatrix.m02 + m01 * inMatrix.m12 + m02 * inMatrix.m22; 

			mTemp.m10 = m10 * inMatrix.m00 + m11 * inMatrix.m10 + m12 * inMatrix.m12; 
			mTemp.m11 = m10 * inMatrix.m01 + m11 * inMatrix.m11 + m12 * inMatrix.m21; 
			mTemp.m12 = m10 * inMatrix.m02 + m11 * inMatrix.m12 + m12 * inMatrix.m22; 

			mTemp.m20 = m20 * inMatrix.m00 + m21 * inMatrix.m10 + m22 * inMatrix.m12; 
			mTemp.m21 = m20 * inMatrix.m01 + m21 * inMatrix.m11 + m22 * inMatrix.m21; 
			mTemp.m22 = m20 * inMatrix.m02 + m21 * inMatrix.m12 + m22 * inMatrix.m22; 

			this->set(mTemp);
		}

		void Matrix3f::multiply(Matrix3f& inMatrix1 , Matrix3f& inMatrix2)
		{
			m00 = inMatrix1.m00 * inMatrix2.m00 + inMatrix1.m01 * inMatrix2.m10 + inMatrix1.m02 * inMatrix2.m20; 
			m01 = inMatrix1.m00 * inMatrix2.m01 + inMatrix1.m01 * inMatrix2.m11 + inMatrix1.m02 * inMatrix2.m21; 
			m02 = inMatrix1.m00 * inMatrix2.m02 + inMatrix1.m01 * inMatrix2.m12 + inMatrix1.m02 * inMatrix2.m22; 

			m10 = inMatrix1.m10 * inMatrix2.m00 + inMatrix1.m11 * inMatrix2.m10 + inMatrix1.m12 * inMatrix2.m20; 
			m11 = inMatrix1.m10 * inMatrix2.m01 + inMatrix1.m11 * inMatrix2.m11 + inMatrix1.m12 * inMatrix2.m21; 
			m12 = inMatrix1.m10 * inMatrix2.m02 + inMatrix1.m11 * inMatrix2.m12 + inMatrix1.m12 * inMatrix2.m22; 

			m20 = inMatrix1.m20 * inMatrix2.m00 + inMatrix1.m21 * inMatrix2.m10 + inMatrix1.m22 * inMatrix2.m20; 
			m21 = inMatrix1.m20 * inMatrix2.m01 + inMatrix1.m21 * inMatrix2.m11 + inMatrix1.m22 * inMatrix2.m21; 
			m22 = inMatrix1.m20 * inMatrix2.m02 + inMatrix1.m21 * inMatrix2.m12 + inMatrix1.m22 * inMatrix2.m22; 

		}

		Matrix3f::Matrix3f()
		{
			m00 = 1.0f ; m01 = 0.0 ; m02 = 0.0 ;
			m10 = 0.0f ; m11 = 1.0 ; m12 = 0.0 ;
			m20 = 0.0f ; m21 = 0.0 ; m22 = 1.0 ;
		}

		Matrix3f::Matrix3f(Matrix3f& inMatrix)
		{
			m00 = inMatrix.m00 ; m01 = inMatrix.m01 ; m02 = inMatrix.m02 ; 
			m10 = inMatrix.m10 ; m11 = inMatrix.m11 ; m12 = inMatrix.m12 ; 
			m20 = inMatrix.m20 ; m21 = inMatrix.m21 ; m22 = inMatrix.m22 ; 
		}



		///////////////

		void Matrix4f::set(sf32* data)
		{
			m00 = data[0];  m01 = data[1]  ; m02 = data[2];  m03 = data[3];
			m10 = data[4];  m11 = data[5]  ; m12 = data[6];  m13 = data[7];
			m20 = data[8];  m21 = data[9]  ; m22 = data[10]; m23 = data[11];
			m30 = data[12]; m31 = data[13] ; m32 = data[14]; m33 = data[15];
		}

		void Matrix4f::set(Matrix4f& matrix)
		{
			m00 = matrix.m00 ; m01 = matrix.m01 ; m02 = matrix.m02; m03 = matrix.m03;
			m10 = matrix.m10 ; m11 = matrix.m11 ; m12 = matrix.m12; m13 = matrix.m13;
			m20 = matrix.m20 ; m21 = matrix.m21 ; m22 = matrix.m22; m23 = matrix.m23;
			m30 = matrix.m30 ; m31 = matrix.m31 ; m32 = matrix.m32; m33 = matrix.m33;
		}


		void Matrix4f::setIdentity()
		{
			m00 = 1.0 ; m01 = 0.0 ; m02 = 0.0 ; m03 = 0.0;
			m10 = 0.0 ; m11 = 1.0 ; m12 = 0.0 ; m13 = 0.0;
			m20 = 0.0 ; m21 = 0.0 ; m22 = 1.0 ; m23 = 0.0;
			m30 = 0.0 ; m31 = 0.0 ; m32 = 0.0 ; m33 = 1.0;
		}

		void Matrix4f::setTranslation(const Vector3& translation )
		{
			m03 = translation.x;
			m13 = translation.y;
			m23 = translation.z;
		}

		void Matrix4f::setTranslation(sf32 x, sf32 y, sf32 z )
		{
			m03 = x;
			m13 = y;
			m23 = z;
		}

		void Matrix4f::rotX(sf32 angle)
		{
			this->setIdentity();
			m11 = (sf32)cos ( angle ); m12 = (sf32)-sin ( angle );
			m21 = (sf32)sin ( angle ); m22 = (sf32)cos ( angle );
		}

		void Matrix4f::rotY(sf32 angle)
		{
			this->setIdentity();
			m00 = (sf32)cos ( angle ); m02 = (sf32)-sin ( angle );
			m20 = (sf32)sin ( angle ); m22 = (sf32)cos ( angle );
		}

		void Matrix4f::rotZ(sf32 angle)
		{
			this->setIdentity();
			m00 = (sf32)cos ( angle ); m01 = (sf32)-sin ( angle );
			m10 = (sf32)sin ( angle ); m11 = (sf32)cos ( angle );
		}

		void Matrix4f::setScale(const Vector3& scale)
		{
			Matrix4f temp;
			temp.setIdentity();
			temp.m00 = scale.x;
			temp.m11 = scale.y;
			temp.m22 = scale.z;

			this->multiply( temp , *this  );
		}

		void Matrix4f::setScale(sf32 scale)
		{
			Matrix4f temp;
			temp.setIdentity();
			temp.m00 = scale;
			temp.m11 = scale;
			temp.m22 = scale;

			this->multiply( temp , *this  );
		}

		Vector3& Matrix4f::getTranslation()
		{
			Vector3 v;
			v.x = m03;
			v.y = m13;
			v.z = m23;
			return v;
		}


		Vector4 Matrix4f::getRow(si32 index)
		{
			if( index == 0 )
			{
				return Vector4( m00, m01, m02, m03);
			}
			else if( index == 1 )
			{
				return Vector4( m10, m11, m12, m13);
			}
			else if( index == 2 )
			{
				return Vector4( m20, m21, m22, m23);
			}
			else if( index == 3 )
			{
				return Vector4( m30, m31, m32, m33);
			}
			else
			{
				return Vector4( 0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		Vector4 Matrix4f::getColumn(si32 index)
		{
			if( index == 0 )
			{
				return Vector4( m00, m10, m20, m30);
			}
			else if( index == 1 )
			{
				return Vector4( m01, m11, m21, m31);
			}
			else if( index == 2 )
			{
				return Vector4( m02, m12, m22, m32);
			}
			else if( index == 3 )
			{
				return Vector4( m02, m13, m23, m33);
			}
			else
			{
				return Vector4( 0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		void Matrix4f::getReflectionMatrix(Matrix4f& reflectionMatrix, Vector4& planeEq) 
		{
			float dx = planeEq.x;
			float dy = planeEq.y;
			float dz = planeEq.z;
			float d = planeEq.w;

			reflectionMatrix.m00 = -dx * dx + dy * dy + dz * dz;
			reflectionMatrix.m01 = -2 * dy * dx;//-a * dy;
			reflectionMatrix.m02 = -2 * dz * dx;//-a * dz;
			reflectionMatrix.m03 = -2 * d * dx;

			reflectionMatrix.m10 = -2 * dx * dy;//-b * dx;
			reflectionMatrix.m11 = dx * dx + dz * dz - dy * dy;
			reflectionMatrix.m12 = -2 * dz * dy;//-b * dz;
			reflectionMatrix.m13 = -2 * d * dy;

			reflectionMatrix.m20 = -2 * dx * dz;//-c * dx;
			reflectionMatrix.m21 = -2 * dy * dz;//-c * dy;
			reflectionMatrix.m22 = dx * dx + dy * dy - dz * dz;
			reflectionMatrix.m23 = -2 * d * dz;

			reflectionMatrix.m30 = 0.0f;//-d * dx;
			reflectionMatrix.m31 = 0.0f;//-d * dy;
			reflectionMatrix.m32 = 0.0f;//-d * dz;
			reflectionMatrix.m33 = dx * dx + dy * dy + dz * dz;
		}

		void Matrix4f::getReflectionMatrix(Matrix4f& reflectionMatrix, Plane& planeEq) 
		{
			float dx = planeEq.Normal.x;
			float dy = planeEq.Normal.y;
			float dz = planeEq.Normal.z;
			float d = planeEq.D;

			reflectionMatrix.m00 = -dx * dx + dy * dy + dz * dz;
			reflectionMatrix.m01 = -2 * dy * dx;//-a * dy;
			reflectionMatrix.m02 = -2 * dz * dx;//-a * dz;
			reflectionMatrix.m03 = -2 * d * dx;

			reflectionMatrix.m10 = -2 * dx * dy;//-b * dx;
			reflectionMatrix.m11 = dx * dx + dz * dz - dy * dy;
			reflectionMatrix.m12 = -2 * dz * dy;//-b * dz;
			reflectionMatrix.m13 = -2 * d * dy;

			reflectionMatrix.m20 = -2 * dx * dz;//-c * dx;
			reflectionMatrix.m21 = -2 * dy * dz;//-c * dy;
			reflectionMatrix.m22 = dx * dx + dy * dy - dz * dz;
			reflectionMatrix.m23 = -2 * d * dz;

			reflectionMatrix.m30 = 0.0f;//-d * dx;
			reflectionMatrix.m31 = 0.0f;//-d * dy;
			reflectionMatrix.m32 = 0.0f;//-d * dz;
			reflectionMatrix.m33 = dx * dx + dy * dy + dz * dz;
		}

		float* Matrix4f::pointer()
		{
			float* data = new float[16];

			data[0]  = m00 ; data[1]  = m10; data[2]  = m20; data[3]  = m30;
			data[4]  = m01 ; data[5]  = m11; data[6]  = m21; data[7]  = m31;
			data[8]  = m02 ; data[9]  = m12; data[10] = m22; data[11] = m32;
			data[12] = m03 ; data[13] = m13; data[14] = m23; data[15] = m33;

			return data;
		}

		void Matrix4f::pointer(float* data)
		{
			data[0]  = m00 ; data[1]  = m10; data[2]  = m20; data[3]  = m30;
			data[4]  = m01 ; data[5]  = m11; data[6]  = m21; data[7]  = m31;
			data[8]  = m02 ; data[9]  = m12; data[10] = m22; data[11] = m32;
			data[12] = m03 ; data[13] = m13; data[14] = m23; data[15] = m33;
		}

		void Matrix4f::print()
		{
			printf("%6.4f%s%6.4f%s%6.4f%s%6.4f\n" , m00 , " " , m01 , " " , m02 , " " , m03 ); 
			printf("%6.4f%s%6.4f%s%6.4f%s%6.4f\n" , m10 , " " , m11 , " " , m12 , " " , m13 );    
			printf("%6.4f%s%6.4f%s%6.4f%s%6.4f\n" , m20 , " " , m21 , " " , m22 , " " , m23 );    
			printf("%6.4f%s%6.4f%s%6.4f%s%6.4f\n" , m30 , " " , m31 , " " , m32 , " " , m33 );    
		}

		void Matrix4f::transform(Point3 &point)
		{
			sf32 x = m00 * point.x + m01 * point.y + m02 * point.z + m03;
			sf32 y = m10 * point.x + m11 * point.y + m12 * point.z + m13;
			sf32 z = m20 * point.x + m21 * point.y + m22 * point.z + m23;
			point.x = x;
			point.y = y;
			point.z = z;
		}

		void Matrix4f::transform(Point4 &point)
		{
			sf32 x = m00 * point.x + m01 * point.y + m02 * point.z + m03 * point.w;
			sf32 y = m10 * point.x + m11 * point.y + m12 * point.z + m13 * point.w;
			sf32 z = m20 * point.x + m21 * point.y + m22 * point.z + m23 * point.w;
			sf32 w = m30 * point.x + m31 * point.y + m32 * point.z + m33 * point.w;
			point.x = x;
			point.y = y;
			point.z = z;
			point.w = w;
		}

		void Matrix4f::transform(Vector3 &vector)
		{
			sf32 x = m00 * vector.x + m01 * vector.y + m02 * vector.z ;
			sf32 y = m10 * vector.x + m11 * vector.y + m12 * vector.z ;
			sf32 z = m20 * vector.x + m21 * vector.y + m22 * vector.z ;
			vector.x = x;
			vector.y = y;
			vector.z = z;
		}

		void Matrix4f::transformPoint(Tuple3 &inPoint)
		{
			sf32 x = m00 * inPoint.x + m01 * inPoint.y + m02 * inPoint.z + m03;
			sf32 y = m10 * inPoint.x + m11 * inPoint.y + m12 * inPoint.z + m13;
			sf32 z = m20 * inPoint.x + m21 * inPoint.y + m22 * inPoint.z + m23;
			inPoint.x = x;
			inPoint.y = y;
			inPoint.z = z;
		}

		void Matrix4f::transformVector(Tuple3 &inVector)
		{
			sf32 x = m00 * inVector.x + m01 * inVector.y + m02 * inVector.z ;
			sf32 y = m10 * inVector.x + m11 * inVector.y + m12 * inVector.z ;
			sf32 z = m20 * inVector.x + m21 * inVector.y + m22 * inVector.z ;
			inVector.x = x;
			inVector.y = y;
			inVector.z = z;
		}

		bool Matrix4f::operator!= ( Matrix4f& other)
		{
			return !( m00 == other.m00 && m01 == other.m01 && m02 == other.m02 && m03 == other.m03 
				  && m10 == other.m10 && m11 == other.m11 && m12 == other.m12 && m13 == other.m13 
				  && m20 == other.m20 && m21 == other.m21 && m22 == other.m22 && m23 == other.m23 
				  && m30 == other.m30 && m31 == other.m31 && m32 == other.m32 && m33 == other.m33 );
		}

		void Matrix4f::multiply(Matrix4f& inMatrix)
		{
			Matrix4f mTemp;

			mTemp.m00 = m00 * inMatrix.m00 + m01 * inMatrix.m10 + m02 * inMatrix.m20 + m03 * inMatrix.m30; 
			mTemp.m01 = m00 * inMatrix.m01 + m01 * inMatrix.m11 + m02 * inMatrix.m21 + m03 * inMatrix.m31;
			mTemp.m02 = m00 * inMatrix.m02 + m01 * inMatrix.m12 + m02 * inMatrix.m22 + m03 * inMatrix.m32;
			mTemp.m03 = m00 * inMatrix.m03 + m01 * inMatrix.m13 + m02 * inMatrix.m23 + m03 * inMatrix.m33;

			mTemp.m10 = m10 * inMatrix.m00 + m11 * inMatrix.m10 + m12 * inMatrix.m20 + m13 * inMatrix.m30; 
			mTemp.m11 = m10 * inMatrix.m01 + m11 * inMatrix.m11 + m12 * inMatrix.m21 + m13 * inMatrix.m31;
			mTemp.m12 = m10 * inMatrix.m02 + m11 * inMatrix.m12 + m12 * inMatrix.m22 + m13 * inMatrix.m32;
			mTemp.m13 = m10 * inMatrix.m03 + m11 * inMatrix.m13 + m12 * inMatrix.m23 + m13 * inMatrix.m33;


			mTemp.m20 = m20 * inMatrix.m00 + m21 * inMatrix.m10 + m22 * inMatrix.m20 + m23 * inMatrix.m30; 
			mTemp.m21 = m20 * inMatrix.m01 + m21 * inMatrix.m11 + m22 * inMatrix.m21 + m23 * inMatrix.m31;
			mTemp.m22 = m20 * inMatrix.m02 + m21 * inMatrix.m12 + m22 * inMatrix.m22 + m23 * inMatrix.m32;
			mTemp.m23 = m20 * inMatrix.m03 + m21 * inMatrix.m13 + m22 * inMatrix.m23 + m23 * inMatrix.m33;

			mTemp.m30 = m30 * inMatrix.m00 + m31 * inMatrix.m10 + m32 * inMatrix.m20 + m33 * inMatrix.m30; 
			mTemp.m31 = m30 * inMatrix.m01 + m31 * inMatrix.m11 + m32 * inMatrix.m21 + m33 * inMatrix.m31;
			mTemp.m32 = m30 * inMatrix.m02 + m31 * inMatrix.m12 + m32 * inMatrix.m22 + m33 * inMatrix.m32;
			mTemp.m33 = m30 * inMatrix.m03 + m31 * inMatrix.m13 + m32 * inMatrix.m23 + m33 * inMatrix.m33;

			this->set(mTemp);
		}


		Matrix4f Matrix4f::operator* (Matrix4f& inMatrix)
		{
			Matrix4f mTemp;

			mTemp.m00 = m00 * inMatrix.m00 + m01 * inMatrix.m10 + m02 * inMatrix.m20 + m03 * inMatrix.m30; 
			mTemp.m01 = m00 * inMatrix.m01 + m01 * inMatrix.m11 + m02 * inMatrix.m21 + m03 * inMatrix.m31;
			mTemp.m02 = m00 * inMatrix.m02 + m01 * inMatrix.m12 + m02 * inMatrix.m22 + m03 * inMatrix.m32;
			mTemp.m03 = m00 * inMatrix.m03 + m01 * inMatrix.m13 + m02 * inMatrix.m23 + m03 * inMatrix.m33;

			mTemp.m10 = m10 * inMatrix.m00 + m11 * inMatrix.m10 + m12 * inMatrix.m20 + m13 * inMatrix.m30; 
			mTemp.m11 = m10 * inMatrix.m01 + m11 * inMatrix.m11 + m12 * inMatrix.m21 + m13 * inMatrix.m31;
			mTemp.m12 = m10 * inMatrix.m02 + m11 * inMatrix.m12 + m12 * inMatrix.m22 + m13 * inMatrix.m32;
			mTemp.m13 = m10 * inMatrix.m03 + m11 * inMatrix.m13 + m12 * inMatrix.m23 + m13 * inMatrix.m33;


			mTemp.m20 = m20 * inMatrix.m00 + m21 * inMatrix.m10 + m22 * inMatrix.m20 + m23 * inMatrix.m30; 
			mTemp.m21 = m20 * inMatrix.m01 + m21 * inMatrix.m11 + m22 * inMatrix.m21 + m23 * inMatrix.m31;
			mTemp.m22 = m20 * inMatrix.m02 + m21 * inMatrix.m12 + m22 * inMatrix.m22 + m23 * inMatrix.m32;
			mTemp.m23 = m20 * inMatrix.m03 + m21 * inMatrix.m13 + m22 * inMatrix.m23 + m23 * inMatrix.m33;

			mTemp.m30 = m30 * inMatrix.m00 + m31 * inMatrix.m10 + m32 * inMatrix.m20 + m33 * inMatrix.m30; 
			mTemp.m31 = m30 * inMatrix.m01 + m31 * inMatrix.m11 + m32 * inMatrix.m21 + m33 * inMatrix.m31;
			mTemp.m32 = m30 * inMatrix.m02 + m31 * inMatrix.m12 + m32 * inMatrix.m22 + m33 * inMatrix.m32;
			mTemp.m33 = m30 * inMatrix.m03 + m31 * inMatrix.m13 + m32 * inMatrix.m23 + m33 * inMatrix.m33;

			return mTemp;
		}


		void Matrix4f::multiply(Matrix4f& inMatrix1 , Matrix4f& inMatrix2)
		{
			m00 = inMatrix1.m00 * inMatrix2.m00 + inMatrix1.m01 * inMatrix2.m10 + inMatrix1.m02 * inMatrix2.m20 + inMatrix1.m03 * inMatrix2.m30; 
			m01 = inMatrix1.m00 * inMatrix2.m01 + inMatrix1.m01 * inMatrix2.m11 + inMatrix1.m02 * inMatrix2.m21 + inMatrix1.m03 * inMatrix2.m31;
			m02 = inMatrix1.m00 * inMatrix2.m02 + inMatrix1.m01 * inMatrix2.m12 + inMatrix1.m02 * inMatrix2.m22 + inMatrix1.m03 * inMatrix2.m32;
			m03 = inMatrix1.m00 * inMatrix2.m03 + inMatrix1.m01 * inMatrix2.m13 + inMatrix1.m02 * inMatrix2.m23 + inMatrix1.m03 * inMatrix2.m33;

			m10 = inMatrix1.m10 * inMatrix2.m00 + inMatrix1.m11 * inMatrix2.m10 + inMatrix1.m12 * inMatrix2.m20 + inMatrix1.m13 * inMatrix2.m30; 
			m11 = inMatrix1.m10 * inMatrix2.m01 + inMatrix1.m11 * inMatrix2.m11 + inMatrix1.m12 * inMatrix2.m21 + inMatrix1.m13 * inMatrix2.m31;
			m12 = inMatrix1.m10 * inMatrix2.m02 + inMatrix1.m11 * inMatrix2.m12 + inMatrix1.m12 * inMatrix2.m22 + inMatrix1.m13 * inMatrix2.m32;
			m13 = inMatrix1.m10 * inMatrix2.m03 + inMatrix1.m11 * inMatrix2.m13 + inMatrix1.m12 * inMatrix2.m23 + inMatrix1.m13 * inMatrix2.m33;

			m20 = inMatrix1.m20 * inMatrix2.m00 + inMatrix1.m21 * inMatrix2.m10 + inMatrix1.m22 * inMatrix2.m20 + inMatrix1.m23 * inMatrix2.m30; 
			m21 = inMatrix1.m20 * inMatrix2.m01 + inMatrix1.m21 * inMatrix2.m11 + inMatrix1.m22 * inMatrix2.m21 + inMatrix1.m23 * inMatrix2.m31;
			m22 = inMatrix1.m20 * inMatrix2.m02 + inMatrix1.m21 * inMatrix2.m12 + inMatrix1.m22 * inMatrix2.m22 + inMatrix1.m23 * inMatrix2.m32;
			m23 = inMatrix1.m20 * inMatrix2.m03 + inMatrix1.m21 * inMatrix2.m13 + inMatrix1.m22 * inMatrix2.m23 + inMatrix1.m23 * inMatrix2.m33;

			m30 = inMatrix1.m30 * inMatrix2.m00 + inMatrix1.m31 * inMatrix2.m10 + inMatrix1.m32 * inMatrix2.m20 + inMatrix1.m33 * inMatrix2.m30; 
			m31 = inMatrix1.m30 * inMatrix2.m01 + inMatrix1.m31 * inMatrix2.m11 + inMatrix1.m32 * inMatrix2.m21 + inMatrix1.m33 * inMatrix2.m31;
			m32 = inMatrix1.m30 * inMatrix2.m02 + inMatrix1.m31 * inMatrix2.m12 + inMatrix1.m32 * inMatrix2.m22 + inMatrix1.m33 * inMatrix2.m32;
			m33 = inMatrix1.m30 * inMatrix2.m03 + inMatrix1.m31 * inMatrix2.m13 + inMatrix1.m32 * inMatrix2.m23 + inMatrix1.m33 * inMatrix2.m33;

		}

		Matrix4f Matrix4f::getInverse()
		{
			Matrix4f out;
			out.setIdentity();

			sf32 d = ( m00 * m11 - m01 * m10 ) * (m22 * m33 - m23 * m32) -
				(m00 * m12 - m02 * m10) * (m21 * m33 - m23 * m31 ) +
				(m00 * m13 - m03 * m10) * (m21 * m32 - m22 * m31) +
				(m01 * m12 - m02 * m11) * (m20 * m33 - m23 * m30) -
				(m01 * m13 - m03 * m11) * (m20 * m32 - m22 * m30) +
				(m02 * m13 - m03 * m12) * (m20 * m31 - m21 * m30);

			if( fabs(d) < 1e-08  )
				return out;

			d = 1.0f/ d ;

			out.m00 = d * ( m11 * (m22 * m33 - m23 * m32) +
				m12 * ( m23 * m31 - m21 * m33) +
				m13 * ( m21 * m32 - m22 * m31) );
			out.m01 = d * ( m21 * ( m02 * m33 - m03 * m32) +
				m22 * ( m03 * m31 - m01 * m33 ) +
				m23 * ( m01 * m32 - m02 * m31) );
			out.m02 = d * ( m31 * ( m02 * m13 - m03 * m12) +
				m32 * ( m03 * m11 - m01 * m13) +
				m33 * ( m01 * m12 - m02 * m11) );
			out.m03 = d * (m01 * (m13 * m22 - m12 * m23) +
				m02 * (m11 * m23 - m13 * m21) +
				m03 * (m12 * m21 - m11 * m22));
			out.m10 = d * (m12 * (m20 * m33 - m23 * m30) +
				m13 * (m22 * m30 - m20 * m32) +
				m10 * (m23 * m32 - m22 * m33));
			out.m11 = d * (m22 * (m00 * m33 - m03 * m30) +
				m23 * (m02 * m30 - m00 * m32) +
				m20 * (m03 * m32 - m02 * m33));
			out.m12 = d * (m32 * (m00 * m13 - m03 * m10) +
				m33 * (m02 * m10 - m00 * m12) +
				m30 * (m03 * m12 - m02 * m13));
			out.m13 = d * (m02 * (m13 * m20 - m10 * m23) +
				m03 * (m10 * m22 - m12 * m20) +
				m00 * (m12 * m23 - m13 * m22));
			out.m20 = d * (m13 * (m20 * m31 - m21 * m30) +
				m10 * (m21 * m33 - m23 * m31) +
				m11 * (m23 * m30 - m20 * m33));
			out.m21 = d * (m23 * (m00 * m31 - m01 * m30) +
				m20 * (m01 * m33 - m03 * m31) +
				m21 * (m03 * m30 - m00 * m33));
			out.m22 = d * (m33 * (m00 * m11 - m01 * m10) +
				m30 * (m01 * m13 - m03 * m11) +
				m31 * (m03 * m10 - m00 * m13));
			out.m23 = d * (m03 * (m11 * m20 - m10 * m21) +
				m00 * (m13 * m21 - m11 * m23) +
				m01 * (m10 * m23 - m13 * m20));
			out.m30 = d * (m10 * (m22 * m31 - m21 * m32) +
				m11 * (m20 * m32 - m22 * m30) +
				m12 * (m21 * m30 - m20 * m31));
			out.m31 = d * (m20 * (m02 * m31 - m01 * m32) +
				m21 * (m00 * m32 - m02 * m30) +
				m22 * (m01 * m30 - m00 * m31));
			out.m32 = d * (m30 * (m02 * m11 - m01 * m12) +
				m31 * (m00 * m12 - m02 * m10) +
				m32 * (m01 * m10 - m00 * m11));
			out.m33 = d * (m00 * (m11 * m22 - m12 * m21) +
				m01 * (m12 * m20 - m10 * m22) +
				m02 * (m10 * m21 - m11 * m20));

			return out;
		}

		Matrix4f Matrix4f::getTranspose()
		{
			Matrix4f out;
			out.m00 = m00 ;out.m01 = m10; out.m02 = m20; out.m03 = m30;
			out.m10 = m01 ;out.m11 = m11; out.m12 = m21; out.m13 = m31;
			out.m20 = m02 ;out.m21 = m12; out.m22 = m22; out.m23 = m32;
			out.m30 = m03 ;out.m31 = m13; out.m32 = m23; out.m33 = m33;

			return out;
		}

		Matrix4f& Matrix4f::getInverseTranspose()
		{
			Matrix4f out , transpose;
			out = this->getTranspose();
			transpose = out.getInverse();
			return transpose;
		}

		void Matrix4f::lookAt(Point3& eye , Point3& target , Vector3& up )
		{
			setIdentity();
			Vector3 dir , newUp;

			dir.sub( target , eye );
			dir.normalize();

			Vector3 n;
			n = Vector3::cross(dir , up );
			n.normalize();

			newUp = Vector3::cross(n ,dir);

			sf32 eyeX = n.dot(eye);
			sf32 eyeY = newUp.dot(eye);
			sf32 eyeZ = dir.dot(eye);

			m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = -eyeX;
			m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = -eyeY;
			m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = eyeZ;
		}


		void Matrix4f::lookAt(sf32 eyeX , sf32 eyeY , sf32 eyeZ
			, sf32 targetX , sf32 targetY , sf32 targetZ , sf32 upX , sf32 upY , sf32 upZ )
		{
			setIdentity();
			Vector3 dir , newUp , eye(eyeX , eyeY, eyeZ) , target(targetX , targetY , targetZ);
			Vector3 up(upX, upY, upZ);
			dir.sub( target , eye );
			dir.normalize();

			Vector3 n;
			n  = Vector3::cross(dir , up );
			n.normalize();

			newUp = Vector3::cross(n ,dir);

			m00 = n.x  ;  m01 = n.y  ;  m02 = n.z  ;  m03 = n.dot(eye);
			m10 = newUp.x ;  m11 = newUp.y ;  m12 = newUp.z ;  m13 = newUp.dot(eye);
			m20 = -dir.x;  m21 = -dir.y;  m22 = -dir.z;  m23 = dir.dot(eye);
		}

		void Matrix4f::frustum(sf32 left , sf32 right , sf32 bottom , sf32 top , sf32 zNear , sf32 zFar )
		{
			setIdentity();

			m00 = (2*zNear/(right-left));
			m01 = 0;
			m02 = (right + left )/(right - left );
			m03 = 0;

			m10 = 0;
			m11 = (2*zNear/(top-bottom));
			m12 = (top + bottom)/(top - bottom);
			m13 = 0;

			m20 = 0;
			m21 = 0;
			m22 = -(zFar + zNear)/(zFar-zNear);
			m23 = -2*zFar*zNear/(zFar-zNear);

			m30 = 0;
			m31 = 0;
			m32 = -1;
			m33 = 0;
		}

		void Matrix4f::ortho(sf32 left , sf32 right , sf32 bottom , sf32 top , sf32 zNear , sf32 zFar)
		{
			setIdentity();

			m00 = (2/(right-left));
			m01 = 0;
			m02 = -(right + left )/(right - left );
			m03 = 0;

			m10 = 0;
			m11 = (2/(top-bottom));
			m12 = -(top + bottom)/(top - bottom);
			m13 = 0;

			m20 = 0;
			m21 = 0;
			m22 = -2/(zFar-zNear);
			m23 = -(zFar + zNear)/(zFar-zNear);

			m30 = 0;
			m31 = 0;
			m32 = 0;
			m33 = 1.0f;
		}

		void Matrix4f::perspective(sf32 fov , sf32 aspectRatio , sf32 zNear , sf32 zFar)
		{
			setIdentity();

			sf32 volumeHeight = 2 * zNear * tan(fov/2);
			sf32 volumeWidth  = volumeHeight * aspectRatio;

			m00 = 2*zNear/volumeWidth;
			m01 = 0;
			m02 = 0;
			m03 = 0;

			m10 = 0;
			m11 = 2*zNear/volumeHeight;
			m12 = 0;
			m13 = 0;

			m20 = 0;
			m21 = 0;
			m22 = -(zFar + zNear)/(zFar-zNear);
			m23 = -2*zFar*zNear/(zFar-zNear);

			m30 = 0;
			m31 = 0;
			m32 = -1;
			m33 = 0;
		}

		// Un Project 2d window point in world 3d point
		void Matrix4f::UnProject(sf32 winx, sf32 winy, sf32 winz, Matrix4f& modelView, Matrix4f& projection, su32 *viewport , Point3& out)
		{
            Point3 in;
			// Negate the viewport transform : 
			winx = ( (2*(winx - viewport[0])) / viewport[2] ) - 1 ;
			winy = ( (2*(winy - viewport[1])) / viewport[3] ) - 1 ;

			in.x = winx;
			in.y = winy;
			in.z = 2.0f*winz - 1.0;;
			// Get the Multiplied [P][VM] matrix
			Matrix4f pvm_t3D(projection);
			Matrix4f vm_t3D(modelView);
			pvm_t3D.multiply(vm_t3D);

			// Get Inverse --> ([p][vm]) ^-1 = [vm]^-1  * [p]^-1  = [m]^-1 * [v]^-1 * [p]^-1; 
			pvm_t3D = pvm_t3D.getInverse();

			// Multiply to get Unprojected point at Specified Z plane
			sf32 z1 = pvm_t3D.m30 * in.x + pvm_t3D.m31 * in.y + pvm_t3D.m32 * in.z + pvm_t3D.m33;
			pvm_t3D.transform(in);
			z1 = 1.0f/z1;
			in.x *= z1;
			in.y *= z1;
			in.z *= z1;
			out.set(in);
		}

		// Project 3d point on to screen coordinates
		void Matrix4f::Project(Point3& objectPoint , Matrix4f& modelView , Matrix4f& projection , su32* viewport , si32& winx, si32& winy)
		{
		// Mimic the Opengl pipeline
			Point4 point;
			point.x = objectPoint.x;
			point.y = objectPoint.y;
			point.z = objectPoint.z;
			point.w = 1.0f;
			// 1. Convert the input point into Eye space
			modelView.transform(point);
			// 2. Convert from Eye Space to Projection Space
			projection.transform(point);
			// 3. Convert to NDC Space.( Perspective divide ).
			if( fabs(point.w) > 0.0f )
			{
				sf32 invZ = 1.0f/point.w;
				point.x *= invZ;
				point.y *= invZ;
			}
			// 4. ViewPort Scale. ( -1 1 to width height )
			winx = viewport[0] + 0.5f * ( point.x + 1.0f ) * viewport[2];
			winy = viewport[1] + 0.5f * ( point.y + 1.0f ) * viewport[3];
		}

		Matrix4f::Matrix4f()
		{
			m00 = 1.0f ; m01 = 0.0 ; m02 = 0.0 ; m03 = 0.0 ;
			m10 = 0.0f ; m11 = 1.0 ; m12 = 0.0 ; m13 = 0.0 ;
			m20 = 0.0f ; m21 = 0.0 ; m22 = 1.0 ; m23 = 0.0 ;
			m30 = 0.0f ; m31 = 0.0 ; m32 = 0.0 ; m33 = 1.0 ;
		}

		Matrix4f::Matrix4f(const Matrix4f& inMatrix)
		{
			m00 = inMatrix.m00 ; m01 = inMatrix.m01 ; m02 = inMatrix.m02 ; m03 = inMatrix.m03 ;
			m10 = inMatrix.m10 ; m11 = inMatrix.m11 ; m12 = inMatrix.m12 ; m13 = inMatrix.m13 ;
			m20 = inMatrix.m20 ; m21 = inMatrix.m21 ; m22 = inMatrix.m22 ; m23 = inMatrix.m23 ;
			m30 = inMatrix.m30 ; m31 = inMatrix.m31 ; m32 = inMatrix.m32 ; m33 = inMatrix.m33 ;
		}


	} // end namespace math

}  // end namespace spider3d
