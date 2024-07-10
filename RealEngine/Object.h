#pragma once
#include "3DMath.h"
#include <vector>
#include "Graphics.h"
#include "Timer.h"
#include "Random.h"
#include "Component.h"
#include "Model.h"

namespace Game
{
  class Object
  {
  public:
    Object();
    ~Object();

  public:
    void SetScale(Vector3D scale);
    void SetAngle(Vector3D ang);
    Vector3D GetPosition();
    void SetPosition(Vector3D pos);    
    Model* GetModel();
    void SetModel(Model* model);
    void SetColor(float r, float g, float b);
    void SetVelocity(Vector3D vel);
    void SetAcceleration(Vector3D acc);
    void Update(float dt);
    virtual void SelfUpdate(float dt) {}
    void Render(Graphics::Device);
    Timer lifetime;
    Graphics::Device* gfx;
  public:
    std::vector<Component*>components;
    std::vector<Object*>attachments;
    void AddAttachment(Object* object);
    template<typename T>
    void AddComponent(T* component) {
      components.push_back(component);
      component->SetParent(this);
    }
    template<typename T>
    T* GetComponent(std::vector<Component*>& components) {
      for (auto component : components) {
        T* derived = dynamic_cast<T*>(component);
        if (derived != nullptr) {
          return derived;
        }
      }
      return nullptr;
    }
    template<typename T>
    void DeleteComponent(std::vector<Component*>& components) {
        for (auto component : components) {
            T* derived = dynamic_cast<T*>(component);
            if (derived != nullptr) {
                delete derived;
            }
        }
        return nullptr;
    }
  };
}

// Array of objects
extern std::vector<Game::Object*>objects;

// Creates a new object with a texture
Game::Object CreateObject(Vector3D pos, Model* model, Graphics::Device* gfx, const wchar_t* fileName);
// Create a new object basic params
Game::Object CreateObject(Vector3D pos, Model* model);
// Composite objects
// Tree
void CreateTree(Vector3D pos, Graphics::Device* gfx, float size);
