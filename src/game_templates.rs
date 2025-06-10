// Game Templates - Pre-built game types and mechanics
use crate::math::Vector3D;
use crate::game_framework::*;
use crate::game_objects::{GameObject, Player, Enemy, Collectible, Platform};
use crate::scene::Scene;
use crate::input::{InputManager, Key};
use crate::advanced_rendering::*;
use std::collections::HashMap;

// Game template types
#[derive(Debug, Clone, PartialEq)]
pub enum GameTemplate {
    Platformer2D,
    RPG3D,
    FirstPersonShooter,
    RacingGame,
    PuzzleGame,
    SurvivalGame,
    AdventureGame,
    StrategyGame,
    Custom(String),
}

// Base game template trait
pub trait GameTemplateBuilder {
    fn create_game(&self, config: GameConfig) -> Result<Box<dyn GameInstance>, String>;
    fn get_template_name(&self) -> &str;
    fn get_description(&self) -> &str;
    fn get_required_assets(&self) -> Vec<String>;
}

// Game configuration
#[derive(Debug, Clone)]
pub struct GameConfig {
    pub title: String,
    pub world_size: Vector3D,
    pub player_start_position: Vector3D,
    pub difficulty: Difficulty,
    pub game_mode: GameMode,
    pub max_players: usize,
    pub enable_multiplayer: bool,
    pub custom_settings: HashMap<String, String>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum GameMode {
    SinglePlayer,
    Cooperative,
    Competitive,
    Sandbox,
    Campaign,
    Endless,
}

// Base game instance trait
pub trait GameInstance {
    fn initialize(&mut self, scene: &mut Scene) -> Result<(), String>;
    fn update(&mut self, delta_time: f32, input: &InputManager, framework: &mut GameFramework) -> GameUpdateResult;
    fn handle_event(&mut self, event: GameEvent) -> Result<(), String>;
    fn get_game_state(&self) -> &GameState;
    fn save_game(&self) -> Result<SaveData, String>;
    fn load_game(&mut self, save_data: SaveData) -> Result<(), String>;
    fn get_ui_elements(&self) -> Vec<UIElement>;
    fn cleanup(&mut self);
}

#[derive(Debug, Clone)]
pub enum GameUpdateResult {
    Continue,
    StateChange(GameState),
    Victory,
    GameOver,
    Restart,
    Quit,
}

#[derive(Debug, Clone)]
pub enum GameEvent {
    PlayerDied,
    EnemyDefeated { enemy_id: usize },
    ItemCollected { item_id: String, quantity: i32 },
    QuestCompleted { quest_id: String },
    LevelCompleted,
    CheckpointReached { checkpoint_id: String },
    DialogueStarted { npc_id: usize },
    DialogueEnded,
    Custom { event_type: String, data: HashMap<String, String> },
}

// UI System
#[derive(Debug, Clone)]
pub struct UIElement {
    pub id: String,
    pub element_type: UIElementType,
    pub position: (f32, f32),
    pub size: (f32, f32),
    pub visible: bool,
    pub interactive: bool,
    pub style: UIStyle,
    pub content: UIContent,
}

#[derive(Debug, Clone)]
pub enum UIElementType {
    Text,
    Button,
    Image,
    ProgressBar,
    HealthBar,
    Inventory,
    Minimap,
    DialogueBox,
    Menu,
    HUD,
}

#[derive(Debug, Clone)]
pub struct UIStyle {
    pub background_color: (f32, f32, f32, f32),
    pub border_color: (f32, f32, f32, f32),
    pub text_color: (f32, f32, f32, f32),
    pub font_size: f32,
    pub padding: (f32, f32, f32, f32),
    pub margin: (f32, f32, f32, f32),
}

#[derive(Debug, Clone)]
pub enum UIContent {
    Text(String),
    Image(String),
    Progress { current: f32, max: f32 },
    Health { current: i32, max: i32 },
    Items(Vec<(String, i32)>),
    Menu(Vec<MenuItem>),
}

#[derive(Debug, Clone)]
pub struct MenuItem {
    pub text: String,
    pub action: MenuAction,
    pub enabled: bool,
    pub submenu: Option<Vec<MenuItem>>,
}

#[derive(Debug, Clone)]
pub enum MenuAction {
    StartGame,
    LoadGame,
    SaveGame,
    Settings,
    Quit,
    Resume,
    Restart,
    Custom(String),
}

// RPG Game Template
pub struct RPGGameTemplate;

impl GameTemplateBuilder for RPGGameTemplate {
    fn create_game(&self, config: GameConfig) -> Result<Box<dyn GameInstance>, String> {
        Ok(Box::new(RPGGame::new(config)?))
    }

