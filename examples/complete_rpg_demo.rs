// Complete RPG Demo - Showcasing all engine features
use epoch_of_elria::*;
use epoch_of_elria::game_framework::*;
use epoch_of_elria::game_templates::*;
use epoch_of_elria::advanced_rendering::*;
use epoch_of_elria::input::Key;
use std::collections::HashMap;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("🎮 Epoch of Elria - Complete RPG Demo");
    println!("=====================================");
    
    // Initialize the game engine
    let config = EngineConfig {
        window_width: 1280,
        window_height: 720,
        window_title: "Epoch of Elria - RPG Demo".to_string(),
        vsync: true,
        fullscreen: false,
        max_fps: Some(60),
        enable_physics: true,
        enable_audio: true,
        debug_mode: true,
    };

    let mut engine = GameEngine::new(config)?;
    
    // Create game framework
    let mut framework = GameFramework::new();
    
    // Setup game template
    let game_config = GameConfig {
        title: "The Legend of Elria".to_string(),
        world_size: Vector3D::new(200.0, 50.0, 200.0),
        player_start_position: Vector3D::new(0.0, 2.0, 0.0),
        difficulty: Difficulty::Normal,
        game_mode: GameMode::SinglePlayer,
        max_players: 1,
        enable_multiplayer: false,
        custom_settings: HashMap::new(),
    };

    // Create RPG game instance
    let template = GameTemplateFactory::create_template(GameTemplate::RPG3D);
    let mut game_instance = template.create_game(game_config)?;
    
    // Initialize the game
    game_instance.initialize(engine.get_scene())?;
    
    // Setup initial quests
    setup_initial_quests(&mut framework)?;
    
    // Setup inventory with starting items
    setup_starting_inventory(&mut framework)?;
    
    // Setup dialogue system
    setup_dialogue_system(&mut framework)?;
    
    println!("✅ Game initialized successfully!");
    println!("\n🎯 Controls:");
    println!("  WASD - Move");
    println!("  Space - Jump");
    println!("  Tab - Show Stats");
    println!("  I - Toggle Inventory");
    println!("  M - Toggle Map");
    println!("  ESC - Pause/Menu");
    println!("  R - Restart (when game over)");
    println!("  Q - Quit (when game over)");
    println!("\n🚀 Starting adventure...\n");

    // Game loop
    let mut frame_count = 0;
    let mut last_stats_time = std::time::Instant::now();
    
    engine.run(move |scene, input, delta_time| {
        frame_count += 1;
        
        // Update framework systems
        framework.update(delta_time);
        
        // Update game instance
        let update_result = game_instance.update(delta_time, input, &mut framework);
        
        // Handle game state changes
        match update_result {
            GameUpdateResult::Continue => {
                // Continue normal gameplay
            }
            GameUpdateResult::StateChange(new_state) => {
                println!("Game state changed to: {:?}", new_state);
            }
            GameUpdateResult::Victory => {
                println!("🎉 Victory! You have completed the game!");
                // Could transition to victory screen
            }
            GameUpdateResult::GameOver => {
                println!("💀 Game Over! Press R to restart or Q to quit.");
            }
            GameUpdateResult::Restart => {
                println!("🔄 Restarting game...");
                // Reset game state
                let _ = game_instance.initialize(scene);
            }
            GameUpdateResult::Quit => {
                println!("👋 Thanks for playing!");
                std::process::exit(0);
            }
        }
        
        // Update all game objects
        for object in scene.get_objects_mut() {
            object.update(delta_time);
        }
        
        // Handle player movement
        handle_player_input(scene, input, delta_time);
        
        // Check for collisions and handle events
        handle_game_events(scene, &mut game_instance, &mut framework);
        
        // Update UI elements
        update_ui_elements(&game_instance, &framework);
        
        // Show periodic stats
        if last_stats_time.elapsed().as_secs() >= 10 {
            show_game_stats(&framework, frame_count);
            last_stats_time = std::time::Instant::now();
            frame_count = 0;
        }
    })?;

    println!("🎮 Game ended. Thanks for playing!");
    Ok(())
}

