#include <iostream>
#include <gtest/gtest.h>
#include <matrix.h>


TEST(MatrixTests, SimpleArithmetic)
{
    // init
    OurMatrix<2, 3> a;
    OurMatrix<3, 2> b;

    float k = 1.f;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
        a[i][j] = k;
        ++k;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            b[i][j] = k;
            ++k;
        }
    }

    // copy constructor & operator ==
    OurMatrix<2, 3> a_copy = a;
    OurMatrix<3, 2> b_copy = b;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(a[i][j], a_copy[i][j]);
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            ASSERT_EQ(b[i][j], b_copy[i][j]);
        }
    }

    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);


    // operator +
    OurMatrix<2, 3> c = a;
    c.setColumn(1, 5);
    c.setRow(0, 3);
    auto c_copy = c;

    OurMatrix<2, 3> d = a + c;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(d[i][j], a[i][j] + c[i][j]);
        }
    }

    ASSERT_TRUE(a_copy == a);
    ASSERT_TRUE(c_copy == c);


    // zero init
    OurMatrix<2, 3> e;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(e[i][j], 0);
        }
    }


    // operator - and =
    e = a - c;
    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(c == c_copy);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(e[i][j], a[i][j] - c[i][j]);
        }
    }


    // operator !=
    ASSERT_TRUE(e != a);

    // operator *
    // a * b
    OurMatrix<2, 2> m1 = a * b;
    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);

    OurMatrix<2, 2, float> result;
    result[0][0] = 58;
    result[0][1] = 64;
    result[1][0] = 139;
    result[1][1] = 154;


    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            ASSERT_EQ(m1[i][j], result[i][j]);
        }
    }


    // b * a

    OurMatrix<3, 3> m2 = b * a;
    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);

    OurMatrix<3, 3> res;

    res[0][0] = 39;
    res[0][1] = 54;
    res[0][2] = 69;
    res[1][0] = 49;
    res[1][1] = 68;
    res[1][2] = 87;
    res[2][0] = 59;
    res[2][1] = 82;
    res[2][2] = 105;


    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(m2[i][j], res[i][j]);
        }
    }


    // square matrix 3x3

    OurMatrix<3, 3> m3, m4;
    k = 1.f;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            m3[i][j] = k;
            ++k;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            m4[i][j] = k;
            ++k;
        }
    }

    OurMatrix<3, 3> m3_copy = m3;
    OurMatrix<3, 3> m4_copy = m4;

    auto r = m3 * m4;

    ASSERT_TRUE(m3 == m3_copy);
    ASSERT_TRUE(m4 == m4_copy);

    OurMatrix<3, 3> ans;
    ans[0][0] = 84;
    ans[0][1] = 90;
    ans[0][2] = 96;
    ans[1][0] = 201;
    ans[1][1] = 216;
    ans[1][2] = 231;
    ans[2][0] = 318;
    ans[2][1] = 342;
    ans[2][2] = 366;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(ans[i][j], r[i][j]);
        }
    }


    // const and matrix product
    m3 = 13 * m4;
    ASSERT_TRUE(m4 == m4_copy);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(m3[i][j], m4[i][j] * 13);
        }
    }

    // matrix and const product
    m3 = m4 * 10;
    ASSERT_TRUE(m4 == m4_copy);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(m3[i][j], m4[i][j] * 10);
        }
    }

    // setValue
    m3.setValue(15.f);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            assert(m3[i][j] == 15);
        }
    }

    // swap
    auto mtx = m3;
    mtx.setRow(0, 6);
    mtx.setColumn(1, 4);

    m3_copy = m3;
    auto mtx_copy = mtx;

    m3.swap(mtx);
    ASSERT_TRUE(m3 == mtx_copy);
    ASSERT_TRUE(mtx == m3_copy);

    // invert with -
    a = -a;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(a[i][j], (-1) * a_copy[i][j]);
        }
    }

    // setValue
    m3.setValue(15.f);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(m3[i][j], 15);
        }
    }
}


TEST(MatrixTests, MultiSum)
{
    OurMatrix<3, 2> a, b, c, d, res;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            a[i][j] = (i + j + 1) * 10.0f;
            b[i][j] = (i + j + 2) * 10.0f;
            c[i][j] = (i + j + 3) * 10.0f;
            d[i][j] = (i + j + 3) * 10.0f;
        }
    }

    OurMatrix<3, 2> a_copy, b_copy, c_copy, d_copy;
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
        for (int j = 0; j < 2; ++j)
        {
            ASSERT_EQ(res[i][j], a[i][j] + b[i][j] + c[i][j] + d[i][j]);
        }
    }
}


