use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::nalgebra::{Vector3, Translation3};

fn main() {
    println!("🎮 Starting Epoch of Elria Game Engine...");
    
    let mut window = Window::new("Epoch of Elria - Game Engine");
    window.set_light(Light::StickToCamera);
    
    // Create a baseplate (ground)
    let mut baseplate = window.add_cube(10.0, 0.1, 10.0);
    baseplate.set_color(0.3, 0.8, 0.3); // Green ground
    baseplate.set_local_translation(Translation3::new(0.0, -1.0, 0.0));
    
    // Create a controllable character (cube for now)
    let mut player = window.add_cube(0.5, 1.0, 0.5);
    player.set_color(0.8, 0.2, 0.2); // Red player
    
    let mut player_pos = Vector3::new(0.0, 0.0, 0.0);
    let move_speed = 0.1;
    
    println!("🎮 Game window opened!");
    println!("🎮 Controls: WASD to move, ESC to exit");
    println!("�� Main Menu: Play, Customize, Settings, Exit");
    
    while window.render() {
        // Handle input for character movement
        for event in window.events().iter() {
            match event.value {
                kiss3d::event::WindowEvent::Key(key, kiss3d::event::Action::Press, _) => {
                    match key {
                        kiss3d::event::Key::W => player_pos.z -= move_speed,
                        kiss3d::event::Key::S => player_pos.z += move_speed,
                        kiss3d::event::Key::A => player_pos.x -= move_speed,
                        kiss3d::event::Key::D => player_pos.x += move_speed,
                        kiss3d::event::Key::Escape => return,
                        _ => {}
                    }
                }
                _ => {}
            }
        }
        
        // Update player position
        player.set_local_translation(Translation3::new(player_pos.x, player_pos.y, player_pos.z));
    }
    
    println!("🎮 Game engine closed. Thanks for playing!");
}
