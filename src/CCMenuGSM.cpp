#include "CCMenuGSM.hpp"

void CCMenuGSM::updateLayout() {
    Layout* layout = m_menu->getLayout();

    m_menu->setLayout(layout);

    if (auto columnLayout = typeinfo_cast<ColumnLayout*>(layout)) {
        columnLayout->setAxisAlignment(AxisAlignment::End);
        columnLayout->setCrossAxisAlignment(AxisAlignment::End);
    }

    m_menu->updateLayout();
}
