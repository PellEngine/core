# Build shaders
echo "Building shaders..."
echo ""
bazel build ///shaders:build_all

# Copy compiled shaders to results folder
rm -rf ./build/shaders
mkdir -p ./build/shaders

echo ""
echo "Copying build products"
echo ""

find ./bazel-bin/shaders -name '*.spv' -exec cp -prv '{}' './build/shaders/' ';'