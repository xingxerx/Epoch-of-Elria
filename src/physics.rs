// physics.rs - Physics system for the game engine

use crate::math::Vector3D;
use crate::game_objects::{GameObject, BoundingBox};
use std::collections::HashMap;

pub struct PhysicsWorld {
    gravity: Vector3D,
    air_resistance: f32,
    ground_friction: f32,
    collision_pairs: Vec<(usize, usize)>,
    rigid_bodies: HashMap<usize, RigidBody>,
    next_id: usize,
    time_accumulator: f32,
    fixed_timestep: f32,
}

impl PhysicsWorld {
    pub fn new() -> Self {
        Self {
            gravity: Vector3D::new(0.0, -9.81, 0.0),
            air_resistance: 0.01,
            ground_friction: 0.8,
            collision_pairs: Vec::new(),
            rigid_bodies: HashMap::new(),
            next_id: 0,
            time_accumulator: 0.0,
            fixed_timestep: 1.0 / 60.0, // 60 FPS physics
        }
    }

    pub fn step(&mut self, delta_time: f32) {
        self.time_accumulator += delta_time;
        
        while self.time_accumulator >= self.fixed_timestep {
            self.fixed_step(self.fixed_timestep);
            self.time_accumulator -= self.fixed_timestep;
        }
    }

    fn fixed_step(&mut self, dt: f32) {
        // Apply forces
        self.apply_forces(dt);
        
        // Integrate velocities
        self.integrate_velocities(dt);
        
        // Detect collisions
        self.detect_collisions();
        
        // Resolve collisions
        self.resolve_collisions();
        
        // Integrate positions
        self.integrate_positions(dt);
    }

    pub fn add_rigid_body(&mut self, body: RigidBody) -> usize {
        let id = self.next_id;
        self.next_id += 1;
        self.rigid_bodies.insert(id, body);
        id
    }

    pub fn remove_rigid_body(&mut self, id: usize) {
        self.rigid_bodies.remove(&id);
    }

    pub fn get_rigid_body(&self, id: usize) -> Option<&RigidBody> {
        self.rigid_bodies.get(&id)
    }

    pub fn get_rigid_body_mut(&mut self, id: usize) -> Option<&mut RigidBody> {
        self.rigid_bodies.get_mut(&id)
    }

    pub fn set_gravity(&mut self, gravity: Vector3D) {
        self.gravity = gravity;
    }

    pub fn get_gravity(&self) -> Vector3D {
        self.gravity
    }

    pub fn set_air_resistance(&mut self, resistance: f32) {
        self.air_resistance = resistance;
    }

    pub fn set_ground_friction(&mut self, friction: f32) {
        self.ground_friction = friction;
    }

    fn apply_forces(&mut self, dt: f32) {
        for (_, body) in self.rigid_bodies.iter_mut() {
            if body.body_type == RigidBodyType::Dynamic {
                // Apply gravity
                if body.use_gravity {
                    body.add_force(self.gravity * body.mass);
                }
                
                // Apply air resistance
                let air_force = body.velocity * (-self.air_resistance * body.velocity.magnitude());
                body.add_force(air_force);
                
                // Apply ground friction if grounded
                if body.grounded {
                    let friction_force = Vector3D::new(
                        -body.velocity.x * self.ground_friction * body.mass,
                        0.0,
                        -body.velocity.z * self.ground_friction * body.mass,
                    );
                    body.add_force(friction_force);
                }
            }
        }
    }

    fn integrate_velocities(&mut self, dt: f32) {
        for (_, body) in self.rigid_bodies.iter_mut() {
            if body.body_type == RigidBodyType::Dynamic {
                // v = v + (F/m) * dt
                let acceleration = body.force * (1.0 / body.mass);
                body.velocity = body.velocity + acceleration * dt;
                
                // Apply velocity limits
                if body.max_velocity > 0.0 {
                    body.velocity = body.velocity.clamp_magnitude(body.max_velocity);
                }
                
                // Clear forces for next frame
                body.force = Vector3D::zero();
            }
        }
    }

    fn integrate_positions(&mut self, dt: f32) {
        for (_, body) in self.rigid_bodies.iter_mut() {
            if body.body_type == RigidBodyType::Dynamic {
                // x = x + v * dt
                body.position = body.position + body.velocity * dt;
            }
        }
    }

    fn detect_collisions(&mut self) {
        self.collision_pairs.clear();
        
        let ids: Vec<usize> = self.rigid_bodies.keys().cloned().collect();
        
        for i in 0..ids.len() {
            for j in (i + 1)..ids.len() {
                let id1 = ids[i];
                let id2 = ids[j];
                
                if let (Some(body1), Some(body2)) = (self.rigid_bodies.get(&id1), self.rigid_bodies.get(&id2)) {
                    if self.check_collision(body1, body2) {
                        self.collision_pairs.push((id1, id2));
                    }
                }
            }
        }
    }

