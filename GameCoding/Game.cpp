#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "MeshRenderer.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	//_width = GWinSizeX;
	//_height = GWinSizeY;
	_graphics = make_shared<Graphics>(hwnd);
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	//GameObject
	_monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		_monster->GetorAddTransform();
		_monster->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
		_monster->GetTransform()->SetScale(Vec3(100.f, 100.0f, 1.f));
		// ..
	}

	_camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		_camera->GetorAddTransform();
		_camera->AddComponent(make_shared<Camera>());
	}
}

void Game::Update()
{
	_monster->Update();

	_camera->Update();
}

void Game::Render()
{
	_graphics->RenderBegin();

	//TEMP
	_monster->GetMeshRenderer()->Render(_pipeline);

	_graphics->RenderEnd();
}