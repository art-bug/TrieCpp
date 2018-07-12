#ifndef TRIE_H
#define TRIE_H

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

	bool removeImpl(const char* key, Node* currentNode, size_t keyLength, unsigned level);

	KeyValue* addKeyValue(KeyValue* head, KeyValue* keyValue);

	KeyValue* getGreaterOfImpl(Node* currentNode, KeyValue* keyValueHead, char* key, unsigned number, unsigned level);

	public:
	
		Trie() 
			: root(new Node())
		{}

		void insert(const char* key, unsigned value);

		int get(const char* key) const;

		bool remove(const char* key);

		// Returns pointer to the head of KeyValue linked list
		KeyValue* getGreatersOf(unsigned number);

		void flush();
		
		~Trie() {}
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

bool Trie::removeImpl(const char* key, Node* currentNode, size_t keyLength, unsigned level)
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

KeyValue* Trie::getGreaterOfImpl(Node* currentNode, KeyValue* keyValueHead, char* key, unsigned number, unsigned level)
{
	KeyValue* keyValue = nullptr;

	if (currentNode->isKey) {
		if (level == strlen(key) ||
			currentNode->isLeaf())
		{
			key[level] = '\0';

			if (currentNode->value > number) {
				keyValue = new KeyValue(key, currentNode->value);
			}
		}
	}

	for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
		Node* currentLink = currentNode->links[i];

		if (currentLink) {
			key[level] = i + 'a';

			KeyValue* resultKeyValue = getGreaterOfImpl(currentLink, keyValueHead, key, number, level + 1);

			return resultKeyValue;
		}
	}

	return keyValue;
}

KeyValue* Trie::getGreatersOf(unsigned number)
{
	KeyValue* keyValueHead = nullptr;

	if (root) {
		for (unsigned i = 0; i < ENG_ALPHABET_SIZE; ++i) {
			Node* currentLink = root->links[i];

			if (currentLink) {
				KeyValue* keyValue = getGreaterOfImpl(currentLink, keyValueHead, new char(i + 'a'), number, 1);

				if (keyValue) {
					if (!keyValueHead) {
						keyValueHead = keyValue;
					}
					else {
						KeyValue* newKeyValueHead = keyValueHead;

						while (newKeyValueHead &&
							   newKeyValueHead->next)
						{
							newKeyValueHead = newKeyValueHead->next;
						}

						newKeyValueHead->next = keyValue;
					}
				}
			}
		}
	}

	return keyValueHead;
}

void Trie::flush()
{
}

#endif // TRIE_H