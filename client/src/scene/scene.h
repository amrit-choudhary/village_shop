/**
 * Scene class implementation
 * This file contains the implementation of the Scene class, which is responsible for managing
 * the 3D scene, including loading models, managing lights, and rendering the scene.
 * The Scene class provides methods to load models, set up lights, and render the scene using
 * the rendering engine. It also handles the camera and view transformations.
 */

#pragma once

#include <vector>

#include "../rendering/shared/camera.h"
#include "../rendering/shared/light.h"

namespace ME {

class Scene {
   public:
    Scene();
    ~Scene();

    void BuildLights();
    void BuildCamera();

    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;

   private:
};

}  // namespace ME
