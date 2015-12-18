all: generate_just_busted_hands main
	./generate_just_busted_hands > just_busted_hands
	./main
generate_just_busted_hands:
	g++ generate_just_busted_hands.cpp -o generate_just_busted_hands
main:
	g++ main.cpp -o main
debug: generate_just_busted_hands main #sudo apt-get install valgrind
	valgrind ./generate_just_busted_hands > just_busted_hands
	valgrind ./main
clean:
	rm generate_just_busted_hands main just_busted_hands
