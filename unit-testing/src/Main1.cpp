#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

void run(const std::string& inputFile)
{
  std::ifstream inputStream (inputFile, std::ifstream::in);

  if (!inputStream.is_open())
    {
      std::cout << "Error opening file \'" << inputFile << "\': " << std::strerror(errno) << std::endl;
    }

  char ch;

  // main logic: replace all zeroes with X-s
  while (inputStream.get(ch))
    {
      if (ch == '0')
        {
          std::cout << 'X';
        }
      else
        {
          std::cout << ch;
        }
    }

  inputStream.close();
}

int main(int argc, char* argv[])
{
  // assume first arg is file with input program
  std::string inputFile = argv[1];

  // simply execute main logic
  run(inputFile);

  return 0;
}
