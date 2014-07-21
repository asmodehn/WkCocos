#ifndef __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__
#define __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__

#include "entityx/entityx.h"

#include "cocos/cocos2d.h"

namespace WkCocos
{
	namespace Loading
	{
		
		namespace Comp
		{
			struct LoadSyncFunc : entityx::Component<LoadSyncFunc> {
				static const unsigned short load_priority_max = 100;
				static const unsigned short load_priority_min = 0;

				LoadSyncFunc(std::string filepath, int apriority, std::function<void()> af)
					: m_filepath(filepath)
					, m_loadfunction(af)
					, m_priority(apriority)
				{}

				inline std::string getFilepath() const
				{
					return m_filepath;
				}
				inline std::function<void()> getLoadfunction() const
				{
					return m_loadfunction;
				}
				inline unsigned short getPriority() const
				{
					return m_priority;
				}
			private:
				std::string m_filepath;
				std::function<void()> m_loadfunction;
				//priority to determine the position in the queue of loading function
				unsigned short m_priority;
			};

			struct LoadASyncFunc : entityx::Component<LoadASyncFunc> {
				static const unsigned short load_priority_max = 100;
				static const unsigned short load_priority_min = 0;

				LoadASyncFunc(std::string filepath, int apriority, std::function<void(std::function<void(cocos2d::Texture2D*)> endcb)> af)
					: m_filepath(filepath)
					, m_loadfunction(af)
					, m_priority(apriority)
					, m_inprogress(false) 
				{}

				inline std::string getFilepath() const
				{
					return m_filepath;
				}
				inline std::function<void(std::function<void(cocos2d::Texture2D*)> endcb)> getLoadfunction() const
				{
					return m_loadfunction;
				}
				inline unsigned short getPriority() const
				{
					return m_priority;
				}
				inline bool getInprogress() const
				{
					return m_inprogress;
				}
				inline void setInprogress(bool p)
				{
					m_inprogress = p;
				}
			private:
				std::string m_filepath;
				std::function<void(std::function<void(cocos2d::Texture2D*)> endcb)> m_loadfunction;
				//priority to determine the position in the queue of loading function
				unsigned short m_priority;
				//marker to set while the loadfunction is running;
				bool m_inprogress;
			};

		}//namespace Comp
	}//namespace Loading
}//namespace WkCocos

#endif // __DFGAME_LOADING_ENTITY_COMP_LOADFUNC_H__
