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
    vector<pair<int,int>> mySoldier;
    vector<pair<int,int>> otherSoldier;
    // vector of cannons - seems tough to be inferenced from the board
    map<pair<int, int>, vector< tup >> mycannon;  //sol to all other sols in any cannon it is in
    map<pair<int, int>, vector< tup >> otherCannon;
    //vector of my cannons with centre piecs and direction
    vector<can> allmycan;
    vector<can> allothercan;

    // can be done away with the following variables maybe
    //int pno;// +1 or -1
    int nummysol;
    int numothsol;

    int nummyth;
    int numothth;

    //initialiser
    board() 
    {
        nummyth = 4;
        numothth = 4;
        vector<vector<int>> temp1{{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2},{-2,0,0,0,0,1,1,1},{-1,-1,-1,0,0,0,0,2}};
        grid=temp1;

        vector<pair<int, int>> temp2 = { {0,5},{0,6},{0,7}, {2,5},{2,6},{2,7}, {4,5},{4,6},{4,7}, {6,5},{6,6},{6,7} };
        mySoldier = temp2;

        vector<pair<int, int>> temp3 = { {1,0},{1,1},{1,2}, {3,0},{3,1},{3,2}, {5,0},{5,1},{5,2}, {7,0},{7,1},{7,2} };
        otherSoldier = temp3;

        tup a(0,6,0,7);        vector<tup> v={a};        mycannon[{0,5}]=v;        v[0].a[1]=5;        mycannon[{0,6}]=v;

        v[0].a[3]=6;        mycannon[{0,7}]=v;        tup a1(2,6,2,7);        vector<tup> v1={a1};        mycannon[{2,5}]=v1;        v1[0].a[1]=5;        mycannon[{2,6}]=v1;
        v1[0].a[3]=6;        mycannon[{2,7}]=v1;

        tup a2(4,6,4,7);        vector<tup> v2={a2};        mycannon[{4,5}]=v2;
        v2[0].a[1]=5;        mycannon[{4,6}]=v2;
        v2[0].a[3]=6;        mycannon[{4,7}]=v2;

        tup a3(6,6,6,7);        vector<tup> v3={a3};        mycannon[{6,5}]=v3;
        v3[0].a[1]=5;        mycannon[{6,6}]=v3;
        v3[0].a[3]=6;        mycannon[{6,7}]=v3;

        tup b(1,1,1,2);
        vector<tup> t={b};
        otherCannon[{1,0}]=t;

        t[0].a[1]=0;
        otherCannon[{1,1}]=t;

        t[0].a[3]=1;
        otherCannon[{1,2}]=t;

        tup b1(3,1,3,2);
        vector<tup> t1={b1};
        otherCannon[{3,0}]=t1;

        t1[0].a[1]=0;
        otherCannon[{3,1}]=t1;

        t1[0].a[3]=1;
        otherCannon[{3,2}]=t1;

        tup b2(5,1,5,2);
        vector<tup> t2={b2};
        otherCannon[{5,0}]=t2;

        t2[0].a[1]=0;
        otherCannon[{5,1}]=t2;

        t2[0].a[3]=1;
        otherCannon[{5,2}]=t2;

        tup b3(7,1,7,2);
        vector<tup> t3={b3};
        otherCannon[{7,0}]=t3;

        t3[0].a[1]=0;
        otherCannon[{7,1}]=t3;

        t3[0].a[3]=1;
        otherCannon[{7,2}]=t3;


        can c{{0,6},0};
        allmycan.push_back(c);
        c.centre.first=2;
        allmycan.push_back(c);
        c.centre.first=4;
        allmycan.push_back(c);
        c.centre.first=6;
        allmycan.push_back(c);

        can c1{{1,1},0};
        allothercan.push_back(c1);
        c1.centre.first=3;
        allothercan.push_back(c1);
        c1.centre.first=5;
        allothercan.push_back(c1);
        c1.centre.first=7;
        allothercan.push_back(c1);

        nummysol=12;
        numothsol=12;
    }

    float eval() {
        int ans = nummyth - numothth;
        // cerr<<"eval called "<<nummysol<<" "<<numothsol<<"  "<<nummyth<<"  "<<numothth<<endl;
        return ans;
    }
};

