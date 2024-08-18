//
// Created by charlie on 8/18/2024.
//

#include <array>
#include <gtest/gtest.h>
import Concepts;

class Vec3
{
public:
    Vec3(const float x, const float y, const float z) : coords{x, y, z} {}

    [[nodiscard]] float x() const { return coords[0]; }
    [[nodiscard]] float y() const { return coords[1]; }
    [[nodiscard]] float z() const { return coords[2]; }





private:
    std::array<float, 3> coords{};
};
inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
    return
    {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.z() + rhs.z()
    };
}
inline bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return  lhs.x() == rhs.x() &&
            lhs.y() == rhs.y() &&
            lhs.z() == rhs.z();
}
inline bool operator!=(const Vec3& lhs, const Vec3& rhs)
{
    return !(lhs == rhs);
}

// Demonstrate some basic assertions.
TEST(ConceptsTest, TypesafeAdd) {
    // integer - obviously should be able to add together
    EXPECT_EQ(Concepts::Add(1, 2), 3) << "Adding `int` types together failed";
    // float -  testing decimal points too
    EXPECT_FLOAT_EQ(Concepts::Add(1.f, 2.f), 3.f) << "Adding `float` types together failed";
    // Vec3 - supporting a custom type with operator+ overloaded
    Vec3 v1 = Vec3(1.f, 2.f, 3.f), v2 = Vec3(2.f, 3.f, 4.f);
    EXPECT_EQ(v1 + v2, Vec3(3.f, 5.f, 7.f)) << "Adding operator+ overloaded types together failed";
}
