// It appears the first main function was a test suite.
// Commenting it out to focus on the winit-based entry point.
/*
use epoch_of_elria::{GameEngine, EngineConfig, initialize_logging};
use epoch_of_elria::math::Vector3D;
use epoch_of_elria::input::Key;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Initialize logging
    initialize_logging();
    
    println!("=== Epoch of Elria Game Engine Test ===");
    
    // Test basic math functionality
    println!("Testing Vector3D math...");
    let v1 = Vector3D::new(1.0, 2.0, 3.0);
    let v2 = Vector3D::new(4.0, 5.0, 6.0);
    let v3 = v1 + v2;
    println!("Vector addition: {:?} + {:?} = {:?}", v1, v2, v3);
    println!("Vector magnitude: |{:?}| = {:.2}", v1, v1.magnitude());
    
    // Test engine configuration
    println!("Testing engine configuration...");
    let config = EngineConfig {
        window_title: "Epoch of Elria - Rust Game Engine Test".to_string(),
        window_width: 800,
        window_height: 600,
        enable_physics: true,
        enable_audio: false,
        vsync: true,
        fullscreen: true,
        max_fps: Some(120),
        debug_mode: true,
    };
    println!("Engine config created: {}x{}", config.window_width, config.window_height);
    
    // Test engine creation (but don't run the graphics loop)
    println!("Testing engine creation...");
    match GameEngine::new(config) {
        Ok(mut engine) => {
            println!("✓ Game engine created successfully!");
            
            // Test scene manipulation
            println!("Testing scene objects...");
            {
                let scene = engine.get_scene();
                let player_id = scene.add_player(Vector3D::new(0.0, 1.0, 0.0));
                let collectible_id = scene.add_collectible(Vector3D::new(2.0, 1.0, 0.0), 10);
                let platform_id = scene.add_platform(
                    Vector3D::new(4.0, 0.5, 4.0), 
                    Vector3D::new(0.0, 0.0, 0.0)
                );
                
                println!("✓ Created player (ID: {})", player_id);
                println!("✓ Created collectible (ID: {})", collectible_id);
                println!("✓ Created platform (ID: {})", platform_id);
                println!("✓ Total objects in scene: {}", scene.get_object_count());
                
                // Test object access
                if let Some(player) = scene.get_object(player_id) {
                    println!("✓ Player position: {:?}", player.get_position());
                    println!("✓ Player name: {}", player.get_name());
                }
            }
            
            // Test input manager
            println!("Testing input manager...");
            let input = engine.get_input();
            println!("✓ Input manager accessible");
            
            // Test physics (if enabled)
            if let Some(_physics) = engine.get_physics() {
                println!("✓ Physics world initialized");
            }
            
            println!("✓ All engine components initialized successfully!");
            
            // Note: We're not running the actual game loop here to avoid graphics issues in WSL
            println!("Note: Skipping graphics loop test (may not work in WSL environment)");
            
        },
        Err(e) => {
            println!("✗ Failed to create game engine: {}", e);
            return Err(e);
        }
    }
    
    println!("=== All tests completed successfully! ===");
    println!("The Epoch of Elria Rust game engine is working correctly.");
    println!("To run the full graphics demo, use: cargo run --release");
    
    Ok(())
}
*/

'''
use epoch_of_elria::{GameEngine, EngineConfig, initialize_logging, Scene, InputManager};
use epoch_of_elria::game_state::GameStateContext;
use epoch_of_elria::ui::UI;
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    initialize_logging();
    log::info!("Starting Epoch of Elria Engine...");

    let config = EngineConfig {
        window_title: "Epoch of Elria".to_string(),
        window_width: 1024,
        window_height: 768,
        ..Default::default() // Use default for other settings
    };

    let event_loop = EventLoop::new();
    let window = WindowBuilder::new().build(&event_loop)?;

    let mut engine = GameEngine::new(config)?;
    log::info!("GameEngine initialized.");

    let mut ui = UI::new();
    let mut game_state_context = GameStateContext::new();

    // Placeholder update function for the idle game
    let mut update_logic = move |scene: &mut Scene, idle_manager: &mut epoch_of_elria::idle_systems::IdleManager, input: &InputManager, delta_time: f32, ui: &mut UI| {
        game_state_context.update(ui, input);
    };

    log::info!("Starting engine run loop...");

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Poll;

        match event {
            Event::WindowEvent { event, .. } => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                _ => (),
            },
            Event::MainEventsCleared => {
                engine.update(&mut update_logic, &mut ui).unwrap();
            }
            _ => (),
        }
    });

    log::info!("Engine run loop finished.");
    log::info!("Application closed.");
    Ok(())
}
'''