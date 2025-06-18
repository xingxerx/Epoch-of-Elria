// Game Framework - High-level game development utilities
use crate::math::Vector3D;
use crate::game_objects::{GameObject, Player, Collectible, Enemy, Platform};
use crate::scene::Scene;
use crate::input::{InputManager, Key};
use std::collections::HashMap;
use std::time::{Duration, Instant};

// Game state management
#[derive(Debug, Clone, PartialEq)]
pub enum GameState {
    MainMenu,
    Playing,
    Paused,
    GameOver,
    Victory,
    Loading,
    Settings,
}

// Quest system
#[derive(Debug, Clone)]
pub struct Quest {
    pub id: String,
    pub title: String,
    pub description: String,
    pub objectives: Vec<QuestObjective>,
    pub rewards: Vec<QuestReward>,
    pub is_completed: bool,
    pub is_active: bool,
}

#[derive(Debug, Clone)]
pub struct QuestObjective {
    pub id: String,
    pub description: String,
    pub target_type: String,
    pub target_count: i32,
    pub current_count: i32,
    pub is_completed: bool,
}

#[derive(Debug, Clone)]
pub struct QuestReward {
    pub reward_type: String,
    pub amount: i32,
    pub item_id: Option<String>,
}

// Inventory system
#[derive(Debug, Clone)]
pub struct Item {
    pub id: String,
    pub name: String,
    pub description: String,
    pub item_type: ItemType,
    pub rarity: ItemRarity,
    pub value: i32,
    pub stackable: bool,
    pub max_stack: i32,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ItemType {
    Weapon,
    Armor,
    Consumable,
    Material,
    Quest,
    Misc,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ItemRarity {
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary,
}

#[derive(Debug, Clone)]
pub struct InventorySlot {
    pub item: Option<Item>,
    pub quantity: i32,
}

pub struct Inventory {
    pub slots: Vec<InventorySlot>,
    pub max_slots: usize,
    pub gold: i32,
}

impl Inventory {
    pub fn new(max_slots: usize) -> Self {
        Self {
            slots: vec![InventorySlot { item: None, quantity: 0 }; max_slots],
            max_slots,
            gold: 0,
        }
    }

    pub fn add_item(&mut self, item: Item, quantity: i32) -> bool {
        // Try to stack with existing items first
        if item.stackable {
            for slot in &mut self.slots {
                if let Some(existing_item) = &slot.item {
                    if existing_item.id == item.id && slot.quantity < item.max_stack {
                        let can_add = (item.max_stack - slot.quantity).min(quantity);
                        slot.quantity += can_add;
                        if can_add == quantity {
                            return true;
                        }
                    }
                }
            }
        }

        // Find empty slot
        for slot in &mut self.slots {
            if slot.item.is_none() {
                slot.item = Some(item);
                slot.quantity = quantity;
                return true;
            }
        }

        false // Inventory full
    }

    pub fn remove_item(&mut self, item_id: &str, quantity: i32) -> bool {
        let mut remaining = quantity;
        
        for slot in &mut self.slots {
            if let Some(item) = &slot.item {
                if item.id == item_id {
                    let can_remove = slot.quantity.min(remaining);
                    slot.quantity -= can_remove;
                    remaining -= can_remove;
                    
                    if slot.quantity == 0 {
                        slot.item = None;
                    }
                    
                    if remaining == 0 {
                        return true;
                    }
                }
            }
        }
        
        false
    }

    pub fn has_item(&self, item_id: &str, quantity: i32) -> bool {
        let mut total = 0;
        for slot in &self.slots {
            if let Some(item) = &slot.item {
                if item.id == item_id {
                    total += slot.quantity;
                    if total >= quantity {
                        return true;
                    }
                }
            }
        }
        false
    }
}

// Dialogue system
#[derive(Debug, Clone)]
pub struct DialogueNode {
    pub id: String,
    pub speaker: String,
    pub text: String,
    pub choices: Vec<DialogueChoice>,
    pub conditions: Vec<DialogueCondition>,
    pub actions: Vec<DialogueAction>,
}

#[derive(Debug, Clone)]
pub struct DialogueChoice {
    pub text: String,
    pub next_node_id: String,
    pub conditions: Vec<DialogueCondition>,
}

#[derive(Debug, Clone)]
pub struct DialogueCondition {
    pub condition_type: String,
    pub target: String,
    pub value: String,
}

#[derive(Debug, Clone)]
pub struct DialogueAction {
    pub action_type: String,
    pub target: String,
    pub value: String,
}

pub struct DialogueSystem {
    pub current_dialogue: Option<String>,
    pub current_node: Option<DialogueNode>,
    pub dialogue_trees: HashMap<String, Vec<DialogueNode>>,
    pub variables: HashMap<String, String>,
}

impl DialogueSystem {
    pub fn new() -> Self {
        Self {
            current_dialogue: None,
            current_node: None,
            dialogue_trees: HashMap::new(),
            variables: HashMap::new(),
        }
    }

    pub fn start_dialogue(&mut self, dialogue_id: &str) {
        self.current_dialogue = Some(dialogue_id.to_string());
        if let Some(nodes) = self.dialogue_trees.get(dialogue_id) {
            if let Some(first_node) = nodes.first() {
                self.current_node = Some(first_node.clone());
            }
        }
    }

    pub fn choose_option(&mut self, choice_index: usize) {
        if let Some(node) = &self.current_node {
            if choice_index < node.choices.len() {
                let choice = &node.choices[choice_index];
                let next_node_id = choice.next_node_id.clone();

                // Clone actions to avoid borrowing issues
                let actions = node.actions.clone();

                // Execute actions
                for action in &actions {
                    self.execute_action(action);
                }
                
                // Move to next node
                if let Some(dialogue_id) = &self.current_dialogue {
                    if let Some(nodes) = self.dialogue_trees.get(dialogue_id) {
                        if let Some(next_node) = nodes.iter().find(|n| n.id == next_node_id) {
                            self.current_node = Some(next_node.clone());
                        } else {
                            // End dialogue
                            self.current_dialogue = None;
                            self.current_node = None;
                        }
                    }
                }
            }
        }
    }

    fn execute_action(&mut self, action: &DialogueAction) {
        match action.action_type.as_str() {
            "set_variable" => {
                self.variables.insert(action.target.clone(), action.value.clone());
            }
            "give_item" => {
                // This would integrate with the inventory system
                println!("Giving item: {} x{}", action.target, action.value);
            }
            "start_quest" => {
                // This would integrate with the quest system
                println!("Starting quest: {}", action.target);
            }
            _ => {}
        }
    }
}

// Game statistics and progression
#[derive(Debug, Clone)]
pub struct GameStats {
    pub play_time: Duration,
    pub enemies_defeated: i32,
    pub items_collected: i32,
    pub quests_completed: i32,
    pub distance_traveled: f32,
    pub deaths: i32,
    pub level: i32,
    pub experience: i32,
    pub experience_to_next_level: i32,
}

impl GameStats {
    pub fn new() -> Self {
        Self {
            play_time: Duration::new(0, 0),
            enemies_defeated: 0,
            items_collected: 0,
            quests_completed: 0,
            distance_traveled: 0.0,
            deaths: 0,
            level: 1,
            experience: 0,
            experience_to_next_level: 100,
        }
    }

    pub fn add_experience(&mut self, amount: i32) {
        self.experience += amount;
        while self.experience >= self.experience_to_next_level {
            self.level_up();
        }
    }

    fn level_up(&mut self) {
        self.experience -= self.experience_to_next_level;
        self.level += 1;
        self.experience_to_next_level = (self.level * 100) + (self.level * self.level * 10);
        println!("Level up! Now level {}", self.level);
    }
}

// Save/Load system
use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize)]
pub struct SaveData {
    pub player_position: Vector3D,
    pub player_health: i32,
    pub inventory: Vec<(String, i32)>, // (item_id, quantity)
    pub gold: i32,
    pub completed_quests: Vec<String>,
    pub active_quests: Vec<String>,
    pub game_stats: GameStatsData,
    pub dialogue_variables: HashMap<String, String>,
    pub current_scene: String,
}

