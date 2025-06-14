struct MetaCognition {
    past_decisions: Vec<String>,
    evaluation_scores: Vec<f64>,
}

impl MetaCognition {
    /// Returns a reflection string on the last decision, if any.
    fn analyze(&self) -> Option<String> {
        self.past_decisions
            .last()
            .map(|decision| format!("Reflecting on: {}", decision))
    }
}

fn main() {
    let cog = MetaCognition {
        past_decisions: vec![
            "Chose to moderate strictly".to_string(),
            "Allowed ambiguous message".to_string(),
        ],
        evaluation_scores: vec![0.6, 0.8],
    };

    match cog.analyze() {
        Some(reflection) => println!("{}", reflection),
        None => println!("No decisions to reflect upon."),
    }
}