//written assuming that we will be having the above attributes to a board
//returns a vector of int where 1 - original x, 2 - original y, 3 - new x, 4 - new y 
vector<vector<int> > find_soldier_moves(board current, int pno) {

    // cerr<<"find_soldier_moves called"<<endl;

	//ans vector to be returned
	vector<vector<int> > ans;
	vector<int> temp;

	//iterator for traversing the soldier vector
	vector<pair<int, int> >::iterator ptr;
	int x, y;

	//looping over the soldiers
	for (ptr = current.mySoldier.begin(); ptr!=current.mySoldier.end(); ptr++) {
		//x coordinate and y coordinate
		x = (*ptr).first;
		y = (*ptr).second;

		//forward move: <=0 because empty or other's soldier
        // cerr<<"1" << endl;
		if ((y-pno >= 0) && (y-pno <= 7) && (current.grid[x][y-pno]*pno <= 0)) {
			temp = vector<int> {x, y, x, y-pno};
			ans.push_back(temp);
		}

		//right diagonal move
		if ((x+pno <= 7) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= 7) && (current.grid[x+pno][y-pno]*pno <= 0)) {
			temp = vector<int> {x, y, x+pno, y-pno};
			ans.push_back(temp);
		} 

		//left diagonal move
		if ((x-pno >= 0) && (x-pno <= 7) && (y-pno >= 0) && (y-pno <= 7) && (current.grid[x-pno][y-pno]*pno <= 0)) {
			temp = vector<int> {x, y, x-pno, y-pno};
			ans.push_back(temp);
		}

		//right adjacent move
		if ((x+pno <= 7) && (x+pno >= 0) && (current.grid[x+pno][y]*pno < 0)) {
			temp = vector<int> {x, y, x+pno, y};
			ans.push_back(temp);
		}

		//left adjacent move
		if ((x-pno >= 0) && (x-pno <= 7) && (current.grid[x-pno][y]*pno < 0)) {
			temp = vector<int> {x, y, x-pno, y};
			ans.push_back(temp);
		}

		//retreat moves, considering that it can kill soldiers while retreating
        // ??????????????????? can a soldier kill other soldiers while retreating
        if (((y-pno >= 0) && (y-pno <= 7) && (current.grid[x][y-pno]*pno < 0)) || ((x+pno <= 7) && (x+pno >= 0) && (y-pno >= 0) && (y-pno <= 7) && (current.grid[x+pno][y-pno]*pno < 0)) || 
            ((x-pno >= 0) && (x-pno <= 7) && (y-pno >= 0) && (y-pno <= 7) && (current.grid[x-pno][y-pno]*pno < 0)) || 
            ((x+pno <= 7) && (x+pno >= 0) && (current.grid[x+pno][y]*pno < 0)) || ((x-pno >= 0) && (x-pno <= 7) && (current.grid[x-pno][y]*pno < 0))) {

            if ((y+2*pno <= 7) && (y+2*pno >= 0) && (current.grid[x][y+2*pno]*pno <= 0)) {
                temp = vector<int> {x, y, x, y+2*pno};
                ans.push_back(temp);
            }

            if ((y+2*pno <=7) && (y+2*pno >= 0) && (x+2*pno <=7) && (x+2*pno >= 0) && (current.grid[x+2*pno][y+2*pno]*pno <= 0)) {
                temp = vector<int> {x, y, x+2*pno, y+2*pno};
                ans.push_back(temp);
            }

            if ((y+2*pno <=7) && (y+2*pno >= 0) && (x-2*pno >= 0) && (x-2*pno <= 7) && (current.grid[x-2*pno][y+2*pno]*pno <= 0)) {
                temp = vector<int> {x, y, x-2*pno, y+2*pno};
                ans.push_back(temp);
            }
        }
	}

    //??????????????????????? a cannon can't kill soldier while moving
    vector<can>::iterator ptr2;
    int dir;
    for (ptr2 = current.allmycan.begin(); ptr2 != current.allmycan.end(); ptr2++) {
        x = (*ptr2).centre.first;
        y = (*ptr2).centre.second;
        dir = (*ptr2).dir;

        if (dir == 0) {
            if ((y-2 >= 0) && (current.grid[x][y-2] == 0)) {
                temp = vector<int> {x, y+1, x, y-2};
                ans.push_back(temp);
            }
            if ((y+2 <= 7) && (current.grid[x][y+2] == 0)) {
                temp = vector<int> {x, y-1, x, y+2};
                ans.push_back(temp);
            }
        }
        else if (dir == 1) {
            if ((y-2 >= 0) && (x+2 <= 7) && (current.grid[x+2][y-2] == 0)) {
                temp = vector<int> {x-1, y+1, x+2, y-2};
                ans.push_back(temp);
            }
            if ((y+2 <= 7) && (x-2 >= 0) && (current.grid[x-2][y+2] == 0)) {
                temp = vector<int> {x+1, y-1, x-2, y+2};
                ans.push_back(temp);
            }
        }
        else if (dir == 2) {
            if ((x-2 >= 0) && (current.grid[x-2][y] == 0)) {
                temp = vector<int> {x+1, y, x-2, y};
                ans.push_back(temp);
            }
            if ((x+2 <= 7) && (current.grid[x+2][y] == 0)) {
                temp = vector<int> {x-1, y, x+2, y};
                ans.push_back(temp);
            }
        }
        else { //if dir == 3
            if ((y+2 <= 7) && (x+2 <= 7) && (current.grid[x+2][y+2] == 0)) {
                temp = vector<int> {x-1, y-1, x+2, y+2};
                ans.push_back(temp);
            }
            if ((y-2 >= 0) && (x-2 >= 0) && (current.grid[x-2][y-2] == 0)) {
                temp = vector<int> {x+1, y+1, x-2, y-2};
                ans.push_back(temp);
            }
        }
    }
    // cerr<<"find_soldier_moves ended"<<endl;

	return ans;
}

