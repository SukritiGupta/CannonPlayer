#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <unistd.h>

//changed the order of moves, changed eval

using namespace std;
int ply_MAX;
struct can
{
    pair<int,int> centre;
    int dir; //0,1,2,3

    bool operator () (const can & m) const
    {
        return (m.dir==dir && m.centre.first==centre.first && m.centre.second==centre.second);
    }
};

struct tup
{
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

//change equality by or and "if else" can be removed*****

class board
{
    public:
    vector< vector <int> > grid; //1 mysol, -1 other sol, 2 my townhall, -2 other townhall, 0 empty
    vector<pair<int,int>> mySoldier;
    // vector< pair<int,int>> myTown;
    vector<pair<int,int>> otherSoldier;
    map<pair<int, int>, vector< tup >> mycannon;  //sol to all other sols in any cannon it is in
    map<pair<int, int>, vector< tup >> otherCannon;

    vector<can> allmycan;
    vector<can> allothercan;

    //int pno;// +1 or -1
    int nummysol;
    int numothsol;

    int nummyth;
    int numothth;

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

        tup a(0,6,0,7);
        vector<tup> v={a};
        mycannon[{0,5}]=v;

        v[0].a[1]=5;
        mycannon[{0,6}]=v;

        v[0].a[3]=6;
        mycannon[{0,7}]=v;

        tup a1(2,6,2,7);
        vector<tup> v1={a1};
        mycannon[{2,5}]=v1;

        v1[0].a[1]=5;
        mycannon[{2,6}]=v1;

        v1[0].a[3]=6;
        mycannon[{2,7}]=v1;


        tup a2(4,6,4,7);
        vector<tup> v2={a2};
        mycannon[{4,5}]=v2;

        v2[0].a[1]=5;
        mycannon[{4,6}]=v2;

        v2[0].a[3]=6;
        mycannon[{4,7}]=v2;

        tup a3(6,6,6,7);
        vector<tup> v3={a3};
        mycannon[{6,5}]=v3;

        v3[0].a[1]=5;
        mycannon[{6,6}]=v3;

        v3[0].a[3]=6;
        mycannon[{6,7}]=v3;




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