    fn get_template_name(&self) -> &str {
        "3D RPG Adventure"
    }

    fn get_description(&self) -> &str {
        "A classic 3D RPG with character progression, quests, inventory, and combat"
    }

    fn get_required_assets(&self) -> Vec<String> {
        vec![
            "models/player.obj".to_string(),
            "models/enemies/".to_string(),
            "textures/terrain/".to_string(),
            "audio/music/".to_string(),
            "audio/sfx/".to_string(),
            "ui/hud/".to_string(),
        ]
    }
}

pub struct RPGGame {
    config: GameConfig,
    state: GameState,
    player_stats: PlayerStats,
    current_level: String,
    active_quests: Vec<Quest>,
    npcs: HashMap<usize, NPC>,
    combat_system: CombatSystem,
    level_system: LevelSystem,
    ui_elements: Vec<UIElement>,
}

#[derive(Debug, Clone)]
pub struct PlayerStats {
    pub level: i32,
    pub experience: i32,
    pub health: i32,
    pub max_health: i32,
    pub mana: i32,
    pub max_mana: i32,
    pub strength: i32,
    pub dexterity: i32,
    pub intelligence: i32,
    pub defense: i32,
    pub speed: f32,
    pub skill_points: i32,
    pub attributes: HashMap<String, i32>,
}

#[derive(Debug, Clone)]
pub struct NPC {
    pub id: usize,
    pub name: String,
    pub position: Vector3D,
    pub dialogue_tree: String,
    pub shop_items: Option<Vec<Item>>,
    pub quest_giver: bool,
    pub available_quests: Vec<String>,
    pub faction: String,
    pub relationship: i32, // -100 to 100
}

#[derive(Debug, Clone)]
pub struct CombatSystem {
    pub turn_based: bool,
    pub real_time: bool,
    pub damage_types: Vec<DamageType>,
    pub status_effects: HashMap<String, StatusEffect>,
    pub combat_stats: CombatStats,
}

#[derive(Debug, Clone)]
pub enum DamageType {
    Physical,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Dark,
}

#[derive(Debug, Clone)]
pub struct StatusEffect {
    pub name: String,
    pub duration: f32,
    pub effect_type: StatusEffectType,
    pub intensity: f32,
}

#[derive(Debug, Clone)]
pub enum StatusEffectType {
    Poison,
    Burn,
    Freeze,
    Stun,
    Slow,
    Haste,
    Regeneration,
    Shield,
}

#[derive(Debug, Clone)]
pub struct CombatStats {
    pub battles_won: i32,
    pub battles_lost: i32,
    pub total_damage_dealt: i32,
    pub total_damage_taken: i32,
    pub enemies_defeated: HashMap<String, i32>,
}

#[derive(Debug, Clone)]
pub struct LevelSystem {
    pub current_level: String,
    pub available_levels: Vec<LevelData>,
    pub level_progression: Vec<String>,
    pub unlocked_levels: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct LevelData {
    pub name: String,
    pub description: String,
    pub required_level: i32,
    pub required_quests: Vec<String>,
    pub environment_type: EnvironmentType,
    pub enemy_types: Vec<String>,
    pub collectibles: Vec<String>,
    pub secrets: Vec<Secret>,
}

#[derive(Debug, Clone)]
pub enum EnvironmentType {
    Forest,
    Desert,
    Mountains,
    Cave,
    City,
    Dungeon,
    Castle,
    Swamp,
    Beach,
    Volcano,
}

#[derive(Debug, Clone)]
pub struct Secret {
    pub id: String,
    pub position: Vector3D,
    pub secret_type: SecretType,
    pub reward: QuestReward,
    pub discovered: bool,
}

#[derive(Debug, Clone)]
pub enum SecretType {
    HiddenChest,
    SecretArea,
    EasterEgg,
    Lore,
    PowerUp,
}

impl RPGGame {
    pub fn new(config: GameConfig) -> Result<Self, String> {
        let player_stats = PlayerStats {
            level: 1,
            experience: 0,
            health: 100,
            max_health: 100,
            mana: 50,
            max_mana: 50,
            strength: 10,
            dexterity: 10,
            intelligence: 10,
            defense: 10,
            speed: 5.0,
            skill_points: 0,
            attributes: HashMap::new(),
        };

        let combat_system = CombatSystem {
            turn_based: false,
            real_time: true,
            damage_types: vec![
                DamageType::Physical,
                DamageType::Fire,
                DamageType::Ice,
                DamageType::Lightning,
            ],
            status_effects: HashMap::new(),
            combat_stats: CombatStats {
                battles_won: 0,
                battles_lost: 0,
                total_damage_dealt: 0,
                total_damage_taken: 0,
                enemies_defeated: HashMap::new(),
            },
        };

        let level_system = LevelSystem {
            current_level: "starting_village".to_string(),
            available_levels: vec![
                LevelData {
                    name: "Starting Village".to_string(),
                    description: "A peaceful village where your adventure begins".to_string(),
                    required_level: 1,
                    required_quests: vec![],
                    environment_type: EnvironmentType::City,
                    enemy_types: vec![],
                    collectibles: vec!["health_potion".to_string(), "copper_coin".to_string()],
                    secrets: vec![],
                },
                LevelData {
                    name: "Whispering Woods".to_string(),
                    description: "A mysterious forest filled with ancient magic".to_string(),
                    required_level: 2,
                    required_quests: vec!["tutorial_complete".to_string()],
                    environment_type: EnvironmentType::Forest,
                    enemy_types: vec!["wolf".to_string(), "goblin".to_string()],
                    collectibles: vec!["herb".to_string(), "wood".to_string()],
                    secrets: vec![
                        Secret {
                            id: "hidden_shrine".to_string(),
                            position: Vector3D::new(25.0, 0.0, 30.0),
                            secret_type: SecretType::HiddenChest,
                            reward: QuestReward {
                                reward_type: "item".to_string(),
                                amount: 1,
                                item_id: Some("magic_sword".to_string()),
                            },
                            discovered: false,
                        }
                    ],
                },
            ],
            level_progression: vec![
                "starting_village".to_string(),
                "whispering_woods".to_string(),
                "goblin_caves".to_string(),
                "ancient_ruins".to_string(),
                "dragon_lair".to_string(),
            ],
            unlocked_levels: vec!["starting_village".to_string()],
        };

        let ui_elements = vec![
            UIElement {
                id: "health_bar".to_string(),
                element_type: UIElementType::HealthBar,
                position: (10.0, 10.0),
                size: (200.0, 20.0),
                visible: true,
                interactive: false,
                style: UIStyle {
                    background_color: (0.2, 0.2, 0.2, 0.8),
                    border_color: (1.0, 1.0, 1.0, 1.0),
                    text_color: (1.0, 1.0, 1.0, 1.0),
                    font_size: 14.0,
                    padding: (5.0, 5.0, 5.0, 5.0),
                    margin: (0.0, 0.0, 0.0, 0.0),
                },
                content: UIContent::Health { current: 100, max: 100 },
            },
            UIElement {
                id: "experience_bar".to_string(),
                element_type: UIElementType::ProgressBar,
                position: (10.0, 40.0),
                size: (200.0, 15.0),
                visible: true,
                interactive: false,
                style: UIStyle {
                    background_color: (0.1, 0.1, 0.3, 0.8),
                    border_color: (0.5, 0.5, 1.0, 1.0),
                    text_color: (1.0, 1.0, 1.0, 1.0),
                    font_size: 12.0,
                    padding: (2.0, 2.0, 2.0, 2.0),
                    margin: (0.0, 0.0, 0.0, 0.0),
                },
                content: UIContent::Progress { current: 0.0, max: 100.0 },
            },
        ];

        Ok(Self {
            config,
            state: GameState::Playing,
            player_stats,
            current_level: "starting_village".to_string(),
            active_quests: vec![],
            npcs: HashMap::new(),
            combat_system,
            level_system,
            ui_elements,
        })
    }

