// audio.rs - Audio system for the game engine

use std::collections::HashMap;
use std::path::Path;

// Note: This is a placeholder audio system. In a real implementation,
// you would use a crate like `rodio`, `cpal`, or `kira` for audio playback.

pub struct AudioSystem {
    master_volume: f32,
    music_volume: f32,
    sfx_volume: f32,
    loaded_sounds: HashMap<String, AudioClip>,
    playing_sounds: Vec<PlayingSound>,
    current_music: Option<String>,
    next_sound_id: u32,
    muted: bool,
}

impl AudioSystem {
    pub fn new() -> Result<Self, Box<dyn std::error::Error>> {
        Ok(Self {
            master_volume: 1.0,
            music_volume: 0.7,
            sfx_volume: 0.8,
            loaded_sounds: HashMap::new(),
            playing_sounds: Vec::new(),
            current_music: None,
            next_sound_id: 0,
            muted: false,
        })
    }

    pub fn update(&mut self, delta_time: f32) {
        // Update playing sounds
        self.playing_sounds.retain_mut(|sound| {
            sound.update(delta_time);
            !sound.is_finished()
        });
    }

    // Sound loading and management
    pub fn load_sound(&mut self, name: &str, file_path: &str) -> Result<(), Box<dyn std::error::Error>> {
        // In a real implementation, this would load the audio file
        let clip = AudioClip::new(file_path)?;
        self.loaded_sounds.insert(name.to_string(), clip);
        println!("Loaded audio: {} from {}", name, file_path);
        Ok(())
    }

    pub fn unload_sound(&mut self, name: &str) {
        self.loaded_sounds.remove(name);
        println!("Unloaded audio: {}", name);
    }

    pub fn is_sound_loaded(&self, name: &str) -> bool {
        self.loaded_sounds.contains_key(name)
    }

    // Sound playback
    pub fn play_sound(&mut self, name: &str) -> Option<u32> {
        self.play_sound_with_settings(name, 1.0, 1.0, false)
    }

    pub fn play_sound_with_volume(&mut self, name: &str, volume: f32) -> Option<u32> {
        self.play_sound_with_settings(name, volume, 1.0, false)
    }

    pub fn play_sound_with_settings(&mut self, name: &str, volume: f32, pitch: f32, looping: bool) -> Option<u32> {
        if self.muted {
            return None;
        }

        if let Some(clip) = self.loaded_sounds.get(name) {
            let sound_id = self.next_sound_id;
            self.next_sound_id += 1;

            let playing_sound = PlayingSound {
                id: sound_id,
                clip: clip.clone(),
                volume: volume * self.sfx_volume * self.master_volume,
                pitch,
                looping,
                position: 0.0,
                finished: false,
            };

            self.playing_sounds.push(playing_sound);
            println!("Playing sound: {} (ID: {})", name, sound_id);
            Some(sound_id)
        } else {
            println!("Warning: Sound '{}' not found", name);
            None
        }
    }

    pub fn stop_sound(&mut self, sound_id: u32) {
        if let Some(sound) = self.playing_sounds.iter_mut().find(|s| s.id == sound_id) {
            sound.finished = true;
            println!("Stopped sound ID: {}", sound_id);
        }
    }

    pub fn stop_all_sounds(&mut self) {
        self.playing_sounds.clear();
        println!("Stopped all sounds");
    }

    pub fn is_sound_playing(&self, sound_id: u32) -> bool {
        self.playing_sounds.iter().any(|s| s.id == sound_id && !s.finished)
    }

    // Music playback
    pub fn play_music(&mut self, name: &str) -> Result<(), Box<dyn std::error::Error>> {
        self.play_music_with_settings(name, true, 1.0)
    }

    pub fn play_music_with_settings(&mut self, name: &str, looping: bool, volume: f32) -> Result<(), Box<dyn std::error::Error>> {
        if self.muted {
            return Ok(());
        }

        // Stop current music
        if let Some(current) = &self.current_music {
            self.stop_music();
        }

        if let Some(clip) = self.loaded_sounds.get(name) {
            let sound_id = self.next_sound_id;
            self.next_sound_id += 1;

            let playing_sound = PlayingSound {
                id: sound_id,
                clip: clip.clone(),
                volume: volume * self.music_volume * self.master_volume,
                pitch: 1.0,
                looping,
                position: 0.0,
                finished: false,
            };

            self.playing_sounds.push(playing_sound);
            self.current_music = Some(name.to_string());
            println!("Playing music: {}", name);
            Ok(())
        } else {
            Err(format!("Music '{}' not found", name).into())
        }
    }