    double eval(bool isThisMe, int pno)
    {
        // int ans = allmycan.size() - allothercan.size();
        int ans = 500*(nummyth - numothth) + 10*(nummysol-numothsol);
        int soldiersOpp=0, cannontot=0;

        // 5*(allmycan.size() - allothercan.size()) +

        int a=allmycan.size(),b=allothercan.size();

        for (int i = 0; i < a; ++i)
        {
            if (allmycan[i].dir!=2)
            {
                cannontot+=3;
            }
            else
                cannontot++;
        }

        for (int i = 0; i < b; ++i)
        {
            if (allothercan[i].dir!=2)
            {
                cannontot-=3;
            }
            else
                cannontot--;
        }
        ans+=cannontot;
        if(pno==1)
        {
            for (int i = 0; i < nummysol; i++)
            {
                // if (mySoldier[i].second<4)
                    soldiersOpp-=mySoldier[i].second;
            }
        
            for (int i = 0; i < numothsol; i++)
            {
                // if (otherSoldier[i].second>3)
                    soldiersOpp-=otherSoldier[i].second;
            }
        }
        else
        {
            for (int i = 0; i < nummysol; i++)
            {
                // if (mySoldier[i].second>3)
                    soldiersOpp+=mySoldier[i].second;
            }
        
            for (int i = 0; i < numothsol; i++)
            {
                // if (otherSoldier[i].second<4)
                    soldiersOpp+=otherSoldier[i].second;
            }
        }

        ans = ans + soldiersOpp;
        if (!isThisMe)
        {
            ans*=-1;
        }
        return (double)ans;
    }

};

board make_changes_sol(board newboard, int pno, int a1, int a2, int b1, int b2)
{
    // board newboard=currboard;
    if (newboard.grid[b1][b2]==(-1)*pno)
    {
        newboard.otherSoldier.erase(find(newboard.otherSoldier.begin(), newboard.otherSoldier.end(), pair<int,int>(b1,b2)));

        map<pair<int, int>, vector<tup>>::iterator   dicval=newboard.otherCannon.find(pair<int,int>(b1,b2));

        if (dicval!=newboard.otherCannon.end())
        {
            vector<tup> neighbours, neighbours1, neighbours2;
            neighbours=dicval->second;
            vector<tup>::iterator iter,temp;
            int s1x,s1y,s2x,s2y;

            for (iter = neighbours.begin(); iter < neighbours.end(); iter++)
            {
                s1x=(*iter).a[0];
                s1y=(*iter).a[1];
                s2x=(*iter).a[2];
                s2y=(*iter).a[3];

                // cerr<<s1x<<s1y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
                neighbours1=newboard.otherCannon.at(pair<int,int>(s1x,s1y));
                // cerr<<s1x<<" "<<s1y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;

                // 

                if(neighbours1.size()>1)
                {
                    // int arr[4]={b1,b2,s2x,s2y};
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
                    newboard.otherCannon[pair<int,int>(s1x,s1y)]=neighbours1;
                }
                else
                {
                    newboard.otherCannon.erase(pair<int,int>(s1x,s1y));
                }

                //repeat for s2
                // cerr<<s2x<<s2y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
                neighbours2=newboard.otherCannon.at(pair<int,int>(s2x,s2y));
                

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
                    newboard.otherCannon[pair<int,int>(s2x,s2y)]=neighbours2;
                }
                else
                {
                    newboard.otherCannon.erase(pair<int,int>(s2x,s2y));
                }

                if (b1==s1x && b1==s2x) //dir=0
                {
                    if ((b2<s1y && s1y<s2y)|| (b2>s1y && s1y>s2y))
                    {
                        can a{pair<int,int>(s1x,s1y),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b2<s2y && s2y<s1y)|| (b2>s2y && s2y>s1y))
                    {
                        can a{pair<int,int>(s2x,s2y),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1y<b2 && b2<s2y)||(s1y>b2 && b2>s2y))
                    {
                        can a{pair<int,int>(b1,b2),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }

                else if (b2==s1y && b2==s2y) //dir=2
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }

                else if ( b2-s2y == b1-s2x) //dir=1
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }
                else //dir=3
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }
            }
            newboard.otherCannon.erase(pair<int,int>(b1,b2));
            // newboard.otherCannon.erase(dicval);
        }
        newboard.numothsol-=1;
    }
    else if (newboard.grid[b1][b2]==(-2)*pno)
    {
        newboard.numothth-=1;
        //can be used in eval?? ***********************add code for udaying townhall
    }

    //***************repeat remove at [a1][a2] with mycannon and allmy can

    newboard.mySoldier.erase(find(newboard.mySoldier.begin(), newboard.mySoldier.end(), pair<int,int>(a1,a2)));

    map<pair<int, int>, vector<tup>>::iterator   dicval=newboard.mycannon.find(pair<int,int>(a1,a2));

    if (dicval!=newboard.mycannon.end())
    {
        // cerr<<"removing cannon from "<<a1<<"  "<<a2<<endl;
        vector<tup> neighbours, neighbours1, neighbours2;
        neighbours=dicval->second;
        vector<tup>::iterator iter,temp;
        int s1x,s1y,s2x,s2y;


        for (iter = neighbours.begin(); iter < neighbours.end(); iter++)
        {
            s1x=(*iter).a[0];
            s1y=(*iter).a[1];
            s2x=(*iter).a[2];
            s2y=(*iter).a[3];

            // cerr<<s1x<<s1y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            neighbours1=newboard.mycannon.at(pair<int,int>(s1x,s1y));
            

            if(neighbours1.size()>1)
            {
                // int arr[4]={b1,b2,s2x,s2y};
                struct tup x(a1,a2,s2x,s2y);

                temp=find_if(neighbours1.begin(), neighbours1.end(), x );

                if (temp!=neighbours1.end())
                {
                    neighbours1.erase(temp);
                }
                else
                {
                    x.a[0]=s2x;
                    x.a[1]=s2y;
                    x.a[2]=a1;
                    x.a[3]=a2;
                    neighbours1.erase( find_if(neighbours1.begin(), neighbours1.end(), x ) );   
                }
                newboard.mycannon[{s1x,s1y}]=neighbours1;
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
                struct tup x(a1,a2,s1x,s1y);

                temp=find_if(neighbours2.begin(), neighbours2.end(), x );

                if (temp!=neighbours2.end())
                {
                    neighbours2.erase(temp);
                }
                else
                {
                    x.a[0]=s1x;
                    x.a[1]=s1y;
                    x.a[2]=a1;
                    x.a[3]=a2;
                    neighbours2.erase( find_if(neighbours2.begin(), neighbours2.end(), x ) );   
                }
                newboard.mycannon[{s2x,s2y}]=neighbours2;
            }
            else
            {
                // cerr<<"******************"<<endl;
                newboard.mycannon.erase(pair<int,int>(s2x,s2y));
            }

            // cerr<<"outIF"<<endl;

            if (a1==s1x && a1==s2x) //dir=0
            {
                if ((a2<s1y && s1y<s2y)|| (a2>s1y && s1y>s2y))
                {
                    // cerr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
                    can a{pair<int,int>(s1x,s1y),0};
                    // cerr<<"here"<<endl;
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                    // cerr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
                }
                else if ((a2<s2y && s2y<s1y)|| (a2>s2y && s2y>s1y))
                {
                    can a{pair<int,int>(s2x,s2y),0};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1y<a2 && a2<s2y)||(s1y>a2 && a2>s2y))
                {
                    // cerr<<"******************"<<endl;
                    can a{pair<int,int>(a1,a2),0};
                    // cerr<<"I am here"<<endl;
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                    // cerr<<"&&&&&&&&&&&&&&&&&&&&&"<<endl;
                }
            }

            else if (a2==s1y && a2==s2y) //dir=2
            {
                if ((a1<s1x && s1x<s2x)|| (a1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((a1<s2x && s2x<s1x)|| (a1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<a1 && a1<s2x)||(s1x>a1 && a1>s2x))
                {
                    can a{pair<int,int>(a1,a2),2};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }

            else if ( a2-s2y == a1-s2x) //dir=3
            {
                if ((a1<s1x && s1x<s2x)|| (a1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((a1<s2x && s2x<s1x)|| (a1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<a1 && a1<s2x)||(s1x>a1 && a1>s2x))
                {
                    can a{pair<int,int>(a1,a2),3};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }
            else //dir=1
            {
                if ((a1<s1x && s1x<s2x)|| (a1>s1x && s1x>s2x))
                {
                    can a{pair<int,int>(s1x,s1y),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((a1<s2x && s2x<s1x)|| (a1>s2x && s2x>s1x))
                {
                    can a{pair<int,int>(s2x,s2y),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
                else if ((s1x<a1 && a1<s2x)||(s1x>a1 && a1>s2x))
                {
                    can a{pair<int,int>(a1,a2),1};
                    newboard.allmycan.erase(    find_if(newboard.allmycan.begin(), newboard.allmycan.end(), a)   );
                }
            }

        }

        newboard.mycannon.erase(pair<int,int>(a1,a2));//**********************************************
        // newboard.mycannon.erase(dicval);
    }
    // cerr<<"09"<<endl;
    newboard.grid[b1][b2]=pno;
    // cerr<<"10"<<endl;
    newboard.grid[a1][a2]=0;
    // cerr<<"11"<<endl;
    // cerr<<newboard.nummysol<<endl;
    // cerr<<"13"<<endl;
    newboard.mySoldier.push_back(pair<int,int>(b1,b2));
    // cerr<<"12"<<endl;

    //Updating cannons
    map<pair<int, int>, vector<tup>>::iterator   exist;

    vector<tup> finalpush,y;
    
    // cerr<<"13"<<endl;
    // cerr<<"Starting new cannon now that I have moved "<<b1<<"  "<<b2<<endl;
    if (b2+2<=7 && newboard.grid[b1][b2+1]==pno && newboard.grid[b1][b2+2]==pno) //pno ko multiply by -1 karna hai kya*****
    {
        can a{pair<int,int>(b1,b2+1),0};
        newboard.allmycan.push_back(a);

        struct tup x(b1, b2+1, b1, b2+2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1,b2+1));

        // struct tup x {{b1, b2, b1, b2+2}};
        x.a[1]=b2;
        
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2+1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1,b2+2));

        // struct tup x {{b1, b2+1, b1, b2}};
        x.a[1]=b2+1;
        x.a[3]=b2;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2+2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2+2));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2+2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2+2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b2+1<=7 && b2-1>=0 && newboard.grid[b1][b2+1]==pno && newboard.grid[b1][b2-1]==pno) //pno*****
    {
        can a{pair<int,int>(b1,b2),0};
        newboard.allmycan.push_back(a);

        struct tup x(b1, b2+1, b1, b2-1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1,b2+1));

        // struct tup x {{b1, b2, b1, b2-1}};
        x.a[1]=b2;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2+1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1,b2-1));

        // struct tup x {{b1, b2+1, b1, b2}};
        x.a[1]=b2+1;
        x.a[3]=b2;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2-1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b2-2>=0 && b2-1>=0 && newboard.grid[b1][b2-2]==pno && newboard.grid[b1][b2-1]==pno) //pno*****
    {
        can a{pair<int,int>(b1,b2-1),0};
        newboard.allmycan.push_back(a);

        struct tup x(b1, b2-2, b1, b2-1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1,b2-2));

        // struct tup x {{b1, b2, b1, b2-1}};
        x.a[1]=b2;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2-2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2-2));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2-2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2-2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1,b2-1));

        // struct tup x {{b1, b2-2, b1, b2}};
        x.a[1]=b2-2;
        x.a[3]=b2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1,b2-1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+2<=7 && newboard.grid[b1+2][b2]==pno && newboard.grid[b1+1][b2]==pno) //pno*****
    {
        can a{pair<int,int>(b1+1,b2),2};
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2, b1+2, b2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2));

        // struct tup x {{b1, b2, b1+2, b2}};
        x.a[0]=b1;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1+2,b2));

        // struct tup x {{b1, b2, b1+1, b2}};
        x.a[2]=b1+1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+2<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+2,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1+2,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+2,b2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+1<=7 && b1-1>=0 && newboard.grid[b1-1][b2]==pno && newboard.grid[b1+1][b2]==pno) //pno*****
    {
        can a{pair<int,int>(b1,b2),2};
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2, b1-1, b2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2));

        // struct tup x {{b1, b2, b1-1, b2}};
        x.a[0]=b1;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2));

        // struct tup x {{b1, b2, b1+1, b2}};
        x.a[2]=b1+1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1-2>=0 && b1-1>=0 && newboard.grid[b1-1][b2]==pno && newboard.grid[b1-2][b2]==pno) //pno*****
    {
        can a{pair<int,int>(b1-1,b2),2};
        newboard.allmycan.push_back(a);

        struct tup x (b1-2, b2, b1-1, b2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1-2,b2));

        // struct tup x {{b1, b2, b1-1, b2}};
        x.a[0]=b1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-2<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-2,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1-2,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-2,b2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2));

        // struct tup x {{b1, b2, b1-2, b2}};
        x.a[2]=b1-2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-1<<b2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+2<=7 && b2+2<=7 && newboard.grid[b1+2][b2+2]==pno && newboard.grid[b1+1][b2+1]==pno) //pno*****
    {
        can a{pair<int,int>(b1+1,b2+1),3};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2+1, b1+2, b2+2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2+1));

        // struct tup x {{b1, b2, b1+2, b2+2}};
        x.a[0]=b1;
        x.a[1]=b2;
        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2+1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1+2,b2+2));

        // struct tup x {{b1, b2, b1+1, b2+1}};
        x.a[2]=b1+1;
        x.a[3]=b2+1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+2<<b2+2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+2,b2+2));
            
            y.push_back(x);

            newboard.mycannon[{b1+2,b2+2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+2,b2+2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+1<=7 && b2+1<=7 && b1-1>=0 && b2-1>=0 && newboard.grid[b1-1][b2-1]==pno && newboard.grid[b1+1][b2+1]==pno) //pno*****
    {
        can a{pair<int,int>(b1,b2),3};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2+1, b1-1, b2-1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2+1));

        // struct tup x {{b1, b2, b1-1, b2-1}};
        x.a[0]=b1;
        x.a[1]=b2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2+1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2-1));

        // struct tup x {{b1, b2, b1+1, b2+1}};
        x.a[2]=b1+1;
        x.a[3]=b2+1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2-1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if ( b1-2>=0 && b2-2>=0 && newboard.grid[b1-1][b2-1]==pno && newboard.grid[b1-2][b2-2]==pno) //pno*****
    {
        can a{pair<int,int>(b1-1,b2-1),3};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1-2, b2-2, b1-1, b2-1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1-2,b2-2));

        // struct tup x {{b1, b2, b1-1, b2-1}};
        x.a[0]=b1;
        x.a[1]=b2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-2<<b2-2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-2,b2-2));
            
            y.push_back(x);

            newboard.mycannon[{b1-2,b2-2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-2,b2-2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2-1));

        // struct tup x {{b1, b2, b1-2, b2-2}};
        x.a[2]=b1-2;
        x.a[3]=b2-2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2-1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+2<=7 && b2-2>=0 && newboard.grid[b1+2][b2-2]==pno && newboard.grid[b1+1][b2-1]==pno) //pno*****
    {
        can a{pair<int,int>(b1+1,b2-1),1};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2-1, b1+2, b2-2);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2-1));

        // struct tup x {{b1, b2, b1+2, b2-2}};
        x.a[0]=b1;
        x.a[1]=b2;


        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2-1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1+2,b2-2));

        // struct tup x {{b1, b2, b1+1, b2-1}};
        x.a[2]=b1+1;
        x.a[3]=b2-1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+2<<b2-2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+2,b2-2));
            
            y.push_back(x);

            newboard.mycannon[{b1+2,b2-2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+2,b2-2}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (b1+1<=7 && b2-1>=0 && b1-1>=0 && b2+1<=7 && newboard.grid[b1+1][b2-1]==pno && newboard.grid[b1-1][b2+1]==pno) //pno*****
    {
        can a{pair<int,int>(b1,b2),1};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1+1, b2-1, b1-1, b2+1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1+1,b2-1));

        // struct tup x {{b1, b2, b1-1, b2+1}};
        x.a[0]=b1;
        x.a[1]=b2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1+1<<b2-1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1+1,b2-1));
            
            y.push_back(x);

            newboard.mycannon[{b1+1,b2-1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1+1,b2-1}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2+1));

        // struct tup x {{b1, b2, b1+1, b2-1}};
        x.a[2]=b1+1;
        x.a[3]=b2-1;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2+1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if ( b1-2>=0 && b2+2<=7 && newboard.grid[b1-1][b2+1]==pno && newboard.grid[b1-2][b2+2]==pno) //pno*****
    {
        can a{pair<int,int>(b1-1,b2+1),1};  //need to check for direction****************
        newboard.allmycan.push_back(a);

        struct tup x (b1-2, b2+2, b1-1, b2+1);
        finalpush.push_back(x);

        exist=newboard.mycannon.find(pair<int,int>(b1-2,b2+2));

        // struct tup x {{b1, b2, b1-1, b2+1}};
        x.a[0]=b1;
        x.a[1]=b2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-2<<b2+2<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-2,b2+2));
            
            y.push_back(x);

            newboard.mycannon[{b1-2,b2+2}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-2,b2+2}] = yy;
        }

        exist=newboard.mycannon.find(pair<int,int>(b1-1,b2+1));

        // struct tup x {{b1, b2, b1-2, b2+2}};
        x.a[2]=b1-2;
        x.a[3]=b2+2;

        if (exist != newboard.mycannon.end())
        {
            // cerr<<b1-1<<b2+1<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
            y = newboard.mycannon.at(pair<int,int>(b1-1,b2+1));
            
            y.push_back(x);

            newboard.mycannon[{b1-1,b2+1}] = y; //check correctness
        }
        else
        {
            vector<tup> yy{x};
            newboard.mycannon[{b1-1,b2+1}] = yy;
        }


        //newboard.mycannon.(); //b1,b2
        //insert s1,s2 - check if they already existed -> append;
    }

    if (finalpush.size()>0)
    {
        newboard.mycannon[{b1,b2}] = finalpush;
    }

    // cerr<<"Looking for new cannons done when move to "<<b1<<"  "<<b2<<endl;
    //7 more times for 7 directional cannons

    return newboard;

}

