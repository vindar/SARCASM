import os

def main():
    input_path = input("Enter the path to the binary file: ").strip()
    if not os.path.isfile(input_path):
        print("❌ File not found.")
        return

    with open(input_path, "rb") as f:
        data = f.read()

    # Base name without extension
    base_name = os.path.splitext(os.path.basename(input_path))[0]
    var_name = f"espeak_ng_data_{base_name}"
    output_name = f"{base_name}.h"

    with open(output_name, "w") as out:
        out.write(f"// Automatically generated from {input_path}\n")
        out.write(f"const unsigned char {var_name}[] PROGMEM = {{\n")

        for i, byte in enumerate(data):
            if i % 12 == 0:
                out.write("    ")
            out.write(f"0x{byte:02X}, ")
            if (i + 1) % 12 == 0:
                out.write("\n")

        out.write("\n};\n")
        out.write(f"const unsigned int {var_name}_len = {len(data)};\n")

    print(f"✅ Header generated: {output_name} with variable: {var_name}")

if __name__ == "__main__":
    main()
