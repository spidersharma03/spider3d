#ifndef _COREMATH_H

#define _COREMATH_H

#include "common/spiderDataTypes.h"
#include <math.h>

namespace spider3d
{

	namespace core
	{

class Plane;

#define M_PI (3.1415926535897932)

#define M_PI_2 (3.1415926535897932/2.0)

#define M_PI_3 (3.1415926535897932/3.0)

#define M_PI_4 (3.1415926535897932/4.0)

#define TWO_M_PI 2 * 3.1415926535897932

		////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////
		class Tuple3
		{
		public:
			sf32 x;
			sf32 y;
			sf32 z;
			void set(sf32, sf32, sf32);
			void set(const Tuple3&);
			void interpolate(Tuple3&, Tuple3&, float);
			
			inline float dist(Tuple3& other)
			{
				return sqrt( distSquared(other) ); 
			}
			inline float distSquared(Tuple3& other)
			{
				return ( (x-other.x)*(x-other.x) 
					   + (y-other.y)*(y-other.y) 
					   + (z-other.z)*(z-other.z) );
			}

			//virtual Tuple3& operator= ( Tuple3& ) = 0;
			/*Tuple3& operator= ( sf32 );
			Tuple3& operator* ( sf32 );
			Tuple3& operator*= ( sf32 );
			Tuple3& operator+ ( sf32 );
			Tuple3& operator+= ( sf32 );
			Tuple3& operator- ( sf32 );
			Tuple3& operator-= ( sf32 );

			Tuple3& operator+ ( Tuple3& );
			Tuple3& operator+= ( Tuple3& );
			Tuple3& operator- ( Tuple3& );
			Tuple3& operator-= ( Tuple3& );*/

			Tuple3();
			Tuple3(sf32 , sf32, sf32 );
			Tuple3(const Tuple3&);
		};


		class Tuple4
		{
		public:
			sf32 x;
			sf32 y;
			sf32 z;
			sf32 w;
			
			Tuple4():x(0.0f),y(0.0f),z(0.0f),w(0.0f)
			{
			}

			Tuple4(sf32 x, sf32 y, sf32 z, sf32 w):x(x),y(y),z(z),w(w)
			{
			}
//83260529
			Tuple4(const Tuple4& other):x(other.x),y(other.y),z(other.z),w(other.w)
			{

			}

			inline void set(float x, float y, float z, float w)
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}

			inline void set(Tuple4& other)
			{
				this->x = other.x;
				this->y = other.y;
				this->z = other.z;
				this->w = other.w;
			}
		};


		class Point3:public Tuple3
		{
		public:
			Point3();
			Point3(sf32 , sf32 , sf32);
			Point3(const Point3&);
			Point3 operator* ( sf32 );
			void operator*= ( sf32 );
		};


		class Point4:public Tuple4
		{
		};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
		class Vector3:public Tuple3
		{
		public:
			Vector3();
			Vector3(sf32 , sf32 , sf32);
			Vector3(const Vector3&);
			Vector3(const Tuple3&);
			Vector3& operator= ( Tuple3& );
			Vector3 operator* ( Tuple3& );
			Vector3& operator*= ( Tuple3& );
			Vector3 operator- ( Tuple3& );
			Vector3& operator-= ( Tuple3& );
			Vector3 operator+ ( Tuple3& );
			Vector3& operator+= ( Tuple3& );
			Vector3 operator* ( sf32 );
			Vector3& operator*= ( sf32 );
			Vector3 operator+ ( sf32 );
			Vector3& operator+= ( sf32 );
			Vector3 operator- ( sf32 );
			Vector3& operator-= ( sf32 );
			void sub(Tuple3&);
			void sub(Tuple3& , Tuple3&);
            void add(Tuple3&);
			void add(Tuple3& , Tuple3&);
			void scale(sf32);
			void scale(Tuple3&);
			void scale(Tuple3& , Tuple3&);
			sf32 dot(Tuple3&);
            
			Vector3 cross(const Tuple3&);
			static Vector3 cross(const Tuple3& , const Tuple3&);
			sf32 length();
			sf32 lengthSquared();
			void normalize();
		
		};

		class Vector4:public Tuple4
		{
		public:

			Vector4():Tuple4()
			{
			}

			Vector4(sf32 x, sf32 y, sf32 z, sf32 w):Tuple4(x,y,z,w)
			{
			}
		};

		class Color3:public Tuple3
		{
		public:
		   Color3(const Color3&  other):Tuple3(other.x,other.y,other.z)
		   {
		   }

		   Color3():Tuple3(0,0,0)
		   {
		   }

