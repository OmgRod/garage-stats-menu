#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>

using namespace geode::prelude;

const int ELEMENTS_PER_PAGE = 10;
// the distance of the first label from the top of the screen
const float TOP_MARGIN = 6.f;
// the distance from the right side of the screen
float RIGHT_MARGIN = 18.f;
// the pages api arrow scale
float ARROW_SCALE = 0.6f;

inline void addDefaultItem(
	CCNode* statsMenu,
	const char* setting,
	const char* id,
	const char* spriteFrameName,
	const char* statNum,
	float scale
) {
	if (Mod::get()->getSettingValue<bool>(setting)) {
		statsMenu->addChild(
			StatsDisplayAPI::getNewItem(
				id,
				CCSprite::createWithSpriteFrameName(spriteFrameName),
				GameStatsManager::sharedState()->getStat(statNum),
				scale
			)
		);
	}
}

bool StatsGarageLayer::init() {
	if (!GJGarageLayer::init())
		return false;

	auto winSize = CCDirector::get()->getWinSize();
	auto mod = Mod::get();

	m_fields->m_statsMenu = CCMenu::create();

	m_fields->m_statsMenu->setID("stats-menu"_spr);
	m_fields->m_statsMenu->setZOrder(2);
	m_fields->m_statsMenu->setLayout(
		ColumnLayout::create()
		->setAxisReverse(true)
		->setCrossAxisAlignment(AxisAlignment::End)
		->setAxisAlignment(AxisAlignment::End)
		->setGap(15)
	);
	
	this->addChild(m_fields->m_statsMenu);
	
	addDefaultItem(m_fields->m_statsMenu, "stars-stat", "stars", "GJ_starsIcon_001.png", "6", 0.54f);
	addDefaultItem(m_fields->m_statsMenu, "moons-stat", "moons", "GJ_moonsIcon_001.png", "28", 0.54f);
	addDefaultItem(m_fields->m_statsMenu, "gold-coins-stat", "coins", "GJ_coinsIcon_001.png", "8", 0.51f);
	addDefaultItem(m_fields->m_statsMenu, "user-coins-stat", "user-coins", "GJ_coinsIcon2_001.png", "12", 0.51f);
	addDefaultItem(m_fields->m_statsMenu, "orbs-stat", "orbs", "currencyOrbIcon_001.png", "14", 0.54f);
	addDefaultItem(m_fields->m_statsMenu, "diamonds-stat", "diamonds", "GJ_diamondsIcon_001.png", "13", 0.6f);
	addDefaultItem(m_fields->m_statsMenu, "diamond-shards-stat", "diamond-shards", "currencyDiamondIcon_001.png", "29", 0.54f);
	
	float bottomMargin = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png")->getContentHeight();
	float arrowSize = ARROW_SCALE * CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png")->getContentWidth();
	
	// the maximum height that our elements should cover, including the arrows and their paddings as "elements"
	float maxHeight = winSize.height - TOP_MARGIN - bottomMargin;
	// the actual maximum of the content, excluding the arrows
	m_fields->m_statsMenu->setContentSize({0, maxHeight});
	m_fields->m_statsMenu->setAnchorPoint(CCPoint{0.5f, 1.f});
	m_fields->m_statsMenu->setPosition(CCPoint{winSize.width - RIGHT_MARGIN, winSize.height - TOP_MARGIN});
	
	// set up the arrow buttons for pages
	CCSprite* prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	CCSprite* nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	nextSprite->setFlipX(true);
	prevSprite->setRotation(90);
	nextSprite->setRotation(90);
	prevSprite->setScale(ARROW_SCALE);
	nextSprite->setScale(ARROW_SCALE);
	m_fields->m_prevArrow = CCMenuItemSpriteExtra::create(prevSprite, this, menu_selector(StatsGarageLayer::switchPage));
	m_fields->m_nextArrow = CCMenuItemSpriteExtra::create(nextSprite, this, menu_selector(StatsGarageLayer::switchPage));
	m_fields->m_nextArrow->setID("next-arrow"_spr);
	m_fields->m_prevArrow->setID("prev-arrow"_spr);
	
	// set the Z order so that the arrows are always at the start and end
	m_fields->m_statsMenu->addChild(m_fields->m_prevArrow, INT_MIN, -1);
	m_fields->m_statsMenu->addChild(m_fields->m_nextArrow, INT_MAX, 1);

	// this checks for newly added children and updates paging if they exist
	schedule(schedule_selector(StatsGarageLayer::pageChildren));

	return true;
}

