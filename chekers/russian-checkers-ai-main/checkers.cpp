#include <iostream>
#include <vector>
#include <windows.h>

struct Move
{
	int x1, y1, x2, y2;
	bool jump = false;
	int x3 = 0, y3 = 0;

	Move(int _x1, int _y1, int _x2, int _y2, int _x3 = 0, int _y3 = 0, bool _jump = false)
		: x1(_x1), x2(_x2), y1(_y1), y2(_y2), x3(_x3), y3(_y3), jump(_jump)
	{}
};

class Board
{
public:

	char board[8][8] =
	{
		{ '#', 'b', '#', ' ', '#', 'w', '#', 'w' },
		{ 'b', '#', 'b', '#', ' ', '#', 'w', '#' },
		{ '#', 'b', '#', ' ', '#', 'w', '#', 'w' },
		{ 'b', '#', 'b', '#', ' ', '#', 'w', '#' },
		{ '#', 'b', '#', ' ', '#', 'w', '#', 'w' },
		{ 'b', '#', 'b', '#', ' ', '#', 'w', '#' },
		{ '#', 'b', '#', ' ', '#', 'w', '#', 'w' },
		{ 'b', '#', 'b', '#', ' ', '#', 'w', '#' }
	};

	std::vector<Move> getMoves(char color)
	{
		std::vector<Move> moves;
		bool jump = false;
		if (color == 'w')
		{
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
				{
					if (board[i][j] == 'w')
					{
						if (i > 1 && j > 1)
							if ((board[i - 1][j - 1] == 'b' || board[i - 1][j - 1] == 'B') && board[i - 2][j - 2] == ' ')
							{
								Move temp(i, j, i - 2, j - 2, i - 1, j - 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i > 1 && j < 6)
							if ((board[i - 1][j + 1] == 'b' || board[i - 1][j + 1] == 'B') && board[i - 2][j + 2] == ' ')
							{
								Move temp(i, j, i - 2, j + 2, i - 1, j + 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i < 6 && j > 1)
							if ((board[i + 1][j - 1] == 'b' || board[i + 1][j - 1] == 'B') && board[i + 2][j - 2] == ' ')
							{
								Move temp(i, j, i + 2, j - 2, i + 1, j - 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i > 6 && j > 6)
							if ((board[i + 1][j + 1] == 'b' || board[i + 1][j + 1] == 'B') && board[i + 2][j + 2] == ' ')
							{
								Move temp(i, j, i + 2, j + 2, i + 1, j + 1, true);
								moves.push_back(temp);
								jump = true;
							}
					}

					if (board[i][j] == 'W')
					{
						bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;
						bool jumpPP = false, jumpPM = false, jumpMP = false, jumpMM = false;
						int jumpPPx = 0, jumpPPy = 0;
						int jumpPMx = 0, jumpPMy = 0;
						int jumpMPx = 0, jumpMPy = 0;
						int jumpMMx = 0, jumpMMy = 0;
						for (int k = 1; k < 8; k++)
						{
							if (flagPP && i + k < 7 && j + k < 7)
							{
								if ((board[i + k][j + k] == 'b' || board[i + k][j + k] == 'B') && board[i + k + 1][j + k + 1] == ' ')
								{
									Move temp(i, j, i + k + 1, j + k + 1, i + k, j + k, true);
									moves.push_back(temp);
									jump = true;
									jumpPP = true;
									jumpPPx = i + k;
									jumpPPy = j + k;
								}
								if ((board[i + k][j + k] == 'w' || board[i + k][j + k] == 'W') || (board[i + k][j + k] != ' ' && board[i + k + 1][j + k + 1] != ' '))
								{
									jumpPP = false;
									flagPP = false;
								}
								if (board[i + k + 1][j + k + 1] == ' ' && jumpPP)
								{
									Move temp(i, j, i + k + 1, j + k + 1, jumpPPx, jumpPPy, true);
									moves.push_back(temp);
								}
							}

							if (flagPM && i + k < 7 && j - k > 0)
							{
								if ((board[i + k][j - k] == 'b' || board[i + k][j - k] == 'B') && board[i + k + 1][j - k - 1] == ' ')
								{
									Move temp(i, j, i + k + 1, j - k - 1, i + k, j - k, true);
									moves.push_back(temp);
									jump = true;
									jumpPM = true;
									jumpPMx = i + k;
									jumpPMy = j - k;
								}
								if ((board[i + k][j - k] == 'w' || board[i + k][j - k] == 'W') || (board[i + k][j - k] != ' ' && board[i + k + 1][j - k - 1] != ' '))
								{
									jumpPM = false;
									flagPM = false;
								}
								if (board[i + k + 1][j - k - 1] == ' ' && jumpPM)
								{
									Move temp(i, j, i + k + 1, j - k - 1, jumpPMx, jumpPMy, true);
									moves.push_back(temp);
								}
							}

							if (flagMP && i - k > 0 && j + k < 7)
							{
								if ((board[i - k][j + k] == 'b' || board[i - k][j + k] == 'B') && board[i - k - 1][j + k + 1] == ' ')
								{
									Move temp(i, j, i - k - 1, j + k + 1, i - k, j + k, true);
									moves.push_back(temp);
									jump = true;
									jumpMP = true;
									jumpMPx = i - k;
									jumpMPy = j + k;
								}
								if ((board[i - k][j + k] == 'w' || board[i - k][j + k] == 'W') || (board[i - k][j + k] != ' ' && board[i - k - 1][j + k + 1] != ' '))
								{
									jumpMP = false;
									flagMP = false;
								}
								if (board[i - k - 1][j + k + 1] == ' ' && jumpMP)
								{
									Move temp(i, j, i - k - 1, j + k + 1, jumpMPx, jumpMPy, true);
									moves.push_back(temp);
								}
							}

							if (flagMM && i - k > 0 && j - k > 0)
							{
								if ((board[i - k][j - k] == 'b' || board[i - k][j - k] == 'B') && board[i - k - 1][j - k - 1] == ' ')
								{
									Move temp(i, j, i - k - 1, j - k - 1, i - k, j - k, true);
									moves.push_back(temp);
									jump = true;
									jumpMM = true;
									jumpMMx = i - k;
									jumpMMy = j - k;
								}
								if ((board[i - k][j - k] == 'w' || board[i - k][j - k] == 'W') || (board[i - k][j - k] != ' ' && board[i - k - 1][j - k - 1] != ' '))
								{
									jumpMM = false;
									flagMM = false;
								}
								if (board[i - k - 1][j - k - 1] == ' ' && jumpMM)
								{
									Move temp(i, j, i - k - 1, j - k - 1, jumpMMx, jumpMMy, true);
									moves.push_back(temp);
								}
							}
						}
					}
				}

			if (!jump)
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
					{
						if (board[i][j] == 'w')
						{
							if (i > 0 && j > 0)
								if (board[i - 1][j - 1] == ' ')
								{
									Move temp(i, j, i - 1, j - 1);
									moves.push_back(temp);
								}
							if (i < 7 && j > 0)
								if (board[i + 1][j - 1] == ' ')
								{
									Move temp(i, j, i + 1, j - 1);
									moves.push_back(temp);
								}
						}

						if (board[i][j] == 'W')
						{
							bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;

							for (int k = 1; k < 8; k++)
							{
								if (flagPP && i + k < 8 && j + k < 8)
									if (board[i + k][j + k] == ' ')
									{
										Move temp(i, j, i + k, j + k);
										moves.push_back(temp);
									}
									else
										flagPP = false;

								if (flagPM && i + k < 8 && j - k > 0)
									if (board[i + k][j - k] == ' ')
									{
										Move temp(i, j, i + k, j - k);
										moves.push_back(temp);
									}
									else
										flagPM = false;

								if (flagMP && i - k > 0 && j + k < 8)
									if (board[i - k][j + k] == ' ')
									{
										Move temp(i, j, i - k, j + k);
										moves.push_back(temp);
									}
									else
										flagMP = false;

								if (flagMM && i - k > 0 && j - k > 0)
									if (board[i - k][j - k] == ' ')
									{
										Move temp(i, j, i - k, j - k);
										moves.push_back(temp);
									}
									else
										flagMM = false;
							}
						}
					}
		}

		if (color == 'b')
		{
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
				{
					if (board[i][j] == 'b')
					{
						if (i > 1 && j > 1)
							if ((board[i - 1][j - 1] == 'w' || board[i - 1][j - 1] == 'W') && board[i - 2][j - 2] == ' ')
							{
								Move temp(i, j, i - 2, j - 2, i - 1, j - 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i > 1 && j < 6)
							if ((board[i - 1][j + 1] == 'w' || board[i - 1][j + 1] == 'W') && board[i - 2][j + 2] == ' ')
							{
								Move temp(i, j, i - 2, j + 2, i - 1, j + 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i < 6 && j > 1)
							if ((board[i + 1][j - 1] == 'w' || board[i + 1][j - 1] == 'W') && board[i + 2][j - 2] == ' ')
							{
								Move temp(i, j, i + 2, j - 2, i + 1, j - 1, true);
								moves.push_back(temp);
								jump = true;
							}
						if (i < 6 && j < 6)
							if ((board[i + 1][j + 1] == 'w' || board[i + 1][j + 1] == 'W') && board[i + 2][j + 2] == ' ')
							{
								Move temp(i, j, i + 2, j + 2, i + 1, j + 1, true);
								moves.push_back(temp);
								jump = true;
							}
					}

					if (board[i][j] == 'B')
					{
						bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;
						bool jumpPP = false, jumpPM = false, jumpMP = false, jumpMM = false;
						int jumpPPx = 0, jumpPPy = 0;
						int jumpPMx = 0, jumpPMy = 0;
						int jumpMPx = 0, jumpMPy = 0;
						int jumpMMx = 0, jumpMMy = 0;
						for (int k = 1; k < 8; k++)
						{
							if (flagPP && i + k < 7 && j + k < 7)
							{
								if ((board[i + k][j + k] == 'w' || board[i + k][j + k] == 'W') && board[i + k + 1][j + k + 1] == ' ')
								{
									Move temp(i, j, i + k + 1, j + k + 1, i + k, j + k, true);
									moves.push_back(temp);
									jump = true;
									jumpPP = true;
									jumpPPx = i + k;
									jumpPPy = j + k;
								}
								if ((board[i + k][j + k] == 'b' || board[i + k][j + k] == 'B') || (board[i + k][j + k] != ' ' && board[i + k + 1][j + k + 1] != ' '))
								{
									jumpPP = false;
									flagPP = false;
								}
								if (board[i + k + 1][j + k + 1] == ' ' && jumpPP)
								{
									Move temp(i, j, i + k + 1, j + k + 1, jumpPPx, jumpPPy, true);
									moves.push_back(temp);
								}
							}

							if (flagPM && i + k < 7 && j - k > 0)
							{
								if ((board[i + k][j - k] == 'w' || board[i + k][j - k] == 'W') && board[i + k + 1][j - k - 1] == ' ')
								{
									Move temp(i, j, i + k + 1, j - k - 1, i + k, j - k, true);
									moves.push_back(temp);
									jump = true;
									jumpPM = true;
									jumpPMx = i + k;
									jumpPMy = j - k;
								}
								if ((board[i + k][j - k] == 'b' || board[i + k][j - k] == 'B') || (board[i + k][j - k] != ' ' && board[i + k + 1][j - k - 1] != ' '))
								{
									jumpPM = false;
									flagPM = false;
								}
								if (board[i + k + 1][j - k - 1] == ' ' && jumpPM)
								{
									Move temp(i, j, i + k + 1, j - k - 1, jumpPMx, jumpPMy, true);
									moves.push_back(temp);
								}
							}

							if (flagMP && i - k > 0 && j + k < 7)
							{
								if ((board[i - k][j + k] == 'w' || board[i - k][j + k] == 'W') && board[i - k - 1][j + k + 1] == ' ')
								{
									Move temp(i, j, i - k - 1, j + k + 1, i - k, j + k, true);
									moves.push_back(temp);
									jump = true;
									jumpMP = true;
									jumpMPx = i - k;
									jumpMPy = j + k;
								}
								if ((board[i - k][j + k] == 'b' || board[i - k][j + k] == 'B') || (board[i - k][j + k] != ' ' && board[i - k - 1][j + k + 1] != ' '))
								{
									jumpMP = false;
									flagMP = false;
								}
								if (board[i - k - 1][j + k + 1] == ' ' && jumpMP)
								{
									Move temp(i, j, i - k - 1, j + k + 1, jumpMPx, jumpMPy, true);
									moves.push_back(temp);
								}
							}

							if (flagMM && i - k > 0 && j - k > 0)
							{
								if ((board[i - k][j - k] == 'w' || board[i - k][j - k] == 'W') && board[i - k - 1][j - k - 1] == ' ')
								{
									Move temp(i, j, i - k - 1, j - k - 1, i - k, j - k, true);
									moves.push_back(temp);
									jump = true;
									jumpMM = true;
									jumpMMx = i - k;
									jumpMMy = j - k;
								}
								if ((board[i - k][j - k] == 'b' || board[i - k][j - k] == 'B') || (board[i - k][j - k] != ' ' && board[i - k - 1][j - k - 1] != ' '))
								{
									jumpMM = false;
									flagMM = false;
								}
								if (board[i - k - 1][j - k - 1] == ' ' && jumpMM)
								{
									Move temp(i, j, i - k - 1, j - k - 1, jumpMMx, jumpMMy, true);
									moves.push_back(temp);
								}
							}
						}
					}
				}

			if (!jump)
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
					{
						if (board[i][j] == 'b')
						{
							if (i < 7 && j < 7)
								if (board[i + 1][j + 1] == ' ')
								{
									Move temp(i, j, i + 1, j + 1);
									moves.push_back(temp);
								}
							if (i > 0 && j < 7)
								if (board[i - 1][j + 1] == ' ')
								{
									Move temp(i, j, i - 1, j + 1);
									moves.push_back(temp);
								}
						}

						if (board[i][j] == 'B')
						{
							bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;

							for (int k = 1; k < 8; k++)
							{
								if (flagPP && i + k < 8 && j + k < 8)
									if (board[i + k][j + k] == ' ')
									{
										Move temp(i, j, i + k, j + k);
										moves.push_back(temp);
									}
									else
										flagPP = false;

								if (flagPM && i + k < 8 && j - k > 0)
									if (board[i + k][j - k] == ' ')
									{
										Move temp(i, j, i + k, j - k);
										moves.push_back(temp);
									}
									else
										flagPM = false;

								if (flagMP && i - k > 0 && j + k < 8)
									if (board[i - k][j + k] == ' ')
									{
										Move temp(i, j, i - k, j + k);
										moves.push_back(temp);
									}
									else
										flagMP = false;

								if (flagMM && i - k > 0 && j - k > 0)
									if (board[i - k][j - k] == ' ')
									{
										Move temp(i, j, i - k, j - k);
										moves.push_back(temp);
									}
									else
										flagMM = false;
							}
						}
					}
		}

		if (moves.empty())
		{
			Move temp(-1, -1, -1, -1);
			moves.push_back(temp);
		}
		return moves;
	}

	std::vector<Move> getMovesJump(char color, int x, int y)
	{
		std::vector<Move> moves;

		if (color == 'w')
		{
			if (board[x][y] == 'w')
			{
				if (x > 1 && y > 1)
					if ((board[x - 1][y - 1] == 'b' || board[x - 1][y - 1] == 'B') && board[x - 2][y - 2] == ' ')
					{
						Move temp(x, y, x - 2, y - 2, x - 1, y - 1, true);
						moves.push_back(temp);
					}
				if (x > 1 && y < 6)
					if ((board[x - 1][y + 1] == 'b' || board[x - 1][y + 1] == 'B') && board[x - 2][y + 2] == ' ')
					{
						Move temp(x, y, x - 2, y + 2, x - 1, y + 1, true);
						moves.push_back(temp);
					}
				if (x < 6 && y > 1)
					if ((board[x + 1][y - 1] == 'b' || board[x + 1][y - 1] == 'B') && board[x + 2][y - 2] == ' ')
					{
						Move temp(x, y, x + 2, y - 2, x + 1, y - 1, true);
						moves.push_back(temp);
					}
				if (x < 6 && y < 6)
					if ((board[x + 1][y + 1] == 'b' || board[x + 1][y + 1] == 'B') && board[x + 2][y + 2] == ' ')
					{
						Move temp(x, y, x + 2, y + 2, x + 1, y + 1, true);
						moves.push_back(temp);
					}
			};

			if (board[x][y] == 'W')
			{
				bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;
				bool jumpPP = false, jumpPM = false, jumpMP = false, jumpMM = false;
				int jumpPPx = 0, jumpPPy = 0;
				int jumpPMx = 0, jumpPMy = 0;
				int jumpMPx = 0, jumpMPy = 0;
				int jumpMMx = 0, jumpMMy = 0;
				for (int k = 1; k < 8; k++)
				{
					if (flagPP && x + k < 7 && y + k < 7)
					{
						if ((board[x + k][y + k] == 'b' || board[x + k][y + k] == 'B') && board[x + k + 1][y + k + 1] == ' ')
						{
							Move temp(x, y, x + k + 1, y + k + 1, x + k, y + k, true);
							moves.push_back(temp);
							jumpPP = true;
							jumpPPx = x + k;
							jumpPPy = y + k;
						}
						if ((board[x + k][y + k] == 'w' || board[x + k][y + k] == 'W') || (board[x + k][y + k] != ' ' && board[x + k + 1][y + k + 1] != ' '))
						{
							jumpPP = false;
							flagPP = false;
						}
						if (board[x + k + 1][y + k + 1] == ' ' && jumpPP)
						{
							Move temp(x, y, x + k + 1, y + k + 1, jumpPPx, jumpPPy, true);
							moves.push_back(temp);
						}
					}

					if (flagPM && x + k < 7 && y - k > 0)
					{
						if ((board[x + k][y - k] == 'b' || board[x + k][y - k] == 'B') && board[x + k + 1][y - k - 1] == ' ')
						{
							Move temp(x, y, x + k + 1, y - k - 1, x + k, y - k, true);
							moves.push_back(temp);
							jumpPM = true;
							jumpPMx = x + k;
							jumpPMy = y - k;
						}
						if ((board[x + k][y - k] == 'w' || board[x + k][y - k] == 'W') || (board[x + k][y - k] != ' ' && board[x + k + 1][y - k - 1] != ' '))
						{
							jumpPM = false;
							flagPM = false;
						}
						if (board[x + k + 1][y - k - 1] == ' ' && jumpPM)
						{
							Move temp(x, y, x + k + 1, y - k - 1, jumpPMx, jumpPMy, true);
							moves.push_back(temp);
						}
					}

					if (flagMP && x - k > 0 && y + k < 7)
					{
						if ((board[x - k][y + k] == 'b' || board[x - k][y + k] == 'B') && board[x - k - 1][y + k + 1] == ' ')
						{
							Move temp(x, y, x - k - 1, y + k + 1, x - k, y + k, true);
							moves.push_back(temp);
							jumpMP = true;
							jumpMPx = x - k;
							jumpMPy = y + k;
						}
						if ((board[x - k][y + k] == 'w' || board[x - k][y + k] == 'W') || (board[x - k][y + k] != ' ' && board[x - k - 1][y + k + 1] != ' '))
						{
							jumpMP = false;
							flagMP = false;
						}
						if (board[x - k - 1][y + k + 1] == ' ' && jumpMP)
						{
							Move temp(x, y, x - k - 1, y + k + 1, jumpMPx, jumpMPy, true);
							moves.push_back(temp);
						}
					}

					if (flagMM && x - k > 0 && y - k > 0)
					{
						if ((board[x - k][y - k] == 'b' || board[x - k][y - k] == 'B') && board[x - k - 1][y - k - 1] == ' ')
						{
							Move temp(x, y, x - k - 1, y - k - 1, x - k, y - k, true);
							moves.push_back(temp);
							jumpMM = true;
							jumpMMx = x - k;
							jumpMMy = y - k;
						}
						if ((board[x - k][y - k] == 'w' || board[x - k][y - k] == 'W') || (board[x - k][y - k] != ' ' && board[x - k - 1][y - k - 1] != ' '))
						{
							jumpMM = false;
							flagMM = false;
						}
						if (board[x - k - 1][y - k - 1] == ' ' && jumpMM)
						{
							Move temp(x, y, x - k - 1, y - k - 1, jumpMMx, jumpMMy, true);
							moves.push_back(temp);
						}
					}
				}
			}
		}

		if (color == 'b')
		{
			if (board[x][y] == 'b')
			{
				if (x > 1 && y > 1)
					if ((board[x - 1][y - 1] == 'w' || board[x - 1][y - 1] == 'W') && board[x - 2][y - 2] == ' ')
					{
						Move temp(x, y, x - 2, y - 2, x - 1, y - 1, true);
						moves.push_back(temp);
					}
				if (x > 1 && y < 6)
					if ((board[x - 1][y + 1] == 'w' || board[x - 1][y + 1] == 'W') && board[x - 2][y + 2] == ' ')
					{
						Move temp(x, y, x - 2, y + 2, x - 1, y + 1, true);
						moves.push_back(temp);
					}
				if (x < 6 && y > 1)
					if ((board[x + 1][y - 1] == 'w' || board[x + 1][y - 1] == 'W') && board[x + 2][y - 2] == ' ')
					{
						Move temp(x, y, x + 2, y - 2, x + 1, y - 1, true);
						moves.push_back(temp);
					}
				if (x < 6 && y < 6)
					if ((board[x + 1][y + 1] == 'w' || board[x + 1][y + 1] == 'W') && board[x + 2][y + 2] == ' ')
					{
						Move temp(x, y, x + 2, y + 2, x + 1, y + 1, true);
						moves.push_back(temp);
					}
			};

			if (board[x][y] == 'B')
			{
				bool flagPP = true, flagPM = true, flagMP = true, flagMM = true;
				bool jumpPP = false, jumpPM = false, jumpMP = false, jumpMM = false;
				int jumpPPx = 0, jumpPPy = 0;
				int jumpPMx = 0, jumpPMy = 0;
				int jumpMPx = 0, jumpMPy = 0;
				int jumpMMx = 0, jumpMMy = 0;
				for (int k = 1; k < 8; k++)
				{
					if (flagPP && x + k < 7 && y + k < 7)
					{
						if ((board[x + k][y + k] == 'w' || board[x + k][y + k] == 'W') && board[x + k + 1][y + k + 1] == ' ')
						{
							Move temp(x, y, x + k + 1, y + k + 1, x + k, y + k, true);
							moves.push_back(temp);
							jumpPP = true;
							jumpPPx = x + k;
							jumpPPy = y + k;
						}
						if ((board[x + k][y + k] == 'b' || board[x + k][y + k] == 'B') || (board[x + k][y + k] != ' ' && board[x + k + 1][y + k + 1] != ' '))
						{
							jumpPP = false;
							flagPP = false;
						}
						if (board[x + k + 1][y + k + 1] == ' ' && jumpPP)
						{
							Move temp(x, y, x + k + 1, y + k + 1, jumpPPx, jumpPPy, true);
							moves.push_back(temp);
						}
					}

					if (flagPM && x + k < 7 && y - k > 0)
					{
						if ((board[x + k][y - k] == 'w' || board[x + k][y - k] == 'W') && board[x + k + 1][y - k - 1] == ' ')
						{
							Move temp(x, y, x + k + 1, y - k - 1, x + k, y - k, true);
							moves.push_back(temp);
							jumpPM = true;
							jumpPMx = x + k;
							jumpPMy = y - k;
						}
						if ((board[x + k][y - k] == 'b' || board[x + k][y - k] == 'B') || (board[x + k][y - k] != ' ' && board[x + k + 1][y - k - 1] != ' '))
						{
							jumpPM = false;
							flagPM = false;
						}
						if (board[x + k + 1][y - k - 1] == ' ' && jumpPM)
						{
							Move temp(x, y, x + k + 1, y - k - 1, jumpPMx, jumpPMy, true);
							moves.push_back(temp);
						}
					}

					if (flagMP && x - k > 0 && y + k < 7)
					{
						if ((board[x - k][y + k] == 'w' || board[x - k][y + k] == 'W') && board[x - k - 1][y + k + 1] == ' ')
						{
							Move temp(x, y, x - k - 1, y + k + 1, x - k, y + k, true);
							moves.push_back(temp);
							jumpMP = true;
							jumpMPx = x - k;
							jumpMPy = y + k;
						}
						if ((board[x - k][y + k] == 'b' || board[x - k][y + k] == 'B') || (board[x - k][y + k] != ' ' && board[x - k - 1][y + k + 1] != ' '))
						{
							jumpMP = false;
							flagMP = false;
						}
						if (board[x - k - 1][y + k + 1] == ' ' && jumpMP)
						{
							Move temp(x, y, x - k - 1, y + k + 1, jumpMPx, jumpMPy, true);
							moves.push_back(temp);
						}
					}

					if (flagMM && x - k > 0 && y - k > 0)
					{
						if ((board[x - k][y - k] == 'w' || board[x - k][y - k] == 'W') && board[x - k - 1][y - k - 1] == ' ')
						{
							Move temp(x, y, x - k - 1, y - k - 1, x - k, y - k, true);
							moves.push_back(temp);
							jumpMM = true;
							jumpMMx = x - k;
							jumpMMy = y - k;
						}
						if ((board[x - k][y - k] == 'b' || board[x - k][y - k] == 'B') || (board[x - k][y - k] != ' ' && board[x - k - 1][y - k - 1] != ' '))
						{
							jumpMM = false;
							flagMM = false;
						}
						if (board[x - k - 1][y - k - 1] == ' ' && jumpMM)
						{
							Move temp(x, y, x - k - 1, y - k - 1, jumpMMx, jumpMMy, true);
							moves.push_back(temp);
						}
					}
				}
			}
		}

		if (moves.empty())
		{
			Move temp(-1, -1, -1, -1);
			moves.push_back(temp);
		}
		return moves;

	};

	Board getNewBoard(Move move)
	{
		Board newBoard = *this;

		if (newBoard.board[move.x1][move.y1] == 'w' && move.y2 == 0)
			newBoard.board[move.x1][move.y1] = 'W';

		if (newBoard.board[move.x1][move.y1] == 'b' && move.y2 == 7)
			newBoard.board[move.x1][move.y1] = 'B';

		newBoard.board[move.x2][move.y2] = newBoard.board[move.x1][move.y1];
		newBoard.board[move.x1][move.y1] = ' ';

		if (move.jump)
			newBoard.board[move.x3][move.y3] = ' ';
		return newBoard;
	}

	void  printBoard()
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		system("cls");
		SetConsoleTextAttribute(handle, 7);

		std::cout << "\n   A B C D E F G H\n";

		for (int i = 0; i < 8; i++)
		{
			SetConsoleTextAttribute(handle, 15);
			std::cout << i << " ";
			for (int j = 0; j < 8; j++)
			{
				if (board[j][i] == 'b')
				{
					SetConsoleTextAttribute(handle, 3);
					std::cout << "()";
				}
				if (board[j][i] == 'W')
				{
					SetConsoleTextAttribute(handle, 12);
					std::cout << "@@";
				}
				if (board[j][i] == 'w')
				{
					SetConsoleTextAttribute(handle, 12);
					std::cout << "()";
				}
				if (board[j][i] == 'B')
				{
					SetConsoleTextAttribute(handle, 11);
					std::cout << "@@";
				}
				if (board[j][i] == ' ')
				{
					SetConsoleTextAttribute(handle, 0);
					std::cout << "  ";
				}
				if (board[j][i] == '#')
				{
					SetConsoleTextAttribute(handle, 7);
					std::cout << char(219) << char(219);
				}
			}
			std::cout << "\n";
		}
	};
};

class AI
{
private:
	char color;

	int evaluation(Board _board, char color)
	{
		int score = 0, k = -1;

		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				if (_board.board[i][j] == 'w')
					score += 1 * k;

				if (_board.board[i][j] == 'W')
					score += 7 * k;

				if (_board.board[i][j] == 'b')
					score -= 1 * k;

				if (_board.board[i][j] == 'B')
					score -= 3 * k;
			}
		return score;
	}

	int jumpRecursion(Board board, int depth, char _color, int x, int y, int score)
	{
		if (board.getMovesJump(_color, x, y)[0].x1 == -1)
			return (std::min)(score, miniMax(board, depth + 1, _color == 'w' ? 'b' : 'w'));

		for (Move move : board.getMovesJump(_color, x, y))
			jumpRecursion(board.getNewBoard(move), depth, _color, move.x2, move.y2, score);
	}



public:

	AI(char _color) : color(_color)
	{};

	int miniMax(Board board, int depth, char _color)
	{
		if (depth >= 6 || board.getMoves(_color)[0].x1 == -1)
		{
			return evaluation(board, _color);
		}

		int score;

		if (_color == color)
		{
			score = 10000;
			for (auto move : board.getMoves(_color))
			{
				if (move.jump)
					score = (std::min)(score, jumpRecursion(board.getNewBoard(move), depth + 1, _color == 'w' ? 'b' : 'w', move.x2, move.y2, score));
				else
					score = (std::min)(score, miniMax(board.getNewBoard(move), depth + 1, _color == 'w' ? 'b' : 'w'));
			}
		}
		else
		{
			score = -10000;
			for (auto move : board.getMoves(_color))
			{
				if (move.jump)
					score = (std::max)(score, jumpRecursion(board.getNewBoard(move), depth + 1, _color == 'w' ? 'b' : 'w', move.x2, move.y2, score));
				else
					score = (std::max)(score, miniMax(board.getNewBoard(move), depth + 1, _color == 'w' ? 'b' : 'w'));
			}
		}
		return score;
	}
};


int main()
{
	Board game;
	AI ai('b');

	game.printBoard();

	std::vector<Move> moves;
	while (game.getMoves('w')[0].x1 != -1 && game.getMoves('b')[0].x1 != -1)
	{
		moves = game.getMoves('w');

		int n = 1;
		std::cout << "\n";
		for (auto move : moves)
		{
			if (!move.jump)
				std::cout << n << ". " << char(move.x1 + 'a') << move.y1 << " -> " << char(move.x2 + 'a') << move.y2 << " \n";
			else
				std::cout << n << ". " << char(move.x1 + 'a') << move.y1 << " -> " << char(move.x3 + 'a') << move.y3 << " -> " << char(move.x2 + 'a') << move.y2 << " \n";
			n++;
		}
		std::cout << "Choose your move (1-" << n - 1 << "): ";
		int i;
		std::cin >> i;
		while (i < 0 || i > n - 1)
		{
			std::cout << "Imput Error! Try again: ";
			std::cin >> i;
		}

		game = game.getNewBoard(moves[i - 1]);

		game.printBoard();
		if (game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2)[0].x1 != -1 && moves[i - 1].jump)
		{
			i = 1;
			for (auto move : game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2))
			{
				std::cout << i << ". " << char(move.x1 + 'a') << move.y1 << " -> " << char(move.x3 + 'a') << move.y3 << " -> " << char(move.x2 + 'a') << move.y2 << " \n";
				i++;
			}
			std::cout << "Choose your move (1-" << i - 1 << "): ";
			std::cin >> i;
			game = game.getNewBoard(moves[i - 1]);
			game.printBoard();
			if (game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2)[0].x1 != -1)
			{
				i = 1;
				for (auto move : game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2))
				{
					std::cout << i << ". " << char(move.x1 + 'a') << move.y1 << " -> " << char(move.x3 + 'a') << move.y3 << " -> " << char(move.x2 + 'a') << move.y2 << " \n";
					i++;
				}
				std::cout << "Choose your move (1-" << i - 1 << "): ";
				std::cin >> i;
				game = game.getNewBoard(moves[i - 1]);
				game.printBoard();
				if (game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2)[0].x1 != -1)
				{
					i = 1;
					for (auto move : game.getMovesJump('w', moves[i - 1].x2, moves[i - 1].y2))
					{
						std::cout << i << ". " << char(move.x1 + 'a') << move.y1 << " -> " << char(move.x3 + 'a') << move.y3 << " -> " << char(move.x2 + 'a') << move.y2 << " \n";
						i++;
					}
					std::cout << "Choose your move (1-" << i - 1 << "): ";
					std::cin >> i;
					game = game.getNewBoard(moves[i - 1]);
					game.printBoard();
				}
			}
		}
		moves = game.getMoves('b');

		Move maxMove = moves[0];
		int max = ai.miniMax(game.getNewBoard(moves[0]), 0, 'b');
		for (auto move : moves)
		{
			if (ai.miniMax(game.getNewBoard(move), 0, 'b') > max)
			{
				max = ai.miniMax(game.getNewBoard(move), 0, 'b');
				maxMove = move;
				game = game.getNewBoard(maxMove);
				game.printBoard();
				if (game.getMovesJump('b', maxMove.x2, maxMove.y2)[0].x1 != -1 && maxMove.jump)
				{
					int max = ai.miniMax(game.getNewBoard(moves[0]), 0, 'b');
					for (auto move : game.getMovesJump('b', maxMove.x2, maxMove.y2))
					{
						if (ai.miniMax(game.getNewBoard(move), 0, 'b') > max)
						{
							max = ai.miniMax(game.getNewBoard(move), 0, 'b');
							maxMove = move;
							game = game.getNewBoard(maxMove);
							game.printBoard();
							int max = ai.miniMax(game.getNewBoard(moves[0]), 0, 'b');
							if (game.getMovesJump('b', maxMove.x2, maxMove.y2)[0].x1 != -1 && maxMove.jump)
							{
								if (ai.miniMax(game.getNewBoard(move), 0, 'b') > max)
								{
									max = ai.miniMax(game.getNewBoard(move), 0, 'b');
									maxMove = move;
									game = game.getNewBoard(maxMove);
									game.printBoard();

								}
							}
						}
					}
				}
			}
		}

		game = game.getNewBoard(maxMove);
		game.printBoard();
	}

	if (game.getMoves('b')[0].x1 != -1)
	{
		std::cout << "Computer win!";
	}
	else
		std::cout << "You win!";


}
