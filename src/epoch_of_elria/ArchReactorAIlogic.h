#pragma once
#include <string>
#include <vector>
#include <optional>

namespace elria {

struct Config {
    float max_caps_ratio = 0.7f;
    std::string toxicity_sensitivity = "moderate";
    bool auto_response_enabled = false;
};

class ModerationEngine {
public:
    ModerationEngine();
    void setMaxCapsRatio(float r) { max_caps_ratio_ = r; }
    void setBanned(std::vector<std::string> words) { banned_words_ = std::move(words); }
    bool filterMessage(const std::string& message) const; // true = flagged
private:
    std::vector<std::string> banned_words_;
    float max_caps_ratio_ = 0.7f;
};

struct AnalysisResult {
    bool flagged = false;
    std::string moderation_note; // e.g., "Message approved" or reason
    std::string gemini_suggestion; // simulated external analysis
};

class ArchReactorAIlogic {
public:
    ArchReactorAIlogic();
    void reset();
    void tick(double dt);
    std::string status() const;

    // Ported helpers
    std::optional<Config> loadConfig(const std::string& tomlPath);
    std::string adjustModeration(const Config& cfg);
    AnalysisResult processChatMessage(const std::string& message);

private:
    double time_{0.0};
    ModerationEngine moderation_;
    Config cfg_{};

    static std::string simulateGeminiAnalyze(const std::string& message);
};

} // namespace elria