// the below 2 implement custom paging

void StatsGarageLayer::switchPage(CCObject* sender) {
	if (!m_fields->m_statsMenu) return;
	CCNode* button = static_cast<CCNode*>(sender);
	if (m_fields->m_statsMenu->getChildrenCount() < 2) {
		m_fields->m_currentPage = 0;
		return;
	}
	if (m_fields->m_statsMenu->getChildrenCount() == 2) m_fields->m_requestedPage = 0;

	int actualChildren = m_fields->m_statsMenu->getChildrenCount() - 2;
	int pageDelta = button->getTag();
	int maxPage = (actualChildren - 1)/ELEMENTS_PER_PAGE;
	if (m_fields->m_currentPage + pageDelta < 0) m_fields->m_requestedPage = maxPage;
	else if (m_fields->m_currentPage + pageDelta > maxPage) m_fields->m_requestedPage = 0;
	else m_fields->m_requestedPage = m_fields->m_currentPage + pageDelta;
}

void StatsGarageLayer::pageChildren(float) {
	if (!m_fields->m_statsMenu) return;

	if (m_fields->m_statsMenu->getChildrenCount() < 2) return;
	int actualChildren = m_fields->m_statsMenu->getChildrenCount() - 2;
	// don't unnecessarily update, only do it if anything actually changed
	if (
		m_fields->m_requestedPage == m_fields->m_currentPage &&
		actualChildren == m_fields->m_previousActualChildren.size()
	) {
		bool allSame = true;
		for (size_t i = 1; i < m_fields->m_statsMenu->getChildrenCount()-1; i++)
			if (m_fields->m_statsMenu->getChildByIndex(i) != m_fields->m_previousActualChildren[i-1])
				allSame = false;
		if (allSame) return;
	}

	m_fields->m_previousActualChildren.clear();
	m_fields->m_currentPage = m_fields->m_requestedPage;

	m_fields->m_prevArrow->setVisible(m_fields->m_statsMenu->getChildrenCount() > ELEMENTS_PER_PAGE + 2);
	m_fields->m_nextArrow->setVisible(m_fields->m_statsMenu->getChildrenCount() > ELEMENTS_PER_PAGE + 2);
	
	// set up the current page and save the previous children
	int maxPage = actualChildren == 0 ? 0 : (actualChildren - 1)/ELEMENTS_PER_PAGE;
	if (m_fields->m_currentPage > maxPage) m_fields->m_currentPage = maxPage; 
	for (size_t i = 1; i < m_fields->m_statsMenu->getChildrenCount()-1; i++) {
		CCNode* child = m_fields->m_statsMenu->getChildByIndex(i);
		m_fields->m_previousActualChildren.push_back(child);
		child->setVisible((i - 1)/ELEMENTS_PER_PAGE == m_fields->m_currentPage);
	}
	
	m_fields->m_statsMenu->updateLayout();
	
	// adjust position of menu. ideally we would not have to do this, but oh well
	auto winSize = CCDirector::get()->getWinSize();
	if (maxPage == 0 && actualChildren != 0) {
		// the containers have 0 height, and that's in the api now
		// so i have to do this to not touch the api
		// on the next major version, remove this and give the containers actual height
		// depending on the children; may have to adjust the gap as a result
		// i will use the label's top position to set the menu's position
		CCNode* firstChild = m_fields->m_statsMenu->getChildByIndex(1);
		CCNode* firstLabel = firstChild->getChildByIndex(1);
		if (!firstLabel) return;

		float topLabelMaxY = firstChild->convertToWorldSpace(CCPoint{0, firstLabel->boundingBox().getMaxY()}).y;
		float newTopLabelMaxY = winSize.height - TOP_MARGIN;
		float topLabelMaxYDelta = topLabelMaxY - newTopLabelMaxY;
		m_fields->m_statsMenu->setPositionY(m_fields->m_statsMenu->getPositionY() - topLabelMaxYDelta);
	} else {
		// otherwise we can do the sensible thing of keeping the position as it's supposed to be
		m_fields->m_statsMenu->setPositionY(winSize.height - TOP_MARGIN);
	}
}