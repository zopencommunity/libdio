import json
import sys

def json_to_c_string(json_file, header_file):
    with open(json_file, 'r') as f:
        json_data = f.read()

    # Escape necessary characters for C string
    escaped_json = json_data.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')

    with open(header_file, 'w') as f:
        f.write('#ifndef DEFAULT_JSON_H\n')
        f.write('#define DEFAULT_JSON_H\n\n')
        f.write('const char *build_default_json =\n')
        f.write('    "{}";\n\n'.format(escaped_json))
        f.write('#endif // DEFAULT_JSON_H\n')

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: json_to_c.py <input_json_file> <output_header_file>")
        sys.exit(1)
    json_to_c_string(sys.argv[1], sys.argv[2])

