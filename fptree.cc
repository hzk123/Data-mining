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
/************************************************************************/
/* predefined value                                                                                   */
/************************************************************************/
#define TRANSACTION_NUMBER 9
#define MINMUN_SUPPORT_RATE 0.2
#define NON_EXIST -100
/************************************************************************/
/* inline function                                                                                        */
/************************************************************************/
inline int getThreshold()
{
	return ceil(TRANSACTION_NUMBER*MINMUN_SUPPORT_RATE);
}

/************************************************************************/
/* type definition                                                                                      */
/************************************************************************/
/*FP-Tree�ڵ�����ݽṹ*/
typedef struct FpNode
{
	unsigned int frequence;
	string itemName;
	FpNode * parent;
	vector<FpNode *>children;
	FpNode * nextfriend;
	bool operator < (FpNode const & Node) const
	{
		return frequence < Node.frequence;
	}
}node;
/*Header table�еı���*/
struct headerItem
{
	string item;
	unsigned int frequence;
	list<node *> phead;
	bool operator <(headerItem const & other) const
	{
		return  frequence < other.frequence;
	}
};
/*���뽻�׼�¼�����ݱ����ʽ*/
typedef struct transaction
{
	string tranId;
	unsigned int ItemNum;
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
		exit(-1);
	}
	string rowContent;
	while (getline(fin, rowContent))
	{
		istringstream row(rowContent);
		transaction trans_temp;
		row >> trans_temp.tranId;
		row >> trans_temp.ItemNum;
		for (int i = 0; i < trans_temp.ItemNum; i++)
		{
			node tempNode;
			tempNode.children.clear();
			tempNode.parent = NULL;
			tempNode.nextfriend = NULL;
			row >> tempNode.itemName;
			tempNode.frequence = -1;
			trans_temp.Items.push_back(tempNode);
		}
		transactions.push_back(trans_temp);
	}
	return transactions;
}
/************************************************************************/
/* �Խڵ�����ıȽϺ���                                                                     */
/************************************************************************/
bool cmp(const node &a, const node &b)
{
	return (a.frequence > b.frequence);
}
/************************************************************************/
/* ����vector<transaction>,ͳ��ÿ��item���ִ���                                    */
/************************************************************************/
map<string, int> getFrequentItem(vector<transaction>const & transactions)
{
	map<string, int > result;
	for (auto p = transactions.cbegin(); p != transactions.cend(); p++)
	{
		for (auto q = p->Items.cbegin(); q != p->Items.cend(); q++)
		{
			result[q->itemName]++;
		}
	}
	return result;
}
/************************************************************************/
/* ȥ��map<string,int> 1-Ƶ�����в�Ƶ����                                                 */
/************************************************************************/
void deleteUnfrequentItem(map<string, int>& result)
{
	vector<string> deleteKeys;
	for (auto p = result.begin(); p != result.end(); p++)
	{
		if (p->second < getThreshold())
		{
			deleteKeys.push_back(p->first);
		}
	}
	for (auto p = deleteKeys.begin(); p != deleteKeys.end(); p++)
	{
		result.erase(*p);
	}
}
/************************************************************************/
/* ����map�������transactions�е�Item���ţ����map����û�У���ֱ��delete  */
/************************************************************************/
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
		//ɾ����Ƶ��Ԫ�أ��޸���Ƶ�ʺ󣬶�������Ƶ������
		p->Items.sort(cmp);
		//������󣬴�ʱvector�������Ϊ׼������Fp-tree���������������Ƶ����Ϊ1
		for (auto ptr = p->Items.begin(); ptr != p->Items.end(); ptr++)
		{
			ptr->frequence = 1;
		}
	}
	//��ȡÿ��transaction�����ź��frequent item
	for (auto p = transactions.begin(); p != transactions.end(); p++)
	{
		vector<node>temp(p->Items.begin(), p->Items.end());
		reRankResult.push_back(temp);
	}
	return reRankResult;
}
/************************************************************************/
/* ����vector<vector<node>>���б������ɴ�С���е�HeaderTable  */
/************************************************************************/
vector<headerItem> makeheaderTable(vector<vector<node>>const &Fitems)
{
	//����Fitems,��map������м���
	map<string, int> mapItem;
	vector<headerItem> headerTable;
	for (auto p = Fitems.cbegin(); p != Fitems.cend(); p++)
	{
		for (auto q = p->cbegin(); q != p->cend(); q++)
		{
			if (mapItem.find(q->itemName) == mapItem.end())
			{
				//��Ƶ����û����mapItem��,��ӽ�ȥ������Ϊ���ʼƵ��
				mapItem[q->itemName] = q->frequence;
			}
			else
			{
				//��mapItem�У�ֱ�Ӽ�node��Ƶ��
				mapItem[q->itemName] += q->frequence;
			}
		}
	}
	//�������ȥ��map�в�Ƶ������Ŀ
	deleteUnfrequentItem(mapItem);
	//��map���ֵ���ƽ�headerItem��
	for (auto p = mapItem.begin(); p != mapItem.end(); p++)
	{
		headerItem tempItem;
		tempItem.item = p->first;
		tempItem.frequence = p->second;
		tempItem.phead.clear();
		headerTable.push_back(tempItem);
	}
	//��headerTable�е�Ԫ���ɴ�С����
	sort(headerTable.rbegin(), headerTable.rend());
	return headerTable;
}
/************************************************************************/
/* ����Item,���Ҹýڵ��Ƿ���headerTable�У���������λ��                    */
/************************************************************************/
int findPosInheaderTable(string Item, vector<headerItem>const& headerTable)
{
	int pos = NON_EXIST;
	for (int p = 0; p != headerTable.size(); p++)
	{
		if (headerTable[p].item == Item)
		{
			pos = p;
		}
	}
	return pos;
}
/************************************************************************/
/* ����һ��node,�жϴ�node�Ƿ���vector<node*>��                                                                     */
/************************************************************************/
int findPosInChildren(node s, vector<node*>const & children)
{
	int pos = NON_EXIST;
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->itemName == s.itemName)
		{
			pos = i;
		}
	}
	return pos;
}
/************************************************************************/
/* ����vector<vector<node>>���б�����FP-tree                                   */
/************************************************************************/
node *  CreateFpTree(vector<vector<node>>const &Fitems, vector<headerItem>& headerTable)
{
	//����FP-tree��ͷ�ڵ㲢��ʼ��
	node* HeaderNode = new node;
	HeaderNode->itemName = "";
	HeaderNode->nextfriend = NULL;
	HeaderNode->parent = NULL;
	HeaderNode->frequence = 0;
	HeaderNode->children.clear();
	//ע�⣺��FP-treeʱ����Ҫ��֤Fitems�У������ǰ��ճ��ִ����ݼ����еģ������޷�����FP-tree
	for (int i = 0; i < Fitems.size(); i++)
	{
		//ÿһϵ�еĽ��׼�¼����һ������λ
		//���ȶ�λ�˽��׼�¼����Ԫ����FP-tree�е�λ��
		node* NodeInFp = HeaderNode;
		int j = 0;//��¼��ǰ���ȶԵ�Fitems[i](vector)�е�node�����
		while (j != Fitems[i].size())
		{
			//�ڵ�ǰ��FPNode�в��Ҵ�����Ľڵ�
			int pos = findPosInChildren(Fitems[i][j], NodeInFp->children);
			if (pos != NON_EXIST)
			{
				//����ҵ�������ǰ���ӵ�Ƶ�����ӣ�FPNodeָ��ú��ӣ����亢���в�����һ���ڵ�
				NodeInFp->children[pos]->frequence += Fitems[i][j].frequence;
				NodeInFp = NodeInFp->children[pos];
				j++;
			}
			else
			{
				//û�ҵ�����Fitems[i][j]�Ժ��Ԫ��(����j)ȫ��new �ڵ㣬�����ӵ�headerTable��
				while (j != Fitems[i].size())
				{
					node * new_node = new node;
					*new_node = Fitems[i][j];
					//ÿ����һ���ڵ㣬������Ƿ���HeaderTable�У�����ڣ�����ָ������ָ��
					int pos = findPosInheaderTable(new_node->itemName, headerTable);
					if (pos != NON_EXIST)
					{
						headerTable[pos].phead.push_back(new_node);
					}
					//��ǰ�ڵ�new_node��parent��NodeInFp,�ѵ�ǰ�ڵ����FP-tree,��ʹ����Ϊ�µ�NodeInFp
					NodeInFp->children.push_back(new_node);
					new_node->parent = NodeInFp;
					NodeInFp = new_node;
					j++;
				}
				break;
			}
		}
	}
	return HeaderNode;
}
/************************************************************************/
/* ����HeaderTable�е�ĳһ�����Ӧ��FP-tree,��ȡ��Ӧ��Ƶ��ģʽ��vector<vector<node>> */
/************************************************************************/
vector<vector<node>> getFrequentModeBase(headerItem const &hitem, node * const &fpTreeRoot)
{
	vector<vector<node>> frequentModes;
	for (auto p = hitem.phead.begin(); p != hitem.phead.end(); p++)
	{
		vector<node> frequentMode;
		node * ptr = *p;
		int currentFrequency = ptr->frequence;
		//��ptrָ������Ѱ�Ҹ��׽ڵ�
		while (ptr->parent != NULL)
		{
			if (ptr->parent->itemName != "")
			{
				node temp = *(ptr->parent);
				temp.frequence = currentFrequency;
				temp.children.clear();
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
/************************************************************************/
/*�����б��е�����һ��ݹ�������е�Ƶ���                                             */
/************************************************************************/
void  generateFrequentItems(vector<headerItem>& headerTable, node * &fpTreeRoot, string &base,
	list<frequentMode>& modeLists)
{
	if (headerTable.empty())
	{
		return;
	}
	for (auto p = headerTable.rbegin(); p != headerTable.rend(); p++)
	{
		/*����headerTable�е�ÿһ��*/
		frequentMode model;
		model.mode = base + " " + p->item;
		model.count = p->frequence;
		modeLists.push_back(model);
		//����currentMode������ģʽ��
		vector<vector<node>> ConditionalmodeBase = getFrequentModeBase(*p, fpTreeRoot);
		//��������ģʽ���������µ�HeaderTable��Fp-tree
		vector<headerItem> headerTable2 = makeheaderTable(ConditionalmodeBase);
		node * fptree2 = CreateFpTree(ConditionalmodeBase, headerTable2);
		string s2 = base + " " + (p->item);
		generateFrequentItems(headerTable2, fptree2, s2 , modeLists);
	}
}
/************************************************************************/
/* output all frequent Items                                                                     */
/************************************************************************/
void outputItems(list<frequentMode> const &modeLists)
{
	cout << "Frequent Items:" << endl;
	for (auto p = modeLists.cbegin(); p != modeLists.cend(); p++)
	{
		cout << p->mode << "\t" << p->count << endl;
	}
}

int main()
{
	string path = "in.txt";
	vector<transaction> transactions = readFile(path);//argv[1]�������ļ���·���������ļ���ʽ���ո�Ŀ¼��transactions�ļ�
	map<string, int> result = getFrequentItem(transactions);
	deleteUnfrequentItem(result);//��ȡһ��Ƶ����
	vector<vector<node>>rerankResult = reRankFrequentItem(result, transactions);//�����н����һ��Ƶ����˳������
	vector<headerItem> headerTable = makeheaderTable(rerankResult);//��ʼ��HeaderTable
	node * fpTreeRoot = CreateFpTree(rerankResult, headerTable);//��ʼ��Fp-tree
	list<frequentMode> modeLists;//����Ƶ����
	string tmp="";
	generateFrequentItems(headerTable, fpTreeRoot, tmp, modeLists);//�ݹ��ھ�Fp-tree
	outputItems(modeLists);//�������Ƶ���

	return 0;
}
