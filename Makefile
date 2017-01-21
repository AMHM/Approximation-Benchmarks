all: make_directories streamcluster

make_directories:
	mkdir -p "bin/arm"
	mkdir -p "bin/x86"

streamcluster: 
	$(MAKE) -C src/streamcluster/ MAKEFLAGS=all
	
clean:
	rm -rf bin