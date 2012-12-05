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

// dekompozycja PA=LU

//author: akuda

#include "LUP.h"
#include "vector3.h"
#include "cstring"
#include "HabMath.h"
#include "RT.h"

//źródła: http://www.gamedev.net/page/resources/_/technical/game-programming/shadow-caster-volumes-for-the-culling-of-potential-shadow-casters-r2330
// oraz Cormen

namespace Habanero
{

	template <typename Real, int SIZE>
	LUP<Real, SIZE>::LUP(const Real * m)
	{
		for (uint i = 0; i < SIZE; ++i)
			P[i] = i;

		memcpy(&LU[0], m, sizeof(Real) * SIZE);

		for (uint k = 0; k < SIZE; ++k)
		{
			// szukam elementu głównego

			Real _abs = 0.;
			uint maxRow;
			for (uint i = k; i < SIZE; ++i)
			{
				Real _tmp_abs = std::fabs(LU[i * SIZE + k]);
				if (_tmp_abs > _abs)
				{
					_abs = _tmp_abs;
					maxRow = i;
				}	
			}
			HASSERT(_abs != 0.f);
			
			//zamieniam miejscami
			std::swap(P[k], P[maxRow]);
			swapLURows(k, maxRow);

			//dekompozycja (odejmuje uzupełnienie Schura)

			Real mkk = LU[k * SIZE + k];
			for (uint i = k + 1; i < SIZE; ++i)
			{
				LU[i * SIZE + k] /= mkk;
				Real mik = LU[i * SIZE + k];
				for (uint j = k + 1; j < SIZE; ++j)
				{
					LU[i * SIZE + j] -= (mik * LU[k * SIZE + j]);
				}
			}

		}
	}

	template<typename Real, int SIZE>
	void LUP<Real, SIZE>::solve(const Real * d, Real * out) const
	{
		for (uint i = 0; i < SIZE; ++i)
		{
			Real sum = 0.;
			for (uint j = 0; j < i; ++j)
			{
				sum += LU[i * SIZE + j] * out[j];
			}
			out[i] = d[P[i]] - sum;
		}
		
		for (uint i = SIZE - 1; i >= 0; --i)
		{
			Real sum = 0.;
			for (int j = i + 1; j < SIZE; ++j)
			{
				sum += LU[i+SIZE + j] * out[j];
			}
			out[i] = (out[i] - sum) / LU[i*i];
		}
	}

	template <typename Real, int SIZE>
	inline void LUP<Real, SIZE>::swapLURows(uint row1, uint row2)
	{
		Real _tmp[SIZE];
		memcpy(&_tmp[0], &LU[row1 * SIZE], sizeof (Real) * SIZE);
		memcpy(&LU[row1 * SIZE], &LU[row2 * SIZE], sizeof (Real) * SIZE);
		memcpy(&LU[row2 * SIZE], &_tmp[0], sizeof (Real) * SIZE);
	}
	
	template class LUP<float, 4>;
	template class LUP<double, 4>;
}