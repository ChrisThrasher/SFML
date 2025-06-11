////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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

#include <SFML/Window/Android/JniHelper.hpp> // NOLINT(misc-header-include-cycle)

#include <SFML/System/Err.hpp>

#include <android/native_activity.h>

#include <algorithm>
#include <optional>
#include <ostream>
#include <string>

#include <cassert>

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over Java arrays
///
////////////////////////////////////////////////////////////
template <class T>
class JniArray
{
public:
    JniArray(JNIEnv* env, jintArray array) :
    m_env(env),
    m_array(array),
    m_length(env->GetArrayLength(array)),
    m_data(env->GetIntArrayElements(array, nullptr))
    {
    }

    JniArray(JniArray&& other) noexcept
    {
        std::swap(m_env, other.m_env);
        std::swap(m_array, other.m_array);
        std::swap(m_length, other.m_length);
        std::swap(m_data, other.m_data);
    }

    ~JniArray()
    {
        if (m_data)
            m_env->ReleaseIntArrayElements(m_array, m_data, 0);
    }

    T operator[](ssize_t idx) const
    {
        assert(0 <= idx && idx <= m_length);
        return m_data[idx];
    }

    [[nodiscard]] ssize_t getLength() const noexcept
    {
        return m_length;
    }

private:
    JNIEnv*   m_env    = nullptr;
    jintArray m_array  = nullptr;
    ssize_t   m_length = 0;
    T*        m_data   = nullptr;
};

template <class T, class TClass>
class JniList
{
private:
    JniList(JNIEnv* env, jobject list, jmethodID getMethod, jmethodID sizeMethod) :
    m_env(env),
    m_list(list),
    m_getMethod(getMethod),
    m_sizeMethod(sizeMethod)
    {
    }

    friend class JniListClass;

public:
    [[nodiscard]] std::optional<T> operator[](ssize_t idx) const
    {
        auto cls = TClass::findClass(m_env);
        if (!cls)
            return std::nullopt;

        jobject motionRange = m_env->CallObjectMethod(m_list, m_getMethod, idx);
        if (!motionRange)
            return std::nullopt;

        return cls->makeFromJava(motionRange);
    }

    [[nodiscard]] ssize_t getSize() const
    {
        return m_env->CallIntMethod(m_list, m_sizeMethod);
    }

private:
    JNIEnv*   m_env;
    jobject   m_list;
    jmethodID m_getMethod;
    jmethodID m_sizeMethod;
};

template <class T, class TClass>
[[nodiscard]] std::optional<JniList<T, TClass>> JniListClass::makeFromJava(jobject list)
{
    jmethodID getMethod  = m_env->GetMethodID(m_listClass, "get", "(I)Ljava/lang/Object;");
    jmethodID sizeMethod = m_env->GetMethodID(m_listClass, "size", "()I");

    if (!getMethod || !sizeMethod)
    {
        sf::err() << "Could not locate required List methods" << std::endl;
        return std::nullopt;
    }

    return JniList<T, TClass>(m_env, list, getMethod, sizeMethod);
}
