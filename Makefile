BUILD := build

clean:
	@rm -rf ${BUILD}

build:
	@mkdir -p build
	@cmake -S . -B ${BUILD}
	@cmake --build ${BUILD}

.PHONY: build clean all
