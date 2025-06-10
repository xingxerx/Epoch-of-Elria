// main.rs - Epoch of Elria Rust Game Engine
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
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Vector2D {
    pub x: f32,
    pub y: f32,
}

impl Vector2D {
    pub fn new(x: f32, y: f32) -> Self {
        Self { x, y }
    }

    pub fn zero() -> Self {
        Self::new(0.0, 0.0)
    }

    pub fn magnitude(&self) -> f32 {
        (self.x * self.x + self.y * self.y).sqrt()
    }

    pub fn normalize(&self) -> Self {
        let mag = self.magnitude();
        if mag > 0.0 {
            Self::new(self.x / mag, self.y / mag)
        } else {
            Self::zero()
        }
    }

    pub fn distance(&self, other: &Vector2D) -> f32 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        (dx * dx + dy * dy).sqrt()
    }
}

impl std::ops::Add for Vector2D {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self::new(self.x + other.x, self.y + other.y)
    }
}

impl std::ops::Sub for Vector2D {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self::new(self.x - other.x, self.y - other.y)
    }
}

impl std::ops::Mul<f32> for Vector2D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Self::new(self.x * scalar, self.y * scalar)
    }
}

// 3D Vector utilities
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Vector3D {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

impl Vector3D {
    pub fn new(x: f32, y: f32, z: f32) -> Self {
        Self { x, y, z }
    }

    pub fn zero() -> Self {
        Self::new(0.0, 0.0, 0.0)
    }

    pub fn magnitude(&self) -> f32 {
        (self.x * self.x + self.y * self.y + self.z * self.z).sqrt()
    }

    pub fn normalize(&self) -> Self {
        let mag = self.magnitude();
        if mag > 0.0 {
            Self::new(self.x / mag, self.y / mag, self.z / mag)
        } else {
            Self::zero()
        }
    }

    pub fn distance(&self, other: &Vector3D) -> f32 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        let dz = self.z - other.z;
        (dx * dx + dy * dy + dz * dz).sqrt()
    }

    pub fn to_point3(&self) -> Point3<f32> {
        Point3::new(self.x, self.y, self.z)
    }

    pub fn to_vector3(&self) -> Vector3<f32> {
        Vector3::new(self.x, self.y, self.z)
    }
}

impl std::ops::Add for Vector3D {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self::new(self.x + other.x, self.y + other.y, self.z + other.z)
    }
}

impl std::ops::Sub for Vector3D {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self::new(self.x - other.x, self.y - other.y, self.z - other.z)
    }
}

impl std::ops::Mul<f32> for Vector3D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Self::new(self.x * scalar, self.y * scalar, self.z * scalar)
    }
}

// Game object traits
pub trait GameObject {
    fn update(&mut self, delta_time: f32);
    fn get_position(&self) -> Vector3D;
    fn set_position(&mut self, position: Vector3D);
    fn get_velocity(&self) -> Vector3D;
    fn set_velocity(&mut self, velocity: Vector3D);
    fn is_active(&self) -> bool;
    fn set_active(&mut self, active: bool);
    fn get_name(&self) -> &str;
}

// Player implementation
pub struct Player {
    name: String,
    position: Vector3D,
    velocity: Vector3D,
    speed: f32,
    active: bool,
    scene_node: Option<SceneNode>,
}

impl Player {
    pub fn new(name: String, position: Vector3D) -> Self {
        Self {
            name,
            position,
            velocity: Vector3D::zero(),
            speed: 5.0,
            active: true,
            scene_node: None,
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
    }

    pub fn handle_input(&mut self, input: &InputState) {
        let mut movement = Vector3D::zero();

        if input.w { movement.z -= 1.0; }
        if input.s { movement.z += 1.0; }
        if input.a { movement.x -= 1.0; }
        if input.d { movement.x += 1.0; }
        if input.space { movement.y += 1.0; }
        if input.shift { movement.y -= 1.0; }

        if movement.magnitude() > 0.0 {
            movement = movement.normalize();
            self.velocity = movement * self.speed;
        } else {
            self.velocity = Vector3D::zero();
        }
    }

    pub fn update_scene_node(&mut self) {
        if let Some(ref mut node) = self.scene_node {
            node.set_local_translation(Translation3::new(
                self.position.x,
                self.position.y,
                self.position.z,
            ));
        }
    }
}

impl GameObject for Player {
    fn update(&mut self, delta_time: f32) {
        if self.active {
            self.position = self.position + self.velocity * delta_time;
            self.update_scene_node();
        }
    }

