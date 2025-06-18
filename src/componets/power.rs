use hecs::{Component, Entity}; // Adjust if you're using a different ECS like Legion or Bevy

/// Represents an entity's current power level and maximum capacity.
#[derive(Component, Debug, Clone, Copy)]
pub struct PowerLevel {
    pub current: f32,
    pub max: f32,
}

/// Power generated per second from internal sources (e.g., engine, miniature reactor).
#[derive(Component, Debug, Clone, Copy)]
pub struct PowerGenerationRate(pub f32);

/// Represents various categories of power consumption for an entity.
/// Systems that manage movement, weapons, shields etc. would update these values.
#[derive(Component, Debug, Clone)]
pub struct PowerConsumptionRates {
    pub movement: f32,
    pub weapons: f32,
    pub shields: f32,
    pub utility: f32, // e.g., scanning, cloaking
    // Add more specific consumption categories as needed by your game mechanics
}

/// Marker component for entities that are power consumers and whose power state should be managed by AI.
#[derive(Component, Debug)]
pub struct PowerConsumerAI;

/// Configuration settings for the Power Management AI.
#[derive(Component, Debug, Clone, Copy)]
pub struct PowerManagementAIConfig {
    pub low_threshold_pct: f32,      // e.g., 0.3 (30% of max power)
    pub critical_threshold_pct: f32, // e.g., 0.1 (10% of max power)
    pub offer_power_threshold_pct: f32, // e.g., 0.8 (80% before offering to others)
    pub charge_target_percentage: f32, // e.g., 0.95 (charge up to 95%)
    pub charge_seeking_range: f32,     // How far to look for charging sources
    pub beam_target_seek_range: f32, // How far to look for entities to beam power to
}

/// Represents the current power state of an AI-controlled entity.
#[derive(Component, Debug, Clone, Copy, PartialEq, Eq)]
pub enum CurrentPowerState {
    Optimal,
    Low,
    Critical,
    SeekingCharger, // AI is actively moving towards a charger
    Charging,       // AI is at a charger, actively receiving power
    BeamingPower,   // AI is actively transmitting power
    Idle,           // Default state, perhaps for non-AI entities or when AI has no immediate power concerns
}

/// Component for entities that can transmit wireless power.
#[derive(Component, Debug, Clone, Copy)]
pub struct WirelessPowerTransmitter {
    pub rate: f32,     // Power per second transmitted
    pub range: f32,    // Effective transmission range
    pub is_active: bool, // Whether the transmitter is currently on
    pub target_entity: Option<Entity>, // Which entity (if any) it's actively targeting
}

/// Component for entities that can receive wireless power.
#[derive(Component, Debug, Clone, Copy)]
pub struct WirelessPowerReceiver {
    pub max_rate: f32, // Max power per second it can receive
    pub is_receiving: bool, // Whether the receiver is currently on
    pub active_source_entity: Option<Entity>, // From which entity it's currently receiving
}

/// Component for an "Arc Reactor" like internal power source.
/// This would generate a large amount of power.
#[derive(Component, Debug, Clone, Copy)]
pub struct ArcReactor {
    pub base_output_rate: f32, // Constant power generation
    pub efficiency_multiplier: f32, // Modifies base output (e.g., based on health, upgrades)
    pub startup_cost: f32,     // Power consumed to activate
    pub cooldown_timer: f32,   // Time until it can be reactivated
    pub is_active: bool,
    pub current_cooldown: f32,
}

// You might also have a `Position` or `Transform` component in your engine
// For example:
// #[derive(Component, Debug, Clone, Copy)]
// pub struct Position(pub glam::Vec3); // Assuming glam for vectors
