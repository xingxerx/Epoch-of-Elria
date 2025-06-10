// math.rs - Mathematical utilities for the game engine

use std::ops::{Add, Sub, Mul, Div};

// 2D Vector implementation
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Vector2D {
    pub x: f32,
    pub y: f32,
}

impl Vector2D {
    pub fn new(x: f32, y: f32) -> Self {
        Self { x, y }
    }

    pub fn zero() -> Self {
        Self::new(0.0, 0.0)
    }

    pub fn one() -> Self {
        Self::new(1.0, 1.0)
    }

    pub fn up() -> Self {
        Self::new(0.0, 1.0)
    }

    pub fn down() -> Self {
        Self::new(0.0, -1.0)
    }

    pub fn left() -> Self {
        Self::new(-1.0, 0.0)
    }

    pub fn right() -> Self {
        Self::new(1.0, 0.0)
    }

    pub fn magnitude(&self) -> f32 {
        (self.x * self.x + self.y * self.y).sqrt()
    }

    pub fn magnitude_squared(&self) -> f32 {
        self.x * self.x + self.y * self.y
    }

    pub fn normalize(&self) -> Self {
        let mag = self.magnitude();
        if mag > f32::EPSILON {
            Self::new(self.x / mag, self.y / mag)
        } else {
            Self::zero()
        }
    }

    pub fn normalized(&mut self) -> &mut Self {
        let mag = self.magnitude();
        if mag > f32::EPSILON {
            self.x /= mag;
            self.y /= mag;
        } else {
            self.x = 0.0;
            self.y = 0.0;
        }
        self
    }

    pub fn distance(&self, other: &Vector2D) -> f32 {
        (*self - *other).magnitude()
    }

    pub fn distance_squared(&self, other: &Vector2D) -> f32 {
        (*self - *other).magnitude_squared()
    }

    pub fn dot(&self, other: &Vector2D) -> f32 {
        self.x * other.x + self.y * other.y
    }

    pub fn cross(&self, other: &Vector2D) -> f32 {
        self.x * other.y - self.y * other.x
    }

    pub fn angle(&self) -> f32 {
        self.y.atan2(self.x)
    }

    pub fn angle_to(&self, other: &Vector2D) -> f32 {
        (other.y - self.y).atan2(other.x - self.x)
    }

    pub fn rotate(&self, angle: f32) -> Self {
        let cos_a = angle.cos();
        let sin_a = angle.sin();
        Self::new(
            self.x * cos_a - self.y * sin_a,
            self.x * sin_a + self.y * cos_a,
        )
    }

    pub fn lerp(&self, other: &Vector2D, t: f32) -> Self {
        *self + (*other - *self) * t
    }

    pub fn reflect(&self, normal: &Vector2D) -> Self {
        *self - *normal * (2.0 * self.dot(normal))
    }

    pub fn project(&self, onto: &Vector2D) -> Self {
        let dot = self.dot(onto);
        let mag_sq = onto.magnitude_squared();
        if mag_sq > f32::EPSILON {
            *onto * (dot / mag_sq)
        } else {
            Self::zero()
        }
    }

    pub fn perpendicular(&self) -> Self {
        Self::new(-self.y, self.x)
    }

    pub fn clamp_magnitude(&self, max_magnitude: f32) -> Self {
        let mag = self.magnitude();
        if mag > max_magnitude {
            self.normalize() * max_magnitude
        } else {
            *self
        }
    }
}

// Operator implementations for Vector2D
impl Add for Vector2D {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self::new(self.x + other.x, self.y + other.y)
    }
}

impl Sub for Vector2D {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self::new(self.x - other.x, self.y - other.y)
    }
}

impl Mul<f32> for Vector2D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Self::new(self.x * scalar, self.y * scalar)
    }
}

impl Mul<Vector2D> for f32 {
    type Output = Vector2D;
    fn mul(self, vector: Vector2D) -> Vector2D {
        Vector2D::new(vector.x * self, vector.y * self)
    }
}

impl Div<f32> for Vector2D {
    type Output = Self;
    fn div(self, scalar: f32) -> Self {
        Self::new(self.x / scalar, self.y / scalar)
    }
}

