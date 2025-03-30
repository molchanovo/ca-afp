# Автоматизоване тестування

## Що таке автоматизоване тестування?

Автоматизоване тестування полягає у виконанні тестування програмного коду за рахунок
визначення:
- наборів вхідних даних функцій, які необхідно перевірити/протестувати (фактичні
  параметри);
- очікуваних результатів, які функції мають повернути для заданих наборів вхідних даних;
- утиліт, що виконують порівняння результатів виконання з очікуваними результатами;
- (і виконання) тестів, тобто функцій, що виконують виклик коду, який необхідно
  перевірити, для визначених вхідних даних, та виклик утиліт порівняння фактичних
  результатів виконання з визначеними очікуваними результатами.

Автоматизоване тестування використовується для:
- визначення очікуваної поведінки інтерфейсних функцій (TDD — Test Driven Development);
- перевірки коректності роботи окремих функцій, методів, модулів програми (*модульне
  тестування*);
- перевірка коректності роботи кількох модулів програми разом (*інтеграційне
  тестуваання*);
- для спрощення перевірки результату, за рахунок порівняння фактичного результату
  виконання функції з певним очікуваним результатом;
- перевірки поведінки програми для "граничних" випадків та для випадків некоректних
  вхідних даних;
- зворотна підтримка (backward compatibility)

