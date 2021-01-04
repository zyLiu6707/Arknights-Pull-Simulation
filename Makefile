CXX = g++

CFLAGS = -std=c++11 -O3 -pedantic -Wall -Werror

OBJS = simulation_sequential.o probability_wrapper.o

TARGETS = simulation_sequential

$(TARGETS): $(OBJS)
	$(CXX) -o $@ $(OBJS)

simulation_sequential.o: simulation_sequential.cpp utils.h error_flag.h
	$(CXX) -c $< $(CFLAGS)

probability_wrapper.o: probability_wrapper.cpp probability_wrapper.h
	$(CXX) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(OBJS) $(TARGETS)