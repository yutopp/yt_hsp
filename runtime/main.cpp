#include "../include/runtime/cl.hpp"

#include <iostream>

int main() {
	auto r = yt_hsp::make_interpreter<yt_hsp::cl::interpreter<>>( "hsp_test_2.ax" );
	r.run();

	{
		int i;
		std::cin >> i;
	}
}