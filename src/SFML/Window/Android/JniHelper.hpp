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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

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
class JniArray;

template <class T, class TClass>
class JniList;

class JniListClass
{
private:
    JniListClass(JNIEnv* env, jclass listClass);

public:
    [[nodiscard]] static std::optional<JniListClass> findClass(JNIEnv* env);

    template <class T, class TClass>
    [[nodiscard]] std::optional<JniList<T, TClass>> makeFromJava(jobject list);

private:
    JNIEnv* m_env;
    jclass  m_listClass;
};

class JniMotionRangeClass;

class JniMotionRange
{
private:
    JniMotionRange(JNIEnv* env, jobject motionRange, jmethodID getAxisMethod);

    friend class JniMotionRangeClass;

public:
    [[nodiscard]] int getAxis() const;

private:
    JNIEnv*   m_env;
    jobject   m_motionRange;
    jmethodID m_getAxisMethod;
};

class JniMotionRangeClass
{
private:
    JniMotionRangeClass(JNIEnv* env, jclass motionRangeClass);

public:
    [[nodiscard]] static std::optional<JniMotionRangeClass> findClass(JNIEnv* env);

    [[nodiscard]] std::optional<JniMotionRange> makeFromJava(jobject motionRange);

private:
    JNIEnv* m_env;
    jclass  m_motionRangeClass;
};

class JniInputDeviceClass;

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over JNI InputDevice instances
///
/// This class abstracts access to native Java object
////////////////////////////////////////////////////////////
class JniInputDevice
{
private:
    JniInputDevice(JNIEnv*   env,
                   jobject   inputDevice,
                   jmethodID getNameMethod,
                   jmethodID getVendorIdMethod,
                   jmethodID getProductIdMethod,
                   jmethodID supportsSourceMethod,
                   jmethodID getMotionRangesMethod);

    friend class JniInputDeviceClass;

public:
    [[nodiscard]] unsigned getVendorId() const;

    [[nodiscard]] unsigned getProductId() const;

    [[nodiscard]] std::string getName() const;

    [[nodiscard]] bool supportsSource(size_t sourceFlags) const;

    [[nodiscard]] std::optional<JniList<JniMotionRange, JniMotionRangeClass>> getMotionRanges() const;

private:
    std::string javaStringToStd(jstring str) const;

    JNIEnv*   m_env;
    jobject   m_inputDevice;
    jmethodID m_getNameMethod;
    jmethodID m_getVendorIdMethod;
    jmethodID m_getProductIdMethod;
    jmethodID m_supportsSourceMethod;
    jmethodID m_getMotionRangesMethod;
};

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over JNI InputDevice class
///
/// This class abstracts access to native Java class
////////////////////////////////////////////////////////////
class JniInputDeviceClass
{
private:
    JniInputDeviceClass(JNIEnv* env, jclass inputDeviceClass, jmethodID getDeviceIdsMethod, jmethodID getDeviceMethod);

public:
    [[nodiscard]] static std::optional<JniInputDeviceClass> findClass(JNIEnv* env);

    [[nodiscard]] std::optional<JniArray<jint>> getDeviceIds();

    [[nodiscard]] std::optional<JniInputDevice> getDevice(jint idx);

private:
    JNIEnv*   m_env;
    jclass    m_inputDeviceClass;
    jmethodID m_getDeviceIdsMethod;
    jmethodID m_getDeviceMethod;
};

////////////////////////////////////////////////////////////
/// \brief RAII wrapper for attaching JavaVM thread
///
/// Thread is detached automatically when this class is destroyed
////////////////////////////////////////////////////////////
struct Jni
{
private:
    explicit Jni(JavaVM* vm);

public:
    Jni(Jni&& other) noexcept;

    Jni(const Jni&&) = delete;

    ~Jni();

    ////////////////////////////////////////////////////////////
    /// \brief Binds \p env to thread called NativeThread
    ///
    /// The thread will be detached once the instance of the optional
    /// is detached.
    ///
    /// \param vm Pointer to JavaVM instance (from ActivityStates)
    /// \param env Pointer to pointer to JNIEnv that will be used to
    /// invoke JNI.
    ///
    /// \return Returns instance of this object on success or std::nullopt on fail
    ///
    ////////////////////////////////////////////////////////////
    static std::optional<Jni> attachCurrentThread(JavaVM* vm, JNIEnv** env);

private:
    JavaVM* m_vm = nullptr;
};

#include <SFML/Window/Android/JniHelper.inl>
