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
        // ??????????????????? can a soldier kill other soldiers while retreating
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

    //??????????????????????? a cannon can't kill soldier while moving
    vector<can>::iterator ptr2;
    int dir;
    for (ptr2 = current.allmycan.begin(); ptr2 != current.allmycan.end(); ptr2++) {
        x = (*ptr2).centre.first;
        y = (*ptr2).centre.second;
        dir = (*ptr2).dir;

        if (dir == 0) {
            if ((current.grid[x][y-2] == 0) && (y-2 >= 0)) {
                temp = vector<int> {x, y+1, x, y-2};
                ans.push_back(temp);
            }
            if ((current.grid[x][y+2] == 0) && (y+2 <= 7)) {
                temp = vector<int> {x, y-1, x, y+2};
                ans.push_back(temp);
            }
        }
        else if (dir == 1) {
            if ((current.grid[x+2][y-2] == 0) && (y-2 >= 0) && (x+2 <= 7)) {
                temp = vector<int> {x-1, y+1, x+2, y-2};
                ans.push_back(temp);
            }
            if ((current.grid[x-2][y+2] == 0) && (y+2 <= 7) && (x-2 >= 0)) {
                temp = vector<int> {x+1, y-1, x-2, y+2};
                ans.push_back(temp);
            }
        }
        else if (dir == 2) {
            if ((current.grid[x-2][y] == 0) && (x-2 >= 0)) {
                temp = vector<int> {x+1, y, x-2, y};
                ans.push_back(temp);
            }
            if ((current.grid[x+2][y] == 0) && (x+2 <= 7)) {
                temp = vector<int> {x-1, y, x+2, y};
                ans.push_back(temp);
            }
        }
        else { //if dir == 3
            if ((current.grid[x+2][y+2] == 0) && (y+2 <= 7) && (x+2 <= 7)) {
                temp = vector<int> {x-1, y-1, x+2, y+2};
                ans.push_back(temp);
            }
            if ((current.grid[x-2][y-2] == 0) && (y-2 >= 0) && (x-2 >= 0)) {
                temp = vector<int> {x+1, y+1, x-2, y-2};
                ans.push_back(temp);
            }
        }
    }
	return ans;
}

vector<vector<int> > find_cannon_moves(board current) {
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
            if ((current.grid[x][y-3] <= 0) && (y-3 >= 0)) {
                temp = vector<int> {x, y, x, y-3};
                ans.push_back(temp);
            }
            if ((current.grid[x][y-4] <= 0) && (y-4 >= 0)) {
                temp = vector<int> {x, y, x, y-4};
                ans.push_back(temp);
            }
            if ((current.grid[x][y+3] <= 0) && (y+3 <= 7)) {
                temp = vector<int> {x, y, x, y+3};
                ans.push_back(temp);
            }
            if ((current.grid[x][y+4] <= 0) && (y+4 <= 7)) {
                temp = vector<int> {x, y, x, y+4};
                ans.push_back(temp);
            }
        }
        else if (dir == 1) {
            if ((current.grid[x+3][y-3] <= 0) && (y-3 >= 0) && (x+3 <= 7)) {
                temp = vector<int> {x, y, x+3, y-3};
                ans.push_back(temp);
            }
            if ((current.grid[x+4][y-4] <= 0) && (y-4 >= 0) && (x+4 <= 7)) {
                temp = vector<int> {x, y, x+4, y-4};
                ans.push_back(temp);
            }
            if ((current.grid[x-3][y+3] <= 0) && (y+3 <= 7) && (x-3 >= 0)) {
                temp = vector<int> {x, y, x-3, y+3};
                ans.push_back(temp);
            }
            if ((current.grid[x-4][y+4] <= 0) && (y+4 <= 7) && (x-4 >= 0)) {
                temp = vector<int> {x, y, x-4, y+4};
                ans.push_back(temp);
            }
        }
        else if (dir == 2) {
            if ((current.grid[x-3][y] <= 0) && (x-3 >= 0)) {
                temp = vector<int> {x, y, x-3, y};
                ans.push_back(temp);
            }
            if ((current.grid[x-4][y] <= 0) && (x-4 >= 0)) {
                temp = vector<int> {x, y, x-4, y};
                ans.push_back(temp);
            }
            if ((current.grid[x+3][y] <= 0) && (x+3 <= 7)) {
                temp = vector<int> {x, y, x+3, y};
                ans.push_back(temp);
            }
            if ((current.grid[x+4][y] <= 0) && (x+4 <= 7)) {
                temp = vector<int> {x, y, x+4, y};
                ans.push_back(temp);
            }
        }
        else { //if dir == 3
            if ((current.grid[x+3][y+3] <= 0) && (y+3 <= 7) && (x+3 <= 7)) {
                temp = vector<int> {x, y, x+3, y+3};
                ans.push_back(temp);
            }
            if ((current.grid[x+4][y+4] <= 0) && (y+4 <= 7) && (x+4 <= 7)) {
                temp = vector<int> {x, y, x+4, y+4};
                ans.push_back(temp);
            }
            if ((current.grid[x-3][y-3] <= 0) && (y-3 >= 0) && (x-3 >= 0)) {
                temp = vector<int> {x, y, x-3, y-3};
                ans.push_back(temp);
            }
            if ((current.grid[x-4][y-4] == 0) && (y-4 >= 0) && (x-4 >= 0)) {
                temp = vector<int> {x, y, x-4, y-4};
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

board delete_change_cannon(board newboard, int b1, int b2) {

    map<pair<int, int>, vector<tup>>::iterator dicval = newboard.mycannon.find(pair<int,int>(b1,b2));
    if (dicval!=newboard.mycannon.end())
    {
        vector<tup> neighbours, neighbours1, neighbours2;
        neighbours = dicval->second;
        vector<tup>::iterator iter,temp;
        int s1x,s1y,s2x,s2y;

        for (iter = neighbours.begin(); iter < neighbours.end(); iter++)
        {
            s1x=(*iter).a[0];
            s1y=(*iter).a[1];
            s2x=(*iter).a[2];
            s2y=(*iter).a[3];

            neighbours1=newboard.mycannon.at(pair<int,int>(s1x,s1y));
            

            if(neighbours1.size()>1)
            {
                struct tup x(b1,b2,s2x,s2y);

                temp=find_if(neighbours1.begin(), neighbours1.end(), x );

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
                    can a{pair<int,int>(s1x,s1y),0};
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
        // newboard.otherCannon.erase(dicval);
    }
    return newboard;
}

board delete_soldier(board, x, y)
{
    change_cannon()
}

board apply_moves(board,move)
{

}


minimax(board,pno,isthisme,ply)
{

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