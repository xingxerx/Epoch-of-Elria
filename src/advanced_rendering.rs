// Advanced Rendering System - Enhanced graphics features
use crate::math::Vector3D;
use kiss3d::nalgebra::{Point3, Vector3, Matrix4, Perspective3, Isometry3, Translation3, UnitQuaternion};
use kiss3d::scene::SceneNode;
use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::camera::{Camera, ArcBall, FirstPerson};
use std::collections::HashMap;

// Enhanced material system
#[derive(Debug, Clone)]
pub struct Material {
    pub name: String,
    pub albedo: (f32, f32, f32, f32),      // RGBA
    pub metallic: f32,
    pub roughness: f32,
    pub emission: (f32, f32, f32),         // RGB emission
    pub normal_strength: f32,
    pub ambient_occlusion: f32,
    pub texture_paths: MaterialTextures,
    pub shader_type: ShaderType,
    pub double_sided: bool,
    pub transparent: bool,
    pub alpha_cutoff: f32,
}

#[derive(Debug, Clone)]
pub struct MaterialTextures {
    pub albedo: Option<String>,
    pub normal: Option<String>,
    pub metallic: Option<String>,
    pub roughness: Option<String>,
    pub emission: Option<String>,
    pub ambient_occlusion: Option<String>,
    pub height: Option<String>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ShaderType {
    Standard,
    Unlit,
    Toon,
    Water,
    Glass,
    Hologram,
    Fire,
    Custom(String),
}

// Lighting system
#[derive(Debug, Clone)]
pub struct DirectionalLight {
    pub direction: Vector3D,
    pub color: (f32, f32, f32),
    pub intensity: f32,
    pub cast_shadows: bool,
    pub shadow_distance: f32,
    pub shadow_resolution: u32,
}

#[derive(Debug, Clone)]
pub struct PointLight {
    pub position: Vector3D,
    pub color: (f32, f32, f32),
    pub intensity: f32,
    pub range: f32,
    pub attenuation: (f32, f32, f32), // constant, linear, quadratic
    pub cast_shadows: bool,
}

#[derive(Debug, Clone)]
pub struct SpotLight {
    pub position: Vector3D,
    pub direction: Vector3D,
    pub color: (f32, f32, f32),
    pub intensity: f32,
    pub range: f32,
    pub inner_cone_angle: f32,
    pub outer_cone_angle: f32,
    pub cast_shadows: bool,
}

#[derive(Debug, Clone)]
pub struct AmbientLight {
    pub color: (f32, f32, f32),
    pub intensity: f32,
    pub sky_color: (f32, f32, f32),
    pub ground_color: (f32, f32, f32),
}

// Post-processing effects
#[derive(Debug, Clone)]
pub struct PostProcessingStack {
    pub effects: Vec<PostProcessingEffect>,
    pub enabled: bool,
}

#[derive(Debug, Clone)]
pub struct PostProcessingEffect {
    pub effect_type: PostProcessingType,
    pub enabled: bool,
    pub intensity: f32,
    pub parameters: HashMap<String, f32>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum PostProcessingType {
    Bloom,
    ToneMapping,
    ColorGrading,
    FXAA,
    SSAO,
    MotionBlur,
    DepthOfField,
    Vignette,
    ChromaticAberration,
    FilmGrain,
    Sharpen,
    Blur,
}

// Particle system
#[derive(Debug, Clone)]
pub struct ParticleSystem {
    pub name: String,
    pub particles: Vec<Particle>,
    pub emitter: ParticleEmitter,
    pub material: Material,
    pub max_particles: usize,
    pub active: bool,
    pub looping: bool,
    pub duration: f32,
    pub elapsed_time: f32,
}

#[derive(Debug, Clone)]
pub struct Particle {
    pub position: Vector3D,
    pub velocity: Vector3D,
    pub acceleration: Vector3D,
    pub color: (f32, f32, f32, f32),
    pub size: f32,
    pub rotation: f32,
    pub angular_velocity: f32,
    pub lifetime: f32,
    pub age: f32,
    pub active: bool,
}

#[derive(Debug, Clone)]
pub struct ParticleEmitter {
    pub position: Vector3D,
    pub emission_rate: f32,
    pub emission_shape: EmissionShape,
    pub velocity_range: (Vector3D, Vector3D),
    pub size_range: (f32, f32),
    pub lifetime_range: (f32, f32),
    pub color_over_lifetime: Vec<(f32, (f32, f32, f32, f32))>, // (time, color)
    pub size_over_lifetime: Vec<(f32, f32)>, // (time, size)
}

#[derive(Debug, Clone)]
pub enum EmissionShape {
    Point,
    Sphere { radius: f32 },
    Box { size: Vector3D },
    Cone { radius: f32, angle: f32 },
    Circle { radius: f32 },
}

// Skybox system
#[derive(Debug, Clone)]
pub struct Skybox {
    pub skybox_type: SkyboxType,
    pub tint: (f32, f32, f32),
    pub exposure: f32,
    pub rotation: f32,
}

#[derive(Debug, Clone)]
pub enum SkyboxType {
    Cubemap { paths: [String; 6] }, // +X, -X, +Y, -Y, +Z, -Z
    Procedural { 
        sun_direction: Vector3D,
        sun_color: (f32, f32, f32),
        sky_color: (f32, f32, f32),
        horizon_color: (f32, f32, f32),
        ground_color: (f32, f32, f32),
    },
    Gradient {
        top_color: (f32, f32, f32),
        bottom_color: (f32, f32, f32),
    },
}

// Terrain system
#[derive(Debug, Clone)]
pub struct Terrain {
    pub heightmap: Vec<Vec<f32>>,
    pub size: (usize, usize),
    pub scale: Vector3D,
    pub materials: Vec<TerrainLayer>,
    pub detail_distance: f32,
    pub lod_levels: Vec<TerrainLOD>,
}

#[derive(Debug, Clone)]
pub struct TerrainLayer {
    pub material: Material,
    pub tiling: (f32, f32),
    pub blend_map: Option<String>,
    pub height_range: (f32, f32),
    pub slope_range: (f32, f32),
}

#[derive(Debug, Clone)]
pub struct TerrainLOD {
    pub distance: f32,
    pub resolution: usize,
    pub mesh_data: Option<Vec<Vector3D>>, // Cached mesh vertices
}

// Water system
#[derive(Debug, Clone)]
pub struct Water {
    pub position: Vector3D,
    pub size: (f32, f32),
    pub wave_height: f32,
    pub wave_speed: f32,
    pub wave_frequency: f32,
    pub foam_threshold: f32,
    pub transparency: f32,
    pub refraction_strength: f32,
    pub reflection_strength: f32,
    pub color: (f32, f32, f32, f32),
    pub normal_maps: Vec<String>,
    pub flow_map: Option<String>,
}

// Advanced camera system
#[derive(Debug, Clone)]
pub struct CameraController {
    pub camera_type: CameraType,
    pub position: Vector3D,
    pub target: Vector3D,
    pub up: Vector3D,
    pub fov: f32,
    pub near_plane: f32,
    pub far_plane: f32,
    pub movement_speed: f32,
    pub rotation_speed: f32,
    pub zoom_speed: f32,
    pub smooth_follow: bool,
    pub follow_target: Option<usize>, // Object ID to follow
    pub follow_offset: Vector3D,
    pub follow_smoothness: f32,
    pub shake_intensity: f32,
    pub shake_duration: f32,
    pub shake_elapsed: f32,
}

#[derive(Debug, Clone, PartialEq)]
pub enum CameraType {
    FirstPerson,
    ThirdPerson,
    Orbital,
    Fixed,
    Cinematic,
}

// Render queue and batching
#[derive(Debug)]
pub struct RenderQueue {
    pub opaque_objects: Vec<RenderCommand>,
    pub transparent_objects: Vec<RenderCommand>,
    pub ui_objects: Vec<RenderCommand>,
    pub particle_systems: Vec<usize>,
    pub lights: LightData,
}

#[derive(Debug)]
pub struct RenderCommand {
    pub object_id: usize,
    pub material_id: usize,
    pub mesh_id: usize,
    pub transform: Matrix4<f32>,
    pub distance_to_camera: f32,
    pub layer: RenderLayer,
}

#[derive(Debug, Clone, PartialEq)]
pub enum RenderLayer {
    Background = 0,
    Opaque = 1,
    Transparent = 2,
    UI = 3,
    Debug = 4,
}

#[derive(Debug)]
pub struct LightData {
    pub directional_lights: Vec<DirectionalLight>,
    pub point_lights: Vec<PointLight>,
    pub spot_lights: Vec<SpotLight>,
    pub ambient_light: AmbientLight,
}

// Main Advanced Renderer
pub struct AdvancedRenderer {
    pub window: Window,
    pub materials: HashMap<usize, Material>,
    pub particle_systems: HashMap<usize, ParticleSystem>,
    pub camera_controller: CameraController,
    pub skybox: Option<Skybox>,
    pub terrain: Option<Terrain>,
    pub water_bodies: Vec<Water>,
    pub post_processing: PostProcessingStack,
    pub render_queue: RenderQueue,
    pub render_settings: RenderSettings,
    pub performance_stats: RenderStats,
    next_material_id: usize,
    next_particle_id: usize,
}

#[derive(Debug, Clone)]
pub struct RenderSettings {
    pub shadow_quality: ShadowQuality,
    pub texture_quality: TextureQuality,
    pub anti_aliasing: AntiAliasing,
    pub anisotropic_filtering: u32,
    pub vsync: bool,
    pub target_fps: Option<u32>,
    pub render_scale: f32,
    pub lod_bias: f32,
    pub max_lights: usize,
    pub enable_hdr: bool,
    pub enable_bloom: bool,
    pub enable_ssao: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ShadowQuality {
    Off,
    Low,
    Medium,
    High,
    Ultra,
}

#[derive(Debug, Clone, PartialEq)]
pub enum TextureQuality {
    Low,
    Medium,
    High,
    Ultra,
}

#[derive(Debug, Clone, PartialEq)]
pub enum AntiAliasing {
    None,
    FXAA,
    MSAA2x,
    MSAA4x,
    MSAA8x,
    TAA,
}

#[derive(Debug, Clone)]
pub struct RenderStats {
    pub frame_time: f32,
    pub fps: f32,
    pub draw_calls: u32,
    pub triangles_rendered: u32,
    pub vertices_rendered: u32,
    pub texture_memory_used: u64,
    pub geometry_memory_used: u64,
    pub lights_rendered: u32,
    pub particles_rendered: u32,
}

impl AdvancedRenderer {
    pub fn new(window: Window) -> Self {
        let camera_controller = CameraController {
            camera_type: CameraType::ThirdPerson,
            position: Vector3D::new(0.0, 5.0, 10.0),
            target: Vector3D::zero(),
            up: Vector3D::new(0.0, 1.0, 0.0),
            fov: 60.0,
            near_plane: 0.1,
            far_plane: 1000.0,
            movement_speed: 10.0,
            rotation_speed: 2.0,
            zoom_speed: 5.0,
            smooth_follow: true,
            follow_target: None,
            follow_offset: Vector3D::new(0.0, 5.0, 10.0),
            follow_smoothness: 5.0,
            shake_intensity: 0.0,
            shake_duration: 0.0,
            shake_elapsed: 0.0,
        };

        let post_processing = PostProcessingStack {
            effects: vec![
                PostProcessingEffect {
                    effect_type: PostProcessingType::ToneMapping,
                    enabled: true,
                    intensity: 1.0,
                    parameters: HashMap::new(),
                },
                PostProcessingEffect {
                    effect_type: PostProcessingType::FXAA,
                    enabled: true,
                    intensity: 1.0,
                    parameters: HashMap::new(),
                },
            ],
            enabled: true,
        };

        let render_settings = RenderSettings {
            shadow_quality: ShadowQuality::High,
            texture_quality: TextureQuality::High,
            anti_aliasing: AntiAliasing::FXAA,
            anisotropic_filtering: 16,
            vsync: true,
            target_fps: Some(60),
            render_scale: 1.0,
            lod_bias: 0.0,
            max_lights: 32,
            enable_hdr: true,
            enable_bloom: true,
            enable_ssao: true,
        };

        Self {
            window,
            materials: HashMap::new(),
            particle_systems: HashMap::new(),
            camera_controller,
            skybox: None,
            terrain: None,
            water_bodies: Vec::new(),
            post_processing,
            render_queue: RenderQueue {
                opaque_objects: Vec::new(),
                transparent_objects: Vec::new(),
                ui_objects: Vec::new(),
                particle_systems: Vec::new(),
                lights: LightData {
                    directional_lights: Vec::new(),
                    point_lights: Vec::new(),
                    spot_lights: Vec::new(),
                    ambient_light: AmbientLight {
                        color: (0.2, 0.2, 0.3),
                        intensity: 0.3,
                        sky_color: (0.5, 0.7, 1.0),
                        ground_color: (0.3, 0.3, 0.2),
                    },
                },
            },
            render_settings,
            performance_stats: RenderStats {
                frame_time: 0.0,
                fps: 0.0,
                draw_calls: 0,
                triangles_rendered: 0,
                vertices_rendered: 0,
                texture_memory_used: 0,
                geometry_memory_used: 0,
                lights_rendered: 0,
                particles_rendered: 0,
            },
            next_material_id: 0,
            next_particle_id: 0,
        }
    }

