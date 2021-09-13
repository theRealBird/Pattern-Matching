#include <bits/stdc++.h>
using namespace std;

class Vertex { 

    private:

    Vertex* address;
    unordered_map <char,Vertex*> children;
    Vertex* parent;
    char prevChar;
    Vertex* suffixLink;
    Vertex* endWordLink;
    int wordID;
    bool leaf;

    public:

    Vertex():address(this) , children({}) , parent(nullptr) , prevChar('*') , suffixLink(nullptr) , endWordLink(nullptr) , wordID(0) , leaf(false) {}

    Vertex(const Vertex& v): address(v.address) , children(v.children) , parent(v.parent) , prevChar(v.prevChar) , suffixLink(v.suffixLink) , endWordLink(v.endWordLink) , wordID(v.wordID) , leaf(v.leaf){}

    Vertex& operator= (const Vertex& v){
        address = v.address;
        children = v.children;
        parent = v.parent;
        prevChar = v.prevChar;
        suffixLink = v.suffixLink;
        endWordLink = v.endWordLink;
        wordID = v.wordID;
        leaf = v.leaf;

        return *this;
    }

    ~Vertex(){}

    friend class Trie;

};

class Trie {

    private:

    Vertex* root;
    int wordIndex;
    vector <string> patterns; 

    public:
    
    Trie(): wordIndex((int)0) , root(nullptr) , patterns(vector <string> (0,"")) {

        root = new Vertex();
        wordIndex = 0;

    }

    void addPattern(string pattern){

        Vertex*current = root;

        for(int i=0;i<pattern.size();i++){
            char c = pattern[i];

            if(current->children.find(c) == current->children.end()){
                
                Vertex* newState = new Vertex();
                newState->parent = current->address;
                newState->prevChar = c;
                current->children[c] = newState->address;
                current = current->children[c];

            }

            else{

                current = current->children[c];

            }
        }

        current->leaf = true;
        current->wordID = wordIndex;
        wordIndex++;
        patterns.push_back(pattern); 

    }


    void makeLinks(Vertex* vertex){

        if(vertex==root){

            vertex->suffixLink = root;
            vertex->endWordLink = root;
            return;

        }

        else if(vertex->parent == root){
            
            vertex->suffixLink = root;

            if(vertex->leaf == true){
                vertex->endWordLink = vertex;
            }

            else{
                vertex->endWordLink = root;
            }

            return;

        }

        else{

            Vertex* parentSuffix = vertex->parent->suffixLink;
            char c = vertex->prevChar;

            while(true){

                if(parentSuffix->children.find(c)!=parentSuffix->children.end()){

                    vertex->suffixLink = parentSuffix->children[c];
                    break;

                }

                if(parentSuffix == root){
                    vertex->suffixLink = root;
                    break;
                }

                else{

                    parentSuffix = parentSuffix->parent->suffixLink;

                }

            }

            if(vertex->leaf == true){

                vertex->endWordLink = vertex;

            }

            else{

                vertex->endWordLink = vertex->suffixLink->endWordLink;

            }

        }

    }

    void processTrie(){

        queue <Vertex*> q;
        q.push(root);

        while(!q.empty()){

            int size = q.size();
            
            for(int i=0;i<size;i++){
                
                makeLinks(q.front());

                for(auto x: q.front()->children){
                    q.push(x.second);
                }

                q.pop();
            }
        }
    }

    vector<string> search (string text){
            
        Vertex* current = root;
        vector <string> matches;
        int n = text.size();

        for(int i=0;i<n;i++){

            while(true){

                if(current->children.find(text[i])!=current->children.end()){
                    current = current->children[text[i]];
                    break;
                }

                else{
                    current = current->suffixLink;
                }

                if(current == root){
                    break;
                }
            }

            Vertex* Check = current;

            while(true){

                Check = Check->endWordLink;

                if(Check == root){
                    break;
                }

                else{
                    matches.push_back(patterns[Check->wordID]);
                    Check = Check->suffixLink;
                }
            }
        }

        return matches;
            
    }

    void DestructorHelp (Vertex* v){

        for(auto x : v->children){

            char tempChar = x.first;
            Vertex* tempAddress = x.second;

            DestructorHelp(x.second);

        }

        delete v;
    }

    ~Trie(){

        DestructorHelp(root);

    }

};





//TEST
int main(){
    string text = "BUY TSLA NOW IT WILL GO UP TOMORROW";
    vector <string> pats;
    pats.push_back("TSLA");
    pats.push_back("SLA");
    pats.push_back("GAOF");

    Trie x;

    for(int i=0;i<pats.size();i++){
        x.addPattern(pats[i]);
    }
    
    x.processTrie();
    
    vector<string> o = x.search(text);

    for(int i=0;i<o.size();i++){
        cout<<o[i]<<"\n";
    }
}

