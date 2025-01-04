#include <SFML/Graphics.hpp>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <iostream>
#include <random>

struct EventListener : Catch::EventListenerBase
{
    using Catch::EventListenerBase::EventListenerBase;

    void benchmarkEnded(const Catch::BenchmarkStats<>& stats) override
    {
        benchmarkStats = stats;
    }

    static inline Catch::BenchmarkStats<> benchmarkStats;
};
CATCH_REGISTER_LISTENER(EventListener)

TEST_CASE("Benchmark")
{
    // Set up scene
    sf::RenderWindow               window(sf::VideoMode({720, 480}), "Benchmark");
    std::mt19937                   rng(std::random_device{}());
    std::uniform_real_distribution xDist(0.f, static_cast<float>(window.getSize().x));
    std::uniform_real_distribution yDist(0.f, static_cast<float>(window.getSize().y));
    std::uniform_int_distribution  pixelDist(0, 255);
    std::vector<sf::Texture>       textures(2'500);
    std::vector<sf::Sprite>        sprites;
    sprites.reserve(textures.size());
    for (auto& texture : textures)
    {
        REQUIRE(texture.loadFromImage(
            sf::Image(sf::Vector2u(8, 8),
                      sf::Color(static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng))))));
        auto& sprite = sprites.emplace_back(texture);
        sprite.setPosition({xDist(rng), yDist(rng)});
    }
    REQUIRE(sprites.size() == textures.size());

    // Perform benchmark
    auto eventCount = 0;
    auto frameCount = 0;
    BENCHMARK("Main loop")
    {
        while (const std::optional event = window.pollEvent())
            ++eventCount;

        window.clear();
        for (const auto& sprite : sprites)
            window.draw(sprite);
        window.display();

        ++frameCount;
    };
    CHECK(frameCount > 0);
    CHECK(eventCount < frameCount); // Fewer than 1 event per frame is to be expected

    // Show results
    const auto fps = 1.f /
                     std::chrono::duration_cast<std::chrono::duration<float>>(EventListener::benchmarkStats.mean.point).count();
    std::cout << '\n';
    std::cout << "Event count:\t" << eventCount << "\n";
    std::cout << "Frame count:\t" << frameCount << "\n";
    std::cout << "Avg fps:\t" << fps << "\n";
}
