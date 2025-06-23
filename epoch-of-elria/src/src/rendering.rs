// rendering.rs - Rendering system for the game engine

use crate::math::Vector3D;
use crate::scene::Scene;
use crate::EngineConfig;
use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::camera::{ArcBall, FirstPerson, Camera};
use kiss3d::nalgebra::{Point3, Vector3};

pub struct RenderingSystem {
    window: Window,
    camera_type: CameraType,
    arcball_camera: Option<ArcBall>,
    fps_camera: Option<FirstPerson>,
    background_color: (f32, f32, f32),
    wireframe_mode: bool,
    show_debug_info: bool,
    frame_count: u64,
    fps: f32,
    frame_time_accumulator: f32,
    last_fps_update: std::time::Instant,
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum CameraType {
    ArcBall,
    FirstPerson,
}

impl RenderingSystem {
    pub fn new(config: &EngineConfig) -> Result<Self, Box<dyn std::error::Error>> {
        let mut window = Window::new_with_size(&config.window_title, config.window_width, config.window_height);
        
        // Set up lighting
        window.set_light(Light::StickToCamera);
        
        // Set up camera
        let camera_type = CameraType::ArcBall;
        let arcball_camera = Some(ArcBall::new(Point3::new(10.0, 10.0, 10.0), Point3::origin()));
        let fps_camera = None;
        
        // Set background color
        let background_color = (0.1, 0.1, 0.2);
        window.set_background_color(background_color.0, background_color.1, background_color.2);
        
        Ok(Self {
            window,
            camera_type,
            arcball_camera,
            fps_camera,
            background_color,
            wireframe_mode: false,
            show_debug_info: config.debug_mode,
            frame_count: 0,
            fps: 0.0,
            frame_time_accumulator: 0.0,
            last_fps_update: std::time::Instant::now(),
        })
    }

    pub fn should_continue(&self) -> bool {
        !self.window.should_close()
    }

    pub fn render(&mut self, scene: &Scene) -> Result<(), Box<dyn std::error::Error>> {
        // Update FPS counter
        self.update_fps();
        
        // Handle camera updates
        self.update_camera();
        
        // Render the scene
        if self.window.render() {
            // Scene rendering is handled by kiss3d automatically
            // Additional rendering logic can be added here
            
            if self.show_debug_info {
                self.render_debug_info();
            }
        }
        
        self.frame_count += 1;
        Ok(())
    }

    pub fn set_camera_type(&mut self, camera_type: CameraType) {
        self.camera_type = camera_type;
        match camera_type {
            CameraType::ArcBall => {
                if self.arcball_camera.is_none() {
                    self.arcball_camera = Some(ArcBall::new(Point3::new(10.0, 10.0, 10.0), Point3::origin()));
                }
            },
            CameraType::FirstPerson => {
                if self.fps_camera.is_none() {
                    self.fps_camera = Some(FirstPerson::new(Point3::new(10.0, 10.0, 10.0), Point3::origin()));
                }
            },
        }
    }

    pub fn set_background_color(&mut self, r: f32, g: f32, b: f32) {
        self.background_color = (r, g, b);
        self.window.set_background_color(r, g, b);
    }

    pub fn toggle_wireframe(&mut self) {
        self.wireframe_mode = !self.wireframe_mode;
        // Note: kiss3d doesn't have direct wireframe support
        // This would need to be implemented differently
    }

    pub fn toggle_debug_info(&mut self) {
        self.show_debug_info = !self.show_debug_info;
    }

    pub fn get_fps(&self) -> f32 {
        self.fps
    }

    pub fn get_frame_count(&self) -> u64 {
        self.frame_count
    }

    pub fn get_window_size(&self) -> (u32, u32) {
        let size = self.window.size();
        (size.x as u32, size.y as u32)
    }

    pub fn set_window_title(&mut self, title: &str) {
        // Note: kiss3d doesn't provide a direct way to change window title after creation
        // This would need to be implemented at the windowing system level
    }

    pub fn screenshot(&mut self, filename: &str) -> Result<(), Box<dyn std::error::Error>> {
        // Note: kiss3d doesn't provide direct screenshot functionality
        // This would need to be implemented using the underlying graphics API
        println!("Screenshot functionality not yet implemented: {}", filename);
        Ok(())
    }

    // Camera control methods
    pub fn move_camera(&mut self, direction: Vector3D, amount: f32) {
        match self.camera_type {
            CameraType::ArcBall => {
                // ArcBall camera movement is handled differently
                if let Some(ref mut camera) = self.arcball_camera {
                    // Implement arcball camera movement
                }
            },
            CameraType::FirstPerson => {
                if let Some(ref mut camera) = self.fps_camera {
                    let kiss3d_direction = Vector3::new(direction.x, direction.y, direction.z);
                    // Implement first person camera movement
                }
            },
        }
    }

    pub fn rotate_camera(&mut self, yaw: f32, pitch: f32) {
        match self.camera_type {
            CameraType::ArcBall => {
                // ArcBall rotation is handled by mouse input automatically
            },
            CameraType::FirstPerson => {
                if let Some(ref mut camera) = self.fps_camera {
                    // Implement first person camera rotation
                }
            },
        }
    }

    pub fn zoom_camera(&mut self, amount: f32) {
        match self.camera_type {
            CameraType::ArcBall => {
                if let Some(ref mut camera) = self.arcball_camera {
                    // Implement arcball zoom
                }
            },
            CameraType::FirstPerson => {
                // First person camera doesn't typically zoom
            },
        }
    }

