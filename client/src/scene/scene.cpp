#include "scene.h"

ME::Scene::Scene() {
    BuildLights();
    BuildCamera();
}

ME::Scene::~Scene() {
    delete ambientLight;
    delete directionalLight;
    delete camera;
}

void ME::Scene::BuildLights() {
    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.1f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3{1.0f, 1.0f, -1.0f}.Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

void ME::Scene::BuildCamera() {
    camera = new ME::Camera();
    camera->position = ME::Vec3{0.0f, 0.0f, -10.0f};
    camera->viewPosition = ME::Vec3{0.0f, 0.0f, 100.0f};
}
