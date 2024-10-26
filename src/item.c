#include "arenalib.h"

Item itemDatabase[] = {
    {1, "Pocao de vida pequena"},
	{2, "Pocao de vida media"},
	{3, "Pocao de vida grande"}
};

const int itemPoolSize = sizeof(itemDatabase) / sizeof(itemDatabase[0]);

Inventory* createInventory() {
    Inventory *inventory = (Inventory*)malloc(sizeof(Inventory));
    inventory->head = NULL;
    inventory->itemCount = 0;
    return inventory;
}

void addItem(Inventory *inventory, int id, const char *name) {
    Item *current = inventory->head;
    while (current != NULL) {
        if (current->id == id) {
            current->amount++;
            return;
        }
        current = current->next;
    }
    Item *newItem = (Item*)malloc(sizeof(Item));
    newItem->id = id;
    newItem->name = strdup(name);
    newItem->amount = 1;
    newItem->next = inventory->head;
    inventory->head = newItem;
    inventory->itemCount++;
}

bool removeItem(Inventory *inventory, int id) {
    Item *current = inventory->head;
    Item *previous = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (previous == NULL) {
                inventory->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->name);
            free(current);
            inventory->itemCount--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

void listItems(Inventory *inventory) {
    Item *current = inventory->head;
    while (current != NULL) {
        printf("Item ID: %d, Name: %s Quantidade: %d\n", current->id, current->name, current->amount);
        current = current->next;
    }
}

Item getRandomItem() {
    int randomIndex = rand() % itemPoolSize;
    return itemDatabase[randomIndex];
}