    pub fn add_npc(&mut self, npc: NPC) {
        self.npcs.insert(npc.id, npc);
    }

    pub fn start_quest(&mut self, quest_id: &str) -> Result<(), String> {
        // Implementation for starting a quest
        println!("Starting quest: {}", quest_id);
        Ok(())
    }

    pub fn level_up_player(&mut self) {
        self.player_stats.level += 1;
        self.player_stats.skill_points += 3;
        self.player_stats.max_health += 10;
        self.player_stats.max_mana += 5;
        self.player_stats.health = self.player_stats.max_health;
        self.player_stats.mana = self.player_stats.max_mana;
        
        println!("Level up! Now level {}", self.player_stats.level);
    }

    pub fn take_damage(&mut self, damage: i32) {
        self.player_stats.health = (self.player_stats.health - damage).max(0);
        if self.player_stats.health == 0 {
            self.state = GameState::GameOver;
        }
    }

    pub fn heal_player(&mut self, amount: i32) {
        self.player_stats.health = (self.player_stats.health + amount).min(self.player_stats.max_health);
    }

    pub fn gain_experience(&mut self, amount: i32) {
        self.player_stats.experience += amount;
        let exp_needed = self.calculate_exp_for_next_level();
        
        while self.player_stats.experience >= exp_needed {
            self.player_stats.experience -= exp_needed;
            self.level_up_player();
        }
    }

