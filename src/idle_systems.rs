use serde::{Serialize, Deserialize};
use std::collections::HashMap;
use std::time::{SystemTime, UNIX_EPOCH};

#[derive(Serialize, Deserialize, Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum ResourceType {
    Gold,
    ElrianDust,
    ResearchPoints,
    // Prestige currency
    ElrianShards,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct GeneratorState {
    pub id: String,
    pub level: u32,
    // For now, let's assume base_rate_per_second is configured elsewhere (e.g., GameConfig)
    // and applied during calculation.
    // pub base_rate_per_second: f64,
    pub resource_type: ResourceType,
}

impl GeneratorState {
    pub fn new(id: String, resource_type: ResourceType) -> Self {
        Self {
            id,
            level: 1, // Start at level 1
            resource_type,
        }
    }
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct PlayerData {
    pub resources: HashMap<ResourceType, f64>,
    pub current_epoch: u32,
    pub hero_level: u32,
    pub hero_xp: u64,
    pub hero_skill_points: u32, // New: For hero upgrades
    pub last_update_timestamp_secs: i64, // Store as seconds since UNIX_EPOCH
    pub generators: HashMap<String, GeneratorState>, // Keyed by generator ID
    pub permanent_bonuses: HashMap<String, f64>, // Example: "global_gold_multiplier" -> 1.1 (for +10%)
    // TODO: Add unlocked_upgrades later
    // pub unlocked_upgrades: HashSet<String>,
}

impl PlayerData {
    pub fn new() -> Self {
        let mut resources = HashMap::new();
        resources.insert(ResourceType::Gold, 0.0);
        resources.insert(ResourceType::ElrianDust, 0.0);
        resources.insert(ResourceType::ResearchPoints, 0.0);
        resources.insert(ResourceType::ElrianShards, 0.0);

        // Initialize with one basic gold generator for testing
        let mut generators = HashMap::new();
        generators.insert("gold_mine_1".to_string(), GeneratorState::new("gold_mine_1".to_string(), ResourceType::Gold));

        Self {
            resources,
            current_epoch: 1,
            hero_level: 1,
            hero_xp: 0,
            hero_skill_points: 0,
            last_update_timestamp_secs: current_timestamp_secs(),
            generators,
            permanent_bonuses: HashMap::new(),
        }
    }

    /// Updates resources based on elapsed time and generator rates.
    /// `generator_configs` would provide details like rate per level.
    /// For now, we'll use a placeholder rate.
    pub fn update_resources(&mut self, elapsed_secs: f64, game_config: &GameConfig) {
        let global_rate_multiplier = *self.permanent_bonuses.get("global_rate_multiplier").unwrap_or(&1.0);

        for (gen_id, gen_state) in &self.generators {
            if let Some(gen_config) = game_config.generator_configs.get(gen_id) {
                if gen_state.resource_type == gen_config.produces {
                    let base_generation = (gen_state.level as f64) * gen_config.base_rate_per_level_per_sec;
                    let final_generation_per_sec = base_generation * global_rate_multiplier;
                    let amount_generated = final_generation_per_sec * elapsed_secs;

                    *self.resources.entry(gen_state.resource_type).or_insert(0.0) += amount_generated;
                    log::trace!("Generated {} of {:?} from {} (base: {}, mult: {}, final_rate: {})",
                                amount_generated, gen_state.resource_type, gen_id, base_generation, global_rate_multiplier, final_generation_per_sec);
                }
            } else {
                // Fallback for basic testing if config not fully fleshed out
                if gen_state.resource_type == ResourceType::Gold {
                     let placeholder_rate_per_level_per_sec = 1.0; // 1 gold per level per second
                     let base_generation = (gen_state.level as f64) * placeholder_rate_per_level_per_sec;
                     let final_generation_per_sec = base_generation * global_rate_multiplier;
                     let amount_generated = final_generation_per_sec * elapsed_secs;

                    *self.resources.entry(gen_state.resource_type).or_insert(0.0) += amount_generated;
                    log::trace!("Generated {} of {:?} from {} (placeholder rate, base: {}, mult: {}, final_rate: {})",
                                amount_generated, gen_state.resource_type, gen_id, base_generation, global_rate_multiplier, final_generation_per_sec);
                }
            }
        }
    }

    pub fn record_update_time(&mut self) {
        self.last_update_timestamp_secs = current_timestamp_secs();
    }

    pub fn get_xp_for_next_level(&self, game_config: &GameConfig) -> u64 {
        if self.hero_level == 0 { return game_config.hero_xp_per_level.get(1).cloned().unwrap_or(100); } // Should not happen if level starts at 1
        game_config.hero_xp_per_level
            .get(self.hero_level as usize) // hero_level is 1-indexed for display, array is 0-indexed
            .cloned()
            .unwrap_or_else(|| {
                // Fallback if config doesn't have enough levels: very high number or extend previous logic
                log::warn!("XP requirement for level {} not found in config. Using fallback.", self.hero_level + 1);
                ((self.hero_level as f64).powf(1.8) * 150.0) as u64 // Slightly steeper fallback
            })
    }

