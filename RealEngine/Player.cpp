#include "Player.h"
#include "BIB.h"
#include "Transform.h"
#include "Physics.h"

void Player::SelfUpdate(float dt)
{
	// Get the transform and physics component
	Transform* transform = GetComponent<Transform>(components);
	Physics* physics = GetComponent<Physics>(components);

	Vector3D acceleration = physics->GetAcceleration();
	Vector3D pos = transform->GetPosition();

	// Apply player movement and logic in here
	// Get the direction vectors for up, left, down, right
	Vector3D forwardDir, sideDir;
	XMFLOAT3 camDir = camera->camDir;

	Vector3D velocity = { camDir.x * speed, camDir.y * speed, camDir.z * speed };

	// Move in the direction the player is facing
	if (KeyDown('W'))
	{
		SetAcceleration({ 10,10,10 });
	}
}
