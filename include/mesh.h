#ifndef _MESH_H_
#define _MESH_H_
#include "defines.h"
#include <vector>

struct Vertex {
  vec3 position;
  vec3 normal;
};
struct Transform {
	vec3 position= vec3(0);
	mat4 rotation = mat4(1.0);
	mat4 scale = mat4(1.0);
};

class Mesh {
 public:
  /**
   * construct a mesh from a obj file
   */
  Mesh(const std::string &path);
  ~Mesh();

  void signColor(float color[], int length);
  void setPosition(vec3);
  void setRotation(vec3 axis, float angle);
  void setScale(vec3);
  mat4 getModelMatrix();

  void draw() const;

  //VAO
  GLuint VertexArrayID; 
  //VBO
  GLuint vertexbuffer;
  GLuint uvbuffer;
  GLuint colorbuffer;

  
  Transform transform;
  glm::mat4 getPositionMatrix();


 private:
  //  mesh data
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<vec3> colors;


  void loadDataFromFile(const std::string &path);
};
#endif