    fn calculate_exp_for_next_level(&self) -> i32 {
        // Standard RPG experience curve
        100 + (self.player_stats.level * 50) + (self.player_stats.level * self.player_stats.level * 10)
    }
}

impl GameInstance for RPGGame {
    fn initialize(&mut self, scene: &mut Scene) -> Result<(), String> {
        // Create player
        let player_id = scene.add_player(self.config.player_start_position);
        
        // Add some initial NPCs
        let village_elder = NPC {
            id: 1,
            name: "Village Elder".to_string(),
            position: Vector3D::new(5.0, 0.0, 5.0),
            dialogue_tree: "elder_intro".to_string(),
            shop_items: None,
            quest_giver: true,
            available_quests: vec!["find_lost_cat".to_string()],
            faction: "village".to_string(),
            relationship: 50,
        };
        self.add_npc(village_elder);

        // Add some collectibles
        let _health_potion_id = scene.add_collectible(Vector3D::new(10.0, 1.0, 10.0), 25);
        let _coin_id = scene.add_collectible(Vector3D::new(-5.0, 1.0, 8.0), 10);

        // Add some platforms/terrain
        let _platform_id = scene.add_platform(
            Vector3D::new(20.0, 2.0, 20.0),
            Vector3D::new(0.0, 0.0, 0.0)
        );

        println!("RPG game initialized with player at {:?}", self.config.player_start_position);
        Ok(())
    }

