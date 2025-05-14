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
#include "../rendering/shared/material.h"
#include "../rendering/shared/mesh_renderer.h"

namespace ME {

class Scene {
   public:
    Scene();
    ~Scene();

    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;

    /**
     * List of scene resources like meshes, textures, shaders, etc.
     * Items index in the list are used to create the mesh renderer.
     */
    std::vector<const char*> meshPaths;
    std::vector<const char*> texturePaths;
    std::vector<const char*> shaderPaths;

    std::vector<ME::MeshRenderer> meshRenderers;

   private:
    void CreateResources();
    void BuildLights();
    void BuildCamera();
    void BuildMeshRenderers();
};

}  // namespace ME
