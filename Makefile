all: make_directories blackscholes scale sobel streamcluster susan  

make_directories:
	mkdir -p "bin/arm"
	mkdir -p "bin/x86"
	
blackscholes:
	$(MAKE) -C src/blackscholes/ MAKEFLAGS=all
	
scale: 
	$(MAKE) -C src/scale/ MAKEFLAGS=all
	
sobel:
	$(MAKE) -C src/sobel/ MAKEFLAGS=all
	
streamcluster: 
	$(MAKE) -C src/streamcluster/ MAKEFLAGS=all
	
susan: 
	$(MAKE) -C src/susan/ MAKEFLAGS=all
	
clean:
	rm -rf bin