    fn update(&mut self, delta_time: f32, input: &InputManager, framework: &mut GameFramework) -> GameUpdateResult {
        match self.state {
            GameState::Playing => {
                // Handle input
                if input.is_key_pressed(Key::Escape) {
                    return GameUpdateResult::StateChange(GameState::Paused);
                }

                if input.is_key_pressed(Key::I) {
                    // Toggle inventory
                    println!("Inventory toggled");
                }

                if input.is_key_pressed(Key::M) {
                    // Toggle map
                    println!("Map toggled");
                }

                if input.is_key_pressed(Key::Tab) {
                    // Show character stats
                    println!("Player Stats - Level: {}, HP: {}/{}, XP: {}", 
                        self.player_stats.level,
                        self.player_stats.health,
                        self.player_stats.max_health,
                        self.player_stats.experience
                    );
                }

                // Update UI
                for ui_element in &mut self.ui_elements {
                    match ui_element.id.as_str() {
                        "health_bar" => {
                            ui_element.content = UIContent::Health {
                                current: self.player_stats.health,
                                max: self.player_stats.max_health,
                            };
                        }
                        "experience_bar" => {
                            let exp_needed = self.calculate_exp_for_next_level();
                            ui_element.content = UIContent::Progress {
                                current: self.player_stats.experience as f32,
                                max: exp_needed as f32,
                            };
                        }
                        _ => {}
                    }
                }

                GameUpdateResult::Continue
            }
            GameState::Paused => {
                if input.is_key_pressed(Key::Escape) {
                    return GameUpdateResult::StateChange(GameState::Playing);
                }
                GameUpdateResult::Continue
            }
            GameState::GameOver => {
                if input.is_key_pressed(Key::R) {
                    return GameUpdateResult::Restart;
                }
                if input.is_key_pressed(Key::Q) {
                    return GameUpdateResult::Quit;
                }
                GameUpdateResult::Continue
            }
            _ => GameUpdateResult::Continue,
        }
    }

    fn handle_event(&mut self, event: GameEvent) -> Result<(), String> {
        match event {
            GameEvent::ItemCollected { item_id, quantity } => {
                println!("Collected {} x{}", item_id, quantity);
                
                // Give experience for collecting items
                self.gain_experience(5);
                
                // Special handling for different items
                match item_id.as_str() {
                    "health_potion" => self.heal_player(25),
                    "coin" => {
                        // Add to inventory (would integrate with framework.inventory)
                    }
                    _ => {}
                }
            }
            GameEvent::EnemyDefeated { enemy_id } => {
                println!("Defeated enemy {}", enemy_id);
                self.gain_experience(25);
                self.combat_system.combat_stats.battles_won += 1;
            }
            GameEvent::PlayerDied => {
                self.state = GameState::GameOver;
                self.combat_system.combat_stats.battles_lost += 1;
            }
            GameEvent::QuestCompleted { quest_id } => {
                println!("Quest completed: {}", quest_id);
                self.gain_experience(100);
                
                // Remove from active quests
                self.active_quests.retain(|q| q.id != quest_id);
            }
            GameEvent::DialogueStarted { npc_id } => {
                if let Some(npc) = self.npcs.get(&npc_id) {
                    println!("Started dialogue with {}", npc.name);
                }
            }
            _ => {}
        }
        Ok(())
    }

    fn get_game_state(&self) -> &GameState {
        &self.state
    }

    fn save_game(&self) -> Result<SaveData, String> {
        // Create save data specific to RPG
        let save_data = SaveData {
            player_position: self.config.player_start_position, // Would get actual position
            player_health: self.player_stats.health,
            inventory: vec![], // Would populate from actual inventory
            gold: 0, // Would get from player stats
            completed_quests: vec![], // Would populate from quest system
            active_quests: self.active_quests.iter().map(|q| q.id.clone()).collect(),
            game_stats: GameStatsData {
                play_time_secs: 0, // Would get from framework
                enemies_defeated: self.combat_system.combat_stats.battles_won,
                items_collected: 0,
                quests_completed: 0,
                distance_traveled: 0.0,
                deaths: self.combat_system.combat_stats.battles_lost,
                level: self.player_stats.level,
                experience: self.player_stats.experience,
            },
            dialogue_variables: HashMap::new(),
            current_scene: self.current_level.clone(),
        };
        Ok(save_data)
    }

    fn load_game(&mut self, save_data: SaveData) -> Result<(), String> {
        self.player_stats.health = save_data.player_health;
        self.player_stats.level = save_data.game_stats.level;
        self.player_stats.experience = save_data.game_stats.experience;
        self.current_level = save_data.current_scene;
        self.combat_system.combat_stats.battles_won = save_data.game_stats.enemies_defeated;
        self.combat_system.combat_stats.battles_lost = save_data.game_stats.deaths;
        
        println!("Game loaded successfully");
        Ok(())
    }

