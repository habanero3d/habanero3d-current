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


#include "Node.h"
#include <cstdio>
#include "assert.h"

namespace Habanero
{
	NodeBase::NodeBase() : parent(NULL), previousSibling(NULL), nextSibling(NULL), firstChild(NULL)
	{
	}

	NodeBase::~NodeBase()
	{
		for (NodeBase *nd = getFirstChild(), *next; nd; nd = next)
		{
			next = nd->getNextSibling();
			delete nd;
		}
	}

	void NodeBase::attach(NodeBase *what)
	{
		what->parent = this;
		what->nextSibling = firstChild;
        if (firstChild)
            firstChild->previousSibling = what;
		what->previousSibling = NULL;
		firstChild = what;
	}

	void NodeBase::detach()
	{
		if (previousSibling)
			previousSibling->nextSibling = nextSibling;
		else if (parent)
			parent->firstChild = nextSibling;
		if (nextSibling)
			nextSibling->previousSibling = previousSibling;
        previousSibling = nextSibling = parent = NULL; //Dodane przez Michauzo, mam nadzieje, że nie bedzie sie przez to wykrzaczać. Mi się to przyda:)
	}

	NodeBase* NodeBase::getParent()
	{
		return parent;
	}

	NodeBase* NodeBase::getPreviousSibling()
	{
		return previousSibling;
	}

	NodeBase* NodeBase::getNextSibling()
	{
		return nextSibling;
	}

	NodeBase* NodeBase::getFirstChild()
	{
		return firstChild;
	}

	const NodeBase* NodeBase::getParent() const
	{
		return parent;
	}

	const NodeBase* NodeBase::getPreviousSibling() const
	{
		return previousSibling;
	}

	const NodeBase* NodeBase::getNextSibling() const
	{
		return nextSibling;
	}

	const NodeBase* NodeBase::getFirstChild() const
	{
		return firstChild;
	}

	//NodeBase * NodeBase::cloneTree(Frustum<float> &frustum)
	//{
	//	if(Type == TSubScene)
	//	{
	//		SubScene *subscene = dynamic_cast<SubScene*>(this);
	//		return subscene->getScene(frustum);
	//	}
	//	if(Type == TCamera)	// kamery się nie rysuje
	//		return nullptr;
	//	if(Type == TSceneObject)	// zwracam swój klon
	//		return clone();

	//	// jestem tu wtw gdy jestem spatialem
	//	Spatial *spatial = dynamic_cast<Spatial*>(this);
	//	Frustum<float> nowe = Frustum<float>(frustum);

	//	// TODO TODO TODO TODO TODO TODO TODO TODO
	//	// przekształcić nowe frustum używając WorldToLocal
	//	// TODO TODO TODO TODO TODO TODO TODO TODO

	//	if(nowe.isCulling(spatial->BoundingVolume))
	//		return nullptr;

	//	NodeBase *child = spatial->firstChild;	// moje pierwsze dziecko
	//	Spatial *cloned = spatial->clone();	// to sklonowany ja
	//	NodeBase *previouslyCloned = nullptr;
	//	while(child != nullptr)
	//	{
	//		NodeBase *clonedChild = child->cloneTree(nowe);	// klonuję dziecko wraz z jego drzewem korzystając z frustry
	//		if(clonedChild != nullptr)
	//		{
	//			clonedChild->parent = cloned;
	//			clonedChild->previousSibling = previouslyCloned;

	//			if(previouslyCloned != nullptr)
	//				previouslyCloned->nextSibling = clonedChild;
	//			previouslyCloned = clonedChild;
	//		}
	//		child = child->nextSibling;
	//	}
	//	return cloned;
	//}
}