    pub fn stop_music(&mut self) {
        if let Some(music_name) = &self.current_music {
            // Find and stop the music
            for sound in &mut self.playing_sounds {
                if sound.looping {
                    sound.finished = true;
                }
            }
            println!("Stopped music: {}", music_name);
            self.current_music = None;
        }
    }

    pub fn pause_music(&mut self) {
        // In a real implementation, this would pause the music
        println!("Music paused");
    }

    pub fn resume_music(&mut self) {
        // In a real implementation, this would resume the music
        println!("Music resumed");
    }

    pub fn is_music_playing(&self) -> bool {
        self.current_music.is_some()
    }

    // Volume controls
    pub fn set_master_volume(&mut self, volume: f32) {
        self.master_volume = volume.clamp(0.0, 1.0);
        println!("Master volume set to: {:.2}", self.master_volume);
    }

    pub fn set_music_volume(&mut self, volume: f32) {
        self.music_volume = volume.clamp(0.0, 1.0);
        println!("Music volume set to: {:.2}", self.music_volume);
    }

    pub fn set_sfx_volume(&mut self, volume: f32) {
        self.sfx_volume = volume.clamp(0.0, 1.0);
        println!("SFX volume set to: {:.2}", self.sfx_volume);
    }

    pub fn get_master_volume(&self) -> f32 {
        self.master_volume
    }

    pub fn get_music_volume(&self) -> f32 {
        self.music_volume
    }

    pub fn get_sfx_volume(&self) -> f32 {
        self.sfx_volume
    }

    pub fn mute(&mut self) {
        self.muted = true;
        println!("Audio muted");
    }

    pub fn unmute(&mut self) {
        self.muted = false;
        println!("Audio unmuted");
    }

    pub fn toggle_mute(&mut self) {
        if self.muted {
            self.unmute();
        } else {
            self.mute();
        }
    }

    pub fn is_muted(&self) -> bool {
        self.muted
    }

    // 3D Audio (placeholder for spatial audio)
    pub fn play_sound_3d(&mut self, name: &str, position: (f32, f32, f32), listener_position: (f32, f32, f32)) -> Option<u32> {
        // Calculate distance-based volume
        let distance = {
            let dx = position.0 - listener_position.0;
            let dy = position.1 - listener_position.1;
            let dz = position.2 - listener_position.2;
            (dx * dx + dy * dy + dz * dz).sqrt()
        };

        // Simple distance attenuation
        let max_distance = 100.0;
        let volume = if distance < max_distance {
            1.0 - (distance / max_distance)
        } else {
            0.0
        };

        if volume > 0.01 {
            self.play_sound_with_volume(name, volume)
        } else {
            None
        }
    }

    // Audio effects (placeholder)
    pub fn set_sound_pitch(&mut self, sound_id: u32, pitch: f32) {
        if let Some(sound) = self.playing_sounds.iter_mut().find(|s| s.id == sound_id) {
            sound.pitch = pitch.clamp(0.1, 3.0);
            println!("Set pitch for sound ID {}: {:.2}", sound_id, sound.pitch);
        }
    }

    pub fn set_sound_volume(&mut self, sound_id: u32, volume: f32) {
        if let Some(sound) = self.playing_sounds.iter_mut().find(|s| s.id == sound_id) {
            sound.volume = volume.clamp(0.0, 1.0) * self.sfx_volume * self.master_volume;
            println!("Set volume for sound ID {}: {:.2}", sound_id, volume);
        }
    }

    // Utility methods
    pub fn get_playing_sound_count(&self) -> usize {
        self.playing_sounds.len()
    }

    pub fn get_loaded_sound_count(&self) -> usize {
        self.loaded_sounds.len()
    }

    pub fn get_loaded_sound_names(&self) -> Vec<String> {
        self.loaded_sounds.keys().cloned().collect()
    }

    // Preload common sounds
    pub fn preload_common_sounds(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        // This would load common game sounds
        let common_sounds = vec![
            ("jump", "assets/audio/jump.wav"),
            ("collect", "assets/audio/collect.wav"),
            ("hurt", "assets/audio/hurt.wav"),
            ("menu_select", "assets/audio/menu_select.wav"),
            ("menu_back", "assets/audio/menu_back.wav"),
        ];

        for (name, path) in common_sounds {
            if Path::new(path).exists() {
                self.load_sound(name, path)?;
            } else {
                println!("Warning: Audio file not found: {}", path);
            }
        }

        Ok(())
    }
}

impl Default for AudioSystem {
    fn default() -> Self {
        Self::new().unwrap_or_else(|_| {
            println!("Warning: Failed to initialize audio system");
            Self {
                master_volume: 1.0,
                music_volume: 0.7,
                sfx_volume: 0.8,
                loaded_sounds: HashMap::new(),
                playing_sounds: Vec::new(),
                current_music: None,
                next_sound_id: 0,
                muted: false,
            }
        })
    }
}

