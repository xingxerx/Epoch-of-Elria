// lib.rs - Epoch of Elria Game Engine Library

pub mod math;
pub mod game_objects;
pub mod rendering;
pub mod input;
pub mod physics;
pub mod audio;
pub mod scene;
pub mod ui;
pub mod game_framework;
pub mod advanced_rendering;
pub mod game_templates;
pub mod idle_systems;
pub mod game_state;

// Re-export commonly used types
pub use math::{Vector2D, Vector3D};
pub use game_objects::{GameObject, Player, Collectible};
pub use rendering::RenderingSystem;
pub use input::InputManager;
pub use physics::PhysicsWorld;
pub use scene::Scene;
pub use ui::UI;
use crate::idle_systems::IdleManager;
use crate::audio::AudioSystem;

// Engine configuration
#[derive(Debug, Clone)]
pub struct EngineConfig {
    pub window_width: u32,
    pub window_height: u32,
    pub window_title: String,
    pub vsync: bool,
    pub fullscreen: bool,
    pub max_fps: Option<u32>,
    pub enable_physics: bool,
    pub enable_audio: bool,
    pub debug_mode: bool,
}

impl Default for EngineConfig {
    fn default() -> Self {
        Self {
            window_width: 1024,
            window_height: 768,
            window_title: "Epoch of Elria Game Engine".to_string(),
            vsync: true,
            fullscreen: false,
            max_fps: None,
            enable_physics: true,
            enable_audio: true,
            debug_mode: false,
        }
    }
}

// Main game engine struct
pub struct GameEngine {
    config: EngineConfig,
    rendering_system: RenderingSystem,
    input_manager: InputManager,
    physics_world: Option<PhysicsWorld>,
    audio_system: Option<AudioSystem>,
    scene: Scene,
    idle_manager: IdleManager, // Added IdleManager
    running: bool,
    delta_time: f32,
    total_time: f32,
}

impl GameEngine {
    pub fn new(config: EngineConfig) -> Result<Self, Box<dyn std::error::Error>> {
        let rendering_system = RenderingSystem::new(&config)?;
        let input_manager = InputManager::new();
        let physics_world = if config.enable_physics {
            Some(PhysicsWorld::new())
        } else {
            None
        };
        let audio_system = if config.enable_audio {
            Some(AudioSystem::new()?)
        } else {
            None
        };
        let scene = Scene::new("Main Scene");
        let idle_manager = IdleManager::new(); // Initialize IdleManager

        Ok(Self {
            config,
            rendering_system,
            input_manager,
            physics_world,
            audio_system,
            scene,
            idle_manager, // Store IdleManager
            running: false,
            delta_time: 0.0,
            total_time: 0.0,
        })
    }

        pub fn update<F>(&mut self, mut update_fn: F, ui: &mut UI) -> Result<(), Box<dyn std::error::Error>>
    where
        F: FnMut(&mut Scene, &mut IdleManager, &InputManager, f32, &mut UI),
    {
        let mut last_time = std::time::Instant::now();

        // Initial update to process offline time before first frame
        self.idle_manager.update(0.0); // Processes offline time based on saved timestamp

        let current_time = std::time::Instant::now();
        self.delta_time = current_time.duration_since(last_time).as_secs_f32();
        self.total_time += self.delta_time;
        last_time = current_time;

        // Handle input
        self.input_manager.update(&mut self.rendering_system);

        // Update idle systems
        self.idle_manager.update(self.delta_time as f64);

        // Update game logic (now receives IdleManager)
        update_fn(&mut self.scene, &mut self.idle_manager, &self.input_manager, self.delta_time, ui);

        // Update physics
        if let Some(ref mut physics) = self.physics_world {
            physics.step(self.delta_time);
        }

        // Update audio
        if let Some(ref mut audio) = self.audio_system {
            audio.update(self.delta_time);
        }

        // Render
        self.rendering_system.render(&self.scene, ui)?;

        // Check for exit conditions
        if self.input_manager.is_key_pressed(input::Key::Escape) {
            self.running = false;
        }

        // Save game data on graceful exit
        log::info!("Game loop ended. Saving game data...");
        self.idle_manager.player_data.record_update_time(); // Ensure timestamp is current before save
        self.idle_manager.save_game_data();

        Ok(())
    }


    pub fn stop(&mut self) {
        // This method might also be a good place to trigger a save if called.
        if self.running { // Only save if it was running, to prevent saving on initial error perhaps
            log::info!("GameEngine stop called. Saving game data...");
            self.idle_manager.player_data.record_update_time();
            self.idle_manager.save_game_data();
        }
        self.running = false;
    }

    pub fn get_delta_time(&self) -> f32 {
        self.delta_time
    }

    pub fn get_total_time(&self) -> f32 {
        self.total_time
    }

    pub fn get_scene(&mut self) -> &mut Scene {
        &mut self.scene
    }

    pub fn get_input(&self) -> &InputManager {
        &self.input_manager
    }

    pub fn get_physics(&mut self) -> Option<&mut PhysicsWorld> {
        self.physics_world.as_mut()
    }

    pub fn get_audio(&mut self) -> Option<&mut AudioSystem> {
        self.audio_system.as_mut()
    }

    // Getter for IdleManager if direct access is needed outside update_fn
    pub fn get_idle_manager(&mut self) -> &mut IdleManager {
        &mut self.idle_manager
    }
}

// Utility functions
pub fn initialize_logging() {
    env_logger::init();
}

pub fn get_version() -> &'static str {
    env!("CARGO_PKG_VERSION")
}

// Error types
#[derive(Debug)]
pub enum EngineError {
    InitializationError(String),
    RenderingError(String),
    PhysicsError(String),
    AudioError(String),
    IoError(std::io::Error),
}

impl std::fmt::Display for EngineError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            EngineError::InitializationError(msg) => write!(f, "Initialization error: {}", msg),
            EngineError::RenderingError(msg) => write!(f, "Rendering error: {}", msg),
            EngineError::PhysicsError(msg) => write!(f, "Physics error: {}", msg),
            EngineError::AudioError(msg) => write!(f, "Audio error: {}", msg),
            EngineError::IoError(err) => write!(f, "IO error: {}", err),
        }
    }
}

impl std::error::Error for EngineError {}

impl From<std::io::Error> for EngineError {
    fn from(err: std::io::Error) -> Self {
        EngineError::IoError(err)
    }
}