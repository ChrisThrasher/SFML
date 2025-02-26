#include <SFML/Window/WindowHandle.hpp>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501 // NOLINT(bugprone-reserved-identifier)
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef UNICODE
#define UNICODE 1
#endif

#ifndef _UNICODE
#define _UNICODE 1 // NOLINT(bugprone-reserved-identifier)
#endif
#include <windows.h>

// Other 1st party headers
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowBase.hpp>

// Include after Windows.h to fix printing sf::Vector2<T>s when assertions fail
#include <catch2/catch_template_test_macros.hpp>

#include <SystemUtil.hpp>
#include <optional>

namespace
{
bool gotWmShowWindow = false;

LRESULT WINAPI wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_SHOWWINDOW:
            gotWmShowWindow = true;
            assert(wParam == TRUE); // If wParam is TRUE, the window is being shown
            assert(lParam == 0); // If lParam is zero, the message was sent because of a call to the ShowWindow function
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(handle, message, wParam, lParam);
}

sf::WindowHandle createWindowWrapper(LPWSTR className, HINSTANCE hInstance, DWORD dwExStyle, HMENU hMenu)
{
    return CreateWindowExW(dwExStyle,
                           className,
                           L"WindowHandle Tests",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           640,
                           480,
                           nullptr,
                           hMenu,
                           hInstance,
                           nullptr);
}

struct NormalWindow
{
};
struct NormalMenuWindow
{
};
struct ExStyleWindow
{
};
struct ExStyleMenuWindow
{
};

template <typename WindowType>
sf::WindowHandle createWindow(LPWSTR className, HINSTANCE hInstance);

template <>
sf::WindowHandle createWindow<NormalWindow>(LPWSTR className, HINSTANCE hInstance)
{
    return createWindowWrapper(className, hInstance, 0, nullptr);
}

template <>
sf::WindowHandle createWindow<NormalMenuWindow>(LPWSTR className, HINSTANCE hInstance)
{
    const HMENU menu = CreateMenu();
    AppendMenuW(menu, MF_STRING, 1, L"Test");
    return createWindowWrapper(className, hInstance, 0, menu);
}

template <>
sf::WindowHandle createWindow<ExStyleWindow>(LPWSTR className, HINSTANCE hInstance)
{
    return createWindowWrapper(className, hInstance, WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE, nullptr);
}

template <>
sf::WindowHandle createWindow<ExStyleMenuWindow>(LPWSTR className, HINSTANCE hInstance)
{
    const HMENU menu = CreateMenu();
    AppendMenuW(menu, MF_STRING, 1, L"Test");
    return createWindowWrapper(className, hInstance, WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE, menu);
}
} // namespace

TEMPLATE_TEST_CASE("[Window] sf::WindowHandle (Win32)", "", NormalWindow, NormalMenuWindow, ExStyleWindow, ExStyleMenuWindow)
{
    using sf::operator<<;

    const WNDCLASSW classInfo{{}, wndProc, {}, {}, GetModuleHandleW(nullptr), {}, {}, {}, {}, L"sfml_WindowHandleTests"};

    const ATOM winClassId = RegisterClassW(&classInfo);
    REQUIRE(winClassId);

    const sf::WindowHandle handle = createWindow<TestType>(reinterpret_cast<LPWSTR>(static_cast<ULONG_PTR>(winClassId)),
                                                           classInfo.hInstance);
    REQUIRE(handle);
    REQUIRE(!gotWmShowWindow);
    REQUIRE(IsWindow(handle));

    RECT windowRect{};
    REQUIRE(GetWindowRect(handle, &windowRect));
    const auto position = sf::Vector2i(sf::Vector2(windowRect.left, windowRect.top));

    RECT clientRect{};
    REQUIRE(GetClientRect(handle, &clientRect));
    const auto initialSize = sf::Vector2u(
        sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
    constexpr sf::Vector2u newSize(640, 480);

    SECTION("sf::WindowBase")
    {
        std::optional<sf::WindowBase> windowBase;

        SECTION("WindowHandle constructor")
        {
            windowBase.emplace(handle);
        }

        SECTION("create(WindowHandle)")
        {
            windowBase.emplace().create(handle);
        }

        CHECK(windowBase->isOpen());
        CHECK(windowBase->getPosition() == position);
        CHECK(windowBase->getSize() == initialSize);
        CHECK(windowBase->getNativeHandle() == handle);

        CHECK(windowBase->getSize() != newSize);
        windowBase->setSize(newSize);

        REQUIRE(GetClientRect(handle, &clientRect));
        const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
        CHECK(size == newSize);               // Validate that the actual client rect is indeed what we asked for
        CHECK(windowBase->getSize() == size); // Validate that the `getSize` also returns the _actual_ client size
    }

    SECTION("sf::Window")
    {
        std::optional<sf::Window> window;

        SECTION("Default context settings")
        {
            SECTION("WindowHandle constructor")
            {
                window.emplace(handle);
            }

            SECTION("create(WindowHandle)")
            {
                window.emplace().create(handle);
            }

            CHECK(window->getSettings().attributeFlags == sf::ContextSettings::Default);
        }

        SECTION("Custom context settings")
        {
            static constexpr sf::ContextSettings contextSettings{/* depthBits*/ 1, /* stencilBits */ 1, /* antiAliasingLevel */ 1};

            SECTION("WindowHandle constructor")
            {
                window.emplace(handle, contextSettings);
            }

            SECTION("create(WindowHandle)")
            {
                window.emplace().create(handle, contextSettings);
            }

            CHECK(window->getSettings().depthBits >= 1);
            CHECK(window->getSettings().stencilBits >= 1);
            CHECK(window->getSettings().antiAliasingLevel >= 1);
        }

        CHECK(window->isOpen());
        CHECK(window->getPosition() == position);
        CHECK(window->getSize() == initialSize);
        CHECK(window->getNativeHandle() == handle);

        CHECK(window->getSize() != newSize);
        window->setSize(newSize);

        REQUIRE(GetClientRect(handle, &clientRect));
        const auto size = sf::Vector2u(sf::Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
        CHECK(size == newSize);           // Validate that the actual client rect is indeed what we asked for
        CHECK(window->getSize() == size); // Validate that the `getSize` also returns the _actual_ client size
    }

    CHECK(gotWmShowWindow);
    CHECK(IsWindow(handle)); // The window is not destroyed

    CHECK(DestroyWindow(handle));
    CHECK(UnregisterClassW(classInfo.lpszClassName, classInfo.hInstance));
    gotWmShowWindow = false;

    CHECK(!IsWindow(handle)); // Now it is gone
}
