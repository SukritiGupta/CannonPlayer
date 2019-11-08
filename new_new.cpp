#include <iostream>
#include <vector>
#include <map>
#include<algorithm>

int ply_MAX;
using namespace std;

//Struct for cannons
struct can {
    //centre piece
    pair<int,int> centre;
    int dir; //0,1,2,3 //direction

    bool operator () (const can & m) const {
        return (m.dir==dir && m.centre.first==centre.first && m.centre.second==centre.second);
    }
};

struct tup {
    int a[4]; 
    bool operator () (const tup & m) const
    {
        return (m.a[0] == a[0] && m.a[1] == a[1] && m.a[2] == a[2] && m.a[3] == a[3]);
    }
    tup(int a0, int a1, int a2, int a3)
    {
        a[0]=a0;
        a[1]=a1;
        a[2]=a2;
        a[3]=a3;
    }
    tup(const tup &t)
    {
        a[0]=t.a[0];
        a[1]=t.a[1];
        a[2]=t.a[2];
        a[3]=t.a[3];
    }
};

int n; // y dimension
int m; // x dimension 

//defining class board which would consisit of all essential functions and atributes
class board {

public:
    //vector of vector of int to store the board grid
    vector< vector <int> > grid; //1 mysol, -1 other sol, 2 my townhall, -2 other townhall, 0 empty
    // vector of positions of my soldiers - can be easily inferenced from the board
    vector<pair<int,int>> soldier[2];
    vector<can> allcan[2];
    
    int numsol[2];
    int numth[2];

    //nXm == 8X8, 10X8, 10X10     

    //initialiser
    board()  {

        if ((n == 8) && (m == 8)){
            numsol[0]=12;
            numsol[1]=12;
            vector<vector<int>> temp1{{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2}};
            grid=temp1;

            vector<pair<int, int>> temp2 = { {0,5},{0,6},{0,7}, {2,5},{2,6},{2,7}, {4,5},{4,6},{4,7}, {6,5},{6,6},{6,7} };
            soldier[1] = temp2;

            vector<pair<int, int>> temp3 = { {1,0},{1,1},{1,2}, {3,0},{3,1},{3,2}, {5,0},{5,1},{5,2}, {7,0},{7,1},{7,2} };
            soldier[0] = temp3;

            can c{{0,6},0};
            allcan[1].push_back(c);
            c.centre.first=2;
            allcan[1].push_back(c);
            c.centre.first=4;
            allcan[1].push_back(c);
            c.centre.first=6;
            allcan[1].push_back(c);

            can c1{{1,1},0};
            allcan[0].push_back(c1);
            c1.centre.first=3;
            allcan[0].push_back(c1);
            c1.centre.first=5;
            allcan[0].push_back(c1);
            c1.centre.first=7;
            allcan[0].push_back(c1);

            numth[0]=4;
            numth[1]=4;
        }
        else if ((n == 10) && (m == 8)) {
            numsol[0]=12;
            numsol[1]=12;
            vector<vector<int>> temp1{{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2}};
            grid=temp1;

            vector<pair<int, int>> temp2 = { {0,7},{0,8},{0,9}, {2,7},{2,8},{2,9}, {4,7},{4,8},{4,9}, {6,7},{6,8},{6,9} };
            soldier[1] = temp2;

            vector<pair<int, int>> temp3 = { {1,0},{1,1},{1,2}, {3,0},{3,1},{3,2}, {5,0},{5,1},{5,2}, {7,0},{7,1},{7,2} };
            soldier[0] = temp3;

            can c{{0,8},0};
            allcan[1].push_back(c);
            c.centre.first=2;
            allcan[1].push_back(c);
            c.centre.first=4;
            allcan[1].push_back(c);
            c.centre.first=6;
            allcan[1].push_back(c);

            can c1{{1,1},0};
            allcan[0].push_back(c1);
            c1.centre.first=3;
            allcan[0].push_back(c1);
            c1.centre.first=5;
            allcan[0].push_back(c1);
            c1.centre.first=7;
            allcan[0].push_back(c1);

            numth[0]=4;
            numth[1]=4;
        }
        else if ((n == 10) && (m == 10)) {
            numsol[0]=15;
            numsol[1]=15;
            vector<vector<int>> temp1{{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2},{-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2}, 
                                    {-2,0,0,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,0,0,2}};
            grid=temp1;

            vector<pair<int, int>> temp2 = { {0,7},{0,8},{0,9}, {2,7},{2,8},{2,9}, {4,7},{4,8},{4,9}, {6,7},{6,8},{6,9}, {8,7},{8,8},{8,9} };
            soldier[1] = temp2;

            vector<pair<int, int>> temp3 = { {1,0},{1,1},{1,2}, {3,0},{3,1},{3,2}, {5,0},{5,1},{5,2}, {7,0},{7,1},{7,2}, {9,0},{9,1},{9,2} };
            soldier[0] = temp3;

            can c{{0,8},0};
            allcan[1].push_back(c);
            c.centre.first=2;
            allcan[1].push_back(c);
            c.centre.first=4;
            allcan[1].push_back(c);
            c.centre.first=6;
            allcan[1].push_back(c);
            c.centre.first=8;
            allcan[1].push_back(c);

            can c1{{1,1},0};
            allcan[0].push_back(c1);
            c1.centre.first=3;
            allcan[0].push_back(c1);
            c1.centre.first=5;
            allcan[0].push_back(c1);
            c1.centre.first=7;
            allcan[0].push_back(c1);
            c1.centre.first=9;
            allcan[0].push_back(c1);

            numth[0]=5;
            numth[1]=5;
        }

        // numsol[0]=6;
        // numsol[1]=1;
        // vector<vector<int>> temp1{{-2,0,0,0,0,0,0,0},{0,0,-1,0,1,0,0,2},{-2,0,-1,0,0,0,0,0},{0,0,0,0,0,0,0,2},{-2,0,0,-1,0,0,0,0},{0,-1,0,0,0,0,0,2},{-2,0,0,-1,0,-1,0,0},{0,0,0,0,0,0,0,0}};
        // grid=temp1;

        // vector<pair<int, int>> temp2 = { {1,4} };
        // soldier[1] = temp2;

        // vector<pair<int, int>> temp3 = { {5,1},{1,2},{6,3}, {6,5},{4,3},{2,2} };
        // soldier[0] = temp3;

        // numth[0]=4;
        // numth[1]=3;
    }

