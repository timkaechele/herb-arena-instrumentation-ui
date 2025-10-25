# Ensure build directory exists
mkdir -p build

# Building tradergui
SDL_FLAGS=$(pkg-config sdl3 --cflags)
SDL_LIBS=$(pkg-config sdl3 --libs)
IMGUI_FLAGS="-Ivendor/imgui -Ivendor/imgui/backends -Ivendor/implot"
COMMON_FLAGS='-g -std=c++11 -O3'

if [ -f compile_flags.txt ]; then
  rm compile_flags.txt
fi

for flag in $COMMON_FLAGS $SDL_FLAGS $IMGUI_FLAGS; do
  echo "$flag" >> ./compile_flags.txt
done

files=$(find ./src -name *.cpp -type f);

for f in $files; do
  file_name=$(basename $f)
  object_file_name=${file_name%.*}.o
  object_path=build/$object_file_name

  g++ $IMGUI_FLAGS $SDL_FLAGS $COMMON_FLAGS -c $f -o $object_path
done

object_files=$(find ./build -name *.o -not -name "*_test.o")
mkdir -p bin
g++ $COMON_FLAGS $IMGUI_FLAGS $SDL_FLAGS $SDL_LIBS $object_files -o bin/arena_instrumentation