    fn check_collision(&self, body1: &RigidBody, body2: &RigidBody) -> bool {
        // Skip collision if both bodies are static
        if body1.body_type == RigidBodyType::Static && body2.body_type == RigidBodyType::Static {
            return false;
        }
        
        // Skip collision if either body is a trigger and we're not checking triggers
        if body1.is_trigger || body2.is_trigger {
            return self.check_trigger_collision(body1, body2);
        }
        
        // AABB collision detection
        let bounds1 = body1.get_bounds();
        let bounds2 = body2.get_bounds();
        
        bounds1.intersects(&bounds2)
    }

    fn check_trigger_collision(&self, body1: &RigidBody, body2: &RigidBody) -> bool {
        let bounds1 = body1.get_bounds();
        let bounds2 = body2.get_bounds();
        bounds1.intersects(&bounds2)
    }

    fn resolve_collisions(&mut self) {
        for &(id1, id2) in &self.collision_pairs.clone() {
            if let (Some(body1), Some(body2)) = (
                self.rigid_bodies.get(&id1).cloned(),
                self.rigid_bodies.get(&id2).cloned()
            ) {
                if body1.is_trigger || body2.is_trigger {
                    // Handle trigger collision
                    self.handle_trigger_collision(id1, id2, &body1, &body2);
                } else {
                    // Handle physical collision
                    self.resolve_physical_collision(id1, id2, &body1, &body2);
                }
            }
        }
    }

    fn handle_trigger_collision(&mut self, id1: usize, id2: usize, body1: &RigidBody, body2: &RigidBody) {
        // Trigger collisions don't affect physics, just notify
        // This would typically call callbacks or set flags
        println!("Trigger collision between {} and {}", id1, id2);
    }

    fn resolve_physical_collision(&mut self, id1: usize, id2: usize, body1: &RigidBody, body2: &RigidBody) {
        let collision_normal = self.calculate_collision_normal(body1, body2);
        let relative_velocity = body1.velocity - body2.velocity;
        let velocity_along_normal = relative_velocity.dot(&collision_normal);
        
        // Don't resolve if velocities are separating
        if velocity_along_normal > 0.0 {
            return;
        }
        
        // Calculate restitution
        let restitution = (body1.restitution + body2.restitution) * 0.5;
        
        // Calculate impulse scalar
        let impulse_scalar = -(1.0 + restitution) * velocity_along_normal;
        let impulse_scalar = impulse_scalar / (1.0 / body1.mass + 1.0 / body2.mass);
        
        // Apply impulse
        let impulse = collision_normal * impulse_scalar;
        
        if let Some(body1_mut) = self.rigid_bodies.get_mut(&id1) {
            if body1_mut.body_type == RigidBodyType::Dynamic {
                body1_mut.velocity = body1_mut.velocity + impulse * (1.0 / body1_mut.mass);
            }
        }
        
        if let Some(body2_mut) = self.rigid_bodies.get_mut(&id2) {
            if body2_mut.body_type == RigidBodyType::Dynamic {
                body2_mut.velocity = body2_mut.velocity - impulse * (1.0 / body2_mut.mass);
            }
        }
        
        // Separate overlapping bodies
        self.separate_bodies(id1, id2, &collision_normal);
    }

    fn calculate_collision_normal(&self, body1: &RigidBody, body2: &RigidBody) -> Vector3D {
        // Simple collision normal calculation (center to center)
        let direction = body2.position - body1.position;
        if direction.magnitude() > 0.0 {
            direction.normalize()
        } else {
            Vector3D::new(0.0, 1.0, 0.0) // Default up direction
        }
    }

    fn separate_bodies(&mut self, id1: usize, id2: usize, normal: &Vector3D) {
        let separation_amount = 0.01; // Small separation to prevent overlap
        
        if let Some(body1) = self.rigid_bodies.get_mut(&id1) {
            if body1.body_type == RigidBodyType::Dynamic {
                body1.position = body1.position - *normal * separation_amount;
            }
        }
        
        if let Some(body2) = self.rigid_bodies.get_mut(&id2) {
            if body2.body_type == RigidBodyType::Dynamic {
                body2.position = body2.position + *normal * separation_amount;
            }
        }
    }

