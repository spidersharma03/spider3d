#include "cad/GeneralShape2D.h"
#include <math.h>

namespace spider3d
{
	namespace cad
	{

		GeneralShape2D::GeneralShape2D(int npoint , int flag)
		{
			this->npoint = npoint;
			this->flag = flag;
			coord = new float[3*npoint];
			transcoord = new float[3*npoint];
			matrix = new float[16];
			for(int i=0 ; i<16 ; i++)
				matrix[i]=0;
			v5 = new float[3];
			v4 = new float[3];
			v  = new float[3];
			zaxis = new float[3];
			zaxis[0] = 0;
			zaxis[1] = 0;
			zaxis[2] = 1;
		}

		void GeneralShape2D::applyTransform(float* point , float* dir , float* scale)
		{
			getTransform(point , dir , scale);
			for(int i=0 ; i<3*npoint ; i+=3){
				v4[0] = coord[i]; v4[1] = coord[i+1]; v4[2] = coord[i+2];
				Point3 point;
				point.x = coord[i]; point.y = coord[i+1]; point.z = coord[i+2];	
				//multiply(matrix , v4);
				transform.transform(point);	
				transcoord[i] =  point.x;
				transcoord[i+1] =  point.y;
				transcoord[i+2] =  point.z;	
			}
		}


		float* GeneralShape2D::getTransCoord()
		{
			return transcoord;
		}


		int GeneralShape2D::getNumPoints()
		{
			return npoint;
		}

		int GeneralShape2D::getFlag()
		{
			return flag;
		}


		void GeneralShape2D::getTransform(float* point , float* dir , float* scale)
		{
			//glMatrixMode(GL_MODELVIEW); 
			//glLoadIdentity();
			Matrix4f T, S, R1, R2;
			T.setTranslation( Vector3(point[0] , point[1] , point[2]) );
			//glTranslatef(point[0] , point[1] , point[2]);
			//glScalef(scale[0] , scale[1] , scale[2] );
			S.setScale( Vector3(scale[0] , scale[1] , scale[2]) );
			double theta = atan2(dir[0] , dir[1] );
			double phi = angle ( dir , zaxis);
			//theta += PI;
			phi -= M_PI/2;
			//glRotatef(-theta*180/PI , 0 , 0 , 1);
			R1.rotZ(-theta);
			//glRotatef(phi*180/PI , 1 , 0 , 0);
			R2.rotX(phi);
			//T*S*R1*R2
			transform = T;
			transform.multiply(S);
			transform.multiply(R1);
			transform.multiply(R2);
			//glScalef( scale[0] , scale[1] , scale[2] );
			//glGetFloatv(GL_MODELVIEW_MATRIX , matrix);
			//glLoadIdentity();

			/*cout<<"matrix"<<endl;
			cout<<matrix[0]<<" "<<matrix[4]<<" "<<matrix[8]<<" "<<matrix[12]<<endl;
			cout<<matrix[1]<<" "<<matrix[5]<<" "<<matrix[9]<<" "<<matrix[13]<<endl;
			cout<<matrix[2]<<" "<<matrix[6]<<" "<<matrix[10]<<" "<<matrix[14]<<endl;
			cout<<matrix[3]<<" "<<matrix[7]<<" "<<matrix[11]<<" "<<matrix[15]<<endl;*/
		}

		float GeneralShape2D::angle( float* v1 , float* v2)
		{
			float a = (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])/len(v1) / len(v2);
			return acos(a);
		}

		float GeneralShape2D::len(float* a)
		{
			return sqrt( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
		}

		void GeneralShape2D::createCoord(){
		};

		void GeneralShape2D::setCoord(float* coord){
			for(int i=0; i<3*npoint; i++)
				this->coord[i] = coord[i];
		};

	}
}