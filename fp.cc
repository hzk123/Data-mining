#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<list>
#include<algorithm>
#include <bits/stdc++.h>
using namespace std;

#define NON_EXIST -12345
const int minsup = 50;
typedef struct Fptree
{
	unsigned int frequence;
	string itemName;
	Fptree * parent;
	vector<Fptree *>child;
	Fptree * nxt;
    Fptree() {}
	Fptree( int _fre,string item,Fptree * par,vector<Fptree *> _child , Fptree * _nxt)
	{
	    frequence = _fre;
	    itemName = item;
	    for (auto r : _child) child.push_back(r);
	    nxt = _nxt;
	}

    void operator = (const Fptree &r)
    {
        frequence = r.frequence;
        itemName = r.itemName;
        parent = r.parent;
        for (auto c: r.child) child.push_back(c);
        nxt = r.nxt;
    }
}node;

bool cmp(const node &a, const node &b) {return (a.frequence > b.frequence);}

struct head
{
	string item;
	unsigned int frequence;
	list<node *> phead;
	bool operator <(head const & other) const { return  frequence < other.frequence; }
};
typedef struct transaction
{
	string tranId;
	int ItemNum;
	list<node> Items;
}transaction;

struct frequentMode
{
	string mode;
	int count;
};

pair<string,int> ret;

vector<transaction> readFile(string file)
{
	vector<transaction> transactions;
	ifstream fin(file);
	if (fin.fail())
	{
		cout << "ERROR: CAN NOT OPEN FILE!" << endl;
		exit(-2333);
	}
	string row;
	while (getline(fin, row))
	{
		istringstream fuck(row);
		transaction trans_temp;
		fuck >> trans_temp.ItemNum;
		for (int i = 0; i < trans_temp.ItemNum; i++)
		{
			node tempNode;
			tempNode.child.clear();
			tempNode.parent = NULL;
			tempNode.nxt = NULL;
			fuck >> tempNode.itemName;
			tempNode.frequence = -1;
			trans_temp.Items.push_back(tempNode);
		}
		transactions.push_back(trans_temp);
	}

	cout << "Loadend" << endl;
	return transactions;
}
map<string, int> getFrequentItem(vector<transaction>const & transactions)
{
	map<string, int > result;
	for (auto p : transactions)
        for ( auto q : (p.Items)) result[q.itemName]++;

    vector<string> del;
    for (auto p : result)
        if ( p.second <minsup) del.push_back( p.first);
    for (auto p : del) result.erase(p);
	return result;
}


vector<vector<node>> reRankFrequentItem(map<string, int> const & result, vector<transaction>& transactions)
{
	vector<vector<node>> reRankResult;
	for (auto p = transactions.begin(); p != transactions.end(); p++)
	{
		auto q = p->Items.begin();
		while (q != p->Items.end())
		{
			if (result.find(q->itemName) == result.end())
				q = p->Items.erase(q);
			else
                q->frequence = result.at(q->itemName),q++;
		}
		p->Items.sort(cmp);
		for (auto ptr = p->Items.begin(); ptr != p->Items.end(); ptr++) ptr->frequence = 1;
	}
	for (auto p = transactions.begin(); p != transactions.end(); p++)
	{
		vector<node>temp(p->Items.begin(), p->Items.end());
		reRankResult.push_back(temp);
	}
	return reRankResult;
}

vector<head> makeheaderTable(vector<vector<node>>const &dataset)
{
	map<string, int> mp;
	vector<head> headerTable;
    for (auto p : dataset)
        for (auto q : p)
            mp[q.itemName] += q.frequence;
    vector<string> del;
    for (auto p : mp)
    if ( p.second < minsup) del.push_back( p.first );
    for (auto p : del) mp.erase(p);
    for (auto p : mp)
	{
		head tempItem;
		tempItem.item = p.first;
		tempItem.frequence = p.second;
		tempItem.phead.clear();
		headerTable.push_back(tempItem);
	}
	sort(headerTable.rbegin(), headerTable.rend());
	return headerTable;
}

