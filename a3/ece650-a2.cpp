

// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>


#define UINT_MAX_ 4294967295

class Graph
{
    unsigned N; // number of vertices
    std::vector<unsigned*> edges;
    std::vector<std::vector<unsigned> > adj;

    bool ready_for_setEdge;
    bool ready_for_bfs;

public:

    bool is_ready_for_setEdge(){
        return ready_for_setEdge;
    }

    bool is_ready_for_bfs(){
        return ready_for_bfs;
    }

    void set_to_false(){
        ready_for_setEdge = false;
        ready_for_bfs = false;
    }

    void setN(unsigned newN){
        delete_graph();
        N = newN;
        ready_for_setEdge = true;
    }

    bool trySetEdges(std::string newEdgesString){
        delete_graph();
        unsigned start_pos = 0, end_pos = 0;
        newEdgesString = newEdgesString.substr(1, newEdgesString.size() - 2);
        // std::cout << newEdgesString<<std::endl;
        while(newEdgesString[end_pos+1]){
            start_pos = newEdgesString.find('<', end_pos);
            end_pos = newEdgesString.find('>', start_pos);

            unsigned* edge = parse_Edge(newEdgesString.substr(start_pos+1, end_pos-start_pos));

            if(!isValidNode(edge[0])){
                std::cerr << "Error: Node "<< std::to_string(edge[0]) <<" does not exist!\n";
                ready_for_setEdge = false;
                ready_for_bfs = false;
                delete_graph();
                return false;
            }
            if(!isValidNode(edge[1])){
                std::cerr << "Error: Node "<< std::to_string(edge[1]) <<" does not exist!\n";
                ready_for_setEdge = false;
                ready_for_bfs = false;
                delete_graph();
                return false;
            }

            edges.push_back(edge);
        }
        compute_adj();
        ready_for_bfs = true;
        return true;
    }

    void print_graph(){
        std::cout << N << std::endl;
        for(unsigned i=0; i < edges.size(); i++){
            std::cout <<'<'<< edges[i][0] << ',' << edges[i][1] << ">,";
        }
        std::cout << std::endl;
        for(unsigned i = 0; i<N; i++){
            std::cout << i+1 << ": ";
            for(unsigned j=0; j<adj[i].size(); j++){
                std::cout << adj[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }


    std::string bfs(unsigned source, unsigned dest){
        std::vector<unsigned> pi(this->N, 0); // int may cause a problem in very large graphs
        std::vector<unsigned> d(this->N, UINT_MAX_);
        std::vector<char> color(this->N, 'w'); //w: white, g: gray, b: black

        std::vector<unsigned> Q;

        color[source-1] = 'g';
        d[source-1] = 0;
        pi[source-1] = 0;

        Q.push_back(source);
        while(!Q.empty()){
            unsigned u = Q.front();
            Q.erase(Q.begin());
            for(unsigned i=0; i<adj[u-1].size(); i++){
                unsigned v = adj[u-1][i];
                if(color[v-1] == 'w'){
                    color[v-1] = 'g';
                    d[v-1] = d[u-1] + 1;
                    pi[v-1] = u;
                    Q.push_back(v);
                }
            }
            color[u-1] = 'b';
        }

        std::string pathStr="";
        while(dest != source){
            if(pi[dest-1] == 0){
                pi.clear();
                d.clear();
                color.clear();
                return "Error: Path does not exist!";
            }
            pathStr = std::to_string(dest) + "-" + pathStr;
            dest = pi[dest-1];
        }
        pathStr = std::to_string(dest) + "-" + pathStr;

        pi.clear();
        d.clear();
        color.clear();

        return pathStr.substr(0, pathStr.size()-1);

    }

    bool isValidNode(unsigned node){
        return node <= N && node > 0;
    }

    ~Graph(){
        delete_graph();
    }

private:
    void compute_adj(){
        for (unsigned i = 1; i <= N; i++){
            std::vector<unsigned> nodeAdj;

            for (unsigned j=0; j < edges.size(); j++){
                if(edges[j][0] == i){
                    nodeAdj.push_back(edges[j][1]);
                }
                else if(edges[j][1] == i){
                    nodeAdj.push_back(edges[j][0]);
                }
            }

            adj.push_back(nodeAdj);
        }
    }

    unsigned* parse_Edge(std::string str){
        unsigned* edge = new unsigned[2];
        unsigned commaPos = str.find(',');

        std::istringstream firstNumberStream(str.substr(0, commaPos));
        firstNumberStream >> edge[0];
        // std::cout << edge[0] << ' ';

        std::istringstream secondNumberStream(str.substr(commaPos+1));
        secondNumberStream >> edge[1];
        // std::cout << edge[1] << std::endl;

        return edge;
    }

    void delete_graph(){
        for(unsigned i=0; i < edges.size(); i++){
            delete edges[i];
        }
        edges.clear();
        // delete edge;

        for(unsigned i = 0; i<adj.size(); i++){
            adj[i].clear();
            // delete adj[i];
        }
        adj.clear();

        ready_for_setEdge = false;
        ready_for_bfs = false;
    }
};


int main(int argc, char** argv) {

    Graph g;
    unsigned N, source, destination; // number of vertices
    std::string edge_vector;

    // read from stdin until EOF
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        // std::cout << line << std::endl;

        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);

        while(!input.eof()){
            char command;
            input >> command;
            if (input.fail()) {
                // std::cerr << "Error: parsing the command's character: " << command << '!' << std::endl;
                break;
            }

            if (command == 'V'){ //////////////////////////////////////////////////////////////
                input >> N;
                std::cout << line << std::endl;
                if (input.fail()) {
                    std::cerr << "Error: parsing the number of vertices!\n";
                    break;
                }
                if (N <= 1) {
                    std::cerr << "Error: the number of vertices must be greater than one!\n";
                    break;
                }
                g.setN(N);
            }
            else if (command == 'E') { //////////////////////////////////////////////////////////////
                input >> edge_vector;
                std::cout << line << std::endl;
                if (input.fail()) {
                    std::cerr << "Error: parsing the number of edges!\n";
                    break;
                }

                if(!g.is_ready_for_setEdge()){
                    std::cerr << "Error: Enter number of vertices first!\n";
                    g.set_to_false();
                    break;
                }
                if(!g.trySetEdges(edge_vector)){
                    break;
                }
            }
            else if (command == 's') { //////////////////////////////////////////////////////////////
                input >> source;
                if (input.fail()) {
                    std::cerr << "Error: parsing source!\n";
                    break;
                }
                input >> destination;
                if (input.fail()) {
                    std::cerr << "Error: parsing destination!\n";
                    break;
                }

                if(!g.is_ready_for_bfs()){
                    std::cerr << "Error: The definition of the graph is not complete yet!\n";
                    break;
                }
                if(!g.isValidNode(source)){
                    std::cerr << "Error: Node "<< std::to_string(source) <<" does not exist!\n";
                    break;
                }
                if(!g.isValidNode(destination)){
                    std::cerr << "Error: Node "<< std::to_string(destination) <<" does not exist!\n";
                    break;
                }
                if(source == destination){
                    std::cerr << "Error: Source and destination are the same!\n";
                    break;
                }

                std::cout<<g.bfs(source, destination)<<std::endl;
            }
            else {
                std::cerr << "Error: Wrong input\n";
                break;
            }
        }
    // g.print_graph();
    }
}
