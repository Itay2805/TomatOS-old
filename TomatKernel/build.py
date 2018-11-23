import os
import subprocess
import json

try:
  from pathlib import Path
except ImportError:
  from pathlib2 import Path  # python 2 backport


# Some params
GCC_FLAGS="-g -fdiagnostics-color=always -Wall -Wextra -I. -I../TomatLibc -std=gnu99 -nostdinc -fno-builtin -fno-stack-protector -march=i386 -m32"
NASM_FLAGS=""
LD_FLAGS="-melf_i386"

OUTPUT_DIRECTORY="./build/"

file_changes = {}
file_changes_to_add = {}

# Load the file changes
if os.path.isfile("./build/file_changes.json"):
    with open("./build/file_changes.json", "rb") as f:
        file_changes = json.loads(f.read())

def file_changed(filename, add_if_not_found=True):
    """
    Checks if a file has changed
    """
    if filename in file_changes:
        return os.path.getmtime(filename) > file_changes[filename]
    else:
        if add_if_not_found:
            file_changes_to_add[filename] = os.path.getmtime(filename)
        return True

def header_changed(cfile):
    """
    check if headers included in a C file have changed
    """
    p = None
    try:
        p = subprocess.Popen(['bash', '-c', 'gcc {} -H -fsyntax-only -c {}'.format(GCC_FLAGS, cfile)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    except subprocess.CalledProcessError:
        return False, True
    else:
        out = p.stderr.read()
        for header_file in out.splitlines():
            if header_file.find('. ') != -1:
                header_file = header_file.lstrip('.').lstrip()
                if file_changed(header_file):
                    print "{} has changed, rebuilding {}".format(header_file, cfile)
                    return True, False
        return False, False

"ld $LDPARAMS -T link.ld -o kernel.debug.elf gdt.o kernel.o"

# Create the build directory
Path("./build").mkdir(exist_ok=True)

build_errors = False

# Iterate all files
for root, dirs, files in os.walk("./"):
    for file in files:
        file = os.path.join(root, file)
        output = "{}.o".format(file)
        if file.endswith(".c"):
            changed, error = header_changed(file)
            if error:
                print "Failed to check headers in file {}".format(file)
            elif not os.path.isfile(output) or changed or file_changed(file, False):
                # Rebuild under the following conditions
                # * Output file not found
                # * Input file changed
                # * Included header have changed
                print "{} -> {}".format(file, output)
                Path(os.path.dirname(os.path.join(OUTPUT_DIRECTORY, output))).mkdir(parents=True, exist_ok=True)
                command = "gcc {} -c {} -o {}".format(GCC_FLAGS, file, os.path.join(OUTPUT_DIRECTORY, output))
                try:
                    subprocess.check_output(["bash", "-c", command])
                except subprocess.CalledProcessError:
                    build_errors = True
                else:
                    file_changes_to_add[file] = os.path.getmtime(file)
            else:
                print "{} -> skipped"
        elif file.endswith(".asm"):
            if not os.path.isfile(output) or file_changed(file, False):
                # Rebuild under the following conditions
                # * Output file not found
                # * Input file changed
                print "{} -> {}".format(file, output)
            else:
                print "{} -> skipped"

if build_errors:
    print "Got build errors! Not linking..."
else:
    # Link!
    object_files = []
    for root, dirs, files in os.walk("./"):
        for file in files:
            if file.endswith(".o"):
                object_files.append(os.path.join(root, file))
    command = "ld {} -T link.ld -o kernel.elf {}".format(LD_FLAGS, ' '.join(object_files))
    try:
        subprocess.check_output(["bash", "-c", command])
    except subprocess.CalledProcessError:
        print "Linking failed!"
    else:
        print "Linking finished successfully!"

# Update the file changes table
for key in file_changes_to_add:
    file_changes[key] = file_changes_to_add[key]

# Save the file changes dictionary
with open("./build/file_changes.json", "wb") as f:
    f.write(json.dumps(file_changes))