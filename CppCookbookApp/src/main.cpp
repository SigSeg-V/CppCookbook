import Concepts;
import Coroutines;
#include <print>

int main()
{
    int AddResult = Concepts::Add(1,2,3,4,5);
    std::print("[Cpp20 Concepts] 1 + 2 + 3 + 4 + 5 = {}\n", AddResult);

    for (auto num : Coroutines::Counter(0, 15))
    {
        std::print("[Cpp20 Coroutines] Current count: {}\n", num);
    }
}
