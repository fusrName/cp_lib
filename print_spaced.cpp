template<typename T>
void print_spaced(T first, T last) {
	if (first == last) return;
	std::cout << *first;
	while(++first != last)
		std::cout << ' ' << *first;
	std::cout << std::endl;
}
