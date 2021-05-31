
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct NodeInfo
{
    int prev;
    float distance;
    NodeInfo(int p, float d):prev(p), distance(d) {};
};

ostream& operator << (ostream &os, const NodeInfo &node)
{
    if(node.prev == -1)
    {
        os<<setw(2)<<'-';
    }
    else
    {
        os<<setw(2)<<node.prev;
    }
    os<<" "<<setw(3)<<node.distance;
    return os;
}

class Graph
{
private:

    struct edge
    {
        int startVert;
        int endVert;
        float weight;

        edge(int s, int e, float w): startVert(s), endVert(e), weight(w) {};
    };

    vector<edge> Edges;
    int Vertesis;

public:
    Graph() {};
    ~Graph() {};

    void addEdge(int s, int e, float w)
    {
        Edges.push_back(edge(s,e,w));
    };

    void setVertesis(int v)
    {
        Vertesis = v;
    };

    bool BF(int v=0)
    {
        //inicjalizacja
        vector<NodeInfo> Nodes(Vertesis,NodeInfo(-1,INT_MAX));
        Nodes[v].distance = 0;

        bool bChande = false;

        for (int i = 0; i < Vertesis-1; i++)
        {
            //relaksacja
            for (int e = 0; e < Edges.size(); e++)
            {
                int u = Edges[e].startVert;
                int v = Edges[e].endVert;
                int w = Edges[e].weight;
                if (Nodes[u].distance != INT_MAX && Nodes[u].distance + w < Nodes[v].distance)
                {
                    Nodes[v].distance = Nodes[u].distance + w;
                    Nodes[v].prev = u;
                    bChande = true;
                }
            }
            if(!bChande)
            {
                break;
            }
        }

        //sprawdzanie czy ujemny cykl istnieje
        for (int e = 0; e < Edges.size(); e++)
        {
            int u = Edges[e].startVert;
            int v = Edges[e].endVert;
            int w = Edges[e].weight;
            if (Nodes[u].distance != INT_MAX && Nodes[u].distance + w < Nodes[v].distance)
            {
                cout << "Negative Cycle found!" << endl;
                return false;
            }
        }

        //wypisanie rezultatu
        cout<<"Bellman-Ford:\n";
        for (int i = 0; i < Nodes.size(); i++)
        {
            cout<<i<<": "<<Nodes[i]<<endl;
        }
        
        return true;
    };
};

int main(int argc, char *argv[])
{
    Graph G;
    if(argc>1)
    {
        ifstream file(argv[1]);
        if (file.is_open())
        {
            string str;
            int j=0,maxV=-1;

            //odczytujemy plik
            while(getline(file,str))
            {
                istringstream ss(str);
                int v;
                float w;

                //odczytujemy krawędzie
                while(ss>>v>>w) 
                {
                    G.addEdge(j,v,w);
                    maxV = max(maxV, v);
                }
                j++;
            }
            G.setVertesis(max(j,maxV));
        }
        else
        {
            cout<<"Nie udalo sie otworzyc pliku";
            return 0;
        }
        file.close();
    }
    else
    {
        cout<<"Nie podano pliku"<<endl;
        return 0;
    }

    G.BF();
    return 0;
}