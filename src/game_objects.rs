// game_objects.rs - Game object implementations

use crate::math::{Vector2D, Vector3D};
use kiss3d::scene::SceneNode;
use kiss3d::nalgebra::{Translation3, UnitQuaternion, Vector3};
use std::sync::{Arc, Mutex};

// Base GameObject trait
pub trait GameObject {
    fn update(&mut self, delta_time: f32);
    fn get_position(&self) -> Vector3D;
    fn set_position(&mut self, position: Vector3D);
    fn get_velocity(&self) -> Vector3D;
    fn set_velocity(&mut self, velocity: Vector3D);
    fn is_active(&self) -> bool;
    fn set_active(&mut self, active: bool);
    fn get_name(&self) -> &str;
    fn get_bounds(&self) -> BoundingBox;
    fn on_collision(&mut self, other: &dyn GameObject);
}

// Bounding box for collision detection
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct BoundingBox {
    pub min: Vector3D,
    pub max: Vector3D,
}

impl BoundingBox {
    pub fn new(min: Vector3D, max: Vector3D) -> Self {
        Self { min, max }
    }

    pub fn from_center_size(center: Vector3D, size: Vector3D) -> Self {
        let half_size = size * 0.5;
        Self {
            min: center - half_size,
            max: center + half_size,
        }
    }

    pub fn intersects(&self, other: &BoundingBox) -> bool {
        self.min.x <= other.max.x && self.max.x >= other.min.x &&
        self.min.y <= other.max.y && self.max.y >= other.min.y &&
        self.min.z <= other.max.z && self.max.z >= other.min.z
    }

    pub fn contains_point(&self, point: Vector3D) -> bool {
        point.x >= self.min.x && point.x <= self.max.x &&
        point.y >= self.min.y && point.y <= self.max.y &&
        point.z >= self.min.z && point.z <= self.max.z
    }

    pub fn center(&self) -> Vector3D {
        (self.min + self.max) * 0.5
    }

    pub fn size(&self) -> Vector3D {
        self.max - self.min
    }
}

// Player implementation
pub struct Player {
    name: String,
    position: Vector3D,
    velocity: Vector3D,
    acceleration: Vector3D,
    speed: f32,
    jump_force: f32,
    active: bool,
    grounded: bool,
    health: i32,
    max_health: i32,
    scene_node: Option<SceneNode>,
    size: Vector3D,
}

