#pragma once
#include "Model.h"
#include "Spatial.h"

namespace Habanero
{
	namespace ModelLoading
	{
		class ModelFactory
		{
		public:
			static Spatial *GetModel(const Model &model);
		};
	}
}