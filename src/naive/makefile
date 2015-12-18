main: generate_unbusted_hands
	g++ main.cpp -o main
generate_unbusted_hands:
	g++ generate_unbusted_hands.cpp -o generate_unbusted_hands
run: main
	./generate_unbusted_hands > unbusted_hands
	./main
clean:
	rm generate_unbusted_hands main unbusted_hands
