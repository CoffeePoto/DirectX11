#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"

SceneManager::SceneManager(shared_ptr<Graphics> graphics)
	:_graphics(graphics)
{
}

void SceneManager::Init()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();

	_activeScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	//Resource..

	_activeScene = LoadTestScene();
	Init();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();
	//Camera
	shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		camera->GetorAddTransform();
		camera->AddComponent(make_shared<Camera>());
		scene->AddGameObject(camera);
	}

	//GameObject
	shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		monster->GetorAddTransform();
		monster->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
		//monster ->GetTransform()->SetScale(Vec3(100.f, 100.0f, 1.f));
		scene->AddGameObject(monster);
		// ..
	}

	return scene;
}
