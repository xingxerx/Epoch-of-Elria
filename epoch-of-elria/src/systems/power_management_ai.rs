use hecs::{World, Entity, Query}; // Adjust for your ECS
use glam::Vec3; // Assuming glam for vectors

// You'll need access to `DeltaTime` from your main game loop.
// For example, if you store it as a resource in hecs or pass it explicitly.
// For simplicity here, we'll assume a `delta_time` parameter is passed.

// --- System 1: Update Power Levels ---
/// System to calculate power changes based on generation, consumption, and wireless transfer.
pub fn update_power_levels_system(world: &mut World, delta_time: f32) {
    for (_, (mut power_level, gen_rate, cons_rates, receiver_opt, transmitter_opt)) in world.query::<(
        &mut PowerLevel,
        &PowerGenerationRate,
        &PowerConsumptionRates,
        Option<&WirelessPowerReceiver>,
        Option<&WirelessPowerTransmitter>,
    )>()
    .iter() {
        // Apply internal generation
        power_level.current += gen_rate.0 * delta_time;

        // Apply consumption
        let total_consumption = cons_rates.movement + cons_rates.weapons + cons_rates.shields + cons_rates.utility;
        power_level.current -= total_consumption * delta_time;

        // Apply received wireless power
        if let Some(receiver) = receiver_opt {
            if receiver.is_receiving {
                // In a more complex system, you'd verify the source is active and in range.
                // For now, assume if is_receiving is true, power flows.
                power_level.current += receiver.max_rate * delta_time;
            }
        }

        // Apply transmitted wireless power (deduct from source)
        if let Some(transmitter) = transmitter_opt {
            if transmitter.is_active {
                // Deduct power at the rate it's being transmitted
                power_level.current -= transmitter.rate * delta_time;
            }
        }

        // Clamp power level to min/max
        power_level.current = power_level.current.max(0.0).min(power_level.max);
    }
}

// --- System 2: Manage Arc Reactors (if applicable) ---
pub fn manage_arc_reactor_system(world: &mut World, delta_time: f32) {
    for (_, (mut power_level, mut reactor)) in world.query::<(&mut PowerLevel, &mut ArcReactor)>().iter() {
        // Handle cooldown
        if reactor.current_cooldown > 0.0 {
            reactor.current_cooldown -= delta_time;
            // If reactor becomes active after cooldown, potentially charge startup cost
            if reactor.current_cooldown <= 0.0 && reactor.is_active {
                 // Check if power_level has enough for startup_cost
                 if power_level.current >= reactor.startup_cost {
                     power_level.current -= reactor.startup_cost;
                     println!("Arc Reactor activated for entity {:?}!", reactor); // Debug
                 } else {
                     reactor.is_active = false; // Not enough power to start
                     println!("Not enough power to start Arc Reactor for entity {:?}!", reactor); // Debug
                 }
            }
        }

        if reactor.is_active && reactor.current_cooldown <= 0.0 {
            power_level.current += reactor.base_output_rate * reactor.efficiency_multiplier * delta_time;
        }
    }
}