// 3D Vector implementation
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Vector3D {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

impl Vector3D {
    pub fn new(x: f32, y: f32, z: f32) -> Self {
        Self { x, y, z }
    }

    pub fn zero() -> Self {
        Self::new(0.0, 0.0, 0.0)
    }

    pub fn one() -> Self {
        Self::new(1.0, 1.0, 1.0)
    }

    pub fn up() -> Self {
        Self::new(0.0, 1.0, 0.0)
    }

    pub fn down() -> Self {
        Self::new(0.0, -1.0, 0.0)
    }

    pub fn left() -> Self {
        Self::new(-1.0, 0.0, 0.0)
    }

    pub fn right() -> Self {
        Self::new(1.0, 0.0, 0.0)
    }

    pub fn forward() -> Self {
        Self::new(0.0, 0.0, -1.0)
    }

    pub fn back() -> Self {
        Self::new(0.0, 0.0, 1.0)
    }

    pub fn magnitude(&self) -> f32 {
        (self.x * self.x + self.y * self.y + self.z * self.z).sqrt()
    }

    pub fn magnitude_squared(&self) -> f32 {
        self.x * self.x + self.y * self.y + self.z * self.z
    }

    pub fn normalize(&self) -> Self {
        let mag = self.magnitude();
        if mag > f32::EPSILON {
            Self::new(self.x / mag, self.y / mag, self.z / mag)
        } else {
            Self::zero()
        }
    }

    pub fn normalized(&mut self) -> &mut Self {
        let mag = self.magnitude();
        if mag > f32::EPSILON {
            self.x /= mag;
            self.y /= mag;
            self.z /= mag;
        } else {
            self.x = 0.0;
            self.y = 0.0;
            self.z = 0.0;
        }
        self
    }

    pub fn distance(&self, other: &Vector3D) -> f32 {
        (*self - *other).magnitude()
    }

    pub fn distance_squared(&self, other: &Vector3D) -> f32 {
        (*self - *other).magnitude_squared()
    }

    pub fn dot(&self, other: &Vector3D) -> f32 {
        self.x * other.x + self.y * other.y + self.z * other.z
    }

    pub fn cross(&self, other: &Vector3D) -> Self {
        Self::new(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x,
        )
    }

    pub fn lerp(&self, other: &Vector3D, t: f32) -> Self {
        *self + (*other - *self) * t
    }

    pub fn reflect(&self, normal: &Vector3D) -> Self {
        *self - *normal * (2.0 * self.dot(normal))
    }

    pub fn project(&self, onto: &Vector3D) -> Self {
        let dot = self.dot(onto);
        let mag_sq = onto.magnitude_squared();
        if mag_sq > f32::EPSILON {
            *onto * (dot / mag_sq)
        } else {
            Self::zero()
        }
    }

    pub fn clamp_magnitude(&self, max_magnitude: f32) -> Self {
        let mag = self.magnitude();
        if mag > max_magnitude {
            self.normalize() * max_magnitude
        } else {
            *self
        }
    }

    // Conversion methods for kiss3d compatibility
    pub fn to_point3(&self) -> kiss3d::nalgebra::Point3<f32> {
        kiss3d::nalgebra::Point3::new(self.x, self.y, self.z)
    }

    pub fn to_vector3(&self) -> kiss3d::nalgebra::Vector3<f32> {
        kiss3d::nalgebra::Vector3::new(self.x, self.y, self.z)
    }

    pub fn from_point3(point: &kiss3d::nalgebra::Point3<f32>) -> Self {
        Self::new(point.x, point.y, point.z)
    }

    pub fn from_vector3(vector: &kiss3d::nalgebra::Vector3<f32>) -> Self {
        Self::new(vector.x, vector.y, vector.z)
    }

    // Convert to 2D by dropping Z component
    pub fn to_vector2d(&self) -> Vector2D {
        Vector2D::new(self.x, self.y)
    }
}

// Operator implementations for Vector3D
impl Add for Vector3D {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self::new(self.x + other.x, self.y + other.y, self.z + other.z)
    }
}

impl Sub for Vector3D {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self::new(self.x - other.x, self.y - other.y, self.z - other.z)
    }
}

impl Mul<f32> for Vector3D {
    type Output = Self;
    fn mul(self, scalar: f32) -> Self {
        Self::new(self.x * scalar, self.y * scalar, self.z * scalar)
    }
}

impl Mul<Vector3D> for f32 {
    type Output = Vector3D;
    fn mul(self, vector: Vector3D) -> Vector3D {
        Vector3D::new(vector.x * self, vector.y * self, vector.z * self)
    }
}

impl Div<f32> for Vector3D {
    type Output = Self;
    fn div(self, scalar: f32) -> Self {
        Self::new(self.x / scalar, self.y / scalar, self.z / scalar)
    }
}

// Matrix utilities
#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Matrix3x3 {
    pub m: [[f32; 3]; 3],
}

impl Matrix3x3 {
    pub fn identity() -> Self {
        Self {
            m: [
                [1.0, 0.0, 0.0],
                [0.0, 1.0, 0.0],
                [0.0, 0.0, 1.0],
            ],
        }
    }

    pub fn rotation_z(angle: f32) -> Self {
        let cos_a = angle.cos();
        let sin_a = angle.sin();
        Self {
            m: [
                [cos_a, -sin_a, 0.0],
                [sin_a, cos_a, 0.0],
                [0.0, 0.0, 1.0],
            ],
        }
    }

    pub fn scale(sx: f32, sy: f32) -> Self {
        Self {
            m: [
                [sx, 0.0, 0.0],
                [0.0, sy, 0.0],
                [0.0, 0.0, 1.0],
            ],
        }
    }

    pub fn translation(tx: f32, ty: f32) -> Self {
        Self {
            m: [
                [1.0, 0.0, tx],
                [0.0, 1.0, ty],
                [0.0, 0.0, 1.0],
            ],
        }
    }