    // Material management
    pub fn create_material(&mut self, material: Material) -> usize {
        let id = self.next_material_id;
        self.next_material_id += 1;
        self.materials.insert(id, material);
        id
    }

    pub fn create_standard_material(&mut self, name: &str, albedo: (f32, f32, f32, f32)) -> usize {
        let material = Material {
            name: name.to_string(),
            albedo,
            metallic: 0.0,
            roughness: 0.5,
            emission: (0.0, 0.0, 0.0),
            normal_strength: 1.0,
            ambient_occlusion: 1.0,
            texture_paths: MaterialTextures {
                albedo: None,
                normal: None,
                metallic: None,
                roughness: None,
                emission: None,
                ambient_occlusion: None,
                height: None,
            },
            shader_type: ShaderType::Standard,
            double_sided: false,
            transparent: albedo.3 < 1.0,
            alpha_cutoff: 0.5,
        };
        self.create_material(material)
    }

    // Particle system management
    pub fn create_particle_system(&mut self, name: &str, max_particles: usize) -> usize {
        let id = self.next_particle_id;
        self.next_particle_id += 1;

        let particle_system = ParticleSystem {
            name: name.to_string(),
            particles: Vec::with_capacity(max_particles),
            emitter: ParticleEmitter {
                position: Vector3D::zero(),
                emission_rate: 10.0,
                emission_shape: EmissionShape::Point,
                velocity_range: (Vector3D::new(-1.0, 0.0, -1.0), Vector3D::new(1.0, 5.0, 1.0)),
                size_range: (0.1, 0.5),
                lifetime_range: (1.0, 3.0),
                color_over_lifetime: vec![
                    (0.0, (1.0, 1.0, 1.0, 1.0)),
                    (1.0, (1.0, 1.0, 1.0, 0.0)),
                ],
                size_over_lifetime: vec![
                    (0.0, 1.0),
                    (1.0, 0.0),
                ],
            },
            material: Material {
                name: "ParticleMaterial".to_string(),
                albedo: (1.0, 1.0, 1.0, 0.5),
                metallic: 0.0,
                roughness: 1.0,
                emission: (0.0, 0.0, 0.0),
                normal_strength: 0.0,
                ambient_occlusion: 1.0,
                texture_paths: MaterialTextures {
                    albedo: None,
                    normal: None,
                    metallic: None,
                    roughness: None,
                    emission: None,
                    ambient_occlusion: None,
                    height: None,
                },
                shader_type: ShaderType::Unlit,
                double_sided: true,
                transparent: true,
                alpha_cutoff: 0.1,
            },
            max_particles,
            active: true,
            looping: true,
            duration: 5.0,
            elapsed_time: 0.0,
        };

        self.particle_systems.insert(id, particle_system);
        id
    }

