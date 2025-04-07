# scripts/chat.py

from transformers import pipeline
import readline  # for arrow key history in terminal

# Load the model from local trained folder
model_path = "models"
classifier = pipeline("text-classification", model=model_path)

print("📊 Event Classifier Chat Interface")
print("Type your event descriptions and I'll classify them.")
print("Type 'exit' or 'quit' to end the chat.\n")

while True:
    user_input = input("🧾 You: ").strip()
    
    if user_input.lower() in ["exit", "quit"]:
        print("👋 Goodbye!")
        break

    try:
        result = classifier(user_input)[0]
        label = result["label"]
        score = result["score"]
        print(f"🤖 Model: {label} ({score:.2f} confidence)\n")
    except Exception as e:
        print(f"⚠️ Error: {str(e)}\n")