    pub fn gain_xp(&mut self, amount: u64, game_config: &GameConfig) {
        self.hero_xp += amount;
        log::info!("Gained {} XP. Total XP: {}", amount, self.hero_xp);
        self.check_for_level_up(game_config);
    }

    fn check_for_level_up(&mut self, game_config: &GameConfig) {
        let mut leveled_up = false;
        while self.hero_xp >= self.get_xp_for_next_level(game_config) {
            let xp_needed = self.get_xp_for_next_level(game_config);
            self.hero_xp -= xp_needed;
            self.hero_level += 1;
            self.hero_skill_points += 1; // Grant one skill point per level
            leveled_up = true;
            log::info!(
                "Hero leveled up! Level: {}, Skill Points: {}. XP for next level: {}. Remaining XP: {}",
                self.hero_level, self.hero_skill_points, self.get_xp_for_next_level(game_config), self.hero_xp
            );
        }
        if leveled_up {
            // Potentially trigger other events or notifications for level up
        }
    }

    /// Resets progress for a new epoch, granting Elrian Shards based on progress.
    pub fn reset_for_new_epoch(&mut self) {
        // Calculate Elrian Shards based on hero level (example formula)
        let shards_gained = (self.hero_level as f64).powf(1.2).floor() as u64;
        *self.resources.entry(ResourceType::ElrianShards).or_insert(0.0) += shards_gained as f64;
        log::info!("Epoch Reset! Gained {} Elrian Shards. Total Shards: {}", shards_gained, self.resources[&ResourceType::ElrianShards]);

        // Reset resources (except Elrian Shards)
        self.resources.insert(ResourceType::Gold, 0.0);
        self.resources.insert(ResourceType::ElrianDust, 0.0);
        self.resources.insert(ResourceType::ResearchPoints, 0.0);

        // Reset hero stats
        self.hero_level = 1;
        self.hero_xp = 0;
        self.hero_skill_points = 0; // Or decide if some skill points carry over based on bonuses

        // Reset generators to initial state (level 1 for the first one, others removed or reset)
        let mut initial_generators = HashMap::new();
        initial_generators.insert("gold_mine_1".to_string(), GeneratorState::new("gold_mine_1".to_string(), ResourceType::Gold));
        self.generators = initial_generators;

        self.current_epoch += 1;
        self.record_update_time(); // Important to reset the time basis for the new epoch
        log::info!("Starting Epoch {}. Player state reset.", self.current_epoch);
    }

    // Example: Apply a permanent bonus
    // In a real game, this would be more structured, probably tied to specific bonus IDs and effects.
    pub fn apply_permanent_bonus(&mut self, bonus_id: String, value: f64) {
        self.permanent_bonuses.insert(bonus_id.clone(), value);
        log::info!("Applied permanent bonus: {} -> {}", bonus_id, value);
        // Note: The actual application of these bonuses (e.g., modifying generation rates)
        // would happen in the resource update logic, reading from this map.
    }

    pub fn save_to_file(&self, filepath: &str) -> Result<(), std::io::Error> {
        let serialized_data = serde_json::to_string_pretty(self)?;
        std::fs::write(filepath, serialized_data)?;
        log::info!("Player data saved to {}", filepath);
        Ok(())
    }

    pub fn load_from_file(filepath: &str) -> Result<Self, std::io::Error> {
        if std::path::Path::new(filepath).exists() {
            let serialized_data = std::fs::read_to_string(filepath)?;
            let loaded_data: PlayerData = serde_json::from_str(&serialized_data)?;
            log::info!("Player data loaded from {}", filepath);
            Ok(loaded_data)
        } else {
            log::info!("No save file found at {}. Creating new PlayerData.", filepath);
            Ok(PlayerData::new()) // Return new data if file doesn't exist
        }
    }
}

impl Default for PlayerData {
    fn default() -> Self {
        Self::new()
    }
}

// --- Game Configuration (to be loaded from file eventually) ---
#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct GeneratorConfigEntry {
    pub name: String,
    pub initial_cost: f64,
    pub cost_scaling_factor: f64,
    pub base_rate_per_level_per_sec: f64,
    pub produces: ResourceType,
    pub cost_resource: ResourceType, // What resource is used to upgrade this generator
}

#[derive(Serialize, Deserialize, Debug, Clone, Default)]
pub struct GameConfig {
    pub generator_configs: HashMap<String, GeneratorConfigEntry>, // TODO: This should also be serializable and potentially part of the save or a separate config file.
    pub hero_xp_per_level: Vec<u64>, // Example: [100, 250, 500, ...]
    // TODO: Add epoch_bonus_configs later
}

