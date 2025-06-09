use std::ops::{Add, Sub, Mul};

#[derive(Clone, Copy, Debug)]
struct Vector2D {
    x: f32,
    y: f32,
}

impl Vector2D {
    fn new(x: f32, y: f32) -> Self {
        Vector2D { x, y }
    }

    fn magnitude(&self) -> f32 {
        (self.x * self.x + self.y * self.y).sqrt()
    }

    fn normalize(&self) -> Self {
        let mag = self.magnitude();
        if mag == 0.0 {
            Vector2D { x: 0.0, y: 0.0 }
        } else {
            Vector2D { x: self.x / mag, y: self.y / mag }
        }
    }
}

impl Add for Vector2D {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Vector2D { x: self.x + other.x, y: self.y + other.y }
    }
}

impl Sub for Vector2D {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Vector2D { x: self.x - other.x, y: self.y - other.y }
    }
}

impl Mul<f32> for2D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Vector2D { x: self.x * scalar, y: self.y * scalar }
    }
}

// Add a main function so the project can build and run
fn main() {
    println!("Epoch of Elria game engine (Rust) starting up!");
}