    pub fn get_camera_position(&self) -> Vector3D {
        match self.camera_type {
            CameraType::ArcBall => {
                if let Some(ref camera) = self.arcball_camera {
                    let eye = camera.eye();
                    Vector3D::new(eye.x, eye.y, eye.z)
                } else {
                    Vector3D::zero()
                }
            },
            CameraType::FirstPerson => {
                if let Some(ref camera) = self.fps_camera {
                    let eye = camera.eye();
                    Vector3D::new(eye.x, eye.y, eye.z)
                } else {
                    Vector3D::zero()
                }
            },
        }
    }

    pub fn get_camera_target(&self) -> Vector3D {
        match self.camera_type {
            CameraType::ArcBall => {
                if let Some(ref camera) = self.arcball_camera {
                    let at = camera.at();
                    Vector3D::new(at.x, at.y, at.z)
                } else {
                    Vector3D::zero()
                }
            },
            CameraType::FirstPerson => {
                if let Some(ref camera) = self.fps_camera {
                    let at = camera.at();
                    Vector3D::new(at.x, at.y, at.z)
                } else {
                    Vector3D::zero()
                }
            },
        }
    }

    // Internal methods
    fn update_fps(&mut self) {
        let now = std::time::Instant::now();
        let delta = now.duration_since(self.last_fps_update).as_secs_f32();
        self.frame_time_accumulator += delta;
        
        if self.frame_time_accumulator >= 1.0 {
            self.fps = self.frame_count as f32 / self.frame_time_accumulator;
            self.frame_count = 0;
            self.frame_time_accumulator = 0.0;
        }
        
        self.last_fps_update = now;
    }

    fn update_camera(&mut self) {
        // Camera updates are handled by kiss3d automatically through event handling
    }

    fn render_debug_info(&mut self) {
        // Debug info rendering would be implemented here
        // This could include FPS, camera position, object counts, etc.
        if self.frame_count % 60 == 0 { // Update every second at 60 FPS
            println!("FPS: {:.1}, Camera: {:?}", self.fps, self.get_camera_position());
        }
    }

    // Event handling methods
    pub fn handle_window_events(&mut self) {
        // Handle window resize, close, etc.
        for event in self.window.events().iter() {
            match event.value {
                kiss3d::event::WindowEvent::FramebufferSize(width, height) => {
                    // Handle window resize
                    println!("Window resized to {}x{}", width, height);
                },
                kiss3d::event::WindowEvent::Key(key, action, _) => {
                    if action == kiss3d::event::Action::Press {
                        match key {
                            kiss3d::event::Key::F1 => self.toggle_debug_info(),
                            kiss3d::event::Key::F2 => self.toggle_wireframe(),
                            kiss3d::event::Key::F3 => {
                                let new_type = match self.camera_type {
                                    CameraType::ArcBall => CameraType::FirstPerson,
                                    CameraType::FirstPerson => CameraType::ArcBall,
                                };
                                self.set_camera_type(new_type);
                            },
                            _ => {},
                        }
                    }
                },
                _ => {},
            }
        }
    }

    // Utility methods for accessing the underlying window
    pub fn window(&self) -> &Window {
        &self.window
    }

    pub fn window_mut(&mut self) -> &mut Window {
        &mut self.window
    }
}

// Rendering utilities
pub struct RenderStats {
    pub triangles_rendered: u64,
    pub draw_calls: u64,
    pub vertices_processed: u64,
    pub textures_bound: u64,
}

impl RenderStats {
    pub fn new() -> Self {
        Self {
            triangles_rendered: 0,
            draw_calls: 0,
            vertices_processed: 0,
            textures_bound: 0,
        }
    }

    pub fn reset(&mut self) {
        self.triangles_rendered = 0;
        self.draw_calls = 0;
        self.vertices_processed = 0;
        self.textures_bound = 0;
    }
}

// Color utilities
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Color {
    pub r: f32,
    pub g: f32,
    pub b: f32,
    pub a: f32,
}

impl Color {
    pub fn new(r: f32, g: f32, b: f32, a: f32) -> Self {
        Self { r, g, b, a }
    }

    pub fn rgb(r: f32, g: f32, b: f32) -> Self {
        Self::new(r, g, b, 1.0)
    }

    pub fn white() -> Self {
        Self::rgb(1.0, 1.0, 1.0)
    }

    pub fn black() -> Self {
        Self::rgb(0.0, 0.0, 0.0)
    }

    pub fn red() -> Self {
        Self::rgb(1.0, 0.0, 0.0)
    }

    pub fn green() -> Self {
        Self::rgb(0.0, 1.0, 0.0)
    }

    pub fn blue() -> Self {
        Self::rgb(0.0, 0.0, 1.0)
    }

    pub fn yellow() -> Self {
        Self::rgb(1.0, 1.0, 0.0)
    }

    pub fn cyan() -> Self {
        Self::rgb(0.0, 1.0, 1.0)
    }

    pub fn magenta() -> Self {
        Self::rgb(1.0, 0.0, 1.0)
    }

    pub fn from_hex(hex: u32) -> Self {
        let r = ((hex >> 16) & 0xFF) as f32 / 255.0;
        let g = ((hex >> 8) & 0xFF) as f32 / 255.0;
        let b = (hex & 0xFF) as f32 / 255.0;
        Self::rgb(r, g, b)
    }

    pub fn to_hex(&self) -> u32 {
        let r = (self.r * 255.0) as u32;
        let g = (self.g * 255.0) as u32;
        let b = (self.b * 255.0) as u32;
        (r << 16) | (g << 8) | b
    }

    pub fn lerp(&self, other: &Color, t: f32) -> Self {
        Self::new(
            self.r + (other.r - self.r) * t,
            self.g + (other.g - self.g) * t,
            self.b + (other.b - self.b) * t,
            self.a + (other.a - self.a) * t,
        )
    }
}