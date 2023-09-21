from state import State


# Simple container to track the current flight when parsing through states
class Flight:
	id: int
	first_tuple: State
	first_seen: int
	last_seen: int
	last_tuple: State
