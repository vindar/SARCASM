import os

def sanitize_variant_name(name):
    return name.replace(".h", "").replace("-", "_").replace(".", "_")

def main():
    variant_dir = input("Enter the path to the voices variant folder (e.g. data/voices/!v/): ").strip()
    output_file = input("Enter the path to the output .h file (e.g. all_variants.h): ").strip()

    if not os.path.isdir(variant_dir):
        print("❌ Variant directory not found.")
        return

    variant_files = [f for f in os.listdir(variant_dir) if f.endswith(".h")]
    variant_files.sort()  # optional: sort alphabetically

    with open(output_file, "w") as out:
        out.write("/**\n")
        out.write("* Add all the voices variants (does not take much space)\n")
        out.write("**/\n")
        out.write("#include <Arduino.h> // for PROGMEM\n\n")
        out.write("// the namespace will prevent a linker error if user reloads a variant.\n")
        out.write("namespace espeak_all_variants\n")
        out.write("{\n\n")

        for fname in variant_files:
            out.write(f'#include "data/voices/!v/{fname}"\n')

        out.write("\n} // namespace\n\n")
        out.write('#include "speak_lib.h"\n\n')
        out.write('#define ESPEAK_REGISTER_VARIANT(NAME) ')
        out.write('{ espeak_RegisterVoiceVariant( #NAME , ')
        out.write('espeak_all_variants::espeak_ng_data_voices__v_##NAME , ')
        out.write('espeak_all_variants::espeak_ng_data_voices__v_##NAME##_len); }\n\n')

        out.write("void espeak_RegisterAllVariants()\n{\n")

        for fname in variant_files:
            varname = sanitize_variant_name(fname)
            out.write(f"    ESPEAK_REGISTER_VARIANT({varname})\n")

        out.write("}\n\n")
        out.write("/** end of file */\n")

    print(f"✅ Variant registration header generated: {output_file}")

if __name__ == "__main__":
    main()