impl Player {
    pub fn new(name: String, position: Vector3D) -> Self {
        Self {
            name,
            position,
            velocity: Vector3D::zero(),
            acceleration: Vector3D::zero(),
            speed: 5.0,
            jump_force: 10.0,
            active: true,
            grounded: false,
            health: 100,
            max_health: 100,
            scene_node: None,
            size: Vector3D::new(1.0, 2.0, 1.0),
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
        self.update_scene_node();
    }

    pub fn move_forward(&mut self, amount: f32) {
        self.velocity.z -= amount * self.speed;
    }

    pub fn move_backward(&mut self, amount: f32) {
        self.velocity.z += amount * self.speed;
    }

    pub fn move_left(&mut self, amount: f32) {
        self.velocity.x -= amount * self.speed;
    }

    pub fn move_right(&mut self, amount: f32) {
        self.velocity.x += amount * self.speed;
    }

    pub fn jump(&mut self) {
        if self.grounded {
            self.velocity.y = self.jump_force;
            self.grounded = false;
        }
    }

    pub fn apply_gravity(&mut self, gravity: f32, delta_time: f32) {
        if !self.grounded {
            self.acceleration.y -= gravity;
        }
    }

    pub fn set_grounded(&mut self, grounded: bool) {
        self.grounded = grounded;
        if grounded {
            self.velocity.y = 0.0;
            self.acceleration.y = 0.0;
        }
    }

    pub fn is_grounded(&self) -> bool {
        self.grounded
    }

    pub fn take_damage(&mut self, damage: i32) -> bool {
        self.health = (self.health - damage).max(0);
        self.health <= 0
    }

    pub fn heal(&mut self, amount: i32) {
        self.health = (self.health + amount).min(self.max_health);
    }

    pub fn get_health(&self) -> i32 {
        self.health
    }

    pub fn get_max_health(&self) -> i32 {
        self.max_health
    }

    pub fn get_health_percentage(&self) -> f32 {
        self.health as f32 / self.max_health as f32
    }

    fn update_scene_node(&mut self) {
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
            // Apply acceleration to velocity
            self.velocity = self.velocity + self.acceleration * delta_time;
            
            // Apply velocity to position
            self.position = self.position + self.velocity * delta_time;
            
            // Apply friction to horizontal movement
            let friction = 0.9;
            self.velocity.x *= friction;
            self.velocity.z *= friction;
            
            // Reset acceleration
            self.acceleration = Vector3D::zero();
            
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
    
    fn get_bounds(&self) -> BoundingBox {
        BoundingBox::from_center_size(self.position, self.size)
    }
    
    fn on_collision(&mut self, _other: &dyn GameObject) {
        // Handle collision with other objects
    }
}

// Collectible implementation
pub struct Collectible {
    name: String,
    position: Vector3D,
    velocity: Vector3D,
    value: i32,
    collected: Arc<Mutex<bool>>,
    active: bool,
    rotation: f32,
    rotation_speed: f32,
    bob_offset: f32,
    bob_speed: f32,
    scene_node: Option<SceneNode>,
    size: Vector3D,
    collectible_type: CollectibleType,
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum CollectibleType {
    Coin,
    Gem,
    PowerUp,
    HealthPack,
}

impl Collectible {
    pub fn new(name: String, position: Vector3D, value: i32, collectible_type: CollectibleType) -> Self {
        Self {
            name,
            position,
            velocity: Vector3D::zero(),
            value,
            collected: Arc::new(Mutex::new(false)),
            active: true,
            rotation: 0.0,
            rotation_speed: 2.0,
            bob_offset: 0.0,
            bob_speed: 3.0,
            scene_node: None,
            size: Vector3D::new(0.6, 0.6, 0.6),
            collectible_type,
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
        self.update_scene_node();
    }

    pub fn is_collected(&self) -> bool {
        *self.collected.lock().unwrap()
    }

    pub fn collect(&mut self) -> i32 {
        let mut collected = self.collected.lock().unwrap();
        if !*collected {
            *collected = true;
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

    pub fn get_type(&self) -> CollectibleType {
        self.collectible_type
    }

    fn update_scene_node(&mut self) {
        if let Some(ref mut node) = self.scene_node {
            // Update position with bobbing effect
            let bob_y = (self.bob_offset).sin() * 0.2;
            node.set_local_translation(Translation3::new(
                self.position.x,
                self.position.y + bob_y,
                self.position.z,
            ));
            
            // Update rotation
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
        if self.active && !self.is_collected() {
            // Update position
            self.position = self.position + self.velocity * delta_time;
            
            // Update rotation for visual effect
            self.rotation += self.rotation_speed * delta_time;
            if self.rotation > 2.0 * std::f32::consts::PI {
                self.rotation -= 2.0 * std::f32::consts::PI;
            }
            
            // Update bobbing effect
            self.bob_offset += self.bob_speed * delta_time;
            if self.bob_offset > 2.0 * std::f32::consts::PI {
                self.bob_offset -= 2.0 * std::f32::consts::PI;
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
    fn is_active(&self) -> bool { self.active && !self.is_collected() }
    fn set_active(&mut self, active: bool) { self.active = active; }
    fn get_name(&self) -> &str { &self.name }
    
    fn get_bounds(&self) -> BoundingBox {
        BoundingBox::from_center_size(self.position, self.size)
    }
    
    fn on_collision(&mut self, other: &dyn GameObject) {
        // Check if colliding with player
        if other.get_name() == "Player" {
            self.collect();
        }
    }
}

// Enemy implementation
pub struct Enemy {
    name: String,
    position: Vector3D,
    velocity: Vector3D,
    target_position: Option<Vector3D>,
    speed: f32,
    health: i32,
    max_health: i32,
    damage: i32,
    active: bool,
    ai_state: EnemyAIState,
    patrol_points: Vec<Vector3D>,
    current_patrol_index: usize,
    detection_range: f32,
    attack_range: f32,
    attack_cooldown: f32,
    last_attack_time: f32,
    scene_node: Option<SceneNode>,
    size: Vector3D,
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum EnemyAIState {
    Idle,
    Patrolling,
    Chasing,
    Attacking,
    Returning,
}

impl Enemy {
    pub fn new(name: String, position: Vector3D) -> Self {
        Self {
            name,
            position,
            velocity: Vector3D::zero(),
            target_position: None,
            speed: 3.0,
            health: 50,
            max_health: 50,
            damage: 10,
            active: true,
            ai_state: EnemyAIState::Idle,
            patrol_points: Vec::new(),
            current_patrol_index: 0,
            detection_range: 8.0,
            attack_range: 2.0,
            attack_cooldown: 1.0,
            last_attack_time: 0.0,
            scene_node: None,
            size: Vector3D::new(1.0, 1.5, 1.0),
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
        self.update_scene_node();
    }

    pub fn add_patrol_point(&mut self, point: Vector3D) {
        self.patrol_points.push(point);
    }

    pub fn set_target(&mut self, target: Vector3D) {
        self.target_position = Some(target);
        self.ai_state = EnemyAIState::Chasing;
    }

    pub fn clear_target(&mut self) {
        self.target_position = None;
        self.ai_state = EnemyAIState::Returning;
    }

    pub fn can_attack(&self, current_time: f32) -> bool {
        current_time - self.last_attack_time >= self.attack_cooldown
    }

    pub fn attack(&mut self, current_time: f32) -> i32 {
        if self.can_attack(current_time) {
            self.last_attack_time = current_time;
            self.damage
        } else {
            0
        }
    }

    pub fn take_damage(&mut self, damage: i32) -> bool {
        self.health = (self.health - damage).max(0);
        self.health <= 0
    }

    pub fn get_health(&self) -> i32 {
        self.health
    }

    pub fn get_ai_state(&self) -> EnemyAIState {
        self.ai_state
    }

    pub fn update_ai(&mut self, player_position: Vector3D, current_time: f32) {
        let distance_to_player = self.position.distance(&player_position);

        match self.ai_state {
            EnemyAIState::Idle => {
                if !self.patrol_points.is_empty() {
                    self.ai_state = EnemyAIState::Patrolling;
                } else if distance_to_player <= self.detection_range {
                    self.set_target(player_position);
                }
            },
            EnemyAIState::Patrolling => {
                if distance_to_player <= self.detection_range {
                    self.set_target(player_position);
                } else {
                    self.patrol();
                }
            },
            EnemyAIState::Chasing => {
                if distance_to_player > self.detection_range * 1.5 {
                    self.clear_target();
                } else if distance_to_player <= self.attack_range {
                    self.ai_state = EnemyAIState::Attacking;
                } else {
                    self.set_target(player_position);
                    self.move_towards_target();
                }
            },
            EnemyAIState::Attacking => {
                if distance_to_player > self.attack_range {
                    self.ai_state = EnemyAIState::Chasing;
                }
                // Attack logic handled elsewhere
            },
            EnemyAIState::Returning => {
                if !self.patrol_points.is_empty() {
                    let patrol_point = self.patrol_points[self.current_patrol_index];
                    if self.position.distance(&patrol_point) < 1.0 {
                        self.ai_state = EnemyAIState::Patrolling;
                    } else {
                        self.move_towards(patrol_point);
                    }
                } else {
                    self.ai_state = EnemyAIState::Idle;
                }
            },
        }
    }

    fn patrol(&mut self) {
        if self.patrol_points.is_empty() {
            return;
        }

        let current_target = self.patrol_points[self.current_patrol_index];
        let distance = self.position.distance(&current_target);

        if distance < 1.0 {
            self.current_patrol_index = (self.current_patrol_index + 1) % self.patrol_points.len();
        } else {
            self.move_towards(current_target);
        }
    }

    fn move_towards_target(&mut self) {
        if let Some(target) = self.target_position {
            self.move_towards(target);
        }
    }

    fn move_towards(&mut self, target: Vector3D) {
        let direction = (target - self.position).normalize();
        self.velocity = direction * self.speed;
    }

    fn update_scene_node(&mut self) {
        if let Some(ref mut node) = self.scene_node {
            node.set_local_translation(Translation3::new(
                self.position.x,
                self.position.y,
                self.position.z,
            ));
        }
    }
}

impl GameObject for Enemy {
    fn update(&mut self, delta_time: f32) {
        if self.active && self.health > 0 {
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
    fn is_active(&self) -> bool { self.active && self.health > 0 }
    fn set_active(&mut self, active: bool) { self.active = active; }
    fn get_name(&self) -> &str { &self.name }
    
    fn get_bounds(&self) -> BoundingBox {
        BoundingBox::from_center_size(self.position, self.size)
    }
    
    fn on_collision(&mut self, other: &dyn GameObject) {
        // Handle collision with other objects
        if other.get_name() == "Player" {
            // Player collision handled in game logic
        }
    }
}

// Platform implementation for platformer games
pub struct Platform {
    name: String,
    position: Vector3D,
    size: Vector3D,
    active: bool,
    scene_node: Option<SceneNode>,
    platform_type: PlatformType,
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum PlatformType {
    Static,
    Moving,
    Breakable,
    Bouncy,
}

impl Platform {
    pub fn new(name: String, position: Vector3D, size: Vector3D, platform_type: PlatformType) -> Self {
        Self {
            name,
            position,
            size,
            active: true,
            scene_node: None,
            platform_type,
        }
    }

    pub fn set_scene_node(&mut self, node: SceneNode) {
        self.scene_node = Some(node);
        self.update_scene_node();
    }

    pub fn get_type(&self) -> PlatformType {
        self.platform_type
    }

    fn update_scene_node(&mut self) {
        if let Some(ref mut node) = self.scene_node {
            node.set_local_translation(Translation3::new(
                self.position.x,
                self.position.y,
                self.position.z,
            ));
        }
    }
}

impl GameObject for Platform {
    fn update(&mut self, _delta_time: f32) {
        // Platforms are generally static, but moving platforms could be implemented here
    }

    fn get_position(&self) -> Vector3D { self.position }
    fn set_position(&mut self, position: Vector3D) { 
        self.position = position;
        self.update_scene_node();
    }
    fn get_velocity(&self) -> Vector3D { Vector3D::zero() }
    fn set_velocity(&mut self, _velocity: Vector3D) { /* Platforms don't move by default */ }
    fn is_active(&self) -> bool { self.active }
    fn set_active(&mut self, active: bool) { self.active = active; }
    fn get_name(&self) -> &str { &self.name }
    
    fn get_bounds(&self) -> BoundingBox {
        BoundingBox::from_center_size(self.position, self.size)
    }
    
    fn on_collision(&mut self, _other: &dyn GameObject) {
        // Handle collision with other objects
    }
}