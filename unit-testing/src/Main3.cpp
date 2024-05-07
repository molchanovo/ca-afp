#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


// Initialize local stream variable and check if stream is opened
#define INIT_FSTREAM(type, stream, filePath, mode)      \
  type stream (filePath, mode);                         \
  if (!(stream).is_open())                              \
    {                                                                   \
      std::cout << "Error opening file \'" << filePath << "\': " << std::strerror(errno) << std::endl; \
      exit(1);                                                          \
    }


void run(const std::string& inputFile, const std::string& outputFile)
{
  INIT_FSTREAM(std::ifstream, in, inputFile, std::ifstream::in)
  INIT_FSTREAM(std::ofstream, out, outputFile, std::ofstream::out);

  char ch;

  // main logic: replace all zeroes with X-s
  while (in.get(ch))
    {
      if (ch == '0')
        {
          out << 'X';
        }
      else
        {
          out << ch;
        }
    }

  // close streams
  in.close();
  out.close();
}

int compareFiles(const std::string& inputFile1, const std::string& inputFile2)
{
  INIT_FSTREAM(std::ifstream, in1, inputFile1, std::ifstream::in);
  INIT_FSTREAM(std::ifstream, in2, inputFile2, std::ifstream::in);

  char ch1, ch2;
  int pos = -1;

  do {
    in1.get(ch1);
    in2.get(ch2);
    pos++;
  } while (!in1.eof() && !in2.eof() && ch1 == ch2);

  if (in1.eof() && in2.eof())
    {
      pos = -1;
    }

  // close streams
  in1.close();
  in2.close();

  return pos;
}

int test(const std::string& name, const std::string& inputFile, const std::string& outputFile, const std::string& expectedFile)
{
  // execute target function
  run(inputFile, outputFile);
  // check difference
  int diffPos = compareFiles(outputFile, expectedFile);
  int result = 0;

  if (diffPos >= 0)
    {
      std::cout << "Test " << name << " failed. Result differs on position " << diffPos << std::endl;
      result = 1;
    }
  // when there are multiple tests the fact that one standalone test has "passed" isn't
  // interesting any more
  // else
  //   {
  //     std::cout << "Test passed" << std::endl;
  //   }

  return result;
}

void runAllTests(const int testsN, const std::vector<std::string> inputData[])
{
  // variables for stats
  int passed = 0;
  int failed = 0;

  // execute tests and collect stats
  for (int idx = 0; idx < testsN; idx++)
    {
      test(inputData[idx][0], inputData[idx][1], inputData[idx][2], inputData[idx][3]) == 0
        ? passed++
        : failed++;
    }

  // show stats
  std::cout << "Tests passed: " << passed << std::endl;
  std::cout << "Tests failed: " << failed << std::endl;
  std::cout << "Totally tests: " << testsN << std::endl;
}

int main(int argc, char* argv[])
{
  // define tests input data (could be done on global level)
  std::vector<std::string> inputData[] =
    {{"first test", "../test/test01.txt", "../test/result01.txt", "../test/expected01.txt"},
     {"second test", "../test/test02.txt", "../test/result02.txt", "../test/expected02.txt"},
     {"bad test", "../test/test03.txt", "../test/result03.txt", "../test/expected03.txt"}};
  int testsN = sizeof(inputData) / sizeof(inputData[0]);

  // run testing utility
  runAllTests(testsN, inputData);

  return 0;
}
