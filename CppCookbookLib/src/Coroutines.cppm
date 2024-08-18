//
// Created by charlie on 8/18/2024.
//
#include <coroutine>
#include <future>

export module Coroutines;

/// Fulfilling the promise type API to use it in IntGenerator
template<typename T, typename G>
struct PromiseTypeBase
{
    // this is the value returned or yielded by a coroutine
    T PromiseValue;

    // default method to be invoked by co_yield and co_return
    auto yield_value(T Value)
    {
        PromiseValue = std::move(Value);
        return std::suspend_always{};
    }

    // getter for the generator
    G get_return_object()
    {
        return G{this};
    }

    std::suspend_always initial_suspend()
    {
        return {};
    }

    std::suspend_always final_suspend() noexcept
    {
        return {};
    }

    void return_void() {}

    void unhandled_exception()
    {
        std::terminate();
    }

    static auto get_return_object_on_allocation_failure()
    {
        return G{nullptr};
    }

};

namespace CoroIterator
{
    template <typename T>
    struct Iterator
    {
        std::coroutine_handle<T> CoroHnd{nullptr};

        void Resume()
        {
            if (not CoroHnd.done())
            {
                CoroHnd.resume();
            }
        }

        Iterator() = default;
        Iterator(std::coroutine_handle<T> CoroHnd) : CoroHnd(CoroHnd)
        {
            Resume();
        }

        void operator++()
        {
            Resume();
        }
        bool operator==(const Iterator<T>& other) const
        {
            return CoroHnd.done();
        }

        const auto& operator*() const
        {
            return CoroHnd.promise().PromiseValue;
        }
    };
}

struct IntGenerator
{
    using promise_type = PromiseTypeBase<int, IntGenerator>; // the promise type
    using PromiseTypeHnd = std::coroutine_handle<promise_type>;

    // making the generator iterable
    using Iterator = CoroIterator::Iterator<promise_type>;
    Iterator begin()
    {
        return {CoroHnd};
    }
    Iterator end() { return {}; }

    // ctor and dtors
    IntGenerator(PromiseTypeHnd const&) = delete;
    // std::exchange sets old value to new value, then returns old value
    IntGenerator(IntGenerator&& rhs) : CoroHnd(std::exchange(rhs.CoroHnd, nullptr)) {}
    ~IntGenerator()
    {
        if (CoroHnd)
        {
            CoroHnd.destroy();
        }
    }

private:
    explicit IntGenerator(promise_type* p) : CoroHnd{PromiseTypeHnd::from_promise(*p)} {}

    friend promise_type;
    PromiseTypeHnd CoroHnd;
};

export namespace Coroutines
{

    // Basic example to generate a range of values [start, end)
    IntGenerator Counter(int start, int end)
    {
        while (start < end)
        {
            co_yield start; // yields value and return control flow to caller
            ++start;
        }
    }
}