    pub fn transform_point(&self, point: Vector2D) -> Vector2D {
        Vector2D::new(
            self.m[0][0] * point.x + self.m[0][1] * point.y + self.m[0][2],
            self.m[1][0] * point.x + self.m[1][1] * point.y + self.m[1][2],
        )
    }

    pub fn transform_vector(&self, vector: Vector2D) -> Vector2D {
        Vector2D::new(
            self.m[0][0] * vector.x + self.m[0][1] * vector.y,
            self.m[1][0] * vector.x + self.m[1][1] * vector.y,
        )
    }
}

impl Mul for Matrix3x3 {
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        let mut result = Self::identity();
        for i in 0..3 {
            for j in 0..3 {
                result.m[i][j] = 0.0;
                for k in 0..3 {
                    result.m[i][j] += self.m[i][k] * other.m[k][j];
                }
            }
        }
        result
    }
}

// Utility functions
pub fn clamp(value: f32, min: f32, max: f32) -> f32 {
    if value < min {
        min
    } else if value > max {
        max
    } else {
        value
    }
}

pub fn lerp(a: f32, b: f32, t: f32) -> f32 {
    a + (b - a) * t
}

pub fn smoothstep(edge0: f32, edge1: f32, x: f32) -> f32 {
    let t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    t * t * (3.0 - 2.0 * t)
}

pub fn degrees_to_radians(degrees: f32) -> f32 {
    degrees * std::f32::consts::PI / 180.0
}

pub fn radians_to_degrees(radians: f32) -> f32 {
    radians * 180.0 / std::f32::consts::PI
}

// Random number utilities
pub struct Random {
    seed: u64,
}

impl Random {
    pub fn new(seed: u64) -> Self {
        Self { seed }
    }

    pub fn from_time() -> Self {
        use std::time::{SystemTime, UNIX_EPOCH};
        let seed = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos() as u64;
        Self::new(seed)
    }

    pub fn next_f32(&mut self) -> f32 {
        self.seed = self.seed.wrapping_mul(1103515245).wrapping_add(12345);
        ((self.seed / 65536) % 32768) as f32 / 32767.0
    }

    pub fn range_f32(&mut self, min: f32, max: f32) -> f32 {
        min + self.next_f32() * (max - min)
    }

    pub fn next_i32(&mut self) -> i32 {
        self.seed = self.seed.wrapping_mul(1103515245).wrapping_add(12345);
        (self.seed / 65536) as i32
    }

    pub fn range_i32(&mut self, min: i32, max: i32) -> i32 {
        min + (self.next_i32().abs() % (max - min + 1))
    }

    pub fn random_vector2d(&mut self, min_magnitude: f32, max_magnitude: f32) -> Vector2D {
        let angle = self.range_f32(0.0, 2.0 * std::f32::consts::PI);
        let magnitude = self.range_f32(min_magnitude, max_magnitude);
        Vector2D::new(angle.cos() * magnitude, angle.sin() * magnitude)
    }

    pub fn random_vector3d(&mut self, min_magnitude: f32, max_magnitude: f32) -> Vector3D {
        let theta = self.range_f32(0.0, 2.0 * std::f32::consts::PI);
        let phi = self.range_f32(0.0, std::f32::consts::PI);
        let magnitude = self.range_f32(min_magnitude, max_magnitude);
        
        let sin_phi = phi.sin();
        Vector3D::new(
            sin_phi * theta.cos() * magnitude,
            phi.cos() * magnitude,
            sin_phi * theta.sin() * magnitude,
        )
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vector2d_operations() {
        let v1 = Vector2D::new(3.0, 4.0);
        let v2 = Vector2D::new(1.0, 2.0);

        assert_eq!(v1 + v2, Vector2D::new(4.0, 6.0));
        assert_eq!(v1 - v2, Vector2D::new(2.0, 2.0));
        assert_eq!(v1 * 2.0, Vector2D::new(6.0, 8.0));
        assert_eq!(v1.magnitude(), 5.0);
        assert_eq!(v1.dot(&v2), 11.0);
    }

    #[test]
    fn test_vector3d_operations() {
        let v1 = Vector3D::new(1.0, 2.0, 3.0);
        let v2 = Vector3D::new(4.0, 5.0, 6.0);

        assert_eq!(v1 + v2, Vector3D::new(5.0, 7.0, 9.0));
        assert_eq!(v1 - v2, Vector3D::new(-3.0, -3.0, -3.0));
        assert_eq!(v1 * 2.0, Vector3D::new(2.0, 4.0, 6.0));
        assert_eq!(v1.dot(&v2), 32.0);
        assert_eq!(v1.cross(&v2), Vector3D::new(-3.0, 6.0, -3.0));
    }

    #[test]
    fn test_matrix_operations() {
        let m1 = Matrix3x3::identity();
        let point = Vector2D::new(1.0, 2.0);
        assert_eq!(m1.transform_point(point), point);

        let rotation = Matrix3x3::rotation_z(std::f32::consts::PI / 2.0);
        let rotated = rotation.transform_point(Vector2D::new(1.0, 0.0));
        assert!((rotated.x - 0.0).abs() < 0.001);
        assert!((rotated.y - 1.0).abs() < 0.001);
    }
}