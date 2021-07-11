#include <iostream>
#include <fstream>
#include <String>
#include <stack>
#include <vector>
#include <queue>
using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
string repeat(string str, int n)
{
    if (n == 0)
        return "";
    string s = str;
    for (int i = 1; i < n; i++)
    {
        s.append(str);
    }
    return s;
}
class Node
{
public:
    string tagName;
    vector<string> tagValue;
    vector<int> tagValuePosition;
    string attribute;
    vector<Node*> children;
    vector<pair<string,int>> comments;
    
};
class Tree
{
    Node* root;
    vector<string> preOrder;
    vector<string> postOrder;
    queue<string>* q;
    fstream* createdFile;
public:
    Tree(queue<string>* p)
    {
        root = new Node();
        q = p;
        initilization();
        convert(root);
        postOrderFormat();
    }
    void initilization()
    {
        while (q->front()[1] == '?' || q->front()[1] == '!')
        {
            preOrder.push_back(q->front());
            q->pop();
        }
        initTagName(root);
    }
    void initTagName(Node* p)
    {
        size_t found;
        size_t tempFound;
        tempFound = q->front().find(" ", 1);
        found = q->front().find(">", 1);
        if (found > tempFound)
        {
            p->attribute = q->front().substr(tempFound + 1, found - tempFound - 1);
            found = tempFound;
        }
        else
        {
           p->attribute = "";
        }
        p->tagName = q->front().substr(1, found - 1);
        q->pop();
    }
    void postOrderFormat()
    {
        if (!q->empty())
        {
            postOrder.push_back(q->front());
            q->pop();
        }
    }
    void convert(Node* p)
    {
        Node* temp;
        string name = p->tagName;
        int commentPosition = 0;
        while (q->front() != ("</" + name + ">"))
        {
            if (q->front()[0] != '<')
            {
                p->tagValue.push_back(q->front());
                p->tagValuePosition.push_back(commentPosition);
                q->pop();
            }
            else
            {
                if (q->front()[1] == '!')
                {
                    p->comments.push_back(make_pair(q->front(), commentPosition));
                    q->pop();
                }
                else
                {
                    temp = new Node();
                    initTagName(temp);
                    p->children.push_back(temp);
                    convert(temp);
                    
                }

            }
            commentPosition++;
        }
        q->pop();
    }

    void formatingFile(fstream* file)
    {
        createdFile = file;
        printPreOrder();
        inFormat(root, 0);
        printPostOrder();

    }
    void printPreOrder()
    {
        for (int i = 0; i < preOrder.size(); i++)
        {
            *createdFile << preOrder[i] << endl;
        }
    }
    void printPostOrder()
    {
        for (int i = 0; i < postOrder.size(); i++)
        {
            *createdFile << postOrder[i] << endl;
        }
    }
    void inFormat(Node* p,int level)
    {
        if (p == nullptr)
            return;
        string input = repeat("\t", level);
        string tempAttribute = (p->attribute == "") ? "" : " " + p->attribute;
        *createdFile << input << "<" + p->tagName + tempAttribute + ">" << endl;
        int len = p->children.size() + p->comments.size();
        len = (p->tagValue.empty()) ? len : len + p->tagValue.size();
        int commentCount=0;
        int tagValueCount = 0;
        int childCount = 0;
        level++;
        string tempInput= repeat("\t", level);
        for (int i = 0; i < len; i++)
        {
            if (p->comments.size()> commentCount)
            {
                if (i == p->comments[commentCount].second)
                {
                    *createdFile << tempInput << p->comments[commentCount].first << endl;
                    commentCount++;
                    continue;
                }
            }
            if (p->tagValuePosition.size() > tagValueCount)
            {
                if (p->tagValuePosition[tagValueCount] == i)
                {
                    *createdFile << tempInput << p->tagValue[tagValueCount] << endl;
                    tagValueCount++;
                    continue;
                }
            }
            
            inFormat(p->children[childCount], level);
            childCount++;
        }
        *createdFile << input << "</" + p->tagName + ">" << endl;
    }

};



string ltrim(string* s)
{
    size_t start = s->find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s->substr(start);
}
string rtrim(string* s)
{
    size_t end = s->find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s->substr(0, end + 1);
}
string trim(string& s) 
{
    return rtrim(&(ltrim(&s)));
}
void formatting(fstream* readingFile, fstream* createdFile)
{
    stack<string> st;
    int count = 0;
    bool first = true;

    if (readingFile->is_open())
    {
        string tp;
        string input;
        size_t found;
        size_t found2;
        size_t temp;
        while (getline(*readingFile, tp))
        {
            tp = ltrim(&tp);
            found = tp.find("<");
            if (found != string::npos)
            {
                if (tp[found + 1] != '?')
                {
                    if (tp[found + 1] == '!')
                    {
                        if (first)
                        {
                            input = repeat("\t", count);
                            *createdFile << input.append(tp) << endl;
                        }
                        else
                        {
                            count++;
                            input = repeat("\t", count);
                            *createdFile << input.append(tp) << endl;
                            count--;
                        }
                        continue;
                    }
                    found2 = tp.find(" ", found + 1);
                    temp = tp.find(">", found + 1);
                    found2 = min(found2, temp);
                    if (tp[found + 1] != '/')
                    {
                        st.push(tp.substr(found + 1, (found2 - found - 1)));
                        if (first)
                        {
                            input = repeat("\t", count);
                            *createdFile << input.append(tp) << endl;
                            first = false;
                            continue;
                        }
                        count++;
                        input = repeat("\t", count);
                        *createdFile << input.append(tp) << endl;
                        found = tp.find("<", found2 + 1);
                        if (found != string::npos)
                        {
                            if (tp[found + 1] == '/')
                            {
                                st.pop();
                                count--;
                            }
                        }
                    }
                    else
                    {
                        if (st.top() == tp.substr(found + 2, (found2 - found - 2)))
                        {
                            input = repeat("\t", count);
                            *createdFile << input.append(tp) << endl;
                            st.pop();
                            count--;
                        }
                    }


                }
                else
                {
                    input = repeat("\t", count);
                    *createdFile << input.append(tp) << endl;
                }
            }
            else
            {
                input = repeat("\t", count);
                *createdFile << input.append(tp) << endl;
            }
        }
    }
}
void Minifying(fstream* readingFile, fstream* createdFile)
{
    if (readingFile->is_open())
    {
        string tp;
        while (getline(*readingFile, tp))
        {
            tp = trim(tp);
            *createdFile << tp;
        }
    }
}

int main()
{

    fstream readingFile("E:/Electrical Engineer/3rd CSE/Second Term/Data Structures/data/test3.xml", ios::in);
    fstream createdFile("E:/Electrical Engineer/3rd CSE/Second Term/Data Structures/data/new.xml", ios::out);
    queue<string> q;
    string tp;
    while (getline(readingFile, tp))
    {
        tp = trim(tp);
        q.push(tp);
    }

    Tree tr = Tree(&q);
    tr.formatingFile(&createdFile);
    readingFile.close();
    createdFile.close();
}

