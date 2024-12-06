#include <cstdlib>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

std::string dump_file(std::string filename) {
  std::ifstream file{ "input.txt"};
  if (!file) {
    std::cout << "input file not found" << std::endl;
  }

  std::ostringstream content;
  content << file.rdbuf();
  return content.str();
}

int solve(std::string const& input) {
  // Get all do's
  std::regex do_pattern{ "do(n't)?\\(\\)" };
  auto do_match_begin = std::sregex_iterator(input.begin(), input.end(), do_pattern);
  auto do_match_end = std::sregex_iterator{};

  std::vector<int> do_idx;
  std::vector<int> dont_idx;
  for (auto i = do_match_begin; i != do_match_end; i++) {
    std::smatch match = *i;
    if (match.str() == "do()") {
      do_idx.push_back(match.position());
    } else if (match.str() == "don't()") {
      dont_idx.push_back(match.position());
    }
  }

  std::regex mul_pattern{ "mul\\((\\d+),(\\d+)\\)" };
  auto mul_match_begin = std::sregex_iterator(input.begin(), input.end(), mul_pattern);
  auto mul_match_end = std::sregex_iterator{};

  int sum = 0;
  for (auto i = mul_match_begin; i != mul_match_end; i++) {
    std::smatch match = *i;
    if (match.size() != 3) {
      std::cout << "Invalid match size of " << match.size() << " for match " << match.str() << std::endl;
    }

    int closest_do = -1;
    auto closest_do_it = std::lower_bound(do_idx.begin(), do_idx.end(), match.position());
    if (closest_do_it != do_idx.begin()) {
      closest_do = *(closest_do_it - 1);
    }

    int closest_dont = -2;
    auto closest_dont_it = std::lower_bound(dont_idx.begin(), dont_idx.end(), match.position());
    if (closest_dont_it != dont_idx.begin()) {
      closest_dont = *(closest_dont_it - 1);
    }

    if (closest_do > closest_dont) {
      sum += std::stoi(match[1].str()) * std::stoi(match[2].str());
    }
  }

  return sum;
}

int main() {
  std::string input = dump_file("input.txt");
  std::cout << solve(input) << std::endl;
}
