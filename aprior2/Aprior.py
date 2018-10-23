import numpy as np
import os

Hash={};
ReHash={};
def convey():
    File = open("dataset_group.csv",'r');
    s = File.readlines();
    cnt = 0;
    data=[];
    for row in s:
        row = row[:-1]
        [a,b,c] = row.split(',');
        if c not in Hash:
            cnt+=1;
            Hash[c] = cnt;
            ReHash[cnt] = c;
        data.append([eval(b),Hash[c]]);
    return data ;

def loaddata():

    ret = [ []  for i in range(1000)];
    File = open("test.txt","r");
    content = File.readlines();
    now = 0;
    for line in content:
        tmp = line.split(' ');
        a = eval(tmp[0]);b = eval(tmp[1]);
        ret[b].append(a);
    File.close();
    return ret ;

def loadKaggledata():
    data  = convey();
    ret = [ [] for i in range(2000)];
    for [a,b] in data:
        ret[a].append(b);
    return ret ;

def getC(L , K):
    C = [];
    n = len(L);
    for i in range(n):
        L1 = L[i];
        for j in range(i+1,n):
            L2 = L[j];
            if ( len(L1 & L2) == K - 2):
                tmp = L1 | L2;
                if (tmp not in C):
                    C.append(tmp);
    return C;
def getL(Data,C,minsupport):
    Count={};
    D2 = [ item for item in Data if len(item) >= len(C[0])];
    for d in D2:
        for c in C:
            if c.issubset(d):
                if c not in Count:
                    Count[c] = 1;
                else:
                    Count[c] += 1;

    print(Count);
    L = [];
    supportdata ={};
    for key in Count:
        support = Count[key] ;
        if (support >= minsupport):
            L.append(key);
        supportdata[key] = support;

    return L , supportdata;
def aprior(dataset,minsup ):
    data = loaddata();
    c1 = [];
    for st in data:
        for item in st:
            if not[item] in c1:
                c1.append([item]);
    c1.sort();
    C1 = list(map(frozenset,c1));
    L1 , supportdata = getL(dataset,C1,minsup);
    L = [];
    L.append(L1);
    K = 2;
    while (1):
        preL = L[K-2];
        CK = getC(preL,K);
        if (len(CK)==0): break;
        #print("C%d:"%K,end=""),print(CK);
        Lk, supK = getL(dataset, CK, minsup);
        supportdata.update(supK);
        #print("L%d:"%K,end=""),print(Lk);
        L.append(Lk);
        if (len(Lk) == 0):
            break;
        K += 1;
    return L , supportdata;


def calc2(st,H,supportdata,ret,minconfidence = 0.05):
    pH = [];
    for subst in H:
        conf = supportdata[st] / supportdata[st - subst];
        if ( conf >= minconfidence):
            ret.append( (st-subst,subst,conf) );
            pH.append(subst);
    return st;

def calc3(st,H,supportdata,ret,minconfidence = 0.05):
    m = len(H[0]);
    if ( len(st) > m + 1  ):
        H1 = getC(H,m+1);
        H1 = calc2(st,H1,supportdata,ret,minconfidence);
        if (len(H1) > 1):
            calc3(st,H,supportdata,ret,minconfidence);


def gen(L,supportdata,minconfidence = 0.05):
    largeitem = [];
    for i in range(1,len(L)):
        for st in L[i]:
            H1 = [frozenset([item]) for item in st];
            if (i > 1):
                calc2(st,H1,supportdata,largeitem,minconfidence);
            else:
                calc2(st,H1,supportdata,largeitem,minconfidence);
    return  largeitem;


data  = loadKaggledata();
L , supportdata = aprior(data,minsup=100);
rules = gen(L , supportdata , 0);



for item in rules:
    st1=[];
    st2=[];
    for c in item[0]:
        st1.append(ReHash[c]);
    for c in item[1]:
        st2.append(ReHash[c]);
    print(st1,"->",st2,item[2]);
    
