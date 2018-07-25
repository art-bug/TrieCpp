#ifndef TRIE_H
#define TRIE_H

#define _CRT_SECURE_NO_WARNINGS

#define ENG_ALPHABET_SIZE 26
#include <limits.h>
#include <string.h>
#include <ctype.h>

inline unsigned index(char keyChar)
{
	return keyChar - 'a';
}


struct KeyValue {
	const char* key;
	unsigned value;

	KeyValue* next;

	KeyValue()
		: key(new char()), value(UINT_MAX), next(nullptr)
	{}

	KeyValue(const char* key, unsigned value)
		: key(key), value(value), next(nullptr)
	{}
};


class Trie
{
	struct Node {

		unsigned value;

		bool isKey;

		Node* links[ENG_ALPHABET_SIZE];

		bool isLeaf() const;

		Node()
			: value(UINT_MAX), isKey(false), links()
		{}
	};

	Node *root;

	KeyValue *keyValueHead;

	bool isCorrectKey(const char*) const;

	bool innerRemove(const char*, Node* &, unsigned);

	void innerGetValuesGreaterThan(Node* &, char*, unsigned, unsigned);

	void clearKeyValueList();

	// Removing the trie from memory
	void flush(Node* &);

	public:
	
		Trie()
			: root(new Node()), keyValueHead(nullptr)
		{}
	
		void insert(const char*, unsigned);
	
		int getBy(const char*) const;
	
		bool remove(const char*);
	
		// Returns a pointer to the KeyValue head of the linked list
		KeyValue* getValuesGreaterThan(unsigned);
	
		~Trie();
};


bool Trie::Node::isLeaf() const
{
	for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
		if (this->links[i]) {
			return false;
		}
	}

	return true;
}

bool Trie::isCorrectKey(const char* key) const
{
	size_t keyLength = strlen(key);

	if (keyLength > 0) {
		for (size_t i = 0; i < keyLength; i++) {
			if (!(key[i] >= 'a' && key[i] <= 'z' ||
				  key[i] >= 'A' && key[i] <= 'Z'))
			{
				return false;
			}
		}
	}
	else {
		return false;
	}

	return true;
}

void Trie::insert(const char* key, unsigned value)
{
	Node *currentNode = root;

	if (isCorrectKey(key)) {
		for (size_t i = 0; i < strlen(key); ++i) {

			char lowerCaseChar = tolower(key[i]);

			if (!currentNode->links[index(lowerCaseChar)]) {
				currentNode->links[index(lowerCaseChar)] = new Node();
			}

			currentNode = currentNode->links[index(lowerCaseChar)];
		}

		currentNode->value = value;
		currentNode->isKey = true;
	}
}

int Trie::getBy(const char* key) const
{
	if (root && strlen(key) > 0 && isCorrectKey(key)) {
		Node *currentNode = root;

		for (size_t i = 0; i < strlen(key); ++i) {

			if (!currentNode->links[index(key[i])]) {
				return -1;
			}

			currentNode = currentNode->links[index(key[i])];
		}

		if (currentNode->isKey) {
			return currentNode->value;
		}
	}

	return -1;
}

bool Trie::innerRemove(const char* key, Node* &currentNode, unsigned level)
{
	if (level == strlen(key)) {
		if (!currentNode->isKey) {
			return false;
		}

		currentNode->isKey = false;
		currentNode->value = UINT_MAX;

		return currentNode->isLeaf();
	}

	Node *linkNode = currentNode->links[index(key[level])];

	if (!linkNode) {
		return false;
	}

	bool shouldDeleteCurrentNode = innerRemove(key, linkNode, level + 1);

	if (shouldDeleteCurrentNode) {
		delete linkNode;
		linkNode = nullptr;
		currentNode->links[index(key[level])] = linkNode;

		return currentNode->isLeaf();
	}

	return true;
}

bool Trie::remove(const char* key)
{
	if (!root || strlen(key) == 0 || !isCorrectKey(key)) {
		return false;
	}

	return innerRemove(key, root, 0);
}

void Trie::innerGetValuesGreaterThan(Node* &currentNode, char* prefix, unsigned number, unsigned level)
{
	if (currentNode->isKey) {
		prefix[level] = '\0';

		if (currentNode->value > number) {
			KeyValue *keyValue = new KeyValue(strcpy(new char[strlen(prefix)], prefix), currentNode->value);

			if (!keyValueHead) {
				keyValueHead = keyValue;
			}
			else {
				KeyValue* newKeyValueHead = keyValueHead;

				while (newKeyValueHead->next) {
					newKeyValueHead = newKeyValueHead->next;
				}

				newKeyValueHead->next = keyValue;
			}
		}
	}

	if (!currentNode->isLeaf()) {
		for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
			Node* currentLink = currentNode->links[i];

			if (currentLink) {
				prefix[level] = i + 'a';
				innerGetValuesGreaterThan(currentLink, prefix, number, level + 1);
			}
		}
	}
}

void Trie::clearKeyValueList()
{
	if (keyValueHead) {
		KeyValue *currentHead = keyValueHead;
		KeyValue *next = nullptr;

		while (currentHead) {
			next = currentHead->next;
			delete currentHead;
			currentHead = next;
		}

		keyValueHead = nullptr;
	}
}

KeyValue* Trie::getValuesGreaterThan(unsigned number)
{
	// Clear the list before using
	clearKeyValueList();

	if (root) {
		innerGetValuesGreaterThan(root, new char(), number, 0);
	}

	return keyValueHead;
}

void Trie::flush(Node* &currentNode)
{
	if (root) {
		if (!currentNode) {
			return;
		}

		for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
			Node* currentLink = currentNode->links[i];

			if (currentLink) {
				flush(currentLink);
				delete currentLink;
				currentLink = nullptr;
				currentNode->links[i] = currentLink;
			}
		}
	}
}

Trie::~Trie()
{
	clearKeyValueList();

	flush(root);

	if (root) {
		delete root;
		root = nullptr;
	}
}

#endif // TRIE_H