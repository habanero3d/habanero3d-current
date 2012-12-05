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

#include <cstdio>

/*!
@def LOG(dst, msg, ...)
@brief Loguje komunikat we wskazane miejsce
@param dst Strumień do którego przekazać sformatowany komunikat
@param msg Format komunikatu
@a msg oraz następujące argumenty są takie same jak dla printf. Komunikat automatycznie poprzedzany jest dodatkowymi
informacjami (np. czasem jego wysłania) i kończony nową linią. Wszystkie komunikaty duplikowane są do strumienia
określanego przez zmienną globalną #globalLogger (o ile jest ona różna od nullptr i @a dst).
Jeśli dst wynosi nullptr makro nie ma efektów ubocznych (argumenty nie są ewaluowane).
Najlepiej jako @a dst używać globalnego wskaźnika. W ten sposób chcąc kompletnie wyłączyć logowanie do określonego
strumienia wystarczy zdefiniowac go jako
@code
IStream * const strumien = nullptr;
@endcode
co pozwoli optymalizatorowi całkowicie wyeliminować kod odpowiedzialny za obsługę LOG(strumien, ...).
*/
#ifndef NO_LOGGING
//#	define LOG(dst, msg, ...) (dst && (log_internal(dst, msg, __VA_ARGS__), true))
#   define LOG(dst, msg, ...) printf(msg, __VA_ARGS__)
#else
#	define LOG(dst, msg, ...)
#endif

namespace Habanero {
	class IStream;
	extern IStream *globalLogger; //!< Strumień (logger) zbiorczy
	void log_internal(IStream *dest, const char *fmt, ...); //!< Funkcja pomocnicza dla makra #LOG(dst, msg, ...)
	void setLoggingFormat(...);

}
