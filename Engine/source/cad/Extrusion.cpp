#include "cad/Extrusion.h" 
#include "scene/Mesh.h"
#include "scene/SIndexedMeshBuffer.h"

using namespace spider3d;

namespace spider3d
{
	namespace cad
	{

		Extrusion::Extrusion(GeneralShape2D *shape , GeneralPath *path, sbool bFlipNormal):
	shape(shape) , path(path), m_bFlipNormal(bFlipNormal)
	{
		//appearance = 0;
		meshBuffer = 0;
		mesh = 0;	
		npoints_path = path->getNumPoints();
		npoints_shape = shape->getNumPoints();
		generateCoordinates();
		generateTextureCoordinates();
		GenerateCaps();
		generateNormals();
		generateIndices();
		createMesh();
	}	

	Extrusion::Extrusion(GeneralShape2D *shape , GeneralPath *path , void* app ):
	shape(shape) , path(path) , appearance(app)
	{
		npoints_path = path->getNumPoints();
		npoints_shape = shape->getNumPoints();
		generateCoordinates();
		generateTextureCoordinates();
		GenerateCaps();
		generateNormals();
		generateIndices();
		createMesh();
	}	

	Extrusion::~Extrusion()
	{
	}

	int Extrusion::getNumPolygons()
	{
		int n;
		if(shape->getFlag() == GeneralShape2D::CLOSED )n = (npoints_shape)*(npoints_path-1);
		if(shape->getFlag() == GeneralShape2D::OPEN )n = (npoints_shape-1)*(npoints_path-1);
		return n; 
	}

	void Extrusion::generateCoordinates()
	{
		float *p , *d , *s;
		float* vert;
		int k=0;
		coord = new float[3*npoints_shape*npoints_path];
		for(int i=0 ; i<npoints_path ; i++ ){
			p = path->getPoint(i);
			d = path->getDirection(i);
			s = path->getScaleFactor(i);
			shape->applyTransform( p , d , s);
			vert = shape->getTransCoord();
			for(int j=0 ; j<3*npoints_shape ; j++){
				coord[k++] = vert[j];
			}
		}	
	}

	void Extrusion::generateTextureCoordinates(){


		tcoords = new float[2*npoints_shape*npoints_path];

		double x, y, z , x0 , z0;
		float* pt;
		float sVal = 0.0f, tVal = 0.0f , t=0.0f;
		double angle, frac;
		float len = path->getLength();
		float d = M_PI;
		//if(appearance == 0)return;
		//if(appearance->getTexture() == 0 )return;

		//if(appearance->getTexture()->getMode() == Texture::PACK_S)len = 1;
		//if(appearance->getTexture()->getMode() == Texture::PACK_T)d = 1;
		//if(appearance->getTexture()->getMode() == Texture::PACK_NONE){d = 1; len = 1;}


		int idx = 0 , l = 0;
		pt = path->getPoint(0);
		x0 = pt[0] ; z0 = pt[2]; 


		for(int j=0; j < path->getNumPoints() ; j++){

			if(j>0)t += path->getSegmentLength(j-1) ;
			idx = 0;
			for(int i=0; i < shape->getNumPoints(); i++) {
				x = coord[idx];  y = coord[idx+1];  z = coord[idx+2];

				angle = atan2(x-x0,z-z0);       // -PI to PI
				frac = angle/d;          // -1.0 to 1.0
				sVal = (float) (0.5 + frac/2);   // 0.0f to 1.0f

				tVal = (float) (t/len);    // 0.0f to 1.0f; uses global height value

				tcoords[l++] = sVal;
				tcoords[l++] = tVal;
				idx += 3;
			}
		}
		//    printTexCoords();

	}


	void Extrusion::generateIndices(){
		int l=0,k=0 , n=0 , m=0 , i , j;

		if(shape->getFlag() == GeneralShape2D::CLOSED )indices = new unsigned short[4*(npoints_shape)*(npoints_path-1)];
		if(shape->getFlag() == GeneralShape2D::OPEN )indices = new unsigned short[4*(npoints_shape-1)*(npoints_path-1)];

		if(shape->getFlag() == GeneralShape2D::CLOSED ){
			n = npoints_path-1;
			m = npoints_shape;
		}
		if(shape->getFlag() == GeneralShape2D::OPEN ){
			n = npoints_path-1;
			m = npoints_shape-1;
		}

		switch(shape->getFlag()){
   case(GeneralShape2D::CLOSED):

	   for(j=0;j<n;j++){
		   for(i=0;i<m;i++){
			   if(i==m-1 ){
				   indices[l++] = i+k;	 indices[l++] = 2*i+k+1;
				   indices[l++] = i+k+1; indices[l++] = i+k+1-m;
			   }
			   else if(i!=m-1){
				   indices[l++] = i+k;	indices[l++] = i+k+m; 
				   indices[l++] = i+k+1+m; 	indices[l++] = i+k+1;
			   }
		   }
		   k=k+m;
	   }
	   break;

   case(GeneralShape2D::OPEN):

	   for(j=0;j<n;j++){
		   for(i=0;i<m;i++){
			   indices[l++] = i+k;	indices[l++] = i+k+1; 
			   indices[l++] = i+k+2+m; 	indices[l++] = i+k+m+1;
		   }
		   k=k+m+1;
	   }

	   break;
		}

	}

