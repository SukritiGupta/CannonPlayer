#include <iostream>
#include <vector>
#include <map>

using namespace std;

//Struct for cannons
struct can {
	//centre piece
    pair<int,int> centre;
    int dir; //0,1,2,3 //direction

    // ************************************* No idea of this
    // bool operator () (const can & m) const {
    //     return (m.dir==dir && m.centre.first==centre.first && m.centre.second==centre.second);
    // }
};

//tuple for 
//**************************************No idea of this
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
    board() {
    }
};

//written assuming that we will be having the above attributes to a board
//returns a vector of int where 1 - original x, 2 - original y, 3 - new x, 4 - new y 
vector<vector<int> > find_soldier_moves(board current) {

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
		if ((current.grid[x][y-1] <= 0) && (y-1 >= 0)) {
			temp = vector<int> {x, y, x, y-1};
			ans.push_back(temp);
		}

		//right diagonal move
		if ((current.grid[x+1][y-1] <= 0) && (x+1 <= 7) && (y-1 >= 0)) {
			temp = vector<int> {x, y, x+1, y-1};
			ans.push_back(temp);
		} 

		//left diagonal move
		if ((current.grid[x-1][y-1] <= 0) && (x-1 >= 0) && (y-1 >= 0)) {
			temp = vector<int> {x, y, x-1, y-1};
			ans.push_back(temp);
		}

		//right adjacent move
		if ((current.grid[x+1][y] < 0) && (x+1 <= 7)) {
			temp = vector<int> {x, y, x+1, y};
			ans.push_back(temp);
		}

		//left adjacent move
		if ((current.grid[x-1][y] < 0) && (x-1 >= 0)) {
			temp = vector<int> {x, y, x-1, y};
			ans.push_back(temp);
		}

		//retreat moves, considering that it can kill soldiers while retreating
        if (((current.grid[x][y-1] < 0) && (y-1 >= 0)) || ((current.grid[x+1][y-1] < 0) && (x+1 <= 7) && (y-1 >=0)) || 
            ((current.grid[x-1][y-1] < 0) && (x-1 >= 0) && (y-1 >= 0)) || ((current.grid[x+1][y] < 0) && (x+1 <=7)) || 
            ((current.grid[x-1][y] < 0) && (x-1 >= 0))) {

            if ((current.grid[x][y+2] <= 0) && (y+2 <=7)) {
                temp = vector<int> {x, y, x, y+2};
                ans.push_back(temp);
            }

            if ((current.grid[x+2][y+2] <= 0) && (y+2 <=7) && (x+2 <=7)) {
                temp = vector<int> {x, y, x+2, y+2};
                ans.push_back(temp);
            }

            if ((current.grid[x-2][y+2] <= 0) && (y+2 <=7) && (x-2 >= 0)) {
                temp = vector<int> {x, y, x-2, y+2};
                ans.push_back(temp);
            }
        }
	}
	return ans;
}

board add_change_cannon()
{

}

board add_soldier(board, x,y)
{

}

board delete_change_cannon(board, x, y )
{

}

board delete_soldier(board, x, y)
{
    change_cannon()
}

board apply_moves(board, bool solmove, int a1, int a2, int a3, int a4)
{

}


double minimax(board b, int pno, int isthisme, int ply, string *movefinal="")
{
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
    posmove=find_soldier_moves(b);
    int temp=posmove.size();
    double bestchild=isthisme*10000.0*(-1);
    board tcmove;


    for (int  mno= 0; mno < temp; ++mno)
    {
        tcmove=apply_moves(b,1,posmove[mno][0],posmove[mno][1],posmove[mno][2],posmove[mno][3]);
        if (ply==0)
        {
            val=tcmove.eval(isthisme,pno);
            if (val>bestchild)
            {
                bestchild=val;
            }
        }
        else
        {
            val=minimax(tcmove,pno*(-1),isthisme*(-1),ply-1);
            if (val>bestchild)
            {
                bestchild=val;
                if (ply==ply_MAX)
                {
                    *movefinal="S " + to_string(posmove[mno][0]) + " " + to_string(posmove[mno][1]) + " M " + to_string(posmove[mno][2])+ " " +to_string(posmove[mno][3]);
                }
            }
        }        
    }

    //???? no valid moves at depth handle
}






int main() {
	cout << "Hi" << endl;
	vector<int> temp;
	temp = vector<int> {2,3,4,5};
	temp = vector<int> {1,2,3,4};
    cout << temp[100] << endl;
	for (int i = 0; i < temp.size(); i++) {
		cout << temp[i] << endl;
	}

}