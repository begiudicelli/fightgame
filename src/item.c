#include "arenalib.h"

Item itemDatabase[] = {
    // Wands (Tipo 1)
    {1, "Cajado de Madeira", 5.0f, 10.0f, 1, NULL},
    {2, "Cajado de Ferro", 10.0f, 20.0f, 1, NULL},
    {3, "Cajado de Prata", 15.0f, 40.0f, 1, NULL},
    {4, "Cajado de Ouro", 20.0f, 60.0f, 1, NULL},
    {5, "Cajado Magico Supremo", 30.0f, 100.0f, 1, NULL},

    // Hats (Tipo 2)
    {6, "Chapeu de Feiticeiro Iniciante", 5.0f, 20.0f, 2, NULL},
    {7, "Chapeu de Feiticeiro Adepto", 10.0f, 40.0f, 2, NULL},
    {8, "Chapeu de Feiticeiro Avancado", 15.0f, 60.0f, 2, NULL},
    {9, "Chapeu de Feiticeiro Arcanista", 20.0f, 80.0f, 2, NULL},
    {10, "Chapeu de Feiticeiro Supremo", 30.0f, 120.0f, 2, NULL},

    // Spellbooks (Tipo 3)
    {11, "Livro de Feiticos Iniciante", 10.0f, 30.0f, 3, NULL},
    {12, "Livro de Feiticos Adepto", 15.0f, 50.0f, 3, NULL},
    {13, "Livro de Feiticos Avancado", 20.0f, 70.0f, 3, NULL},
    {14, "Livro de Feiticos Arcanista", 25.0f, 90.0f, 3, NULL},
    {15, "Livro de Feiticos Supremo", 35.0f, 150.0f, 3, NULL}
};

const int itemDatabaseCount = sizeof(itemDatabase) / sizeof(itemDatabase[0]);

Inventory* createInventory() {
    Inventory *inventory = (Inventory*)malloc(sizeof(Inventory));
    inventory->head = NULL;
    inventory->itemCount = 0;
    return inventory;
}

void addItem(Inventory *inventory, int id, const char *name, float value, float price, int type) {
    Item *newItem = (Item*)malloc(sizeof(Item));
    newItem->id = id;
    newItem->name = strdup(name);
    newItem->value = value;
    newItem->price = price;
    newItem->type = type;
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
        printf("Item ID: %d, Name: %s\n", current->id, current->name);
        current = current->next;
    }
}

void displayShop(Player *player) {
    printf("Bem-vindo a loja! Aqui estao os itens disponiveis:\n");
    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        if (player->purchasedItems[i]) {
            printf("Item %s ja foi comprado anteriormente.\n", itemDatabase[i].name);
        } else {
            printf("ID: %d, Nome: %s, Preco: %.2f GOLD\n",
                   itemDatabase[i].id, itemDatabase[i].name, itemDatabase[i].price);
        }
    }
    int itemId;
    printf("Digite o ID do item que voce deseja comprar: ");
    scanf("%d", &itemId);

    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        if (itemDatabase[i].id == itemId) {
            if (player->purchasedItems[i]) {
                printf("Este item ja foi comprado!\n");
                return;
            }
            if (player->gold >= itemDatabase[i].price) {
                player->gold -= itemDatabase[i].price;
                addItem(player->inventory, itemDatabase[i].id, itemDatabase[i].name,
                        itemDatabase[i].value, itemDatabase[i].price, itemDatabase[i].type);
                player->purchasedItems[i] = true;
                printf("Voce comprou %s!\n", itemDatabase[i].name);
            } else {
                printf("Voce nao tem ouro suficiente para comprar %s!\n", itemDatabase[i].name);
            }
            return;
        }
    }
    printf("Item nao encontrado!\n");
}

void displayEquippedItems(Player *player) {
    printf("Itens Equipaveis no Inventario:\n");
    Item *current = player->inventory->head;
    while (current != NULL) {
        if (current->type == 1) {
            printf("ID: %d, Nome: %s (Varinha)\n", current->id, current->name);
        } else if (current->type == 2) {
            printf("ID: %d, Nome: %s (Chapeu)\n", current->id, current->name);
        } else if (current->type == 3) {
            printf("ID: %d, Nome: %s (Livro de Feiticos)\n", current->id, current->name);
        }
        current = current->next;
    }
}


void equipItemFromInventory(Player *player) {
    displayEquippedItems(player);

    int itemId;
    printf("Digite o ID do item que voce deseja equipar: ");
    scanf("%d", &itemId);

    Item *current = player->inventory->head;
    while (current != NULL) {
        if (current->id == itemId) {
            equippItem(player, current);
            return;
        }
        current = current->next;
    }
    printf("Item nao encontrado no inventario.\n");
}


void equippItem(Player *player, Item *item) {
    if (item->type == 1) {
    	player->equippedItems.wand = item;
    	player->critChance += item->value / 100.0;
    	printf("Voce equipou %s (Varinha)! Chance de Critico aumentada para %.2f%%!\n", item->name, player->critChance * 100);
    }
    if (item->type == 2) {
        player->equippedItems.hat = item;
        player->health += item->value;
        printf("Voce equipou %s (Chapeu)! Aumentou sua vida em %.2f\n", item->name, item->value);
    }
    if (item->type == 3) {
        player->equippedItems.spellbook = item;
        printf("Voce equipou %s como seu livro de feiticos!\n", item->name);
    }
}


void listEquippedItems(Player *player) {
    printf("Itens Equipados:\n");
    printf("Varinha: %s\n", player->equippedItems.wand ? player->equippedItems.wand->name : "Nenhuma");
    printf("Chapeu: %s\n", player->equippedItems.hat ? player->equippedItems.hat->name : "Nenhum");
    printf("Livro de Feiticos: %s\n", player->equippedItems.spellbook ? player->equippedItems.spellbook->name : "Nenhum");
}
