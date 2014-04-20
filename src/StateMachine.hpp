/* Tomasz [Tommalla] Zakrzewski, 2014
 * State Machine implementation
 */
#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP
#include <functional>
#include <unordered_map>

#include "EnumHash.hpp"

template<class State, class Transition>
class StateMachine {
	typedef std::function<void(void)> Action;
public:
	StateMachine();

	/**
	 * @brief Tries to perform a transition from the current state using transition.
	 *
	 * @return True if transition succeeded. False otherwise.
	 */
	bool trigger(const Transition& transition);
	/**
	 * @brief Returns current state
	 */
	State getCurrentState() const;

	void addState(const State& state, const Action& enterAction = nullptr, const Action& exitAction = nullptr);
	/**
	 * @brief Adds a transition. Note that this implementation represents a deterministic automata - if a transition
	 * from ``from`` using ``transition`` already exists and it leads to a state other than ``to`` it will be lost.
	 */
	void addTransition(const State& from, const State& to, const Transition& transition);

	void setEnterAction(const State& state, const Action& enterAction);
	void setExitAction(const State& state, const Action& exitAction);

	/**
	 * @brief Starts the state machine from initialState
	 */
	void start(const State& initialState);

private:
	void executeActionsForTransition(const State& from, const State& to) const;
	void executeAction(const Action& f) const;

	typedef std::unordered_map<Transition, State, EnumHash<Transition>> Node;

	bool started;
	State currentState;
	std::unordered_map<State, Node, EnumHash<State>> graph;
	std::unordered_map<State, Action,
		EnumHash<State>> enterActions;
	std::unordered_map<State, Action,
		EnumHash<State>> exitActions;
};

template<class State, class Transition>
StateMachine<State, Transition>::StateMachine()
: started{false} {}

template<class State, class Transition>
bool StateMachine<State, Transition>::trigger(const Transition& transition) {
	if (!started)
		return false;	//machine not yet started, currentState invalid

	auto m = graph[currentState];
	auto iter = m.find(transition);

	if (iter != m.end()) {
		executeActionsForTransition(currentState, iter->second);
		currentState = iter->second;
		return true;
	}

	return false;
}

template<class State, class Transition>
State StateMachine<State, Transition>::getCurrentState() const {
	return currentState;
}

template<class State, class Transition>
void StateMachine<State, Transition>::addState(const State& state, const Action& enterAction, const Action& exitAction) {
	auto iter = graph.find(state);
	if (iter == graph.end()) {
		graph.emplace_hint(iter, state, Node());
		enterActions[state] = enterAction;
		exitActions[state] = exitAction;
	}
}

template<class State, class Transition>
void StateMachine<State, Transition>::addTransition(const State& from, const State& to, const Transition& transition) {
	addState(from);
	addState(to);

	graph[from][transition] = to;
}

template<class State, class Transition>
void StateMachine<State, Transition>::setEnterAction(const State& state, const Action& enterAction) {
	enterActions[state] = enterAction;
}

template<class State, class Transition>
void StateMachine<State, Transition>::setExitAction(const State& state, const Action& exitAction) {
	exitActions[state] = exitAction;
}

template<class State, class Transition>
void StateMachine<State, Transition>::start(const State& initialState) {
	if (started)
		executeActionsForTransition(currentState, initialState);
	else
		executeAction(enterActions[initialState]);
	currentState = initialState;
	started = true;
}

template<class State, class Transition>
void StateMachine<State, Transition>::executeActionsForTransition(const State& from, const State& to) const {
	std::function<void(void)> fns[] = {exitActions.at(from), enterActions.at(to)};
	for (const auto& f: fns)
		executeAction(f);
}

template<class State, class Transition>
void StateMachine<State, Transition>::executeAction(const Action& f) const {
	if (f != nullptr)
		f();
}

#endif // STATE_MACHINE_HPP