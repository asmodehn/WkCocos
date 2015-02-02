#include "WkCocosApp/PreloadUI.h"

#include "WkCocosApp/GameLogic.h"
#include "WkCocos/Utils/ToolBox.h"

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

const std::string PreloadUI::id = "preload";

PreloadUI::PreloadUI()
	: Interface()
	, m_plStarted(false)
	, m_plmgr(new WkCocos::Preload::Preload(1,std::function<void(float)>()))
{
	//building UI hierarchy
	m_widget = cocos2d::ui::Layout::create();

	if (m_widget)
	{
		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_widget->setContentSize(cocos2d::Size(visibleSize.width, visibleSize.height - 80)); //upper + lower lines of buttons
		cocos2d::Size widgetSize = m_widget->getContentSize();

		//loadingbar
		auto loadingbar = cocos2d::ui::LoadingBar::create("loadingbar.png", 0.5f);
		loadingbar->setName("LoadingBar");
		m_widget->addChild(loadingbar);

		//BIG button to start preload
        auto dlLabel = cocos2d::ui::Text::create("Preload", "fonts/Marker Felt.ttf", 21);
        auto button = cocos2d::ui::Button::create("SkipNormal.png", "SkipSelected.png");
		button->addTouchEventListener(CC_CALLBACK_2(PreloadUI::PLCallback, this));
		button->setPosition(cocos2d::Vec2(button->getContentSize().width / 2, -button->getContentSize().height / 2));
        dlLabel->setPosition(button->getPosition() + cocos2d::Vec2(0, button->getContentSize().height));
        m_widget->addChild(button);

        //Image view to check the image we have before preload ( TODO : add timer for performance ? )
		sprite = cocos2d::ui::ImageView::create("HelloWorld.png");
		sprite->setScaleX(widgetSize.width / sprite->getContentSize().width);
		sprite->setScaleY(widgetSize.height / sprite->getContentSize().height);
		sprite->setVisible(true);
		m_widget->addChild(sprite);

		m_widget->retain(); //we need to retain it in memory ( or cocos will drop it )
		widget_cache.insert(std::pair<std::string, cocos2d::ui::Widget*>(id, m_widget));
	}
}

PreloadUI::~PreloadUI()
{
	if (m_plmgr)
		delete m_plmgr, m_plmgr = nullptr;
}

void PreloadUI::PLCallback(cocos2d::Ref* widgetRef, cocos2d::ui::Widget::TouchEventType input)
{
    if (m_plmgr)
    {
        m_plmgr->addDataLoad("HelloWorld.png",{});
        m_plStarted = true;
    }
}

void PreloadUI::update(float delta)
{
    //update should do something only if the interface is visible
    if ( isVisible() )
    {
        if ( m_plmgr && m_plStarted )
        {
            m_plmgr->update(delta);

            ///TMP while progress is not an event...
            std::pair<int,int> preCurProgVal = m_plmgr->getCurrentProgress();

            float pct = 1.0f;
            if (preCurProgVal.second > 0)
            {
                pct -= (float)(preCurProgVal.first) / (float)(preCurProgVal.second);
            }

			cocos2d::ui::LoadingBar* loadbar = getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar");
			if (loadbar)
            {
				loadbar->setPercent(pct * 100.0f);
            }

            if (pct >= 1.0f)
            {
                //we've finished preloading
                m_plStarted = false;
            }
        }
    }
}

