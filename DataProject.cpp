#include <iostream>
#include <fstream>
#include <String>
#include <stack>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
const string WHITESPACE = " \n\r\t\f\v";
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
    vector<pair <string, string>> attribute;
    vector<Node*> children;
    vector<pair<string, int>> comments;
    bool emptyTag;

};
class Tree
{public:
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
        int size = q->front().length();
        if (q->front()[size - 2] == '/')
            p->emptyTag = true;
        else
            p->emptyTag = false;

        if (p->emptyTag)
            found = q->front().find("/", 1);
        else
            found = q->front().find(">", 1);
        tempFound = q->front().find(" ", 1);
        if (found > tempFound)
        {

            string t = q->front().substr(tempFound + 1, found - tempFound - 1);
            found = tempFound;
            t = trim(t);
            int sizeTemp = t.length();
            size_t x, y;
            string tempName, tempValue;
            for (int i = 0; i < sizeTemp; i++)
            {
                x = t.find('=', i);
                tempName = t.substr(i, x - i);
                y = t.find('"', x + 2);
                tempValue = t.substr(x + 1, y - x);
                p->attribute.push_back(make_pair(tempName, tempValue));
                i = y + 1;
            }
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
    void inFormat(Node* p, int level)
    {
        if (p == nullptr)
            return;
        string input = repeat("\t", level);
        int attSize = p->attribute.size();
        string tempAttribute = (p->attribute.empty()) ? "" : " ";
        for (int i = 0; i < attSize; i++)
        {
            tempAttribute.append(p->attribute[i].first + "=" + p->attribute[i].second);
            if (i != attSize - 1)
            {
                tempAttribute.append(" ");
            }
        }
        if (p->emptyTag)
            tempAttribute.append(" /");
        *createdFile << input << "<" + p->tagName + tempAttribute + ">" << endl;
        int len = p->children.size() + p->comments.size();
        len = (p->tagValue.empty()) ? len : len + p->tagValue.size();
        int commentCount = 0;
        int tagValueCount = 0;
        int childCount = 0;
        level++;
        string tempInput = repeat("\t", level);
        for (int i = 0; i < len; i++)
        {
            if (p->comments.size() > commentCount)
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
        if (!p->emptyTag)
            *createdFile << input << "</" + p->tagName + ">" << endl;
    }
    void sortChildren()
    {
        recursiveSort(root);
    }
    void recursiveSort(Node* p)
    {
        int n = p->children.size();
        sort(p->children.begin(), p->children.end(), [](Node* a, Node* b) { return a->tagName < b->tagName; });
        for (int i = 0; i < n; i++)
        {
            recursiveSort(p->children[i]);
        }
    }
};




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
                temp = newFormat.substr(i, found - i + 1);
                q->push(temp);
                i = found;
            }
            else
            {
                found = newFormat.find('<', i);
                temp = newFormat.substr(i, found - i);
                q->push(trim(temp));
                i = found - 1;
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
    for (int i = 0; i < p->tagValue.size(); i++)
    {
        if (first == 0) {
            first = 1;
            pra.push("}");
            cout << "{" << endl;
            cout << "\"" << p->tagName << "\":{" << endl;

            for (int h = 0; h < p->tagValue.size(); h++)
            {
                //cout << h;
                if (h == 0) {
                    cout << "\t"<< "\"#text\":";
                }
                if (p->tagValue.size() > 1 && h == 0) {
                    cout << "[";
                }
                if (p->tagValue.size() == 1) {
                    cout << p->tagValue[h] << endl;
                }
                if (p->tagValue.size() > 1&& h==0) {
                    cout << p->tagValue[h] ;
                }
                    if (p->tagValue.size() > 1 && h != 0) {
                        cout << ","<< p->tagValue[h];;
                    }
                    if (p->tagValue.size() > 1 && h == p->tagValue.size() - 1) {
                        cout << "]"<<endl;
            }
            }
            pra.push("}");
        }

    }
    int f = 0; 
    //int g = 0;
    for (int i = 0; i < p->children.size(); i++)
    {
        /*if (p->children[i]->tagName == p->children[i + 1]->tagName)
        {*/
        tab.push(i + 1);
       
        for (int u = 0; u <= g; u++) {
            cout << "\t";
        }
        int t = p->children.size();
       // if (i!=0)
        int repeat = 0;
        //cout << p->children[i]->tagName << p->children[i-1]->tagName;
        if ( !p->children[i]->children.size()   ) {
          //  cout << p->children[i]->children.size() << i;
            if (i != p->children.size()-1 && p->children[i]->tagName == p->children[i + 1]->tagName) {
                
               // if (p->children[i]->tagName == p->children[i+1]->tagName) {
                cout << "\"" << p->children[i]->tagName << "\":" << endl;
                cout <<"["<<
                //}
                i++;
            }
            else {

                cout << "\"" << p->children[i]->tagName << "\":" << endl;
                for (int h = 0; h < p->children[i]->tagValue.size(); h++)
                {
                    for (int u = 0; u <= g; u++) {
                        cout << "\t";
                    }
                    if (h == 0) {
                        cout << "\t" << "\"#text\":";
                    }
                    // cout << p->children[i]->tagValue.size();
                    if (p->children[i]->tagValue.size() > 1 && h == 0) {
                        cout << "[";
                    }

                    if (p->children[i]->tagValue.size() == 1) {
                        cout << p->children[i]->tagValue[h] << endl;
                    }
                    if (p->children[i]->tagValue.size() > 1 && h == 0) {
                        cout << p->children[i]->tagValue[h];
                    }
                    if (p->children[i]->tagValue.size() > 1 && h != 0) {
                        cout << "," << p->children[i]->tagValue[h];;
                    }
                    if (p->children[i]->tagValue.size() > 1 && h == p->children[i]->tagValue.size() - 1) {
                        cout << "]" << endl;
                    }
                    ///// <summary>
                    ///// //////////////////
                    ///// </summary>
                    ///// <param name="p"></param>
                    ///// <param name="g"></param>

                    //out << p->children[i]->tagValue[h] << endl;
                }
            }
        }
        else {
            f++;
            cout << "\"" << p->children[i]->tagName << "\":{" << endl;
           
            for (int h = 0; h < p->children[i]->tagValue.size(); h++)
            {
                for (int u = 0; u <= g; u++) {
                  if(h==0)
                    cout << "\t";
                }
                if (h == 0) {
                    cout << "\t" << "\"#text\":";
                }
                // cout << p->children[i]->tagValue.size();
                if (p->children[i]->tagValue.size() > 1 && h == 0) {
                    cout << "[";
                }

                if (p->children[i]->tagValue.size() == 1) {
                    cout << p->children[i]->tagValue[h] << endl;
                }
                if (p->children[i]->tagValue.size() > 1 && h == 0) {
                    cout << p->children[i]->tagValue[h];
                }
                if (p->children[i]->tagValue.size() > 1 && h != 0) {
                    cout << "," << p->children[i]->tagValue[h];;
                }
                if (p->children[i]->tagValue.size() > 1 && h == p->children[i]->tagValue.size() - 1) {
                    cout << "]" << endl;
                }
              //  cout << p->children[i]->tagValue[h] << endl;
            }

        }
        //first = 0;
        g++;
        sss(p->children[i], g);
        g--;
        int y = tab.top();
        tab.pop();
       if (p->children[i]->children.size() != 0) {
        for (int h = g + 1; h > 0; h--) {
            cout << "\t";

        }
      
           // cout << f;
          //  cout << p->children[i]->children.size();
            cout << "}" << endl;
        }
        //}
           // sss(p->children[i]);

       //  sss(p->children[i]);

     //}


    }
    //cout << "}"<<endl << "}";
}
int main()
{
    fstream readingFile("o.xml", ios::in);
    fstream createdFile("new.xml", ios::out);
    queue<string> q;
    Reformat(&readingFile, &q);
    //string tp;
    //while (getline(readingFile, tp))
    //{
    //    tp = trim(tp);
    //    q.push(tp);
    //}

    Tree tr = Tree(&q);
    //tr.sortChildren();
    tr.formatingFile(&createdFile);

    // Minifying(&readingFile, &createdFile);

    sss(tr.root, 0);
    readingFile.close();
    createdFile.close();
}

