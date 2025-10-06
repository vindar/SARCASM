import os

def make_valid_c_identifier(path):
    # remplace les caractères interdits pour un identifiant C
    return path.replace(os.sep, "_").replace("-", "_").replace(".", "_").replace("!", "_").replace(" ", "_")

def process_file(input_path, output_path, rel_path_no_ext):
    with open(input_path, "rb") as f:
        data = f.read()

    varname = f"espeak_ng_data_{make_valid_c_identifier(rel_path_no_ext)}"

    with open(output_path, "w") as out:
        out.write(f"const unsigned char {varname}[] PROGMEM = {{\n")

        for i, byte in enumerate(data):
            if i % 12 == 0:
                out.write("    ")
            out.write(f"0x{byte:02X}, ")
            if (i + 1) % 12 == 0:
                out.write("\n")

        out.write("\n};\n")
        out.write(f"const unsigned int {varname}_len = {len(data)};\n")

def main():
    src_dir = input("Enter the source directory: ").strip()
    dst_dir = input("Enter the destination directory: ").strip()

    if not os.path.isdir(src_dir):
        print("❌ Source directory not found.")
        return

    for root, dirs, files in os.walk(src_dir):
        for fname in files:
            input_path = os.path.join(root, fname)

            rel_path = os.path.relpath(input_path, src_dir)
            rel_path_no_ext = os.path.splitext(rel_path)[0]
            dst_subdir = os.path.dirname(rel_path)

            dst_filename = os.path.splitext(os.path.basename(fname))[0] + ".h"
            dst_path_dir = os.path.join(dst_dir, dst_subdir)
            os.makedirs(dst_path_dir, exist_ok=True)
            dst_path = os.path.join(dst_path_dir, dst_filename)

            print(f"📄 Processing: {input_path} → {dst_path}")
            process_file(input_path, dst_path, rel_path_no_ext)

    print("✅ All files converted.")

if __name__ == "__main__":
    main()
