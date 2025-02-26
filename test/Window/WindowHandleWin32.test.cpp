#include <SFML/Window/WindowHandle.hpp>

// Other 1st party headers
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>

TEST_CASE("[Window] sf::WindowHandle (Win32)")
{
    const sf::Vector2i vec1(1, 2);
    const sf::Vector2i vec2(3, 4);
    CHECK(vec1 == vec2);

    sf::Vector2i vec3(1, 2);
    sf::Vector2i vec4(3, 4);
    CHECK(vec3 == vec4);
}
