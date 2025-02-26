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

#include <SystemUtil.hpp>

TEST_CASE("[Window] sf::WindowHandle (Win32)")
{
    using sf::operator<<;
    const sf::Vector2i vec1(1, 2);
    const sf::Vector2i vec2(3, 4);
    CHECK(vec1 == vec2);

    sf::Vector2i vec3(1, 2);
    sf::Vector2i vec4(3, 4);
    CHECK(vec3 == vec4);
}
