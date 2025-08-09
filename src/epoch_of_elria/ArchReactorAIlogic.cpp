#include "ArchReactorAIlogic.h"
#include <sstream>
#include <fstream>
#include <algorithm>

namespace elria {

ModerationEngine::ModerationEngine() {
    banned_words_ = {"badword1", "badword2"};
}

bool ModerationEngine::filterMessage(const std::string& message) const {
    if (message.empty()) return false;
    const float caps = static_cast<float>(std::count_if(message.begin(), message.end(), [](unsigned char c){return std::isupper(c)!=0;}));
    const float ratio = caps / static_cast<float>(message.size());
    bool contains_banned = false;
    for (const auto& w : banned_words_) {
        if (message.find(w) != std::string::npos) { contains_banned = true; break; }
    }
    return ratio > max_caps_ratio_ || contains_banned;
}

ArchReactorAIlogic::ArchReactorAIlogic() {
    moderation_.setBanned({"badword1", "badword2"});
}

void ArchReactorAIlogic::reset() {
    time_ = 0.0;
}

void ArchReactorAIlogic::tick(double dt) {
    time_ += dt;
}

std::string ArchReactorAIlogic::status() const {
    std::ostringstream os;
    os << "ArchReactorAI time=" << time_;
    return os.str();
}

std::optional<Config> ArchReactorAIlogic::loadConfig(const std::string& tomlPath) {
    std::ifstream f(tomlPath);
    if (!f) return std::nullopt;
    Config parsed{};
    // Very naive parse: look for key=value lines
    std::string line;
    while (std::getline(f, line)) {
        if (line.find("max_caps_ratio") != std::string::npos) {
            auto pos = line.find('=');
            if (pos != std::string::npos) parsed.max_caps_ratio = std::stof(line.substr(pos+1));
        } else if (line.find("toxicity_sensitivity") != std::string::npos) {
            auto pos = line.find('=');
            if (pos != std::string::npos) parsed.toxicity_sensitivity = line.substr(pos+1);
        } else if (line.find("auto_response_enabled") != std::string::npos) {
            auto pos = line.find('=');
            if (pos != std::string::npos) parsed.auto_response_enabled = (line.substr(pos+1).find("true") != std::string::npos);
        }
    }
    cfg_ = parsed;
    moderation_.setMaxCapsRatio(cfg_.max_caps_ratio);
    return parsed;
}

std::string ArchReactorAIlogic::adjustModeration(const Config& cfg) {
    std::ostringstream os;
    os << "Moderation level set to: " << cfg.toxicity_sensitivity;
    if (cfg.auto_response_enabled) {
        os << "; AI auto-responses activated.";
    }
    moderation_.setMaxCapsRatio(cfg.max_caps_ratio);
    return os.str();
}

std::string ArchReactorAIlogic::simulateGeminiAnalyze(const std::string& message) {
    (void)message;
    return std::string("Gemini suggests action: No action needed");
}

AnalysisResult ArchReactorAIlogic::processChatMessage(const std::string& message) {
    AnalysisResult res;
    res.flagged = moderation_.filterMessage(message);
    res.moderation_note = res.flagged ? (std::string("Message flagged: ") + message) : (std::string("Message approved: ") + message);
    res.gemini_suggestion = simulateGeminiAnalyze(message);
    return res;
}

} // namespace elria

