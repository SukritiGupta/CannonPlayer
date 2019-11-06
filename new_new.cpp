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

        numth[0]=4;
        numth[1]=4;
    }

    float eval() {
        int ans = nummysol - numothsol + nummyth - numothth;
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
            x=soldier[index].first;
            y=soldier[index].second;

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
                if ((y-3 >= 0) && (grid[x][y-3]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y-3};
                    ans.push_back(temp);
                }
                if ((y-4 >= 0) && (grid[x][y-4]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y-4};
                    ans.push_back(temp);
                }
                if ((y+3 <= 7) && (grid[x][y+3]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y+3};
                    ans.push_back(temp);
                }
                if ((y+4 <= 7) && (grid[x][y+4]*pno <= 0)) {
                    temp = vector<int> {x, y, x, y+4};
                    ans.push_back(temp);
                }
            }
            else if (dir == 1) {
                if ((y-3 >= 0) && (x+3 <= 7) && (grid[x+3][y-3]*pno <= 0)) {
                    temp = vector<int> {x, y, x+3, y-3};
                    ans.push_back(temp);
                }
                if ((y-4 >= 0) && (x+4 <= 7) && (grid[x+4][y-4]*pno <= 0)) {
                    temp = vector<int> {x, y, x+4, y-4};
                    ans.push_back(temp);
                }
                if ((y+3 <= 7) && (x-3 >= 0) && (grid[x-3][y+3]*pno <= 0)) {
                    temp = vector<int> {x, y, x-3, y+3};
                    ans.push_back(temp);
                }
                if ((y+4 <= 7) && (x-4 >= 0) && (grid[x-4][y+4]*pno <= 0)) {
                    temp = vector<int> {x, y, x-4, y+4};
                    ans.push_back(temp);
                }
            }
            else if (dir == 2) {
                if ((x-3 >= 0) && (grid[x-3][y]*pno <= 0)) {
                    temp = vector<int> {x, y, x-3, y};
                    ans.push_back(temp);
                }
                if ((x-4 >= 0) && (grid[x-4][y]*pno <= 0)) {
                    temp = vector<int> {x, y, x-4, y};
                    ans.push_back(temp);
                }
                if ((x+3 <= 7) && (grid[x+3][y]*pno <= 0)) {
                    temp = vector<int> {x, y, x+3, y};
                    ans.push_back(temp);
                }
                if ((x+4 <= 7) && (grid[x+4][y]*pno <= 0)) {
                    temp = vector<int> {x, y, x+4, y};
                    ans.push_back(temp);
                }
            }
            else { //if dir == 3
                if ((y+3 <= 7) && (x+3 <= 7) && (grid[x+3][y+3]*pno <= 0)) {
                    temp = vector<int> {x, y, x+3, y+3};
                    ans.push_back(temp);
                }
                if ((y+4 <= 7) && (x+4 <= 7) && (grid[x+4][y+4]*pno <= 0)) {
                    temp = vector<int> {x, y, x+4, y+4};
                    ans.push_back(temp);
                }
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
        current = delete_soldier(current, a1, a2, true);
    }

    //delete other soldier
    if (current.grid[a3][a4] == (-1)*pno) { //????????pno
        current = delete_soldier(current, a3, a4, false);
    }
    else if (current.grid[a3][a4] == (-2)*pno) {
        current.numothth -=1;
        current.grid[a3][a4]=0;
    }

    //add my soldier
    if (solmove) {
        current = add_soldier(current, a3, a4, pno); //??????????????????Considering 1 is for myself
    }

    return current;

}

board execute_move(board currboard,string move, int pno)
{
    // cerr<<"move executing *********************************************************************************************************************************************** "<<move<<endl;
    if (move[6]=='M')
    {
        currboard=apply_moves(currboard,true,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'),pno);
    }
    else //cannon blast
    {
        currboard=apply_moves(currboard,false,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'),pno);
    }
    return currboard;
}

double minimax(board b, int pno, int isthisme, int ply, string *movefinal)
{
    vector<vector<int>> posmove;
    posmove=b.find_soldier_moves(pno);

    int temp=posmove.size();
    double bestchild=10000.0*(-1);
    //aloha beta pruning ke liye 2 baar likhe???????????????????????
    board tcmove;
    double val;
    string s;
    // cerr<<bestchild<<endl;
    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,true,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            val=tcmove.eval();
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s);
        } 
        if (val>bestchild)
        {
            bestchild=val;
            if (ply==ply_MAX)
            {
                *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
            }
        }       
    }

    posmove=find_cannon_moves(b, pno);
    temp=posmove.size();

    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,false,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            val=tcmove.eval();
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s);
        }
        if (val>bestchild)
        {
            bestchild=val;
            if (ply==ply_MAX)
            {
                *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
            }
        }
    }

    return (-1)*bestchild;
    //???? no valid moves at depth handle
}

