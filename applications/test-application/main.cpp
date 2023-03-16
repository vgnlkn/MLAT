#include <matrix.h>
#include <vector.h>
#include <iostream>

int main()
{
	std::cout << "Hello world" << std::endl;
	OurMatrix<3, 4> m;
	std::cout << m << std::endl;
	std::cout << std::endl;

	OurMatrix<3, 4> n;
	std::cout <<n << std::endl;
	std::cout << std::endl;

	OurMatrix<3, 4> k = m + n;
	std::cout << k << std::endl;
	std::cout << std::endl;
	std::cout << k * 5 << std::endl;
	std::cout << std::endl;

	std::cout << 10*k*5 << std::endl;
	std::cout << std::endl;

	OurVector<4> vec;
	for (int i = 0; i < 4; ++i)
		vec[i] = i;
	std::cout << "VEC_1: " << vec << std::endl;

	

	auto vec2 = k * vec;
	std::cout << "k * VEC_1: " << vec2 << std::endl;
	std::cout << std::endl;
	OurVector<3> vec_3;
	for (int i = 0; i < 3; ++i)
		vec_3[i] = i;
	auto vec4 = vec_3 * k;
	std::cout << std::endl;

	std::cout << k << std::endl;
	std::cout << std::endl;

	std::cout << "VEC_3: " << vec_3 << std::endl;
	std::cout << std::endl;
	std::cout << "VECC: " << vec4 << std::endl;
	std::cout << std::endl;
	return 0;
}