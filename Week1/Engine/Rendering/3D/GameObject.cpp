#include "GameObject.h"
#include "Components/PhysicsComponent.h"

GameObject::GameObject(Model* model_, glm::vec3 position_) : model(nullptr)
{
	tag = "";
	model = model_;
	position = position_;
	angle = 0.0f;
	rotation = glm::vec3(0.0f, 1.0f, 0.0f); //Rotation on the Y axis
	scale = glm::vec3(1.0f);
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
		box = model->GetBoundingBox();
		box.transform = model->GetTransform(modelInstance);
	}

	hit = false;
}

GameObject::~GameObject()
{
//We no longer delete the models here. They're deleted in the scene graph
	model = nullptr;

	if (components.size() > 0)
	{
		for (int i = 0; i < components.size(); i++)
		{
			delete components[i];
			components[i] = nullptr;
		}

		components.clear();
	}
}
void GameObject::Render(Camera* camera_)
{
	if (model)
	{
		model->Render(camera_);
	}
}

void GameObject::RenderInstance(Camera* camera_)
{
	if (model)
	{
		model->RenderInstance(camera_, modelInstance);
	}
}

void GameObject::Update(float deltaTime_)
{
	//angle += 0.3f * deltaTime_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}

	// go through each component and update it 
	for (Component* t : components)
	{
		t->Update(deltaTime_);
	}
}

glm::vec3 GameObject::GetPosition() const
{
	return position;
}
float GameObject::GetAngle() const
{
	return angle;
}
glm::vec3 GameObject::GetRotation() const
{
	return rotation;
}
glm::vec3 GameObject::GetScale() const
{
	return scale;
}

void GameObject::SetPosition(glm::vec3 position_)
{
	position = position_;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetAngle(float angle_)
{
	angle = angle_;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetRotation(glm::vec3 rotation_)
{
	rotation = rotation_;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetScale(glm::vec3 scale_)
{
	scale = scale_;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
		//Under the assumption that scaling xyz by the same value
		box.minVert *= scale.x > 1.0f ? scale.x : (scale.x / 2.0f);
		box.maxVert *= scale.x > 1.0f ? scale.x : (scale.x / 2.0f);
	}
}

BoundingBox GameObject::GetBoundingBox()
{
	return box;
}

std::string GameObject::GetTag() const
{
	return tag;
}
void GameObject::SetTag(std::string tag_)
{
	tag = tag_;
}

bool GameObject::GetHit()
{
	return hit;
}
void GameObject::SetHit(bool hit_, int buttonType_)
{
	hit = hit_;
	if (hit)
	{
		std::cout << "Hi, we have successfully collided with this gameObject " << std::endl;
	}
}

glm::mat4 GameObject::GetModelInstanceTransform()
{
	return model->GetTransform(modelInstance);
}

GLuint GameObject::GetShaderProgram()
{
	return model->GetShaderProgram();
}