    // Lighting
    pub fn add_directional_light(&mut self, light: DirectionalLight) {
        self.render_queue.lights.directional_lights.push(light);
    }

    pub fn add_point_light(&mut self, light: PointLight) {
        self.render_queue.lights.point_lights.push(light);
    }

    pub fn add_spot_light(&mut self, light: SpotLight) {
        self.render_queue.lights.spot_lights.push(light);
    }

    pub fn set_ambient_light(&mut self, light: AmbientLight) {
        self.render_queue.lights.ambient_light = light;
    }

    // Camera control
    pub fn update_camera(&mut self, delta_time: f32) {
        // Handle camera shake
        if self.camera_controller.shake_duration > 0.0 {
            self.camera_controller.shake_elapsed += delta_time;
            if self.camera_controller.shake_elapsed >= self.camera_controller.shake_duration {
                self.camera_controller.shake_intensity = 0.0;
                self.camera_controller.shake_duration = 0.0;
                self.camera_controller.shake_elapsed = 0.0;
            } else {
                // Apply shake offset
                use rand::Rng;
                let mut rng = rand::thread_rng();
                let shake_x = rng.gen_range(-1.0..1.0) * self.camera_controller.shake_intensity;
                let shake_y = rng.gen_range(-1.0..1.0) * self.camera_controller.shake_intensity;
                let shake_z = rng.gen_range(-1.0..1.0) * self.camera_controller.shake_intensity;
                
                self.camera_controller.position = self.camera_controller.position + 
                    Vector3D::new(shake_x, shake_y, shake_z);
            }
        }

        // Handle smooth follow
        if let Some(_target_id) = self.camera_controller.follow_target {
            if self.camera_controller.smooth_follow {
                // Smooth camera following would be implemented here
                // This would require access to the target object's position
            }
        }
    }

