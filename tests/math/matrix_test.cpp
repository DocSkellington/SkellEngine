#include <iostream>

#include <catch.hpp>

#include "SkellEngine/math/Matrix.hpp"

using namespace engine;

SCENARIO("Matrix's operations are correctly implemented", "[math][matrix]") {
    Matrix<int, 2, 3> m1;

    m1.set(1, 0, 5);
    REQUIRE(m1.get(1, 0) == 5);
    REQUIRE(m1[1][0] == 5);

    m1.fillRow(0, {0, 1, 2});
    m1.fillRow(1, {3, 4, 5});

    WHEN("We multiply two matrices") {
        Matrix<long, 3, 2> m2;
        m2.fillRow(0, {6, 7});
        m2.fillRow(1, {8, 9});
        m2.fillRow(2, {10, 11});

        auto resMult = m1 * m2;

        REQUIRE(resMult.getNumberOfRows() == 2);
        REQUIRE(resMult.getNumberOfColumns() == 2);
        REQUIRE(resMult[0][0] == 28);
        REQUIRE(resMult[0][1] == 31);
        REQUIRE(resMult[1][0] == 100);
        REQUIRE(resMult[1][1] == 112);
        Matrix<int, 2, 2> target = {28, 31, 100, 112};
        REQUIRE(resMult == target);
    }

    WHEN("We sum two matrices") {
        Matrix<long, 2, 3> m3;
        m3.fillRow(0, {6, 7, 8});
        m3.fillRow(1, {9, 10, 11});

        auto resSum = m1 + m3;

        REQUIRE(resSum.getNumberOfRows() == 2);
        REQUIRE(resSum.getNumberOfColumns() == 3);
        REQUIRE(resSum == Matrix<int, 2, 3>({6, 8, 10, 12, 14, 16}));
    }
}

SCENARIO("It is possible to create a diagonal matrix", "[math][matrix]") {
    std::array<double, 3> array = {3.14, 0.13, 902.4};
    auto m = createDiagonalMatrix<double, 3>(array);

    for (int i = 0 ; i < 3 ; i++) {
        for (int j = 0 ; j < 3 ; j++) {
            if (i == j) {
                REQUIRE(m[i][j] == array[i]);
            }
            else {
                REQUIRE(m[i][j] == 0);
            }
        }
    }
}