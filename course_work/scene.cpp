#include "scene.h"

Scene::Scene(const char * path)
{
    std::ifstream in(path);
    if(!in)
        std::cout << "cant open model list file" << std::endl;

    std::string s;
    char id;
    while(std::getline(in, s))
    {
        id = s[0];
        std::cout << id;
        std::getline(in, s);
        std::cout << ": " << s << std::endl;
        modelManagers[id] = new ModelManager(s.c_str());
    }

    generator = new Generator(500, 500, 10);

    for(auto block : generator->getScene())
    {
        if(block.second.id != '0')
            modelManagers[block.second.id]->addTransform(block.second.model);
    }

    for(auto manager : modelManagers)
    {
        manager.second->setupBuffer();
    }

}

Scene::~Scene()
{
    
}

void Scene::draw(Shader& shader)
{
    for(auto manager : modelManagers)
    {
        manager.second->draw(shader);
    }
}