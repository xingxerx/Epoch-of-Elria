// Source/Engine/Rendering/Renderer.cpp
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"

void Renderer::RenderScene(const Scene& scene, const Camera& camera) {
  // Assumes scene contains lists of renderable objects.
  const auto& renderables = scene.GetRenderables();

  glm::mat4 projectionMatrix = camera.GetProjectionMatrix();
  glm::mat4 viewMatrix = camera.GetViewMatrix();

  for (const auto& renderable : renderables) {
    const Mesh* mesh = renderable->GetMesh();
    const Material* material = renderable->GetMaterial();
    glm::mat4 modelMatrix = renderable->GetModelMatrix();

    // Calculate MVP matrix
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    // Set shader uniforms
    material->GetShader()->Use();
    material->GetShader()->SetMat4("mvp", mvp);
    material->BindTextures(); //Bind textures for the material

    // Draw the mesh
    mesh->Draw();
  }
}