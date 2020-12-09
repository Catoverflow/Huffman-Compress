#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;
//Huffman tree node
struct Huff
{
    size_t weight{0};
    char code;
    Huff *lchild{nullptr}, *rchild{nullptr};
};
//Append HuffTree to Standard I/O, and get codemap[source]=huffcode
void OutputHuffTree(ostream &out, Huff *root, vector<bool> &path, vector<vector<bool>> &codemap)
{
    if (!root->lchild && !root->rchild) //leaf node
    {
        codemap[root->code] = path;
        out << path.size();
        char code = 0;
        for (int i = 0; i < path.size(); ++i)
        {
            if (!i % 8)
            {
                out << code;
                code = 0;
            }
            code = code << 1 + path[i];
        }
        out << code << root->code;
    }
    if (root->lchild)
    {
        path.push_back(0);
        OutputHuffTree(out, root->lchild, path, codemap);
        path.pop_back();
    }
    if (root->rchild)
    {
        path.push_back(1);
        OutputHuffTree(out, root->lchild, path, codemap);
        path.pop_back();
    }
}
//Print error and exit with code 1
void Throw(string Error)
{
    cerr << "\033[31mERROR: \033[0m" << Error << endl; //[red]ERROR: [/red]
    exit(1);
}
int main(int argc, char **argv)
{
    string InFilePath, OutFilePath;
    bool WorkMode = 0; //0->zip, 1->unzip
    //Parse Command
    switch (argc)
    {
    case 1: //huff
        Throw("No File Specified");
        break;
    case 2: //huff infile
        InFilePath = argv[1];
        OutFilePath = InFilePath + ".huff";
        break;
    case 3: //huff -u/z infile
        if (argv[1][0] != ' ')
            Throw("No Option Specified");
        if (argv[1] == "-u")
            WorkMode = 1;
        else if (argv[1] == "-z")
            WorkMode = 0;
        else
            Throw("Cannot Read Augments");
        InFilePath = argv[2];
        OutFilePath = InFilePath + ".huff";
        break;
    case 4: //huff -ur/zr infile outfile
        if (argv[1][0] != '-')
            Throw("No Option Specified");
        if (argv[1] == "-ur" or argv[1] == "-ru")
            WorkMode = 1;
        else if (argv[1] == "-zr" or argv[1] == "-rz")
            WorkMode = 0;
        else
            Throw("Cannot Read Augments");
        InFilePath = argv[2];
        OutFilePath = argv[3];
        break;
    default:
        Throw("Too Many Augments Passed");
        break;
    }
    //compress mode
    if (!WorkMode)
    {
        //read file
        ifstream in(InFilePath, ios::in | ios::binary);
        if (!in)
            Throw("Cannot Open File");
        in.seekg(0, in.end);
        size_t FileSize = in.tellg();
        in.seekg(0, in.beg);
        char *buffer = new char[FileSize];
        in.read(buffer, FileSize);
        if (!in)
            Throw("Cannot Read File to Memory");
        //count occurance
        size_t Times[256];
        for (int i = 0; i < 256; ++i)
            Times[i] = 0;
        for (size_t i = 0; i < FileSize; ++i)
            Times[buffer[i]]++;
        //construct huffman tree
        struct cmp //use ascending priority queue
        {
            bool operator()(const Huff *n1, const Huff *n2) { return n1->weight < n2->weight; }
        };
        priority_queue<Huff *, vector<Huff *>, cmp> NodeList;
        for (int i = 0; i < 256; ++i)
            if (Times[i])
            {
                Huff *p = new Huff;
                p->weight = Times[i];
                p->code = i;
                NodeList.push(p);
            }
        while (NodeList.size() > 1)
        {
            Huff *p, *t;
            p = NodeList.top();
            NodeList.pop();
            t = NodeList.top();
            NodeList.pop();
            Huff *h = new Huff;
            h->weight = p->weight + t->weight;
            h->lchild = p;
            h->rchild = t;
            NodeList.push(h);
        }
        Huff *root = NodeList.top();
        //output huffman tree
        char hufflen = 0, treesize = 0;
        vector<vector<bool>> codemap;
        vector<bool> path;
        codemap.resize(256);
        ofstream out(OutFilePath, ios::out | ios::binary | ios::app);
        if (!out)
            Throw("Cannot Open Output File");
        OutputHuffTree(out, root, path, codemap);
        //output compressed bitstream
        char *outputbuffer = new char[FileSize];
        size_t bilen = 0;
        for (size_t i = 0; i < FileSize; ++i)
            for (char j=0;j<codemap[buffer[i]].size();++j,++bilen)
                outputbuffer[bilen/8]=outputbuffer[bilen/8]<<1+codemap[buffer[i]][j];
        if(bilen%8) //align to block(8 bit)
            outputbuffer[bilen/8]<<=(bilen%8);
        //output margin
        out.seekp(0, out.beg);
        cout<<bilen%8;
    }
    else //decompress mode
    {
        ifstream in(InFilePath, ios::in | ios::binary);
        if (!in)
            Throw("Cannot Open File");
        in.seekg(0, in.end);
        size_t FileSize = in.tellg();
        in.seekg(0, in.beg);
        char *buffer = new char[FileSize];
        in.read(buffer, FileSize);
    }
    return 0;
}