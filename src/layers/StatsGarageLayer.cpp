#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>
#include <alphalaneous.pages_api/include/PagesAPI.h>
#include <alphalaneous.pages_api/include/PageMenu.h>

using namespace geode::prelude;

// for some reason, if i cast to PageMenu, i can't actually get m_fields->m_elementCount; it's just 0.
// so we're making it a constant i guess
const int ELEMENTS_PER_PAGE = 15;

bool StatsGarageLayer::init() {
	if (!GJGarageLayer::init())
		return false;

	auto winSize = CCDirector::get()->getWinSize();

	m_fields->m_statsMenu = CCMenu::create();

	m_fields->m_statsMenu->setID("stats-menu"_spr);
	m_fields->m_statsMenu->setZOrder(2);
	m_fields->m_statsMenu->setLayout(ColumnLayout::create()->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisAlignment(AxisAlignment::End)->setGap(15)->setAutoScale(true));
	
	this->addChild(m_fields->m_statsMenu);
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("stars", CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png"), GameStatsManager::sharedState()->getStat("6"), 0.54f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("moons", CCSprite::createWithSpriteFrameName("GJ_moonsIcon_001.png"), GameStatsManager::sharedState()->getStat("28"), 0.54f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("coins", CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png"), GameStatsManager::sharedState()->getStat("8"), 0.51f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("user-coins", CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png"), GameStatsManager::sharedState()->getStat("12"), 0.51f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("orbs", CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png"), GameStatsManager::sharedState()->getStat("14"), 0.54f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("diamonds", CCSprite::createWithSpriteFrameName("GJ_diamondsIcon_001.png"), GameStatsManager::sharedState()->getStat("13"), 0.6f));
	m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("diamond-shards", CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png"), GameStatsManager::sharedState()->getStat("29"), 0.54f));

	m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 12));

	auto tempSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
	if (tempSprite) {
		float height = tempSprite->getScaledContentHeight();
		tempSprite->release();
		m_fields->m_statsMenu->setContentHeight(winSize.height - 24 - height);
	}

	m_fields->m_statsMenu->setAnchorPoint(ccp(0.5f, 1.f));

	m_fields->m_statsMenu->updateLayout();

	if (PagesAPI::isLoaded()) {
    schedule(schedule_selector(StatsGarageLayer::moveMenuForArrows));
    PageMenu* pageMenu = static_cast<PageMenu*>(m_fields->m_statsMenu); 
		pageMenu->setPaged(ELEMENTS_PER_PAGE, PageOrientation::VERTICAL, m_fields->m_statsMenu->getContentHeight(), 12.f);
		pageMenu->setButtonScale(0.6f);
	}

	return true;
}


void StatsGarageLayer::moveMenuForArrows(float) {
  auto winSize = CCDirector::get()->getWinSize();
  if (m_fields->m_statsMenu->getChildrenCount() > ELEMENTS_PER_PAGE)
    m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 30));
  else
    m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 12));
}
