#include <SFML/Window/WindowStyle.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Window] sf::Style")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_same_v<std::underlying_type_t<sf::Style>, std::uint8_t>);
    }

    SECTION("any()")
    {
        STATIC_CHECK(!sf::any(sf::Style::None));
        STATIC_CHECK(sf::any(sf::Style::Titlebar));
        STATIC_CHECK(sf::any(sf::Style::Resize));
        STATIC_CHECK(sf::any(sf::Style::Close));
        STATIC_CHECK(sf::any(sf::Style::Default));
    }

    SECTION("Operators")
    {
        SECTION("operator~")
        {
            STATIC_CHECK(~sf::Style::None == static_cast<sf::Style>(255));
        }

        SECTION("operator|")
        {
            STATIC_CHECK((sf::Style::Titlebar | sf::Style::Resize) == static_cast<sf::Style>(3));
        }

        SECTION("operator&")
        {
            STATIC_CHECK((sf::Style::Titlebar & sf::Style::Default) == sf::Style::Titlebar);
        }

        SECTION("operator^")
        {
            STATIC_CHECK((sf::Style::Default ^ sf::Style::Resize) == (sf::Style::Titlebar | sf::Style::Close));
        }

        SECTION("operator|=")
        {
            sf::Style style = sf::Style::None;
            style |= sf::Style::Titlebar;
            CHECK(style == sf::Style::Titlebar);
        }

        SECTION("operator&=")
        {
            sf::Style style = sf::Style::None;
            style &= sf::Style::Titlebar;
            CHECK(style == sf::Style::None);
        }

        SECTION("operator^=")
        {
            sf::Style style = sf::Style::Titlebar | sf::Style::Resize;
            style ^= sf::Style::Titlebar;
            CHECK(style == sf::Style::Resize);
        }
    }
}
