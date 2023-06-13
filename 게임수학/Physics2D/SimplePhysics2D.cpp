#include "SimplePhysics2D.h"

void Box::Initialize(float size)
{
	const float halfSize = size * 0.5f;
	//min.x = min.y = -halfSize;
	//max.x = max.y = halfSize;
	min.x = -halfSize;
	max.x = halfSize;
	min.y = -10;
	max.y = 10;
}

void Box::Draw(HDC hDC) const
{
	Rectangle(hDC, min.x + position.x, min.y + position.y, max.x + position.x, max.y + position.y);
}

float Box::GetVolume()
{
	float w = max.x - min.x;
	float h = max.y - min.y;
	float volume = w * h; // 부피.

	return volume;
}

void Circle::Initialize(float size)
{
	radius = size * 0.5f;
}

void Circle::Draw(HDC hDC) const
{
	Ellipse(hDC, -radius + position.x, -radius + position.y, radius + position.x, radius + position.y);
}

float Circle::GetVolume()
{
	return PI * radius * radius;
}

void RigidBody2D::ComputeMass(float volume)
{
	mass = volume * density;
	//질량이 0 이면 멈춰있는 오브젝트
	invMass = (mass) ? 1.0f / mass : 0.0f;
}

void RigidBody2D::ApplyForces(float deltaTime)
{
	if (0.0f == invMass) return; // 질량이 0 이면 고정

	// (힘 * 질량)(<-가속도) * 델타타임
	velocity += (force * invMass + gravity) * deltaTime;
}

void GameObject::SetStatic()
{
	rigidbody->mass = 0;
	rigidbody->invMass = 0;
}

void GameObject::ApplyVelocity(float deltaTime)
{
	if (0.0f == rigidbody->invMass) return;

	shape->position += rigidbody->velocity * deltaTime;
	rigidbody->ApplyForces(deltaTime);
}

bool Collision::IsCollision()
{
	int collisionType = A->shape->GetType() + B->shape->GetType();
	switch (collisionType)
	{
	case (Shape::Circle + Shape::Circle): return CirclevsCircle(reinterpret_cast<Circle*>(A->shape), reinterpret_cast<Circle*>(B->shape));
	case (Shape::Circle + Shape::Box):
	{
		bool check = false;
		switch (A->shape->GetType())
		{
		case Shape::Box: check = AABBvsCircle(reinterpret_cast<Box*>(A->shape), reinterpret_cast<Circle*>(B->shape)); break;
		default:
		{
			check = AABBvsCircle(reinterpret_cast<Box*>(B->shape), reinterpret_cast<Circle*>(A->shape));
			normal = -normal;
			break;
		}
		}
		return check;
	}
	case (Shape::Box + Shape::Box): return AABBvsAABB(reinterpret_cast<Box*>(A->shape), reinterpret_cast<Box*>(B->shape));
	default: return false;
	}
}

void Collision::Solve()
{
	if (Equal((A->rigidbody->invMass + B->rigidbody->invMass), 0))
	{
		InfiniteMassCorrection();
		return;
	}

	// A가 B로 향하는 상대 속도.
	Vector2 relativeVelocity = B->rigidbody->velocity - A->rigidbody->velocity;
	float velocityDotNormal = Vector2::Dot(relativeVelocity, normal);
	if (0 < velocityDotNormal) return;

	float restitution = 0.02f; // 반발력(force of restitution/탄성력:bouncing) 0 ~ 1
	// projection
	float j = -(1.0f + restitution) * velocityDotNormal;
	// 질량에 의한 반발력 재계산.
	j /= A->rigidbody->invMass + B->rigidbody->invMass;

	// 2n * Dot(-p, n)
	// (1.0f + restitution) * normal * (relativeVelocity Dot normal)
	Vector2 impulse = normal * j;
	// 반사벡터(r) = p + 2n * Dot(-p, n)
	A->rigidbody->ApplyImpulse(-impulse);
	B->rigidbody->ApplyImpulse(impulse);

	//float mass_sum = A->rigidbody->mass + B->rigidbody->mass;
	//A->rigidbody->ApplyImpulse(-impulse * A->rigidbody->mass / mass_sum);
	//B->rigidbody->ApplyImpulse(impulse * B->rigidbody->mass / mass_sum);
}

void Collision::PositionalCorrection()
{
	float invMassA = A->rigidbody->invMass;
	float invMassB = B->rigidbody->invMass;
	Vector2 correction = std::fmax(penetration - slope, 0.0f) / (invMassA + invMassB) * normal * percent;

	A->shape->position -= correction * invMassA;
	B->shape->position += correction * invMassB;
}

