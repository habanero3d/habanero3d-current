/*
Copyright 2009-2012 Andrzej Skalski, Piotr Kufel, Piotr Białecki, Michał Żochowski, and Michał Szczepaniak
This file is part of Habanero3d.

Habanero3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Habanero3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Habanero3d.  If not, see <http://www.gnu.org/licenses/>.


*/


#include "HabMath.h"
#include "vector3.h"

namespace Habanero
{
	template <typename Real>
	const vector3<Real> vector3<Real>::zero(0.0, 0.0, 0.0);
	
	template <typename Real>
	const vector3<Real> vector3<Real>::nx(1.0, 0.0, 0.0);
	
	template <typename Real>
	const vector3<Real> vector3<Real>::ny(0.0, 1.0, 0.0);
	
	template <typename Real>
	const vector3<Real> vector3<Real>::nz(0.0, 0.0, 1.0);

	template <typename Real>
	vector3<Real>::vector3(Real x, Real y, Real z) : 
		x(x),
		y(y),
		z(z)
	{
	}

	template <typename Real>
	vector3<Real>::vector3(const Real *v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	template <typename Real>
	void vector3<Real>::normalize()
	{
		Real len = length();
		//HASSERT(!isAbout<Real>(len, 0.0));
		x /= len;
		y /= len;
		z /= len;
	}

	template <typename Real>
	vector3<Real> vector3<Real>::normalized() const
	{
		vector3<Real> v = *this;
		v.normalize();
		return v;
	}

	template <typename Real>
	vector3<Real> vector3<Real>::operator- () const
	{
		return vector3<Real>(-x, -y, -z);
	}

	template <typename Real>
	vector3<Real> vector3<Real>::operator/ (Real v) const
	{
		return vector3<Real>(x/v, y/v, z/v);
	};

	template <typename Real>
	vector3<Real>& vector3<Real>::operator+= (const vector3<Real> &vector)
	{
		x+= vector.x;
		y+= vector.y;
		z+= vector.z;
		return *this;
	};

	template <typename Real>
	vector3<Real>& vector3<Real>::operator-= (const vector3<Real> &vector)
	{
		x-= vector.x;
		y-= vector.y;
		z-= vector.z;
		return *this;
	};

	template <typename Real>
	vector3<Real>& vector3<Real>::operator*= (Real v)
	{
		x*= v;
		y*= v;
		z*= v;
		return *this;
	};


	template <typename Real>
	vector3<Real>& vector3<Real>::operator/= (Real v)
	{
		x/= v;
		y/= v;
		z/= v;
		return *this;
	};

	template <typename Real>
	Real& vector3<Real>::operator[] (const int index)
	{
		return *( ((Real*)this) + index);
	}
	
	template <typename Real>
	Real vector3<Real>::operator[] (const int index) const
	{
		return *( ((Real*)this) + index);
	}
	
	
	template <typename Real>
	bool vector3<Real>::operator< (const vector3<Real> &that) const
	{
		if(x < that.x) return true;
		if(x > that.x) return false;
		if(y < that.y) return true;
		if(y > that.y) return false;
		if(z < that.z) return true;
		if(z > that.z) return false;
		return false;
	}
	
	
	template <typename Real>
	Real vector3<Real>::length() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	
			template <typename Real>
	std::string vector3<Real>::toString() const
	{
		char buffer[100];
		sprintf(buffer, "v3: [%f\t%f\t%f]", x,y,z);
		return std::string(buffer);
	}
	
	

	template <typename Real>
	vector3<Real> operator+ (const vector3<Real> &l, const vector3<Real> &r)
	{
		return vector3<Real>(l.x + r.x, l.y + r.y, l.z + r.z);
	}

	template <typename Real>
	vector3<Real> operator- (const vector3<Real> &l, const vector3<Real> &r)
	{
		return vector3<Real>(l.x - r.x, l.y - r.y, l.z - r.z);
	}

	template <typename Real>
	vector3<Real> operator* (const vector3<Real> &l, Real r)
	{
		return vector3<Real>(l.x * r, l.y * r, l.z * r);
	}

	template <typename Real>
	vector3<Real> operator* (Real l, const vector3<Real> &r)
	{
		return r * l;
	}

	template <typename Real>
	vector3<Real> cross(const vector3<Real> &a, const vector3<Real> &b)
	{
		return vector3<Real>(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x);
	}

	template <typename Real>
	Real dot(const vector3<Real> &a, const vector3<Real> &b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}


	
	template class vector3<float>;
	
	template vector3<float> operator+ (const vector3<float> &l, const vector3<float> &r);
	template vector3<float> operator- (const vector3<float> &l, const vector3<float> &r);
	template vector3<float> operator* (const vector3<float> &l, float r);
	template vector3<float> operator* (float l, const vector3<float> &r);
	template vector3<float> cross(const vector3<float> &a, const vector3<float> &b);
	template float dot(const vector3<float> &a, const vector3<float> &b);
        
	template class vector3<double>;
	
	template vector3<double> operator+ (const vector3<double> &l, const vector3<double> &r);
	template vector3<double> operator- (const vector3<double> &l, const vector3<double> &r);
	template vector3<double> operator* (const vector3<double> &l, double r);
	template vector3<double> operator* (double l, const vector3<double> &r);
	template vector3<double> cross(const vector3<double> &a, const vector3<double> &b);
	template double dot(const vector3<double> &a, const vector3<double> &b);
        
}
