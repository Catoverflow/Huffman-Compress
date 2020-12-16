#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#define BUFF_SIZE 2000
using namespace std;
//Huffman tree node
struct Huff
{
    size_t weight{0};
    char code; //code: 0~255
    Huff *lchild{nullptr}, *rchild{nullptr};
};
//Huffman tree node for decoding
struct Huff_D
{
    Huff_D *lchild{nullptr}, *rchild{nullptr};
    char code; //code: -128~127c
};
//Append HuffTree to Standard I/O, and get codemap[source]=huffcode
void OutputHuffTree(ostream &out, Huff *root, vector<bool> &path, vector<vector<bool>> &codemap)
{
    if (!root->lchild and !root->rchild) //leaf node
    {
        codemap[root->code + 128] = path;
        out << char(path.size());
        unsigned char code = 0;
        int ind = 0;
        for (bool c : path)
        {
            code = (code << 1) + c;
            ++ind;
            if (ind == 8)
            {
                ind = 0;
                out << code;
                code = 0;
            }
        }
        if (ind)
            out << code;
        out << root->code;
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
        OutputHuffTree(out, root->rchild, path, codemap);
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
        if (argv[1][0] != '-')
            Throw("No Option Specified");
        if (argv[1][1] == 'u')
            WorkMode = 1;
        else if (argv[1][1] == 'z')
            WorkMode = 0;
        else
            Throw("Cannot Read Augments");
        InFilePath = argv[2];
        OutFilePath = InFilePath + ((argv[1][1] == 'z') ? ".huff" : ".src");
        break;
    case 4: //huff -ur/zr infile outfile
        if (argv[1][0] != '-')
            Throw("No Option Specified");
        if ((argv[1][1] == 'u' and argv[1][2] == 'r') or (argv[1][1] == 'r' and argv[1][2] == 'u'))
            WorkMode = 1;
        else if ((argv[1][1] == 'z' and argv[1][2] == 'r') or (argv[1][1] == 'r' and argv[1][2] == 'z'))
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
        const size_t FileSize = in.tellg();
        in.seekg(0, in.beg);
        if (!in)
            Throw("Cannot Read File to Memory");
        //count occurance
        char *buffer = new char[BUFF_SIZE]();
        size_t *Times = new size_t[256]();
        for (size_t i = 0; i < FileSize; ++i)
        {
            if (!(i % BUFF_SIZE)) //read new buffer block
                in.read(buffer, BUFF_SIZE);
            Times[buffer[i % BUFF_SIZE] + 128]++; //char: -128~127 +128 -> 0~255
        }
        //construct huffman tree
        struct cmp //use ascending priority queue
        {
            bool operator()(const Huff *n1, const Huff *n2) { return n1->weight > n2->weight; }
        };
        priority_queue<Huff *, vector<Huff *>, cmp> NodeList;
        for (int i = 0; i < 256; ++i)
            if (Times[i])
            {
                Huff *p = new Huff;
                p->weight = Times[i];
                p->code = i - 128;
                NodeList.push(p);
            }
        ofstream out(OutFilePath, ios::out | ios::binary);
        out << char(NodeList.size() - 1); //output treesize to head part, -1 for 256->255
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
        vector<vector<bool>> codemap;
        vector<bool> path;
        codemap.resize(256);
        if (!out)
            Throw("Cannot Open Output File");
        OutputHuffTree(out, root, path, codemap);
        //output compressed bitstream
        size_t bilen = 0;
        char *outputbuffer = new char[BUFF_SIZE]();
        in.close();
        in.open(InFilePath, ios::in | ios::binary); //reopen file
        for (size_t i = 0; i < FileSize; ++i)
        {
            if (!(i % BUFF_SIZE)) //read new buffer block
                in.read(buffer, BUFF_SIZE);
            for (bool c : codemap[buffer[i % BUFF_SIZE] + 128])
            {
                outputbuffer[(bilen / 8) % BUFF_SIZE] = (outputbuffer[(bilen / 8) % BUFF_SIZE] << 1) + c;
                if (!(++bilen % (BUFF_SIZE * 8))) //buffer full, write to file
                    out.write(outputbuffer, BUFF_SIZE);
            }
        }
        if (bilen % (8 * BUFF_SIZE))
            out.write(outputbuffer, (bilen / 8) % BUFF_SIZE + 1);
        //output align len
        out << char(8 - bilen % 8);
    }
    else //uncompress mode
    {
        ifstream in(InFilePath, ios::in | ios::binary);
        if (!in)
            Throw("Cannot Open File");
        //read file
        in.seekg(-1, in.end);
        size_t FileSize = in.tellg(); //ignore last and first node(align/treesize)
        char align;
        in.read(&align, 1); //read align size at last char
        if (!in or align > 7)
            Throw("File Corrupted");
        in.seekg(0, in.beg);
        char *buffer = new char[BUFF_SIZE](), hufflen;
        //create huffman tree
        Huff_D *root = new Huff_D, *t;
        if (!in)
            Throw("File Corrupted");
        unsigned treesize = in.get() + 1, pos = 0; //position in buffer
        for (; treesize > 0; --treesize)           //read &create huffman tree
        {
            t = root;
            if (!(pos % BUFF_SIZE))
                in.read(buffer, BUFF_SIZE);
            for (hufflen = buffer[(pos++) % BUFF_SIZE]; hufflen > 0; hufflen -= 8)
            {
                if (!(pos % BUFF_SIZE))
                    in.read(buffer, BUFF_SIZE);
                for (int j = hufflen > 8 ? 7 : hufflen - 1; j >= 0; --j)
                {
                    if ((buffer[pos % BUFF_SIZE] >> j) % 2)
                    {
                        if (!t->rchild)
                            t->rchild = new Huff_D;
                        t = t->rchild;
                    }
                    else
                    {
                        if (!t->lchild)
                            t->lchild = new Huff_D;
                        t = t->lchild;
                    }
                }
                ++pos;
            }
            if (!(pos % BUFF_SIZE))
                in.read(buffer, BUFF_SIZE);
            t->code = buffer[(pos++) % BUFF_SIZE];
        }
        //read, uncompress and output bitdata
        ofstream out(OutFilePath, ios::out | ios::binary);
        char *outputbuffer = new char[BUFF_SIZE]();
        unsigned outputpos = 0;
        for (t = root; pos < FileSize - 1; ++pos)
        {
            if (!(pos % BUFF_SIZE))
                in.read(buffer, BUFF_SIZE);
            for (int j = (pos == FileSize - 2) ? 7 - align : 7; j >= 0; --j) //note align for last node
            {
                if ((buffer[pos % BUFF_SIZE] >> j) % 2)
                    t = t->rchild;
                else
                    t = t->lchild;
                if (!t->lchild and !t->rchild) //reach leaf node
                {
                    outputbuffer[outputpos++ % BUFF_SIZE] = t->code;
                    if (!(outputpos % BUFF_SIZE)) //buffer full, write to file
                        out.write(outputbuffer, BUFF_SIZE);
                    t = root;
                }
            }
        }
        if (t != root)
            Throw("File Corrupted");
        out.write(outputbuffer, outputpos % BUFF_SIZE); //clean buffer
    }
    return 0;
}
