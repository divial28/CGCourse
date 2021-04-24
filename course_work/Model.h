#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"
//class Shader;

class Model 
{
    public:
        /*  Методы   */
        Model(const char * path);
        void Draw(Shader& shader);	
        GLuint getVAO() { return meshes[0].getVAO(); }
        GLuint getTexture() { return textures[0].id; }
        std::size_t getIndicesCount() { return meshes[0].indices.size(); }

    private:
        /*  Данные модели  */
        std::vector<Mesh> meshes;
        std::vector<Texture> textures; 
        std::string directory;
        /*  Методы   */
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        GLuint TextureFromFile(const char* file, std::string dir);
};