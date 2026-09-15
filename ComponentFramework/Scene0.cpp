#include <glew.h>
#include <iostream>
#include <SDL3/SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include <MMath.h>
#include <QMath.h>
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "XMLAssetManager.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"

bool Scene0::OnCreate()
{
	assetManager = std::make_shared<XMLAssetManager>();

	// Make sure these names match the stuff in your xml file:
	std::vector<std::string> names{ 
		"ActorGameBoard", 
		"ActorChecker1", 
		"ActorChecker2", 
		"ActorSkull", 
		"ActorCube", 
		"ActorMario"
	};

	// Did everything load ok from the xml file?
	bool everythingLoaded = true;
	for (const std::string& name : names) {
		auto iterator = assetManager->xmlAssets.find(name);
		// Is a name missing or has a typo?
		if (iterator == assetManager->xmlAssets.end()) {
			// Strings are handy to use the "+" symbol to join them up
			Debug::Error("Actor not found in Scene0.xml: " + name, __FILE__, __LINE__);
			everythingLoaded = false;
			continue; // skip to the next iteration
		}
		actors[name] = std::dynamic_pointer_cast<Actor>(iterator->second);
	}

	// Check our actors have the components they need
	for (const auto& pair : actors) {
		Ref<Actor> actor = pair.second;
		if (actor == nullptr ||
			actor->GetComponent<ShaderComponent>()   == nullptr ||
			actor->GetComponent<MaterialComponent>() == nullptr ||
			actor->GetComponent<MeshComponent>()     == nullptr ||
			actor->GetComponent<ShapeComponent>()    == nullptr) 
		{
			Debug::Error("Actor is missing a shader, material, mesh or shape: " + pair.first, __FILE__, __LINE__);
			everythingLoaded = false;
		}
	}

	// Finally load the camera and light actors
	auto cameraIterator = assetManager->xmlAssets.find("Camera1");
	camera = std::dynamic_pointer_cast<CameraActor>(cameraIterator->second);

	auto lightIterator = assetManager->xmlAssets.find("Light1");
	light = std::dynamic_pointer_cast<LightActor>(lightIterator->second);

	return everythingLoaded;
}

void Scene0::OnDestroy()
{
	actors.clear();
	camera = nullptr;
	light = nullptr;
	assetManager = nullptr;
}


void Scene0::HandleEvents(const SDL_Event& sdlEvent)
{
	Ref<TransformComponent> cameraTransform = camera->GetComponent <TransformComponent>();
	const float deltaAngle = 2.0f;

	switch (sdlEvent.type) {
	case SDL_EVENT_KEY_DOWN:
		// Camera rotations
		if (sdlEvent.key.scancode == SDL_SCANCODE_D) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-deltaAngle, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.scancode == SDL_SCANCODE_A) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(deltaAngle, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.scancode == SDL_SCANCODE_W) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(deltaAngle, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.scancode == SDL_SCANCODE_S) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-deltaAngle, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		// Render meshes and or collision shapes
		else if (sdlEvent.key.scancode == SDL_SCANCODE_M) {
			renderMeshes = !renderMeshes;
		}
		else if (sdlEvent.key.scancode == SDL_SCANCODE_C) {
			renderCollisionShapes = !renderCollisionShapes;
		}
		// TODO for YOU
		// Would be nice to have keys to move the camera based on where you are looking

		break;

	// All this mouse clicking stuff below is for Assignment 2: 
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
			// TODO for Assignment 2: 
			// Get a ray pointing into the world, We have the x, y pixel coordinates
			// Need to convert this into world space to build our ray

			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value
			for (auto it = actors.begin(); it != actors.end(); ++it) {
				Ref<Actor> actor = it->second;
				Ref<TransformComponent> transformComponent = actor->GetComponent <TransformComponent>();
				Ref<ShapeComponent> shapeComponent = actor->GetComponent <ShapeComponent>();
				// TODO for Assignment 2: 
				// Transform the ray into the local space of the object and check if a collision occured
			}
		}
		break;

	default:
		break;
	}
}

void Scene0::Update(const float deltaTime)
{
}

void Scene0::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatricesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto it = actors.begin(); it != actors.end(); ++it) {
		Ref<Actor> actor = it->second;
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->getTextureID());
		if (renderMeshes) {
			actor->GetComponent<MeshComponent>()->Render();
		}
		if (renderCollisionShapes) {
			// Drawing the physics shapes that go with each mesh
			actor->GetComponent<ShapeComponent>()->Render();
		}
	}
}