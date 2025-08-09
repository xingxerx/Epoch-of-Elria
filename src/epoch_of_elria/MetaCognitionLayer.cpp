#include "MetaCognitionLayer.h"
#include <iostream>

namespace elria {

void UI::draw() {
    if (!status_.empty()) {
        std::cout << status_ << "\n";
    }
}

void UI::setStatus(const std::string& s) {
    status_ = s;
}

std::string MetaCognition::analyze() const {
    if (past_decisions.empty()) return {};
    return std::string("Reflecting on: ") + past_decisions.back();
}

void GameStateContext::update(UI& ui, double dt) {
    (void)dt;
    // Incorporate meta-cognition reflection
    auto reflect = meta_.analyze();
    if (!reflect.empty()) {
        ui.setStatus(reflect);
    } else {
        ui.setStatus("[Idle] Engine running...");
    }
}

} // namespace elria

