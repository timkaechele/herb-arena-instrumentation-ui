# Ensure build directory exists
mkdir -p build

# SDL3 flags (hardcoded since pkg-config has issues)
SDL_FLAGS="-I/usr/local/include/SDL3"
SDL_LIBS="-L/usr/local/lib -lSDL3 -pthread -lm"
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

# Compile ImGui core files
imgui_files="./vendor/imgui/imgui.cpp ./vendor/imgui/imgui_draw.cpp ./vendor/imgui/imgui_widgets.cpp ./vendor/imgui/imgui_tables.cpp ./vendor/imgui/backends/imgui_impl_sdl3.cpp ./vendor/imgui/backends/imgui_impl_sdlgpu3.cpp"

for f in $imgui_files; do
  file_name=$(basename $f)
  object_file_name=${file_name%.*}.o
  object_path=build/imgui/$object_file_name
  mkdir -p build/imgui

  g++ $IMGUI_FLAGS $SDL_FLAGS $COMMON_FLAGS -c $f -o $object_path
done

object_files=$(find ./build -name *.o -not -name "*_test.o")
mkdir -p bin
g++ $COMMON_FLAGS $IMGUI_FLAGS $SDL_FLAGS $object_files $SDL_LIBS -o bin/arena_instrumentation