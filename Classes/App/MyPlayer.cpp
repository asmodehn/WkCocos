#include "WkCocosApp/MyPlayer.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr)
: WkCocos::Player<MyPlayer>(localdatamngr)
, m_gem(42)
, m_gold(424242)
{
}

MyPlayer::~MyPlayer()
{}


/**
* data accessor as json string
*/
void MyPlayer::get_data_json(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value currency;
	currency.SetObject();
	currency.AddMember(sGold, m_gold, allocator);
	currency.AddMember(sGem, m_gem, allocator);
	doc.AddMember(sCurrency, currency, allocator);
}

/**
* data setter from json string
*/
void MyPlayer::set_data_json(rapidjson::Document& doc)
{
	if (doc.HasMember(sCurrency))
	{
		rapidjson::Value& currencyvalue = doc[sCurrency];
		if (!currencyvalue.IsNull()){
			if (currencyvalue.HasMember(sGold))
			{
				m_gold = currencyvalue[sGold].GetInt();
			}
			if (currencyvalue.HasMember(sGem))
			{
				m_gem = currencyvalue[sGem].GetInt();
			}
		}
	}
	else //first time there is no save. we need sensible values.
	{
		//these should be set in constructor
	}
	
}
