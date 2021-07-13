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
    bool emptyTag;
    
};
class Tree
{
public : 
    Node * root;
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
        int size = q->front().length();
        if (q->front()[size - 2] == '/')
            p->emptyTag = true;
        else
            p->emptyTag = false;
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
                    if (temp->emptyTag)
                        continue;
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
        if(!p->emptyTag)
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
void Reformat(fstream* readingFile, queue<string>* q)
{
    if (readingFile->is_open())
    {
        string newFormat = "";
        string tp;
        while (getline(*readingFile, tp))
        {
            tp = trim(tp);
            newFormat.append(tp);
        }
        int n = newFormat.length();
        size_t found;
        string temp;
        for (int i = 0; i < n; i++)
        {
            if (newFormat[i] == '<')
            {
                found = newFormat.find('>', i);
                temp = newFormat.substr(i, found - i+1);
                q->push(temp);
                i = found;
            }
            else
            {
                found = newFormat.find('<', i);
                temp = newFormat.substr(i, found - i);
                q->push(trim(temp));
                i = found-1;
            }
        }
    }
}
int first = 0; 
void sss(Node* p, int g)
{
    //[li,o,s,li,s,p,o]
    //[li,li,o,o,p,s,s]
    stack <int>tab;
    stack <string>pra;
    stack <int >tab_name; 
    for (int i = 0; i < p->tagValue.size();i++)
    {
        if (first == 0) {
            first = 1;
            pra.push("}");
            cout << "{"<<endl;
            cout <<"\"" << p->tagName << "\":{" << endl;
        pra.push("}");
        }
        
    }
    int g = 0;
    for (int i = 0 ; i < p->children.size(); i++)
    {
        /*if (p->children[i]->tagName == p->children[i + 1]->tagName)
        {*/
        tab.push(i+1);
        for (int h = 1; h <= i+1; h++) {
            //cout << "\t";
        }
        for (int u = 0; u <= g; u++) {
            cout << "\t";
        }

            cout  << "\"" << p->children[i]->tagName << "\":{" << endl<<"\t";
           
            //first = 0;
            g++;
            sss(p->children[i],g);
            g--;
            int y = tab.top();
            tab.pop(); 
           for (int h =g+1; h > 0; h--) {
            cout << "\t";
            
        }
        cout << "}" << endl;
           // sss(p->children[i]);
            
          //  sss(p->children[i]);

        //}


    }
    //cout << "}"<<endl << "}";
}
int main()
{ 
   // cout << "\t" << "dDd";
    fstream readingFile("o.xml", ios::in);
    fstream createdFile("E.xml", ios::out);
    queue<string> q;
    Reformat(&readingFile, &q);
    //string tp;
    //while (getline(readingFile, tp))
    //{
    //    tp = trim(tp);
    //    q.push(tp);
    //}

    Tree tr = Tree(&q);
    tr.formatingFile(&createdFile);

    sss(tr.root,0);
   // Minifying(&readingFile, &createdFile);

    readingFile.close();
    createdFile.close();
}

