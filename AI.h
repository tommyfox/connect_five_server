class AI {
public:
	AI(const FIAR::Board& b) { board = b; }

	virtual std::pair<int, int> getAIMove();

private:
	const FIAR::Board& board;

	Difficulty dif;
}
