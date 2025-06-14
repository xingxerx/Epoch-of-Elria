struct MetaCognition {
    past_decisions: Vec<String>,
    evaluation_scores: Vec<f64>,
}

impl MetaCognition {
    fn analyze(&self) -> Option<String> {
        self.past_decisions.last().map(|decision| format!("Reflecting on: {}", decision))
    }
}