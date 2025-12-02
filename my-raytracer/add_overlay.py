from PIL import Image, ImageDraw, ImageFont
import os

def add_overlay():
    # Configuration
    input_path = "build-release/piece.ppm"
    output_path = "piece.png"
    
    # TEXT TO DISPLAY - EDIT THIS IF NEEDED
    piece_name = "Cinematic Ray Tracer"
    full_name = "Alex Yuan" 
    course_info = "CSC317 Fall 2025"
    
    overlay_text = f"{piece_name}\n{full_name}\n{course_info}"

    # Check if input exists
    if not os.path.exists(input_path):
        print(f"Error: Could not find {input_path}")
        print("Make sure you have built and run the ray tracer first.")
        return

    try:
        # Open the image
        img = Image.open(input_path)
        draw = ImageDraw.Draw(img)
        
        # Load a font (Try Helvetica on macOS, fall back to default)
        try:
            font_size = 24
            font = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", font_size)
        except IOError:
            print("Helvetica not found, using default font.")
            font = ImageFont.load_default()

        # Calculate text position (Bottom Left with padding)
        padding = 20
        x = padding
        # Estimate text height roughly
        text_height = 100 
        y = img.height - text_height - padding

        # Draw Text with Shadow (for readability on any background)
        shadow_offset = 2
        draw.text((x + shadow_offset, y + shadow_offset), overlay_text, font=font, fill="black")
        draw.text((x, y), overlay_text, font=font, fill="white")

        # Save as PNG
        img.save(output_path)
        print(f"Success! Created {output_path} with overlay.")
        
    except Exception as e:
        print(f"An error occurred: {e}")
        print("You might need to install the Pillow library: pip install Pillow")

if __name__ == "__main__":
    add_overlay()
