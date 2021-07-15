#include "Tree.h"
void Tree::initilization()
{
    while (q.front()[1] == '?' || q.front()[1] == '!')
    {
        preOrder.push_back(q.front());
        q.pop();
    }
    initTagName(root);
}

void Tree::initTagName(Node* p)
{
    size_t found;
    size_t tempFound;
    int size = q.front().length();
    if (q.front()[size - 2] == '/')
        p->emptyTag = true;
    else
        p->emptyTag = false;

    if (p->emptyTag)
        found = q.front().find("/", 1);
    else
        found = q.front().find(">", 1);
    tempFound = q.front().find(" ", 1);
    if (found > tempFound)
    {

        string t = q.front().substr(tempFound + 1, found - tempFound - 1);
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

    p->tagName = q.front().substr(1, found - 1);
    q.pop();
}

void Tree::postOrderFormat()
{
    if (!q.empty())
    {
        postOrder.push_back(q.front());
        q.pop();
    }
}

void Tree::convert(Node* p)
{
    Node* temp;
    string name = p->tagName;
    int commentPosition = 0;
    while (q.front() != ("</" + name + ">"))
    {
        if (q.front()[0] != '<')
        {
            p->tagValue.push_back(q.front());
            p->tagValuePosition.push_back(commentPosition);
            q.pop();
        }
        else
        {
            if (q.front()[1] == '!')
            {
                p->comments.push_back(make_pair(q.front(), commentPosition));
                q.pop();
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
    q.pop();
}

void Tree::printPreOrder()
{
    for (int i = 0; i < preOrder.size(); i++)
    {
        out_file << preOrder[i] << endl;
    }
}

void Tree::printPostOrder()
{
    for (int i = 0; i < postOrder.size(); i++)
    {
        out_file << postOrder[i] << endl;
    }
}

void Tree::inFormat(Node* p, int level)
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
    out_file << input << "<" + p->tagName + tempAttribute + ">" << endl;
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
                out_file << tempInput << p->comments[commentCount].first << endl;
                commentCount++;
                continue;
            }
        }
        if (p->tagValuePosition.size() > tagValueCount)
        {
            if (p->tagValuePosition[tagValueCount] == i)
            {
                out_file << tempInput << p->tagValue[tagValueCount] << endl;
                tagValueCount++;
                continue;
            }
        }

        inFormat(p->children[childCount], level);
        childCount++;
    }
    if (!p->emptyTag)
        out_file << input << "</" + p->tagName + ">" << endl;
}

void Tree::recursiveSort(Node* p)
{
    int n = p->children.size();
    sort(p->children.begin(), p->children.end(), [](Node* a, Node* b) { return a->tagName < b->tagName; });
    for (int i = 0; i < n; i++)
    {
        recursiveSort(p->children[i]);
    }
}

void Tree::recursiveCopy(Node* first, Node* second)
{
    second->tagName = first->tagName;
    second->emptyTag = first->emptyTag;
    int tagValueSize = first->tagValue.size();
    int attributsSize = first->attribute.size();
    int commentsSize = first->comments.size();
    int childrenSize = first->children.size();
    for (int i = 0; i < tagValueSize; i++)
    {
        second->tagValue.push_back(first->tagValue[i]);
        second->tagValuePosition.push_back(first->tagValuePosition[i]);
    }
    for (int i = 0; i < attributsSize; i++)
    {
        second->attribute.push_back(first->attribute[i]);
    }
    for (int i = 0; i < commentsSize; i++)
    {
        second->comments.push_back(first->comments[i]);
    }
    Node* temp;
    for (int i = 0; i < childrenSize; i++)
    {
        temp = new Node();
        recursiveCopy(first->children[i], temp);
        second->children.push_back(temp);
    }

}

Tree::Tree(string in)
{
    in_file_name = in;
    root = new Node();
    reformat();
    initilization();
    convert(root);
    postOrderFormat();
}

void Tree::formatingFile(string out)
{
    out_file.open(out, ios::out | ios::trunc);
    printPreOrder();
    inFormat(root, 0);
    printPostOrder();
    out_file.close();
}

void Tree::sortChildren()
{
    recursiveSort(newRoot);
}