int gethead(string Item, vector<head>const& headerTable)
{
    int pos = NON_EXIST , cnt = 0;
	for ( auto p : headerTable)
        { if ( p.item == Item) { pos = cnt ; break; } cnt++;}
	return pos;
}
int findPosInchild(node s, vector<node*>const & child)
{
	int pos = NON_EXIST , cnt = 0;
	for ( auto p : child)
        { if ( p->itemName == s.itemName) { pos = cnt ; break; } cnt++;}
	return pos;
}

node *  CreateFpTree(vector<vector<node>>const &dataset, vector<head>& headerTable)
{
	node* HeaderNode = new node;
	HeaderNode->itemName = "";
	HeaderNode->nxt = NULL;
	HeaderNode->parent = NULL;
	HeaderNode->frequence = 0;
	HeaderNode->child.clear();
	for (int i = 0; i < dataset.size(); i++)
	{
		node* now = HeaderNode;
		int j = 0;
		for ( ; j < dataset[i].size() ; j++)
		{
			int pos = findPosInchild(dataset[i][j], now->child);
			if (pos != NON_EXIST)
			{
				now->child[pos]->frequence += dataset[i][j].frequence;
				now = now->child[pos];
			}
			else
			break;
        }
        for ( ; j < dataset[i].size(); j++)
        {
            node * new_node = new node;
            *new_node = dataset[i][j];
            int pos = gethead(new_node->itemName, headerTable);
            if (pos != NON_EXIST) headerTable[pos].phead.push_back(new_node);
            now->child.push_back(new_node);
            new_node->parent = now;
            now = new_node;
        }


	}
	return HeaderNode;
}
vector<vector<node>> getFrequentModeBase(head const &hitem, node * const &fpTreeRoot)
{
	vector<vector<node>> frequentModes;
	for (auto p = hitem.phead.begin(); p != hitem.phead.end(); p++)
	{
		vector<node> frequentMode;
		node * ptr = *p;
		int currentFrequency = ptr->frequence;
		while (ptr->parent != NULL)
		{
			if (ptr->parent->itemName != "")
			{
				node temp(*(ptr->parent));
				temp.frequence = currentFrequency;
				temp.child.clear();
				temp.parent = NULL;
				frequentMode.push_back(temp);
			}
			ptr = ptr->parent;
		}
		reverse(frequentMode.begin(), frequentMode.end());
		frequentModes.push_back(frequentMode);
	}
	return frequentModes;
}
void  generateFrequentItems(vector<head>& headerTable, node * &fpTreeRoot, string &base,list<frequentMode>& modeLists)
{
	if (headerTable.empty()) return;
	for (auto p = headerTable.rbegin(); p != headerTable.rend(); p++)
	{
		frequentMode model;
		model.mode = base + " " + p->item;
		model.count = p->frequence;
		modeLists.push_back(model);
		vector<vector<node>> ConditionalmodeBase = getFrequentModeBase(*p, fpTreeRoot);
		vector<head> headerTable2 = makeheaderTable(ConditionalmodeBase);
		node * fptree2 = CreateFpTree(ConditionalmodeBase, headerTable2);
		string s2 = base + " " + (p->item);
		generateFrequentItems(headerTable2, fptree2, s2 , modeLists);
	}
}
void outputItems(list<frequentMode> const &modeLists)
{
	cout << "Frequent Items:" << endl;
	for (auto p = modeLists.cbegin(); p != modeLists.cend(); p++)
		cout << p->mode << " : " << p->count << endl;
}

int main()
{
    ios::sync_with_stdio(false);
	int start = clock();
	string path = "Kaggle.txt";
	// string path = "test_convey1.txt";
	// string path = "test_convey2.txt";
	// string path = "test_convey3.txt";
	cout << "Begin" << endl;
	vector<transaction> transactions = readFile(path);
	map<string, int> result = getFrequentItem(transactions);
	vector<vector<node>> rerankResult = reRankFrequentItem(result, transactions);
	vector<head> headerTable = makeheaderTable(rerankResult);
	node * fpTreeRoot = CreateFpTree(rerankResult, headerTable);
	list<frequentMode> modeLists;
	string tmp="";
	generateFrequentItems(headerTable, fpTreeRoot, tmp, modeLists);
	outputItems(modeLists);
	cout << -start + clock() << endl;
	return 0;
}
