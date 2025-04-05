#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>

using namespace geode::prelude;

bool StatsGarageLayer::init() {
	if (!GJGarageLayer::init())
		return false;

	auto winSize = CCDirector::get()->getWinSize();

	m_fields->m_statsMenu = CCMenu::create();

	/*
	m_fields->m_nodeContainer.push_back(getExistingContainer("stars"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("moons"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("coins"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("user-coins"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("orbs"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("diamonds"));
	m_fields->m_nodeContainer.push_back(getExistingContainer("diamond-shards"));

	for (size_t i = 0; i < m_fields->m_nodeContainer.size(); i++) {
		//m_fields->m_nodeContainer[i]->setLayout(AxisLayout::create()->setAutoScale(false)->setAxisReverse(true));
		m_fields->m_statsMenu->addChild(m_fields->m_nodeContainer[i]);
	}
	*/

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
	m_fields->m_statsMenu->setContentHeight(winSize.height - 24 - CCScene::get()->getChildByID("GJGarageLayer")->getChildByID("bottom-right-corner")->getScaledContentHeight());
	m_fields->m_statsMenu->setAnchorPoint(ccp(0.5f, 1.f));

	m_fields->m_statsMenu->updateLayout();

	return true;
}

/*
CCNode* StatsGarageLayer::getExistingContainer(std::string itemName) {
	auto ret = CCMenu::create(); 
	auto icon = this->getChildByID(itemName + "-icon");
	if (icon) {
		icon->removeFromParentAndCleanup(false);
		ret->addChild(icon);
		icon->setPosition({0, 0});
	}
	auto label = this->getChildByID(itemName + "-label"); 
	if (label) {	
		label->removeFromParentAndCleanup(false);
		ret->addChild(label);
		label->setPosition({-12, 0.5});
	}
	ret->setID(""_spr + itemName + "-container");
	ret->setContentSize({ 0, 0 });
	return ret;
}
*/