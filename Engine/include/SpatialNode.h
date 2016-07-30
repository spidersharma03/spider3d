#ifndef _SPATIALNODE_H

#define _SPATIALNODE_H


typedef class SceneManager* SceneManagerptr;;

typedef class SpatialNode* SpatialNodeptr; 

class SpatialNode
{
  public:
 
  virtual void render();  	

  virtual void OnRegister();

  virtual void Update();

  virtual void print();

  void addChild(SpatialNodeptr);

  void addChild(SpatialNodeptr , unsigned int);

  void setTransform(float*);

  void setTransform(Matrix4f&);

  Matrix4f& getLocalTransform();
 
  Matrix4f& getGlobalTransform();
 
  bool removeChild(Spatial&);

  Spatial& getChild(unsigned int);
  
  bool hasChanged();

  void setChanged();

  Spatialptr getParent();

  char* getName();

  void setName(char*);

  SpatialNode( SpatialNodeptr , SceneManagerptr);

  protected:

    char name[50];

    SpatialNodeptr parent;

    SceneManagerptr sceneManager;
	
  private:

    bool _mChanged;
};



#endif