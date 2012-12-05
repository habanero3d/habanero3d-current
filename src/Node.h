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


#pragma once
#include "Enum.h"

namespace Habanero
{
	template <typename Real> class Frustum;

	// Porzadny enum o nazwie NodeType i wartosciach Spatial, ..., SceneObject
	ENUM(NodeType, Spatial, SubScene, Camera, SceneObject);
	
	class NodeBase
	{
	private:
		NodeBase(const NodeBase&);
		void operator =(const NodeBase&);

	protected:
		NodeBase *parent,
		     *previousSibling,
		     *nextSibling,
		     *firstChild;

		NodeBase();

		void attach(NodeBase *what);

		NodeBase* getParent();
		NodeBase* getPreviousSibling();
		NodeBase* getNextSibling();
		NodeBase* getFirstChild();

		const NodeBase* getParent() const;
		const NodeBase* getPreviousSibling() const;
		const NodeBase* getNextSibling() const;
		const NodeBase* getFirstChild() const;

	public:
		/*!
			@brief Niszczy poddrzewo
			@remarks Nie usuwa się automatycznie z drzewa (nie wykonuje detach)
		*/
		virtual ~NodeBase();

		void detach();
	};

	template <typename T>
	class Node : public NodeBase
	{
	public:
		void attach(T *what) { NodeBase::attach(what); }

		T* getParent() { return static_cast<T*>(parent); }
		T* getPreviousSibling() { return static_cast<T*>(previousSibling); }
		T* getNextSibling() { return static_cast<T*>(nextSibling); }
		T* getFirstChild() { return static_cast<T*>(firstChild); }

		const T* getParent() const { return static_cast<T*>(parent); }
		const T* getPreviousSibling() const { return static_cast<T*>(previousSibling); }
		const T* getNextSibling() const { return static_cast<T*>(nextSibling); }
		const T* getFirstChild() const { return static_cast<T*>(firstChild); }
	};
}
