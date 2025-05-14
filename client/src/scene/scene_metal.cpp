#ifdef VG_MAC

#include "scene_metal.h"

ME::SceneMetal::SceneMetal(MTL::Device* device, MTL::CommandQueue* cmdQueue) {
    this->device = device;
    this->cmdQueue = cmdQueue;

    scene = new ME::Scene();
    this->ambientLight = scene->ambientLight;
    this->directionalLight = scene->directionalLight;
    this->camera = scene->camera;
}

ME::SceneMetal::~SceneMetal() {
    delete ambientLight;
    delete directionalLight;
    delete camera;

    delete scene;
}

#endif
