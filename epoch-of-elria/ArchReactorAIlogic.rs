use serde::{Deserialize, Serialize};
use std::fs;

// Dummy risky_function for demonstration:
fn risky_function() -> Result<&'static str, &'static str> {
    // Replace this with your risky logic
    Err("Example error")
}

#[derive(Serialize, Deserialize, Debug)]
struct Config {
    max_caps_ratio: f32,
    toxicity_sensitivity: String,
    auto_response_enabled: bool,
}

fn load_config() -> Config {
    let config_str = fs::read_to_string("moderation_config.toml")
        .expect("Could not read configuration file.");
    toml::from_str(&config_str).expect("Could not parse configuration file.")
}

#[derive(Debug)]
struct ModerationEngine {
    banned_words: Vec<String>,
    max_caps_ratio: f32,
}

impl ModerationEngine {
    fn filter_message(&self, message: &str) -> bool {
        if message.is_empty() {
            return false; // Or true, depending on your policy
        }
        let caps_count = message.chars().filter(|c| c.is_uppercase()).count() as f32;
        let caps_ratio = caps_count / message.len() as f32;
        let contains_banned = self.banned_words.iter().any(|word| message.contains(word));
        caps_ratio > self.max_caps_ratio || contains_banned
    }
}

use tokio::task;

// Dummy gemini error type & API
#[derive(Debug)]
struct GeminiError;
mod gemini {
    use super::GeminiError;
    pub async fn analyze_text(_message: &str) -> Result<&'static str, GeminiError> {
        Ok("No action needed")
    }
}

async fn analyze_with_gemini(message: &str) -> Result<String, GeminiError> {
    let response = gemini::analyze_text(message).await?;
    Ok(format!("Gemini suggests action: {}", response))
}

async fn process_chat_message(message: String) {
    // Moved : spawn_blocking if you do heavy work, not for async
    let moderation_engine = ModerationEngine {
        banned_words: vec!["badword1".into(), "badword2".into()],
        max_caps_ratio: 0.7,
    };

    if moderation_engine.filter_message(&message) {
        println!("Message flagged: {}", message);
    } else {
        println!("Message approved: {}", message);
    }
    // Optionally, demonstrate Gemini:
    match analyze_with_gemini(&message).await {
        Ok(resp) => println!("{}", resp),
        Err(_) => println!("Gemini analysis failed!"),
    }
}

fn adjust_moderation(config: &Config) {
    println!("Moderation level set to: {:?}", config.toxicity_sensitivity);
    if config.auto_response_enabled {
        println!("AI auto-responses activated.");
    }
}

#[tokio::main]
async fn main() {
    let result = risky_function();
    match result {
        Ok(value) => println!("Success: {:?}", value),
        Err(e) => println!("Gemini Suggests Fixing: {:?}", e),
    }
    // Example config
    let config = load_config();
    adjust_moderation(&config);
    // Example chat message processing
    process_chat_message("HELLO badword1 world".to_string()).await;
    process_chat_message("hello world".to_string()).await;
}