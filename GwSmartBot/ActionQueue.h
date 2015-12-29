#pragma once

#include <assert.h>

#include "Actions.h"

class ActionQueue {
	struct Node {
		Node(Action* a) : action(a) {}

		Node* next = nullptr;
		Action* action = nullptr;
		bool visited = false;
		bool need_resume = false;
	};

public:
	ActionQueue() : head_(nullptr) {}

	bool Empty() { return head_ == nullptr; }
	const Action* Head() { return head_->action; }
	const Action* Tail() { return tail_->action; }

	void Prepend(Action* action) {
		assert(action != nullptr);
		Node* node = new Node(action);
		if (head_ == nullptr) {
			head_ = node;
			tail_ = node;
		} else {
			if (head_->visited) head_->need_resume = true;

			node->next = head_;
			head_ = node;
		}
	}

	void Append(Action* action) {
		assert(action != nullptr);
		Node* node = new Node(action);
		if (head_ == nullptr) {
			head_ = node;
			tail_ = node;
		} else {
			assert(tail_ != nullptr);
			assert(tail_->next == nullptr);
			tail_->next = node;
			tail_ = node;
		}
	}

	void Update(const World& world, bool force_resume = false) {
		if (head_ == nullptr) return;

		const Node* current_head = head_;
		if (head_->visited) {
			if (head_->need_resume || force_resume) {
				head_->action->Resume(world);
			} else {
				head_->action->Update(world);
			}

		} else {
			head_->visited = true;
			head_->action->Perform(world);
		}

		// check if the head didn't change. It may have changed in any of the above methods.
		// e.g. an action that creates and prepends sub-actions
		if (head_ == current_head) {
			if (head_->action->Done(world)) {
				Pop();
			}
		}
	}

private:
	void Pop() {
		Node* old = head_;
		head_ = head_->next;
		if (head_ == nullptr) {
			tail_ = nullptr;
		}
		delete old->action;
		delete old;
	}

	Node* head_;
	Node* tail_;
};
