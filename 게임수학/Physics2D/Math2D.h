#ifndef MATH2D_H
#define MATH2D_H

#include <cmath>

const float PI = 3.141592741f;
const float epsilon = 0.0001f;
const float rad2Deg = 57.29577951f;
const float deg2Rad = 0.017453293f;

struct Vector2
{
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}

	Vector2 operator-() const { return Vector2(-x, -y); }

	Vector2 operator*(float value) const { return Vector2(x * value, y * value); }
	Vector2 operator/(float value) const { return Vector2(x / value, y / value); }
	void operator*=(float value) { x *= value; y *= value; }
	void operator/=(float value) { x /= value; y /= value; }

	Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	void operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; }
	void operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; }

	bool operator==(const Vector2& rhs) { return (x == rhs.x && y == rhs.y); }
	bool operator!=(const Vector2& rhs) { return !(x == rhs.x && y == rhs.y); }
	
	float SqrMagnitude() const { return x * x + y * y; }
	float Magnitude() const { return std::sqrt(x * x + y * y); }
	Vector2 Normalize()
	{
		float magnitude = Magnitude();
		if (epsilon < magnitude)
		{
			float invMagnitude = 1.0f / magnitude;
			x *= invMagnitude;
			y *= invMagnitude;
		}

		return Vector2(x, y);
	}

	static float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
	static float Distance(const Vector2& lhs, const Vector2& rhs) { Vector2 retval = rhs - lhs;  return retval.Magnitude(); }
	static float SqrDistance(const Vector2& lhs, const Vector2& rhs) { Vector2 retval = rhs - lhs;  return retval.SqrMagnitude(); }
	// ����纯���� ����.
	static float Cross(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.y - lhs.y * rhs.x; }
};
inline bool operator== (const Vector2& lhs, const Vector2& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y); }
inline Vector2 operator*(float lhs, const Vector2& rhs) { return Vector2(lhs * rhs.x, lhs * rhs.y); }
inline bool Equal(float lhs, float rhs) { return (fabs(lhs - rhs) <= epsilon); }
const float gravityScale = 9.80665f;
const Vector2 gravity(0, gravityScale);
const float slope = 0.05f;	// 0.01~0.1
const float percent = 0.4f;	// 20%~80%

inline float clamp(float value, float min, float max)
{
	if (min > max)
	{
		float tmp = max;
		max = min;
		min = tmp;
	}

	return std::fmin(std::fmax(value, min), max);
}
// ����(mass) = ����(volume) * �е�(density)
// ���(N �� s : momentum) = ����(mass) * ���ӵ�(velocity)
// 
// ���ӵ� = ��ü�� �������� ��(impulse) * ������(inv_mass : (1 / ����(mass)))
// �ݹ߷�(reflection impulse) = Dot(��� �ӷ�(relative velocity), �浹���� ��������(normal))
// ��� �ӵ�(relative velocity) = ����� �ӵ�(other velocity) - ���� �ӵ�(velocity)
// �浹���� ��������(normal) = ���� ���� �� dot(��� �ӷ�(relative velocity), ��������(normal)) loop
//
// ��ü ������ ħ���� �������� ó��.
// ����(correction) = ������ ħ�� ����(penetration depth) / (�� ��ü�� ������(inv_mass)�� ��) * ��������(normal) * ����ġ(percent : 20%~80%)
// ������ ħ�� ����(penetration depth) = max(ħ�� ����(penetration depth) - ����ġ(slop : 0.01~0.1), 0)
// ������ �ִ� ��� ������� ������ �߻��ϱ� ������ ħ�� ���̸� �����Ͽ� ����ġ ������ ���̴� �����ϵ��� �Ѵ�.

#endif // !MATH2D_H