*Примітка:* детальніше про модульне та інтеграційне тестування можна прочитати в Інтернеті,
наприклад, на [сайті](https://elib.lntu.edu.ua/sites/default/files/elib_upload/%D0%A2%D0%B5%D1%81%D1%82%D1%83%D0%B2%D0%B0%D0%BD%D0%BD%D1%8F/page14.html)
Луцького національного технічного університету.

Виконання тестування в проєктах: для забезпечення зворотної підтримки і в цілому коректності
роботи проєкту в умовах, коли зміни в проєкт вносять різні розробники, при розробці таких
проєктів використовується [неперервна інтеграція](https://uk.wikipedia.org/wiki/%D0%9D%D0%B5%D0%BF%D0%B5%D1%80%D0%B5%D1%80%D0%B2%D0%BD%D0%B0_%D1%96%D0%BD%D1%82%D0%B5%D0%B3%D1%80%D0%B0%D1%86%D1%96%D1%8F)
(continuous integration, CI) — практика розробки програмного забезпечення, яка полягає у
виконанні частих автоматизованих складань (build) проєкту для якнайшвидшого виявлення та
розв'язання інтеграційних проблем. При цьому також виконується запуск тестів, які, в тому
числі, мають виявляти можливі проблеми інтеграції різних змін до проєкту.

Реалізації фреймворків для модульного тестування (unit testing) та автоматичного створення
складних вхідних даних (mocking) наведенні в статтях/документації:
- С: [StackOverflow: Unit Testing C Code](https://stackoverflow.com/questions/65820/unit-testing-c-code)
- С++: [Typemock: Unit Testing With C++: The How and the Why](https://www.typemock.com/unit-testing-with-c/)
- Java: [Baeldung: Best Practices for Unit Testing in Java](https://www.baeldung.com/java-unit-testing-best-practices)
- Python: [unittest](https://docs.python.org/3/library/unittest.html)
- C#: [ByteHide: Unit Testing with C# and .NET (Full Guide)](https://www.bytehide.com/blog/unit-testing-csharp)
- Clojure: [clojure.test](https://clojure.github.io/clojure/clojure.test-api.html)
- тощо.

## Реалізація автоматизованих тестів

Тут розглядається реалізація тестів в контексті л.р. №3 і №4, але не обмежується лише цим
контекстом.

### Тестування з файлами без автоматизації

Тестування без автоматизації зазвичай виглядає так: реалізується виклик потрібної функції
з певними вхідними даними, функція робить вивід або повертає результат, і тоді коректність
того результату оцінюється "вручну" — тобто шляхом перегляду і аналізу. Для якихось
незначних завдань цього може бути й достатньо. Проте коли мова заходить про складні
програми, то таке тестувння стає недоречним і вимагатиме багато часу щоразу після внесення
змін в код.

Реалізація програми ([Main1.cpp](./src/Main1.cpp)):

```c++
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
      exit(1);
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
```

Вхідні дані ([test01.txt](./test/test01.txt)):

```
1A 21
1B 11 00 00 00 D6
0C 07 65
0D 43 00 00 0A A1
```

Результат — вивід у консоль:

```
1A 21
1B 11 XX XX XX D6
XC X7 65
XD 43 XX XX XA A1
```

### Проста автоматизація тестів з вхідними файлами

Для простої автоматизації достатньо зробити наступне:
1. Реалізувати функцію, що на вхід приймає шляхи до двох файлів: вхідниї даних та файл з
   очікуваним результатом. Ця функція робить виклик цільової функції (`run`) і виконує
   порівняння отриманого результату з очікуваним.
2. Переписати цільову функцію (`run`) таким чином, щоб вона виконувала вивід не у консоль,
   а в якийсь інший потік виводу, наприклад у файл.

Реалізація програми ([Main2.cpp](./src/Main2.cpp)):

```c++
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

// Initialize local stream variable and check if stream is opened
#define INIT_FSTREAM(type, stream, filePath, mode) \
  type stream (filePath, mode); \
  if (!(stream).is_open()) \
    { \
      std::cout << "Error opening file \'" << filePath << "\': " << std::strerror(errno) << std::endl; \
      exit(1); \
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
  // check diffrence
  int diffPos = compareFiles(outputFile, expectedFile);

  if (diffPos >= 0)
    {
      std::cout << "Test failed. Results differ on position." << diffPos << std::endl;
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
```

Вхідні дані — ті самі ([test01.txt](./test/test01.txt)).

Очікуваний результат [expected01](./test/expected01.txt):

```
1A 21
1B 11 XX XX XX D6
XC X7 65
XD 43 XX XX XA A1
```

Вивід:

```
Test passed
```

### Масштабування: багато тестів

Звісно, можна викликати програму і передавати аргументами перелік вхідних файлів, проте
якщо є певний набір тестів, який має "покрити" логіку цільової функції, і цей набір є
сталим, то можна визначити перелік тестових наборів (або ж виклики тестів) одразу в коді,
і зробити функцію, яка викличе всі тести.

Реалізація програми ([Main3.cpp](./src/Main3.cpp)):

```c++
#include <vector>

int test(const std::string& name, const std::string& inputFile, const std::string& outputFile, const std::string& expectedFile)
{
  // execute target function
  run(inputFile, outputFile);
  // check difference
  int diffPos = compareFiles(outputFile, expectedFile);
  int result = 0;

  if (diffPos >= 0)
    {
      std::cout << "Test '" << name << "' failed. Result differs on position " << diffPos << std::endl;
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
```

Вивід:

```
> ./a.out
Test 'bad test' failed. Result differs on position 1
Tests passed: 2
Tests failed: 1
Totally tests: 3
```

### Універсальний код і тести без файлів

Коли в проєкті є багато маленьких тестів (що, загалом, буває часто, бо тести бажано робити
невеликі, на окремі випадки роботи цільових функцій), тоді робота з файлами стає небажаною,
оскільки вимагатиме більше часу на виконання тестування (адже читання файлів — не таке вже
й швидке, як може здатись). В такому разі, або ж щоб уникнути розпорошення тестових даних
по багатьом файлам, тести можна реалізувати в програмному коді.

При цьому завдання може вимагати, щоб цільова функція могла читати дані саме з файлів, тож
необхідно забезпечити таку можливість, і заразом додати можливість працювати з рядками,
які можуть бути використані як вхідні дані для тестів, визначених в програмному коді. Для
цього необхідно прибрати залежність коду цільової функції від конкретного типу "потоків",
з якими вона працює (вхідна програма та результат). Це можна зробити, використавши
"батьківський" тип потоків `istream` та `ostream` в функції `run`.

Реалізація програми ([Main4.cpp](./src/Main4.cpp)):

```c++
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

// simply define lambda that calls 0-arg `fname` function
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
      std::cout << "Test " << name << " failed. Result differs on position " << diffPos << std::endl;
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
```

Вивід:

```
> ./a.out
Test 'bad test' failed. Result differs on position 1
Actual:
XX 11

Expected:
X0 11

Tests passed: 2
Tests failed: 1
Totally tests: 3
```

### Утиліти для створення вхідних даних

Для того, щоб вхідні дані та/або очікуваний результат виглядали більш зрозуміло, можна
створити утиліти, які формуватимуть відповідні дані, але при цьому матимуть вигляд, що є
більш зрозумілим.

Реалізація ([Main5.cpp](./src/Main5.cpp)):

```c++
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

// Test input utilities

std::string concatStrings(const std::vector<std::string> strs)
{
  std::string result;
  int strsN = strs.size();

  if (strsN == 0) return result;

  for (int idx = 0; idx < strsN; idx++)
    {
      result.append(strs[idx]);
    }

  return result;
}

#define MOV1(reg1, reg2) "1A" #reg1 #reg2              //MOV <4 bit> <4 bit>
#define MOV2(reg, addr) "1B1" #reg #addr               //MOV <4 bit>, <32 bit>
#define MOV3(reg, lit16) "1C1" #reg #lit16             //MOV <4 bit>, <16 bit>

#define PROGRAM(...) concatStrings(std::vector<std::string> {__VA_ARGS__})

// ==========================================================================================
// Tests
// ==========================================================================================

int test1()
{
  return test("first test",
              PROGRAM(MOV1(2, 1),
                      MOV2(1, 000000D6)),
              PROGRAM(MOV1(2, 1),
                      MOV2(1, XXXXXXD6)));
}

int test2()
{
  return test("second test",
              PROGRAM(MOV1(0, 7),
                      MOV3(0, 0001)),
              "1AX7\
1C1XXXX1");
}

int test3()
{
  return test("bad test", "00 11", "X0 11");
}
```

Вивід:

```
> ./a.out
Test 'bad test' failed. Result differs on position 1
Actual:
XX 11

Expected:
X0 11

Tests passed: 2
Tests failed: 1
Totally tests: 3
```

## "Негативні" тести

Крім перевірки роботи функцій/класів/модулів для "правильних" сценаріїв, коли вхідні дані
є коректними і очікуваними, також варто виконувати і тестування випадків, коли вхідні дані
є неочікуваними і мають призвести до помилок виконання ("падіння"). Тести на такі випадки
називаються "негативними" тестами. Наприклад, якщо функція приймає на вхід рядок, в якому
очікуваним є деяке число, проте замість числа в рядку знаходяться символи, то розбір
такого рядка може призвести до виключення (створене або помилкою розбору або прогамістом
після аналізу віхдного рядка). Для того, щоб перевірити, що станеться помилка для
некоректних вхідних даних, варто створити тест, який викличе цільову функцію з рядком
символів і "впіймає" деяке виключення, як в даному випадку буде "очікуваним" результатом.
Таким чином, негативний тест "пройде" так само, як і звичайні тести. Відповідно, в
статистиці виконання тестів він буде в групі "passed", а не "failed".
