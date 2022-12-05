#include <mesh.h>
#include <utils.h>
#include <fstream>
#include <vector>

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("路径不正确\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "n") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			//int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			int matches = fscanf(file, "%d %d %d %d %d %d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex];
		//glm::vec2 uv = temp_uvs[uvIndex];
		glm::vec3 normal = temp_normals[normalIndex];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		//out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}



Mesh::Mesh(const std::string &path) { 
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);

	
	loadDataFromFile(getPath(path));
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	

}

void Mesh::signColor(float color[],int length)
{
	for (size_t i = 0; i < length; i+=3)
	{
		colors.push_back(vec3(color[i], color[i + 1], color[i + 2]));
	}
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, length*sizeof(float), &colors[0][0], GL_STATIC_DRAW);
}

void Mesh::setPosition(vec3 pos)
{
	transform.position = pos;
}
glm::mat4 Mesh::getPositionMatrix()
{
	return glm::translate(glm::mat4(1.0), transform.position);
}
void Mesh::setRotation(vec3 axis,float angle)
{
	transform.rotation= glm::rotate(mat4(1.0f), glm::radians(angle), axis);
}

void Mesh::setScale(vec3 sc)
{
	transform.scale= glm::scale(mat4(1.0f), sc);
}

mat4 Mesh::getModelMatrix()
{
	return  getPositionMatrix() * transform.rotation * transform.scale;
}

void Mesh::loadDataFromFile(const std::string &path) {
  std::ifstream infile(path);

  std::vector<vec3> vertices;
  std::vector<vec2> uvs;
  std::vector<vec3> normals; // Won't be used at the moment.
  bool res = loadOBJ(path.c_str(), vertices, uvs, normals);
  //bool res = loadOBJFormat2(path.c_str(), vertices, uvs, normals);

  if (res)
  {
	  for (int i = 0; i < vertices.size(); i++)
	  {
		  Vertex v;
		  v.position = vertices[i];
		  v.normal = normals[i];
		  this->vertices.push_back(v);
	  }
	  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3)*2, &(this->vertices)[0], GL_STATIC_DRAW);
  }
  else
  {
	  std::cout << "Failed to load obj file" << std::endl;
  }
}


void Mesh::draw() const {
	//载入顶点坐标
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type*
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);
	//载入顶点法线
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute
		3,                  // size
		GL_FLOAT,           // type*
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)(3*sizeof(float))            // array buffer offset
	);
	
	if (colors.size())
	{
		//载入点颜色信息
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                  // attribute
			3,                  // size
			GL_FLOAT,           // type*
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
	}
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	

}
