#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>


// ==========================================================================================
// Utilities
// ==========================================================================================

// Initialize local stream variable and check if stream is opened
#define INIT_FSTREAM(type, stream, filePath, mode)      \
  type stream (filePath, mode);                         \
  if (!(stream).is_open())                              \
    {                                                                   \
      std::cout << "Error opening file \'" << filePath << "\': " << std::strerror(errno) << std::endl; \
      exit(1);                                                          \
    }

// simply define lambda that calls `fname` function
#define FUNC(fname)                             \
  []() { return fname(); }


// ==========================================================================================
// Main logic
// ==========================================================================================

void run(std::istream& in, std::ostream& out)
{
  char ch;

  // main logic: replace all zeroes with X-s
  while (in.get(ch))
    {
      if (ch == '0')
        {
          out << "X";
        }
      else
        {
          out << ch;
        }
    }
}

void runForFiles(const std::string& inputFile, const std::string& outputFile)
{
  INIT_FSTREAM(std::ifstream, in, inputFile, std::ifstream::in)
  INIT_FSTREAM(std::ofstream, out, outputFile, std::ofstream::out);
  // execute generalized function
  run(in, out);
  // close streams
  in.close();
  out.close();
}

// could also be done with pointer to output string as second argument
std::string runForStrings(const std::string& input)
{
  // create string streams
  std::istringstream in(input);
  std::ostringstream out;
  // execute generalized function
  run(in, out);

  return out.str();
}


// ==========================================================================================
// Test utilities
// ==========================================================================================

// standard compare doesn't return position of difference
int compareStrings(const std::string& str1, const std::string& str2)
{
  int len1 = str1.length();
  int len2 = str2.length();
  int len = std::min(len1, len2);
  int idx;

  for (idx = 0; idx < len && str1[idx] == str2[idx]; idx++);

  if (idx == len && len1 == len2)
    {
      idx = -1;
    }

  return idx;
}

int test(const std::string& name, const std::string& input, const std::string& expected)
{
  // execute target function
  std::string actual = runForStrings(input);
  // check difference
  int diffPos = compareStrings(actual, expected);
  int result = 0;

  if (diffPos >= 0)
    {
      std::cout << "Test '" << name << "' failed. Result differs on position " << diffPos << std::endl;
      std::cout << "Actual:\n" << actual << "\n\nExpected:\n" << expected << "\n" << std::endl;
      result = 1;
    }

  return result;
}

void runAllTests(const std::vector<std::function<int()>> testFns)
{
  // variables for stats
  int passed = 0;
  int failed = 0;

  // execute tests and collect stats
  for (auto& testFn : testFns)
    {
      testFn() == 0
        ? passed++
        : failed++;
    }

  // show stats
  std::cout << "Tests passed: " << passed << std::endl;
  std::cout << "Tests failed: " << failed << std::endl;
  std::cout << "Totally tests: " << testFns.size() << std::endl;
}


// ==========================================================================================
// Tests
// ==========================================================================================

int test1()
{
  return test("first test",
              "1A 21\
1B 11 00 00 00 D6\
0C 07 65\
0D 43 00 00 0A A1",
              "1A 21\
1B 11 XX XX XX D6\
XC X7 65\
XD 43 XX XX XA A1");
}

int test2()
{
  return test("second test",
              "1A 07 65\
23 43 00 00 22 A8\
80 98\
1C 30 03\
2C 11 00 16",
              "1A X7 65\
23 43 XX XX 22 A8\
8X 98\
1C 3X X3\
2C 11 XX 16");
}

int test3()
{
  return test("bad test", "00 11", "X0 11");
}


// ==========================================================================================
// Entry point
// ==========================================================================================

int main(int argc, char* argv[])
{
  // define vector of all test functions
  std::vector<std::function<int()>> testFns =
    {FUNC(test1), FUNC(test2), FUNC(test3)};

  // run testing utility
  runAllTests(testFns);

  return 0;
}