//??????????????????khudko mat udana
vector<vector<int> > find_cannon_moves(board current, int pno) {
    //ans vector to be returned
    vector<vector<int> > ans;
    vector<int> temp;

    //????????????????????????????can empty blast
    vector<can>::iterator ptr;
    int x, y, dir;
    for (ptr = current.allmycan.begin(); ptr != current.allmycan.end(); ptr++) {
        x = (*ptr).centre.first;
        y = (*ptr).centre.second;
        dir = (*ptr).dir;

        if (dir == 0) {
            if ((y-3 >= 0) && (current.grid[x][y-3]*pno <= 0)) {
                temp = vector<int> {x, y, x, y-3};
                ans.push_back(temp);
            }
            if ((y-4 >= 0) && (current.grid[x][y-4]*pno <= 0)) {
                temp = vector<int> {x, y, x, y-4};
                ans.push_back(temp);
            }
            if ((y+3 <= 7) && (current.grid[x][y+3]*pno <= 0)) {
                temp = vector<int> {x, y, x, y+3};
                ans.push_back(temp);
            }
            if ((y+4 <= 7) && (current.grid[x][y+4]*pno <= 0)) {
                temp = vector<int> {x, y, x, y+4};
                ans.push_back(temp);
            }
        }
        else if (dir == 1) {
            if ((y-3 >= 0) && (x+3 <= 7) && (current.grid[x+3][y-3]*pno <= 0)) {
                temp = vector<int> {x, y, x+3, y-3};
                ans.push_back(temp);
            }
            if ((y-4 >= 0) && (x+4 <= 7) && (current.grid[x+4][y-4]*pno <= 0)) {
                temp = vector<int> {x, y, x+4, y-4};
                ans.push_back(temp);
            }
            if ((y+3 <= 7) && (x-3 >= 0) && (current.grid[x-3][y+3]*pno <= 0)) {
                temp = vector<int> {x, y, x-3, y+3};
                ans.push_back(temp);
            }
            if ((y+4 <= 7) && (x-4 >= 0) && (current.grid[x-4][y+4]*pno <= 0)) {
                temp = vector<int> {x, y, x-4, y+4};
                ans.push_back(temp);
            }
        }
        else if (dir == 2) {
            if ((x-3 >= 0) && (current.grid[x-3][y]*pno <= 0)) {
                temp = vector<int> {x, y, x-3, y};
                ans.push_back(temp);
            }
            if ((x-4 >= 0) && (current.grid[x-4][y]*pno <= 0)) {
                temp = vector<int> {x, y, x-4, y};
                ans.push_back(temp);
            }
            if ((x+3 <= 7) && (current.grid[x+3][y]*pno <= 0)) {
                temp = vector<int> {x, y, x+3, y};
                ans.push_back(temp);
            }
            if ((x+4 <= 7) && (current.grid[x+4][y]*pno <= 0)) {
                temp = vector<int> {x, y, x+4, y};
                ans.push_back(temp);
            }
        }
        else { //if dir == 3
            if ((y+3 <= 7) && (x+3 <= 7) && (current.grid[x+3][y+3]*pno <= 0)) {
                temp = vector<int> {x, y, x+3, y+3};
                ans.push_back(temp);
            }
            if ((y+4 <= 7) && (x+4 <= 7) && (current.grid[x+4][y+4]*pno <= 0)) {
                temp = vector<int> {x, y, x+4, y+4};
                ans.push_back(temp);
            }
            if ((y-3 >= 0) && (x-3 >= 0) && (current.grid[x-3][y-3]*pno <= 0)) {
                temp = vector<int> {x, y, x-3, y-3};
                ans.push_back(temp);
            }
            if ((y-4 >= 0) && (x-4 >= 0) && (current.grid[x-4][y-4]*pno <= 0)) {
                temp = vector<int> {x, y, x-4, y-4};
                ans.push_back(temp);
            }
        }
    }

    return ans;
}