    fn get_ui_elements(&self) -> Vec<UIElement> {
        self.ui_elements.clone()
    }

    fn cleanup(&mut self) {
        println!("RPG game cleanup");
    }
}

// Platformer Game Template
pub struct PlatformerGameTemplate;

impl GameTemplateBuilder for PlatformerGameTemplate {
    fn create_game(&self, config: GameConfig) -> Result<Box<dyn GameInstance>, String> {
        Ok(Box::new(PlatformerGame::new(config)?))
    }

    fn get_template_name(&self) -> &str {
        "2D Platformer"
    }

    fn get_description(&self) -> &str {
        "A classic 2D platformer with jumping, collectibles, and enemies"
    }

    fn get_required_assets(&self) -> Vec<String> {
        vec![
            "sprites/player/".to_string(),
            "sprites/enemies/".to_string(),
            "sprites/collectibles/".to_string(),
            "sprites/platforms/".to_string(),
            "audio/jump.wav".to_string(),
            "audio/collect.wav".to_string(),
        ]
    }
}

pub struct PlatformerGame {
    config: GameConfig,
    state: GameState,
    lives: i32,
    score: i32,
    current_level: i32,
    checkpoints: Vec<Vector3D>,
    last_checkpoint: Option<Vector3D>,
    power_ups: Vec<PowerUp>,
    ui_elements: Vec<UIElement>,
}

#[derive(Debug, Clone)]
pub struct PowerUp {
    pub power_type: PowerUpType,
    pub duration: f32,
    pub remaining_time: f32,
    pub active: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum PowerUpType {
    SpeedBoost,
    JumpBoost,
    Invincibility,
    DoubleJump,
    FirePower,
    Shield,
}

impl PlatformerGame {
    pub fn new(config: GameConfig) -> Result<Self, String> {
        let ui_elements = vec![
            UIElement {
                id: "score".to_string(),
                element_type: UIElementType::Text,
                position: (10.0, 10.0),
                size: (100.0, 30.0),
                visible: true,
                interactive: false,
                style: UIStyle {
                    background_color: (0.0, 0.0, 0.0, 0.0),
                    border_color: (0.0, 0.0, 0.0, 0.0),
                    text_color: (1.0, 1.0, 1.0, 1.0),
                    font_size: 24.0,
                    padding: (0.0, 0.0, 0.0, 0.0),
                    margin: (0.0, 0.0, 0.0, 0.0),
                },
                content: UIContent::Text("Score: 0".to_string()),
            },
            UIElement {
                id: "lives".to_string(),
                element_type: UIElementType::Text,
                position: (10.0, 50.0),
                size: (100.0, 30.0),
                visible: true,
                interactive: false,
                style: UIStyle {
                    background_color: (0.0, 0.0, 0.0, 0.0),
                    border_color: (0.0, 0.0, 0.0, 0.0),
                    text_color: (1.0, 1.0, 1.0, 1.0),
                    font_size: 20.0,
                    padding: (0.0, 0.0, 0.0, 0.0),
                    margin: (0.0, 0.0, 0.0, 0.0),
                },
                content: UIContent::Text("Lives: 3".to_string()),
            },
        ];

        Ok(Self {
            config,
            state: GameState::Playing,
            lives: 3,
            score: 0,
            current_level: 1,
            checkpoints: vec![],
            last_checkpoint: None,
            power_ups: vec![],
            ui_elements,
        })
    }

    pub fn add_score(&mut self, points: i32) {
        self.score += points;
        
        // Update UI
        for ui_element in &mut self.ui_elements {
            if ui_element.id == "score" {
                ui_element.content = UIContent::Text(format!("Score: {}", self.score));
            }
        }
    }

    pub fn lose_life(&mut self) {
        self.lives -= 1;
        
        // Update UI
        for ui_element in &mut self.ui_elements {
            if ui_element.id == "lives" {
                ui_element.content = UIContent::Text(format!("Lives: {}", self.lives));
            }
        }
        
        if self.lives <= 0 {
            self.state = GameState::GameOver;
        } else {
            // Respawn at last checkpoint
            if let Some(_checkpoint) = self.last_checkpoint {
                // Reset player position to checkpoint
                println!("Respawning at checkpoint");
            }
        }
    }

