#ifdef VG_MAC

#include "scene_metal.h"

ME::SceneMetal::SceneMetal(ME::Scene* scene, MTL::Device* device, MTL::CommandQueue* cmdQueue) {
    this->device = device;
    this->cmdQueue = cmdQueue;

    if (scene != nullptr) {
        this->ambientLight = scene->ambientLight;
        this->directionalLight = scene->directionalLight;
        this->camera = scene->camera;
    } else {
        scene = new ME::Scene();
        this->ambientLight = scene->ambientLight;
        this->directionalLight = scene->directionalLight;
        this->camera = scene->camera;
    }
}

ME::SceneMetal::~SceneMetal() {}

#endif
