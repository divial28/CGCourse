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
        std::getline(in, s);
        modelManagers[id] = new ModelManager(s.c_str());
    }

    generator = new Generator(100, 100, 10);

    for(auto block : generator->getRawScene())
    {
        if(block.second.id != '0')
            modelManagers[block.second.id]->addAttribs(block.second);
    }

    for(auto manager : modelManagers)
    {
        if(manager.first == 'w')
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
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