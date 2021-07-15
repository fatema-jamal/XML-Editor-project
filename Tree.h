#pragma once
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <fstream>
#include <String>
#include <stack>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
class Node
{
    string tagName;
    vector<string> tagValue;
    vector<int> tagValuePosition;
    vector<pair <string, string>> attribute;
    vector<Node*> children;
    vector<pair<string, int>> comments;
    bool emptyTag;
    friend class Tree;
};
class Tree
{
protected:
    Node* root;
    Node* newRoot;
    vector<string> preOrder;
    vector<string> postOrder;
    queue<string> q;
    queue<string> qCheck;
    fstream in_file, out_file;
    string in_file_name;
    vector<int> errorPostion;
    bool errorFound;
    void initilization();
    void initTagName(Node*);
    void postOrderFormat();
    void convert(Node*);
    void printPreOrder();
    void printPostOrder();
    void inFormat(Node*, int);
    void recursiveSort(Node*);
    void recursiveCopy(Node*, Node*);
    void reformat1();
    void reformatForChecking();
    string ltrim(string*);
    string rtrim(string*);
    string trim(string&);
    string repeat(string, int);
    void recursiveConvert(Node*, int, bool);
    bool Check();
public:
    Tree(string);
    void formatingFile(string);
    void sortChildren();
    void copyTree();
    void Minifying(string);
    void convertToJson(string);
    void printErrors(string);

};
#endif