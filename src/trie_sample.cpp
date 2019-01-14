#include "trie.h"
#include <iostream>

void printValue(Trie* &trie, const char* someKey) 
{
	int value = trie->getBy(someKey);

	if (value == -1) {
		std::cout << "There is no key " << "\"" << someKey << "\"";
	}
	else {
		std::cout << "The value of " << someKey << " is " << value;
	}

	std::cout << std::endl << std::endl;
}

void printList(KeyValue* &listHead, unsigned someNumber)
{
	if (!listHead) {
		std::cout << "List is empty" << std::endl;
	}
	else {
		std::cout << "Values greater than " << someNumber << ':' << std::endl;

		std::cout << listHead->key << ' ' << listHead->value << std::endl;

		while (listHead->next) {
			listHead = listHead->next;
			std::cout << listHead->key << ' ' << listHead->value << std::endl;
		}
	}
}

void printKeyIsRemoved(Trie* &trie, const char* key)
{
	if (trie->remove(key)) {
		std::cout << "\"" << key << "\" is removed";
	}
	else {
		std::cout << "Fail with removing the key \"" << key << "\"";
	}

	std::cout << std::endl << std::endl;
}

int main()
{
	Trie *trie = new Trie();

	system("chcp 1251");

	std::cout << "Trie initialization" << std::endl << std::endl;
	
	printValue(trie, "hello");
	
	printValue(trie, "music");

	KeyValue *keyValueHead = trie->getValuesGreaterThan(3);

	printList(keyValueHead, 3);

	std::cout << std::endl << "//////////////////////////////////////////////" << std::endl;
	std::cout << std::endl << "Inserting the keys" << std::endl;

	trie->insert("hello", 1);
	trie->insert("I", 12);
	trie->insert("friend", 3);
	trie->insert("свинья", 5);
	trie->insert("mystery", 4);
	trie->insert("music", 9);
	trie->insert("яблоко", 8);
	trie->insert("m", 41);
	trie->insert("S", 35);
	trie->insert("window", 8);
	trie->insert("w", 10);
	trie->insert("324kkff", 50);
	trie->insert("fg3@k-ff", 56);
	trie->insert("    ", 30);
	trie->insert("spider", 12);
	trie->insert("stick", 16);
	trie->insert("", 46);
	trie->insert("spy", 16);

	std::cout << "Inserting is done" << std::endl << std::endl;

	printValue(trie, "music");

	printValue(trie, "свинья");

	printKeyIsRemoved(trie, "stick");

	printKeyIsRemoved(trie, "music");

	printValue(trie, "music");

	printKeyIsRemoved(trie, "яблоко");

	printValue(trie, "яблоко");

	std::cout << "///////////////////////////////////////////////////" << std::endl << std::endl;

	const char* keys[5] = { "hello", "friend", "mystery", "window", "spider" };

	for (auto key : keys) {

		unsigned value = trie->getBy(key);

		if (value > 3) {
			std::cout << value << " of \"" << key << "\" greater than " << 3 << std::endl;
		}
	}

	std::cout << std::endl;

	keyValueHead = trie->getValuesGreaterThan(5);

	printList(keyValueHead, 5);

	std::cin.get();

	return 0;
}