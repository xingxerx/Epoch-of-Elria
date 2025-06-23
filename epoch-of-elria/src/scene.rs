// scene.rs - Scene management system

use crate::math::Vector3D;
use crate::game_objects::{GameObject, Player, Collectible, Enemy, Platform};
use crate::physics::{PhysicsWorld, RigidBody};
use std::collections::HashMap;

pub struct Scene {
    name: String,
    objects: HashMap<usize, Box<dyn GameObject>>,
    physics_world: PhysicsWorld,
    next_object_id: usize,
    active: bool,
    background_color: (f32, f32, f32),
    ambient_light: f32,
    camera_position: Vector3D,
    camera_target: Vector3D,
}

impl Scene {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            objects: HashMap::new(),
            physics_world: PhysicsWorld::new(),
            next_object_id: 0,
            active: true,
            background_color: (0.1, 0.1, 0.2),
            ambient_light: 0.3,
            camera_position: Vector3D::new(0.0, 5.0, 10.0),
            camera_target: Vector3D::zero(),
        }
    }

    pub fn update(&mut self, delta_time: f32) {
        if !self.active {
            return;
        }

        // Update physics
        self.physics_world.step(delta_time);

        // Update all game objects
        let object_ids: Vec<usize> = self.objects.keys().cloned().collect();
        for id in object_ids {
            if let Some(object) = self.objects.get_mut(&id) {
                object.update(delta_time);
                
                // Sync physics body position with game object
                if let Some(physics_body) = self.physics_world.get_rigid_body_mut(id) {
                    object.set_position(physics_body.position);
                    object.set_velocity(physics_body.velocity);
                }
            }
        }

        // Handle object removal (objects marked for deletion)
        self.objects.retain(|&id, object| {
            if !object.is_active() {
                self.physics_world.remove_rigid_body(id);
                false
            } else {
                true
            }
        });
    }

    pub fn render(&self) {
        // Rendering is handled by the rendering system
        // This method could be used for scene-specific rendering logic
    }

    // Object management
    pub fn add_object(&mut self, mut object: Box<dyn GameObject>) -> usize {
        let id = self.next_object_id;
        self.next_object_id += 1;

        // Create physics body for the object
        let position = object.get_position();
        let size = object.get_size();
        let physics_body = RigidBody::new(position, size);
        self.physics_world.add_rigid_body(physics_body);

        object.set_id(id);
        self.objects.insert(id, object);
        id
    }

    pub fn add_static_object(&mut self, mut object: Box<dyn GameObject>) -> usize {
        let id = self.next_object_id;
        self.next_object_id += 1;

        // Create static physics body
        let position = object.get_position();
        let size = object.get_size();
        let physics_body = RigidBody::new_static(position, size);
        self.physics_world.add_rigid_body(physics_body);

        object.set_id(id);
        self.objects.insert(id, object);
        id
    }

    pub fn remove_object(&mut self, id: usize) -> bool {
        if let Some(_) = self.objects.remove(&id) {
            self.physics_world.remove_rigid_body(id);
            true
        } else {
            false
        }
    }

    pub fn get_object(&self, id: usize) -> Option<&dyn GameObject> {
        self.objects.get(&id).map(|obj| obj.as_ref())
    }

    pub fn get_object_mut(&mut self, id: usize) -> Option<&mut Box<dyn GameObject>> {
        self.objects.get_mut(&id)
    }

    pub fn find_object_by_name(&self, name: &str) -> Option<(usize, &dyn GameObject)> {
        self.objects.iter()
            .find(|(_, obj)| obj.get_name() == name)
            .map(|(&id, obj)| (id, obj.as_ref()))
    }

    pub fn find_objects_by_type<T: GameObject + 'static>(&self) -> Vec<(usize, &T)> {
        let mut results = Vec::new();
        for (&id, obj) in &self.objects {
            if let Some(typed_obj) = obj.as_any().downcast_ref::<T>() {
                results.push((id, typed_obj));
            }
        }
        results
    }

    pub fn get_object_count(&self) -> usize {
        self.objects.len()
    }

    pub fn get_all_object_ids(&self) -> Vec<usize> {
        self.objects.keys().cloned().collect()
    }

    // Convenience methods for common object types
    pub fn add_player(&mut self, position: Vector3D) -> usize {
        let player = Box::new(Player::new(position));
        self.add_object(player)
    }

    pub fn add_collectible(&mut self, position: Vector3D, value: i32) -> usize {
        let collectible = Box::new(Collectible::new(position, value, crate::game_objects::CollectibleType::Coin));
        self.add_object(collectible)
    }

    pub fn add_enemy(&mut self, position: Vector3D) -> usize {
        let enemy = Box::new(Enemy::new(position));
        self.add_object(enemy)
    }

    pub fn add_platform(&mut self, size: Vector3D, position: Vector3D) -> usize {
        let platform = Box::new(Platform::new(size, position, crate::game_objects::PlatformType::Static));
        self.add_static_object(platform)
    }

    // Physics integration
    pub fn get_physics_world(&self) -> &PhysicsWorld {
        &self.physics_world
    }

    pub fn get_physics_world_mut(&mut self) -> &mut PhysicsWorld {
        &mut self.physics_world
    }

    pub fn apply_force_to_object(&mut self, object_id: usize, force: Vector3D) {
        if let Some(physics_body) = self.physics_world.get_rigid_body_mut(object_id) {
            physics_body.add_force(force);
        }
    }

    pub fn apply_impulse_to_object(&mut self, object_id: usize, impulse: Vector3D) {
        if let Some(physics_body) = self.physics_world.get_rigid_body_mut(object_id) {
            physics_body.add_impulse(impulse);
        }
    }

    pub fn set_object_velocity(&mut self, object_id: usize, velocity: Vector3D) {
        if let Some(physics_body) = self.physics_world.get_rigid_body_mut(object_id) {
            physics_body.set_velocity(velocity);
        }
    }

    // Scene properties
    pub fn get_name(&self) -> &str {
        &self.name
    }

    pub fn set_name(&mut self, name: &str) {
        self.name = name.to_string();
    }

    pub fn is_active(&self) -> bool {
        self.active
    }

    pub fn set_active(&mut self, active: bool) {
        self.active = active;
    }

    pub fn get_background_color(&self) -> (f32, f32, f32) {
        self.background_color
    }

    pub fn set_background_color(&mut self, r: f32, g: f32, b: f32) {
        self.background_color = (r.clamp(0.0, 1.0), g.clamp(0.0, 1.0), b.clamp(0.0, 1.0));
    }

    pub fn get_ambient_light(&self) -> f32 {
        self.ambient_light
    }

    pub fn set_ambient_light(&mut self, intensity: f32) {
        self.ambient_light = intensity.clamp(0.0, 1.0);
    }

    // Camera management
    pub fn get_camera_position(&self) -> Vector3D {
        self.camera_position
    }

    pub fn set_camera_position(&mut self, position: Vector3D) {
        self.camera_position = position;
    }

    pub fn get_camera_target(&self) -> Vector3D {
        self.camera_target
    }

    pub fn set_camera_target(&mut self, target: Vector3D) {
        self.camera_target = target;
    }

    pub fn move_camera(&mut self, delta: Vector3D) {
        self.camera_position = self.camera_position + delta;
    }

    pub fn look_at(&mut self, target: Vector3D) {
        self.camera_target = target;
    }

    // Utility methods
    pub fn clear(&mut self) {
        self.objects.clear();
        self.physics_world = PhysicsWorld::new();
        self.next_object_id = 0;
    }

    pub fn get_objects_in_radius(&self, center: Vector3D, radius: f32) -> Vec<usize> {
        let mut results = Vec::new();
        let radius_squared = radius * radius;

        for (&id, obj) in &self.objects {
            let distance_squared = (obj.get_position() - center).magnitude_squared();
            if distance_squared <= radius_squared {
                results.push(id);
            }
        }

        results
    }

    pub fn get_closest_object(&self, position: Vector3D) -> Option<(usize, f32)> {
        let mut closest_id = None;
        let mut closest_distance = f32::INFINITY;

        for (&id, obj) in &self.objects {
            let distance = (obj.get_position() - position).magnitude();
            if distance < closest_distance {
                closest_distance = distance;
                closest_id = Some(id);
            }
        }

        closest_id.map(|id| (id, closest_distance))
    }

    // Collision queries
    pub fn raycast(&self, origin: Vector3D, direction: Vector3D, max_distance: f32) -> Option<RaycastResult> {
        if let Some(hit) = self.physics_world.raycast(origin, direction, max_distance) {
            Some(RaycastResult {
                object_id: hit.body_id,
                point: hit.point,
                normal: hit.normal,
                distance: hit.distance,
            })
        } else {
            None
        }
    }

    pub fn overlap_sphere(&self, center: Vector3D, radius: f32) -> Vec<usize> {
        // Simple sphere overlap test
        self.get_objects_in_radius(center, radius)
    }

    // Scene serialization (placeholder)
    pub fn save_to_file(&self, filename: &str) -> Result<(), Box<dyn std::error::Error>> {
        // In a real implementation, this would serialize the scene to a file
        println!("Saving scene '{}' to file: {}", self.name, filename);
        Ok(())
    }

    pub fn load_from_file(&mut self, filename: &str) -> Result<(), Box<dyn std::error::Error>> {
        // In a real implementation, this would load the scene from a file
        println!("Loading scene from file: {}", filename);
        Ok(())
    }

    // Debug information
    pub fn print_debug_info(&self) {
        println!("Scene: {}", self.name);
        println!("  Active: {}", self.active);
        println!("  Objects: {}", self.objects.len());
        println!("  Camera Position: {:?}", self.camera_position);
        println!("  Camera Target: {:?}", self.camera_target);
        println!("  Background Color: {:?}", self.background_color);
        println!("  Ambient Light: {:.2}", self.ambient_light);
    }
}

