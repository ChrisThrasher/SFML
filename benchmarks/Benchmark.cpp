#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <random>

int main()
{
    // Set up scene
    sf::RenderWindow               window(sf::VideoMode({720, 480}), "Benchmark");
    std::mt19937                   rng(std::random_device{}());
    std::uniform_real_distribution xDist(0.f, static_cast<float>(window.getSize().x));
    std::uniform_real_distribution yDist(0.f, static_cast<float>(window.getSize().y));
    std::uniform_int_distribution  pixelDist(0, 255);
    std::vector<sf::Texture>       textures;
    textures.reserve(2'500);
    std::vector<sf::Sprite> sprites;
    sprites.reserve(textures.capacity());
    for (std::size_t i = 0; i < textures.capacity(); ++i)
    {
        auto& texture = textures.emplace_back(
            sf::Image(sf::Vector2u(8, 8),
                      sf::Color(static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng)),
                                static_cast<std::uint8_t>(pixelDist(rng)))));
        auto& sprite = sprites.emplace_back(texture);
        sprite.setPosition({xDist(rng), yDist(rng)});
    }

    // Perform benchmark
    constexpr auto maxRuntime = std::chrono::seconds(5);
    auto           eventCount = 0;
    auto           frameCount = 0;
    const auto     start      = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < maxRuntime)
    {
        while (const std::optional event = window.pollEvent())
            ++eventCount;

        window.clear();
        for (const auto& sprite : sprites)
            window.draw(sprite);
        window.display();

        ++frameCount;
    }
    const auto runtime = std::chrono::steady_clock::now() - start;

    // Show results
    struct CommaNumpunct : std::numpunct<char>
    {
        [[nodiscard]] char do_thousands_sep() const override
        {
            return ',';
        }
        [[nodiscard]] std::string do_grouping() const override
        {
            return "\3";
        }
    };
    const auto runtimeInSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(runtime).count();
    const auto averageFramerate = static_cast<float>(frameCount) / runtimeInSeconds;
    std::cout.imbue(std::locale(std::locale::classic(), new CommaNumpunct));
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Event count\t" << eventCount << "\n";
    std::cout << "Frame count\t" << frameCount << "\n";
    std::cout << "Runtime:\t" << runtimeInSeconds << " seconds\n";
    std::cout << "Framerate:\t" << averageFramerate << " fps\n";
}
