#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>
#include <raydeeux.pages_api/include/PagesAPI.h>
#include <raydeeux.pages_api/include/PageMenu.h>
#include "../CCMenuGSM.hpp"

using namespace geode::prelude;

const int ELEMENTS_PER_PAGE = 15;

bool StatsGarageLayer::init() {
	if (!GJGarageLayer::init())
		return false;

	auto winSize = CCDirector::get()->getWinSize();
	auto mod = Mod::get();

	auto layout = ColumnLayout::create()
		->setAutoScale(false)
		->setAxisReverse(true)
		->setAxisAlignment(AxisAlignment::End)
		->setCrossAxisAlignment(AxisAlignment::End)
		->setGap(15)
		->setAutoScale(true);

	m_fields->m_statsMenu = CCMenuGSM::create();
	if (!m_fields->m_statsMenu) {
		log::debug("Failed to create m_statsMenu");
		return false;
	}

	m_fields->m_statsMenu->getMenu()->setID("stats-menu"_spr);
	m_fields->m_statsMenu->getMenu()->setZOrder(2);
	m_fields->m_statsMenu->getMenu()->setLayout(layout);
	
	this->addChild(m_fields->m_statsMenu->getMenu());

	const std::vector<std::tuple<std::string, std::string, std::string, float>> stats = {
		{"stars-stat", "stars", "GJ_starsIcon_001.png", 0.54f},
		{"moons-stat", "moons", "GJ_moonsIcon_001.png", 0.54f},
		{"gold-coins-stat", "coins", "GJ_coinsIcon_001.png", 0.51f},
		{"user-coins-stat", "user-coins", "GJ_coinsIcon2_001.png", 0.51f},
		{"orbs-stat", "orbs", "currencyOrbIcon_001.png", 0.54f},
		{"diamonds-stat", "diamonds", "GJ_diamondsIcon_001.png", 0.6f},
		{"diamond-shards-stat", "diamond-shards", "currencyDiamondIcon_001.png", 0.54f}
	};

	for (const auto& [setting, id, sprite, scale] : stats) {
		if (m_fields->m_statsMenu && m_fields->m_statsMenu->getMenu()) {
			if (auto item = StatsDisplayAPI::getNewItem(id.c_str(), CCSprite::createWithSpriteFrameName(sprite.c_str()), GameStatsManager::sharedState()->getStat(id.c_str()), scale)) {
				m_fields->m_statsMenu->getMenu()->addChild(item);
			} else {
				log::debug("Failed to create item: {}", id);
			}
		} else {
			log::debug("Invalid state: m_statsMenu is null");
		}
	}

	if (m_fields->m_statsMenu) {
		m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 12));

		if (auto tempSprite = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png")) {
			float height = tempSprite->getScaledContentHeight();
			tempSprite->release();
			m_fields->m_statsMenu->setContentHeight(winSize.height - 24 - height);
		} else {
			log::debug("Failed to create tempSprite");
		}

		m_fields->m_statsMenu->setAnchorPoint(ccp(0.5f, 1.f));
	} else {
		log::debug("m_fields or m_statsMenu is null");
	}

	schedule(schedule_selector(StatsGarageLayer::moveMenuForArrows));
	if (auto pageMenu = dynamic_cast<PageMenu*>(m_fields->m_statsMenu)) {
		pageMenu->setPaged(ELEMENTS_PER_PAGE, PageOrientation::VERTICAL, m_fields->m_statsMenu->getContentHeight(), 12.f);
		pageMenu->setButtonScale(0.6f);
		pageMenu->setLayout(layout);
		pageMenu->updateLayout();
	} else {
		log::debug("Failed to cast m_statsMenu to PageMenu");
	}

	m_fields->m_statsMenu->updateLayout();

	return true;
}

void StatsGarageLayer::moveMenuForArrows(float) {
	auto winSize = CCDirector::get()->getWinSize();
	if (m_fields->m_statsMenu && m_fields->m_statsMenu->getChildrenCount() > ELEMENTS_PER_PAGE) {
		m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 30));
	} else {
		m_fields->m_statsMenu->setPosition(ccp(winSize.width - 18, winSize.height - 12));
	}
}
