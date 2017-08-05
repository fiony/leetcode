class Solution {
	
public:
    void solveSudoku(vector<vector<char>>& board) {
		
		Initialize(board);		//initialize unsolved_cells, cell_constraint
		
		while(!unsolved_cells.empty())
		{
			auto& cur = unsolved_cells.top();	//TODO: select the cell with least candidates
			if(MoveOn(cur.i, cur.j, board))
			{
				// cur_cell solved, store for backtrack
				solved_cells.push(cur);
				unsolved_cells.pop();
			}else{
				//assert(_board[i][j]=='.')
				while(true)
				{
					auto& prev = solved_cells.top();	//TODO: select the cell with least candidate remained
					if(MoveOn(prev.i, prev.j, board))
					{
						break;
					}else{
						unsolved_cells.push(prev);
						solved_cells.pop();
						if(solved_cells.empty())
							return;	//FAILED to solve
					}
				}
			}
		}
	}
	
private:
	const char EMPTY_CELL = '.';
    const int NUM = 9;
    const int SUB_NUM = 3;
    const int ALL_CANDIDATE = 0x1FF;
    const int VAL2MASK [10] = {0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100};
	
	typedef struct { 
		int i;
		int j;
	}Cell;	
	
	struct OrderedCells
	{
		void push(const Cell& c){ cells.push_back(c); }
		void pop(){ cells.pop_back();}
		Cell& top(){ return cells.back(); }
		bool empty() { return cells.empty(); }
		void update(){}
		vector<Cell> cells;
	};
	// for each possible candidate(1 - 9), use 2 bits to represent the constraints:
	// because constraint can be applied by most 3 neighbors (same row, same col, same sub-box)
	// 0 : no neighbors in use (we can use it), 1~3 : 1~3 neighbor in use

	typedef struct{
		uint32_t _status;	// lower 18 bits represent the constraints for 1~9
		
		void add_constraint(int x)
		{
			//assert( x > 0 && x <= 9 && ((_status >> ((x-1)*2)) & (0x3)) < 3);
			_status += (1 << ((x-1)*2) );
		}		
		void remove_constrait(int x)
		{
			//assert( x > 0 && x <= 9 && ((_status >> ((x-1)*2)) & (0x3)) > 0);
			_status -= (1 << ((x-1)*2) );
		}
		bool is_avaiable(int x)
		{
			return ((_status >> ((x-1)*2)) & (0x3)) == 0;
		}
	}candidate;
	
	
	OrderedCells solved_cells;		// for backtrack
	OrderedCells unsolved_cells;	// finish if empty
	vector<vector<candidate>> cell_candidates;	// available number to fill
	
	inline uint8_t CHAR2INT(char x){ return x==EMPTY_CELL? 0 : x-'0';}
	inline char INT2CHAR(int x){ return x==0? EMPTY_CELL : x + '0';} 
	
	void Initialize(vector<vector<char>>& board)
	{		
		cell_candidates = vector<vector<candidate>>(NUM, vector<candidate>(NUM, {0}));
		for(int i = 0; i < NUM; i++)
		{
			for(int j = 0; j < NUM; j++)
			{
				if(board[i][j]==EMPTY_CELL)
				{
					unsolved_cells.push({i,j});
				}else{
					propogate(i, j, CHAR2INT(board[i][j]), 0); 
				}
			}
		}
	}
	
	std::ostream& _DEBUG_PRINT(int i, int j, char val)
	{
		return cout << "[" << i << ", " << j << "]=" << val <<" candidates=" << ParseCandidates(cell_candidates[i][j]);
	}
	string ParseCandidates(candidate& x)
    {
        string result;
        for(int i = 1; i <= NUM; i++)
            if(x.is_avaiable(i)) result.append(1, i +'0' );
        return result;
    } 
	
	// move cell(i,j) to next avaiable
	bool MoveOn(int i, int j, vector<vector<char>>& board)
	{
		// move to next candidates, step back if failed to move
		// propogate to clear the old constraint and set the new constraint
		uint8_t cur_val = CHAR2INT(board[i][j]);
		uint8_t next_val = 0;	//need to step back
		for(int val = cur_val + 1; val <= NUM; val++)
		{
			if(cell_candidates[i][j].is_avaiable(val))
			{
				next_val = val;
				break;
			}
		}	
		
		//TODO: if propogate got some cell has no candidates, then we can step back early
		propogate(i, j, next_val, cur_val);
		board[i][j] = INT2CHAR(next_val);
		
		//_DEBUG_PRINT(i,j,board[i][j]) << (next_val==0 ? "STEP BACK" : "MOVE ON") << endl;
		
		return next_val != 0;	// true: move on, false: step back
	}
	
	void _propogate(int i, int j, const std::function< void(candidate&) >& candidate_op)
	{
		int box_i = i/SUB_NUM * SUB_NUM;
        int box_j = j/SUB_NUM * SUB_NUM;
		
		for(int k = 0; k < NUM; k++)
		{
			// exclude val_set for others on the same row i
			if(k != j )	candidate_op(cell_candidates[i][k]);
			// exclude val_set for others on the same col j
			if(k != i)	candidate_op(cell_candidates[k][j]);
			//exclude others in the same sub box
			int ii = box_i + k / SUB_NUM;
			int jj = box_j + k % SUB_NUM;
			if( ii != i && jj != j)	candidate_op(cell_candidates[ii][jj]);
		}		
	}
	
	void propogate(int i, int j, int val_set, int val_clear)
	{
		//val_set: new value for i, j to add constraint (move on)
		//val_clear: old value for i, j to clear constraint (back track)
		if( val_set != 0)
		{
			auto apply_constraint = [&](candidate& c){ c.add_constraint(val_set); };
			_propogate(i, j, apply_constraint);
		}
		if( val_clear !=0 )
		{
			auto clear_constraint = [&](candidate& c){ c.remove_constrait(val_clear); };
			_propogate(i, j, clear_constraint);
		}
		// TODO: keep the two list sorted to make sure top has least candidates
		//solved_cells.update();
		//unsolved_cells.update();
	}
};
