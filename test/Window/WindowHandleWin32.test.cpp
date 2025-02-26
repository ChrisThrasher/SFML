#include <SFML/Window/WindowHandle.hpp>

// Other 1st party headers
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

//
#include <catch2/catch_test_macros.hpp>

#include <iomanip>
#include <limits>
#include <sstream>

namespace Catch
{
template <typename T>
struct StringMaker<sf::Vector2<T>>
{
    static std::string convert(sf::Vector2<T> vector)
    {
        std::ostringstream os;
        os << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10);
        os << "(" << vector.x << ", " << vector.y << ")";
        return os.str();
    }
};
} // namespace Catch

TEST_CASE("[Window] sf::WindowHandle (Win32)")
{
    const sf::Vector2i vec1(1, 2);
    const sf::Vector2i vec2(3, 4);
    CHECK(vec1 == vec2);

    sf::Vector2i vec3(1, 2);
    sf::Vector2i vec4(3, 4);
    CHECK(vec3 == vec4);
}
