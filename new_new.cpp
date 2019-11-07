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

    //initialiser
    board() 
    {
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

        // numth[0]=4;
        // numth[1]=4;

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

    float eval(int pno, int isthisme) {
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
            if ((y-pno >= 0) && (y-pno <= 7) && (grid[x][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x, y-pno};
                ans.push_back(temp);
            }

            //right diagonal move
            if ((x+pno <= 7) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= 7) && (grid[x+pno][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x+pno, y-pno};
                ans.push_back(temp);
            } 

            //left diagonal move
            if ((x-pno >= 0) && (x-pno <= 7) && (y-pno >= 0) && (y-pno <= 7) && (grid[x-pno][y-pno]*pno <= 0)) {
                temp = vector<int> {x, y, x-pno, y-pno};
                ans.push_back(temp);
            }

            //right adjacent move
            if ((x+pno <= 7) && (x+pno >= 0) && (grid[x+pno][y]*pno < 0)) {
                temp = vector<int> {x, y, x+pno, y};
                ans.push_back(temp);
            }

            //left adjacent move
            if ((x-pno >= 0) && (x-pno <= 7) && (grid[x-pno][y]*pno < 0)) {
                temp = vector<int> {x, y, x-pno, y};
                ans.push_back(temp);
            }

            //retreat moves, considering that it can kill soldiers while retreating
            // ??????????????????? can a soldier kill other soldiers while retreating
            if (((y-pno >= 0) && (y-pno <= 7) && (grid[x][y-pno]*pno == -1)) || ((x+pno <= 7) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= 7) && (grid[x+pno][y-pno]*pno == -1)) || 
                ((x-pno >= 0) && (x-pno <= 7) && (y-pno >= 0) && (y-pno <= 7) && (grid[x-pno][y-pno]*pno == -1)) || 
                ((x+pno <= 7) && (x+pno >= 0) && (grid[x+pno][y]*pno == -1)) || ((x-pno >= 0) && (x-pno <= 7) && (grid[x-pno][y]*pno == -1))) {

                if ((y+2*pno <= 7) && (y+2*pno >= 0) && (grid[x][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y+2*pno};
                    ans.push_back(temp);
                }

                if ((y+2*pno <=7) && (y+2*pno >= 0) && (x+2*pno <=7) && (x+2*pno >= 0) && (grid[x+2*pno][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x+2*pno, y+2*pno};
                    ans.push_back(temp);
                }

                if ((y+2*pno <=7) && (y+2*pno >= 0) && (x-2*pno >= 0) && (x-2*pno <= 7) && (grid[x-2*pno][y+2*pno]*pno <= 0)) {
                    temp = vector<int> {x, y, x-2*pno, y+2*pno};
                    ans.push_back(temp);
                }
            }
        }

        //??????????????????????? a cannon can't kill soldier while moving
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
                if ((y+2 <= 7) && (grid[x][y+2] == 0)) {
                    temp = vector<int> {x, y-1, x, y+2};
                    ans.push_back(temp);
                }
            }
            else if (dir == 1) {
                if ((y-2 >= 0) && (x+2 <= 7) && (grid[x+2][y-2] == 0)) {
                    temp = vector<int> {x-1, y+1, x+2, y-2};
                    ans.push_back(temp);
                }
                if ((y+2 <= 7) && (x-2 >= 0) && (grid[x-2][y+2] == 0)) {
                    temp = vector<int> {x+1, y-1, x-2, y+2};
                    ans.push_back(temp);
                }
            }
            else if (dir == 2) {
                if ((x-2 >= 0) && (grid[x-2][y] == 0)) {
                    temp = vector<int> {x+1, y, x-2, y};
                    ans.push_back(temp);
                }
                if ((x+2 <= 7) && (grid[x+2][y] == 0)) {
                    temp = vector<int> {x-1, y, x+2, y};
                    ans.push_back(temp);
                }
            }
            else { //if dir == 3
                if ((y+2 <= 7) && (x+2 <= 7) && (grid[x+2][y+2] == 0)) {
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

    //??????????????????khudko mat udana
    vector<vector<int> > find_cannon_moves(int pno) {
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
                    if ((y-3 >= 0) && (grid[x][y-3]*pno <= 0)) {
                        temp = vector<int> {x, y, x, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (grid[x][y-4]*pno <= 0)) {
                        temp = vector<int> {x, y, x, y-4};
                        ans.push_back(temp);
                    }
                }
                if ((y+2 <= 7) && (grid[x][y+2] == 0))
                {
                    if ((y+3 <= 7) && (grid[x][y+3]*pno <= 0)) {
                        temp = vector<int> {x, y, x, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= 7) && (grid[x][y+4]*pno <= 0)) {
                        temp = vector<int> {x, y, x, y+4};
                        ans.push_back(temp);
                    }
                }
            }
            else if (dir == 1) {
                if ((y-2 >= 0) && (x+2 <= 7) && (grid[x+2][y-2] == 0)) {
                    if ((y-3 >= 0) && (x+3 <= 7) && (grid[x+3][y-3]*pno <= 0)) {
                        temp = vector<int> {x, y, x+3, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (x+4 <= 7) && (grid[x+4][y-4]*pno <= 0)) {
                        temp = vector<int> {x, y, x+4, y-4};
                        ans.push_back(temp);
                    }
                }
                if ((y+2 <= 7) && (x-2 >= 0) && (grid[x-2][y+2] == 0)) {
                    if ((y+3 <= 7) && (x-3 >= 0) && (grid[x-3][y+3]*pno <= 0)) {
                        temp = vector<int> {x, y, x-3, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= 7) && (x-4 >= 0) && (grid[x-4][y+4]*pno <= 0)) {
                        temp = vector<int> {x, y, x-4, y+4};
                        ans.push_back(temp);
                    }
                }
            }
            else if (dir == 2) {
                if ((x-2 >= 0) && (grid[x-2][y] == 0)) {
                    if ((x-3 >= 0) && (grid[x-3][y]*pno <= 0)) {
                        temp = vector<int> {x, y, x-3, y};
                        ans.push_back(temp);
                    }
                    if ((x-4 >= 0) && (grid[x-4][y]*pno <= 0)) {
                        temp = vector<int> {x, y, x-4, y};
                        ans.push_back(temp);
                    }
                }
                if ((x+3 <= 7) && (grid[x+2][y] == 0)) {
                    if ((x+3 <= 7) && (grid[x+3][y]*pno <= 0)) {
                        temp = vector<int> {x, y, x+3, y};
                        ans.push_back(temp);
                    }
                    if ((x+4 <= 7) && (grid[x+4][y]*pno <= 0)) {
                        temp = vector<int> {x, y, x+4, y};
                        ans.push_back(temp);
                    }
                }
            }
            else { //if dir == 3
                if ((y+2 <=7) && (x+2 <= 7) && (grid[x+2][y+2] == 0)) {
                    if ((y+3 <= 7) && (x+3 <= 7) && (grid[x+3][y+3]*pno <= 0)) {
                        temp = vector<int> {x, y, x+3, y+3};
                        ans.push_back(temp);
                    }
                    if ((y+4 <= 7) && (x+4 <= 7) && (grid[x+4][y+4]*pno <= 0)) {
                        temp = vector<int> {x, y, x+4, y+4};
                        ans.push_back(temp);
                    }
                }
                if ((y-2 >= 0) && (x-2 >= 0) && (grid[x-2][y-2] == 0)) {
                    if ((y-3 >= 0) && (x-3 >= 0) && (grid[x-3][y-3]*pno <= 0)) {
                        temp = vector<int> {x, y, x-3, y-3};
                        ans.push_back(temp);
                    }
                    if ((y-4 >= 0) && (x-4 >= 0) && (grid[x-4][y-4]*pno <= 0)) {
                        temp = vector<int> {x, y, x-4, y-4};
                        ans.push_back(temp);
                    }
                }
            }
        }

        return ans;
    }

    void add_soldier(int b1, int b2, int pno)
    {
        int index=(1+pno)/2;
        grid[b1][b2]=pno;
        soldier[index].push_back(pair<int,int>(b1,b2));
        numsol[index]+=1;

        if (b2+2<=7 && grid[b1][b2+1]==pno && grid[b1][b2+2]==pno)
        {
            can a{pair<int,int>(b1,b2+1),0};
            allcan[index].push_back(a);
        }

        if (b2+1<=7 && b2-1>=0 && grid[b1][b2+1]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2),0};
            allcan[index].push_back(a);
        }

        if (b2-2>=0 && grid[b1][b2-2]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2-1),0};
            allcan[index].push_back(a);
        }

        if (b1+2<=7 && grid[b1+2][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1+1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1+1<=7 && b1-1>=0 && grid[b1-1][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1-2>=0 && grid[b1-1][b2]==pno && grid[b1-2][b2]==pno)
        {
            can a{pair<int,int>(b1-1,b2),2};
            allcan[index].push_back(a);
        }

        if (b1+2<=7 && b2+2<=7 && grid[b1+2][b2+2]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1+1,b2+1),3};
            allcan[index].push_back(a);
        }

        if (b1+1<=7 && b2+1<=7 && b1-1>=0 && b2-1>=0 && grid[b1-1][b2-1]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),3};
            allcan[index].push_back(a);
        }

        if ( b1-2>=0 && b2-2>=0 && grid[b1-1][b2-1]==pno && grid[b1-2][b2-2]==pno)
        {
            can a{pair<int,int>(b1-1,b2-1),3};
            allcan[index].push_back(a);
        }

        if (b1+2<=7 && b2-2>=0 && grid[b1+2][b2-2]==pno && grid[b1+1][b2-1]==pno)
        {
            can a{pair<int,int>(b1+1,b2-1),1};
            allcan[index].push_back(a);
        }

        if (b1+1<=7 && b2-1>=0 && b1-1>=0 && b2+1<=7 && grid[b1+1][b2-1]==pno && grid[b1-1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),1};
            allcan[index].push_back(a);
        }

        if ( b1-2>=0 && b2+2<=7 && grid[b1-1][b2+1]==pno && grid[b1-2][b2+2]==pno)
        {
            can a{pair<int,int>(b1-1,b2+1),1};
            allcan[index].push_back(a);
        }
    }


    void delete_soldier(int b1, int b2, int pno) {
        int index=(1+pno)/2;
        soldier[index].erase(find(soldier[index].begin(), soldier[index].end(), pair<int,int>(b1,b2)));
        numsol[index]-=1;

        if (b2+2<=7 && grid[b1][b2+1]==pno && grid[b1][b2+2]==pno)
        {
            can a{pair<int,int>(b1,b2+1),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b2+1<=7 && b2-1>=0 && grid[b1][b2+1]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b2-2>=0 && grid[b1][b2-2]==pno && grid[b1][b2-1]==pno)
        {
            can a{pair<int,int>(b1,b2-1),0};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<=7 && grid[b1+2][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1+1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<=7 && b1-1>=0 && grid[b1-1][b2]==pno && grid[b1+1][b2]==pno)
        {
            can a{pair<int,int>(b1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1-2>=0 && grid[b1-1][b2]==pno && grid[b1-2][b2]==pno)
        {
            can a{pair<int,int>(b1-1,b2),2};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<=7 && b2+2<=7 && grid[b1+2][b2+2]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1+1,b2+1),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<=7 && b2+1<=7 && b1-1>=0 && b2-1>=0 && grid[b1-1][b2-1]==pno && grid[b1+1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if ( b1-2>=0 && b2-2>=0 && grid[b1-1][b2-1]==pno && grid[b1-2][b2-2]==pno)
        {
            can a{pair<int,int>(b1-1,b2-1),3};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+2<=7 && b2-2>=0 && grid[b1+2][b2-2]==pno && grid[b1+1][b2-1]==pno)
        {
            can a{pair<int,int>(b1+1,b2-1),1};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if (b1+1<=7 && b2-1>=0 && b1-1>=0 && b2+1<=7 && grid[b1+1][b2-1]==pno && grid[b1-1][b2+1]==pno)
        {
            can a{pair<int,int>(b1,b2),1};
            allcan[index].erase(    find_if(allcan[index].begin(), allcan[index].end(), a)   );
        }

        if ( b1-2>=0 && b2+2<=7 && grid[b1-1][b2+1]==pno && grid[b1-2][b2+2]==pno)
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

double minimax(board b, int pno, int isthisme, int ply, string *movefinal)
{
    vector<vector<int>> posmove;
    posmove=b.find_soldier_moves(pno);

    int temp=posmove.size();
    double bestchild=isthisme*10000.0*(-1);
    //aloha beta pruning ke liye 2 baar likhe???????????????????????
    board tcmove;
    double val;
    string s;
    // cerr<<bestchild<<endl;
    for (int mno = 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,true,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            val=tcmove.eval(pno,isthisme);
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s);
        } 

        // cerr << ply << " " << val << " " << bestchild << endl;

        if (isthisme == 1) {
            if (val>bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }
        }
        else {
            if (val < bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }   
        }

        // cerr << ply << " " << val << " " << bestchild << endl;
               
    }

    posmove=b.find_cannon_moves(pno);
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
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s);
        }

        // cerr << ply << " " << val << " " << bestchild << endl;
        
        if ((isthisme == 1)) {
            if (val>bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }
        }
        else{
            if (val < bestchild) {
                bestchild=val;
                if (ply==ply_MAX) {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }   
        }

        // cerr << ply << " " << val << " " << bestchild << endl;
    }

    if (bestchild*isthisme == -10000) {
        bestchild = (-1)*bestchild;
    }

    return bestchild;
    //???? no valid moves at depth handle
}

//??????????????????????????? pno = 1 is me so index is 1, if pno = -1 so index is 0
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

// int main() {
//     board curr;
//     string move;
//     double ttt;
//     print(curr);
//     ttt = minimax(curr, 1, 1, 2, &move);
//     cout << move << endl;

// }

int main() 
{
    string line;
    int pno, N,M, timeq;
    cin>>pno>>N>>M>>timeq;
    getline(cin,line);

    ply_MAX=2;

    // if(pno==2)
    //     ply_MAX=2;
    // else
    //     ply_MAX=3;

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
        ttt=minimax(curr,pno, 1,ply_MAX,&move);
        cout<<move<<endl;
        execute_move(&curr,move,pno);

        getline(cin,move);
        while(move=="")
            getline(cin,move);

        execute_move(&curr,move,(-1)*pno);

        print(curr);
        // exit(0);
    }

}



//AlphaBetaPruning *******************************
//transpose output!!??, @execute move???