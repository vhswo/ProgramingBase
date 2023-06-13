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
	// 평행사변형의 넓이.
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
// 질량(mass) = 부피(volume) * 밀도(density)
// 운동량(N · s : momentum) = 질량(mass) * 가속도(velocity)
// 
// 가속도 = 물체에 가해지는 힘(impulse) * 역질량(inv_mass : (1 / 질량(mass)))
// 반발력(reflection impulse) = Dot(상대 속력(relative velocity), 충돌면의 법선벡터(normal))
// 상대 속도(relative velocity) = 상대의 속도(other velocity) - 나의 속도(velocity)
// 충돌면의 법선벡터(normal) = 가장 작은 값 dot(상대 속력(relative velocity), 법선벡터(normal)) loop
//
// 물체 끼리의 침투를 막기위한 처리.
// 보정(correction) = 보정된 침투 깊이(penetration depth) / (두 물체의 역질략(inv_mass)의 합) * 법선벡터(normal) * 보정치(percent : 20%~80%)
// 보전된 침투 깊이(penetration depth) = max(침투 깊이(penetration depth) - 보정치(slop : 0.01~0.1), 0)
// 가만히 있는 경우 계속적인 떨림이 발생하기 때문에 침투 깊이를 보정하여 보정치 정도의 깊이는 무시하도록 한다.

#endif // !MATH2D_H