TEST(MatrixTests, MultiProduct)
{
    // square matrix
    OurMatrix<3, 3> a, b, c, d, res;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            a[i][j] = (i + j + 1);
            b[i][j] = (i + j + 2);
            c[i][j] = (i + j + 3);
            d[i][j] = (i + j + 4);
        }
    }
    OurMatrix<3, 3> a_copy, b_copy, c_copy, d_copy;
    a_copy = a;
    b_copy = b;
    c_copy = c;
    d_copy = d;

    res = a * b * c * d;

    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);
    ASSERT_TRUE(c == c_copy);
    ASSERT_TRUE(d == d_copy);

    OurMatrix<3, 3> result;

    result[0][0] = 6186;
    result[0][1] = 7392;
    result[0][2] = 8598;
    result[1][0] = 9048;
    result[1][1] = 10812;
    result[1][2] = 12576;
    result[2][0] = 11910;
    result[2][1] = 14232;
    result[2][2] = 16554;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(res[i][j], result[i][j]);
        }
    }

    // different sizes
    OurMatrix<2, 2> m1, m1_copy;
    OurMatrix<2, 3> m2, m2_copy;
    OurMatrix<3, 2> m3, m3_copy;
    OurMatrix<2, 1> m4, m4_copy;
    OurMatrix<2, 1> ans, answer;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            m1[i][j] = (i + j + 1);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            m2[i][j] = (i + j + 2);
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            m3[i][j] = (i + j + 3);
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 1; ++j)
        {
            m4[i][0] = (i + j + 4);
        }
    }

    m1_copy = m1;
    m2_copy = m2;
    m3_copy = m3;
    m4_copy = m4;

    answer[0][0] = 1407;
    answer[1][0] = 2304;

    ans = m1 * m2 * m3 * m4;

    ASSERT_TRUE(m1 == m1_copy);
    ASSERT_TRUE(m2 == m2_copy);
    ASSERT_TRUE(m3 == m3_copy);
    ASSERT_TRUE(m4 == m4_copy);

    ASSERT_TRUE(answer[0][0] == ans[0][0] && answer[1][0] == ans[1][0]);
}

TEST(MatrixTests, AdvancedArithmetic_1)
{
    OurMatrix<3, 3> a, b, c, d, res;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            a[i][j] = (i + j + 1) * 10.0f;
            b[i][j] = (i + j + 2) * 10.0f;
            c[i][j] = (i + j + 3) * 10.0f;
            d[i][j] = (i + j + 4) * 10.0f;
        }
    }

    OurMatrix<3, 3> a_copy, b_copy, c_copy, d_copy;
    a_copy = a;
    b_copy = b;
    c_copy = c;
    d_copy = d;

    OurMatrix<3, 3> multiplication;
    multiplication[0][0] = 3800;
    multiplication[0][1] = 4700;
    multiplication[0][2] = 5600;
    multiplication[1][0] = 5000;
    multiplication[1][1] = 6200;
    multiplication[1][2] = 7400;
    multiplication[2][0] = 6200;
    multiplication[2][1] = 7700;
    multiplication[2][2] = 9200;

    res = a + b * c + d;
    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);
    ASSERT_TRUE(c == c_copy);
    ASSERT_TRUE(d == d_copy);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(res[i][j], a[i][j] + multiplication[i][j] + d[i][j]);
        }
    }
}


TEST(MatrixTests, AdvancedArithmetic_2)
{
    OurMatrix<3, 3> a, b, c, d, res, ans;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            a[i][j] = i + j + 1;
            b[i][j] = i + j + 2;
            c[i][j] = i + j + 3;
            d[i][j] = i + j + 4;
        }
    }

    OurMatrix<3, 3> a_copy, b_copy, c_copy, d_copy;
    a_copy = a;
    b_copy = b;
    c_copy = c;
    d_copy = d;

    res = a * b * (d - c);
    ASSERT_TRUE(a == a_copy);
    ASSERT_TRUE(b == b_copy);
    ASSERT_TRUE(c == c_copy);
    ASSERT_TRUE(d == d_copy);

    ans[0][0] = 78;
    ans[0][1] = 78;
    ans[0][2] = 78;
    ans[1][0] = 114;
    ans[1][1] = 114;
    ans[1][2] = 114;
    ans[2][0] = 150;
    ans[2][1] = 150;
    ans[2][2] = 150;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(res[i][j], ans[i][j]);
        }
    }
}


TEST(MatrixTests, AdvancedArithmetic_3)
{
    OurMatrix<3, 3> a, b, c, d, res;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            a[i][j] = i + j + 1;
            b[i][j] = i + j + 2;
            c[i][j] = i + j + 3;
            d[i][j] = i + j + 4;
        }
    }

    OurMatrix<3, 3> a_copy, b_copy, c_copy, d_copy;
    a_copy = a;
    b_copy = b;
    c_copy = c;
    d_copy = d;

    res = -(-a * 2 + b * (3 * d - c)) + b;
    auto e = b * (3 * d - c);
    assert(a == a_copy);
    assert(b == b_copy);
    assert(c == c_copy);
    assert(d == d_copy);

    OurMatrix<3, 3> ans;

    ans[0][0] = 103;
    ans[0][1] = 121;
    ans[0][2] = 139;
    ans[1][0] = 136;
    ans[1][1] = 160;
    ans[1][2] = 184;
    ans[2][0] = 169;
    ans[2][1] = 199;
    ans[2][2] = 229;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(e[i][j], ans[i][j]);
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(res[i][j], (-1) * ((-1) * a[i][j] * 2 + e[i][j]) + b[i][j]);
        }
    }
}

TEST(MatrixTests, TestInverseMatrix)
{
    OurMatrix<3, 3, double> a, ans;

    a[0][0] = 1.0;
    a[0][1] = 0.0;
    a[0][2] = 2.0;
    a[1][0] = 2.0;
    a[1][1] = -1.0;
    a[1][2] = 1.0;
    a[2][0] = 1.0;
    a[2][1] = 3.0;
    a[2][2] = -1.0;

    ans[0][0] = -2.0;
    ans[0][1] = 6.0;
    ans[0][2] = 2.0;
    ans[1][0] = 3.0;
    ans[1][1] = -3.0;
    ans[1][2] = 3.0;
    ans[2][0] = 7.0;
    ans[2][1] = -3.0;
    ans[2][2] = -1.0;

    for (uint8_t i = 0; i < 3; ++i)
    {
        for (uint8_t j = 0; j < 3; ++j)
        {
            ans[i][j] /= 12.0;
        }
    }
}
