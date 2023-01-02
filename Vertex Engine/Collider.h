#pragma once

enum ColliderType {
	Box = 0,
	Circle,
};

struct Collider {

public:
	Collider();
	Collider(ColliderType _collisionType);

	void SetSize(float _size);


private:
	void ConfigureSprite();
	float m_ColliderSize;

};