#ifndef __DFGAME_PRELOAD_ENTITY_COMP_PROGRESSVALUE_H__
#define __DFGAME_PRELOAD_ENTITY_COMP_PROGRESSVALUE_H__

#include "entityx/entityx.h"

namespace WkCocos
{
	namespace Preload
	{
		namespace Comp
		{

			struct ProgressValue : entityx::Component<ProgressValue> {
				ProgressValue(unsigned short a_progval) : progval(a_progval) {}

				unsigned short progval;
			};

		}//namespace Comp
	}//namespace Preload
}//namespace WkCocos

#endif // __DFGAME_PRELOAD_ENTITY_COMP_PROGRESSVALUE_H__