	//**************************************
	//   Calculate Normals ( Surface Normals )
	//**************************************
	void Extrusion::generateNormals(){
		int k=0 , l=0;  
		normals = new float[3*npoints_path*npoints_shape];
		float *v1 , *v2 , *v3 , *norm , *d1 , *d2;
		v1 = new float[3] ; v2 = new float[3]; v3 = new float[3]; norm = new float[3];
		d1 = new float[3] ; d2 = new float[3] ;
		
		for(int i=0 ; i<npoints_path-1 ; i++){
			for(int j=0 ; j<npoints_shape-1 ; j++){
				v1[0] = coord[i*npoints_shape*3 + 3*j];
				v1[1] = coord[i*npoints_shape*3 + 3*j + 1];
				v1[2] = coord[i*npoints_shape*3 + 3*j + 2];
				v2[0] = coord[i*npoints_shape*3 + 3*j + 3];
				v2[1] = coord[i*npoints_shape*3 + 3*j + 4];
				v2[2] = coord[i*npoints_shape*3 + 3*j + 5];
				v3[0] = coord[(i+1)*npoints_shape*3 + 3*j + 3];
				v3[1] = coord[(i+1)*npoints_shape*3 + 3*j + 4];
				v3[2] = coord[(i+1)*npoints_shape*3 + 3*j + 5];
				sub(v2 , v1 , d1);
				sub(v3 , v1 , d2);
				
				if(m_bFlipNormal)
					crossprod(d1 , d2 , norm);

				if(!m_bFlipNormal)
					crossprod(d2 , d1 , norm);

				normals[i*npoints_shape*3 + 3*j ] = norm[0];
				normals[i*npoints_shape*3 + 3*j +1 ] = norm[1];
				normals[i*npoints_shape*3 + 3*j + 2] = norm[2];

				normals[i*npoints_shape*3 + 3*j + 3] = norm[0];
				normals[i*npoints_shape*3 + 3*j + 4] = norm[1];
				normals[i*npoints_shape*3 + 3*j + 5] = norm[2];

				normals[(i+1)*npoints_shape*3 + 3*j ] = norm[0];
				normals[(i+1)*npoints_shape*3 + 3*j + 1] = norm[1];
				normals[(i+1)*npoints_shape*3 + 3*j + 2] = norm[2];

				normals[(i+1)*npoints_shape*3 + 3*j + 3] = norm[0];
				normals[(i+1)*npoints_shape*3 + 3*j + 4] = norm[1];
				normals[(i+1)*npoints_shape*3 + 3*j + 5] = norm[2];
			}
		}
		delete[] v1;
		delete[] v2;
		delete[] v3;
		delete[] d1;
		delete[] d2;
		delete[] norm;
	}

