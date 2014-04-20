/* Tomasz [Tommalla] Zakrzewski, 2014
 */
#include <cstdio>
#include <ctime>

#include "EnumHash.hpp"
#include "StateMachine.hpp"

using namespace std;

enum class State {
	STANDING,
	WALKING,
	JUMPING,
	DIVING
};

enum class Event {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	GAME_EVENT
};

int main() {
	srand(time(NULL));
	State states[] = {State::STANDING, State::WALKING, State::JUMPING, State::DIVING};
	const char* stateNames[] = {"standing", "walking", "jumping", "diving"};
	Event events[] = {Event::UP, Event::DOWN, Event::LEFT, Event::RIGHT, Event::GAME_EVENT};
	const char* eventNames[] = {"UP", "DOWN", "LEFT", "RIGHT", "GAME_EVENT"};
	//transitions
	State from[] = {State::STANDING, State::STANDING, State::STANDING, State::WALKING, State::WALKING, State::JUMPING, State::JUMPING, State::DIVING};
	State to[] = {State::WALKING, State::WALKING, State::JUMPING, State::JUMPING, State::STANDING, State::DIVING, State::STANDING, State::STANDING};
	Event transitions[] = {Event::RIGHT, Event::LEFT, Event::UP, Event::UP, Event::GAME_EVENT, Event::DOWN, Event::GAME_EVENT, Event::GAME_EVENT};
	StateMachine<State, Event> s;
	for (int i = 0; i < 5; ++i)
		s.addState(states[i], [&,i](){printf("Changed state to %s\n", stateNames[i]);});

	for (int i = 0; i < 8; ++i)
		s.addTransition(from[i], to[i], transitions[i]);

	s.start(State::STANDING);

	for (int i = 0; i < 20; ++i) {
		int id = rand() % 5;
		printf("Event: %s\n", eventNames[id]);
		s.trigger(events[id]);
	}

	return 0;
}