tup add_change_cannon_single(board *newboard, int b1, int b2, int x1, int y1, int x2, int y2, int dir, int ccx, int ccy)
{
    map<pair<int, int>, vector<tup>>::iterator   exist;
    can a{pair<int,int>(ccx,ccy),dir};
    (*newboard).allmycan.push_back(a);

    struct tup x{b1,b2,x2,y2};
    exist=(*newboard).mycannon.find(pair<int,int>(x1,y1));
    vector<tup> yy{x};
    
    if (exist != (*newboard).mycannon.end())
    {
        yy = (*newboard).mycannon.at(pair<int,int>(x1,y1));
        yy.push_back(x);
    }
    (*newboard).mycannon[{x1,y1}] = yy;

    exist=(*newboard).mycannon.find(pair<int,int>(x2,y2));
    x.a[0]=b1;
    x.a[1]=b2;
    x.a[2]=x1;
    x.a[3]=y1;

    vector<tup> y{x};

    if (exist != (*newboard).mycannon.end())
    {
        y = (*newboard).mycannon.at(pair<int,int>(x2,y2));
        y.push_back(x);
    }

    (*newboard).mycannon[{x2,y2}] = y;

    x.a[0]=x2;
    x.a[1]=y2;
    x.a[2]=x1;
    x.a[3]=y1;
    return x;
}

board add_soldier(board newboard, int b1, int b2, int pno)
{
    newboard.grid[b1][b2]=pno;
    newboard.mySoldier.push_back(pair<int,int>(b1,b2));
    newboard.nummysol+=1;

    //Updating cannons
    vector<tup> finalpush,y;

    if (b2+2<=7 && newboard.grid[b1][b2+1]==pno && newboard.grid[b1][b2+2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1,b2+1,b1,b2+2,0,b1,b2+1));
    }

    if (b2+1<=7 && b2-1>=0 && newboard.grid[b1][b2+1]==pno && newboard.grid[b1][b2-1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1 ,b2+1,b1,b2-1, 0 ,b1  ,b2));
    }

    if (b2-2>=0 && newboard.grid[b1][b2-2]==pno && newboard.grid[b1][b2-1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1 ,b2-1,b1,b2-2,0,b1,b2-1));
    }

    if (b1+2<=7 && newboard.grid[b1+2][b2]==pno && newboard.grid[b1+1][b2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2,b1+2,b2,2,b1+1,b2));
    }

    if (b1+1<=7 && b1-1>=0 && newboard.grid[b1-1][b2]==pno && newboard.grid[b1+1][b2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2,b1-1,b2,2,b1,b2));
    }

    if (b1-2>=0 && newboard.grid[b1-1][b2]==pno && newboard.grid[b1-2][b2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1-1,b2,b1-2,b2,2,b1-1,b2));
    }

    if (b1+2<=7 && b2+2<=7 && newboard.grid[b1+2][b2+2]==pno && newboard.grid[b1+1][b2+1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2+1,b1+2,b2+2,3,b1+1,b2+1));
    }

    if (b1+1<=7 && b2+1<=7 && b1-1>=0 && b2-1>=0 && newboard.grid[b1-1][b2-1]==pno && newboard.grid[b1+1][b2+1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2+1,b1-1,b2-1,3,b1,b2));
    }

    if ( b1-2>=0 && b2-2>=0 && newboard.grid[b1-1][b2-1]==pno && newboard.grid[b1-2][b2-2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1-1,b2-1,b1-2,b2-2,3,b1-1,b2-1));
    }

    if (b1+2<=7 && b2-2>=0 && newboard.grid[b1+2][b2-2]==pno && newboard.grid[b1+1][b2-1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2-1,b1+2,b2-2,1,b1+1,b2-1));
    }

    if (b1+1<=7 && b2-1>=0 && b1-1>=0 && b2+1<=7 && newboard.grid[b1+1][b2-1]==pno && newboard.grid[b1-1][b2+1]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1+1,b2-1,b1-1,b2+1,1,b1,b2));
    }

    if ( b1-2>=0 && b2+2<=7 && newboard.grid[b1-1][b2+1]==pno && newboard.grid[b1-2][b2+2]==pno)
    {
        finalpush.push_back(add_change_cannon_single(&newboard, b1,b2,b1-1,b2+1,b1-2,b2+2,1,b1-1,b2+1));
    }

    if (finalpush.size()>0)
    {
        newboard.mycannon[{b1,b2}] = finalpush;
    }

    return newboard;

}