// --- System 3: Power Management AI Decision System ---
/// This system contains the core AI logic for entities with power needs.
pub fn power_management_ai_system(world: &mut World, delta_time: f32) {
    // Iterate over entities that have AI config and power levels
    for (
        entity,
        (power_level, ai_config, mut current_state, transform_pos_opt, mut receiver_opt, mut transmitter_opt)
    ) in world.query::<(
        &PowerLevel,
        &PowerManagementAIConfig,
        &mut CurrentPowerState,
        Option<&Position>, // Assuming a Position component exists
        Option<&mut WirelessPowerReceiver>,
        Option<&mut WirelessPowerTransmitter>,
    )>()
    .with::<PowerConsumerAI>() // Only query entities that are marked as AI-controlled power consumers
    .iter() {
        let percentage = power_level.current / power_level.max;
        let mut decided_action = None; // What the AI *wants* to do this frame

        // Determine the current power situation
        let new_power_state = if percentage >= ai_config.low_threshold_pct {
            CurrentPowerState::Optimal
        } else if percentage >= ai_config.critical_threshold_pct {
            CurrentPowerState::Low
        } else {
            CurrentPowerState::Critical
        };

        // --- State-based Decision Logic ---
        match *current_state {
            CurrentPowerState::Optimal => {
                if new_power_state != CurrentPowerState::Optimal {
                    *current_state = new_power_state; // Transition to low/critical
                    decided_action = Some("Power dropped to non-optimal. Assessing...");
                } else {
                    // Consider offering power to allies if optimal and has transmitter
                    if percentage > ai_config.offer_power_threshold_pct {
                        if let Some(transmitter) = transmitter_opt.as_mut() {
                            if !transmitter.is_active {
                                // TODO: Logic to find a needy ally within beam_target_seek_range
                                // If found, set transmitter.is_active = true and transmitter.target_entity
                                // For now, just a placeholder print
                                // println!("Entity {:?} is optimal and considering beaming power.", entity);
                            }
                        }
                    }
                }
            }
            CurrentPowerState::Low => {
                if new_power_state == CurrentPowerState::Optimal {
                    *current_state = new_power_state; // Recovered
                    decided_action = Some("Power restored. Resuming normal operations.");
                } else if new_power_state == CurrentPowerState::Critical {
                    *current_state = new_power_state; // Got worse
                    decided_action = Some("Power is critical! Emergency!");
                } else {
                    // Logic to seek charger if not already charging
                    if let Some(receiver) = receiver_opt.as_mut() {
                        if !receiver.is_receiving {
                            if let Some(pos) = transform_pos_opt {
                                if let Some(charger_entity_data) = find_nearby_charger(
                                    world,
                                    pos.0,
                                    ai_config.charge_seeking_range,
                                ) {
                                    // Set AI's movement goal to charger_entity_data.1 (position)
                                    // This would involve adding a `MovementGoal` component or similar
                                    // println!("Entity {:?} seeking charger at {:?}", entity, charger_entity_data.1);
                                    *current_state = CurrentPowerState::SeekingCharger;
                                    receiver.active_source_entity = Some(charger_entity_data.0); // Store target for next step
                                    decided_action = Some("Seeking charger.");
                                } else {
                                    // No charger found, just waiting or exploring
                                    decided_action = Some("Low power, no charger in range. Conserving.");
                                }
                            }
                        }
                    }
                }
            }
            CurrentPowerState::Critical => {
                // Similar to Low, but with higher urgency, possibly disabling all non-essential systems.
                // Could also broadcast a distress call if a communication system exists.
                // Always try to seek charger if possible.
                if new_power_state == CurrentPowerState::Optimal {
                    *current_state = new_power_state;
                    decided_action = Some("Power recovered from critical! Emergency over.");
                } else if new_power_state == CurrentPowerState::Low {
                     *current_state = new_power_state;
                     decided_action = Some("Power still low, but not critical anymore.");
                }
                // ... (add charger seeking logic similar to 'Low')
                decided_action = Some("Critical power! Seeking charger or emergency shutdown.");
            }
            CurrentPowerState::SeekingCharger => {
                if let Some(receiver) = receiver_opt.as_mut() {
                    // Check if we've reached the charger's range
                    if let Some(source_entity) = receiver.active_source_entity {
                        if let (Some(own_pos), Some(source_pos)) = (
                            transform_pos_opt.map(|p| p.0),
                            world.get::<Position>(source_entity).ok().map(|p| p.0),
                        ) {
                            if own_pos.distance(source_pos) <= ai_config.charge_seeking_range {
                                // We are in range, activate charging
                                receiver.is_receiving = true;
                                *current_state = CurrentPowerState::Charging;
                                decided_action = Some("Reached charger, initiating charge.");
                            } else {
                                // Still moving towards charger
                                decided_action = Some("Moving to charger...");
                            }
                        } else {
                            // Target charger disappeared or invalid, revert to seeking
                            receiver.active_source_entity = None;
                            *current_state = CurrentPowerState::Low;
                            decided_action = Some("Charger vanished, reverting to seeking.");
                        }
                    } else {
                        // Lost track of charger, revert to seeking
                        *current_state = CurrentPowerState::Low;
                        decided_action = Some("Lost charger target, reverting to seeking.");
                    }
                } else {
                    // AI has no receiver, shouldn't be in this state, revert
                    *current_state = new_power_state; // Based on current power percentage
                    decided_action = Some("Invalid state (SeekingCharger without receiver), re-evaluating.");
                }
            }
            CurrentPowerState::Charging => {
                if let Some(receiver) = receiver_opt.as_mut() {
                    if percentage >= ai_config.charge_target_percentage {
                        receiver.is_receiving = false; // Stop receiving
                        receiver.active_source_entity = None;
                        *current_state = CurrentPowerState::Optimal; // Or whatever is next
                        decided_action = Some("Charging complete!");
                    } else if !receiver.is_receiving {
                        // If somehow stopped receiving while in Charging state, reactivate
                        receiver.is_receiving = true;
                        decided_action = Some("Reactivating charge.");
                    } else {
                        decided_action = Some("Actively charging.");
                    }
                } else {
                    // AI has no receiver, shouldn't be in this state, revert
                    *current_state = new_power_state;
                    decided_action = Some("Invalid state (Charging without receiver), re-evaluating.");
                }
            }
            CurrentPowerState::BeamingPower => {
                if let Some(transmitter) = transmitter_opt.as_mut() {
                    if !transmitter.is_active || transmitter.target_entity.is_none() {
                        // No longer beaming, revert
                        *current_state = CurrentPowerState::Optimal;
                        decided_action = Some("No longer beaming power, reverting to optimal.");
                    } else {
                        // Check if target still needs power, if so, continue
                        // let target_needs_power = world.get::<PowerLevel>(transmitter.target_entity.unwrap()).map_or(false, |p| p.current < p.max * ai_config.charge_target_percentage);
                        // if !target_needs_power {
                        //      transmitter.is_active = false;
                        //      transmitter.target_entity = None;
                        //      *current_state = CurrentPowerState::Optimal;
                        //      decided_action = Some("Target fully charged, stopping beam.");
                        // } else {
                             decided_action = Some("Beaming power to target.");
                        // }
                    }
                } else {
                    *current_state = new_power_state;
                    decided_action = Some("Invalid state (BeamingPower without transmitter), re-evaluating.");
                }
            }
            CurrentPowerState::Idle => {
                // If it's idle, just check current power status and transition if needed
                if new_power_state != CurrentPowerState::Optimal {
                    *current_state = new_power_state;
                    decided_action = Some("Awakening from idle due to power change.");
                }
            }
        }

        // --- Debugging print for AI decisions ---
        if let Some(action) = decided_action {
            // This assumes you have some way to log to console or in-game UI
            // println!("[PowerAI {:?}] State: {:?} ({:.2}%) - {}", entity, *current_state, percentage * 100.0, action);
        }
    }
}