    pub fn shake_camera(&mut self, intensity: f32, duration: f32) {
        self.camera_controller.shake_intensity = intensity;
        self.camera_controller.shake_duration = duration;
        self.camera_controller.shake_elapsed = 0.0;
    }

    // Skybox
    pub fn set_skybox(&mut self, skybox: Skybox) {
        self.skybox = Some(skybox);
    }

    pub fn create_procedural_skybox(&mut self, sun_direction: Vector3D, sun_color: (f32, f32, f32)) {
        let skybox = Skybox {
            skybox_type: SkyboxType::Procedural {
                sun_direction,
                sun_color,
                sky_color: (0.5, 0.7, 1.0),
                horizon_color: (1.0, 0.9, 0.7),
                ground_color: (0.3, 0.3, 0.2),
            },
            tint: (1.0, 1.0, 1.0),
            exposure: 1.0,
            rotation: 0.0,
        };
        self.set_skybox(skybox);
    }

    // Terrain
    pub fn create_terrain(&mut self, heightmap: Vec<Vec<f32>>, scale: Vector3D) -> Result<(), String> {
        if heightmap.is_empty() || heightmap[0].is_empty() {
            return Err("Invalid heightmap data".to_string());
        }

        let size = (heightmap.len(), heightmap[0].len());
        let terrain = Terrain {
            heightmap,
            size,
            scale,
            materials: Vec::new(),
            detail_distance: 100.0,
            lod_levels: vec![
                TerrainLOD { distance: 50.0, resolution: 256, mesh_data: None },
                TerrainLOD { distance: 100.0, resolution: 128, mesh_data: None },
                TerrainLOD { distance: 200.0, resolution: 64, mesh_data: None },
            ],
        };

        self.terrain = Some(terrain);
        Ok(())
    }