    // Raycast functionality
    pub fn raycast(&self, origin: Vector3D, direction: Vector3D, max_distance: f32) -> Option<RaycastHit> {
        let mut closest_hit: Option<RaycastHit> = None;
        let mut closest_distance = max_distance;
        
        for (&id, body) in &self.rigid_bodies {
            if let Some(hit) = self.raycast_body(origin, direction, max_distance, id, body) {
                if hit.distance < closest_distance {
                    closest_distance = hit.distance;
                    closest_hit = Some(hit);
                }
            }
        }
        
        closest_hit
    }

    fn raycast_body(&self, origin: Vector3D, direction: Vector3D, max_distance: f32, id: usize, body: &RigidBody) -> Option<RaycastHit> {
        let bounds = body.get_bounds();
        
        // Simple AABB raycast
        let inv_dir = Vector3D::new(
            if direction.x != 0.0 { 1.0 / direction.x } else { f32::INFINITY },
            if direction.y != 0.0 { 1.0 / direction.y } else { f32::INFINITY },
            if direction.z != 0.0 { 1.0 / direction.z } else { f32::INFINITY },
        );
        
        let t1 = Vector3D::new(
            (bounds.min.x - origin.x) * inv_dir.x,
            (bounds.min.y - origin.y) * inv_dir.y,
            (bounds.min.z - origin.z) * inv_dir.z,
        );
        let t2 = Vector3D::new(
            (bounds.max.x - origin.x) * inv_dir.x,
            (bounds.max.y - origin.y) * inv_dir.y,
            (bounds.max.z - origin.z) * inv_dir.z,
        );
        
        let t_min = Vector3D::new(
            t1.x.min(t2.x),
            t1.y.min(t2.y),
            t1.z.min(t2.z),
        );
        let t_max = Vector3D::new(
            t1.x.max(t2.x),
            t1.y.max(t2.y),
            t1.z.max(t2.z),
        );
        
        let t_near = t_min.x.max(t_min.y).max(t_min.z);
        let t_far = t_max.x.min(t_max.y).min(t_max.z);
        
        if t_near <= t_far && t_near >= 0.0 && t_near <= max_distance {
            let hit_point = origin + direction * t_near;
            let normal = self.calculate_surface_normal(&bounds, hit_point);
            
            Some(RaycastHit {
                point: hit_point,
                normal,
                distance: t_near,
                body_id: id,
            })
        } else {
            None
        }
    }

