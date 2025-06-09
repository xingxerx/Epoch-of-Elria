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

impl Mul<f32> for Vector2D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Vector2D { x: self.x * scalar, y: self.y * scalar }
    }
}

use std::fmt::Write as FmtWrite;
use std::io::{self, Write};

trait GameObject {
    fn update(&mut self);
    fn draw(&self, svg: &mut String);
    fn get_position(&self) -> Vector2D;
    fn get_size(&self) -> (f32, f32);

    fn collides_with(&self, other: &dyn GameObject) -> bool {
        let (x1, y1) = (self.get_position().x, self.get_position().y);
        let (w1, h1) = self.get_size();
        let (x2, y2) = (other.get_position().x, other.get_position().y);
        let (w2, h2) = other.get_size();

        x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2
    }
}

struct Player {
    position: Vector2D,
    velocity: Vector2D,
    width: f32,
    height: f32,
}

impl Player {
    fn new(position: Vector2D) -> Self {
        Player {
            position,
            velocity: Vector2D::new(2.0, 0.0),
            width: 40.0,
            height: 40.0,
        }
    }
}

impl GameObject for Player {
    fn update(&mut self) {
        self.position = self.position + self.velocity;
    }

    fn draw(&self, svg: &mut String) {
        let _ = write!(
            svg,
            r#"<rect x="{}" y="{}" width="{}" height="{}" fill="blue" />"#,
            self.position.x, self.position.y, self.width, self.height
        );
    }

    fn get_position(&self) -> Vector2D {
        self.position
    }

    fn get_size(&self) -> (f32, f32) {
        (self.width, self.height)
    }
}

fn main() {
    println!("Epoch of Elria game engine (Rust) starting up!");

    let mut player = Player::new(Vector2D::new(0.0, 0.0));

    for frame in 0..10 {
        println!("Frame {}", frame);
        player.update();

        let mut svg = String::from(
            r#"<svg xmlns="http://www.w3.org/2000/svg" width="800" height="600">"#,
        );
        player.draw(&mut svg);
        svg.push_str("</svg>");

        println!("{}", svg);

        print!("> ");
        io::stdout().flush().unwrap();
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        println!("You entered: {}", input.trim());
    }
}

use std::fmt::Write as FmtWrite; // For SVG string building
use std::io::{self, Write};

trait GameObject {
    fn update(&mut self);
    fn draw(&self, svg: &mut String);
    fn get_position(&self) -> Vector2D;
    fn get_size(&self) -> (f32, f32);

    // Axis-Aligned Bounding Box collision
    fn collides_with(&self, other: &dyn GameObject) -> bool {
        let (x1, y1) = (self.get_position().x, self.get_position().y);
        let (w1, h1) = self.get_size();
        let (x2, y2) = (other.get_position().x, other.get_position().y);
        let (w2, h2) = other.get_size();

        x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2
    }
}

struct Player {
    position: Vector2D,
    velocity: Vector2D,
    width: f32,
    height: f32,
}

impl Player {
    fn new(position: Vector2D) -> Self {
        Player {
            position,
            velocity: Vector2D::new(2.0, 0.0), // Moves right
            width: 40.0,
            height: 40.0,
        }
    }
}

impl GameObject for Player {
    fn update(&mut self) {
        self.position = self.position + self.velocity;
    }

    fn draw(&self, svg: &mut String) {
        let _ = write!(
            svg,
            r#"<rect x=\"{}\" y=\"{}\" width=\"{}\" height=\"{}\" fill=\"blue\" />"#,
            self.position.x, self.position.y, self.width, self.height
        );
    }

    fn get_position(&self) -> Vector2D {
        self.position
    }

    fn get_size(&self) -> (f32, f32) {
        (self.width, self.height)
    }
}

fn main() {
    println!("Epoch of Elria game engine (Rust) starting up!");

    // Create a player at position (0, 0)
    let mut player = Player::new(Vector2D::new(0.0, 0.0));

    // Simple game loop (10 frames for demonstration)
    for frame in 0..10 {
        println!("Frame {}", frame);
        player.update();

        // Draw to SVG string (for demonstration)
        let mut svg = String::from(
            r#"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">"#,
        );
        player.draw(&mut svg);
        svg.push_str("</svg>");

        // Print SVG output to terminal (for now)
        println!("{}", svg);

        // Prompt for user input (future interactive features)
        print!("> ");
        io::stdout().flush().unwrap();
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        // For now, just echo the input
        println!("You entered: {}", input.trim());
    }
}

use std::fmt::Write as FmtWrite; // For SVG string building

trait GameObject {
    fn update(&mut self);
    fn draw(&self, svg: &mut String);
    fn get_position(&self) -> Vector2D;
    fn get_size(&self) -> (f32, f32);

    // Axis-Aligned Bounding Box collision
    fn collides_with(&self, other: &dyn GameObject) -> bool {
        let (x1, y1) = (self.get_position().x, self.get_position().y);
        let (w1, h1) = self.get_size();
        let (x2, y2) = (other.get_position().x, other.get_position().y);
        let (w2, h2) = other.get_size();

        x1 < x2 + w && x1 + w1 > x2 && y1 < y2 + h && y1 + h1 > y2
    }
}

struct Player {
    position: Vector2D,
    velocity: Vector2D,
    width: f32,
    height: f32,
}

impl Player {
    fn new(position: Vector2D) -> Self {
        Player {
            position,
            velocity: Vector2D::new(2.0, 0.0), // Moves right
            width: 40.0,
            height: 40.0,
        }
    }
}

impl GameObject for Player {
    fn update(&mut self) {
        self.position = self.position + self.velocity;
    }

    fn draw(&self, svg: &mut String) {
        let _ = write!(
            svg,
            r#"<rect x{}" y="{}" width="{}" height="{}" fill="blue" />"#,
            self.position.x, self.position.y, self.width, self.height
        );
    }

    fn get_position(&self) -> Vector2D {
        self.position
    }

    fn get_size(&self) -> (f32, f32) {
        (self.width, self.height)
    }
}

use std::io::{self, Write};

fn main() {
    println!("Epoch of Elria game engine (Rust) starting up!");

    // Create a player at position (0, 0)
    let mut player = Player::new(Vector2D::new(0.0, 0.0));

    // Simple game loop (10 frames for demonstration)
    for frame in 0..10 {
        println!("Frame {}", frame);
        player.update();

        // Draw to SVG string (for demonstration)
        let mut svg = String::from(
            r#"<svg xmlnshttp://www.w3.org/2000/svg" width="800" height="600">"#,
        );
        player.draw(&mut svg);
        svg.push_str("</svg>");

 // Print SVG output to terminal (for now)
        println!("{}", svg);

        // Prompt for user input (future interactive features)
        print!("> ");
        io::stdout().flush().unwrap();
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        // For now, just echo the input
        println!("You entered: {}", input.trim());
    }
}