    // Water
    pub fn add_water(&mut self, water: Water) {
        self.water_bodies.push(water);
    }

    // Update systems
    pub fn update(&mut self, delta_time: f32) {
        self.update_camera(delta_time);
        self.update_particle_systems(delta_time);
        self.update_water(delta_time);
    }

    fn update_water(&mut self, _delta_time: f32) {
        for _water in &mut self.water_bodies {
            // Update wave animation
            // This would typically involve updating shader uniforms for wave displacement
        }
    }

    fn update_particle_systems(&mut self, delta_time: f32) {
        for (_, particle_system) in &mut self.particle_systems {
            if !particle_system.active {
                continue;
            }

            particle_system.elapsed_time += delta_time;

            // Emit new particles
            let particles_to_emit = (particle_system.emitter.emission_rate * delta_time) as usize;
            let emitter = particle_system.emitter.clone();
            let mut new_particles = Vec::new();
            for _ in 0..particles_to_emit {
                if particle_system.particles.len() + new_particles.len() < particle_system.max_particles {
                    let particle = create_particle_standalone(&emitter);
                    new_particles.push(particle);
                }
            }
            particle_system.particles.extend(new_particles);

            // Update existing particles
            let color_curve = particle_system.emitter.color_over_lifetime.clone();
            let size_curve = particle_system.emitter.size_over_lifetime.clone();

            particle_system.particles.retain_mut(|particle| {
                particle.age += delta_time;
                if particle.age >= particle.lifetime {
                    return false;
                }

                // Update physics
                particle.velocity = particle.velocity + particle.acceleration * delta_time;
                particle.position = particle.position + particle.velocity * delta_time;
                particle.rotation += particle.angular_velocity * delta_time;

                // Update color and size over lifetime
                let life_progress = particle.age / particle.lifetime;
                particle.color = interpolate_color(&color_curve, life_progress);
                particle.size *= interpolate_size(&size_curve, life_progress);

                true
            });

            // Handle looping
            if !particle_system.looping && particle_system.elapsed_time >= particle_system.duration {
                particle_system.active = false;
            }
        }
    }

