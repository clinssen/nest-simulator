#! /usr/bin/env bash

# With this script you can easily format all C/C++ files contained in
# any (sub)directory of NEST. Internally it uses clang-format to do
# the actual formatting. You can give a different clang-format command,
# e.g. by executing `CLANG_FORMAT=clang-format-3.6 ./format_all_c_c++_files.sh`.
# By default the script starts at the current working directory ($PWD), but
# supply a different starting directory as the first argument to the command.
CLANG_FORMAT=/home/archels/julich/clang-format/clang+llvm-3.6.2-x86_64-linux-gnu-ubuntu-14.04/bin/clang-format
CLANG_FORMAT_FILE=/home/archels/julich/nest-simulator-fork-/nest-simulator/.clang-format

# Drop files that should not be checked
FILES_TO_IGNORE="libnestutil/compose.hpp librandom/knuthlfg.h librandom/knuthlfg.cpp"


# Recursively process all C/C++ files in all sub-directories.
function process_dir {
  dir=$1
  echo "Process directory: $dir"
  for f in $dir/*; do
    if [[ -d $f ]]; then
      # Recursively process sub-directories.
      process_dir $f
    else
      echo "Process file: $f"
      ignore_file=0

      for FILE_TO_IGNORE in $FILES_TO_IGNORE; do
        if [[ $f == *$FILE_TO_IGNORE* ]]; then
          ignore_file=1
          break
        fi
      done

      if [ $ignore_file == 1 ] ; then
        continue
      fi

      case $f in
        *pynestkernel.cpp )
          # Ignore very large generated files.
          ;;
        *.cpp | *.cc | *.c | *.h | *.hpp )
          # Format C/C++ files.
          echo " - Format C/C++ file: $f"
          $CLANG_FORMAT -i $f
          ;;
        * )
          # Ignore all other files.
      esac
    fi
  done
}

function help_output {
  echo "The $CLANG_FORMAT_FILE requires clang-format version 3.6 or later."
  echo "Use like: [CLANG_FORMAT=<clang-format-3.6>] ./extras/`basename $0` [start folder, defaults to '$PWD']"
  exit 0
}

if [[ ! -f $CLANG_FORMAT_FILE ]]; then
  echo "Cannot find $CLANG_FORMAT_FILE file. Please start '`basename $0`' from the NEST base source directory."
  help_output
fi

if [[ $# -eq 0 ]]; then
  # Start with current directory.
  startdir=$PWD
elif [[ $# -eq 1 ]]; then
  if [[ -d $1 ]]; then
    # Start with given directory.
    startdir=$1
  else
    # Not a directory.
    help_output
  fi
else
  # Two or more arguments...
  help_output
fi

# Start formatting the $startdir and all subdirectories
process_dir $startdir
