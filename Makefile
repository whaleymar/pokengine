.PHONY: all clean

all:
	cmake . -B build && $(MAKE) -C build 

clean:
	rm -r build || true
