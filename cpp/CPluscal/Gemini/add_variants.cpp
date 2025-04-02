// 2025-04-02: DOES NOT COMPILE!

#include <iostream>
#include <variant>

// Define a generic add function using std::visit
template <typename T, typename U>
auto add_variants(const std::variant<T,U>& v1, const std::variant<T,U>& v2) {
    return std::visit(
        [](const auto& a, const auto& b) {
            return a + b;
        }, 
        v1, 
        v2
    );
}

int main() {
    std::variant<int, double> var1 = 10;
    std::variant<int, double> var2 = 20.5;

    // Adding two variants
    auto result = add_variants(var1, var2);

    // Printing the result
    std::cout << "The sum is: " << result << std::endl; // Output: The sum is: 30.5

    return 0;
}