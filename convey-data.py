import csv

def convey():
    File = open("dataset_group.csv",'r');
    File2 = open("Kaggle.txt","w");
    s = File.readlines();
    Hash={};
    cnt = 0;
    data=[];
    data_unhash=[];
    for row in s:
        row = row[:-1]
        [a,b,c] = row.split(',');
        if c not in Hash:
            cnt+=1;
            Hash[c] = cnt;
        data.append([eval(b),Hash[c]]);
        data_unhash.append([eval(b),c]);

    st=[];
    for [a,b] in data_unhash:
        if (len(st) == a ):
            st[a-1].append(b);
        else:
            st.append([b]);

    for itemset in st:
        print(len(itemset),end=" ",file=File2);
        for fuck in itemset:
            print(fuck.replace(" ","-"),end=" ",file = File2);
        print("",file=File2);
    File.close();
    File2.close();
convey()