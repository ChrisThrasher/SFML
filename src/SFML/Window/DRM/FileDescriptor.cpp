////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/DRM/FileDescriptor.hpp>

#include <fcntl.h>
#include <unistd.h>
#include <utility>


namespace sf::priv
{
////////////////////////////////////////////////////////////
FileDescriptor::FileDescriptor(const char* path, int flags) : m_fd(::open(path, flags))
{
}


////////////////////////////////////////////////////////////
FileDescriptor::FileDescriptor(FileDescriptor&& fileDescriptor) noexcept : m_fd(std::exchange(fileDescriptor.m_fd, -1))
{
}


////////////////////////////////////////////////////////////
FileDescriptor& FileDescriptor::operator=(FileDescriptor&& fileDescriptor) noexcept
{
    if (this == &fileDescriptor)
        return *this;

    reset();

    m_fd = std::exchange(fileDescriptor.m_fd, -1);

    return *this;
}


////////////////////////////////////////////////////////////
FileDescriptor::~FileDescriptor()
{
    reset();
}


////////////////////////////////////////////////////////////
void FileDescriptor::reset()
{
    if (*this)
        ::close(m_fd);
    m_fd = -1;
}


////////////////////////////////////////////////////////////
int FileDescriptor::get() const
{
    return m_fd;
}


////////////////////////////////////////////////////////////
FileDescriptor::operator bool() const
{
    return m_fd >= 0;
}
} // namespace sf::priv
