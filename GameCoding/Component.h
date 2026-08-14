#pragma once

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() abstract;
	virtual void Update() abstract;

	shared_ptr<GameObject> GetComponent() { return _owner.lock(); }

protected:
	weak_ptr<GameObject> _owner;
};

