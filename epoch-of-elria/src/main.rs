use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::nalgebra::{Vector3, Translation3};
use kiss3d::scene::SceneNode;
use std::collections::HashSet;

struct Player {
    node: SceneNode,
    position: Vector3<f32>,
    velocity: Vector3<f32>,
    on_ground: bool,
}

struct Platform {
    node: SceneNode,
    position: Vector3<f32>,
    size: Vector3<f32>,
}

impl Player {
    fn new(mut node: SceneNode, start_pos: Vector3<f32>) -> Self {
        node.set_local_translation(Translation3::new(start_pos.x, start_pos.y, start_pos.z));
        Self {
            node,
            position: start_pos,
            velocity: Vector3::new(0.0, 0.0, 0.0),
            on_ground: false,
        }
    }

    fn update(&mut self, delta_time: f32) {
        // Apply gravity
        if !self.on_ground {
            self.velocity.y -= 15.0 * delta_time; // Gravity strength
        }

        // Update position based on velocity
        self.position += self.velocity * delta_time;

        // Update visual position
        self.node.set_local_translation(Translation3::new(
            self.position.x, 
            self.position.y, 
            self.position.z
        ));
    }

    fn jump(&mut self) {
        if self.on_ground {
            self.velocity.y = 8.0; // Jump strength
            self.on_ground = false;
            println!("🦘 Jump!");
        }
    }

    fn move_horizontal(&mut self, direction: Vector3<f32>, speed: f32) {
        self.velocity.x = direction.x * speed;
        self.velocity.z = direction.z * speed;
    }

    fn check_platform_collision(&mut self, platforms: &[Platform]) {
        self.on_ground = false;
        
        for platform in platforms {
            // Simple AABB collision detection
            let player_half_size = 0.25; // Half the player cube size
            
            // Check if player is above platform and falling
            if self.velocity.y <= 0.0 &&
               self.position.x + player_half_size > platform.position.x - platform.size.x/2.0 &&
               self.position.x - player_half_size < platform.position.x + platform.size.x/2.0 &&
               self.position.z + player_half_size > platform.position.z - platform.size.z/2.0 &&
               self.position.z - player_half_size < platform.position.z + platform.size.z/2.0 &&
               self.position.y > platform.position.y + platform.size.y/2.0 &&
               self.position.y - player_half_size <= platform.position.y + platform.size.y/2.0 + 0.1 {
                
                // Land on platform
                self.position.y = platform.position.y + platform.size.y/2.0 + player_half_size;
                self.velocity.y = 0.0;
                self.on_ground = true;
                break;
            }
        }
    }
}

impl Platform {
    fn new(mut node: SceneNode, position: Vector3<f32>, size: Vector3<f32>) -> Self {
        node.set_local_translation(Translation3::new(position.x, position.y, position.z));
        Self { node, position, size }
    }
}

fn main() {
    println!("🎮 Starting Epoch of Elria - Platformer Edition!");
    
    let mut window = Window::new("Epoch of Elria - Platformer Game Engine");
    window.set_light(Light::StickToCamera);
    
    // Create platforms
    let mut platforms = Vec::new();
    
    // Ground platform (large)
    let ground_node = window.add_cube(20.0, 0.5, 20.0);
    ground_node.set_color(0.3, 0.8, 0.3); // Green ground
    platforms.push(Platform::new(
        ground_node,
        Vector3::new(0.0, -2.0, 0.0),
        Vector3::new(20.0, 0.5, 20.0)
    ));
    
    // Floating platforms
    let platform1_node = window.add_cube(4.0, 0.5, 4.0);
    platform1_node.set_color(0.8, 0.6, 0.2); // Orange platform
    platforms.push(Platform::new(
        platform1_node,
        Vector3::new(5.0, 0.0, 0.0),
        Vector3::new(4.0, 0.5, 4.0)
    ));
    
    let platform2_node = window.add_cube(3.0, 0.5, 3.0);
    platform2_node.set_color(0.6, 0.2, 0.8); // Purple platform
    platforms.push(Platform::new(
        platform2_node,
        Vector3::new(-4.0, 2.0, 3.0),
        Vector3::new(3.0, 0.5, 3.0)
    ));
    
    let platform3_node = window.add_cube(2.5, 0.5, 2.5);
    platform3_node.set_color(0.2, 0.6, 0.8); // Blue platform
    platforms.push(Platform::new(
        platform3_node,
        Vector3::new(2.0, 4.0, -5.0),
        Vector3::new(2.5, 0.5, 2.5)
    ));
    
    // Create player
    let player_node = window.add_cube(0.5, 0.5, 0.5);
    player_node.set_color(0.8, 0.2, 0.2); // Red player
    let mut player = Player::new(player_node, Vector3::new(0.0, 5.0, 0.0));
    
    let move_speed = 5.0;
    let mut last_time = std::time::Instant::now();
    let mut pressed_keys = HashSet::new();
    
    println!("🎮 Platformer controls:");
    println!("  WASD: Move horizontally");
    println!("  SPACE: Jump");
    println!("  ESC: Exit");
    println!("🏃 Try jumping between the colored platforms!");
    
    while window.render() {
        // Calculate delta time
        let current_time = std::time::Instant::now();
        let delta_time = current_time.duration_since(last_time).as_secs_f32();
        last_time = current_time;
        
        // Handle input events
        for event in window.events().iter() {
            match event.value {
                kiss3d::event::WindowEvent::Key(key, action, _) => {
                    match action {
                        kiss3d::event::Action::Press => {
                            pressed_keys.insert(key);
                            if key == kiss3d::event::Key::Space {
                                player.jump();
                            }
                            if key == kiss3d::event::Key::Escape {
                                return;
                            }
                        }
                        kiss3d::event::Action::Release => {
                            pressed_keys.remove(&key);
                        }
                        _ => {}
                    }
                }
                _ => {}
            }
        }
        
        // Handle continuous movement
        let mut movement = Vector3::new(0.0, 0.0, 0.0);
        
        if pressed_keys.contains(&kiss3d::event::Key::W) {
            movement.z -= 1.0;
        }
        if pressed_keys.contains(&kiss3d::event::Key::S) {
            movement.z += 1.0;
        }
        if pressed_keys.contains(&kiss3d::event::Key::A) {
            movement.x -= 1.0;
        }
        if pressed_keys.contains(&kiss3d::event::Key::D) {
            movement.x += 1.0;
        }
        
        // Apply movement
        if movement.magnitude() > 0.0 {
            movement = movement.normalize();
            player.move_horizontal(movement, move_speed);
        } else {
            // Apply friction when not moving
            player.velocity.x *= 0.8;
            player.velocity.z *= 0.8;
        }
        
        // Update physics
        player.check_platform_collision(&platforms);
        player.update(delta_time);
        
        // Reset if player falls too far
        if player.position.y < -10.0 {
            player.position = Vector3::new(0.0, 5.0, 0.0);
            player.velocity = Vector3::new(0.0, 0.0, 0.0);
            println!("💀 Respawned! Try again!");
        }
    }
    
    println!("🎮 Thanks for playing Epoch of Elria Platformer!");
}
