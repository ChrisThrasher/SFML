#include <SFML/Graphics.hpp>

#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <random>

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
    std::cout << '\n';
    std::cout << "Event count: " << eventCount << "\n";
    std::cout << "Frame count: " << frameCount << "\n";
}
