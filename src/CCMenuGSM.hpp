#pragma once

#include <Geode/Geode.hpp>
#include <raydeeux.pages_api/include/PagesAPI.h>
#include <raydeeux.pages_api/include/PageMenu.h>

using namespace geode::prelude;

class CCMenuGSM : public PageMenu {
private:
    CCMenu* m_menu;

public:
    CREATE_FUNC(CCMenuGSM);

    CCMenuGSM() : m_menu(CCMenu::create()) {}

    void updateLayout();

    // Delegate CCMenu methods
    void addChild(CCNode* child) { m_menu->addChild(child); }
    void setPosition(const CCPoint& position) { m_menu->setPosition(position); }
    void setAnchorPoint(const CCPoint& anchorPoint) { m_menu->setAnchorPoint(anchorPoint); }
    void setZOrder(int zOrder) { m_menu->setZOrder(zOrder); }
    unsigned int getChildrenCount() const { return m_menu->getChildrenCount(); }
    void setContentHeight(float height) { m_menu->setContentHeight(height); }
    float getContentHeight() const { return m_menu->getContentHeight(); }
    void setLayout(geode::Layout* layout) { m_menu->setLayout(layout); }
    geode::Layout* getLayout() const { return m_menu->getLayout(); }

    CCMenu* getMenu() const { return m_menu; } // Accessor for the composed CCMenu
};