void Collision::InfiniteMassCorrection()
{
	A->rigidbody->velocity = Vector2();
	B->rigidbody->velocity = Vector2();
}

bool Collision::AABBvsAABB(Box* lhs, Box* rhs)
{
	const Vector2 incident = rhs->position - lhs->position;	// 충돌 방향 벡터.

	// 각 Box의 너비(width)의 반을 구한다.
	float lhs_extent = (lhs->max.x - lhs->min.x) * 0.5f;
	float rhs_extent = (rhs->max.x - rhs->min.x) * 0.5f;
	float overlapX = lhs_extent + rhs_extent - std::abs(incident.x);
	if (0 < overlapX)	// x축으로 겹치는 위치.
	{
		Vector2 closet;
		closet.x = clamp(incident.x, -lhs_extent, lhs_extent);
		// 각 Box의 높이(height)의 반을 구한다.
		lhs_extent = (lhs->max.y - lhs->min.y) * 0.5f;
		rhs_extent = (rhs->max.y - rhs->min.y) * 0.5f;
		float overlapY = lhs_extent + rhs_extent - std::abs(incident.y);
		if (0 < overlapY)	// y축으로 겹치는 위치.
								// 두 Box는 충돌한 상태이다.
		{
			closet.y = clamp(incident.y, -lhs_extent, lhs_extent);
			normal = incident - closet;
			normal.Normalize();

			penetration = (std::abs(normal.x) > std::abs(normal.y)) ? overlapX : overlapY;

			return true;
		}
	}

	return false;
}

bool Collision::AABBvsCircle(Box* lhs, Circle* rhs)
{
	const Vector2 incident = rhs->position - lhs->position;
	float extentX = (lhs->max.x - lhs->min.x) * 0.5f;
	float extentY = (lhs->max.y - lhs->min.y) * 0.5f;
	
	Vector2 closet;
	closet.x = clamp(incident.x, -extentX, extentX);
	closet.y = clamp(incident.y, -extentY, extentY);
	
	bool inside = false;
	if (incident == closet)
	{
		inside = true;
		if (std::abs(incident.x) > std::abs(incident.y))
		{
			closet.x = (0 < closet.x) ? extentX : -extentX;
		}
		else
		{
			closet.y = (0 < closet.y) ? extentY : -extentY;
		}
	}

	normal = incident - closet;
	float dist = normal.SqrMagnitude();
	float radius = rhs->radius;
	if ((radius * radius) < dist && !inside) return false;

	penetration = radius - std::sqrt(dist);
	if (inside) normal = -normal;
	normal.Normalize();

	return true;
}

bool Collision::CirclevsCircle(Circle* lhs, Circle* rhs)
{
	const Vector2 incident = rhs->position - lhs->position;
	float radiusSum = lhs->radius + rhs->radius;

	// 두 원의 반지름의 합보다 두원 사이의 거리가 크면 충돌하지 않은 상태.
	if (incident.SqrMagnitude() > (radiusSum * radiusSum)) return false;

	float dist = incident.Magnitude();
	if (0 != dist)
	{
		penetration = radiusSum - dist;
		normal = incident;
	}
	else
	{
		penetration = lhs->radius;
		normal = Vector2(1, 0);
	}
	normal.Normalize();

	return true;
}

Scene::~Scene()
{
	Clear();
}

void Scene::Update(float deltaTime)
{
	contacts.clear(); // 지우고 넣는다?
	const UINT count = objects.size();
	for (int i = 0; count > i; i++)
	{
		for (int j = i + 1; count > j; j++)
		{
			GameObject* A = objects[i];
			GameObject* B = objects[j];
			if (0 == A->rigidbody->invMass &&
				0 == B->rigidbody->invMass) continue;

			Collision collision(A, B);
			if (collision.IsCollision()) contacts.push_back(collision);
		}
	}

	for (GameObject* obj : objects) obj->rigidbody->ApplyForces(deltaTime);
	for (int i = 0; iterations > i; i++)
	{
		for (Collision col : contacts) col.Solve();
	}
	for (GameObject* obj : objects) obj->ApplyVelocity(deltaTime);
	for (Collision col : contacts) col.PositionalCorrection();
	for (GameObject* obj : objects) obj->rigidbody->RemoveForce();
}

void Scene::Render(HDC hDC)
{
	for (GameObject* obj : objects)
	{
		obj->shape->Draw(hDC);
	}
}

void Scene::Clear()
{
	contacts.clear();

	for (GameObject* obj : objects) DEL(obj);
	objects.clear();
}