fn setup_initial_quests(framework: &mut GameFramework) -> Result<(), Box<dyn std::error::Error>> {
    // Create the tutorial quest
    let tutorial_quest = Quest {
        id: "tutorial_complete".to_string(),
        title: "Welcome to Elria".to_string(),
        description: "Learn the basics of your adventure. Collect 3 coins and talk to the Village Elder.".to_string(),
        objectives: vec![
            QuestObjective {
                id: "collect_coins".to_string(),
                description: "Collect 3 coins".to_string(),
                objective_type: "collect".to_string(),
                target: "coin".to_string(),
                current_count: 0,
                required_count: 3,
                completed: false,
            },
            QuestObjective {
                id: "talk_to_elder".to_string(),
                description: "Speak with the Village Elder".to_string(),
                objective_type: "talk".to_string(),
                target: "village_elder".to_string(),
                current_count: 0,
                required_count: 1,
                completed: false,
            },
        ],
        rewards: vec![
            QuestReward {
                reward_type: "experience".to_string(),
                amount: 100,
                item_id: None,
            },
            QuestReward {
                reward_type: "item".to_string(),
                amount: 1,
                item_id: Some("iron_sword".to_string()),
            },
        ],
        status: QuestStatus::Available,
        required_level: 1,
        required_quests: vec![],
        giver_npc: Some("village_elder".to_string()),
    };
    
    framework.quest_system.add_quest(tutorial_quest);
    
    // Create exploration quest
    let exploration_quest = Quest {
        id: "explore_forest".to_string(),
        title: "Into the Whispering Woods".to_string(),
        description: "Venture into the mysterious forest and discover its secrets.".to_string(),
        objectives: vec![
            QuestObjective {
                id: "enter_forest".to_string(),
                description: "Enter the Whispering Woods".to_string(),
                objective_type: "location".to_string(),
                target: "whispering_woods".to_string(),
                current_count: 0,
                required_count: 1,
                completed: false,
            },
            QuestObjective {
                id: "defeat_wolves".to_string(),
                description: "Defeat 5 wolves".to_string(),
                objective_type: "kill".to_string(),
                target: "wolf".to_string(),
                current_count: 0,
                required_count: 5,
                completed: false,
            },
        ],
        rewards: vec![
            QuestReward {
                reward_type: "experience".to_string(),
                amount: 250,
                item_id: None,
            },
            QuestReward {
                reward_type: "gold".to_string(),
                amount: 50,
                item_id: None,
            },
        ],
        status: QuestStatus::Locked,
        required_level: 2,
        required_quests: vec!["tutorial_complete".to_string()],
        giver_npc: Some("forest_ranger".to_string()),
    };
    
    framework.quest_system.add_quest(exploration_quest);
    
    println!("📜 Initial quests created");
    Ok(())
}

fn setup_starting_inventory(framework: &mut GameFramework) -> Result<(), Box<dyn std::error::Error>> {
    // Add starting items
    let starting_items = vec![
        Item {
            id: "health_potion".to_string(),
            name: "Health Potion".to_string(),
            description: "Restores 50 health points".to_string(),
            item_type: ItemType::Consumable,
            rarity: ItemRarity::Common,
            value: 25,
            weight: 0.5,
            max_stack: 10,
            usable: true,
            equippable: false,
            stats_modifier: HashMap::new(),
            icon_path: Some("icons/health_potion.png".to_string()),
        },
        Item {
            id: "rusty_sword".to_string(),
            name: "Rusty Sword".to_string(),
            description: "An old sword, but still sharp enough to be useful".to_string(),
            item_type: ItemType::Weapon,
            rarity: ItemRarity::Common,
            value: 15,
            weight: 2.0,
            max_stack: 1,
            usable: false,
            equippable: true,
            stats_modifier: {
                let mut stats = HashMap::new();
                stats.insert("attack".to_string(), 10);
                stats
            },
            icon_path: Some("icons/rusty_sword.png".to_string()),
        },
        Item {
            id: "leather_armor".to_string(),
            name: "Leather Armor".to_string(),
            description: "Basic protection for novice adventurers".to_string(),
            item_type: ItemType::Armor,
            rarity: ItemRarity::Common,
            value: 30,
            weight: 3.0,
            max_stack: 1,
            usable: false,
            equippable: true,
            stats_modifier: {
                let mut stats = HashMap::new();
                stats.insert("defense".to_string(), 5);
                stats
            },
            icon_path: Some("icons/leather_armor.png".to_string()),
        },
    ];
    
    for item in starting_items {
        framework.inventory.add_item(item, 1)?;
    }
    
    // Add some starting gold
    framework.inventory.gold = 100;
    
    println!("🎒 Starting inventory setup complete");
    Ok(())
}

