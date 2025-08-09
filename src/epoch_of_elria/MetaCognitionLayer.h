#pragma once
#include <string>
#include <vector>

namespace elria {

class UI { // minimal placeholder UI
public:
    void draw();
    void setStatus(const std::string& s);
private:
    std::string status_;
};

class MetaCognition {
public:
    std::string analyze() const; // reflection on last decision
    std::vector<std::string> past_decisions {"Chose to moderate strictly", "Allowed ambiguous message"};
    std::vector<double> evaluation_scores {0.6, 0.8};
};

class GameStateContext {
public:
    void update(UI& ui, double dt);
private:
    MetaCognition meta_;
};

} // namespace elria