// Scene manager for handling multiple scenes
pub struct SceneManager {
    scenes: HashMap<String, Scene>,
    active_scene: Option<String>,
    transition_state: TransitionState,
}

impl SceneManager {
    pub fn new() -> Self {
        Self {
            scenes: HashMap::new(),
            active_scene: None,
            transition_state: TransitionState::None,
        }
    }

    pub fn add_scene(&mut self, scene: Scene) {
        let name = scene.get_name().to_string();
        self.scenes.insert(name, scene);
    }

    pub fn remove_scene(&mut self, name: &str) -> bool {
        if let Some(active) = &self.active_scene {
            if active == name {
                self.active_scene = None;
            }
        }
        self.scenes.remove(name).is_some()
    }

    pub fn set_active_scene(&mut self, name: &str) -> bool {
        if self.scenes.contains_key(name) {
            // Deactivate current scene
            if let Some(current_name) = &self.active_scene {
                if let Some(current_scene) = self.scenes.get_mut(current_name) {
                    current_scene.set_active(false);
                }
            }

            // Activate new scene
            if let Some(new_scene) = self.scenes.get_mut(name) {
                new_scene.set_active(true);
                self.active_scene = Some(name.to_string());
                true
            } else {
                false
            }
        } else {
            false
        }
    }

