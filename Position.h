//! \file
//! Position class declaration

#pragma once

class Position
{
	public:
		int X;
		int Y;

		Position(int x = 0, int y = 0)
		{
			X = x;
			Y = y;
		}

		Position(const Position &other)
		{
			X = other.X;
			Y = other.Y;
		}

		Position &operator=(const Position &rhs)
		{
			X = rhs.X;
			Y = rhs.Y;
			return *this;
		}

		bool operator==(const Position &rhs) const
		{
			return X == rhs.X && Y == rhs.Y;
		}

		Position operator+(const Position &rhs) const
		{
			return Position(X + rhs.X, Y + rhs.Y);
		}

		Position &operator+=(const Position &rhs)
		{
			return (*this = *this + rhs);
		}

		Position operator*(int factor) const
		{
			return Position(X * factor, Y * factor);
		}

		static char ByteEncode(const Position &p)
		{
			return (char)(((p.X + 1 & 0xF) << 4) + (p.Y + 1 & 0xF));
		}

		static Position ByteDecode(char c)
		{
			return Position((int)(c >> 4) - 1, (int)(c & 0x0F) - 1);
		}
};

const Position Right = Position(1, 0);
const Position Up = Position(0, -1);
const Position Left = Position(-1, 0);
const Position Down = Position(0, 1);
