#include "Vector.h"
#include <cmath>

namespace engine {

    Vector& engine::Vector::operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y; 
        return *this; 
    }

    Vector& Vector::operator+=(const float num)
    {
        Vector temp(num, num);
        return *this += temp;
    }

    const Vector Vector::operator+(const Vector& other) const 
    {
        Vector temp(*this);
        temp += other;
        return temp; 
    }

    const Vector Vector::operator+(const float num) const 
    {
        Vector temp(num, num);
        return temp += *this; 
    }

    Vector& Vector::operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& Vector::operator-=(const float num)
    {
        Vector temp(num, num);
        return *this -= temp; 
    }

    const Vector Vector::operator-(const Vector& other)  const 
    {
        Vector temp(*this);
        temp -= other;
        return temp;
    }

	const Vector Vector::operator-(const float num) const 
	{
		Vector temp(num, num);
		return temp -= *this;
	}

    const Vector& Vector::operator++() // Pre
    {
        return *this += 1; 
    }

    const Vector Vector::operator++(int) // Post
    {
        Vector temp(*this);
        ++(*this);
        return temp; 
    }

    const Vector& Vector::operator--() // Pre
    {
        return *this -= 1; 
    }

    const Vector Vector::operator--(int) // Post
    {
        Vector temp(*this);
        --(*this);
        return temp;
    }

    Vector& Vector::operator*=(const float num)
    {
        x *= num;
        y *= num;
        return *this; 
    }

    const Vector& Vector::operator*(const float num) const
    {
        Vector temp(*this);
        return temp *= num; 
    }

    bool Vector::operator==(const Vector& other)
    {
        return x == other.x && y == other.y;
    }

    float Vector::getMagnitude() const {
        return sqrt(this->x * this->x + this->y * this->y); // Pythagoras 
    }

    Vector Vector::normalized() const
    {
        // Math: https://www.khanacademy.org/computing/computer-programming/programming-natural-simulations/programming-vectors/a/vector-magnitude-normalization
        float magnitude = getMagnitude(); // Magnitude = l�ngden p� vectorn (hypotenusan)
        if (magnitude > 0) { // Kan inte dividera med 0
            Vector norm(*this);
            norm.x /= magnitude;
            norm.y /= magnitude; // L�ngden/magnituden �r nu 1
            return norm; 
        }
        else {
            return Vector(0, 0);
        }
    }

    const Vector operator+(const float num, const Vector& vec) 
    {
        Vector temp(vec);
        return temp + num;
    }

    std::ostream& operator<<(std::ostream& os, const Vector& vec)
    {
        os << "x: " << vec.x << " y: " << vec.y;
        return os; 
    }

}