    pub fn get_active_scene(&self) -> Option<&Scene> {
        if let Some(name) = &self.active_scene {
            self.scenes.get(name)
        } else {
            None
        }
    }

    pub fn get_active_scene_mut(&mut self) -> Option<&mut Scene> {
        if let Some(name) = &self.active_scene {
            self.scenes.get_mut(name)
        } else {
            None
        }
    }

    pub fn get_scene(&self, name: &str) -> Option<&Scene> {
        self.scenes.get(name)
    }

    pub fn get_scene_mut(&mut self, name: &str) -> Option<&mut Scene> {
        self.scenes.get_mut(name)
    }

    pub fn update(&mut self, delta_time: f32) {
        if let Some(scene) = self.get_active_scene_mut() {
            scene.update(delta_time);
        }
    }

    pub fn render(&self) {
        if let Some(scene) = self.get_active_scene() {
            scene.render();
        }
    }

    pub fn get_scene_names(&self) -> Vec<String> {
        self.scenes.keys().cloned().collect()
    }

    pub fn get_active_scene_name(&self) -> Option<&String> {
        self.active_scene.as_ref()
    }

    // Scene transitions
    pub fn transition_to_scene(&mut self, name: &str, transition_type: TransitionType) -> bool {
        if self.scenes.contains_key(name) {
            self.transition_state = TransitionState::InProgress {
                from: self.active_scene.clone(),
                to: name.to_string(),
                transition_type,
                progress: 0.0,
            };
            true
        } else {
            false
        }
    }

    pub fn update_transition(&mut self, delta_time: f32) {
        match &mut self.transition_state {
            TransitionState::InProgress { progress, to, .. } => {
                *progress += delta_time * 2.0; // 0.5 second transition
                
                if *progress >= 1.0 {
                    let target_scene = to.clone();
                    self.transition_state = TransitionState::None;
                    self.set_active_scene(&target_scene);
                }
            },
            _ => {},
        }
    }

