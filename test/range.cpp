/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <gtest/gtest.h>
#include <arrayfire.h>
#include <af/dim4.hpp>
#include <af/defines.h>
#include <af/traits.hpp>
#include <vector>
#include <iostream>
#include <complex>
#include <string>
#include <testHelpers.hpp>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using af::cfloat;
using af::cdouble;

template<typename T>
class Range : public ::testing::Test
{
    public:
        virtual void SetUp() {
            subMat0.push_back(af_make_seq(0, 4, 1));
            subMat0.push_back(af_make_seq(2, 6, 1));
            subMat0.push_back(af_make_seq(0, 2, 1));
        }
        vector<af_seq> subMat0;
};

// create a list of types to be tested
typedef ::testing::Types<float, double, int, unsigned int, unsigned char> TestTypes;

// register the type list
TYPED_TEST_CASE(Range, TestTypes);

template<typename T>
void rangeTest(const uint x, const uint y, const uint z, const uint w, const uint rep)
{
    if (noDoubleTests<T>()) return;

    af::dim4 idims(x, y, z, w);

    af_array outArray = 0;

    ASSERT_EQ(AF_SUCCESS, af_range(&outArray, idims.ndims(), idims.get(), rep, (af_dtype) af::dtype_traits<T>::af_type));

    // Get result
    T* outData = new T[idims.elements()];
    ASSERT_EQ(AF_SUCCESS, af_get_data_ptr((void*)outData, outArray));

    // Compare result
    unsigned mul1 = rep > 0;
    unsigned mul2 = rep > 1;
    unsigned mul3 = rep > 2;
    for(int w = 0; w < idims[3]; w++) {
        for(int z = 0; z < idims[2]; z++) {
            for(int y = 0; y < idims[1]; y++) {
                for(int x = 0; x < idims[0]; x++) {
                    T val =  (mul3 * w * idims[0] * idims[1] * idims[2]) +
                             (mul2 * z * idims[0] * idims[1]) +
                             (mul1 * y * idims[0]) + x;
                    dim_type idx = (w * idims[0] * idims[1] * idims[2]) +
                                   (z * idims[0] * idims[1]) +
                                   (y * idims[0]) + x;
                    ASSERT_EQ(val, outData[idx]) << "at: " << idx << std::endl;
                }
            }
        }
    }

    // Delete
    delete[] outData;

    if(outArray  != 0) af_destroy_array(outArray);
}

#define RANGE_INIT(desc, x, y, z, w, rep)                                                    \
    TYPED_TEST(Range, desc)                                                                  \
    {                                                                                       \
        rangeTest<TypeParam>(x, y, z, w, rep);                                               \
    }

    RANGE_INIT(Range1D0, 100,  1, 1, 1, 0);

    RANGE_INIT(Range2D0,  10, 20, 1, 1, 0);
    RANGE_INIT(Range2D1, 100,  5, 1, 1, 1);

    RANGE_INIT(Range3D0,  20,  6, 3, 1, 0);
    RANGE_INIT(Range3D1,  10, 12, 5, 1, 1);
    RANGE_INIT(Range3D2,  25, 30, 2, 1, 2);

    RANGE_INIT(Range4D0,  20,  6, 3, 2, 0);
    RANGE_INIT(Range4D1,  10, 12, 5, 2, 1);
    RANGE_INIT(Range4D2,  25, 30, 2, 2, 2);
    RANGE_INIT(Range4D3,  25, 30, 2, 2, 3);

///////////////////////////////// CPP ////////////////////////////////////
//
TEST(Range, CPP)
{
    if (noDoubleTests<float>()) return;

    const unsigned x = 23;
    const unsigned y = 15;
    const unsigned z = 4;
    const unsigned w = 2;
    const unsigned rep = 2;

    af::dim4 idims(x, y, z, w);
    af::array output = af::range(x, y, z, w, rep, f32);

    // Get result
    float* outData = new float[idims.elements()];
    output.host((void*)outData);

    // Compare result
    unsigned mul1 = rep > 0;
    unsigned mul2 = rep > 1;
    unsigned mul3 = rep > 2;
    for(int w = 0; w < idims[3]; w++) {
        for(int z = 0; z < idims[2]; z++) {
            for(int y = 0; y < idims[1]; y++) {
                for(int x = 0; x < idims[0]; x++) {
                    float val =  (mul3 * w * idims[0] * idims[1] * idims[2]) +
                                 (mul2 * z * idims[0] * idims[1]) +
                                 (mul1 * y * idims[0]) + x;
                    dim_type idx = (w * idims[0] * idims[1] * idims[2]) +
                                   (z * idims[0] * idims[1]) +
                                   (y * idims[0]) + x;
                    ASSERT_EQ(val, outData[idx]) << "at: " << idx << std::endl;
                }
            }
        }
    }

    // Delete
    delete[] outData;
}
