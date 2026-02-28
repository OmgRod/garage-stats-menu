#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>
#include <raydeeux.pages_api/include/PagesAPI.h>
#include <raydeeux.pages_api/include/PageMenu.h>

using namespace geode::prelude;

// for some reason, if i cast to PageMenu, i can't actually get m_fields->m_elementCount; it's just 0.
// so we're making it a constant i guess
const int ELEMENTS_PER_PAGE = 15;

bool StatsGarageLayer::init() {
	if (!GJGarageLayer::init())
		return false;

	auto winSize = CCDirector::get()->getWinSize();
	auto mod = Mod::get();

	m_fields->m_statsMenu = CCMenu::create();

	/*if (mod->getSettingValue<bool>("stars-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("stars"));
	if (mod->getSettingValue<bool>("moons-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("moons"));
	if (mod->getSettingValue<bool>("gold-coins-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("coins"));
	if (mod->getSettingValue<bool>("user-coins-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("user-coins"));
	if (mod->getSettingValue<bool>("orbs-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("orbs"));
	if (mod->getSettingValue<bool>("diamonds-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("diamonds"));
	if (mod->getSettingValue<bool>("diamond-shards-stat")) m_fields->m_nodeContainer.push_back(getExistingContainer("diamond-shards"));

	for (size_t i = 0; i < m_fields->m_nodeContainer.size(); i++) {
		//m_fields->m_nodeContainer[i]->setLayout(AxisLayout::create()->setAutoScale(false)->setAxisReverse(true));
		m_fields->m_statsMenu->addChild(m_fields->m_nodeContainer[i]);
	}*/

	m_fields->m_statsMenu->setID("stats-menu"_spr);
	m_fields->m_statsMenu->setZOrder(2);
	m_fields->m_statsMenu->setLayout(ColumnLayout::create()->setAutoScale(false)->setAxisReverse(true)->setAxisAlignment(AxisAlignment::End)->setCrossAxisAlignment(AxisAlignment::End)->setGap(15)->setAutoScale(true));
	
	this->addChild(m_fields->m_statsMenu);
	if (mod->getSettingValue<bool>("stars-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("stars", CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png"), GameStatsManager::sharedState()->getStat("6"), 0.54f));
	if (mod->getSettingValue<bool>("moons-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("moons", CCSprite::createWithSpriteFrameName("GJ_moonsIcon_001.png"), GameStatsManager::sharedState()->getStat("28"), 0.54f));
	if (mod->getSettingValue<bool>("gold-coins-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("coins", CCSprite::createWithSpriteFrameName("GJ_coinsIcon_001.png"), GameStatsManager::sharedState()->getStat("8"), 0.51f));
	if (mod->getSettingValue<bool>("user-coins-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("user-coins", CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png"), GameStatsManager::sharedState()->getStat("12"), 0.51f));
	if (mod->getSettingValue<bool>("orbs-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("orbs", CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png"), GameStatsManager::sharedState()->getStat("14"), 0.54f));
	if (mod->getSettingValue<bool>("diamonds-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("diamonds", CCSprite::createWithSpriteFrameName("GJ_diamondsIcon_001.png"), GameStatsManager::sharedState()->getStat("13"), 0.6f));
	if (mod->getSettingValue<bool>("diamond-shards-stat")) m_fields->m_statsMenu->addChild(StatsDisplayAPI::getNewItem("diamond-shards", CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png"), GameStatsManager::sharedState()->getStat("29"), 0.54f));

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

/*CCNode* StatsGarageLayer::getExistingContainer(std::string itemName) {
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
}*/

void StatsGarageLayer::moveMenuForArrows(float) {
  auto winSize = CCDirector::get()->getWinSize();
  if (m_fields->m_statsMenu->getChildrenCount() > ELEMENTS_PER_PAGE)
    m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 30));
  else
    m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 12));
}
