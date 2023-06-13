#ifndef SIMPLE_PHYSICS2D_H
#define SIMPLE_PHYSICS2D_H
#include "Math2D.h"
#include <Windows.h>
#include <vector>

struct Shape
{
	// ���� ��ġ, ���������Լ��� ������ �ִ�
	enum Type { Circle = 0, Box, Count };

	Vector2 position;

	Shape() {}
	virtual ~Shape() {}

	//virtual void ComputeMass(float density) abstract;
	virtual void Initialize(float size) abstract; // ũ��
	virtual void Draw(HDC hDC) const abstract; // �׸���
	virtual Type GetType() const abstract; 
	virtual float GetVolume() abstract; // ����
};

struct Box : public Shape
{
	Vector2 min;
	Vector2 max;

	virtual void Initialize(float size) override;
	virtual void Draw(HDC hDC) const override;
	virtual Type GetType() const override { return Shape::Box; }
	virtual float GetVolume() override;
};

struct Circle : public Shape
{
	float radius;

	virtual void Initialize(float size) override;
	virtual void Draw(HDC hDC) const override;
	virtual Type GetType() const override { return Shape::Circle; }
	virtual float GetVolume() override;
};

struct RigidBody2D
{
	Vector2 velocity; // �ӵ�
	Vector2 force; // ��

	const float density;	// density : �е�.
	float mass; //����
	float invMass; // ���ӵ� ���ϱ����� ����

	RigidBody2D(float density) : density(density), mass(0), invMass(0) {}

	void ComputeMass(float volume); // ���� ���ϱ�
	void AddForce(const Vector2& force) { this->force += force; } //��
	void RemoveForce() { force = Vector2(); }
	void ApplyForces(float deltaTime); // �ӵ�,���ӵ� ���ϱ�
	void ApplyImpulse(const Vector2& impulse) { velocity += impulse * invMass; } //�浹���� * �浹
};

#ifndef DEL
#define DEL(p) { if(p) { delete (p); (p) = NULL; } }
#endif // !DEL
struct GameObject
{
	Shape* shape;
	RigidBody2D* rigidbody;

	GameObject(const Vector2& position, float size, Shape::Type type = Shape::Box)
	{
		// size�� ������ ũ��
		switch (type)
		{
		case Shape::Box: shape = new Box; break;
		default: shape = new Circle; break;
		}
		shape->Initialize(size);
		shape->position = position;

		rigidbody = new RigidBody2D(1);
		rigidbody->ComputeMass(shape->GetVolume());
	}
	~GameObject() { DEL(shape); DEL(rigidbody); }

	void SetStatic(); //���� ���� �����ִ�
	void ApplyVelocity(float deltaTime); //
};

struct Collision
{
	GameObject* A;
	GameObject* B;

	Vector2 normal;	// �浹 ���� ����.
	float penetration;	// �浹�� ���� ħ���� ����.

	Collision(GameObject* lhs, GameObject*rhs) : A(lhs), B(rhs), penetration(0){}

	bool IsCollision();
	void Solve();
	void PositionalCorrection();
	void InfiniteMassCorrection();

private:
	Collision() {}

	bool AABBvsAABB(Box* lhs, Box* rhs);
	bool AABBvsCircle(Box* lhs, Circle* rhs);
	bool CirclevsCircle(Circle* lhs, Circle* rhs);
};

struct Scene
{
	const UINT iterations; // ��Ʈ��? 10U?
	std::vector<GameObject*> objects;
	std::vector<Collision> contacts;

	Scene(UINT collisionIterations = 10U) : iterations(collisionIterations) {}
	~Scene();

	void Update(float deltaTime);
	void Render(HDC hDC);
	void AddObject(GameObject* object) { if (object) objects.push_back(object); }
	void Clear();
};

#endif // !SIMPLE_PHYSICS2D_H