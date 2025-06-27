
// src/ui.rs

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

// A container for all UI elements in a scene.
pub struct UI {
    pub texts: Vec<UIText>,
    pub images: Vec<UIImage>,
}

impl UI {
    pub fn new() -> Self {
        UI {
            texts: Vec::new(),
            images: Vec::new(),
        }
    }
}
