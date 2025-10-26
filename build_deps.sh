# Building imgui dependency
mkdir -p build/imgui

# Set PKG_CONFIG_PATH to include /usr/local/lib/pkgconfig for SDL3
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH

SDL_FLAGS="$(pkg-config sdl3 --cflags)"

COMMON_FLAGS="-g -O3";

imgui_files=($(find ./vendor/imgui -maxdepth 1 -name "*.cpp" -type f))
imgui_files+=("./vendor/imgui/backends/imgui_impl_sdl3.cpp")
imgui_files+=("./vendor/imgui/backends/imgui_impl_sdlgpu3.cpp")

IMGUI_FLAGS="-Ivendor/imgui"
for f in ${imgui_files[@]}; do
  file_name=$(basename $f)
  object_file_name=${file_name%.*}.o
  object_path=build/imgui/$object_file_name

  imgui_build_files+=($object_path)

  g++ $SDL_FLAGS $IMGUI_FLAGS $COMMON_FLAGS -c $f -o $object_path
done

mkdir -p build/implot

implot_files=($(find ./vendor/implot -maxdepth 1 -name "*.cpp" -type f))
IMPLOT_FLAGS="-Ivendor/imgui"
for f in ${implot_files[@]}; do
  file_name=$(basename $f)
  object_file_name=${file_name%.*}.o
  object_path=build/implot/$object_file_name

  imgui_build_files+=($object_path)

  g++ $IMPLOT_FLAGS $COMMON_FLAGS -c $f -o $object_path
done