		   Color3(float r, float g, float b):Tuple3(r,g,b)
		   {
		   }

		   Color3 interpolate(Color3& c1 , Color3& c2 , sf32 val)
		   {
             Color3 c;
			 c.x = c1.x * val + ( 1 - val ) * c2.x;
 			 c.y = c1.y * val + ( 1 - val ) * c2.y;
			 c.z = c1.z * val + ( 1 - val ) * c2.z;
			 return c;
		   }

		   void set(Color3& c )
		   {
			   x = c.x;
			   y = c.y;
			   z = c.z;
		   }

		   void set(sf32 x , sf32 y , sf32 z )
		   {
			   this->x = x;
			   this->y = y;
			   this->z = z;
		   }

		   Color3& operator=(const Color3& other)
		   {
			   x = other.x;
			   y = other.y;
			   z = other.z;
			   return *this;
		   }
		};

		class Color4:public Tuple4
		{
		public:
			sbool operator== ( Color4& other)
			{
				return ( x==other.x && y==other.y && z==other.z && w==other.w );
			}

			sbool operator!= ( Color4& other)
			{
				return !( x==other.x && y==other.y && z==other.z && w==other.w );
			}

			void set(sf32 r ,sf32 g ,sf32 b ,sf32 a )
			{
               x = r; y = g; z = b; w = a;
			}

			void set(Color3& c )
		    {
			   x = c.x; y = c.y; z = c.z;
		    }

			void set(Color4& c )
		    {
			   x = c.x; y = c.y; z = c.z; w = c.w;
		    }
		};
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		//  Matrix3f
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		class Matrix3f
		{

		public:

			void set(sf32*);

			void set(Matrix3f&);

			void multiply(Matrix3f&); 	

			void multiply(Matrix3f& , Matrix3f&); 	

			Matrix3f();

			Matrix3f(Matrix3f&);


			sf32 m00 , m01 , m02;
			sf32 m10 , m11 , m12;
			sf32 m20 , m21 , m22;

			sf32 data[9];

		};

		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		//  Matrix4f
		///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////

		class Matrix4f
		{
		public:
			static Matrix4f Identity;

			void set(sf32*);

			void set(Matrix4f&);

			void print();

			void setTranslation(const Vector3&);

			void setTranslation(sf32 x, sf32 y, sf32 z );

			void setIdentity();

			void setScale(const Vector3&);

			void setScale(sf32);

			void rotX(sf32);

			void rotY(sf32);

			void rotZ(sf32);

			void lookAt(Point3& eye , Point3& target , Vector3& up);
         
			void lookAt(sf32, sf32, sf32, sf32, sf32, sf32, sf32, sf32, sf32);

			void perspective( sf32 , sf32 , sf32 , sf32);

			void frustum( sf32 , sf32 , sf32 , sf32 , sf32 , sf32);

			void ortho(sf32 left , sf32 right , sf32 bottom , sf32 top , sf32 zNear , sf32 zFar);

			static void UnProject(sf32 winx ,sf32 winy ,sf32 winz , Matrix4f& modelView , Matrix4f& projection , su32* viewport , Point3& );

  			static void Project(Point3& objectPoint , Matrix4f& modelView , Matrix4f& projection , su32* viewport , si32& x, si32& y);

			void transform(Point3&);

			void transform(Point4&);

			void transform(Vector3&);

			void transformPoint(Tuple3&);

			void transformVector(Tuple3&);

			Vector3& getTranslation();

			Vector4 getRow(si32);

			Vector4 getColumn(si32);

			void getReflectionMatrix(Matrix4f& reflectionMatrix, Vector4& planeEq); 
		
			void getReflectionMatrix(Matrix4f& reflectionMatrix, Plane& planeEq); 
		
			void multiply(Matrix4f&); 	

			Matrix4f operator* ( Matrix4f& );
			
			bool operator!= ( Matrix4f& );

			void multiply(Matrix4f& , Matrix4f&); 	

			Matrix4f getInverse();

			void Invert();

			Matrix4f getTranspose();

			void Transpose();

			Matrix4f& getInverseTranspose();

			//void operator+(Matrix4f& , Matrix4f&);
			float* pointer();

			void pointer(float*);

			Matrix4f();

			Matrix4f(const Matrix4f&);

			//Matrix4f& operator= ( Matrix4f& );

			sf32 m00 , m01 , m02 , m03;
			sf32 m10 , m11 , m12 , m13;
			sf32 m20 , m21 , m22 , m23;
			sf32 m30 , m31 , m32 , m33;

			sf32 m[16];

		};


	}

}


#endif