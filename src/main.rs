use epoch_of_elria::{GameEngine, EngineConfig, initialize_logging};
use epoch_of_elria::math::Vector3D;
use epoch_of_elria::input::Key;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Initialize logging
    initialize_logging();
    
    // Create engine configuration
    let config = EngineConfig {
        window_title: "Epoch of Elria - Rust Game Engine".to_string(),
        window_width: 1024,
        window_height: 768,
        enable_physics: true,
        enable_audio: false, // Disable audio for now since it's not fully implemented
        vsync: true,
        fullscreen: false,
        max_fps: Some(60),
        debug_mode: true,
    };
    
    // Create and initialize the game engine
    let mut engine = GameEngine::new(config)?;
    
    // Add some test objects to the scene
    {
        let scene = engine.get_scene();
        let player_id = scene.add_player(Vector3D::new(0.0, 1.0, 0.0));
        let _collectible_id = scene.add_collectible(Vector3D::new(2.0, 1.0, 0.0), 10);
        let _platform_id = scene.add_platform(
            Vector3D::new(4.0, 0.5, 4.0), 
            Vector3D::new(0.0, 0.0, 0.0)
        );
        
        println!("Epoch of Elria Game Engine initialized successfully!");
        println!("Player ID: {}", player_id);
        println!("Press ESC to exit");
    }
    
    // Run the game engine with our update function
    engine.run(|scene, input, delta_time| {
        // Update all game objects in the scene
        scene.update(delta_time);
        
        // Handle player input
        if let Some(player) = scene.get_object_mut(0) {
            // Simple movement logic - this would be more sophisticated in a real game
            let mut movement = Vector3D::new(0.0, 0.0, 0.0);
            
            if input.is_key_pressed(Key::W) {
                movement.z -= 1.0;
            }
            if input.is_key_pressed(Key::S) {
                movement.z += 1.0;
            }
            if input.is_key_pressed(Key::A) {
                movement.x -= 1.0;
            }
            if input.is_key_pressed(Key::D) {
                movement.x += 1.0;
            }
            if input.is_key_pressed(Key::Space) {
                movement.y += 1.0;
            }
            if input.is_key_pressed(Key::LeftShift) {
                movement.y -= 1.0;
            }
            
            if movement.magnitude() > 0.0 {
                movement = movement.normalize();
                let current_pos = player.get_position();
                let speed = 5.0;
                let new_pos = current_pos + movement * speed * delta_time;
                player.set_position(new_pos);
            }
        }
        
        // Simple collision detection between player and collectibles
        if let Some(player) = scene.get_object(0) {
            let player_pos = player.get_position();
            let collision_distance = 1.5;
            
            // Check collisions with collectibles (assuming they start from ID 1)
            for i in 1..scene.get_object_count() {
                if let Some(obj) = scene.get_object(i) {
                    if obj.get_name().starts_with("Collectible") && obj.is_active() {
                        let distance = player_pos.distance(&obj.get_position());
                        if distance < collision_distance {
                            // Deactivate the collectible (simulate collection)
                            if let Some(collectible) = scene.get_object_mut(i) {
                                collectible.set_active(false);
                                println!("Collected {}!", collectible.get_name());
                            }
                        }
                    }
                }
            }
        }
    })?;
    
    println!("Game engine shutting down...");
    Ok(())
}