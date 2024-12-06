#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

// Rule : Page -> {Pages that should not appear before}
using Rules = std::unordered_map<int, std::unordered_set<int>>;
using Pages = std::vector<int>;

std::vector<int> split(std::string const& input, char delim) {
  std::vector<int> ret;
  int start = 0;
  int end = 0;
  while (end != std::string::npos) {
    end = input.find(delim, start);
    ret.push_back(stoi(input.substr(start, end - start)));
    start = end + 1;
  }
  return ret;
}

void printVec(std::vector<int> const& vec) {
  std::string msg = "{";
  for (auto const& el : vec) {
    msg += std::to_string(el) + ", ";
  }
  std::cout << msg << "}" << std::endl;
}

void parseInput(std::string const& filename, Rules& rules, std::vector<Pages>& pages) {
  std::ifstream input {filename};
  if (!input) {
    std::cout << "Can't read input file" << std::endl;
    return;
  }

  std::string line;
  while(input >> line) {
    if (line == "\n") {
      continue;
    }

    size_t pipe = line.find('|');
    if (pipe != std::string::npos) {
      int left = std::stoi(line.substr(0, pipe));
      int right = std::stoi(line.substr(pipe + 1));
      rules[left].insert(right);
    } else {
      pages.push_back(split(line, ','));
    }
  }
}


int solvePart1(Rules const& rules, Pages const& pages) {
  std::unordered_set<int> encountered;
  bool ok = true;
  // Number of pages should be odd
  int middle_number = 0;
  for (int i = 0; i < pages.size(); i++) {
    if (!ok) {
      break;
    }

    int page_nb = pages[i];
    encountered.insert(page_nb);
    if (rules.find(page_nb) == rules.end()) {
      continue;
    }

    for (int must_not_encounter_before : rules.at(page_nb)) {
      if (encountered.find(must_not_encounter_before) != encountered.end()) {
        ok = false;
        break;
      }
    }
  }

  return ok ? pages[pages.size() / 2] : 0;
}

// Copy here is intentional, because pages will be modified in place
int solvePart2(Rules const& rules, Pages pages) {
  // Is the vector initally unsorted
  bool unsorted = false;
  // Has the while loop modified the vector
  bool modified = true;

  while (modified) {
    modified = false;

    // Find a page with a mismatching rule
    // Swap it with the first mismatch
    for (int i = 0; i < pages.size(); i++) {
      int page = pages[i];
      // If the current number has no rule, just skip it
      if (rules.find(page) == rules.end()) {
        continue;
      }
      // Find potential mismatch
      for (int j = 0; j < i; j++) {
        int prev_page = pages[j];
        // A previous page is present while it shouldn't be
        if (rules.at(page).find(prev_page) != rules.at(page).end()) {
          modified = true;
          unsorted = true;
          std::swap(pages[i], pages[j]);
        }
      }
    }
  }

  return unsorted ? pages[pages.size() / 2] : 0;
}

int main() {
  Rules rules;
  std::vector<Pages> all_pages;
  parseInput("input.txt", rules, all_pages);

  int total = 0;
  int total2 = 0;
  for (auto const& pages : all_pages) {
    total += solvePart1(rules, pages);
    total2 += solvePart2(rules, pages);
  }
  std::cout << "Solution for part 1: " << total << std::endl;
  std::cout << "Solution for part 2: " << total2 << std::endl;
}
