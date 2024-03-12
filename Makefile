run: compile
	./build/Debug/quantity_concepts
compile:
	conan install . -s build_type=Debug
	cmake --preset conan-default
	cmake --build build --config Debug