    fn create_particle(&self, emitter: &ParticleEmitter) -> Particle {
        use rand::Rng;
        let mut rng = rand::thread_rng();

        let position = match &emitter.emission_shape {
            EmissionShape::Point => emitter.position,
            EmissionShape::Sphere { radius } => {
                let theta = rng.gen_range(0.0..std::f32::consts::TAU);
                let phi = rng.gen_range(0.0..std::f32::consts::PI);
                let r = rng.gen_range(0.0..*radius);
                
                emitter.position + Vector3D::new(
                    r * phi.sin() * theta.cos(),
                    r * phi.cos(),
                    r * phi.sin() * theta.sin(),
                )
            }
            EmissionShape::Box { size } => {
                emitter.position + Vector3D::new(
                    rng.gen_range(-size.x / 2.0..size.x / 2.0),
                    rng.gen_range(-size.y / 2.0..size.y / 2.0),
                    rng.gen_range(-size.z / 2.0..size.z / 2.0),
                )
            }
            EmissionShape::Circle { radius } => {
                let angle = rng.gen_range(0.0..std::f32::consts::TAU);
                let r = rng.gen_range(0.0..*radius);
                
                emitter.position + Vector3D::new(
                    r * angle.cos(),
                    0.0,
                    r * angle.sin(),
                )
            }
            EmissionShape::Cone { radius, angle } => {
                let cone_angle = rng.gen_range(0.0..*angle);
                let rotation = rng.gen_range(0.0..std::f32::consts::TAU);
                let distance = rng.gen_range(0.0..*radius);
                
                emitter.position + Vector3D::new(
                    distance * cone_angle.sin() * rotation.cos(),
                    distance * cone_angle.cos(),
                    distance * cone_angle.sin() * rotation.sin(),
                )
            }
        };

        let velocity = Vector3D::new(
            rng.gen_range(emitter.velocity_range.0.x..emitter.velocity_range.1.x),
            rng.gen_range(emitter.velocity_range.0.y..emitter.velocity_range.1.y),
            rng.gen_range(emitter.velocity_range.0.z..emitter.velocity_range.1.z),
        );

        Particle {
            position,
            velocity,
            acceleration: Vector3D::new(0.0, -9.81, 0.0), // Gravity
            color: emitter.color_over_lifetime.first().map(|(_, color)| *color).unwrap_or((1.0, 1.0, 1.0, 1.0)),
            size: rng.gen_range(emitter.size_range.0..emitter.size_range.1),
            rotation: 0.0,
            angular_velocity: rng.gen_range(-5.0..5.0),
            lifetime: rng.gen_range(emitter.lifetime_range.0..emitter.lifetime_range.1),
            age: 0.0,
            active: true,
        }
    }

    fn interpolate_color(&self, color_curve: &[(f32, (f32, f32, f32, f32))], t: f32) -> (f32, f32, f32, f32) {
        interpolate_color(color_curve, t)
    }

