//
// Created by charlie on 8/18/2024.
//

#include <complex.h>
#include <type_traits>
#include <concepts>

export module Concepts;

#pragma region Add function

# pragma region Add helper functions

/// Determine if all arguments are of the same type
template<typename T, typename ...Ts>
constexpr inline bool TAllAreSameType = std::conjunction_v<std::is_same<T, Ts>...>;

/// Extracts the first to TFirstType can determine the type
template<typename T, typename ...>
struct FFirstArg
{
    using Type = T;
};

/// Determines the type of the first argument supplied to it
template<typename ...TArgs>
using TFirstArg = typename FFirstArg<TArgs...>::Type;

#pragma endregion "helper functions to ensure Add fn is typesafe"

/// Type-safe Add function for cpp17 - where compilation will fail when not all types are the same
template<typename ...TArgs>
std::enable_if_t<TAllAreSameType<TArgs...>, TFirstArg<TArgs...>> AddCpp17(const TArgs& ...Args)
{
    return (... + Args); // this is a cpp17 fold expr, equivalent to arg1 + arg2 + ... + argN
}

/// Type-safe Add function for cpp20 - where compilation will fail when not all types are the same
/// this is how we can implement Add without concepts, this just introduces the <code>requires</code> keyword
template<typename ...TArgs>
// this replaces the old std::enable_if_t which polluted our return type.
// it also gives us the nice message "substitution failed due to constraints: <constraints>"
requires TAllAreSameType<TArgs...>
auto AddCpp20(const TArgs& ...Args) noexcept
{
    return (... + Args);
}

// creating a new concept to cover add-able types
template<typename ...TArgs>
concept CSameTypeAdd = requires(TArgs ...Args)
{
    (... + Args); // checks operator+ is implemented
    requires TAllAreSameType<TArgs...>; // checks all arguments are of the same type
    requires sizeof ...(TArgs) > 1; /// needs 2 or more arguments to be valid Add operation
    // ensure operator+ is noexcept and same as first argument
    { (... + Args) } noexcept -> std::same_as<TFirstArg<TArgs...>>;
};

#pragma endregion "Add function"

#pragma region Optional class in cpp17

template<typename T>
class BadOptional
{
public:
    // normal ctor
    BadOptional() = default;

    // copy ctor - note the ridiculous template decl
    // it also doesn't work because this is a conversion ctor, no longer a copy ctor
    template <typename U, typename = std::enable_if_t<std::is_same_v<U, BadOptional> and std::is_copy_constructible_v<T>>>
    BadOptional(const BadOptional<U>& other);

    T& Unwrap()
    {
        return Value;
    }
private:
    T Value;
};

// now for a cpp17 version that works but difficult to understand
// structs are copyable by default so provides a copyable interface
struct Copyable{};
// deleting copy here to provide a noncopyable interface
struct NonCopyable
{
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable operator = (const NonCopyable&) = delete;
};

template<typename T>
class Cpp17Optional :
// deriving Optional from Copyable if T has a copy ctor otherwise
// Optional will be derived form NonCopyable
std::conditional_t<std::is_copy_constructible_v<T>, Copyable, NonCopyable>
{
public:
    Cpp17Optional() = default;
    T& Unwrap()
    {
        return Value;
    }
private:
    T Value;
};

#pragma endregion "Optional class"

export namespace Concepts
{
    // Type-safe Add function for cpp20 using concepts
    template<typename ...TArgs>
    requires CSameTypeAdd<TArgs...>
    auto Add(const TArgs& ...Args) noexcept
    {
        return (... + Args);
    }

    // Implementation of the Optional type using cpp20 concepts
    template<typename T>
    class Optional
    {
    public:
        // default ctor
        Optional() = default;
        Optional(T Value)
        {
            this->Value = std::make_unique<T>(Value);
            bHasValue = true;
        }
        // copy ctor
        Optional(const Optional& other) requires std::is_copy_constructible_v<T>;

        //dtor
        ~Optional() requires (not std::is_trivially_destructible_v<T>)
        {
            if (bHasValue)
            {
                Value->~Value();
            }
        }
        T& Unwrap()
        {
            return Value;
        }
    private:
        std::unique_ptr<T> Value = nullptr;
        bool bHasValue = false;
    };
}

void TestingAdd()
{
    AddCpp17(7, 5);
    AddCpp20(7, 5);
    Concepts::Add(7, 5);

    // try uncommenting these to see the difference between the error messages
    // AddCpp17(7, 5.f);
    // AddCpp20(7, 5.f);
    // Add(7, 5.f);
}