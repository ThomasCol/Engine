#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>
#include <string>

#include "Vec3.h"

namespace Core::Maths
{
	struct Vec4
	{
		float	x;
		float	y;
		float	z;
		float	w;

		/* constructor */
		Vec4(float posX, float posY, float posZ, float posW);
		Vec4(const Vec3& vec, float posW);
		Vec4(float pos = 0);
		Vec4(const Vec4& vec);
		Vec4(Vec4&& vec) = default;

		/* destructor */
		~Vec4();

		/* direction vectors */
		static Vec4	zero;
		static Vec4	up;
		static Vec4	down;
		static Vec4	left;
		static Vec4	right;
		static Vec4	forward;
		static Vec4	backward;

		/* math operators */
		/* ============== */
		Vec4&		operator=(const Vec4&);
		Vec4		operator++(int);
		Vec4		operator--(int);
		/* dot product */
		float		operator,(const Vec4& vect) const;
		/* vectorial product */
		Vec4		operator^(const Vec4& v) const;

		Vec4&		operator/=(float nb);
		Vec4&		operator*=(float nb);
		Vec4&		operator+=(const Vec4& vect);
		Vec4&		operator-=(const Vec4& vect);

		Vec4		operator+(const Vec4& vect);

		Vec4		operator-(const Vec4& vect);
		Vec4		operator-();

		Vec4		operator*(float nb);

		Vec4		operator/(float nb);
		/* ============== */

		/* binary operators */
		/* ================ */
		bool		operator==(const Vec4& vect);
		bool		operator!=(const Vec4& vect);
		bool		operator<(const Vec4& vect);
		bool		operator<=(const Vec4& vect);
		bool		operator>(const Vec4& vect);
		bool		operator>=(const Vec4& vect);
		/* ================ */

		/* vector length */
		float		Length() const;
		float		SqrLength() const;

		Vec4&		Scale(float nb);
		Vec4		Scale(float nb) const;

		Vec4		Normalize() const;
		Vec4&		Normalize();

		void 		Homogenize();

		/* to debug */
		std::string	ToString() const;
	};
	/* operators to display a vector */
	std::ostream&	operator<<(std::ostream& os, const Vec4& vect);

	std::string		operator+(std::string& str, const Vec4& vect);

	/* multiplication and division */
	Vec4			operator*(float nb, const Vec4& vect);
	Vec4			operator/(float nb, const Vec4& vect);
}

#endif //_VEC4_H_