	//**************************************
	//  difference of two vectors
	//**************************************
	void Extrusion::sub(float* v1 , float* v2 , float* dif){
		dif[0] = v1[0] -  v2[0];
		dif[1] = v1[1] -  v2[1];
		dif[2] = v1[2] -  v2[2];
	}
	//**************************************
	// cross product of two vectors
	//**************************************
	void Extrusion::crossprod( float* v1 , float* v2 , float* norm){
		float d;
		norm[0] = v1[1]*v2[2] - v1[2]*v2[1];
		norm[1] = v1[2]*v2[0] - v1[0]*v2[2];
		norm[2] = v1[0]*v2[1] - v1[1]*v2[0];
		d = (float) (sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]) );
		norm[0] /= d;
		norm[1] /= d;
		norm[2] /= d;
	}



	Mesh* Extrusion::getMesh()
	{
		return mesh;
	}

	void Extrusion::createMesh()
	{
		int indexCount;
		if(shape->getFlag() == GeneralShape2D::CLOSED )
			indexCount = 4*(npoints_shape)*(npoints_path-1);
		if(shape->getFlag() == GeneralShape2D::OPEN )
			indexCount = 4*(npoints_shape-1)*(npoints_path-1);

		meshBuffer = new SIndexedMeshBuffer( npoints_shape*npoints_path , COORDINATES | NORMALS | TEXTURE_COORDINATE_2, 1 , indexCount );

		meshBuffer->setCoordinates(coord);
		meshBuffer->setTexCoords(0,tcoords);
		meshBuffer->setNormals(normals);
		meshBuffer->setIndices(indices);
		meshBuffer->setPrimitiveType(QUADS);
		meshBuffer->reCalculateAABB();

		delete[] coord;
		delete[] tcoords;
		delete[] normals;
		delete[] indices;

		mesh = new Mesh();
		mesh->addMeshBuffer(meshBuffer);
		mesh->reCalculateAABB(); 
	}
	/*
	void CALLBACK tessBeginCB(GLenum which)
	{
	glBegin(which);

	}



	void CALLBACK tessEndCB()
	{
	glEnd();

	}



	void CALLBACK tessVertexCB(const GLvoid *data)
	{
	// cast back to double type
	const GLdouble *ptr = (const GLdouble*)data;

	glVertex3dv(ptr);

	//cout << "  glVertex3d(" << *ptr << ", " << *(ptr+1) << ", " << *(ptr+2) << ");\n";

	}

	void CALLBACK tessErrorCB(GLenum errorCode)
	{
	const GLubyte *errorStr;

	errorStr = gluErrorString(errorCode);
	cerr << "[ERROR]: " << errorStr << endl;
	}


	*/
	void Extrusion::GenerateCaps()
	{
		/*
		int i , j , k=0; 
		startcap_coord = new GLdouble*[npoints_shape ];
		endcap_coord = new GLdouble*[npoints_shape ];

		for(i=0;i<npoints_shape;i++){
		startcap_coord[i] = new GLdouble[3];
		endcap_coord[i] = new GLdouble[3];
		}

		for(i=0;i<npoints_shape;i++){
		for(j=0;j<3;j++){
		startcap_coord[i][j] = 0.0;
		endcap_coord[i][j] = 0.0;
		}
		}

		for(i=0;i<npoints_shape;i++){
		for(j=0;j<3;j++){
		startcap_coord[i][j] = coord[k++];	
		}
		}
		k=0;

		for(i=0;i<npoints_shape;i++){
		for(j=0;j<3;j++){
		endcap_coord[i][j] = coord[k + 3*npoints_shape*(npoints_path-1)];	k++;
		}
		}

		float tempnorm[3] = {0,0,0};
		float temp1[3] = {0,0,0};
		float temp2[3] = {0,0,0};
		float temp3[3] = {0,0,0};
		float d1[3] = {0,0,0};
		float d2[3] = {0,0,0};
		temp1[0] = startcap_coord[0][0] ; temp1[1] = startcap_coord[0][1] ; temp1[2] = startcap_coord[0][2];
		temp2[0] = startcap_coord[1][0] ; temp2[1] = startcap_coord[1][1] ; temp2[2] = startcap_coord[1][2];
		temp3[0] = startcap_coord[2][0] ; temp3[1] = startcap_coord[2][1] ; temp3[2] = startcap_coord[2][2];
		sub(temp1 , temp2 , d1);
		sub(temp1 , temp3 , d2);
		crossprod( d1 , d2 , tempnorm);

		cap = glGenLists(2);

		GLUtesselator *tess = gluNewTess(); // create a tessellator

		gluTessCallback(tess, GLU_TESS_BEGIN, (void (__stdcall*)(void))tessBeginCB);
		gluTessCallback(tess, GLU_TESS_END, (void (__stdcall*)(void))tessEndCB);
		gluTessCallback(tess, GLU_TESS_ERROR, (void (__stdcall*)(void))tessErrorCB);
		gluTessCallback(tess, GLU_TESS_VERTEX, (void (__stdcall*)(void))tessVertexCB);

		glNewList(cap, GL_COMPILE);
		glNormal3fv( (float*)tempnorm );
		gluTessBeginPolygon(tess, 0);                   // with NULL data
		gluTessBeginContour(tess);
		for( i =0 ; i < npoints_shape ; i++)
		gluTessVertex(tess, startcap_coord[i], startcap_coord[i]);
		gluTessEndContour(tess);
		gluTessEndPolygon(tess);
		glEndList();


		temp1[0] = endcap_coord[0][0] ; temp1[1] = endcap_coord[0][1] ; temp1[2] = endcap_coord[0][2];
		temp2[0] = endcap_coord[1][0] ; temp2[1] = endcap_coord[1][1] ; temp2[2] = endcap_coord[1][2];
		temp3[0] = endcap_coord[2][0] ; temp3[1] = endcap_coord[2][1] ; temp3[2] = endcap_coord[2][2];
		sub(temp1 , temp2 , d1);
		sub(temp1 , temp3 , d2);
		crossprod( d1 , d2 , tempnorm);

		glNewList(cap+1, GL_COMPILE);
		glNormal3fv( tempnorm );
		gluTessBeginPolygon(tess, 0);                   // with NULL data
		gluTessBeginContour(tess);
		for( i =0 ; i < npoints_shape ; i++){
		gluTessVertex(tess, endcap_coord[i], endcap_coord[i]);
		} 
		gluTessEndContour(tess);
		gluTessEndPolygon(tess);
		glEndList();

		gluDeleteTess(tess);        // delete after tessellation   
		*/
	}

	}
}