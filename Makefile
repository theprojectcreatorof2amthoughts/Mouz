# Mouz project Makefile
# GNU Make + CMake wrapper. CMake remains the source of build configuration.

CMAKE ?= cmake
BUILD_DIR ?= build
BUILD_TYPE ?= Release
PREFIX ?= /usr/local

.PHONY: all configure build test clean rebuild install help

all: build

configure:
	$(CMAKE) -S . -B "$(BUILD_DIR)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMAKE_INSTALL_PREFIX="$(PREFIX)"

build: configure
	$(CMAKE) --build "$(BUILD_DIR)" --config $(BUILD_TYPE) --parallel

test: build
	ctest --test-dir "$(BUILD_DIR)" --build-config $(BUILD_TYPE) --output-on-failure

clean:
	$(CMAKE) -E rm -rf "$(BUILD_DIR)"

rebuild: clean build

install: build
	$(CMAKE) --install "$(BUILD_DIR)" --config $(BUILD_TYPE)

help:
	@echo "Mouz build targets:"
	@echo "  make                 Configure and build Mouz + Mouzi"
	@echo "  make configure       Configure the CMake build directory"
	@echo "  make build           Build all configured targets"
	@echo "  make test            Build and run the CTest regression suite"
	@echo "  make clean           Remove the build directory"
	@echo "  make rebuild         Clean and build from scratch"
	@echo "  make install         Install configured targets"
	@echo ""
	@echo "Overrides: BUILD_DIR=build BUILD_TYPE=Debug CMAKE=cmake PREFIX=/usr/local"