#[derive(Serialize, Deserialize)]
pub struct GameStatsData {
    pub play_time_secs: u64,
    pub enemies_defeated: i32,
    pub items_collected: i32,
    pub quests_completed: i32,
    pub distance_traveled: f32,
    pub deaths: i32,
    pub level: i32,
    pub experience: i32,
}

pub struct SaveSystem;

impl SaveSystem {
    pub fn save_game(save_data: &SaveData, slot: u8) -> Result<(), Box<dyn std::error::Error>> {
        let filename = format!("save_slot_{}.json", slot);
        let json = serde_json::to_string_pretty(save_data)?;
        std::fs::write(filename, json)?;
        Ok(())
    }

    pub fn load_game(slot: u8) -> Result<SaveData, Box<dyn std::error::Error>> {
        let filename = format!("save_slot_{}.json", slot);
        let json = std::fs::read_to_string(filename)?;
        let save_data: SaveData = serde_json::from_str(&json)?;
        Ok(save_data)
    }

    pub fn save_exists(slot: u8) -> bool {
        let filename = format!("save_slot_{}.json", slot);
        std::path::Path::new(&filename).exists()
    }
}

// Main Game Framework
pub struct GameFramework {
    pub state: GameState,
    pub previous_state: GameState,
    pub inventory: Inventory,
    pub dialogue_system: DialogueSystem,
    pub quest_system: Vec<Quest>,
    pub stats: GameStats,
    pub start_time: Instant,
    pub paused_time: Duration,
    pub settings: GameSettings,
}

