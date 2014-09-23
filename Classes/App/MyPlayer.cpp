#include "WkCocosApp/MyPlayer.h"
//including json from cocos
#include "json/document.h"         // rapidjson's DOM-style API
#include "json/stringbuffer.h"
#include "json/writer.h"

MyPlayer::MyPlayer(std::shared_ptr<WkCocos::LocalData::LocalDataManager> localdatamngr, std::shared_ptr<WkCocos::Shop::Inventory> shopInventory)
: WkCocos::Player(localdatamngr, shopInventory, WkCocos::Save::Mode::ONLINE)
, m_gem("53cr3t") // encrypted
, m_gold() // not encrypted
{
	m_gem.set(42);
	m_gold.set(424242);
}

MyPlayer::~MyPlayer()
{
}


/**
* data accessor as json string
*/
std::string MyPlayer::get_data_json()
{
	rapidjson::Document doc;
	doc.SetObject();
	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	//rapidjson::Value currency;
	//currency.SetObject();
	//currency.AddMember(sGold, m_gold.get<int>(), allocator);
	//currency.AddMember(sGem, m_gem.get<int>(), allocator);
	//doc.AddMember(sCurrency, currency, allocator);
	doc.AddMember(sGold, m_gold.get<int>(), allocator);
	doc.AddMember(sGem, m_gem.get<int>(), allocator);

	//TMP debug
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	doc.Accept(writer);

	return std::string(strbuf.GetString());
}

/**
* data setter from json string
*/
void MyPlayer::set_data_json(std::string data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (doc.HasParseError()) 
	{
		//if parse error (also empty string), we ignore existing data.
		doc.SetObject();
	}
	
	/*if (doc.HasMember(sCurrency))
	{
		rapidjson::Value& currencyvalue = doc[sCurrency];
		if (!currencyvalue.IsNull()){
			if (currencyvalue.HasMember(sGold) && currencyvalue[sGold].IsInt())
			{
				m_gold.set<int>(currencyvalue[sGold].GetInt());
			}
			if (currencyvalue.HasMember(sGem) && currencyvalue[sGem].IsInt())
			{
				m_gem.set<int>(currencyvalue[sGem].GetInt());
			}
		}
	}*/

	if (doc.HasMember(sGold) && doc[sGold].IsInt())
	{
		m_gold.set<int>(doc[sGold].GetInt());
	}
	if (doc.HasMember(sGem) && doc[sGem].IsInt())
	{
		m_gem.set<int>(doc[sGem].GetInt());
	}

}
