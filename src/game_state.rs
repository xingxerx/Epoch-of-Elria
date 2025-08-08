'''
// src/game_state.rs

use crate::ui::UIButton;

pub enum GameState {
    MainMenu,
    Playing,
    Settings,
}

pub struct GameStateContext {
    pub current_state: GameState,
}

impl GameStateContext {
    pub fn new() -> Self {
        GameStateContext {
            current_state: GameState::MainMenu,
        }
    }

    pub fn update(&mut self, ui: &mut crate::ui::UI, input: &crate::input::InputManager) {
        match self.current_state {
            GameState::MainMenu => {
                self.main_menu_update(ui, input);
            }
            GameState::Playing => {
                // Gameplay logic will go here
            }
            GameState::Settings => {
                // Settings screen logic will go here
            }
        }
    }

    fn main_menu_update(&mut self, ui: &mut crate::ui::UI, input: &crate::input::InputManager) {
        ui.buttons.clear();
        ui.texts.clear();

        // Add title
        ui.texts.push(crate::ui::UIText {
            text: "Epoch of Elria".to_string(),
            x: 400.0,
            y: 100.0,
            font_size: 50.0,
        });

        // Add buttons
        let play_button = UIButton {
            text: "Play".to_string(),
            x: 400.0,
            y: 250.0,
            width: 200.0,
            height: 50.0,
        };

        let settings_button = UIButton {
            text: "Settings".to_string(),
            x: 400.0,
            y: 320.0,
            width: 200.0,
            height: 50.0,
        };

        let quit_button = UIButton {
            text: "Quit".to_string(),
            x: 400.0,
            y: 390.0,
            width: 200.0,
            height: 50.0,
        };

        if ui.do_button(play_button, input) {
            self.current_state = GameState::Playing;
        }
        if ui.do_button(settings_button, input) {
            self.current_state = GameState::Settings;
        }
        if ui.do_button(quit_button, input) {
            // This is a bit of a hack, but it's the easiest way to quit
            // without a major refactor of the main loop.
            std::process::exit(0);
        }
    }
}
''