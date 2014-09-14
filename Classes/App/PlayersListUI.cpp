#include "WkCocosApp/PlayersListUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "json/document.h"  

const std::string PlayersListUI::id = "players_list";

PlayersListUI::PlayersListUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	//m_filepath = id;

	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width / 2, visibleSize.height / 2));
		m_widgetSize = m_widget->getContentSize();

		m_refreshButton = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_refreshButton->addTouchEventListener(CC_CALLBACK_2(PlayersListUI::refreshCallback, this));
		m_refreshButton->setPosition(cocos2d::Vec2(m_widgetSize.width / 2, m_widgetSize.height / 3 * 2));
		m_widget->addChild(m_refreshButton);

		m_refreshLabel = cocos2d::ui::Text::create("reload players list", "Arial", 21);
		m_refreshLabel->setPosition(m_refreshButton->getPosition() + cocos2d::Vec2(0, m_refreshButton->getContentSize().height));
		m_widget->addChild(m_refreshLabel);
	
		m_enemyData = cocos2d::ui::Text::create("... GEMS and ... GOLD", "Arial", 21);
		m_enemyData->setPosition(cocos2d::Vec2(m_widgetSize.width / 2, m_widgetSize.height / 3));
		m_widget->addChild(m_enemyData);

		m_enemyLabel = cocos2d::ui::Text::create("player ... has", "Arial", 15);
		m_enemyLabel->setPosition(m_enemyData->getPosition() + cocos2d::Vec2(0, m_enemyData->getContentSize().height));
		m_widget->addChild(m_enemyLabel);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));

	}

	GameLogic::Instance().getPlayer().getOnlineDatamgr()->getEventManager()->subscribe<WkCocos::OnlineData::Events::PlayersList>(*this);
	GameLogic::Instance().getPlayer().getUsersWithDocs();
}

PlayersListUI::~PlayersListUI()
{
	std::map<std::string, cocos2d::ui::Text*>::iterator currentPTB = m_ptb.begin();
	for (; currentPTB != m_ptb.end(); ++currentPTB)
	{
		m_widget->removeChild(currentPTB->second);
		//delete currentPTB->second;
	}
	m_ptb.clear();
}

void PlayersListUI::refreshCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{	
	if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
	{	
		std::map<std::string, cocos2d::ui::Text*>::iterator currentPTB = m_ptb.begin();
		for (; currentPTB != m_ptb.end(); ++currentPTB)
		{
			m_widget->removeChild(currentPTB->second);
			//delete currentPTB->second;
		}
		m_ptb.clear();

		GameLogic::Instance().getPlayer().getUsersWithDocs();
	}
}

void PlayersListUI::receive(const WkCocos::OnlineData::Events::PlayersList &pl)
{
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
	{
		rapidjson::Document doc;
		doc.Parse<0>(pl.eventMessage.c_str());
		if (!doc.HasParseError())
		{
			if (doc.Size())
			{
				// with this value font size is no more than 15 and player name does not exceed half screen width
				//unsigned int listSize = 42; 
				// leaving lower half screen free for navui
				unsigned int listSize = 21; 

				if (doc.Size() < listSize)
					listSize = doc.Size();

				for (rapidjson::SizeType i = 0; i < listSize; i++)
				{
					rapidjson::Value & part = doc[i];

					std::string enemy_name = part["owner"].GetString();

					cocos2d::ui::Text* playertextbutton = cocos2d::ui::Text::create("#" + WkCocos::ToolBox::itoa(i) + " " + enemy_name, "Arial", 15);
					playertextbutton->setPosition(cocos2d::Vec2(-m_widgetSize.width / 2, m_widgetSize.height * (1 - /*2.0*/ 1.0 / (listSize + 1) * (i + 1))));
					playertextbutton->setTouchEnabled(true);

					playertextbutton->addTouchEventListener
					(
						[=](cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
						{
							//if (input == cocos2d::ui::Widget::TouchEventType::ENDED)
								//get data;
						}
					);

					m_ptb[enemy_name] = playertextbutton;

					m_widget->addChild(playertextbutton);
				}
			}
		}
	});
}