    pub fn activate_power_up(&mut self, power_type: PowerUpType, duration: f32) {
        // Deactivate existing power-up of same type
        for power_up in &mut self.power_ups {
            if power_up.power_type == power_type {
                power_up.active = false;
            }
        }
        
        // Add new power-up
        self.power_ups.push(PowerUp {
            power_type,
            duration,
            remaining_time: duration,
            active: true,
        });
        
        println!("Power-up activated: {:?}", power_type);
    }

    pub fn update_power_ups(&mut self, delta_time: f32) {
        for power_up in &mut self.power_ups {
            if power_up.active {
                power_up.remaining_time -= delta_time;
                if power_up.remaining_time <= 0.0 {
                    power_up.active = false;
                    println!("Power-up expired: {:?}", power_up.power_type);
                }
            }
        }
        
        // Remove expired power-ups
        self.power_ups.retain(|p| p.active);
    }
}

impl GameInstance for PlatformerGame {
    fn initialize(&mut self, scene: &mut Scene) -> Result<(), String> {
        // Create player
        let _player_id = scene.add_player(self.config.player_start_position);
        
        // Create level geometry
        self.create_level_1(scene)?;
        
        println!("Platformer game initialized");
        Ok(())
    }

    fn update(&mut self, delta_time: f32, input: &InputManager, _framework: &mut GameFramework) -> GameUpdateResult {
        match self.state {
            GameState::Playing => {
                if input.is_key_pressed(Key::Escape) {
                    return GameUpdateResult::StateChange(GameState::Paused);
                }

                // Update power-ups
                self.update_power_ups(delta_time);

                GameUpdateResult::Continue
            }
            GameState::Paused => {
                if input.is_key_pressed(Key::Escape) {
                    return GameUpdateResult::StateChange(GameState::Playing);
                }
                GameUpdateResult::Continue
            }
            GameState::GameOver => {
                if input.is_key_pressed(Key::R) {
                    return GameUpdateResult::Restart;
                }
                if input.is_key_pressed(Key::Q) {
                    return GameUpdateResult::Quit;
                }
                GameUpdateResult::Continue
            }
            _ => GameUpdateResult::Continue,
        }
    }

    fn handle_event(&mut self, event: GameEvent) -> Result<(), String> {
        match event {
            GameEvent::ItemCollected { item_id, quantity } => {
                match item_id.as_str() {
                    "coin" => self.add_score(quantity * 10),
                    "gem" => self.add_score(quantity * 50),
                    "power_up_speed" => self.activate_power_up(PowerUpType::SpeedBoost, 10.0),
                    "power_up_jump" => self.activate_power_up(PowerUpType::JumpBoost, 15.0),
                    "1up" => {
                        self.lives += 1;
                        for ui_element in &mut self.ui_elements {
                            if ui_element.id == "lives" {
                                ui_element.content = UIContent::Text(format!("Lives: {}", self.lives));
                            }
                        }
                    }
                    _ => {}
                }
            }
            GameEvent::PlayerDied => {
                self.lose_life();
            }
            GameEvent::CheckpointReached { checkpoint_id: _ } => {
                // Save checkpoint
                self.last_checkpoint = Some(self.config.player_start_position); // Would use actual position
                println!("Checkpoint reached!");
            }
            GameEvent::LevelCompleted => {
                self.current_level += 1;
                self.add_score(1000);
                println!("Level {} completed!", self.current_level - 1);
            }
            _ => {}
        }
        Ok(())
    }

    fn get_game_state(&self) -> &GameState {
        &self.state
    }

