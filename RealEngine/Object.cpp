#include "Object.h"
#include "BIB.h"
#include "Graphics.h"
#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "Cube.h"
#include "IcoSphere.h"

std::vector<Game::Object*> objects;

Game::Object::Object()
{
  // Base object components, all objects have these
  AddComponent(new Transform());
  AddComponent(new Physics());
  AddComponent(new Model());
}

Game::Object::~Object()
{
    for (auto attachment : attachments)
    {
        if (attachment)
        {
            delete attachment;
        }
    }
}

void Game::Object::SetScale(Vector3D scale)
{
  Transform* transform = GetComponent<Transform>(components);

  if (transform)
  {
    transform->SetScale(scale);
  }
}

void Game::Object::SetAngle(Vector3D ang)
{
  Transform* transform = GetComponent<Transform>(components);

  if (transform)
  {
    transform->SetAngle(ang);
  }
}

Vector3D Game::Object::GetPosition()
{
  Transform* transform = GetComponent<Transform>(components);
  return transform->GetPosition();
}

void Game::Object::SetPosition(Vector3D pos)
{
  Transform* transform = GetComponent<Transform>(components);

  if (transform)
  {
    transform->SetPosition(pos);
  }
}

void Game::Object::SetModel(Model* model)
{
    Model* oldModel = GetComponent<Model>(components);
    *oldModel = *model;
    delete model;
}

Model* Game::Object::GetModel()
{
    Model* model = GetComponent<Model>(components);
    return model;
}

void Game::Object::SetColor(float r, float g, float b)
{
  Model* model = GetComponent<Model>(components);

  if (model)
  {
    int size = model->vertices.size();
    for (int i = 0; i < size; i++)
    {
      model->vertices[i].col = {r,g,b};
    }
  }
}

void Game::Object::SetVelocity(Vector3D vel)
{
  Physics* physics = GetComponent<Physics>(components);

  if (physics)
  {
    physics->SetVelocity(vel);
  }
}

void Game::Object::SetAcceleration(Vector3D acc)
{
    Physics* physics = GetComponent<Physics>(components);

    if (physics)
    {
        physics->SetAcceleration(acc);
    }
}

void Game::Object::Update(float dt)
{
  // Update all attached components
  for (auto component : components)
  {
    component->Update(dt);
  }
  // Update composite objects
  for (auto attachment : attachments)
  {
      Transform* transform = GetComponent<Transform>(components);
      Transform* attachmentTransform = attachment->GetComponent<Transform>(components);
      attachmentTransform->SetPosition(transform->GetPosition()+attachmentTransform->GetOffset());
  }
}

// Render a current object
// Params: the object to be rendered
void Game::Object::Render(Graphics::Device gfx)
{
  gfx.ObjectRender(this);
}

void Game::Object::AddAttachment(Object* object)
{
    attachments.push_back(object);
}

// Create an object with a position, model, and texture
// Returns: Newly created object
Game::Object CreateObject(Vector3D pos, Model* model, Graphics::Device* gfx, const const wchar_t* fileName)
{
    Game::Object *object = new Game::Object();

    // This will set the current model to point at the derived new one
    // The "delete" deletes the old model so it can be freshly replaced
    Model* emptyModel = object->GetComponent<Model>(object->components);
    *emptyModel = *model;
    delete model;

    // Set the transform's position
    Transform* transform = object->GetComponent<Transform>(object->components);
    transform->SetPosition(pos);

    // This will set our object's graphics pointer to the application's graphics
    // This allows our object to have textures and other rendering properties
    // We also attach a texture component to our object here for testing purposes, if it is not null
    object->gfx = gfx;
    Texture* texture = new Texture(gfx->pDevice);
    object->AddComponent(texture);
    texture->LoadFromFile(fileName);

    objects.push_back(object);
    return *object;
}

// Create an object with a position and model
// Returns: Newly created object
Game::Object CreateObject(Vector3D pos, Model* model)
{
    Game::Object* object = new Game::Object();

    // This will set the current model to point at the derived new one
    // The "delete" deletes the old model so it can be freshly replaced
    Model* emptyModel = object->GetComponent<Model>(object->components);
    *emptyModel = *model;
    delete model;

    // Set the transform's position
    Transform* transform = object->GetComponent<Transform>(object->components);
    transform->SetPosition(pos);

    objects.push_back(object);
    return *object;
}

void CreateTree(Vector3D pos, Graphics::Device* gfx, float size)
{
    // Create the components
    Game::Object trunk = CreateObject(pos,new Cube(), gfx, L"wood.png");
    trunk.SetScale({ 1,18,1 });

    Game::Object branch = CreateObject(pos + Vector3D(0.75,-1,0), new Cube(), gfx, L"wood.png");
    branch.SetAngle({0,0,90});
    branch.SetScale({ 0.5,3,0.5 });
    Vector3D leavesScale = { 5 * size,5 * size,5 * size };

    pos += {-2,6,0};
    Game::Object leavesOne = CreateObject(pos + Vector3D(0,-1,0),new IcoSphere(), gfx, L"leaves.png");
    leavesOne.SetScale(leavesScale*1.25);
    leavesOne.GetModel()->shaderType = ShaderType::sTexture;
    Game::Object leavesTwo = CreateObject(pos + Vector3D(4,2,0), new IcoSphere(), gfx, L"leaves.png");
    leavesTwo.SetScale(leavesScale*0.75);
    leavesTwo.GetModel()->shaderType = ShaderType::sTexture;
    Game::Object leavesThree = CreateObject(pos + Vector3D(6, -4, -2), new IcoSphere(), gfx, L"leaves.png");
    leavesThree.SetScale(leavesScale * 0.75);
    leavesThree.GetModel()->shaderType = ShaderType::sTexture;
}
