#include "WkCocosApp/PlayersListUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "json/document.h"  

USING_NS_CC;

const std::string PlayersListUI::id = "players_list";

PlayersListUI::PlayersListUI()
	: Interface()
{
	//filepath is the id for the preload memory ( not used here )
	//m_filepath = id;

	//building UI hierarchy
	m_widget = ui::Layout::create();

	if (m_widget)
	{

		Size visibleSize = Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
		Size widgetSize = m_widget->getContentSize();

		m_loadButton = ui::Button::create("SkipNormal.png", "SkipSelected.png");
		m_loadButton->addTouchEventListener(CC_CALLBACK_2(PlayersListUI::loadCallback, this));
		m_loadButton->setPosition(Vec2(- widgetSize.width / 2, widgetSize.height / 3));
		m_widget->addChild(m_loadButton);

		m_loadLabel = ui::Text::create("LOAD", "Arial", 21);
		m_loadLabel->setPosition(m_loadButton->getPosition() + Vec2(0, m_loadButton->getContentSize().height));
		m_widget->addChild(m_loadLabel);
	
		m_gemLabel = ui::Text::create("? GEM", "Arial", 21);
		m_gemLabel->setPosition(Vec2(0, widgetSize.height / 3));
		m_widget->addChild(m_gemLabel);

		m_goldLabel = ui::Text::create("? GOLD", "Arial", 21);
		m_goldLabel->setPosition(Vec2(0, - widgetSize.height / 3));
		m_widget->addChild(m_goldLabel);
	
		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, ui::Widget*>(id, m_widget));

	}

	GameLogic::Instance().getPlayer().getOnlineDatamgr()->getEventManager()->subscribe<WkCocos::OnlineData::Events::PlayersList>(*this);
	GameLogic::Instance().getPlayer().getAllUsers();
}

PlayersListUI::~PlayersListUI()
{}

void PlayersListUI::update(float delta)
{
	if (m_gemLabel->isVisible())
	{
		m_gemLabel->setText(WkCocos::ToolBox::itoa(GameLogic::Instance().getPlayer().m_gem) + " GEM");
	}
	if (m_goldLabel->isVisible())
	{
		m_goldLabel->setText(WkCocos::ToolBox::itoa(GameLogic::Instance().getPlayer().m_gold) + " GOLD");
	}

}

void PlayersListUI::loadCallback(Ref* widgetRef, ui::Widget::TouchEventType input)
{
	if (input == ui::Widget::TouchEventType::ENDED)
	{
		CCLOG("LOAD BUTTON CLICKED");

		GameLogic::Instance().getPlayer().loadData([](){
		});

	}
}

void PlayersListUI::receive(const WkCocos::OnlineData::Events::PlayersList &pl)
{
	//rapidjson::Document doc;
	//doc.Parse<0>(pl.eventMessage.c_str());
	//if (!doc.HasParseError())
	//{
		//if (doc.Size())
		//{
			//

			//for (rapidjson::SizeType i = 0; i < doc.Size(); i++)
			//{
			//	rapidjson::Value & userName = doc[i];
			//	CCLOG("user number %d is %s\n", i, userName["userName"].GetString());
			//	userName = userName["profile"];

			//	cocos2d::ui::Text* playertextbutton = ui::Text::create("#" + WkCocos::ToolBox::itoa(i) + " " + userName["userName"].GetString(), "Arial", m_widget->getContentSize().height / doc.Size());
			//	playertextbutton->setPosition(Vec2(-m_widget->getContentSize().width / 2, m_widget->getContentSize().height / doc.Size() * i));
			//	m_widget->addChild(playertextbutton);

				/*
				static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
				for (rapidjson::Value::ConstMemberIterator itr = userName.MemberonBegin(); itr != userName.MemberonEnd(); ++itr)
				CCLOG("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
				*/
			//}
		//}
	//}
}