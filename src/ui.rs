
'''
// src/ui.rs

use crate::input::InputManager;

// A simple component for text that will be displayed on the screen.
pub struct UIText {
    pub text: String,
    pub x: f32,
    pub y: f32,
    pub font_size: f32,
}

// A simple component for an image that will be displayed on the screen.
pub struct UIImage {
    pub path: String,
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
}

// A simple component for a button that will be displayed on the screen.
#[derive(Clone)]
pub struct UIButton {
    pub text: String,
    pub x: f32,
    pub y: f32,
    pub width: f32,
    pub height: f32,
}

// A container for all UI elements in a scene.
pub struct UI {
    pub texts: Vec<UIText>,
    pub images: Vec<UIImage>,
    pub buttons: Vec<UIButton>,
}

impl UI {
    pub fn new() -> Self {
        UI {
            texts: Vec::new(),
            images: Vec::new(),
            buttons: Vec::new(),
        }
    }

    pub fn do_button(&mut self, button: UIButton, input: &InputManager) -> bool {
        let mouse_pos = input.get_mouse_pos();
        let mouse_clicked = input.is_mouse_button_just_pressed(0); // Assuming 0 is the left mouse button

        let is_hovered = mouse_pos.0 >= button.x - button.width / 2.0
            && mouse_pos.0 <= button.x + button.width / 2.0
            && mouse_pos.1 >= button.y - button.height / 2.0
            && mouse_pos.1 <= button.y + button.height / 2.0;

        self.buttons.push(button.clone());

        is_hovered && mouse_clicked
    }
}
'''