impl GameConfig {
    pub fn new_default_configs() -> Self {
        let mut configs = HashMap::new();
        configs.insert("gold_mine_1".to_string(), GeneratorConfigEntry {
            name: "Gold Mine".to_string(),
            initial_cost: 10.0,
            cost_scaling_factor: 1.15,
            base_rate_per_level_per_sec: 1.0,
            produces: ResourceType::Gold,
            cost_resource: ResourceType::Gold,
        });
        configs.insert("elrian_dust_extractor_1".to_string(), GeneratorConfigEntry {
            name: "Elrian Dust Extractor".to_string(),
            initial_cost: 100.0, // Costs gold to build
            cost_scaling_factor: 1.20,
            base_rate_per_level_per_sec: 0.1,
            produces: ResourceType::ElrianDust,
            cost_resource: ResourceType::Gold,
        });
        Self {
            generator_configs: configs,
            hero_xp_per_level: vec![0, 100, 250, 500, 1000, 2000], // Level 0, then 1 to 5
        }
    }
}


// --- Utility Functions ---
pub fn current_timestamp_secs() -> i64 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .expect("Time went backwards")
        .as_secs() as i64
}

const SAVE_FILE_PATH: &str = "player_data.json";

// --- Idle Manager (to be part of GameEngine or a dedicated system) ---
pub struct IdleManager {
    pub player_data: PlayerData,
    pub game_config: GameConfig, // This would ideally be loaded externally
}

impl IdleManager {
    pub fn new() -> Self {
        let mut player_data = PlayerData::load_from_file(SAVE_FILE_PATH).unwrap_or_else(|e| {
            log::warn!("Failed to load player data from {}: {}. Creating new.", SAVE_FILE_PATH, e);
            PlayerData::new()
        });

        let game_config = GameConfig::new_default_configs(); // Load default or from file

        // Process offline progress on initialization
        // Note: PlayerData's last_update_timestamp is from the loaded data.
        let now_secs = current_timestamp_secs();
        let time_elapsed_offline_secs = now_secs - player_data.last_update_timestamp_secs;

        if time_elapsed_offline_secs > 0 {
            log::info!("Offline for {} seconds. Calculating offline progress...", time_elapsed_offline_secs);
            player_data.update_resources(time_elapsed_offline_secs as f64, &game_config); // player_data needs to be mutable
            log::info!("Offline progress applied. Current Gold: {}", player_data.resources.get(&ResourceType::Gold).unwrap_or(&0.0));
        }
        player_data.record_update_time(); // Set current time as last updated

        Self {
            player_data,
            game_config,
        }
    }

    /// Main update tick for the idle systems.
    /// Call this regularly from the main game loop.
    pub fn update(&mut self, delta_time_secs: f64) {
        if delta_time_secs < 0.0 { // Allow 0.0 for initial offline processing
            return;
        }
        self.player_data.update_resources(delta_time_secs, &self.game_config);
        self.player_data.record_update_time(); // Important for accurate offline calculation if game crashes
    }

    pub fn manually_gain_xp(&mut self, amount: u64) {
        self.player_data.gain_xp(amount, &self.game_config);
    }

    pub fn reset_epoch(&mut self) {
        self.player_data.reset_for_new_epoch();
        // Potentially re-apply permanent bonuses or other epoch start logic here
    }

    pub fn save_game_data(&self) {
        // Before saving, ensure the last_update_timestamp is current,
        // especially if there were no other updates that frame.
        // However, player_data is not mutable here. Best to call record_update_time in update()
        // or make save_game_data take &mut self if it needs to modify before saving.
        // For now, assuming timestamp is reasonably up-to-date from regular updates.
        match self.player_data.save_to_file(SAVE_FILE_PATH) {
            Ok(_) => log::info!("Game data saved successfully."),
            Err(e) => log::error!("Failed to save game data: {}", e),
        }
    }

    // Placeholder for upgrading a generator
    pub fn upgrade_generator(&mut self, generator_id: &str) -> Result<(), String> {
        let gen_config = self.game_config.generator_configs.get(generator_id)
            .ok_or_else(|| format!("Generator config not found: {}", generator_id))?;

        let gen_state = self.player_data.generators.get_mut(generator_id)
            .ok_or_else(|| format!("Generator state not found: {}", generator_id))?;

        // Simple cost formula: initial_cost * (scaling_factor ^ current_level)
        let upgrade_cost = gen_config.initial_cost * gen_config.cost_scaling_factor.powi(gen_state.level as i32);

        let player_resource = self.player_data.resources.get_mut(&gen_config.cost_resource)
            .ok_or_else(|| format!("Player does not have resource type {:?}", gen_config.cost_resource))?;

        if *player_resource >= upgrade_cost {
            *player_resource -= upgrade_cost;
            gen_state.level += 1;
            log::info!("Upgraded {} to level {}. Cost: {:.2} {:?}. New player {:?}: {:.2}",
                generator_id, gen_state.level, upgrade_cost, gen_config.cost_resource, gen_config.cost_resource, *player_resource);
            Ok(())
        } else {
            Err(format!("Not enough {:?} to upgrade {}. Need {:.2}, have {:.2}",
                gen_config.cost_resource, generator_id, upgrade_cost, *player_resource))
        }
    }
}

impl Default for IdleManager {
    fn default() -> Self {
        Self::new()
    }
}