    fn calculate_surface_normal(&self, bounds: &BoundingBox, point: Vector3D) -> Vector3D {
        let center = bounds.center();
        let size = bounds.size();
        let local_point = point - center;
        
        // Find which face the point is closest to
        let abs_x = (local_point.x / (size.x * 0.5)).abs();
        let abs_y = (local_point.y / (size.y * 0.5)).abs();
        let abs_z = (local_point.z / (size.z * 0.5)).abs();
        
        if abs_x > abs_y && abs_x > abs_z {
            Vector3D::new(if local_point.x > 0.0 { 1.0 } else { -1.0 }, 0.0, 0.0)
        } else if abs_y > abs_z {
            Vector3D::new(0.0, if local_point.y > 0.0 { 1.0 } else { -1.0 }, 0.0)
        } else {
            Vector3D::new(0.0, 0.0, if local_point.z > 0.0 { 1.0 } else { -1.0 })
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum RigidBodyType {
    Static,    // Never moves
    Kinematic, // Moves but not affected by forces
    Dynamic,   // Affected by forces and collisions
}

#[derive(Debug, Clone)]
pub struct RigidBody {
    pub position: Vector3D,
    pub velocity: Vector3D,
    pub force: Vector3D,
    pub mass: f32,
    pub restitution: f32, // Bounciness (0 = no bounce, 1 = perfect bounce)
    pub size: Vector3D,
    pub body_type: RigidBodyType,
    pub use_gravity: bool,
    pub is_trigger: bool,
    pub grounded: bool,
    pub max_velocity: f32, // 0 = no limit
}

impl RigidBody {
    pub fn new(position: Vector3D, size: Vector3D) -> Self {
        Self {
            position,
            velocity: Vector3D::zero(),
            force: Vector3D::zero(),
            mass: 1.0,
            restitution: 0.3,
            size,
            body_type: RigidBodyType::Dynamic,
            use_gravity: true,
            is_trigger: false,
            grounded: false,
            max_velocity: 0.0,
        }
    }

    pub fn new_static(position: Vector3D, size: Vector3D) -> Self {
        let mut body = Self::new(position, size);
        body.body_type = RigidBodyType::Static;
        body.use_gravity = false;
        body.mass = f32::INFINITY;
        body
    }

    pub fn new_kinematic(position: Vector3D, size: Vector3D) -> Self {
        let mut body = Self::new(position, size);
        body.body_type = RigidBodyType::Kinematic;
        body.use_gravity = false;
        body
    }

    pub fn new_trigger(position: Vector3D, size: Vector3D) -> Self {
        let mut body = Self::new(position, size);
        body.is_trigger = true;
        body.use_gravity = false;
        body
    }

    pub fn add_force(&mut self, force: Vector3D) {
        if self.body_type == RigidBodyType::Dynamic {
            self.force = self.force + force;
        }
    }

    pub fn add_impulse(&mut self, impulse: Vector3D) {
        if self.body_type == RigidBodyType::Dynamic {
            self.velocity = self.velocity + impulse * (1.0 / self.mass);
        }
    }

    pub fn set_velocity(&mut self, velocity: Vector3D) {
        if self.body_type != RigidBodyType::Static {
            self.velocity = velocity;
        }
    }

    pub fn get_bounds(&self) -> BoundingBox {
        BoundingBox::from_center_size(self.position, self.size)
    }

    pub fn set_mass(&mut self, mass: f32) {
        if self.body_type == RigidBodyType::Dynamic {
            self.mass = mass.max(0.001); // Prevent zero or negative mass
        }
    }

    pub fn set_restitution(&mut self, restitution: f32) {
        self.restitution = restitution.clamp(0.0, 1.0);
    }
}

#[derive(Debug, Clone)]
pub struct RaycastHit {
    pub point: Vector3D,
    pub normal: Vector3D,
    pub distance: f32,
    pub body_id: usize,
}

// Physics material for different surface properties
#[derive(Debug, Clone)]
pub struct PhysicsMaterial {
    pub friction: f32,
    pub restitution: f32,
    pub density: f32,
}

impl PhysicsMaterial {
    pub fn new(friction: f32, restitution: f32, density: f32) -> Self {
        Self {
            friction: friction.clamp(0.0, 1.0),
            restitution: restitution.clamp(0.0, 1.0),
            density: density.max(0.001),
        }
    }

    pub fn default() -> Self {
        Self::new(0.5, 0.3, 1.0)
    }

    pub fn ice() -> Self {
        Self::new(0.1, 0.1, 0.9)
    }

    pub fn rubber() -> Self {
        Self::new(0.8, 0.9, 1.2)
    }

    pub fn metal() -> Self {
        Self::new(0.3, 0.2, 7.8)
    }

    pub fn wood() -> Self {
        Self::new(0.6, 0.4, 0.6)
    }
}

// Collision layers for filtering
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct CollisionLayer(pub u32);

impl CollisionLayer {
    pub const DEFAULT: CollisionLayer = CollisionLayer(1);
    pub const PLAYER: CollisionLayer = CollisionLayer(2);
    pub const ENEMY: CollisionLayer = CollisionLayer(4);
    pub const ENVIRONMENT: CollisionLayer = CollisionLayer(8);
    pub const PROJECTILE: CollisionLayer = CollisionLayer(16);
    pub const TRIGGER: CollisionLayer = CollisionLayer(32);

    pub fn intersects(&self, other: CollisionLayer) -> bool {
        (self.0 & other.0) != 0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rigid_body_creation() {
        let body = RigidBody::new(Vector3D::new(0.0, 0.0, 0.0), Vector3D::new(1.0, 1.0, 1.0));
        assert_eq!(body.position, Vector3D::new(0.0, 0.0, 0.0));
        assert_eq!(body.body_type, RigidBodyType::Dynamic);
        assert!(body.use_gravity);
    }

    #[test]
    fn test_physics_world() {
        let mut world = PhysicsWorld::new();
        let body = RigidBody::new(Vector3D::new(0.0, 10.0, 0.0), Vector3D::new(1.0, 1.0, 1.0));
        let id = world.add_rigid_body(body);
        
        // Simulate for a short time
        world.step(0.016); // ~60 FPS
        
        let updated_body = world.get_rigid_body(id).unwrap();
        assert!(updated_body.position.y < 10.0); // Should have fallen due to gravity
    }

    #[test]
    fn test_collision_detection() {
        let mut world = PhysicsWorld::new();
        
        let body1 = RigidBody::new(Vector3D::new(0.0, 0.0, 0.0), Vector3D::new(1.0, 1.0, 1.0));
        let body2 = RigidBody::new(Vector3D::new(0.5, 0.0, 0.0), Vector3D::new(1.0, 1.0, 1.0));
        
        let id1 = world.add_rigid_body(body1);
        let id2 = world.add_rigid_body(body2);
        
        world.detect_collisions();
        assert!(!world.collision_pairs.is_empty());
    }
}