#[derive(Debug, Clone)]
pub struct GameSettings {
    pub master_volume: f32,
    pub music_volume: f32,
    pub sfx_volume: f32,
    pub graphics_quality: GraphicsQuality,
    pub fullscreen: bool,
    pub vsync: bool,
    pub show_fps: bool,
    pub difficulty: Difficulty,
}

#[derive(Debug, Clone, PartialEq)]
pub enum GraphicsQuality {
    Low,
    Medium,
    High,
    Ultra,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Difficulty {
    Easy,
    Normal,
    Hard,
    Nightmare,
}

impl GameFramework {
    pub fn new() -> Self {
        Self {
            state: GameState::MainMenu,
            previous_state: GameState::MainMenu,
            inventory: Inventory::new(30),
            dialogue_system: DialogueSystem::new(),
            quest_system: Vec::new(),
            stats: GameStats::new(),
            start_time: Instant::now(),
            paused_time: Duration::new(0, 0),
            settings: GameSettings {
                master_volume: 1.0,
                music_volume: 0.8,
                sfx_volume: 1.0,
                graphics_quality: GraphicsQuality::High,
                fullscreen: false,
                vsync: true,
                show_fps: false,
                difficulty: Difficulty::Normal,
            },
        }
    }

    pub fn change_state(&mut self, new_state: GameState) {
        self.previous_state = self.state.clone();
        self.state = new_state;
        
        match self.state {
            GameState::Playing => {
                if self.previous_state == GameState::Paused {
                    // Resume from pause
                    self.start_time = Instant::now() - (self.stats.play_time - self.paused_time);
                }
            }
            GameState::Paused => {
                self.paused_time = self.stats.play_time;
            }
            _ => {}
        }
    }

    pub fn update(&mut self, delta_time: f32) {
        // Update play time
        if self.state == GameState::Playing {
            self.stats.play_time = self.start_time.elapsed();
        }

        // Update quest system
        self.update_quests();
    }

    fn update_quests(&mut self) {
        for quest in &mut self.quest_system {
            if quest.is_active && !quest.is_completed {
                // Check if all objectives are completed
                let all_completed = quest.objectives.iter().all(|obj| obj.is_completed);
                if all_completed {
                    quest.is_completed = true;
                    self.stats.quests_completed += 1;
                    println!("Quest completed: {}", quest.title);
                    
                    // Give rewards
                    for reward in &quest.rewards {
                        match reward.reward_type.as_str() {
                            "experience" => self.stats.add_experience(reward.amount),
                            "gold" => self.inventory.gold += reward.amount,
                            "item" => {
                                if let Some(item_id) = &reward.item_id {
                                    // This would need to integrate with item database
                                    println!("Rewarded item: {} x{}", item_id, reward.amount);
                                }
                            }
                            _ => {}
                        }
                    }
                }
            }
        }
    }

    pub fn add_quest(&mut self, quest: Quest) {
        self.quest_system.push(quest);
    }

    pub fn update_quest_objective(&mut self, quest_id: &str, objective_id: &str, progress: i32) {
        for quest in &mut self.quest_system {
            if quest.id == quest_id && quest.is_active {
                for objective in &mut quest.objectives {
                    if objective.id == objective_id {
                        objective.current_count += progress;
                        if objective.current_count >= objective.target_count {
                            objective.is_completed = true;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    pub fn create_save_data(&self, player_position: Vector3D, player_health: i32, current_scene: &str) -> SaveData {
        let inventory_items: Vec<(String, i32)> = self.inventory.slots
            .iter()
            .filter_map(|slot| {
                if let Some(item) = &slot.item {
                    Some((item.id.clone(), slot.quantity))
                } else {
                    None
                }
            })
            .collect();

        let completed_quests: Vec<String> = self.quest_system
            .iter()
            .filter(|q| q.is_completed)
            .map(|q| q.id.clone())
            .collect();

        let active_quests: Vec<String> = self.quest_system
            .iter()
            .filter(|q| q.is_active && !q.is_completed)
            .map(|q| q.id.clone())
            .collect();

        SaveData {
            player_position,
            player_health,
            inventory: inventory_items,
            gold: self.inventory.gold,
            completed_quests,
            active_quests,
            game_stats: GameStatsData {
                play_time_secs: self.stats.play_time.as_secs(),
                enemies_defeated: self.stats.enemies_defeated,
                items_collected: self.stats.items_collected,
                quests_completed: self.stats.quests_completed,
                distance_traveled: self.stats.distance_traveled,
                deaths: self.stats.deaths,
                level: self.stats.level,
                experience: self.stats.experience,
            },
            dialogue_variables: self.dialogue_system.variables.clone(),
            current_scene: current_scene.to_string(),
        }
    }
}