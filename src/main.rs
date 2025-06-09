extern crate kiss3d;

use kiss3d::window::Window;
use kiss3d::light::Light;
use kiss3d::nalgebra as na;
use na::{Vector3, Translation3};

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
    cubes[0].0.set_local_translation(Translation3::new(-2.0, 0.0, 0.0));
    cubes[1].0.set_local_translation(Translation3::new(2.0, 0.0, 0.0));
    cubes[2].0.set_local_translation(Translation3::new(0.0, 2.0, 0.0));

    let mut frame = 0;
    while window.render() {
        // Move cubes for 120 frames, then stop
        if frame < 120 {
 for (cube, velocity) in cubes.iter_mut() {
                let mut pos = cube.0.data().local_translation().vector;
                pos += *velocity;
                cube.0.set_local_translation(Translation3::from(pos));
            }
            frame += 1;
        }
        // Window stays open until user closes it
    }
}