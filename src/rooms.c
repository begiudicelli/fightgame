#include "arenalib.h"

Room* createRoom(int id, Enemy *enemy) {
	Room *newRoom = (Room*) malloc(sizeof(Room));
	newRoom->id = id;
	newRoom->enemy = *enemy;
	newRoom->next = NULL;
	return newRoom;
}

void addRoom(Room **head, int id, Enemy *enemy) {
	Room *newRoom = createRoom(id, enemy);
	if (*head == NULL) {
		*head = newRoom;
	} else {
		Room *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newRoom;
	}
}

void freeRoomList(Room *head) { // just in case
	while (head != NULL) {
		Room *temp = head;
		head = head->next;
		free(temp);
	}
}

