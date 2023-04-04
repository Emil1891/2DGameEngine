#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace engine {
	class Vector 
	{
	public: // Alla aritmetiska operationer är inte skapade 
		Vector(float x, float y) : x(x), y(y) {}
		Vector() {}

		Vector& operator+=(const Vector& other); 
		Vector& operator+=(const float num);

		const Vector operator+(const Vector& other) const;
		const Vector operator+(const float num) const; 

		Vector& operator-=(const Vector& other);
		Vector& operator-=(const float num);

		const Vector operator-(const Vector& other) const;
		const Vector operator-(const float num) const;

		const Vector& operator++(); // Preinc.
		const Vector operator++(int); // Postinc.
		const Vector& operator--(); // Predec.
		const Vector operator--(int); // Postdec. 

		Vector& operator *=(const float num); 
		const Vector& operator*(const float num) const; 

		bool operator==(const Vector& other); 

		Vector normalized() const; 

		float getMagnitude() const; 

		float x = 0, y = 0;
	};

	const Vector operator+(const float num, const Vector& vec);

	std::ostream& operator<<(std::ostream& os, const Vector& vec); 
}

#endif