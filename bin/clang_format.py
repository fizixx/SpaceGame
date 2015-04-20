
import os
import subprocess

CLANG_FORMAT_EXECUTABLE = 'clang-format'

def enum_dir(path):
  extensions = ('.h', '.cpp', '.mm',)

  files = []
  directories = []

  for name in os.listdir(path):
    if name[0] == '.':
      continue

    full_name = os.path.join(path, name)

    if os.path.isdir(full_name):
      directories.append(full_name)
      continue

    if not os.path.splitext(name)[1] in extensions:
      continue

    files.append(full_name)

  for p in directories:
    files.extend(enum_dir(p))

  return files

def clang_format(path):
  output = subprocess.check_output([CLANG_FORMAT_EXECUTABLE, path])
  with open(path, 'wb') as file:
    file.write(output)

if __name__ == '__main__':
  map(clang_format, enum_dir(os.getcwd()))
