//! \file
//! MenuItem and derived class declarations

#pragma once

#include <vector>
#include <string>
#include "Player.h"

class MenuItem
{
	public:
		virtual void Forward() { }
		virtual void Backward() { }
		virtual void Render(int x, int y) const = 0;
};

class FunctionalMenuItem : public MenuItem
{
	public:
		FunctionalMenuItem(
				std::string text,
				void (*forwardFunction)(),
				void (*backwardFunction)()) :
			Text(text),
			ForwardFunction(forwardFunction),
			BackwardFunction(backwardFunction) { }
		void Forward() { ForwardFunction(); }
		void Backward() { BackwardFunction(); }
		void Render(int x, int y) const;

	private:
		std::string Text;
		void (*ForwardFunction)();
		void (*BackwardFunction)();
};

template <class T>
class EnumMenuItem : public MenuItem
{
	public:
		EnumMenuItem(T *value, int numItems) :
			Value(value), NumItems(numItems) { }
		virtual void Forward()
		{
			if ((int)(*Value) < NumItems - 1)
			{
				*Value = (T)(*Value + 1);
			}
			else
			{
				*Value = (T)0;
			}
		}
		virtual void Backward()
		{
			if ((int)(*Value) > 0)
			{
				*Value = (T)(*Value - 1);
			}
			else
			{
				*Value = (T)(NumItems - 1);
			}
		}

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