    fn interpolate_size(&self, size_curve: &[(f32, f32)], t: f32) -> f32 {
        interpolate_size(size_curve, t)
    }
}

// Standalone particle creation function
fn create_particle_standalone(emitter: &ParticleEmitter) -> Particle {
    use rand::Rng;
    let mut rng = rand::thread_rng();

    let velocity = Vector3D::new(
        rng.gen_range(emitter.velocity_range.0.x..=emitter.velocity_range.1.x),
        rng.gen_range(emitter.velocity_range.0.y..=emitter.velocity_range.1.y),
        rng.gen_range(emitter.velocity_range.0.z..=emitter.velocity_range.1.z),
    );

    let size = rng.gen_range(emitter.size_range.0..=emitter.size_range.1);
    let lifetime = rng.gen_range(emitter.lifetime_range.0..=emitter.lifetime_range.1);

    Particle {
        position: emitter.position,
        velocity,
        acceleration: Vector3D::new(0.0, -9.81, 0.0), // Gravity
        size,
        color: (1.0, 1.0, 1.0, 1.0), // White
        lifetime,
        age: 0.0,
        rotation: 0.0,
        angular_velocity: rng.gen_range(-1.0..=1.0),
    }
}

// Standalone interpolation functions
fn interpolate_color(color_curve: &[(f32, (f32, f32, f32, f32))], t: f32) -> (f32, f32, f32, f32) {
        if color_curve.is_empty() {
            return (1.0, 1.0, 1.0, 1.0);
        }

        if color_curve.len() == 1 {
            return color_curve[0].1;
        }

        for i in 0..color_curve.len() - 1 {
            let (t1, color1) = color_curve[i];
            let (t2, color2) = color_curve[i + 1];

            if t >= t1 && t <= t2 {
                let factor = (t - t1) / (t2 - t1);
                return (
                    color1.0 + (color2.0 - color1.0) * factor,
                    color1.1 + (color2.1 - color1.1) * factor,
                    color1.2 + (color2.2 - color1.2) * factor,
                    color1.3 + (color2.3 - color1.3) * factor,
                );
            }
        }

        color_curve.last().unwrap().1
    }

fn interpolate_size(size_curve: &[(f32, f32)], t: f32) -> f32 {
        if size_curve.is_empty() {
            return 1.0;
        }

        if size_curve.len() == 1 {
            return size_curve[0].1;
        }

        for i in 0..size_curve.len() - 1 {
            let (t1, size1) = size_curve[i];
            let (t2, size2) = size_curve[i + 1];

            if t >= t1 && t <= t2 {
                let factor = (t - t1) / (t2 - t1);
                return size1 + (size2 - size1) * factor;
            }
        }

        size_curve.last().unwrap().1
    }

impl AdvancedRenderingSystem {
    // Render methods would be implemented here
    pub fn render(&mut self) {
        // Clear render queue
        self.render_queue.opaque_objects.clear();
        self.render_queue.transparent_objects.clear();
        self.render_queue.ui_objects.clear();
        self.render_queue.particle_systems.clear();

        // Populate render queue (this would be called by the scene)
        // Sort transparent objects back-to-front
        // Sort opaque objects front-to-back for early Z rejection

        // Render skybox
        if let Some(_skybox) = &self.skybox {
            // Render skybox
        }

        // Render terrain
        if let Some(_terrain) = &self.terrain {
            // Render terrain with LOD
        }

        // Render opaque objects
        for _command in &self.render_queue.opaque_objects {
            // Render object
        }

        // Render water
        for _water in &self.water_bodies {
            // Render water surface
        }

        // Render transparent objects
        for _command in &self.render_queue.transparent_objects {
            // Render transparent object
        }

        // Render particle systems
        for particle_id in &self.render_queue.particle_systems {
            if let Some(particle_system) = self.particle_systems.get(particle_id) {
                self.render_particle_system(particle_system);
            }
        }

        // Apply post-processing
        if self.post_processing.enabled {
            self.apply_post_processing();
        }

        // Render UI
        for _command in &self.render_queue.ui_objects {
            // Render UI element
        }
    }

    fn render_particle_system(&self, _particle_system: &ParticleSystem) {
        // Render particles as billboards or meshes
    }

    fn apply_post_processing(&self) {
        for effect in &self.post_processing.effects {
            if effect.enabled {
                match effect.effect_type {
                    PostProcessingType::Bloom => {
                        // Apply bloom effect
                    }
                    PostProcessingType::ToneMapping => {
                        // Apply tone mapping
                    }
                    PostProcessingType::FXAA => {
                        // Apply FXAA
                    }
                    _ => {
                        // Apply other effects
                    }
                }
            }
        }
    }

    // Getters
    pub fn get_render_stats(&self) -> &RenderStats {
        &self.performance_stats
    }

    pub fn get_camera_position(&self) -> Vector3D {
        self.camera_controller.position
    }

    pub fn get_camera_target(&self) -> Vector3D {
        self.camera_controller.target
    }
}

impl Default for Material {
    fn default() -> Self {
        Self {
            name: "Default".to_string(),
            albedo: (1.0, 1.0, 1.0, 1.0),
            metallic: 0.0,
            roughness: 0.5,
            emission: (0.0, 0.0, 0.0),
            normal_strength: 1.0,
            ambient_occlusion: 1.0,
            texture_paths: MaterialTextures {
                albedo: None,
                normal: None,
                metallic: None,
                roughness: None,
                emission: None,
                ambient_occlusion: None,
                height: None,
            },
            shader_type: ShaderType::Standard,
            double_sided: false,
            transparent: false,
            alpha_cutoff: 0.5,
        }
    }
}

impl Default for MaterialTextures {
    fn default() -> Self {
        Self {
            albedo: None,
            normal: None,
            metallic: None,
            roughness: None,
            emission: None,
            ambient_occlusion: None,
            height: None,
        }
    }
}