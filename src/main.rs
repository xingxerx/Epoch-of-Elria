use epoch_of_elria::{GameEngine, EngineConfig};
use epoch_of_elria::math::Vector3D;
use std::time::Instant;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Initialize logging
    env_logger::init();
    
    // Create engine configuration
    let config = EngineConfig {
        window_title: "Epoch of Elria - Rust Game Engine".to_string(),
        window_width: 1024,
        window_height: 768,
        enable_physics: true,
        enable_audio: false, // Disable audio for now since it's not fully implemented
        target_fps: 60.0,
        vsync: true,
    };
    
    // Create and initialize the game engine
    let mut engine = GameEngine::new(config)?;
    
    // Add some test objects to the scene
    let player_id = engine.scene.add_player(Vector3D::new(0.0, 1.0, 0.0));
    let _collectible_id = engine.scene.add_collectible(Vector3D::new(2.0, 1.0, 0.0), 10);
    let _platform_id = engine.scene.add_platform(
        Vector3D::new(4.0, 0.5, 4.0), 
        Vector3D::new(0.0, 0.0, 0.0)
    );
    
    println!("Epoch of Elria Game Engine initialized successfully!");
    println!("Player ID: {}", player_id);
    println!("Press ESC to exit");
    
    // Main game loop
    let mut last_time = Instant::now();
    let target_frame_time = 1.0 / config.target_fps;
    
    loop {
        let current_time = Instant::now();
        let delta_time = current_time.duration_since(last_time).as_secs_f32();
        last_time = current_time;
        
        // Process input
        engine.input_manager.update();
        
        // Check for exit condition
        if engine.input_manager.is_key_pressed(epoch_of_elria::input::Key::Escape) {
            break;
        }
        
        // Update game logic
        engine.scene.update(delta_time);
        
        // Update physics if enabled
        if let Some(ref mut physics) = engine.physics_world {
            physics.step(delta_time);
        }
        
        // Render the scene
        if !engine.rendering_system.render(&engine.scene) {
            break; // Window was closed
        }
        
        // Frame rate limiting
        let frame_time = current_time.elapsed().as_secs_f32();
        if frame_time < target_frame_time {
            let sleep_time = target_frame_time - frame_time;
            std::thread::sleep(std::time::Duration::from_secs_f32(sleep_time));
        }
    }
    
    println!("Game engine shutting down...");
    Ok(())
}