// --- Helper function (can be a private fn in your module) ---
// Finds the nearest charger that has a WirelessPowerTransmitter component
// and optionally a Position component.
fn find_nearby_charger(
    world: &World,
    ai_pos: Vec3,
    range: f32,
) -> Option<(Entity, Vec3)> { // Returns (Entity ID, Position)
    let mut closest_charger = None;
    let mut min_dist_sq = range * range; // Compare squared distances to avoid sqrt

    for (entity, (charger_transmitter, charger_pos)) in world.query::<(&WirelessPowerTransmitter, &Position)>().iter() {
        if charger_transmitter.is_active { // Only consider active chargers
            let dist_sq = ai_pos.distance_squared(charger_pos.0);
            if dist_sq < min_dist_sq {
                min_dist_sq = dist_sq;
                closest_charger = Some((entity, charger_pos.0));
            }
        }
    }
    closest_charger
}

// --- Integration with your main application ---
// In your `main.rs` or `lib.rs` where you set up your game loop:
// You'll need to add these systems to your ECS scheduler.
/*
fn main() {
    let mut world = hecs::World::new();
    // ... spawn entities with PowerLevel, PowerGenerationRate, etc.

    // Example: Spawning an entity with Power AI
    world.spawn((
        Position(Vec3::new(0.0, 0.0, 0.0)),
        PowerLevel { current: 50.0, max: 100.0 },
        PowerGenerationRate(1.0), // Generates 1 power/sec
        PowerConsumptionRates { movement: 0.5, weapons: 0.0, shields: 0.0, utility: 0.0 },
        WirelessPowerReceiver { max_rate: 10.0, is_receiving: false, active_source_entity: None },
        PowerManagementAIConfig {
            low_threshold_pct: 0.3,
            critical_threshold_pct: 0.1,
            offer_power_threshold_pct: 0.8,
            charge_target_percentage: 0.95,
            charge_seeking_range: 20.0, // Look for chargers within 20 units
            beam_target_seek_range: 50.0,
        },
        CurrentPowerState::Optimal,
        PowerConsumerAI, // Mark this entity as one that the AI manages power for
    ));

    // Example: Spawning a charging station
    world.spawn((
        Position(Vec3::new(15.0, 0.0, 0.0)),
        WirelessPowerTransmitter { rate: 10.0, range: 15.0, is_active: true, target_entity: None },
        // Could also have its own PowerLevel if it has limited charge
    ));


    // Your game loop would then run these systems:
    // loop {
    //     let delta_time = ... // Get delta time from your engine
    //     update_power_levels_system(&mut world, delta_time);
    //     manage_arc_reactor_system(&mut world, delta_time); // If applicable
    //     power_management_ai_system(&mut world, delta_time);
    //     // ... other game systems (rendering, physics, input)
    // }
}
*/
