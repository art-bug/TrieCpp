#include "trie.h"
#include <iostream>

int main()
{
	Trie *trie = new Trie();

	trie->insert("hello", 1);
	trie->insert("friend", 3);
	trie->insert("stick", 5);
	trie->insert("mystery", 4);
	trie->insert("window", 8);
	trie->insert("spider", 12);

	if (false)
	{
		int value = trie->get("hello");

		if (value == -1) {
			std::cout << "There is no key \"hello\"";
		}
		else {
			std::cout << value;
		}

		if (false) {
			std::cout << std::endl;

			bool isRemoved = trie->remove("stick");

			if (isRemoved) {
				std::cout << "\"stick\" is removed";
			}
			else {
				std::cout << "Fail with removing";
			}
		}

		std::cout << std::endl;

		int stickValue = trie->get("stick");

		if (stickValue == -1) {
			std::cout << "There is no key \"stick\"";
		}
		else {
			std::cout << stickValue;
		}

		std::cout << std::endl;

		const char* strings[5] = { "hello", "friend", "mystery", "window", "spider" };

		unsigned need = 3;

		for (auto key : strings) {

			unsigned value = trie->get(key);

			if (value > need) {
				std::cout << value << " of \"" << key << "\" greater than " << need << std::endl;
			}
		}
	}

	unsigned need = 3;

	KeyValue *keyValueHead = trie->getGreatersOf(need);

	if (!keyValueHead) {
		std::cout << "List is empty" << std::endl;
	}
	else {
		std::cout << "greater than " << need << std::endl;

		std::cout << keyValueHead->key << ' ' << keyValueHead->value << std::endl;

		while (keyValueHead->next) {
			keyValueHead = keyValueHead->next;
			std::cout << keyValueHead->key << ' ' << keyValueHead->value << std::endl;
		}
	}

	std::cin.get();

	return 0;
}