board delete_change_cannon(board newboard, int b1, int b2) {

    //??????????????? need to check the sequence of events

    map<pair<int, int>, vector<tup>>::iterator dicval = newboard.mycannon.find(pair<int,int>(b1,b2));
    if (dicval!=newboard.mycannon.end()) {
        vector<tup> neighbours, neighbours1, neighbours2;
        neighbours = dicval->second;
        vector<tup>::iterator iter,temp;
        int s1x,s1y,s2x,s2y;

        for (iter = neighbours.begin(); iter < neighbours.end(); iter++) {
            s1x=(*iter).a[0];
            s1y=(*iter).a[1];
            s2x=(*iter).a[2];
            s2y=(*iter).a[3];

            neighbours1 = newboard.mycannon.at(pair<int,int>(s1x,s1y));

            if (neighbours1.size() > 1) {
                struct tup x(b1,b2,s2x,s2y);
                temp = find_if(neighbours1.begin(), neighbours1.end(), x );

                if (temp!=neighbours1.end())
                {
                    neighbours1.erase(temp);
                }
                else
                {
                    x.a[0]=s2x;
                    x.a[1]=s2y;
                    x.a[2]=b1;
                    x.a[3]=b2;
                    neighbours1.erase( find_if(neighbours1.begin(), neighbours1.end(), x ) );   
                }
                newboard.mycannon[pair<int,int>(s1x,s1y)]=neighbours1;
            }
            else
            {
                newboard.mycannon.erase(pair<int,int>(s1x,s1y));
            }

            //repeat for s2
            // cerr<<s2x<<s2y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            neighbours2=newboard.mycannon.at(pair<int,int>(s2x,s2y));
            

            if(neighbours2.size()>1)
            {
                // int arr[4]={b1,b2,s1x,s1y};
                struct tup x(b1,b2,s1x,s1y);

                temp=find_if(neighbours2.begin(), neighbours2.end(), x );

                if (temp!=neighbours2.end())
                {
                    neighbours2.erase(temp);
                }
                else
                {
                    x.a[0]=s1x;
                    x.a[1]=s1y;
                    x.a[2]=b1;
                    x.a[3]=b2;
                    neighbours2.erase( find_if(neighbours2.begin(), neighbours2.end(), x ) );   
                }
                newboard.mycannon[pair<int,int>(s2x,s2y)]=neighbours2;
            }
            else
            {
                newboard.mycannon.erase(pair<int,int>(s2x,s2y));
            }

            if (b1==s1x && b1==s2x) //dir=0
            {
                if ((b2<s1y && s1y<s2y)|| (b2>s1y && s1y>s2y))
                {
                    can a {pair<int,int> {s1x,s1y},0};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((b2<s2y && s2y<s1y)|| (b2>s2y && s2y>s1y))
                {
                    can a{pair<int,int>(s2x,s2y),0};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1y<b2 && b2<s2y)||(s1y>b2 && b2>s2y))
                {
                    can a{pair<int,int>(b1,b2),0};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }

            else if (b2==s1y && b2==s2y) //dir=2
            {
                if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                {
                    can a{pair<int,int>(b1,b2),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }

            else if ( b2-s2y == b1-s2x) //dir=1
            {
                if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                {
                    can a{pair<int,int>(b1,b2),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }
            else //dir=3
            {
                if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                {
                    can a{pair<int,int>(b1,b2),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }
        }
        newboard.mycannon.erase(pair<int,int>(b1,b2));
    }
    return newboard;
}

board delete_soldier(board newboard, int b1, int b2, bool me) {

    vector<pair<int, int> > temp1;
    map<pair<int, int>, vector< tup >> temp2;
    vector<can> temp3;
    int temp4, temp5; 

    if (!me) {
        temp1 = newboard.mySoldier;
        newboard.mySoldier = newboard.otherSoldier;
        newboard.otherSoldier = temp1;

        temp2 = newboard.mycannon;
        newboard.mycannon = newboard.otherCannon;
        newboard.otherCannon = temp2;

        temp3 = newboard.allmycan;
        newboard.allmycan = newboard.allothercan;
        newboard.allothercan = temp3;

        temp4 = newboard.nummysol;
        newboard.nummysol = newboard.numothsol;
        newboard.numothsol = temp4;

        temp5 = newboard.nummyth;
        newboard.nummyth=newboard.numothth;
        newboard.numothth=temp5;
    }

    newboard.mySoldier.erase(find(newboard.mySoldier.begin(), newboard.mySoldier.end(), pair<int,int>(b1,b2)));
    newboard.nummysol-=1;
    newboard = delete_change_cannon(newboard, b1, b2);
    newboard.grid[b1][b2] = 0;

    if (!me) {
        temp1 = newboard.mySoldier;
        newboard.mySoldier = newboard.otherSoldier;
        newboard.otherSoldier = temp1;

        temp2 = newboard.mycannon;
        newboard.mycannon = newboard.otherCannon;
        newboard.otherCannon = temp2;

        temp3 = newboard.allmycan;
        newboard.allmycan = newboard.allothercan;
        newboard.allothercan = temp3;

        temp4 = newboard.nummysol;
        newboard.nummysol = newboard.numothsol;
        newboard.numothsol = temp4;

        temp5 = newboard.nummyth;
        newboard.nummyth=newboard.numothth;
        newboard.numothth=temp5;
    }

    return newboard;
}

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
    cerr << "ye minimax with ply_MAX " << ply_MAX <<  endl;
    if (ply!=ply_MAX)
    {
        pno = pno*(-1);
        vector<pair<int,int> > temp1 = b.mySoldier;
        b.mySoldier = b.otherSoldier;
        b.otherSoldier = temp1;

        map<pair<int, int>, vector< tup >> temp2 = b.mycannon;
        b.mycannon = b.otherCannon;
        b.otherCannon = temp2;

        vector<can> temp3 = b.allmycan;
        b.allmycan = b.allothercan;
        b.allothercan = temp3;

        int temp4 = b.nummysol;
        b.nummysol = b.numothsol;
        b.numothsol = temp4;

        int temp5 = b.nummyth;
        b.nummyth=b.numothth;
        b.numothth=temp5;
    }

    vector<vector<int>> posmove;
    posmove=find_soldier_moves(b, pno);

    int temp=posmove.size();
    double bestchild=10000.0*(-1);
    //aloha beta pruning ke liye 2 baar likhe???????????????????????
    board tcmove;
    double val;
    string s;
    cerr<<bestchild<<endl;
    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,true,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3],pno);
        if (ply==0)
        {
            // val=tcmove.eval(isthisme,pno);
            // cerr<<"This is move sol"<<("S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]));
            val=tcmove.eval();
            // cerr << "ply is 0 so val is " << val << endl;
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1,&s);
            // cerr << "Val " << val << endl;
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
            // val=tcmove.eval(isthisme,pno);
            // cerr<<"This is move cannon"<<("S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " B " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]));
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

    // cerr << "Returned Value " << (-1)*bestchild << endl;
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

// int main() {
//     board newboard;

//     // vector<pair<int, int> > temp1;
//     // map<pair<int, int>, vector< tup >> temp2;
//     // vector<can> temp3;
//     // int temp4, temp5; 
//     // temp1 = newboard.mySoldier;
//     // newboard.mySoldier = newboard.otherSoldier;
//     // newboard.otherSoldier = temp1;

//     // temp2 = newboard.mycannon;
//     // newboard.mycannon = newboard.otherCannon;
//     // newboard.otherCannon = temp2;

//     // temp3 = newboard.allmycan;
//     // newboard.allmycan = newboard.allothercan;
//     // newboard.allothercan = temp3;

//     // temp4 = newboard.nummysol;
//     // newboard.nummysol = newboard.numothsol;
//     // newboard.numothsol = temp4;

//     // temp5 = newboard.nummyth;
//     // newboard.nummyth=newboard.numothth;
//     // newboard.numothth=temp5;

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
        cerr << "Next Iteration" << endl;
        print(curr);
        cerr<<"move "<<move<<endl;
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