#[derive(Debug, Clone)]
pub struct AudioClip {
    pub file_path: String,
    pub duration: f32,
    pub sample_rate: u32,
    pub channels: u16,
}

impl AudioClip {
    pub fn new(file_path: &str) -> Result<Self, Box<dyn std::error::Error>> {
        // In a real implementation, this would load and analyze the audio file
        Ok(Self {
            file_path: file_path.to_string(),
            duration: 1.0, // Placeholder duration
            sample_rate: 44100,
            channels: 2,
        })
    }
}

#[derive(Debug, Clone)]
pub struct PlayingSound {
    pub id: u32,
    pub clip: AudioClip,
    pub volume: f32,
    pub pitch: f32,
    pub looping: bool,
    pub position: f32, // Current playback position in seconds
    pub finished: bool,
}

impl PlayingSound {
    pub fn update(&mut self, delta_time: f32) {
        if self.finished {
            return;
        }

        self.position += delta_time * self.pitch;

        if self.position >= self.clip.duration {
            if self.looping {
                self.position = 0.0;
            } else {
                self.finished = true;
            }
        }
    }

    pub fn is_finished(&self) -> bool {
        self.finished
    }

    pub fn get_progress(&self) -> f32 {
        if self.clip.duration > 0.0 {
            (self.position / self.clip.duration).clamp(0.0, 1.0)
        } else {
            1.0
        }
    }
}

// Audio configuration
#[derive(Debug, Clone)]
pub struct AudioConfig {
    pub master_volume: f32,
    pub music_volume: f32,
    pub sfx_volume: f32,
    pub sample_rate: u32,
    pub buffer_size: u32,
    pub max_concurrent_sounds: usize,
}

impl Default for AudioConfig {
    fn default() -> Self {
        Self {
            master_volume: 1.0,
            music_volume: 0.7,
            sfx_volume: 0.8,
            sample_rate: 44100,
            buffer_size: 1024,
            max_concurrent_sounds: 32,
        }
    }
}

// Audio events for game integration
#[derive(Debug, Clone)]
pub enum AudioEvent {
    SoundStarted { id: u32, name: String },
    SoundFinished { id: u32, name: String },
    MusicStarted { name: String },
    MusicFinished { name: String },
    VolumeChanged { volume_type: VolumeType, new_volume: f32 },
}

#[derive(Debug, Clone)]
pub enum VolumeType {
    Master,
    Music,
    SFX,
}

// Audio listener for 3D audio
#[derive(Debug, Clone, Copy)]
pub struct AudioListener {
    pub position: (f32, f32, f32),
    pub forward: (f32, f32, f32),
    pub up: (f32, f32, f32),
    pub velocity: (f32, f32, f32),
}

impl Default for AudioListener {
    fn default() -> Self {
        Self {
            position: (0.0, 0.0, 0.0),
            forward: (0.0, 0.0, -1.0),
            up: (0.0, 1.0, 0.0),
            velocity: (0.0, 0.0, 0.0),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_audio_system_creation() {
        let audio_system = AudioSystem::new();
        assert!(audio_system.is_ok());
    }

    #[test]
    fn test_volume_controls() {
        let mut audio_system = AudioSystem::new().unwrap();
        
        audio_system.set_master_volume(0.5);
        assert_eq!(audio_system.get_master_volume(), 0.5);
        
        audio_system.set_music_volume(0.3);
        assert_eq!(audio_system.get_music_volume(), 0.3);
        
        audio_system.set_sfx_volume(0.8);
        assert_eq!(audio_system.get_sfx_volume(), 0.8);
    }

    #[test]
    fn test_mute_functionality() {
        let mut audio_system = AudioSystem::new().unwrap();
        
        assert!(!audio_system.is_muted());
        
        audio_system.mute();
        assert!(audio_system.is_muted());
        
        audio_system.unmute();
        assert!(!audio_system.is_muted());
        
        audio_system.toggle_mute();
        assert!(audio_system.is_muted());
    }

    #[test]
    fn test_playing_sound_update() {
        let clip = AudioClip {
            file_path: "test.wav".to_string(),
            duration: 2.0,
            sample_rate: 44100,
            channels: 2,
        };

        let mut sound = PlayingSound {
            id: 1,
            clip,
            volume: 1.0,
            pitch: 1.0,
            looping: false,
            position: 0.0,
            finished: false,
        };

        // Update for 1 second
        sound.update(1.0);
        assert_eq!(sound.position, 1.0);
        assert!(!sound.is_finished());

        // Update for another 1.5 seconds (should finish)
        sound.update(1.5);
        assert!(sound.is_finished());
    }
}