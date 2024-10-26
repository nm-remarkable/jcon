#include "lib/parser.h"
#include "lib/format.h"
#include <string>
#include <vector>
#include <format>

template<typename T>                                                   
struct std::formatter<std::vector<T>> {
  constexpr auto parse(std::format_parse_context& ct) {
    return ct.begin();
  }
  auto format(const std::vector<T>& vc, std::format_context& ct) const {
    std::string str = {"Vector {\n"};
    for(const T& t : vc) {
        str.append(fmt::format("  {},\n", t));
    }
    str.append("}");

    return std::format_to(ct.out(), "{}", str);
  }
};

int main()
{
    fmt::println("{}", lex("{}"));
    fmt::println("{}", lex("{ \"sua\": \"is\", \"key\": { \"object_key\": \"object_value\", \"o_int\": 1, \"o_true\": true}}"));
    return 0;
}
