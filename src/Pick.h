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


/*! @file
@author qfel
*/
#pragma once
#include <utility>
#include <vector>

namespace Habanero
{
	template <class Real> class vector3;
	class Spatial;
	class Camera;

	/*!
	@brief Sprawdza czy obiekt koliduje z prostą.
	@param spatial Obiekt którego BV potencjalnie koliduje z prostą.
	@param p Punkt przez który przechodzi prosta.
	@param dir Znormalizowany kierunek prostej.
	@param distance[out] Odległość punktu \a p odd BV zadanego obiektu (pod warunkiem że funkcja zwraca true, mierzona w wektorze \a dir).
	@return true jeśli prosta przecina BV, false w p.p.
	*/
	bool pick(const Spatial *spatial, const vector3<float> &p, const vector3<float> &dir, float *distance);

	/*!
	@brief Wyszukuje obiekty kolidujące z prostą.
	@param spatial Korzeń drzewa obiektów które należy przeszukać.
	@param p Punkt przez który przechodzi prosta.
	@param dir Znormalizowany kierunek prostej.
	@return Wektor par (obietk, odległość), każdy wygenerowany prez wywołanie pick dla konkretnego obiektu.
	*/
	std::vector<std::pair<const Spatial*, float>> pick(const Spatial *spatial, const vector3<float> &p, const vector3<float> &dir);

	/*!
	@brief Wylicza punkty leżące na prostej prostopadłej do kamery.
	@param camera Kamera.
	@param x Znormalizowana do przedziału [0, 1] współrzędna.
	@param y Znormalizowana do przedziału [0, 1] współrzędna.
	@param p[out]
	@param dir[out]
	*/
	void pick(const Camera *camera, float x, float y, vector3<float> *p, vector3<float> *dir);

	std::vector<std::pair<const Spatial*, float>> pick(const Camera *camera, float x, float y);

	bool pick(const Camera *camera, float x, float y, planef plane, vector3f *point);
};