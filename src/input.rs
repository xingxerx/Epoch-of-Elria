// input.rs - Input management system

use crate::rendering::RenderingSystem;
use std::collections::{HashMap, HashSet};

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Key {
    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    
    // Numbers
    Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
    
    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // Special keys
    Space, Enter, Escape, Tab, Backspace, Delete, Insert, Home, End, PageUp, PageDown,
    
    // Arrow keys
    Up, Down, Left, Right,
    
    // Modifiers
    LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,
    
    // Mouse buttons
    MouseLeft, MouseRight, MouseMiddle,
    
    // Unknown key
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum MouseButton {
    Left,
    Right,
    Middle,
    Other(u8),
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct MouseState {
    pub position: (f32, f32),
    pub delta: (f32, f32),
    pub wheel_delta: f32,
    pub left_pressed: bool,
    pub right_pressed: bool,
    pub middle_pressed: bool,
}

impl Default for MouseState {
    fn default() -> Self {
        Self {
            position: (0.0, 0.0),
            delta: (0.0, 0.0),
            wheel_delta: 0.0,
            left_pressed: false,
            right_pressed: false,
            middle_pressed: false,
        }
    }
}

pub struct InputManager {
    // Keyboard state
    keys_pressed: HashSet<Key>,
    keys_just_pressed: HashSet<Key>,
    keys_just_released: HashSet<Key>,
    
    // Mouse state
    mouse_state: MouseState,
    mouse_buttons_pressed: HashSet<MouseButton>,
    mouse_buttons_just_pressed: HashSet<MouseButton>,
    mouse_buttons_just_released: HashSet<MouseButton>,
    
    // Previous frame state for delta calculations
    previous_mouse_position: (f32, f32),
    
    // Input mapping
    action_mappings: HashMap<String, Vec<Key>>,
    axis_mappings: HashMap<String, Vec<(Key, f32)>>,
}

impl InputManager {
    pub fn new() -> Self {
        Self {
            keys_pressed: HashSet::new(),
            keys_just_pressed: HashSet::new(),
            keys_just_released: HashSet::new(),
            mouse_state: MouseState::default(),
            mouse_buttons_pressed: HashSet::new(),
            mouse_buttons_just_pressed: HashSet::new(),
            mouse_buttons_just_released: HashSet::new(),
            previous_mouse_position: (0.0, 0.0),
            action_mappings: HashMap::new(),
            axis_mappings: HashMap::new(),
        }
    }

    pub fn update(&mut self, rendering_system: &mut RenderingSystem) {
        // Clear just pressed/released states
        self.keys_just_pressed.clear();
        self.keys_just_released.clear();
        self.mouse_buttons_just_pressed.clear();
        self.mouse_buttons_just_released.clear();
        
        // Reset mouse delta and wheel
        self.mouse_state.delta = (0.0, 0.0);
        self.mouse_state.wheel_delta = 0.0;
        
        // Process events from the rendering system
        let window = rendering_system.window_mut();
        
        for event in window.events().iter() {
            match event.value {
                kiss3d::event::WindowEvent::Key(key, action, _) => {
                    let mapped_key = self.map_kiss3d_key(key);
                    
                    match action {
                        kiss3d::event::Action::Press => {
                            if !self.keys_pressed.contains(&mapped_key) {
                                self.keys_just_pressed.insert(mapped_key);
                            }
                            self.keys_pressed.insert(mapped_key);
                        },
                        kiss3d::event::Action::Release => {
                            if self.keys_pressed.contains(&mapped_key) {
                                self.keys_just_released.insert(mapped_key);
                            }
                            self.keys_pressed.remove(&mapped_key);
                        },
                        kiss3d::event::Action::Repeat => {
                            // Handle key repeat if needed
                        },
                    }
                },
                kiss3d::event::WindowEvent::MouseButton(button, action, _) => {
                    let mapped_button = self.map_kiss3d_mouse_button(button);
                    
                    match action {
                        kiss3d::event::Action::Press => {
                            if !self.mouse_buttons_pressed.contains(&mapped_button) {
                                self.mouse_buttons_just_pressed.insert(mapped_button);
                            }
                            self.mouse_buttons_pressed.insert(mapped_button);
                            
                            // Update mouse state
                            match mapped_button {
                                MouseButton::Left => self.mouse_state.left_pressed = true,
                                MouseButton::Right => self.mouse_state.right_pressed = true,
                                MouseButton::Middle => self.mouse_state.middle_pressed = true,
                                _ => {},
                            }
                        },
                        kiss3d::event::Action::Release => {
                            if self.mouse_buttons_pressed.contains(&mapped_button) {
                                self.mouse_buttons_just_released.insert(mapped_button);
                            }
                            self.mouse_buttons_pressed.remove(&mapped_button);
                            
                            // Update mouse state
                            match mapped_button {
                                MouseButton::Left => self.mouse_state.left_pressed = false,
                                MouseButton::Right => self.mouse_state.right_pressed = false,
                                MouseButton::Middle => self.mouse_state.middle_pressed = false,
                                _ => {},
                            }
                        },
                        _ => {},
                    }
                },
                kiss3d::event::WindowEvent::CursorPos(x, y) => {
                    let new_position = (x as f32, y as f32);
                    self.mouse_state.delta = (
                        new_position.0 - self.previous_mouse_position.0,
                        new_position.1 - self.previous_mouse_position.1,
                    );
                    self.mouse_state.position = new_position;
                    self.previous_mouse_position = new_position;
                },
                kiss3d::event::WindowEvent::Scroll(_, y) => {
                    self.mouse_state.wheel_delta = y as f32;
                },
                _ => {},
            }
        }
    }

    // Keyboard input methods
    pub fn is_key_pressed(&self, key: Key) -> bool {
        self.keys_pressed.contains(&key)
    }

    pub fn is_key_just_pressed(&self, key: Key) -> bool {
        self.keys_just_pressed.contains(&key)
    }

    pub fn is_key_just_released(&self, key: Key) -> bool {
        self.keys_just_released.contains(&key)
    }

    pub fn get_pressed_keys(&self) -> Vec<Key> {
        self.keys_pressed.iter().cloned().collect()
    }

    // Mouse input methods
    pub fn is_mouse_button_pressed(&self, button: MouseButton) -> bool {
        self.mouse_buttons_pressed.contains(&button)
    }

    pub fn is_mouse_button_just_pressed(&self, button: MouseButton) -> bool {
        self.mouse_buttons_just_pressed.contains(&button)
    }

    pub fn is_mouse_button_just_released(&self, button: MouseButton) -> bool {
        self.mouse_buttons_just_released.contains(&button)
    }

    pub fn get_mouse_position(&self) -> (f32, f32) {
        self.mouse_state.position
    }

    pub fn get_mouse_delta(&self) -> (f32, f32) {
        self.mouse_state.delta
    }

    pub fn get_mouse_wheel_delta(&self) -> f32 {
        self.mouse_state.wheel_delta
    }

    pub fn get_mouse_state(&self) -> &MouseState {
        &self.mouse_state
    }

    // Action mapping methods
    pub fn add_action_mapping(&mut self, action: String, keys: Vec<Key>) {
        self.action_mappings.insert(action, keys);
    }

    pub fn is_action_pressed(&self, action: &str) -> bool {
        if let Some(keys) = self.action_mappings.get(action) {
            keys.iter().any(|key| self.is_key_pressed(*key))
        } else {
            false
        }
    }

    pub fn is_action_just_pressed(&self, action: &str) -> bool {
        if let Some(keys) = self.action_mappings.get(action) {
            keys.iter().any(|key| self.is_key_just_pressed(*key))
        } else {
            false
        }
    }

    pub fn is_action_just_released(&self, action: &str) -> bool {
        if let Some(keys) = self.action_mappings.get(action) {
            keys.iter().any(|key| self.is_key_just_released(*key))
        } else {
            false
        }
    }

    // Axis mapping methods
    pub fn add_axis_mapping(&mut self, axis: String, key_values: Vec<(Key, f32)>) {
        self.axis_mappings.insert(axis, key_values);
    }

    pub fn get_axis_value(&self, axis: &str) -> f32 {
        if let Some(key_values) = self.axis_mappings.get(axis) {
            key_values.iter()
                .filter(|(key, _)| self.is_key_pressed(*key))
                .map(|(_, value)| *value)
                .sum()
        } else {
            0.0
        }
    }

    // Convenience methods for common input patterns
    pub fn get_movement_input(&self) -> (f32, f32) {
        let mut x = 0.0;
        let mut y = 0.0;

        if self.is_key_pressed(Key::W) || self.is_key_pressed(Key::Up) {
            y += 1.0;
        }
        if self.is_key_pressed(Key::S) || self.is_key_pressed(Key::Down) {
            y -= 1.0;
        }
        if self.is_key_pressed(Key::A) || self.is_key_pressed(Key::Left) {
            x -= 1.0;
        }
        if self.is_key_pressed(Key::D) || self.is_key_pressed(Key::Right) {
            x += 1.0;
        }

        (x, y)
    }

    pub fn get_3d_movement_input(&self) -> (f32, f32, f32) {
        let (x, z) = self.get_movement_input();
        let mut y = 0.0;

        if self.is_key_pressed(Key::Space) {
            y += 1.0;
        }
        if self.is_key_pressed(Key::LShift) || self.is_key_pressed(Key::RShift) {
            y -= 1.0;
        }

        (x, y, z)
    }

    // Setup default input mappings
    pub fn setup_default_mappings(&mut self) {
        // Movement actions
        self.add_action_mapping("MoveForward".to_string(), vec![Key::W, Key::Up]);
        self.add_action_mapping("MoveBackward".to_string(), vec![Key::S, Key::Down]);
        self.add_action_mapping("MoveLeft".to_string(), vec![Key::A, Key::Left]);
        self.add_action_mapping("MoveRight".to_string(), vec![Key::D, Key::Right]);
        self.add_action_mapping("Jump".to_string(), vec![Key::Space]);
        self.add_action_mapping("Crouch".to_string(), vec![Key::LShift, Key::RShift]);

        // Game actions
        self.add_action_mapping("Interact".to_string(), vec![Key::E]);
        self.add_action_mapping("Attack".to_string(), vec![Key::MouseLeft]);
        self.add_action_mapping("Block".to_string(), vec![Key::MouseRight]);
        self.add_action_mapping("Pause".to_string(), vec![Key::Escape]);

        // Movement axes
        self.add_axis_mapping("MoveHorizontal".to_string(), vec![
            (Key::A, -1.0), (Key::Left, -1.0),
            (Key::D, 1.0), (Key::Right, 1.0),
        ]);
        self.add_axis_mapping("MoveVertical".to_string(), vec![
            (Key::S, -1.0), (Key::Down, -1.0),
            (Key::W, 1.0), (Key::Up, 1.0),
        ]);
    }

    // Internal mapping functions
    fn map_kiss3d_key(&self, key: kiss3d::event::Key) -> Key {
        match key {
            kiss3d::event::Key::A => Key::A,
            kiss3d::event::Key::B => Key::B,
            kiss3d::event::Key::C => Key::C,
            kiss3d::event::Key::D => Key::D,
            kiss3d::event::Key::E => Key::E,
            kiss3d::event::Key::F => Key::F,
            kiss3d::event::Key::G => Key::G,
            kiss3d::event::Key::H => Key::H,
            kiss3d::event::Key::I => Key::I,
            kiss3d::event::Key::J => Key::J,
            kiss3d::event::Key::K => Key::K,
            kiss3d::event::Key::L => Key::L,
            kiss3d::event::Key::M => Key::M,
            kiss3d::event::Key::N => Key::N,
            kiss3d::event::Key::O => Key::O,
            kiss3d::event::Key::P => Key::P,
            kiss3d::event::Key::Q => Key::Q,
            kiss3d::event::Key::R => Key::R,
            kiss3d::event::Key::S => Key::S,
            kiss3d::event::Key::T => Key::T,
            kiss3d::event::Key::U => Key::U,
            kiss3d::event::Key::V => Key::V,
            kiss3d::event::Key::W => Key::W,
            kiss3d::event::Key::X => Key::X,
            kiss3d::event::Key::Y => Key::Y,
            kiss3d::event::Key::Z => Key::Z,
            
            kiss3d::event::Key::Key0 => Key::Key0,
            kiss3d::event::Key::Key1 => Key::Key1,
            kiss3d::event::Key::Key2 => Key::Key2,
            kiss3d::event::Key::Key3 => Key::Key3,
            kiss3d::event::Key::Key4 => Key::Key4,
            kiss3d::event::Key::Key5 => Key::Key5,
            kiss3d::event::Key::Key6 => Key::Key6,
            kiss3d::event::Key::Key7 => Key::Key7,
            kiss3d::event::Key::Key8 => Key::Key8,
            kiss3d::event::Key::Key9 => Key::Key9,
            
            kiss3d::event::Key::F1 => Key::F1,
            kiss3d::event::Key::F2 => Key::F2,
            kiss3d::event::Key::F3 => Key::F3,
            kiss3d::event::Key::F4 => Key::F4,
            kiss3d::event::Key::F5 => Key::F5,
            kiss3d::event::Key::F6 => Key::F6,
            kiss3d::event::Key::F7 => Key::F7,
            kiss3d::event::Key::F8 => Key::F8,
            kiss3d::event::Key::F9 => Key::F9,
            kiss3d::event::Key::F10 => Key::F10,
            kiss3d::event::Key::F11 => Key::F11,
            kiss3d::event::Key::F12 => Key::F12,
            
            kiss3d::event::Key::Space => Key::Space,
            kiss3d::event::Key::Enter => Key::Enter,
            kiss3d::event::Key::Escape => Key::Escape,
            kiss3d::event::Key::Tab => Key::Tab,
            kiss3d::event::Key::Backspace => Key::Backspace,
            kiss3d::event::Key::Delete => Key::Delete,
            kiss3d::event::Key::Insert => Key::Insert,
            kiss3d::event::Key::Home => Key::Home,
            kiss3d::event::Key::End => Key::End,
            kiss3d::event::Key::PageUp => Key::PageUp,
            kiss3d::event::Key::PageDown => Key::PageDown,
            
            kiss3d::event::Key::Up => Key::Up,
            kiss3d::event::Key::Down => Key::Down,
            kiss3d::event::Key::Left => Key::Left,
            kiss3d::event::Key::Right => Key::Right,
            
            kiss3d::event::Key::LShift => Key::LShift,
            kiss3d::event::Key::RShift => Key::RShift,
            kiss3d::event::Key::LControl => Key::LCtrl,
            kiss3d::event::Key::RControl => Key::RCtrl,
            kiss3d::event::Key::LAlt => Key::LAlt,
            kiss3d::event::Key::RAlt => Key::RAlt,
            
            _ => Key::Unknown,
        }
    }

    fn map_kiss3d_mouse_button(&self, button: kiss3d::event::MouseButton) -> MouseButton {
        match button {
            kiss3d::event::MouseButton::Button1 => MouseButton::Left,
            kiss3d::event::MouseButton::Button2 => MouseButton::Right,
            kiss3d::event::MouseButton::Button3 => MouseButton::Middle,
            kiss3d::event::MouseButton::Button4 => MouseButton::Other(4),
            kiss3d::event::MouseButton::Button5 => MouseButton::Other(5),
            kiss3d::event::MouseButton::Button6 => MouseButton::Other(6),
            kiss3d::event::MouseButton::Button7 => MouseButton::Other(7),
            kiss3d::event::MouseButton::Button8 => MouseButton::Other(8),
        }
    }
}

impl Default for InputManager {
    fn default() -> Self {
        let mut manager = Self::new();
        manager.setup_default_mappings();
        manager
    }
}