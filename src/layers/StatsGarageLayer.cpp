#include "StatsGarageLayer.h"
#include <StatsDisplayAPI.h>
#include <raydeeux.pages_api/include/PagesAPI.h>
#include <raydeeux.pages_api/include/PageMenu.h>

using namespace geode::prelude;

const int ELEMENTS_PER_PAGE = 10;
// the distance of the first label from the top of the screen
const float TOP_MARGIN = 6.f;
// the distance from the right side of the screen
float RIGHT_MARGIN = 18.f;
// the pages api arrow scale
float ARROW_SCALE = 0.6f;
// the pages api arrow padding
float ARROW_PADDING = 1.f;

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
	m_fields->m_statsMenu->setContentSize({0, maxHeight - 2*ARROW_PADDING - 2*arrowSize});
	m_fields->m_statsMenu->setAnchorPoint(CCPoint{0.5f, 1.f});
	m_fields->m_statsMenu->setPositionX(winSize.width - RIGHT_MARGIN);

	if (PagesAPI::isLoaded()) {
    PageMenu* pageMenu = static_cast<PageMenu*>(m_fields->m_statsMenu); 
		pageMenu->setPaged(ELEMENTS_PER_PAGE, PageOrientation::VERTICAL, maxHeight, ARROW_PADDING);
		pageMenu->setButtonScale(ARROW_SCALE);
		schedule(schedule_selector(StatsGarageLayer::moveMenuForArrows));
	}

	return true;
}

CCNode* getFirstVisibleChild(CCNode* parent) {
	for (auto child : parent->getChildrenExt())
		if (child->isVisible())
			return child;
	return nullptr;
}

CCNode* getLastVisibleChild(CCNode* parent) {
	CCArrayExt<CCNode*> arrayExt = parent->getChildrenExt();
	if (arrayExt.size() == 0)
		return nullptr;
	for (size_t i = arrayExt.size()-1; i >= 0; i--)
		if (arrayExt[i]->isVisible())
			return arrayExt[i];
	return nullptr;
}

// this is a hack to move the node properly so it works with the way pages api works
// another issue this has it that if you switch a page and it needs to move the menu
// the arrows will visually jitter for a frame
// i do not know how to fix this without modifying pages api to add button press callbacks
// so we can do this instantly on button press
// it would genuinely be much easier to implement your own paging at this point
void StatsGarageLayer::moveMenuForArrows(float) {
  auto winSize = CCDirector::get()->getWinSize();
	CCNode* firstChild = getFirstVisibleChild(m_fields->m_statsMenu);
	CCNode* lastChild = getLastVisibleChild(m_fields->m_statsMenu);
	if (!firstChild || !lastChild) return;
	CCNode* firstLabel = firstChild->getChildByIndex(1);
	CCNode* lastLabel = lastChild->getChildByIndex(1);
	if (!firstLabel || !lastLabel) return;

	float topLabelMaxY = firstChild->convertToWorldSpace(CCPoint{0, firstLabel->boundingBox().getMaxY()}).y;
	float bottomLabelMinY = lastChild->convertToWorldSpace(CCPoint{0, lastLabel->boundingBox().getMinY()}).y;
	
	float newTopLabelMaxY;
	if (CCNode* pageMenu = m_fields->m_statsMenu->getParent()->getChildByID("stats-menu-navigation-menu"_spr)) {
		CCNode* previous = pageMenu->getChildByID("prev-button");
		CCNode* next = pageMenu->getChildByID("next-button");
		if (!previous || !next) return;

		float previousMaxY = pageMenu->convertToWorldSpace(CCPoint{0, previous->boundingBox().getMaxY()}).y;
		float nextMinY = pageMenu->convertToWorldSpace(CCPoint{0, next->boundingBox().getMinY()}).y;
		
		float prevDelta = previousMaxY - topLabelMaxY - ARROW_PADDING - previous->getScaledContentHeight();
		float nextDelta = bottomLabelMinY - nextMinY - ARROW_PADDING - next->getScaledContentHeight();
		previous->setPositionY(previous->getPositionY() - prevDelta);
		next->setPositionY(next->getPositionY() + nextDelta);
		// if the current top label's max y is topLabelMaxY
		// we want to move everything so that
		// winSize.height - newTopLabelMaxY = previous->getContentHeight() + ARROW_PADDING + TOP_MARGIN
		// newTopLabelMaxY = winSize.height - previous->getContentHeight() - ARROW_PADDING - TOP_MARGIN
		// we don't multiply the height by button scale because the scale is on the inner sprite,
		// not the button itself, so its content height is adjusted to the scale already
		// if we don't have arrows, we exclude them and their padding from the equation and thus get the 2nd branch
		// newTopLabelMaxY = winSize.height - TOP_MARGIN
		newTopLabelMaxY = winSize.height - previous->getContentHeight() - ARROW_PADDING - TOP_MARGIN;
	} else {
		// if the arrow menu does not exist, then we only have a single page and do not need to
		// keep track of anything else, just move the menu to align with the top margin
		newTopLabelMaxY = winSize.height - TOP_MARGIN;
	}
	// the amount we need to move the menu so that the top label max y becomes newTopLabelMaxY
	float topLabelMaxYDelta = topLabelMaxY - newTopLabelMaxY;
	m_fields->m_statsMenu->setPositionY(m_fields->m_statsMenu->getPositionY() - topLabelMaxYDelta);
}