    fn get_position(&self) -> Vector3D { self.position }
    fn set_position(&mut self, position: Vector3D) { 
        self.position = position;
        self.update_scene_node();
    }
    fn get_velocity(&self) -> Vector3D { self.velocity }
    fn set_velocity(&mut self, velocity: Vector3D) { self.velocity = velocity; }
    fn is_active(&self) -> bool { self.active }
    fn set_active(&mut self, active: bool) { self.active = active; }
    fn get_name(&self) -> &str { &self.name }
}

// Collectible implementation
pub struct Collectible {
    name: String,
    position: Vector3D,
    velocity: Vector3D,
    value: i32,
    collected: bool,
    active: bool,
    rotation: f32,
    scene_node: Option<SceneNode>,
}

impl Collectible {
    pub fn new(name: String, position: Vector3D, value: i32) -> Self {
        Self {
            name,
            position,
            velocity: Vector3D::zero(),
            value,
            collected: false,
            active: true,
            rotation: 0.0,
            scene_node: None,
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
    }

    pub fn is_collected(&self) -> bool {
        self.collected
    }

    pub fn collect(&mut self) -> i32 {
        if !self.collected {
            self.collected = true;
            self.active = false;
            if let Some(ref mut node) = self.scene_node {
                node.set_visible(false);
            }
            self.value
        } else {
            0
        }
    }

    pub fn get_value(&self) -> i32 {
        self.value
    }

    fn update_scene_node(&mut self) {
        if let Some(ref mut node) = self.scene_node {
            node.set_local_translation(Translation3::new(
                self.position.x,
                self.position.y,
                self.position.z,
            ));
            
            // Add rotation animation
            let rotation = UnitQuaternion::from_axis_angle(
                &Vector3::y_axis(),
                self.rotation
            );
            node.set_local_rotation(rotation);
        }
    }
}

impl GameObject for Collectible {
    fn update(&mut self, delta_time: f32) {
        if self.active && !self.collected {
            self.position = self.position + self.velocity * delta_time;
            self.rotation += 2.0 * delta_time; // Rotate for visual effect
            if self.rotation > 2.0 * std::f32::consts::PI {
                self.rotation -= 2.0 * std::f32::consts::PI;
            }
            self.update_scene_node();
        }
    }

    fn get_position(&self) -> Vector3D { self.position }
    fn set_position(&mut self, position: Vector3D) { 
        self.position = position;
        self.update_scene_node();
    }
    fn get_velocity(&self) -> Vector3D { self.velocity }
    fn set_velocity(&mut self, velocity: Vector3D) { self.velocity = velocity; }
    fn is_active(&self) -> bool { self.active && !self.collected }
    fn set_active(&mut self, active: bool) { self.active = active; }
    fn get_name(&self) -> &str { &self.name }
}

// Input state
#[derive(Default)]
pub struct InputState {
    pub w: bool,
    pub a: bool,
    pub s: bool,
    pub d: bool,
    pub space: bool,
    pub shift: bool,
    pub escape: bool,
}

// Game state
pub struct GameState {
    player: Player,
    collectibles: Vec<Collectible>,
    score: i32,
    active_collectibles: usize,
    input: InputState,
}

impl GameState {
    pub fn new() -> Self {
        let mut player = Player::new("Player".to_string(), Vector3D::new(0.0, 0.0, 0.0));
        
        // Create collectibles
        let mut collectibles = Vec::new();
        for i in 0..50 {
            let x = (i as f32 % 10.0 - 5.0) * 3.0;
            let z = (i as f32 / 10.0 - 2.5) * 3.0;
            let y = (i as f32 * 0.1).sin() * 2.0;
            
            let collectible = Collectible::new(
                format!("Collectible_{}", i),
                Vector3D::new(x, y, z),
                10
            );
            collectibles.push(collectible);
        }

        let active_collectibles = collectibles.len();

        Self {
            player,
            collectibles,
            score: 0,
            active_collectibles,
            input: InputState::default(),
        }
    }

    pub fn update(&mut self, delta_time: f32) {
        // Handle input
        self.player.handle_input(&self.input);

        // Update player
        self.player.update(delta_time);

        // Update collectibles in parallel
        self.update_collectibles_parallel(delta_time);

        // Check collisions
        self.check_collisions();

        // Update active collectible count
        self.active_collectibles = self.collectibles.iter()
            .filter(|c| c.is_active())
            .count();
    }

    fn update_collectibles_parallel(&mut self, delta_time: f32) {
        // For now, update sequentially. In a more complex implementation,
        // we could use rayon for parallel processing
        for collectible in &mut self.collectibles {
            collectible.update(delta_time);
        }
    }

    fn check_collisions(&mut self) {
        let player_pos = self.player.get_position();
        let collision_distance = 1.5;

        for collectible in &mut self.collectibles {
            if collectible.is_active() && !collectible.is_collected() {
                let distance = player_pos.distance(&collectible.get_position());
                if distance < collision_distance {
                    let points = collectible.collect();
                    self.score += points;
                    println!("Collected {}! Score: {}", collectible.get_name(), self.score);
                }
            }
        }
    }

    pub fn get_score(&self) -> i32 {
        self.score
    }

    pub fn get_active_collectibles(&self) -> usize {
        self.active_collectibles
    }