    pub fn is_transitioning(&self) -> bool {
        matches!(self.transition_state, TransitionState::InProgress { .. })
    }
}

impl Default for SceneManager {
    fn default() -> Self {
        Self::new()
    }
}

#[derive(Debug, Clone)]
pub enum TransitionState {
    None,
    InProgress {
        from: Option<String>,
        to: String,
        transition_type: TransitionType,
        progress: f32,
    },
}

#[derive(Debug, Clone, Copy)]
pub enum TransitionType {
    Fade,
    Slide,
    Instant,
}

#[derive(Debug, Clone)]
pub struct RaycastResult {
    pub object_id: usize,
    pub point: Vector3D,
    pub normal: Vector3D,
    pub distance: f32,
}

// Scene templates for common scene types
pub struct SceneTemplates;

impl SceneTemplates {
    pub fn create_test_scene() -> Scene {
        let mut scene = Scene::new("Test Scene");
        
        // Add a player
        scene.add_player(Vector3D::new(0.0, 1.0, 0.0));
        
        // Add some collectibles
        for i in 0..5 {
            let x = (i as f32 - 2.0) * 3.0;
            scene.add_collectible(Vector3D::new(x, 1.0, -5.0), 10);
        }
        
        // Add platforms
        scene.add_platform(Vector3D::new(0.0, -1.0, 0.0), Vector3D::new(20.0, 1.0, 20.0));
        scene.add_platform(Vector3D::new(-8.0, 2.0, -5.0), Vector3D::new(4.0, 1.0, 4.0));
        scene.add_platform(Vector3D::new(8.0, 3.0, -5.0), Vector3D::new(4.0, 1.0, 4.0));
        
        // Add some enemies
        scene.add_enemy(Vector3D::new(-5.0, 1.0, -8.0));
        scene.add_enemy(Vector3D::new(5.0, 1.0, -8.0));
        
        scene.set_background_color(0.2, 0.3, 0.6);
        scene.set_ambient_light(0.4);
        
        scene
    }

    pub fn create_empty_scene(name: &str) -> Scene {
        Scene::new(name)
    }

    pub fn create_platform_scene() -> Scene {
        let mut scene = Scene::new("Platform Scene");
        
        // Ground
        scene.add_platform(Vector3D::new(0.0, -2.0, 0.0), Vector3D::new(50.0, 2.0, 50.0));
        
        // Platforms at different heights
        for i in 0..10 {
            let x = (i as f32 - 5.0) * 4.0;
            let y = (i % 3) as f32 * 2.0;
            scene.add_platform(Vector3D::new(x, y, -10.0), Vector3D::new(3.0, 0.5, 3.0));
        }
        
        scene.add_player(Vector3D::new(0.0, 1.0, 0.0));
        scene.set_background_color(0.4, 0.6, 0.8);
        
        scene
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_scene_creation() {
        let scene = Scene::new("Test");
        assert_eq!(scene.get_name(), "Test");
        assert!(scene.is_active());
        assert_eq!(scene.get_object_count(), 0);
    }

    #[test]
    fn test_scene_manager() {
        let mut manager = SceneManager::new();
        let scene1 = Scene::new("Scene1");
        let scene2 = Scene::new("Scene2");
        
        manager.add_scene(scene1);
        manager.add_scene(scene2);
        
        assert!(manager.set_active_scene("Scene1"));
        assert_eq!(manager.get_active_scene_name(), Some(&"Scene1".to_string()));
        
        assert!(manager.set_active_scene("Scene2"));
        assert_eq!(manager.get_active_scene_name(), Some(&"Scene2".to_string()));
    }

    #[test]
    fn test_object_management() {
        let mut scene = Scene::new("Test");
        
        let player_id = scene.add_player(Vector3D::new(0.0, 0.0, 0.0));
        let collectible_id = scene.add_collectible(Vector3D::new(1.0, 0.0, 0.0), 10);
        
        assert_eq!(scene.get_object_count(), 2);
        
        assert!(scene.remove_object(player_id));
        assert_eq!(scene.get_object_count(), 1);
        
        assert!(scene.remove_object(collectible_id));
        assert_eq!(scene.get_object_count(), 0);
    }
}