    fn save_game(&self) -> Result<SaveData, String> {
        let save_data = SaveData {
            player_position: self.last_checkpoint.unwrap_or(self.config.player_start_position),
            player_health: self.lives,
            inventory: vec![],
            gold: self.score,
            completed_quests: vec![],
            active_quests: vec![],
            game_stats: GameStatsData {
                play_time_secs: 0,
                enemies_defeated: 0,
                items_collected: 0,
                quests_completed: 0,
                distance_traveled: 0.0,
                deaths: 0,
                level: self.current_level,
                experience: self.score,
            },
            dialogue_variables: HashMap::new(),
            current_scene: format!("level_{}", self.current_level),
        };
        Ok(save_data)
    }

    fn load_game(&mut self, save_data: SaveData) -> Result<(), String> {
        self.lives = save_data.player_health;
        self.score = save_data.gold;
        self.current_level = save_data.game_stats.level;
        self.last_checkpoint = Some(save_data.player_position);
        
        println!("Platformer game loaded");
        Ok(())
    }

    fn get_ui_elements(&self) -> Vec<UIElement> {
        self.ui_elements.clone()
    }

    fn cleanup(&mut self) {
        println!("Platformer game cleanup");
    }
}

impl PlatformerGame {
    fn create_level_1(&mut self, scene: &mut Scene) -> Result<(), String> {
        // Create platforms
        let platforms = vec![
            (Vector3D::new(0.0, 0.0, 0.0), Vector3D::new(20.0, 1.0, 2.0)),   // Ground
            (Vector3D::new(25.0, 3.0, 0.0), Vector3D::new(5.0, 1.0, 2.0)),   // Platform 1
            (Vector3D::new(35.0, 6.0, 0.0), Vector3D::new(5.0, 1.0, 2.0)),   // Platform 2
            (Vector3D::new(45.0, 9.0, 0.0), Vector3D::new(5.0, 1.0, 2.0)),   // Platform 3
        ];

        for (position, size) in platforms {
            scene.add_platform(position, Vector3D::zero());
        }

        // Add collectibles
        let collectibles = vec![
            (Vector3D::new(5.0, 2.0, 0.0), 10),   // Coin
            (Vector3D::new(15.0, 2.0, 0.0), 10),  // Coin
            (Vector3D::new(27.0, 5.0, 0.0), 50),  // Gem
            (Vector3D::new(37.0, 8.0, 0.0), 50),  // Gem
            (Vector3D::new(47.0, 11.0, 0.0), 100), // Special item
        ];

        for (position, value) in collectibles {
            scene.add_collectible(position, value);
        }

        // Add checkpoints
        self.checkpoints = vec![
            Vector3D::new(10.0, 2.0, 0.0),
            Vector3D::new(30.0, 5.0, 0.0),
            Vector3D::new(50.0, 11.0, 0.0),
        ];

        Ok(())
    }
}

// Game Template Factory
pub struct GameTemplateFactory;

impl GameTemplateFactory {
    pub fn create_template(template_type: GameTemplate) -> Box<dyn GameTemplateBuilder> {
        match template_type {
            GameTemplate::RPG3D => Box::new(RPGGameTemplate),
            GameTemplate::Platformer2D => Box::new(PlatformerGameTemplate),
            // Add more templates as needed
            _ => Box::new(RPGGameTemplate), // Default to RPG for now
        }
    }

    pub fn get_available_templates() -> Vec<(GameTemplate, &'static str, &'static str)> {
        vec![
            (GameTemplate::RPG3D, "3D RPG Adventure", "A classic 3D RPG with character progression, quests, and combat"),
            (GameTemplate::Platformer2D, "2D Platformer", "A classic 2D platformer with jumping and collectibles"),
            (GameTemplate::FirstPersonShooter, "First Person Shooter", "Fast-paced FPS action"),
            (GameTemplate::RacingGame, "Racing Game", "High-speed racing with customizable vehicles"),
            (GameTemplate::PuzzleGame, "Puzzle Game", "Brain-teasing puzzles and challenges"),
            (GameTemplate::SurvivalGame, "Survival Game", "Survive in harsh environments with crafting"),
            (GameTemplate::AdventureGame, "Adventure Game", "Story-driven adventure with exploration"),
            (GameTemplate::StrategyGame, "Strategy Game", "Tactical gameplay with resource management"),
        ]
    }
}