board make_changes_cannon(board newboard, int pno, int a1, int a2, int b1, int b2)
{
    //same as deletion of a soldier as above make b1,b2 as 0!!
    if (newboard.grid[b1][b2]==-1*pno)
    {
        newboard.otherSoldier.erase(find(newboard.otherSoldier.begin(), newboard.otherSoldier.end(), pair<int,int>(b1,b2)));

        map<pair<int, int>, vector<tup>>::iterator   dicval=newboard.otherCannon.find(pair<int,int>(b1,b2));

        if (dicval!=newboard.otherCannon.end())
        {
            vector<tup> neighbours, neighbours1, neighbours2;
            neighbours=dicval->second;
            vector<tup>::iterator iter,temp;
            int s1x,s1y,s2x,s2y;

            for (iter = neighbours.begin(); iter < neighbours.end(); iter++)
            {
                s1x=(*iter).a[0];
                s1y=(*iter).a[1];
                s2x=(*iter).a[2];
                s2y=(*iter).a[3];

                // cerr<<s1x<<s1y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
                neighbours1=newboard.otherCannon.at(pair<int,int>(s1x,s1y));
                

                if(neighbours1.size()>1)
                {
                    // int arr[4]={b1,b2,s2x,s2y};
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
                    newboard.otherCannon[pair<int,int>(s1x,s1y)]=neighbours1;
                }
                else
                {
                    newboard.otherCannon.erase(pair<int,int>(s1x,s1y));
                }

                //repeat for s2
                // cerr<<s2x<<s2y<<"    "<<"b1 :"<<b1<<"b2 :"<<b2<<endl;
                neighbours2=newboard.otherCannon.at(pair<int,int>(s2x,s2y));
                

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
                    newboard.otherCannon[pair<int,int>(s2x,s2y)]=neighbours2;
                }
                else
                {
                    newboard.otherCannon.erase(pair<int,int>(s2x,s2y));
                }

                if (b1==s1x && b1==s2x) //dir=0
                {
                    if ((b2<s1y && s1y<s2y)|| (b2>s1y && s1y>s2y))
                    {
                        can a{pair<int,int>(s1x,s1y),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b2<s2y && s2y<s1y)|| (b2>s2y && s2y>s1y))
                    {
                        can a{pair<int,int>(s2x,s2y),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1y<b2 && b2<s2y)||(s1y>b2 && b2>s2y))
                    {
                        can a{pair<int,int>(b1,b2),0};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }

                else if (b2==s1y && b2==s2y) //dir=2
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),2};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }

                else if ( b2-s2y == b1-s2x) //dir=1
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),3};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }
                else //dir=3
                {
                    if ((b1<s1x && s1x<s2x)|| (b1>s1x && s1x>s2x))
                    {
                        can a{pair<int,int>(s1x,s1y),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((b1<s2x && s2x<s1x)|| (b1>s2x && s2x>s1x))
                    {
                        can a{pair<int,int>(s2x,s2y),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                    else if ((s1x<b1 && b1<s2x)||(s1x>b1 && b1>s2x))
                    {
                        can a{pair<int,int>(b1,b2),1};
                        newboard.allothercan.erase(    find_if(newboard.allothercan.begin(), newboard.allothercan.end(), a)   );
                    }
                }
            }
            newboard.otherCannon.erase(pair<int,int>(b1,b2));
            // newboard.otherCannon.erase(dicval);
        }
        newboard.numothsol-=1;
    }
    else if (newboard.grid[b1][b2]==(-2)*pno)
    {
        newboard.numothth-=1;
        //can be used in eval?? ***********************add code for udaying townhall
    }


    newboard.grid[b1][b2]=0;
    return newboard;
}

