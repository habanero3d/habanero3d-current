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


/*!
@file
@brief Generyczne implementacje co prostszych algorytmów
@author qfel
*/
#pragma once
#include <iterator>

#include "Enum.h"

namespace Habanero
{
	ENUM(Ordering, Greater, Less, Equal);

	/*!
	@brief Wyszukiwanie binarne - dokładny wynik
	@param begin Początek zakresu elementów do przeszukania
	@param end Koniec zakresu elementów do przeszukania
	@param predicate Funktor lub funkcja unarna, określająca czy szukana wartość jest mniejsza,
		większa lub równa argumentowi (element zakresu [begin, end))
	@return Znaleziona wartość lub end
	@remarks Ta implementacja nie jest aż ak ogólna jak std::lower_bound jeżeli chodzi o obsługe wszelkich iteratorów,
		ale powinna w zupełności wystarczyć.
	predicate jest typu zgodnego z Ordering (*)(const T &elem)
	*/
	template <typename FwdIt, typename Pr>
	FwdIt binarySearch(FwdIt begin, FwdIt end, Pr predicate)
	{
		FwdIt notFound = end, mid;
		while (begin != end)
		{
			mid = begin + (end - begin) / 2u; // Dzielenie bez znaku - szybsze
			switch (predicate(*mid))
			{
				case Ordering::Less: end = mid; break;
				case Ordering::Greater: begin = mid + 1; break;
				case Ordering::Equal: return mid;
				default: HASSERT(false); // Optymalizacja
			}
		}
		return notFound;
	}

	/*!
	@brief Wyszukiwanie binarne - pierwszy niemniejszy
	@param begin Początek zakresu elementów do przeszukania
	@param end Koniec zakresu elementów do przeszukania
	@param predicate Funktor lub funkcja unarna, określająca czy szukana wartość jest mniejsza,
		większa lub równa argumentowi (element zakresu [begin, end))
	@return Pierwszy niemniejszy od szukanej wartości, lub end
	@remarks Ta implementacja nie jest aż ak ogólna jak std::lower_bound jeżeli chodzi o obsługe wszelkich iteratorów,
		ale powinna w zupełności wystarczyć.
	predicate jest typu zgodnego z Ordering (*)(const T &elem).
	*/
	template <typename FwdIt, typename Pr>
	FwdIt binarySearchLower(FwdIt begin, FwdIt end, Pr predicate)
	{
		FwdIt notFound = end, mid;
		while (begin != end)
		{
			mid = begin + (end - begin) / 2u; // Dzielenie bez znaku - szybsze
			switch (predicate(*mid))
			{
				case Ordering::Equal:
				case Ordering::Less:
					end = mid;
					break;
				case Ordering::Greater:
					begin = mid + 1;
					break;
				default: HASSERT(false); // Optymalizacja
			}
		}
		return end;
	}
}