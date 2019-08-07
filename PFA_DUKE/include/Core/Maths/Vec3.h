#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>
#include <string>

namespace Core::Maths
{
	struct Vec3
	{
		float	x;
		float	y;
		float	z;

		/* constructor */
		Vec3(float pos = 0);
		Vec3(float poX, float posY, float posZ);
		Vec3(Vec3 point1, Vec3 point2);
		Vec3(const Vec3& vec);
		Vec3(Vec3&& vec);

		/* destructor */
		~Vec3() = default;

		/* direction vectors */
		static Vec3	zero;
		static Vec3	up;
		static Vec3	down;
		static Vec3	left;
		static Vec3	right;
		static Vec3	forward;
		static Vec3	backward;

		/* math operators */
		/* ============== */
		Vec3&		operator=(const Vec3&);
		Vec3&		operator++(int);
		Vec3&		operator--(int);
		/* dot product */
		float		operator,(const Vec3& vect) const;
		/* vectorial product */
		Vec3		operator^(const Vec3& v) const;

		Vec3&		operator/=(const float& nb);
		Vec3&		operator*=(const float& nb);
		Vec3&		operator+=(const Vec3& vect);
		Vec3&		operator-=(const Vec3& vect);

		Vec3		operator+(const Vec3& vect) const;

		Vec3		operator-(const Vec3& vect);
		Vec3		operator-() const;

		Vec3		operator*(const float& nb) const;

		Vec3		operator/(const float& nb) const;
		/* ============== */

		/* binary operators */
		/* ================ */
		bool		operator==(const Vec3& vect) const;
		bool		operator!=(const Vec3& vect) const;
		bool		operator<(const Vec3& vect) const;
		bool		operator<=(const Vec3& vect) const;
		bool		operator>(const Vec3& vect) const;
		bool		operator>=(const Vec3& vect) const;
		/* ================ */

		/* vector length */
		float		Length() const;
		float		SqrLength() const;

		Vec3&		Scale(const float& nb);
		Vec3		Scale(const float& nb) const;

		Vec3		Normalize() const;
		Vec3&		Normalize();

		float		Dot(const Vec3& vect) const;
		Vec3		Cross(const Vec3& v) const;

		Vec3		Lerp(const Vec3& v, const float& t);
		bool		Nearest(const Vec3& v1, const Vec3& v2) const;
		bool		Collinear(const Vec3& v) const;

		/* to debug */
		std::string	ToString() const;
	};

	/* operators to display a vector */
	std::ostream&	operator<<(std::ostream& os, const Vec3& vect);

	std::string		operator+(std::string& str, const Vec3& vect);

	/* multiplication and division */
	Vec3			operator*(const float& nb, const Vec3& vect);
	Vec3			operator-(const Vec3& vec1, const Vec3& vec2);

	float			AngleBetweenVector(const Vec3& a, const Vec3& b);
}

#endif //_VEC3_H_