//if isThisMe is false it is other player, it will try to minimize my score

double next_ply_move_max(board currboard, int ply, string *movefinal, int pno, bool isThisMe, double alpha, double beta)
{
    
    if (ply!=ply_MAX)
    {
        pno = pno*(-1);
        vector<pair<int,int> > temp1 = currboard.mySoldier;
        currboard.mySoldier = currboard.otherSoldier;
        currboard.otherSoldier = temp1;

        map<pair<int, int>, vector< tup >> temp2 = currboard.mycannon;
        currboard.mycannon = currboard.otherCannon;
        currboard.otherCannon = temp2;

        vector<can> temp3 = currboard.allmycan;
        currboard.allmycan = currboard.allothercan;
        currboard.allothercan = temp3;

        int temp4 = currboard.nummysol;
        currboard.nummysol = currboard.numothsol;
        currboard.numothsol = temp4;

        int temp5 = currboard.nummyth;
        currboard.nummyth=currboard.numothth;
        currboard.numothth=temp5;
    }

    //int max=0,x,y;
    //int pno=currboard.pno;

    string move="";
    // double val, minval=1000000, maxval=-10000000; //*******************8relate it to alpha beta
    double val, minval=beta, maxval=alpha; //*******************8relate it to alpha beta
    board newboard;
    int x,y;
    bool flag=false;
    // *********** saveboard; //globalboard

    if (currboard.nummysol==0)
    {
        flag==true;
        double ttt=currboard.eval(isThisMe,pno);
        minval=ttt;
        maxval=ttt;
        
    }

    vector<can>::iterator ptr; 

    if(!flag)
    {
        for (int i = 0; i < currboard.nummysol; ++i)
        {
            x=currboard.mySoldier[i].first;
            y=currboard.mySoldier[i].second;
            
            //Main 3 moves always allowed
            //MOves order change
            if (y!=0 && pno==1)
            {
                if(currboard.grid[x][y-1]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x,y-1);

                    if (ply == 0)
                    {
                        // cerr<<"eval called"<<endl;
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y-1);
                            // cerr<<"Move made"<<endl;
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y-1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }

                        }
                    }
                }
                if (x!=0 && currboard.grid[x-1][y-1]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x-1,y-1);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y-1);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y-1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
                if (x!=7 && currboard.grid[x+1][y-1]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x+1,y-1);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y-1);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val; 
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y-1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
            }
            if (y!=7 && pno==-1)
            {
                if (currboard.grid[x][y+1]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x,y+1);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y+1);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y+1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
                if (x!=0 && currboard.grid[x-1][y+1]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x-1,y+1);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y+1);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y+1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
                if (x!=7 && currboard.grid[x+1][y+1]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x+1,y+1);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y+1);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;  
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                      
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y+1);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
            }


            //side moves allowed when adjacent soldiers
            if (x!=0 && (currboard.grid[x-1][y]==pno*(-1) || currboard.grid[x-1][y]==pno*(-2)))
            {
                board newboard=make_changes_sol(currboard, pno, x,y,x-1,y);  

                if (ply == 0)
                {
                    val = newboard.eval(isThisMe,pno);
                    if (val<minval && not isThisMe)
                    {
                        minval = val;                       
                    }
                    else if(val> maxval && isThisMe)
                    {
                        maxval = val;
                        move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y);
                    }
                }
                else
                {
                    string strtemp;
                    val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                    if (val<minval && not isThisMe)
                    {
                        minval = val; 
                        if(minval<=alpha)
                        {
                            flag=true;
                            break;
                        }                       
                    }
                    else if(val> maxval && isThisMe)
                    {
                        maxval = val;
                        move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-1) + " " + to_string(y);
                        if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                    }
                } 
            }
            if (x!=7 && (currboard.grid[x+1][y]==pno*(-1) || currboard.grid[x+1][y]==pno*(-2) ))
            {
                board newboard=make_changes_sol(currboard, pno, x,y,x+1,y);

                if (ply == 0)
                {
                    val = newboard.eval(isThisMe,pno);
                    if (val<minval && not isThisMe)
                    {
                        minval = val;                       
                    }
                    else if(val> maxval && isThisMe)
                    {
                        maxval = val;
                        move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y);
                    }
                }
                else
                {
                    string strtemp;
                    val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                    if (val<minval && not isThisMe)
                    {
                        minval = val;
                        if(minval<=alpha)
                        {
                            flag=true;
                            break;
                        }                        
                    }
                    else if(val> maxval && isThisMe)
                    {
                        maxval = val;
                        move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+1) + " " + to_string(y);
                        if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                    }
                }
            }


            if (pno==1 && y+2<=7 && (( (x!=0) && currboard.grid[x-1][y]==-1 )||  ((x!=7) && currboard.grid[x+1][y]==-1 )  ||
                        ((x!=0 && y!=0) && currboard.grid[x-1][y-1]==-1 )||
                        ((y!=0) && currboard.grid[x][y-1]==-1 )||
                        ((x!=7 && y!=0) && currboard.grid[x+1][y-1]==-1 )))
            {
                if (currboard.grid[x][y+2]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x,y+2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val>maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y+2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
                if (x-2>=0 && currboard.grid[x-2][y+2]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x-2,y+2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y+2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }   
                }
                if (x+2<=7 && currboard.grid[x+2][y+2]<=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x+2,y+2); 

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y+2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }  
                }
            }



            if (pno==-1 && y-2>=0 && (( (x!=0) && currboard.grid[x-1][y]==1 )||  ((x!=7) && currboard.grid[x+1][y]==1 )  ||
                        ((x!=7 && y!=7) && currboard.grid[x+1][y+1]==1 )||
                        ((y!=7) && currboard.grid[x][y+1]==1 )||
                        ((x!=0 && y!=7) && currboard.grid[x-1][y+1]==1 )))
            {
                if (currboard.grid[x][y-2]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x,y-2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x) + " " + to_string(y-2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }
                }
                if (x-2>=0 && currboard.grid[x-2][y-2]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x-2,y-2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y-2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }   
                }
                if (x+2<=7 && currboard.grid[x+2][y-2]>=0)
                {
                    board newboard=make_changes_sol(currboard, pno, x,y,x+2,y-2); 

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<=alpha)
                            {
                                flag=true;
                                break;
                            }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y-2);
                            if (maxval>=beta)
                            {
                                flag=true;
                                break;
                            }
                        }
                    }  
                }
            }
        
        }
    }

    if(!flag)
    {
        for (ptr = currboard.allmycan.begin(); ptr < currboard.allmycan.end(); ptr++)
        {
            can cannon=*ptr;
            int x=cannon.centre.first;
            int y=cannon.centre.second;
            if (cannon.dir==0)
            {
                if (y+2<=7 && currboard.grid[x][y+2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x, y-1, x, y+2);//move soldier

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y-1) + " M " + to_string(x) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y-1) + " M " + to_string(x) + " " + to_string(y+2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (y+3<=7 && currboard.grid[x][y+3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x, y+3);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y+3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;   
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                     
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y+3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (y+4<=7 && currboard.grid[x][y+4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x, y+4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y+4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y+4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (y-2>=0 && currboard.grid[x][y-2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x, y+1, x, y-2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y+1) + " M " + to_string(x) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;     
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                   
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x) + " " + to_string(y+1) + " M " + to_string(x) + " " + to_string(y-2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (y-3>=0 && currboard.grid[x][y-3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x, y-3);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y-3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y-3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (y-4>=0 && currboard.grid[x][y-4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x, y-4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y-4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x) + " " + to_string(y-4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }            
            }

            if (cannon.dir==2)
            {
                if (x+2<=7 && currboard.grid[x+2][y]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x-1, y, x+2, y);//move soldier

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y) + " M " + to_string(x+2) + " " + to_string(y);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x+3<=7 && currboard.grid[x+3][y]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+3, y);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;  
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                      
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x+4<=7 && currboard.grid[x+4][y]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+4, y);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (x-2>=0 && currboard.grid[x-2][y]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x+1, y, x-2, y);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y) + " M " + to_string(x-2) + " " + to_string(y);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x-3>=0 && currboard.grid[x-3][y]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-3, y);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x-4>=0 && currboard.grid[x-4][y]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-4, y);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;       
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                 
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }            
            }

            if (cannon.dir==3)
            {
                if (x+2<=7 && y+2<=7 && currboard.grid[x+2][y+2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x-1, y-1, x+2, y+2);//move soldier

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y-1) + " M " + to_string(x+2) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;  
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                      
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y-1) + " M " + to_string(x+2) + " " + to_string(y+2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x+3<=7 && y+3<=7 && currboard.grid[x+3][y+3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+3, y+3);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y+3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y+3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x+4<=7 && y+4<=7 && currboard.grid[x+4][y+4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+4, y+4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y+4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y+4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (x-2>=0 && y-2>=0 && currboard.grid[x-2][y-2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x+1, y+1, x-2, y-2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y+1) + " M " + to_string(x-2) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val; 
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y+1) + " M " + to_string(x-2) + " " + to_string(y-2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x-3>=0 && y-3>=0 && currboard.grid[x-3][y-3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-3, y-3);
                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y-3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y-3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x-4>=0 && y-4>=0 && currboard.grid[x-4][y-4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-4, y-4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y-4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y-4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }            
            }

            if (cannon.dir==1)
            {
                if (x+2<=7 && y-2>=0 && currboard.grid[x+2][y-2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x-1, y+1, x+2, y-2);//move soldier

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y+1) + " M " + to_string(x+2) + " " + to_string(y-2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                } 
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x-1) + " " + to_string(y+1) + " M " + to_string(x+2) + " " + to_string(y-2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x+3<=7 && y-3>=0 && currboard.grid[x+3][y-3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+3, y-3);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y-3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+3) + " " + to_string(y-3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x+4<=7 && y-4>=0 && currboard.grid[x+4][y-4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x+4, y-4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y-4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x+4) + " " + to_string(y-4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (x-2>=0 && y+2<=7 && currboard.grid[x-2][y+2]==0 )
                {
                    board newboard = make_changes_sol(currboard, pno, x+1, y-1, x-2, y+2);

                    if (ply == 0)
                    {
                        val = newboard.eval(isThisMe,pno);
                        if (val<minval && not isThisMe)
                        {
                            minval = val;                       
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y-1) + " M " + to_string(x-2) + " " + to_string(y+2);
                        }
                    }
                    else
                    {
                        string strtemp;
                        val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                        if (val<minval && not isThisMe)
                        {
                            minval = val;
                            if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                        }
                        else if(val> maxval && isThisMe)
                        {
                            maxval = val;
                            move = "S " + to_string(x+1) + " " + to_string(y-1) + " M " + to_string(x-2) + " " + to_string(y+2);
                            if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                        }
                    }

                    if (x-3>=0 && y+3<=7 && currboard.grid[x-3][y+3]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-3, y+3);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y+3);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val;
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                        
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-3) + " " + to_string(y+3);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                    if (x-4>=0 && y+4<=7 && currboard.grid[x-4][y+4]<=0)//blast
                    {
                        board newboard = make_changes_cannon(currboard, pno, x, y, x-4, y+4);

                        if (ply == 0)
                        {
                            val = newboard.eval(isThisMe,pno);
                            if (val<minval && not isThisMe)
                            {
                                minval = val;                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y+4);
                            }
                        }
                        else
                        {
                            string strtemp;
                            val = next_ply_move_max(newboard, ply-1, &strtemp, pno, not isThisMe, maxval,minval);

                            if (val<minval && not isThisMe)
                            {
                                minval = val; 
                                if(minval<alpha)
                                {
                                    flag=true;
                                    break;
                                }                       
                            }
                            else if(val> maxval && isThisMe)
                            {
                                maxval = val;
                                move = "S " + to_string(x) + " " + to_string(y) + " B " + to_string(x-4) + " " + to_string(y+4);
                                if (maxval>beta)
                                {
                                    flag=true;
                                    break;
                                }
                            }
                        }
                    }
                }            
            }

        //     //dir=1,2,3

        }
    }

    
        //not my piece ****************
    // } board make_changes_sol(board currboard, int pno, int a1, int a2, int b1, int b2)

    // if (move=="")
    // {
    //     // if (!isThisMe)
    //     // {
    //         //othsoldier==0 is good but othsoldier==1 is very bad
    //     // }
    // }

    *movefinal=move;
    if (isThisMe)
    {
        return maxval;
    }
    return minval;
}


