// Event handling example
print("Setting up event handlers...");

onClick("submitBtn") {
    print("Submit button clicked!");
    let message = "Form submitted successfully!";
    print(message);
}

onClick("cancelBtn") {
    print("Cancel button clicked!");
    print("Operation cancelled.");
}

print("Event handlers registered.");
print("Use interactive mode to trigger events:");
print("  trigger submitBtn");
print("  trigger cancelBtn");