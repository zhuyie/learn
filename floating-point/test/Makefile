all:
	$(CC) -o operations operations.c
	$(CC) -o denormalized denormalized.c
	$(CC) -o speed -O2 speed.c
	$(CXX) -o inner_prod -O2 -std=c++11 inner_prod.cc

clean:
	rm -f operations
	rm -f denormalized
	rm -f speed
	rm -f inner_prod
