use minifb::{Key, Window, WindowOptions};

const WIDTH: usize = 800;
const HEIGHT: usize = 600;

fn main() {
    let mut buffer: Vec<u32> = vec![0; WIDTH * HEIGHT];
    let mut window = Window::new(
        "Epoch of Elria",
        WIDTH,
        HEIGHT,
        WindowOptions::default(),
    ).unwrap_or_else(|e| {
        panic!("Failed to create window: {}", e);
    });

    // Set a maximum FPS to not run too fast
    window.limit_update_rate(Some(std::time::Duration::from_micros(16600)));

    let mut frame_count = 0;
    let mut positions = [(202, 0), (2, 0), (98, 0)];
    let rect_size = 40;

    while window.is_open() && !window.is_key_down(Key::Escape) && frame_count < 120 {
        // Clear buffer
        for pixel in buffer.iter_mut() {
            *pixel = 0;
        }

        // Draw rectangles
        for (x, y) in positions.iter() {
            for dy in 0..rect_size {
                for dx in 0..rect_size {
                    let px = *x + dx;
                    let py = *y + dy;
                    if px < WIDTH && py < HEIGHT {
                        buffer[py * WIDTH + px] = 0x0000FF;  // Blue color
                    }
                }
            }
        }

        // Update positions
        positions[0].0 += 2;  // First rectangle moves right
        positions[1].0 += 2;  // Second rectangle moves right
        positions[2].0 -= 2;  // Third rectangle moves left

        // Update window
        window.update_with_buffer(&buffer, WIDTH, HEIGHT)
            .unwrap_or_else(|e| {
                panic!("Failed to update window: {}", e);
            });

        frame_count += 1;
    }
}