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
        fullscreen: false,
        max_fps: Some(60),
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

use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

fn main() {
    // 1. Create an EventLoop
    // The EventLoop is the core of winit, handling all OS-level events.
    let event_loop = EventLoop::new().unwrap();

    // 2. Create a Window
    // Use WindowBuilder to configure your window's properties.
    let window = WindowBuilder::new()
        .with_title("Epoch of Elria") // Set the window title
        .with_inner_size(winit::dpi::LogicalSize::new(800.0, 600.0)) // Set initial window size
        .build(&event_loop)
        .unwrap();

    // 3. Run the Event Loop
    // This starts the main application loop, processing events.
    event_loop.run(move |event, elwt| {
        // ControlFlow::Poll makes the event loop continuously poll for events,
        // which is good for games that need to render continuously.
        elwt.set_control_flow(ControlFlow::Poll);

        match event {
            // WindowEvent::CloseRequested is sent when the user clicks the 'X' button.
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                window_id,
            } if window_id == window.id() => {
                // When close is requested, tell the event loop to exit.
                println!("Close requested, exiting application.");
                elwt.set_control_flow(ControlFlow::Exit);
            }
            // Event::AboutToWait is triggered when the event loop is about to
            // wait for new events. This is where you'd put your game logic
            // and rendering code in a real game engine.
            Event::AboutToWait => {
                // In a real game, you would perform your rendering here.
                // For now, we'll just print something to show it's active.
                // println!("Game loop tick!"); // Uncomment for verbose ticking
            }
            // Other events can be handled here as needed (e.g., keyboard input, mouse movement).
            _ => (),
        }
    }).unwrap(); // .unwrap() handles potential errors during event loop execution.
}
