#pragma once

#ifndef STATSGARAGELAYER_H
#define STATSGARAGELAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "../Macros.h"
#include "../CCMenuGSM.hpp"

class $modify(StatsGarageLayer, GJGarageLayer) {
public:
	struct Fields {
		CCMenuGSM* m_statsMenu = nullptr;
		std::vector<cocos2d::CCNode*> m_nodeContainer = {};
	};
	$override
	bool init();
	
	void moveMenuForArrows(float);

	EARLY_MODIFY(GJGarageLayer::init);
};
#endif
