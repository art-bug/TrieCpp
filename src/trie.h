#ifndef TRIE_H
#define TRIE_H

#define _CRT_SECURE_NO_WARNINGS

#define ENG_ALPHABET_SIZE 26
#include <limits.h>
#include <string.h>

inline unsigned index(char keyChar)
{
	return keyChar - 'a';
}

struct Node {

	unsigned value;

	bool isKey;

	Node* links[ENG_ALPHABET_SIZE];

	bool isLeaf() const;

	Node()
		: value(UINT_MAX), isKey(false), links()
	{}
};


bool Node::isLeaf() const
{
	for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
		if (this->links[i]) {
			return false;
		}
	}

	return true;
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
	Node *root;

	KeyValue *keyValueHead;

	bool removeImpl(const char*, Node *, size_t, unsigned);

	//KeyValue* addKeyValue(KeyValue*, KeyValue*);

	void getGreaterOfImpl(Node *, char*, unsigned, unsigned);

public:

	Trie()
		: root(new Node()), keyValueHead(nullptr)
	{}

	void insert(const char*, unsigned);

	int get(const char*) const;

	bool remove(const char*);

	// Returns pointer to the head of KeyValue linked list
	KeyValue* getGreatersOf(unsigned);

	void flush();

	~Trie();
};


void Trie::insert(const char* key, unsigned value)
{
	Node *currentNode = root;

	if (strlen(key) > 0) {
		for (size_t i = 0; i < strlen(key); ++i) {

			if (!currentNode->links[index(key[i])]) {
				currentNode->links[index(key[i])] = new Node();
			}

			currentNode = currentNode->links[index(key[i])];
		}

		currentNode->value = value;
		currentNode->isKey = true;
	}
}

int Trie::get(const char* key) const
{
	if (root || strlen(key) > 0) {
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

bool Trie::removeImpl(const char* key, Node *currentNode, size_t keyLength, unsigned level)
{
	if (currentNode) {
		if (level == keyLength) {
			if (currentNode->isKey) {
				currentNode->isKey = false;
				currentNode->value = UINT_MAX;

				if (currentNode->isLeaf()) {
					delete currentNode;
					currentNode = nullptr;
				}

				return true;
			}
		}
		else {
			Node* linkNode = currentNode->links[index(key[level])];

			if (removeImpl(key, linkNode, keyLength, level + 1)) {

				if (linkNode->isLeaf()) {
					delete linkNode;
					linkNode = nullptr;
				}
				else {
					linkNode->isKey = false;
					linkNode->value = UINT_MAX;
				}

				return true;
			}
		}
	}

	return false;
}

bool Trie::remove(const char* key)
{
	if (!root || strlen(key) == 0) {
		return false;
	}

	return removeImpl(key, root, strlen(key), 0);
}

void Trie::getGreaterOfImpl(Node *currentNode, char* prefix, unsigned number, unsigned level)
{
	if (currentNode->isKey) {
		if (level == strlen(prefix) ||
			currentNode->isLeaf())
		{
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

			return;
		}
	}

	for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
		Node* currentLink = currentNode->links[i];

		if (currentLink) {
			prefix[level] = i + 'a';
			getGreaterOfImpl(currentLink, prefix, number, level + 1);
		}
	}
}

KeyValue* Trie::getGreatersOf(unsigned number)
{
	if (keyValueHead) {
		delete keyValueHead;
		keyValueHead = nullptr;
	}

	if (root) {
		for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
			Node* currentLink = root->links[i];

			if (currentLink) {
				getGreaterOfImpl(currentLink, new char(i + 'a'), number, 1);
			}
		}
	}

	return keyValueHead;
}

void Trie::flush()
{
}

Trie::~Trie()
{
	if (keyValueHead) {
		delete keyValueHead;
		keyValueHead = nullptr;
	}
}

#endif // TRIE_H