void Tree::copyTree()
{
    newRoot = new Node();
    recursiveCopy(root, newRoot);
}
void Tree::reformat()
{
    in_file.open(in_file_name, ios::in);
    if (in_file.is_open())
    {
        string newFormat = "";
        string tp;
        while (getline(in_file, tp))
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
                q.push(temp);
                i = found;
            }
            else
            {
                found = newFormat.find('<', i);
                temp = newFormat.substr(i, found - i);
                q.push(trim(temp));
                i = found - 1;
            }
        }
    }
    in_file.close();
}
void Tree::Minifying(string out)
{
    in_file.open(in_file_name, ios::in);
    out_file.open(out, ios::out | ios::trunc);
    if (in_file.is_open())
    {

        string tp;
        while (getline(in_file, tp))
        {

            tp = trim(tp);
            out_file << tp;
        }
    }
    out_file.close();
    in_file.close();
}
string Tree::ltrim(string* s)
{
    size_t start = s->find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s->substr(start);
}
string Tree::rtrim(string* s)
{
    size_t end = s->find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s->substr(0, end + 1);
}
string Tree::trim(string& s)
{
    return rtrim(&(ltrim(&s)));
}
string Tree::repeat(string str, int n)
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
void Tree::recursiveConvert(Node* p, int level, bool printName)
{
    // pinter Node , int level , bool for print name
    //  no print { } in case no att and no child and one value
    // "name" : ---> in case one child
    // attribute 
    // value ----> in case one child or in case attribute print #text
    //      -----> else print tagvalue only
    //       -----> [] in case many values and one child or more  
    // print child 
     //<id/>
    int childNum = p->children.size();
    int tagValueNum = p->tagValue.size();
    int attNum = p->attribute.size();
    string taps = "";
    taps = repeat("\t", level);
    vector<string> uniqueName;
    /*if (!printName)
    {

    }*/
    if (printName)
    {
        out_file << taps << "\"" + p->tagName + "\"" + ":";
    }
    if (childNum == 0 && attNum == 0)
    {

        if (!printName)
        {
            out_file << taps + "\""+p->tagValue[0]+"\"";
        }
        else
            out_file << "\"" + p->tagValue[0]+"\"";

    }
    //<id name="sdsf" age="sdsd"></id>

    else
    {
        if (!printName)
            out_file << taps + "{" << endl;
        else
            out_file << "{" << endl;
        level++;
        taps = "";
        taps = repeat("\t", level);
        for (int i = 0; i < attNum; i++)
        {
            if(!p->emptyTag)
                out_file << taps << +"\"@" + p->attribute[i].first + "\"" << ":" << p->attribute[i].second << "," << endl;
            else
            {
                if (i != attNum - 1)
                    out_file << taps << +"\"@" + p->attribute[i].first + "\"" << ":" << p->attribute[i].second << "," << endl;
                else
                    out_file << taps << +"\"@" + p->attribute[i].first + "\"" << ":" << p->attribute[i].second << endl;
            }
        }
        if (tagValueNum > 0)
        {
            out_file << taps << "\"#text\":";
            bool printComa = false;
            if (tagValueNum > 1)
            {
                out_file << "[";
                printComa = true;
            }

            for (int i = 0; i < tagValueNum; i++)
            {
                out_file << "\""+p->tagValue[i]+"\"";
                if (printComa && (tagValueNum - 1 != i))
                    out_file << ",";
            }
            if (tagValueNum > 1)
                out_file << "]";
            if (childNum > 0)
                out_file << "," << endl;
            else
                out_file << endl;

        }

        bool found = false;
        int count = 0;
        for (int i = 0; i < childNum; i++)
        {
            for (int j = 0; j < uniqueName.size(); j++)
            {
                if (uniqueName[j] == p->children[i]->tagName)
                {
                    found = true;
                }
            }
            if (found)
            {
                found = false;
                continue;
            }

            else
                uniqueName.push_back(p->children[i]->tagName);
            int vv;
            for (int k = 0; k < childNum; k++)
            {
                if (p->children[i]->tagName == p->children[k]->tagName)
                {
                    count++;
                    vv = k;
                }
            }
            if (count > 1)
            {
                /*taps = "";
                taps = repeat("\t", level);*/
                out_file << taps + "\"" + p->children[i]->tagName + "\"" + ":" + "[" << endl;
                level++;
                for (int j = i; j < childNum; j++)
                {
                    if (p->children[i]->tagName == p->children[j]->tagName)
                    {
                        if (!p->children[i]->emptyTag)
                        {
                            recursiveConvert(p->children[j], level, false);

                            if ((vv) != j)
                            {
                                out_file << "," << endl;
                            }
                            else
                                out_file << endl;
                        }
                        else
                        {
                            if (p->children[j]->attribute.size() > 0)
                            {
                                recursiveConvert(p->children[j], level, false);
                                if ((vv) != j)
                                {
                                    out_file << "," << endl;
                                }
                                else
                                    out_file << endl;
                            }
                        }

                    }
                }
                level--;
                taps = "";
                taps = repeat("\t", level);
                out_file << taps << "]" ;
                if (i+count != childNum)
                    out_file <<"," << endl;
                else
                    out_file << endl;
                count = 0;
            }
            else
            {
                /* int x = level - 2;
                 taps = repeat("\t", x);*/
                if (!p->children[i]->emptyTag)
                {
                    recursiveConvert(p->children[i], level, true);
                    if (i != childNum - 1)
                        out_file << "," << endl;
                    count = 0;
                }
                else
                {
                    if (p->children[i]->attribute.size() > 0)
                    {
                        recursiveConvert(p->children[i], level, true);
                        out_file << "," << endl;
                        count = 0;
                    }
                    else
                    {
                        out_file << taps << "\"" + p->children[i]->tagName + "\"" + ":" + " []";
                    }
                }

            }


        }
        out_file << endl;
        level--;
        taps = "";
        taps = repeat("\t", level);
        out_file << taps + "}";
    }
}
void Tree::convertToJson(string out)
{
    out_file.open(out, ios::out | ios::trunc);
    out_file << "{" << endl;
    recursiveConvert(root, 1, true);
    out_file << endl << "}";
    out_file.close();
}