#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

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

void test(const std::string& inputFile, const std::string& outputFile, const std::string& expectedFile)
{
  // execute target function
  run(inputFile, outputFile);
  // check difference
  int diffPos = compareFiles(outputFile, expectedFile);

  if (diffPos >= 0)
    {
      std::cout << "Test failed. Result differs on position." << diffPos << std::endl;
    }
  else
    {
      std::cout << "Test passed" << std::endl;
    }
}

int main(int argc, char* argv[])
{
  // assume first arg is file with input program
  std::string inputFile = argv[1];
  // assume second arg is output file
  std::string outputFile = argv[2];
  // assume third arg is expected result file
  std::string expectedFile = argv[3];

  // run testing utility
  test(inputFile, outputFile, expectedFile);

  return 0;
}