    pub fn is_game_complete(&self) -> bool {
        self.active_collectibles == 0
    }
}

// Game engine
pub struct GameEngine {
    window: Window,
    game_state: GameState,
    last_time: Instant,
    fps_counter: FpsCounter,
}

impl GameEngine {
    pub fn new() -> Self {
        let mut window = Window::new("Epoch of Elria - Rust 3D Game Engine");
        window.set_light(Light::StickToCamera);
        window.set_background_color(0.1, 0.1, 0.2);

        let game_state = GameState::new();
        let last_time = Instant::now();
        let fps_counter = FpsCounter::new();

        Self {
            window,
            game_state,
            last_time,
            fps_counter,
        }
    }

    pub fn initialize(&mut self) {
        // Create player visual representation
        let mut player_cube = self.window.add_cube(1.0, 1.0, 1.0);
        player_cube.set_color(0.0, 0.5, 1.0); // Blue
        self.game_state.player.set_scene_node(player_cube);

        // Create collectible visual representations
        for (i, collectible) in self.game_state.collectibles.iter_mut().enumerate() {
            let mut collectible_sphere = self.window.add_sphere(0.3);
            collectible_sphere.set_color(1.0, 0.8, 0.0); // Gold
            collectible.set_scene_node(collectible_sphere);
        }

        println!("Game initialized!");
        println!("Controls:");
        println!("  WASD - Move horizontally");
        println!("  Space - Move up");
        println!("  Shift - Move down");
        println!("  ESC - Exit");
        println!("Collect all {} items to win!", self.game_state.collectibles.len());
    }

    pub fn run(&mut self) {
        while self.window.render() {
            let current_time = Instant::now();
            let delta_time = current_time.duration_since(self.last_time).as_secs_f32();
            self.last_time = current_time;

            // Handle input
            self.handle_input();

            // Update game state
            self.game_state.update(delta_time);

            // Update FPS counter
            self.fps_counter.update(delta_time);

            // Update window title with game info
            if self.fps_counter.should_update_display() {
                let title = format!(
                    "Epoch of Elria - Score: {} | Collectibles: {} | FPS: {:.1}",
                    self.game_state.get_score(),
                    self.game_state.get_active_collectibles(),
                    self.fps_counter.get_fps()
                );
                // Note: kiss3d doesn't have a direct way to update window title
                // This would need to be implemented differently in a real application
            }

            // Check for game completion
            if self.game_state.is_game_complete() {
                println!("Congratulations! You collected all items!");
                println!("Final Score: {}", self.game_state.get_score());
                break;
            }

            // Exit on escape
            if self.game_state.input.escape {
                break;
            }
        }
    }

    fn handle_input(&mut self) {
        // Reset input state
        self.game_state.input = InputState::default();

        // Check keyboard input
        for event in self.window.events().iter() {
            match event.value {
                kiss3d::event::WindowEvent::Key(key, action, _) => {
                    let pressed = action != kiss3d::event::Action::Release;
                    
                    match key {
                        kiss3d::event::Key::W => self.game_state.input.w = pressed,
                        kiss3d::event::Key::A => self.game_state.input.a = pressed,
                        kiss3d::event::Key::S => self.game_state.input.s = pressed,
                        kiss3d::event::Key::D => self.game_state.input.d = pressed,
                        kiss3d::event::Key::Space => self.game_state.input.space = pressed,
                        kiss3d::event::Key::LShift | kiss3d::event::Key::RShift => {
                            self.game_state.input.shift = pressed;
                        },
                        kiss3d::event::Key::Escape => {
                            if pressed {
                                self.game_state.input.escape = true;
                            }
                        },
                        _ => {}
                    }
                },
                _ => {}
            }
        }
    }
}

// FPS counter utility
struct FpsCounter {
    frame_count: u32,
    time_accumulator: f32,
    fps: f32,
    last_display_update: f32,
}

impl FpsCounter {
    fn new() -> Self {
        Self {
            frame_count: 0,
            time_accumulator: 0.0,
            fps: 0.0,
            last_display_update: 0.0,
        }
    }

    fn update(&mut self, delta_time: f32) {
        self.frame_count += 1;
        self.time_accumulator += delta_time;
        self.last_display_update += delta_time;

        if self.time_accumulator >= 1.0 {
            self.fps = self.frame_count as f32 / self.time_accumulator;
            self.frame_count = 0;
            self.time_accumulator = 0.0;
        }
    }

    fn get_fps(&self) -> f32 {
        self.fps
    }

    fn should_update_display(&mut self) -> bool {
        if self.last_display_update >= 0.5 {
            self.last_display_update = 0.0;
            true
        } else {
            false
        }
    }
}

// Main function
fn main() {
    println!("=== Epoch of Elria - Rust 3D Game Engine ===");
    
    let mut game_engine = GameEngine::new();
    game_engine.initialize();
    game_engine.run();
    
    println!("Thanks for playing!");
}