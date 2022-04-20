#ifndef MESH_H
#define MESH_H
#pragma once
#include <map>
#include <vector>
#include <string>

#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
   
    glm::vec3 Position;
   
    glm::vec3 Normal;

    glm::vec2 TexCoords;
   
    glm::vec3 Tangent;
  
    glm::vec3 Bitangent;
   
    int m_BoneIDs[MAX_BONE_INFLUENCE];
   
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    
    void Draw(Shader& shader);
    

private:
   
    unsigned int VBO, EBO;

  
    void setupMesh();
    
};

#endif