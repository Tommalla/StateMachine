/* Tomasz [Tommalla] Zakrzewski, 2014
 */
#include <cstdio>
#include <unordered_map>

#include "EnumHash.hpp"

using namespace std;

enum class State {
	A = 10,
	B,
	C = 0
};

int main() {
	unordered_map<State, string, EnumHash<State>> m;

	m[State::A] = "A";
	m[State::B] = "B";
	m[State::C] = "C";

	printf("%s %s %s\n", m[State::A].c_str(), m[State::B].c_str(), m[State::C].c_str());

	unordered_map<string, string, EnumHash<string>> m2;

	m2["bar"] = "foo";

	printf("bar: %s, asd: %s\n", m2["bar"].c_str(), m2["asd"].c_str());
}