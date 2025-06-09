[dependencies]
minifb = "0.25"use minifb::{Key, Window, WindowOptions};
use std::time::Duration;
use std::thread::sleep;

const WIDTH: usize = 800;
const HEIGHT: usize = 600;

#[derive(Clone, Copy)]
struct Vector2D {
    x: f32,
    y: f32,
}

impl Vector2D {
    fn new(x: f32, y: f32) -> Self {
        Vector2D { x, y }
    }
}

struct Player {
    position: Vector2D,
    velocity: Vector2D,
    width: f32,
    height: f32,
    color: u32,
}

impl Player {
    fn new(position: Vector2D, velocity: Vector2D, color: u32) -> Self {
        Player {
            position,
            velocity,
            width: 40.0,
            height: 40.0,
            color,
        }
    }

    fn update(&mut self) {
        self.position.x += self.velocity.x;
        self.position.y += self.velocity.y;
    }

    fn draw(&self, buffer: &mut [u32]) {
        let x0 = self.position.x as usize;
        let y0 = self.position.y as usize;
        let x1 = (self.position.x + self.width) as usize;
        let y1 = (self.position.y + self.height) as usize;

        for y in y0..y1 {
            if y >= HEIGHT { continue; }
            for x in x0..x1 {
                if x >= WIDTH { continue; }
                buffer[y * WIDTH + x] = self.color;
            }
        }
    }
}

fn main() {
    let mut window = Window::new(
        "Epoch of Elria Game Engine (Rust) - Windowed",
        WIDTH,
        HEIGHT,
        WindowOptions::default(),
    ).unwrap_or_else(|e| {
        panic!("{}", e);
    });

    let mut buffer: Vec<u32> = vec![0; WIDTH * HEIGHT];

    // Create three players with different starting positions and velocities
    let mut players = vec![
        Player::new(Vector2D::new(200.0, 0.0), Vector2D::new(2.0, 0.0), 0x0000FF), // Blue
        Player::new(Vector2D::new(0.0, 0.0), Vector2D::new(2.0, 0.0), 0x0000FF),   // Blue
        Player::new(Vector2D::new(100.0, 0.0), Vector2D::new(-2.0, 0.0), 0x0000FF),// Blue
    ];

    let mut frame = 0;
    while window.is_open() && !window.is_key_down(Key::Escape) && frame < 120 {
        // Clear buffer
        for pixel in buffer.iter_mut() {
            *pixel = 0xFFFFFF; // White background
        }

        // Update and draw players
        for player in players.iter_mut() {
            player.update();
            player.draw(&mut buffer);
        }

        // Update window
        window
            .update_with_buffer(&buffer, WIDTH, HEIGHT)
            .unwrap();

        // Sleep for ~16ms to cap at ~60 FPS
        sleep(Duration::from_millis(16));
        frame += 1;
    }
}

extern crate kiss3d;
extern crate nalgebra as na;

use kiss3d::window::Window;
use kiss3d::light::Light;
use na::{Point3, Vector3};

fn main() {
    let mut window = Window::new("Epoch of Elria - 3D Demo");
    window.set_light(Light::StickToCamera);

    // Create 3 cubes with different positions and velocities
    let mut cubes = vec![
        (window.add_cube(1.0, 1.0, 1.0), Vector3::new(0.02, 0.0, 0.0)),
        (window.add_cube(1.0, 1.0, 1.0), Vector3::new(0.0, 0.02, 0.0)),
        (window.add_cube(1.0, 1.0, 1.0), Vector3::new(0.0, 0.0, 0.02)),
    ];

    // Set initial positions
    cubes[0].0.set_local_translation(na::Translation3::new(-2.0, 0.0, 0.0));
    cubes[1].0.set_local_translation(na::Translation3::new(2.0, 0.0, 0.0));
    cubes[2].0.set_local_translation(na::Translation3::new(0.0, 2.0, 0.0));

    let mut frame = 0;
    while window.render() {
        // Move cubes for 120 frames, then stop
        if frame < 120 {
 for (cube, velocity) in cubes.iter_mut() {
                let mut pos = cube.data().local_translation().translation.vector;
                pos += *velocity;
                cube.set_local_translation(na::Translation3::from(pos));
            }
            frame += 1;
        }
        // Window stays open until user closes it
    }
}