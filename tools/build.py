#!/usr/bin/env python3

from pathlib import Path
import subprocess
import sys

LOAD_ADDR = 0x80005e18

# Compilation folders
ROOT_DIR = Path(__file__).parent.parent
SRC_DIR = Path(ROOT_DIR, 'src')
KAMEK = 'Kamek' # Kamek assumes the program is on PATH!
SYMBOL_MAP = Path(ROOT_DIR, 'externals.x')
# PORT_MAP = Path(ROOT_DIR.parent, 'tools', 'versions-nsmbw.txt')
OUTPUT_DIR = Path(ROOT_DIR, 'build')
CODEWARRIOR = Path(ROOT_DIR.parent, 'tools', 'cw', 'mwcceppc.exe')
INCLUDE_DIR = Path(ROOT_DIR, 'include')
OUTPUT_BIN_DIR = Path(ROOT_DIR, 'build', 'bin')

# Add wine if not on Windows
CC = [] if sys.platform == 'win32' else ['wine']
CC.append(CODEWARRIOR)

# Compilation flags
CFLAGS = [
    '-i', '.',
    '-I-',
    '-i', INCLUDE_DIR,
    '-Cpp_exceptions', 'off',
    '-enum', 'int',
    '-fp', 'fmadd',
    '-gccext', 'on',
    '-msext', 'on',
    '-nostdinc',
    '-O4',
    '-once',
    '-pragma', '"cpp_extensions on"',
    '-pragma', '"cpp1x on"',
    '-pragma', '"gprfloatcopy on"',
    '-pragma', '"no_static_dtors on"',
    '-rostr',
    '-RTTI', 'off',
    '-schedule', 'on',
    '-sdata', '0',
    '-sdata2', '0',
    '-use_lmw_stmw', 'on']

# Linking flags
KAMEK_FLAGS = [
    f'-static={hex(LOAD_ADDR)}',
    # f'-externals={SYMBOL_MAP}',
    # f'-versions={PORT_MAP}',
    f'-output-code={Path(OUTPUT_DIR, "BToadPhysics.bin")}',
    # f'-output-riiv={Path(OUTPUT_DIR, "BToadPhysics.xml")}'
]


# Print helper function so output isn't all over the place when redirecting it
def printFlush(*args, **kwargs):
	kwargs['flush'] = True
	print(*args, **kwargs)


# Make output folder
OUTPUT_BIN_DIR.mkdir(exist_ok=True)

# Look for source files
CPP_FILES = set(SRC_DIR.rglob('*.cpp'))

# Compile each CPP file, exit on failure
O_FILES = []
for fp in CPP_FILES:
    printFlush('Compiling', fp.name, end='...\n')

    # Make its destination path since CW isn't able to for some reason
    destpath = Path(OUTPUT_DIR, fp.relative_to(ROOT_DIR)).with_suffix('.o')
    destpath.parent.mkdir(parents=True, exist_ok=True)

    # Run CW, exit on failure
    out = subprocess.run([*CC, *CFLAGS, '-c', '-o', destpath, fp])
    if out.returncode != 0:
        exit()

    # Else append successful result
    O_FILES.append(destpath)

# Link, exit on failure
if O_FILES:
    out = subprocess.run([KAMEK, *O_FILES, *KAMEK_FLAGS])
    if out.returncode == 0:
        printFlush('Linked successfully!')