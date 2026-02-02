#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;

class $modify(MyGJGarageLayer, GJGarageLayer) {
public:
    bool init() {
        if (!GJGarageLayer::init()) return false;

        this->getChildByID("moons-icon")->setVisible(false);
        this->getChildByID("stars-icon")->setVisible(false);
        this->getChildByID("coins-icon")->setVisible(false);
        this->getChildByID("user-coins-icon")->setVisible(false);
        this->getChildByID("orbs-icon")->setVisible(false);
        this->getChildByID("diamonds-icon")->setVisible(false);
        this->getChildByID("diamond-shards-icon")->setVisible(false);
        this->getChildByID("moons-label")->setVisible(false);
        this->getChildByID("stars-label")->setVisible(false);
        this->getChildByID("coins-label")->setVisible(false);
        this->getChildByID("user-coins-label")->setVisible(false);
        this->getChildByID("orbs-label")->setVisible(false);
        this->getChildByID("diamonds-label")->setVisible(false);
        this->getChildByID("diamond-shards-label")->setVisible(false);

        return true;
    }
};