board execute_move(board currboard, string move, int pno)
{
    // cerr<<"move executing *********************************************************************************************************************************************** "<<move<<endl;
    if (move[6]=='M')
    {
        currboard=make_changes_sol(currboard,pno,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'));
    }
    else //cannon blast
    {
        currboard=make_changes_cannon(currboard,pno,(move[2]-'0'),(move[4]-'0'),(move[8]-'0'),(move[10]-'0'));
    }
    return currboard;
    
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

int main()
{
    string line;
    int pno, N,M, timeq;
    cin>>pno>>N>>M>>timeq;
    getline(cin,line);

    if(pno==2)
        ply_MAX=2;
    else
        ply_MAX=3;

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
    bool plyincat15=false,plyincat8=false, plyincat4=false;

    while(true)
    {
        if (curr.nummysol+curr.numothsol<15 && !plyincat15)
        {
            cerr<<"ply_MAX now +1********************  15"<<endl;
            ply_MAX+=2;
            plyincat15=true;
        }

        if (curr.nummysol+curr.numothsol<8 && !plyincat8)
        {
            cerr<<"ply_MAX now +1********************  8"<<endl;
            ply_MAX+=2;
            plyincat8=true;
        }

        if (curr.nummysol+curr.numothsol<4 && !plyincat4)
        {
            cerr<<"ply_MAX now +1********************  4"<<endl;
            ply_MAX+=2;
            plyincat4=true;
        }
        print(curr);
        // cerr<<"move "<<move<<endl;
        // usleep(30);
        ttt=next_ply_move_max(curr,ply_MAX, &move ,pno,true, maxval,minval);
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
