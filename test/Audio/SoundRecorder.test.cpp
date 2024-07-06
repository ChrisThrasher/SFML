#include <SFML/Audio/SoundRecorder.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

namespace
{
struct SoundRecorder : sf::SoundRecorder
{
    [[nodiscard]] bool onProcessSamples(const std::int16_t* /* samples*/, std::size_t /* sampleCount */) override
    {
        return true;
    }
};
} // namespace

TEST_CASE("[Audio] sf::SoundRecorder")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::SoundRecorder>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::SoundRecorder>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::SoundRecorder>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::SoundRecorder>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::SoundRecorder>);
    }

    SECTION("Construction")
    {
        const SoundRecorder soundRecorder;
        CHECK(soundRecorder.getSampleRate() == 44100);
        CHECK(soundRecorder.getChannelCount() == 1);
    }
}
