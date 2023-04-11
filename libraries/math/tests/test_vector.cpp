#include <iostream>
#include <gtest/gtest.h>
#include <vector.h>


TEST(VectorTests, SimpleArithmetic)
{
	// init
	OurVector<3> a, b;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 2) * 10.0f;
		b[i] = (i + 3) * 17.0f;
	}
	
	// copy constructor & operator ==
	OurVector<3> a_copy = a;
	OurVector<3> b_copy = b;
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(a_copy[i], a[i]);
		ASSERT_EQ(b_copy[i], b[i]);
	}
	ASSERT_TRUE(a_copy == a);
	ASSERT_TRUE(b_copy == b);

	// operator +
	OurVector<3> c = a + b;
	ASSERT_TRUE(a_copy == a);
	ASSERT_TRUE(b_copy == b);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(c[i], a[i] + b[i]);
	}

	// zero init
	OurVector<3> d;
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], 0.f);
	}
	
	// operator - and =
	d = a - b;
	ASSERT_TRUE(a_copy == a);
	ASSERT_TRUE(b_copy == b);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], a[i] - b[i]);
	}

	// operator *
	d = a * b;
	ASSERT_TRUE(a_copy == a);
	ASSERT_TRUE(b_copy == b);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], a[i] * b[i]);
	}
	
	// operator !=
	ASSERT_TRUE(a != b);

	// const and vector product
	d = 5 * a;
	ASSERT_TRUE(a_copy == a);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], 5 * a[i]);
	}

	// vector and const product
	d = a * 19;
	ASSERT_TRUE(a_copy == a);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], a[i] * 19);
	}
	
	// setValue
	d.setValue(3.f);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(d[i], 3.f);
	}

	// swap
	a.swap(b);
	ASSERT_TRUE(a == b_copy);
	ASSERT_TRUE(b == a_copy);
	a = -a;
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(a[i], (-1) * b_copy[i]);
	}
}

TEST(VectorTests, MultiSum)
{
	OurVector<3> a, b, c, d, res;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 1) * 10.0f;
		b[i] = (i + 2) * 10.0f;
		c[i] = (i + 3) * 10.0f;
		d[i] = (i + 3) * 10.0f;
	}
	OurVector<3> a_copy, b_copy, c_copy, d_copy;
	a_copy = a;
	b_copy = b;
	c_copy = c;
	d_copy = d;	

	res = a + b + c + d;
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], a[i] + b[i] + c[i] + d[i]);
	}
}

TEST(VectorTests, MultiProduct)
{
	OurVector<3> a, b, c, d, res;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 1) * 10.0f;
		b[i] = (i + 2) * 10.0f;
		c[i] = (i + 3) * 10.0f;
		d[i] = (i + 3) * 10.0f;
	}
	OurVector<3> a_copy, b_copy, c_copy, d_copy;
	a_copy = a;
	b_copy = b;
	c_copy = c;
	d_copy = d;

	res = a * b * c * d;
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], a[i] * b[i] * c[i] * d[i]);
	}
}

TEST(VectorTests, AdvancedArithmetic_1)
{
	OurVector<3> a, b, c, d, res;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 1) * 10.0f;
		b[i] = (i + 2) * 10.0f;
		c[i] = (i + 3) * 10.0f;
		d[i] = (i + 4) * 10.0f;
	}

	OurVector<3> a_copy, b_copy, c_copy, d_copy;
	a_copy = a;
	b_copy = b;
	c_copy = c;
	d_copy = d;

	res = a + b * c + d;
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], a[i] + b[i] * c[i] + d[i]);
	}
}

TEST(VectorTests, AdvancedArithmetic_2)
{
	OurVector<3> a, b, c, d, res;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 1) * 10.0f;
		b[i] = (i + 2) * 10.0f;
		c[i] = (i + 3) * 10.0f;
		d[i] = (i + 4) * 10.0f;
	}

	OurVector<3> a_copy, b_copy, c_copy, d_copy;
	a_copy = a;
	b_copy = b;
	c_copy = c;
	d_copy = d;

	res = a * b * (d - c);
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], a[i] * b[i] * (d[i] - c[i]));
	}
}

TEST(VectorTests, AdvancedArithmetic_3)
{
	OurVector<3> a, b, c, d, res;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = (i + 1) * 10.0f;
		b[i] = (i + 2) * 10.0f;
		c[i] = (i + 3) * 10.0f;
		d[i] = (i + 4) * 10.0f;
	}

	OurVector<3> a_copy, b_copy, c_copy, d_copy;
	a_copy = a;
	b_copy = b;
	c_copy = c;
	d_copy = d;

	res = -a * b * (d - c);
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], ((-1) * a[i]) * b[i] * (d[i] - c[i]));
	}
	res = -(a * b * c * d + (a + b + c + d));
	ASSERT_TRUE(a == a_copy);
	ASSERT_TRUE(b == b_copy);
	ASSERT_TRUE(c == c_copy);
	ASSERT_TRUE(d == d_copy);
	for (int i = 0; i < 3; ++i)
	{
		ASSERT_EQ(res[i], (-1) * (a[i] * b[i] * c[i] * d[i] + (a[i] + b[i] + c[i] + d[i])));
	}
}