void print(board currboard)
{
    // vector<pair<int,int>> mySoldier;
    // // vector< pair<int,int>> myTown;
    // vector<pair<int,int>> otherSoldier;
    // map<pair<int, int>, vector< tup >> mycannon;  //sol to all other sols in any cannon it is in
    // map<pair<int, int>, vector< tup >> otherCannon;

    // vector<can> allmycan;
    // vector<can> allothercan;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            cerr<<currboard.grid[j][i]<<" ";
        }
        cerr<<endl;
    }

    cerr<<currboard.nummysol<<endl;
    cerr<<currboard.numothsol<<endl;
    cerr<<currboard.nummyth<<endl;
    cerr<<currboard.numothth<<endl;

    map<pair<int, int>, vector<tup>>::iterator aa;

    cerr<<"My Cannons"<<endl;

    for ( aa= currboard.mycannon.begin(); aa != currboard.mycannon.end(); ++aa)
    {
        vector<tup> x =aa->second;
        vector<tup>::iterator temp;

        for (temp=x.begin(); temp<x.end(); ++temp)
        {
            cerr<<aa->first.first<<aa->first.second<<"  :  "<<(*temp).a[0]<<","<<(*temp).a[1]<<" "<<(*temp).a[2]<<" "<<(*temp).a[3]<<" "<<endl;
        }
    }

    vector<pair<int, int>>::iterator xx;
    cerr<<"mySoldier"<<endl;
    for (xx=currboard.mySoldier.begin(); xx!=currboard.mySoldier.end(); ++xx)
    {
        cerr<<(*xx).first<<"  "<<(*xx).second<<endl;
    }

    cerr<<"My can"<<endl;
    vector<can>::iterator poorva;
    for (poorva = currboard.allmycan.begin(); poorva != currboard.allmycan.end(); ++poorva)
    {
        cerr<<(*poorva).centre.first<<" "<<(*poorva).centre.second<<"    "<<(*poorva).dir<<endl;
    }

    cerr<<"Other Cannons"<<endl;

    for ( aa= currboard.otherCannon.begin(); aa != currboard.otherCannon.end(); ++aa)
    {
        vector<tup> x =aa->second;
        vector<tup>::iterator temp;

        for (temp=x.begin(); temp<x.end(); ++temp)
        {
            cerr<<aa->first.first<<aa->first.second<<"  :  "<<(*temp).a[0]<<","<<(*temp).a[1]<<" "<<(*temp).a[2]<<" "<<(*temp).a[3]<<" "<<endl;
        }
    }

    cerr<<"otherSoldier"<<endl;
    for (xx=currboard.otherSoldier.begin(); xx!=currboard.otherSoldier.end(); ++xx)
    {
        cerr<<(*xx).first<<"  "<<(*xx).second<<endl;
    }

    cerr<<"Oth can"<<endl;
    // vector<can>::iterator poorva;
    for (poorva = currboard.allothercan.begin(); poorva != currboard.allothercan.end(); ++poorva)
    {
        cerr<<(*poorva).centre.first<<" "<<(*poorva).centre.second<<"    "<<(*poorva).dir<<endl;
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
    int pno, N,M, timeq;
    cin>>pno>>N>>M>>timeq;
    getline(cin,line);

    ply_MAX=3;

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
        curr=execute_move(curr,line,1);

        vector<pair<int,int> > ttemp1 = curr.mySoldier;
        curr.mySoldier = curr.otherSoldier;
        curr.otherSoldier = ttemp1;

        map<pair<int, int>, vector< tup >> ttemp2 = curr.mycannon;
        curr.mycannon = curr.otherCannon;
        curr.otherCannon = ttemp2;

        vector<can> ttemp3 = curr.allmycan;
        curr.allmycan = curr.allothercan;
        curr.allothercan = ttemp3;

        int ttemp4 = curr.nummysol;
        curr.nummysol = curr.numothsol;
        curr.numothsol = ttemp4;

        int ttemp5 = curr.nummyth;
        curr.nummyth=curr.numothth;
        curr.numothth=ttemp5;
    }
    string move;

    vector<pair<int,int> > temp1, ttemp1;
    map<pair<int, int>, vector< tup >> temp2, ttemp2;
    vector<can> temp3, ttemp3;
    int temp4, temp5, ttemp4, ttemp5,ttt;
    double maxval=(double)-10000 ,minval=(double)10000;

    while((true))
    {
        // count++;
        // cerr << "Next Iteration" << endl;
        // print(curr);
        // cerr<<"move "<<move<<endl;
        // usleep(30);
        ttt=minimax(curr,pno, 1,ply_MAX,&move);
        cout<<move<<endl;
        curr=execute_move(curr,move,pno);
        // print(curr);

        temp1 = curr.mySoldier;
        curr.mySoldier = curr.otherSoldier;
        curr.otherSoldier = temp1;

        temp2 = curr.mycannon;
        curr.mycannon = curr.otherCannon;
        curr.otherCannon = temp2;

        temp3 = curr.allmycan;
        curr.allmycan = curr.allothercan;
        curr.allothercan = temp3;

        temp4 = curr.nummysol;
        curr.nummysol = curr.numothsol;
        curr.numothsol = temp4;

        temp5 = curr.nummyth;
        curr.nummyth=curr.numothth;
        curr.numothth=temp5;

        // getline(cin,move);
        getline(cin,move);
        while(move=="")
            getline(cin,move);

        curr=execute_move(curr,move,(-1)*pno);
        // print(curr);
        ttemp1 = curr.mySoldier;
        curr.mySoldier = curr.otherSoldier;
        curr.otherSoldier = ttemp1;

        ttemp2 = curr.mycannon;
        curr.mycannon = curr.otherCannon;
        curr.otherCannon = ttemp2;

        ttemp3 = curr.allmycan;
        curr.allmycan = curr.allothercan;
        curr.allothercan = ttemp3;

        ttemp4 = curr.nummysol;
        curr.nummysol = curr.numothsol;
        curr.numothsol = ttemp4;

        ttemp5 = curr.nummyth;
        curr.nummyth=curr.numothth;
        curr.numothth=ttemp5;

        // exit(0);
    }

}



//AlphaBetaPruning *******************************
//transpose output!!??, @execute move???