fn setup_dialogue_system(framework: &mut GameFramework) -> Result<(), Box<dyn std::error::Error>> {
    // Create dialogue for Village Elder
    let elder_dialogue = DialogueNode {
        id: "elder_intro".to_string(),
        speaker: "Village Elder".to_string(),
        text: "Welcome, young adventurer! I sense great potential in you. Are you ready to begin your journey?".to_string(),
        choices: vec![
            DialogueChoice {
                text: "Yes, I'm ready for adventure!".to_string(),
                next_node: Some("elder_quest_offer".to_string()),
                conditions: vec![],
                actions: vec![],
            },
            DialogueChoice {
                text: "Tell me about this place.".to_string(),
                next_node: Some("elder_village_info".to_string()),
                conditions: vec![],
                actions: vec![],
            },
            DialogueChoice {
                text: "Not yet, I need to prepare.".to_string(),
                next_node: None,
                conditions: vec![],
                actions: vec![],
            },
        ],
        conditions: vec![],
        actions: vec![],
    };
    
    framework.dialogue_system.add_dialogue_node(elder_dialogue);
    
    // Add quest offer dialogue
    let quest_offer = DialogueNode {
        id: "elder_quest_offer".to_string(),
        speaker: "Village Elder".to_string(),
        text: "Excellent! Your first task is simple: collect some coins around the village and return to me. This will help you get familiar with the area.".to_string(),
        choices: vec![
            DialogueChoice {
                text: "I accept this quest.".to_string(),
                next_node: None,
                conditions: vec![],
                actions: vec![
                    DialogueAction::StartQuest("tutorial_complete".to_string()),
                ],
            },
            DialogueChoice {
                text: "Maybe later.".to_string(),
                next_node: None,
                conditions: vec![],
                actions: vec![],
            },
        ],
        conditions: vec![
            DialogueCondition::QuestNotStarted("tutorial_complete".to_string()),
        ],
        actions: vec![],
    };
    
    framework.dialogue_system.add_dialogue_node(quest_offer);
    
    // Add village info dialogue
    let village_info = DialogueNode {
        id: "elder_village_info".to_string(),
        speaker: "Village Elder".to_string(),
        text: "This is the village of Elria, a peaceful place at the edge of the Whispering Woods. Many adventurers have started their journeys here. The forest holds many secrets and dangers.".to_string(),
        choices: vec![
            DialogueChoice {
                text: "What kind of dangers?".to_string(),
                next_node: Some("elder_dangers".to_string()),
                conditions: vec![],
                actions: vec![],
            },
            DialogueChoice {
                text: "I'm ready to start my quest.".to_string(),
                next_node: Some("elder_quest_offer".to_string()),
                conditions: vec![],
                actions: vec![],
            },
        ],
        conditions: vec![],
        actions: vec![],
    };
    
    framework.dialogue_system.add_dialogue_node(village_info);
    
    println!("💬 Dialogue system setup complete");
    Ok(())
}

fn handle_player_input(scene: &mut Scene, input: &InputManager, delta_time: f32) {
    let movement_speed = 10.0;
    let mut movement = Vector3D::zero();
    
    // Handle movement input
    if input.is_key_held(Key::W) {
        movement.z -= 1.0;
    }
    if input.is_key_held(Key::S) {
        movement.z += 1.0;
    }
    if input.is_key_held(Key::A) {
        movement.x -= 1.0;
    }
    if input.is_key_held(Key::D) {
        movement.x += 1.0;
    }
    
    // Normalize movement vector
    if movement.magnitude() > 0.0 {
        movement = movement.normalize() * movement_speed * delta_time;
        
        // Apply movement to player
        if let Some(player) = scene.get_objects_mut().first_mut() {
            player.set_position(player.get_position() + movement);
        }
    }
    
    // Handle jump
    if input.is_key_pressed(Key::Space) {
        if let Some(player) = scene.get_objects_mut().first_mut() {
            // Simple jump - add upward velocity
            let current_pos = player.get_position();
            player.set_position(Vector3D::new(current_pos.x, current_pos.y + 0.1, current_pos.z));
        }
    }
}

fn handle_game_events(
    scene: &mut Scene, 
    game_instance: &mut Box<dyn GameInstance>, 
    framework: &mut GameFramework
) {
    // Check for collectible collisions
    let player_pos = if let Some(player) = scene.get_objects().first() {
        player.get_position()
    } else {
        return;
    };
    
    let mut events_to_handle = Vec::new();
    
    // Check collisions with collectibles
    for (i, object) in scene.get_objects().iter().enumerate().skip(1) {
        let distance = (object.get_position() - player_pos).magnitude();
        
        if distance < 2.0 { // Collision threshold
            match object.get_name() {
                "Collectible" => {
                    events_to_handle.push(GameEvent::ItemCollected {
                        item_id: "coin".to_string(),
                        quantity: 1,
                    });
                    
                    // Update quest progress
                    framework.quest_system.update_objective("tutorial_complete", "collect_coins", 1);
                }
                "Enemy" => {
                    events_to_handle.push(GameEvent::EnemyDefeated { enemy_id: i });
                }
                _ => {}
            }
        }
    }
    
    // Handle events
    for event in events_to_handle {
        let _ = game_instance.handle_event(event);
    }
    
    // Remove collected items (in a real game, you'd mark them for removal)
    // This is simplified for the demo
}

fn update_ui_elements(game_instance: &Box<dyn GameInstance>, framework: &GameFramework) {
    // Update UI elements based on game state
    let ui_elements = game_instance.get_ui_elements();
    
    // In a real implementation, this would update the actual UI rendering
    // For now, we'll just track the state
    for element in &ui_elements {
        match element.element_type {
            UIElementType::HealthBar => {
                // Update health bar display
            }
            UIElementType::ProgressBar => {
                // Update progress bar (experience, etc.)
            }
            _ => {}
        }
    }
}

fn show_game_stats(framework: &GameFramework, frames_rendered: u32) {
    println!("\n📊 Game Statistics:");
    println!("  Frames rendered: {}", frames_rendered);
    println!("  Active quests: {}", framework.quest_system.get_active_quests().len());
    println!("  Inventory items: {}", framework.inventory.get_total_items());
    println!("  Gold: {}", framework.inventory.gold);
    
    // Show quest progress
    for quest in framework.quest_system.get_active_quests() {
        println!("  📜 {}: {}/{} objectives complete", 
            quest.title,
            quest.objectives.iter().filter(|o| o.completed).count(),
            quest.objectives.len()
        );
    }
    
    println!();
}