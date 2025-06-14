fn main() {
    let result = risky_function();
    match result {
        Ok(value) => println!("Success: {:?}", value),
        Err(e) => println!("Gemini Suggests Fixing: {:?}", e),
    }
}