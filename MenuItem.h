//! \file
//! MenuItem and derived class declarations

#pragma once

#include <vector>
#include <string>
#include "Player.h"
#include "Renderer.h"

class MenuItem
{
	public:
		virtual void Forward() { }
		virtual void Backward() { }
		virtual void Render(int x, int y) const = 0;
};

template <class T>
class FunctionalMenuItem : public MenuItem
{
	public:
		FunctionalMenuItem(
				std::string text,
				void (*forwardFunction)(T *),
				void (*backwardFunction)(T *),
				T *instance = NULL) :
			Text(text),
			ForwardFunction(forwardFunction),
			BackwardFunction(backwardFunction),
			Instance(instance) { }
		void Forward() { ForwardFunction(Instance); }
		void Backward() { BackwardFunction(Instance); }
		void Render(int x, int y) const { Renderer::DrawText(Text, 24, x, y); }

	private:
		std::string Text;
		void (*ForwardFunction)(T *);
		void (*BackwardFunction)(T *);
		T *Instance;
};

template <class T>
class EnumMenuItem : public MenuItem
{
	public:
		EnumMenuItem(T *value, int numItems) :
			Value(value), NumItems(numItems) { }
		virtual void Forward()
		{ *Value = (T)(((int)(*Value) < NumItems - 1) ? (*Value + 1) : 0); }
		virtual void Backward()
		{ *Value = (T)(((int)(*Value) > 0) ? (*Value - 1) : (NumItems - 1)); }

	protected:
		T *Value;
		int NumItems;
};

class FieldMenuItem : public EnumMenuItem<unsigned int>
{
	public:
		FieldMenuItem(unsigned int *value) :
			EnumMenuItem<unsigned int>(value, 2) { }
		void Render(int x, int y) const;
};

class CharacterMenuItem : public EnumMenuItem<Character>
{
	public:
		CharacterMenuItem(Character *value) :
			EnumMenuItem<Character>(value, Character_NUMITEMS) { }
		void Forward();
		void Backward();
		void Render(int x, int y) const;
};

class ToggleMenuItem : public MenuItem
{
	public:
		ToggleMenuItem(bool *value, std::string active, std::string inactive) :
			Value(value), Active(active), Inactive(inactive) { }
		void Forward();
		void Backward();
		void Render(int x, int y) const;
	private:
		bool *Value;
		std::string Active;
		std::string Inactive;
};
