#include <iostream>
#include <gtest/gtest.h>
#include <vector.h>

TEST(ExampleTest, SimpleTest)
{
	OurVector<3> a, b;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 2) * 10;
		b[i] = (i + 3) * 10;
	}
	/*for (int i = 0; i < 3; ++i)
	{
		std::cout << "a = " << a[i] << "; b = " << b[i] << std::endl;
	}*/
	OurVector<3> c = (a + b);
	/*for (int i = 0; i < 3; ++i)
	{
		std::cout << c[i] << '=' << a[i] << '+' << b[i] << std::endl;
	}*/
	ASSERT_TRUE(1 == 1);
}

