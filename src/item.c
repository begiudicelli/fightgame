#include "arenalib.h"

Item itemDatabase[] = {
    {1, "Pocao de vida pequena", 0.3f, 5},
    {2, "Pocao de vida media", 0.2f, 10},
    {3, "Pocao de vida grande", 0.1f, 15}
};

ShopItem shopDatabase[] = {
    {{4, "Cajado madeira", 0, 10}, 10.0f, 1},
    {{5, "Chapeu feiticeiro iniciante", 0, 20}, 20.0f, 2},
    {{6, "Livro de feitiços", 0, 30}, 30.0f, 3}
};

const int shopItemCount = sizeof(shopDatabase) / sizeof(shopDatabase[0]);

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
            printf("Item %s já existe, quantidade aumentada para %d.\n", name, current->amount);
            return;
        }
        current = current->next;
    }
    Item *newItem = (Item*)malloc(sizeof(Item));
    newItem->id = id;
    newItem->name = strdup(name);
    newItem->amount = 1;
    newItem->next = inventory->head;
    newItem->dropChance = 0.0f;
    newItem->value = 0;
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

void dropItem(Player *player) {
    for (int i = 0; i < itemPoolSize; i++) {
        float chance = (float)rand() / RAND_MAX; // Gera um número aleatório entre 0 e 1
        if (chance < itemDatabase[i].dropChance) {
            addItem(player->inventory, itemDatabase[i].id, itemDatabase[i].name);
            printf("Você ganhou o item %s!\n", itemDatabase[i].name);
        } else {
            printf("Item %s não foi dropado.\n", itemDatabase[i].name);
        }
    }
}

void displayShop(Player *player) {
    printf("Bem-vindo à loja! Aqui estão os itens disponíveis:\n");

    // Lista os itens da loja
    for (int i = 0; i < shopItemCount; i++) {
        // Verifica se o item já foi comprado
        if (player->purchasedItems[i]) {
            printf("Item %s já foi comprado anteriormente.\n", shopDatabase[i].item.name);
        } else {
            printf("ID: %d, Nome: %s, Preço: %.2f GOLD\n",
                   shopDatabase[i].item.id,
                   shopDatabase[i].item.name,
                   shopDatabase[i].price);
        }
    }

    int itemId;
    printf("Digite o ID do item que você deseja comprar: ");
    scanf("%d", &itemId);

    for (int i = 0; i < shopItemCount; i++) {
        if (shopDatabase[i].item.id == itemId) {
            if (player->purchasedItems[i]) {
                printf("Este item já foi comprado!\n");
                return;
            }

            if (player->gold >= shopDatabase[i].price) {
                player->gold -= shopDatabase[i].price;
                addItem(player->inventory, shopDatabase[i].item.id, shopDatabase[i].item.name);

                player->purchasedItems[i] = true;

                printf("Você comprou %s!\n", shopDatabase[i].item.name);
            } else {
                printf("Você não tem ouro suficiente para comprar %s!\n", shopDatabase[i].item.name);
            }
            return;
        }
    }

    printf("Item não encontrado!\n");
}


void displayEquippedItems(Player *player) {
    printf("Itens Equipáveis no Inventário:\n");

    Item *current = player->inventory->head;
    while (current != NULL) {
        for (int i = 0; i < shopItemCount; i++) {
            if (current->id == shopDatabase[i].item.id) {
                printf("ID: %d, Nome: %s\n", current->id, current->name);
            }
        }
        current = current->next;
    }
}

void equipItemFromInventory(Player *player) {
    displayEquippedItems(player);

    int itemId;
    printf("Digite o ID do item que você deseja equipar: ");
    scanf("%d", &itemId);

    Item *current = player->inventory->head;
    while (current != NULL) {
        if (current->id == itemId) {
            equippItem(player, current);
            printf("Você equipou %s!\n", current->name);
            return;
        }
        current = current->next;
    }
    printf("Item não encontrado no inventário.\n");
}

void equippItem(Player *player, Item *item) {
    if (item->id == 4) { // Por exemplo, varinha
        player->equippedItems.wand = item;
    }
    if (item->id == 5) { // Por exemplo, chapéu
        player->equippedItems.hat = item;
    }
    if (item->id == 6) { // Por exemplo, livro de feitiços
        player->equippedItems.spellbook = item;
    }
}

void listEquippedItems(Player *player) {
    printf("Itens Equipados:\n");
    printf("Varinha: %s\n", player->equippedItems.wand ? player->equippedItems.wand->name : "Nenhuma");
    printf("Chapéu: %s\n", player->equippedItems.hat ? player->equippedItems.hat->name : "Nenhum");
    printf("Livro de Feitiços: %s\n", player->equippedItems.spellbook ? player->equippedItems.spellbook->name : "Nenhum");
}

