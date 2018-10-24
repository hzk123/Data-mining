# Report About Association Rule Mining

# dataset
dataset_group.csv from kaggle
test.txt test2.txt test3.txt form IBM Quest Data Generator
 
# FP-growth

finished in C++

## Something about this code
1. I first use **convey-data.py** to translate data 
2. run FP-growth to find Frequent Item set
3. take a little reference to https://github.com/LandyGuo/FpTree  , but his Code is wrong , I just take the data structure
4. Only find frequent itemset
5. I use too much **map<string,int>** , so the time cost will be multpied by a log(n) , if i do hash first and use **map<int,int>** , the cost will redures a log(n) 
6. I use brute force to find if the item is in a node's child, It will cost many time in the root. 
## speed


1. 10K items support = 100 , about S
2. 1k items support = 10 , about 40S
3. 100K items support = 100 , about 60S


# Aprior 
finished in python 
�H�ͭW�u�A�ڥ�python

1. reference : code in <<��????>> , but every code in Internet looks very fimilar to each other.
2. Can find association rule

## speed
without build and compile time
1. 10K items support = 500 , about 85S
2. 1K items suport = 10 , about 0.63s
3. 100 k items support = 500, can't finish .

# Compare
�bC2���j�_1e3���e�AAprior�OFp-growth���t�׬ۮt���j�A��C2�D�`�j?�A�Y�K�ڪ�fp-growth�ϥ�map<string,int>,������?�]��Aprior�n�ܦh�C