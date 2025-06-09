use std::ops::{Add, Sub, Mul};

use std::fmt::Write as FmtWrite;

#[derive(Clone, Copy, Debug)]
struct Vector2D {
    x: f32,
    y: f32,
}

#[allow(dead_code)]
impl Vector2D {
    #[allow(dead_code)]
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
        let _ = FmtWrite::write_fmt(
            svg,
            format_args!(
                r#"<rect x="{}" y="{}" width="{}" height="{}" fill="blue" />"#,
                self.position.x, self.position.y, self.width, self.height
            ),
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

    // Create two players at different positions
    let mut player1 = Player::new(Vector2D::new(200.0, 0.0));
    let mut player2 = Player::new(Vector2D::new(0.0, 0.0));
    let mut player3 = Player {
        position: Vector2D::new(100.0, 0.0),
        velocity: Vector2D::new(-2.0, 0.0),
        width: 40.0,
        height: 40.0,
    };

    // Game loop for 120 frames
    for frame in 0..120 {
        println!("Frame {}", frame);
        player1.update();
        player2.update();
        player3.update();

        // Draw to SVG string (for demonstration)
        let mut svg = String::from(
            r#"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">"#,
        );
        player1.draw(&mut svg);
        player2.draw(&mut svg);
        player3.draw(&mut svg);
        svg.push_str("</svg>");

        println!("{}", svg);

        // Check for collisions between all pairs
        let collision_1_2 = player1.collides_with(&player2);
        let collision_1_3 = player1.collides_with(&player3);
        let collision_2_3 = player2.collides_with(&player3);
        println!(
            "Collisions: 1-2: {}, 1-3: {}, 2-3: {}",
            collision_1_2, collision_1_3, collision_2_3
        );
    }
}
