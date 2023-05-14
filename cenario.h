#ifndef CENARIO_H_INCLUDED
#define CENARIO_H_INCLUDED

#include <vector>
#include <string>

extern std::vector<std::string> cenario0;
extern std::vector<std::string> cenario1;
extern std::vector<std::string> cenario2;

std::vector<std::string>& getCenario(int index);

#endif // CENARIO_H_INCLUDED
