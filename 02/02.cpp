#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

bool check_report(std::vector<int> const& report) {
  if (report.size() < 2)
    return false;

  bool const increasing = report[0] < report[1];
  int diff = std::abs(report[0] - report[1]);
  bool safe_distance = diff >= 1 && diff <= 3;
  if (!safe_distance)
    return false;

  for (int i = 1; i < report.size(); i++) {
    bool order_kept = increasing ? report[i - 1] < report[i] : report[i - 1] > report[i];
    diff = std::abs(report[i - 1] - report[i]);
    safe_distance = diff >= 1 && diff <= 3;
    if (!order_kept || !safe_distance) {
      return false;
    }
  }

  return true;
}

int main() {
  std::ifstream file{ "input.txt" };
  if (!file) {
    std::cout << "Can't find input file" << std::endl;
    return 1;
  }

  int num_safe_reports = 0;
  std::string input;
  while (std::getline(file, input)) {
    std::vector<int> report = split(input, ' ');
    num_safe_reports += check_report(report);
  }
  std::cout << "Number of safe reports part 1: " << num_safe_reports << std::endl;

  std::ifstream file2{ "input.txt"};
  if (!file2) {
    std::cout << "Can't find input file2" << std::endl;
    return 1;
  }

  num_safe_reports = 0;
  input.clear();
  while (std::getline(file2, input)) {
    std::vector<int> report = split(input, ' ');
    bool safe_v2 = check_report(report);
    for (int i = 0; i < report.size(); i++) {
      if (safe_v2) break;
      std::vector<int> report_cp = report;
      report_cp.erase(report_cp.begin() + i);
      safe_v2 |= check_report(report_cp);
    }
    num_safe_reports += safe_v2;
  }
  std::cout << "Number of safe reports part 2: " << num_safe_reports << std::endl;
}