    double eval(int pno, int isthisme) {
        pno*=isthisme;
        int index=(1+pno)/2;
        int index2=(1-pno)/2;
        int ans = numsol[index] - numsol[index2];
        //numsol[index] - numsol[index2] + numth[index] - numth[index2];
        // cerr<<"eval called "<<nummysol<<" "<<numothsol<<"  "<<nummyth<<"  "<<numothth<<endl;
        return ans;
    }
    vector<vector<int> > find_soldier_moves(int pno) 
    {
        vector<vector<int> > ans;
        vector<int> temp;
        int index=(1+pno)/2;

        //iterator for traversing the soldier vector
        vector<pair<int, int> >::iterator ptr;
        int x, y;

        for (int i = 0; i < numsol[index]; ++i)
        {
            x=soldier[index][i].first;
            y=soldier[index][i].second;

            //forward move: <=0 because empty or other's soldier
            // cerr<<"1" << endl;
            if ((y-pno >= 0) && (y-pno <= n) && (grid[x][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x, y-pno};
                ans.push_back(temp);
            }

            //right diagonal move
            if ((x+pno <= m) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= n) && (grid[x+pno][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x+pno, y-pno};
                ans.push_back(temp);
            } 

            //left diagonal move
            if ((x-pno >= 0) && (x-pno <= m) && (y-pno >= 0) && (y-pno <= n) && (grid[x-pno][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x-pno, y-pno};
                ans.push_back(temp);
            }

            //right adjacent move
            if ((x+pno <= m) && (x+pno >= 0) && (grid[x+pno][y]*pno < 0)) {
                temp = vector<int> {x, y, x+pno, y};
                ans.push_back(temp);
            }

            //left adjacent move
            if ((x-pno >= 0) && (x-pno <= m) && (grid[x-pno][y]*pno < 0)) {
                temp = vector<int> {x, y, x-pno, y};
                ans.push_back(temp);
            }

            //retreat moves, considering that it can kill soldiers while retreating
            // ??????????????????? can a soldier kill other soldiers while retreating
            if (((y-pno >= 0) && (y-pno <= n) && (grid[x][y-pno]*pno == -1)) || ((x+pno <= m) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= n) && (grid[x+pno][y-pno]*pno == -1)) || 
                ((x-pno >= 0) && (x-pno <= m) && (y-pno >= 0) && (y-pno <= n) && (grid[x-pno][y-pno]*pno == -1)) || 
                ((x+pno <= m) && (x+pno >= 0) && (grid[x+pno][y]*pno == -1)) || ((x-pno >= 0) && (x-pno <= m) && (grid[x-pno][y]*pno == -1))) {

                if ((y+2*pno <= n) && (y+2*pno >= 0) && (grid[x][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y+2*pno};
                    ans.push_back(temp);
                }

                if ((y+2*pno <= n) && (y+2*pno >= 0) && (x+2*pno <= m) && (x+2*pno >= 0) && (grid[x+2*pno][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x+2*pno, y+2*pno};
                    ans.push_back(temp);
                }

                if ((y+2*pno <= n) && (y+2*pno >= 0) && (x-2*pno >= 0) && (x-2*pno <= m) && (grid[x-2*pno][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x-2*pno, y+2*pno};
                    ans.push_back(temp);
                }
            }
        }

        vector<can>::iterator ptr2;
        int dir;
        for (ptr2 = allcan[index].begin(); ptr2 != allcan[index].end(); ptr2++) {
            x = (*ptr2).centre.first;
            y = (*ptr2).centre.second;
            dir = (*ptr2).dir;

            if (dir == 0) {
                if ((y-2 >= 0) && (grid[x][y-2] == 0)) {
                    temp = vector<int> {x, y+1, x, y-2};
                    ans.push_back(temp);
                }
                if ((y+2 <= n) && (grid[x][y+2] == 0)) {
                    temp = vector<int> {x, y-1, x, y+2};
                    ans.push_back(temp);
                }
            }
            else if (dir == 1) {
                if ((y-2 >= 0) && (x+2 <= m) && (grid[x+2][y-2] == 0)) {
                    temp = vector<int> {x-1, y+1, x+2, y-2};
                    ans.push_back(temp);
                }
                if ((y+2 <= n) && (x-2 >= 0) && (grid[x-2][y+2] == 0)) {
                    temp = vector<int> {x+1, y-1, x-2, y+2};
                    ans.push_back(temp);
                }
            }
            else if (dir == 2) {
                if ((x-2 >= 0) && (grid[x-2][y] == 0)) {
                    temp = vector<int> {x+1, y, x-2, y};
                    ans.push_back(temp);
                }
                if ((x+2 <= m) && (grid[x+2][y] == 0)) {
                    temp = vector<int> {x-1, y, x+2, y};
                    ans.push_back(temp);
                }
            }
            else { //if dir == 3
                if ((y+2 <= n) && (x+2 <= m) && (grid[x+2][y+2] == 0)) {
                    temp = vector<int> {x-1, y-1, x+2, y+2};
                    ans.push_back(temp);
                }
                if ((y-2 >= 0) && (x-2 >= 0) && (grid[x-2][y-2] == 0)) {
                    temp = vector<int> {x+1, y+1, x-2, y-2};
                    ans.push_back(temp);
                }
            }
        }
        // cerr<<"find_soldier_moves ended"<<endl;

        return ans;
    }

    vector<vector<int> > find_cannon_moves(int pno, int stagnant) { //stagnant = 0 if empty blast is allowed, 1 if empty blast is not allowed
        //ans vector to be returned
        vector<vector<int> > ans;
        vector<int> temp;
        int index=(1+pno)/2;

        //????????????????????????????can empty blast
        vector<can>::iterator ptr;
        int x, y, dir;
        for (ptr = allcan[index].begin(); ptr != allcan[index].end(); ptr++) {
            x = (*ptr).centre.first;
            y = (*ptr).centre.second;
            dir = (*ptr).dir;

            if (dir == 0) {
                if ((y-2 >= 0) && (grid[x][y-2] == 0))
                {
                    if ((y-3 >= 0) && (grid[x][y-3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (grid[x][y-4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x, y-4};
                        ans.push_back(temp);
                    }
                }
                if ((y+2 <= n) && (grid[x][y+2] == 0))
                {
                    if ((y+3 <= n) && (grid[x][y+3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= n) && (grid[x][y+4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x, y+4};
                        ans.push_back(temp);
                    }
                }
            }
            else if (dir == 1) {
                if ((y-2 >= 0) && (x+2 <= m) && (grid[x+2][y-2] == 0)) {
                    if ((y-3 >= 0) && (x+3 <= m) && (grid[x+3][y-3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+3, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (x+4 <= m) && (grid[x+4][y-4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+4, y-4};
                        ans.push_back(temp);
                    }
                }
                if ((y+2 <= n) && (x-2 >= 0) && (grid[x-2][y+2] == 0)) {
                    if ((y+3 <= n) && (x-3 >= 0) && (grid[x-3][y+3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-3, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= n) && (x-4 >= 0) && (grid[x-4][y+4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-4, y+4};
                        ans.push_back(temp);
                    }
                }
            }
            else if (dir == 2) {
                if ((x-2 >= 0) && (grid[x-2][y] == 0)) {
                    if ((x-3 >= 0) && (grid[x-3][y]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-3, y};
                        ans.push_back(temp);
                    }
                    if ((x-4 >= 0) && (grid[x-4][y]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-4, y};
                        ans.push_back(temp);
                    }
                }
                if ((x+3 <= m) && (grid[x+2][y] == 0)) {
                    if ((x+3 <= m) && (grid[x+3][y]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+3, y};
                        ans.push_back(temp);
                    }
                    if ((x+4 <= m) && (grid[x+4][y]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+4, y};
                        ans.push_back(temp);
                    }
                }
            }
            else { //if dir == 3
                if ((y+2 <= n) && (x+2 <= m) && (grid[x+2][y+2] == 0)) {
                    if ((y+3 <= n) && (x+3 <= m) && (grid[x+3][y+3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+3, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= n) && (x+4 <= m) && (grid[x+4][y+4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x+4, y+4};
                        ans.push_back(temp);
                    }
                }
                if ((y-2 >= 0) && (x-2 >= 0) && (grid[x-2][y-2] == 0)) {
                    if ((y-3 >= 0) && (x-3 >= 0) && (grid[x-3][y-3]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-3, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (x-4 >= 0) && (grid[x-4][y-4]*pno + stagnant <= 0)) {
                        temp = vector<int> {x, y, x-4, y-4};
                        ans.push_back(temp);
                    }
                }
            }
        }

        return ans;
    }


    double eval_score(int pno, int isthisme) {
        pno*=isthisme;
        int index=(1+pno)/2;
        int index2=(1-pno)/2;
        double ans;

        // 4 conditions - 2 townhalls killed, all soldiers killed, stagnant game, no possible move

        if (numth[index2] == 2) {//win by killing townhall
            ans = 2 + 2*numth[index];
        }
        else if (numsol[index2] == 0) {//win by all soldiers killed
            ans = 8 + 2*(numth[index] - numth[index2]);
        }
        // else if (find_soldier_moves(pno*(-1)).size() + find_cannon_moves(pno*(-1)).size() == 0)
        //     ans = 6 + 2*(numth[index] - numth[index2]);
        // }
        else {//I don't know how to write a stagnant game condition
            ans = 5 + 2*(numth[index] - numth[index2]);
        }

        ans += numsol[index]/100.000;
        return ans;

    }

    void add_soldier(int b1, int b2, int pno)
    {
        int index=(1+pno)/2;
        grid[b1][b2]=pno;
        soldier[index].push_back(pair<int,int>(b1,b2));
        numsol[index]+=1;

        if (b2+2<= n && grid[b1][b2+1]==pno && grid[b1][b2+2]==pno)
        {
            can a{pair<int,int>(b1,b2+1),0};
            allcan[index].push_back(a);
        }

        if (b2+1<= n && b2-1>=0 && grid[b1][b2+1]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2),0};
            allcan[index].push_back(a);
        }

        if (b2-2>=0 && grid[b1][b2-2]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2-1),0};
            allcan[index].push_back(a);
        }

        if (b1+2<= m && grid[b1+2][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1+1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1+1<= m && b1-1>=0 && grid[b1-1][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1-2>=0 && grid[b1-1][b2]==pno && grid[b1-2][b2]==pno)
        {
            can a{pair<int,int>(b1-1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1+2<= m && b2+2<= n && grid[b1+2][b2+2]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1+1,b2+1),3};
            allcan[index].push_back(a);
        }

        if (b1+1<= m && b2+1<= n && b1-1>=0 && b2-1>=0 && grid[b1-1][b2-1]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),3};
            allcan[index].push_back(a);
        }

        if ( b1-2>=0 && b2-2>=0 && grid[b1-1][b2-1]==pno && grid[b1-2][b2-2]==pno)
        {
            can a{pair<int,int>(b1-1,b2-1),3};
            allcan[index].push_back(a);
        }

        if (b1+2<= m && b2-2>=0 && grid[b1+2][b2-2]==pno && grid[b1+1][b2-1]==pno)
        {
            can a{pair<int,int>(b1+1,b2-1),1};
            allcan[index].push_back(a);
        }

        if (b1+1<= m && b2-1>=0 && b1-1>=0 && b2+1<= n && grid[b1+1][b2-1]==pno && grid[b1-1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),1};
            allcan[index].push_back(a);
        }

        if ( b1-2>=0 && b2+2<= n && grid[b1-1][b2+1]==pno && grid[b1-2][b2+2]==pno)
        {
            can a{pair<int,int>(b1-1,b2+1),1};
            allcan[index].push_back(a);
        }
    }


    void delete_soldier(int b1, int b2, int pno) {
        int index=(1+pno)/2;
        soldier[index].erase(find(soldier[index].begin(), soldier[index].end(), pair<int,int>(b1,b2)));
        numsol[index]-=1;

        if (b2+2<= n && grid[b1][b2+1]==pno && grid[b1][b2+2]==pno)
        {
            can a{pair<int,int>(b1,b2+1),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b2+1<= n && b2-1>=0 && grid[b1][b2+1]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b2-2>=0 && grid[b1][b2-2]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2-1),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<= m && grid[b1+2][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1+1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<= m && b1-1>=0 && grid[b1-1][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1-2>=0 && grid[b1-1][b2]==pno && grid[b1-2][b2]==pno)
        {
            can a{pair<int,int>(b1-1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<= m && b2+2<= n && grid[b1+2][b2+2]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1+1,b2+1),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<= m && b2+1<= n && b1-1>=0 && b2-1>=0 && grid[b1-1][b2-1]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if ( b1-2>=0 && b2-2>=0 && grid[b1-1][b2-1]==pno && grid[b1-2][b2-2]==pno)
        {
            can a{pair<int,int>(b1-1,b2-1),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<= m && b2-2>=0 && grid[b1+2][b2-2]==pno && grid[b1+1][b2-1]==pno)
        {
            can a{pair<int,int>(b1+1,b2-1),1};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<= m && b2-1>=0 && b1-1>=0 && b2+1<= n && grid[b1+1][b2-1]==pno && grid[b1-1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),1};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if ( b1-2>=0 && b2+2<= n && grid[b1-1][b2+1]==pno && grid[b1-2][b2+2]==pno)
        {
            can a{pair<int,int>(b1-1,b2+1),1};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        grid[b1][b2] = 0;

    }

};

//written assuming that we will be having the above attributes to a board
//returns a vector of int where 1 - original x, 2 - original y, 3 - new x, 4 - new y 

board apply_moves(board current, bool solmove, int a1, int a2, int a3, int a4, int pno)
{
    // cerr<<"Apply move called"<<endl;
    //delete my soldier as it has moved
    if (solmove) {
        current.delete_soldier(a1, a2, pno);
    }

    //delete other soldier
    if (current.grid[a3][a4] == (-1)*pno) { //????????pno
        current.delete_soldier(a3, a4, (-1)*pno);
    }
    else if (current.grid[a3][a4] == (-2)*pno) {
        current.numth[(1-pno)/2] -=1;
        current.grid[a3][a4]=0;
    }

    //add my soldier
    if (solmove) {
        current.add_soldier(a3, a4, pno); //??????????????????Considering 1 is for myself
    }

    return current;

}

void execute_move(board* currboard,string move, int pno)
{
    // cerr<<"move executing *********************************************************************************************************************************************** "<<move<<endl;
    if (move[6]=='M')
    {
        *currboard=apply_moves(*currboard,true,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'),pno);
    }
    else //cannon blast
    {
        *currboard=apply_moves(*currboard,false,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'),pno);
    }
}

double minimax(board b, int pno, int isthisme, int ply, string *movefinal, double alpha, double beta)
{
    vector<vector<int>> posmove;
    posmove=b.find_soldier_moves(pno);

    int temp=posmove.size();
    if (temp==0)
    {
        return b.eval(pno,isthisme);
    }

    double bestchild=isthisme*10000.0*(-1);
    board tcmove;
    double val;
    string s;
    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,true,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            val=tcmove.eval(pno,isthisme);
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s,alpha,beta);
        } 

        if (isthisme == 1) {
            alpha=alpha<val?val:alpha;
            if (val>bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            } 
        }
        else {
            beta=beta>val?val:beta;
            if (val < bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }  
        }
        if (alpha>=beta)
        {
            // cerr<<alpha<<"pruned"<<beta<<"  "<<bestchild<<" "<<isthisme<<endl;
            return val;
        } 
               
    }

    // stagnant=(param==2 && )?1:0;
    // {
    //     /* code */
    // }
    posmove=b.find_cannon_moves(pno,0);
    temp=posmove.size();

    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,false,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            val=tcmove.eval(pno,isthisme);
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s,alpha,beta);
        }
        
        if (isthisme == 1) {
            alpha=alpha<val?val:alpha;
            if (val>bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }
        }
        else {
            beta=beta>val?val:beta;
            if (val < bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }  
        }
        if (alpha>=beta)
        {
            return val;
        } 
    }

    return bestchild;
    //???? no valid moves at depth handle
}
//????super table try
void print(board currboard)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            cerr<<currboard.grid[j][i]<<" ";
        }
        cerr<<endl;
    }

    cerr<<currboard.numsol[0]<<endl;
    cerr<<currboard.numsol[1]<<endl;
    cerr<<currboard.numth[0]<<endl;
    cerr<<currboard.numth[1]<<endl;

    vector<pair<int, int>>::iterator xx;
    cerr<<"Soldiers"<<endl;
    for (int i = 0; i < 2; i++) {
        for (xx=currboard.soldier[i].begin(); xx!=currboard.soldier[i].end(); ++xx)
        {
            cerr<<(*xx).first<<"  "<<(*xx).second<<endl;
        }
    }
    
    cerr<<"My can"<<endl;
    vector<can>::iterator poorva;
    for (int i = 0; i < 2; i++) {
        for (poorva = currboard.allcan[i].begin(); poorva != currboard.allcan[i].end(); ++poorva)
        {
            cerr<<(*poorva).centre.first<<" "<<(*poorva).centre.second<<"    "<<(*poorva).dir<<endl;
        }
    }

}

//     vector<vector<int>> posmove;
//     posmove=find_soldier_moves(newboard, 1);

//     cerr << "Possible moves" << endl;
//     for (int i = 0; i < posmove.size(); i++) {
//         for (int j = 0; j < 4; j++) {
//             cerr << posmove[i][j] << " ";
//         }
//         cerr << endl;
//     }

//     posmove=find_cannon_moves(newboard, 1);

//     cerr << "Possible moves" << endl;
//     for (int i = 0; i < posmove.size(); i++) {
//         for (int j = 0; j < 4; j++) {
//             cerr << posmove[i][j] << " ";
//         }
//         cerr << endl;
//     }
//     print(newboard);

int main() 
{
    string line;
    int pno, timeq;
    cin>>pno>>n>>m>>timeq;
    getline(cin,line);

    ply_MAX=5;

    board curr;

    if (pno==2)
    {
        pno=-1;
        getline(cin,line);
        while(line=="")
            getline(cin,line);
        execute_move(&curr,line,1);
    }
    string move;
    double ttt;

    double maxval=(double)-10000 ,minval=(double)10000;
//eval exactly scoring????
    while((true))
    {
        print(curr);
        ttt=minimax(curr,pno, 1,ply_MAX,&move,maxval,minval);
        cout<<move<<endl;
        execute_move(&curr,move,pno);

        getline(cin,move);
        while(move=="")
            getline(cin,move);

        execute_move(&curr,move,(-1)*pno);

        // print(curr);
        // exit(0);
    }

}



//AlphaBetaPruning *******************************
//transpose output!!??, @execute move???