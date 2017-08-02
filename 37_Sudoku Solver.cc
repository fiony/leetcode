class Solution {
private:
    const int NUM = 9;
    const int SUB_NUM = 3;
    const int ALL_CANDIDATE = 0x1FF;
    const int digit2flag [9] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100};
public:
    typedef struct{
        int i;
        int j;
        int candidates; // avaialbe candidates to fill the cell
    }cell;
    
    void solveSudoku(vector<vector<char>>& board) {
        //vector<vector<int>> candidates(NUM,vector<int>(NUM, 0));
        vector<cell> solved_cells;
        for(int i = 0; i < NUM; i++)
        {
            for(int j = 0; j < NUM; j++)
            {
                if(board[i][j] != '.') continue;
                auto new_cell = cell{ i, j, ALL_CANDIDATE};
                if(GetNextCandidate(new_cell, board))
                {
                    // push to solved cell list
                    solved_cells.push_back(new_cell);
                }else{
                    // cannot fill current cell, back track
                    while(true)
                    {
                        auto& prev_solved_cell = solved_cells.back();
                        if(GetNextCandidate(prev_solved_cell, board))
                        {
                            // set prev cell with next candidate
                            i = prev_solved_cell.i;
                            j = prev_solved_cell.j;
                            break;
                        }else
                        {
                            solved_cells.pop_back();    // back track
                            if(solved_cells.empty())
                                return;     // failed to solve
                        }
                    }
                }
            }
        }
        return;  //solved all cells
    }
    
    string ParseCandidates(int x)
    {
        string result;
        for(int i=0; i < NUM; i++)
        {
            if((x & digit2flag[i]))
                result.append(1, i +'1' );
        }
        return result;
    }
    
    // 1. initialize empty cell with all possible filled values (ie. candidates)
    // 2. Fill the cell with next avaialbe candidate
    // 3. If failed find next candidate to fill, clear the cell to empty
    bool GetNextCandidate(cell& c, vector<vector<char>>& board)
    {        
        int i = c.i;
        int j = c.j;
        int box_i = i/SUB_NUM * SUB_NUM;
        int box_j = j/SUB_NUM * SUB_NUM;
        char x = board[i][j];
        if(x == '.')
        {
            //initialize alll possible candidates
            for(int k = 0; k < NUM; k++)
            {
                // exclude others on the same row i
                if(k != j && board[i][k] != '.')
                    c.candidates &= ~digit2flag[board[i][k]-'1'];   //clear:&=~, set:|=, toggle:^=
                // exclude others on the same col j
                if(k != i && board[k][j] != '.')
                    c.candidates &= ~digit2flag[board[k][j]-'1'];
                //exclude others in the same sub box
                int ii = box_i + k / SUB_NUM;
                int jj = box_j + k % SUB_NUM;
                if( ii != i && jj != j && board[ii][jj] != '.' )
                    c.candidates &= ~digit2flag[board[ii][jj]-'1'];                
            }
        }
        // get next candidate from start_pos
        int start_pos = (x=='.') ? 0 : (x-'1') + 1;
        for(int k=start_pos; k < NUM; k++)
        {
            if((c.candidates & digit2flag[k]))
            {
                board[i][j] = k + '1';  // fill cell with the next candidate
                //cout << "NEXT [" << i << ", " << j << "]=" << board[i][j] <<" candidates=" << ParseCandidates(c.candidates)<<endl;
                return true;
            }
        }
        board[i][j] = '.';  // clear cell if cannot find next candidate
        //cout << "CLEAR [" << i << ", " << j << "]=" << board[i][j] << " candidates=" <<ParseCandidates(c.candidates) << endl;
        return false;
    }
};
