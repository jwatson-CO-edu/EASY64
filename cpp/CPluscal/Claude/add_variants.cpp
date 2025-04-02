#include <iostream>
#include <variant>
#include <type_traits>

// Define our variant type for convenience
using NumericVariant = std::variant<long, double>;

// Addition operator for NumericVariant
NumericVariant operator+(const NumericVariant& lhs, const NumericVariant& rhs) {
    // Using std::visit to handle all possible type combinations
    return std::visit([](auto&& left, auto&& right) -> NumericVariant {
        // If both are the same type, return that type
        if constexpr (std::is_same_v<decltype(left), decltype(right)>) {
            return left + right;
        }
        // If different types, convert to double for maximum precision
        else {
            return static_cast<double>(left) + static_cast<double>(right);
        }
    }, lhs, rhs);
}

// Helper function to print variant contents
void printVariant(const NumericVariant& v) {
    std::visit([](auto&& arg) {
        std::cout << arg;
    }, v);
}

int main() {
    NumericVariant a = 10L;         // long
    NumericVariant b = 20L;         // long
    NumericVariant c = 15.5;        // double
    
    // Test different combinations
    NumericVariant result1 = a + b; // long + long
    NumericVariant result2 = a + c; // long + double
    NumericVariant result3 = c + c; // double + double
    
    std::cout << "long + long = ";
    printVariant(result1);
    std::cout << " (type: " << (std::holds_alternative<long>(result1) ? "long" : "double") << ")\n";
    
    std::cout << "long + double = ";
    printVariant(result2);
    std::cout << " (type: " << (std::holds_alternative<long>(result2) ? "long" : "double") << ")\n";
    
    std::cout << "double + double = ";
    printVariant(result3);
    std::cout << " (type: " << (std::holds_alternative<long>(result3) ? "long" : "double") << ")\n";
    
    return 0;
}