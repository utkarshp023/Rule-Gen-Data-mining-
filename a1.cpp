#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include<vector>
#include <bits/stdc++.h>
#include<math.h>

#define THRESH 300
#define THRESH1 0.5

using namespace std;

void tokenize(int p);
int convert(char *item);
int cur_rule_index[2];
struct tree
{
    // This vector would store all the ineces of the hashed FK to that node.....
    vector<int> vec;
    //int arr[15][50],
    //int count[15],
    int flag; // replace arr[15] with vector // size of Fk / 3^k
    struct tree *child[3];
}*head=NULL,*link=NULL;

struct vecStr
{
    int arr[50];
};

// structure that stores the entire Fk's and counts..
struct store
{
    vector<vecStr> finF;
    vector<int> finCnt;
} stfoo[51];

typedef struct vecStr vecStr;
typedef struct tree tree;
char mat[200][50];
int subset_len;
static int count_1[200];
//check[1000];
//static int F1[200]; // vector f &fk
//F[1000][50],Fk[1000][50],
vector<int> F1,trans,next,cnt,temp;
vector<vecStr> F,Fk;
FILE *fp=fopen("groceries.csv","r+");
ofstream myfile;


void tokenize(int p)//p=0 gets distinct items from data and stores in mat; else we get pth transaction items
{
    // cout<<"Hi"<<" Token\n";
    //if(p!=0) printf("trans: %d",p);
    int i=0,j;
    char buffer[1024],*record,*token;
    while(fgets(buffer,1024,fp)!=NULL)
    {
        record = strtok(buffer,"\n");
        while(record!=NULL)
        {
            i++;
            if(p!=0 && i>p)
            {
                rewind(fp);
                return;
            }
            token = strtok(record,",");
            while(token!=NULL)
            {
                if(p==0)
                {
                    count_1[convert(token)]=count_1[convert(token)]+1;//F[1] is counted here
                }
                else if(i==p)
                {
                    trans.push_back(convert(token));
                }
                token = strtok(NULL,",");
            }
            record = strtok(NULL,"\n");
        }
    }
    rewind(fp);
    return;
}

int convert(char *item) //returns a corresponding number; to reverse just use mat matrix
{

    int i=1;
    while(strcmp(mat[i],"")!=0)
    {
        if(strcmp(mat[i],item)==0)
        {
            return i;
        }
        i++;
    }
    strcpy(mat[i],item);
    return i;
}

int apriori(int k)//checks if temp array follows apriori principle for cardinality k
{
    // cout<<"Hi"<<" Ap\n";

    int i=0,j=0,m=1,flag=0;
    //for(i=0;i<temp.size();i++)cout<<temp[i]<<" ";

    //while(F[m][0]>0)
    for(m=0; m<F.size(); m++)
    {
        i=0;
        for(j=0; j<k; j++)
        {
            if(F[m].arr[j]==temp[i]) i++;
            else if(F[m].arr[j]!=temp[i+1]) break;
            else i++;
        }
        if(j==(k)) flag++;
        //m++;
    }
    //cout<<"flag: "<<flag<<endl;
    if(flag==(k+1)) return 1;
    else return 0;
}

void generate(int k) //generates itemset of cardinality k
{
    int i=0,j=0,m=0;
    //while(F1[m]>0) //set threshold for F[1]
    for(m=0; m<F1.size(); m++)
    {
        for(i=0; i<F.size(); i++) //Every itemset in F[k-1] is checked as i increments for every corresponding F[1]'s lth element
        {
            j=0;
            while(j<k)
            {
                if(F[i].arr[j]>=F1[m]) break;//if any element in itemset is matched with F[1] then break
                temp.push_back(F[i].arr[j]);
                j++;
            }
            vecStr var1;
            if(j==k && temp[j-1]<F1[m])
            {
                temp.push_back(F1[m]);
                if(apriori(k)==1)
                {
                    for(j=0; j<=k; j++)
                    {
                        var1.arr[j] = temp[j];
                        //cout<<"temp: "<<temp[j]<<"var1.arr[j]: "<<var1.arr[j]<<"   ";
                        //Fk[s].arr[j]=temp[j];
                    }
                    //cout<<endl;
                    Fk.push_back(var1);
                    cnt.push_back(0);
                }
            }
            temp.erase(temp.begin(),temp.end());
            //i++;
        }
        // m++;
    }/*
    for(i=0;i<Fk.size();i++)
                    for(j=0; j<=k; j++)
                    {cout<<Fk[i].arr[j]<<" ";}
    cout<<endl;*/

}


void sort(int k)
{
    int i=1,j,m,t;
    //while(Fk[i][0]>0)
    for(i=0; i<Fk.size(); i++)
    {
        sort(Fk[i].arr,Fk[i].arr+k+1);
    }
}

void hashtree(int curr_lev,int max_lev,tree *node)
{
    if(head == NULL)
    {
        tree *tem = new tree;
        tem->child[0] = new tree;
        tem->child[0]->flag=0;
        tem->child[1] = new tree;
        tem->child[1]->flag=0;
        tem->child[2] = new tree;
        tem->child[2]->flag=0;
        head = tem;
        node=tem;
        node->flag=1;
        for(int i=0; i<Fk.size(); i++)
        {
            int m = (Fk[i].arr[0])%3;
            tem->child[m]->vec.push_back(i);
        }

    }
    else
    {
        //cout<<"hey my size: "<<node->vec.size()<<endl;
        if(curr_lev>max_lev) return;
        else if(node==NULL) return;
        else if((node->vec.size())>9) //(Fk.size()/pow(3,max_lev))){
        {
            node->flag=1;
            node->child[0] = new tree;
            node->child[0]->flag=0;
            node->child[1] = new tree;
            node->child[1]->flag=0;
            node->child[2] = new tree;
            node->child[2]->flag=0;
            for(int i=0; i<node->vec.size(); i++)
            {
                int m = (Fk[node->vec[i]].arr[curr_lev])%3;
                node->child[m]->vec.push_back(node->vec[i]);
            }
            node->vec.erase (node->vec.begin(),node->vec.end());//node which is subdivided its elemts are deleted

        }
        else return;
    }

    hashtree(curr_lev+1,max_lev,node->child[0]);
    hashtree(curr_lev+1,max_lev,node->child[1]);
    hashtree(curr_lev+1,max_lev,node->child[2]);
}

void check(int max_lev)
{
    //cout<<"hi check\n";
    //cout<<"next: "<<next[0]<<" "<<next[1]<<endl;
    int curr_lev=0,j;
    tree *node=head;
    while(curr_lev<max_lev && node->flag==1) //node->vec.size()==0){
    {
        int m=next[curr_lev]%3;
        //cout<<"m :"<<m<<endl;
        node=node->child[m];
        curr_lev++;
    }
    // cout<<"vec: ";
    //for(int i=0;i<node->vec.size();i++) cout<<node->vec[i]<<" ";
    //cout<<endl;
    for(int i=0; i<node->vec.size(); i++)
    {
        //cout<<"size: "<<node->vec.size()<<endl;
        for(j=0; j<max_lev; j++)
        {
            //cout<<Fk[node->vec[i]].arr[j]<<" ";
            if(Fk[node->vec[i]].arr[j]!=next[j]) break;
        }
        //cout<<"\n";
        if(j==max_lev) cnt[node->vec[i]]++;
    }
    next.erase(next.begin(),next.end());
}
void supportcount(int start,int curr_len,int max_len,bool used[])
{
    if(curr_len==max_len)
    {
        //cout<<"trans: ";
        for(int i=0; i<trans.size(); i++)
        {
            if(used[i]==true)
            {
                next.push_back(trans[i]);
                //  cout<<trans[i]<<" ";
            }
        }
        //cout<<"\n";
        check(max_len);
        return;
    }
    if(start==trans.size()) return;
    used[start]=true;
    supportcount(start+1,curr_len+1,max_len,used);
    used[start]=false;
    supportcount(start+1,curr_len,max_len,used);
}

void checkV2(int len)
{
    vector<int> store_rest;

    int j;
    int cntNum = stfoo[cur_rule_index[0]].finCnt[cur_rule_index[1]]; //Support count of numenator
    for(int i =0; i<stfoo[subset_len].finF.size(); i++)
    {
        for(j=0; j<subset_len; j++)
        {
            if((stfoo[subset_len].finF[i].arr[j])!=next[j]) break;  /*Here we find where the subset is situated in its own Fk*/
        }
        //if(j==cur_rule_index[0]-1)
        if(j==subset_len)
        {
            //int cntDen = stfoo[cur_rule_index[0]-1].finCnt[i]; //Support count of denominator
            int fl = 1;
            int cntDen = stfoo[subset_len].finCnt[i];
            float ans=cntNum/cntDen;
            if(ans>THRESH1)
            {

                cout<<"Rule :";
                myfile << "Rule : {";

                for(j=0; j<next.size(); j++)
                {
                    cout<<next[j]<< " ";
                    myfile << mat[next[j]]<<" ";
                    if(j != next.size()-1){
                        cout<<",";
                        myfile<<",";
                    }
                }
                cout<< "->"<<" ";

                myfile <<"} "<< "-->"<<" {";
                for(int l=0; l<cur_rule_index[0]; l++)
                {
                    for(j=0; j<next.size(); j++)
                    {
                        if(stfoo[cur_rule_index[0]].finF[cur_rule_index[1]].arr[l] == next[j]) fl=0;
                    }
                    if(fl)
                    {
                        store_rest.push_back(stfoo[cur_rule_index[0]].finF[cur_rule_index[1]].arr[l]);
                    }
                    fl = 1;
                }
                for(j=0; j<store_rest.size(); j++)
                {
                    cout<<store_rest[j]<< " ";
                    myfile << mat[store_rest[j]]<<" ";
                    if(j != store_rest.size()-1){
                        cout<<",";
                        myfile<<",";
                    }
                }
                cout<<"\n";
                myfile<<"}"<<"\n";
                break;
            }
        }
    }
    next.erase(next.begin(),next.end());
}
void subSetGen(int start,int curr_len,int max_len,bool used[])
{
    if(curr_len==max_len)
    {
        //cout<<"trans: ";
        for(int i=0; i<cur_rule_index[0]; i++)
        {
            if(used[i]==true)
            {
                next.push_back(stfoo[cur_rule_index[0]].finF[cur_rule_index[1]].arr[i]);
                //  cout<<trans[i]<<" ";
            }
        }
        //cout<<"\n";
        checkV2(max_len);
        return;
    }
    if(start==cur_rule_index[0]) return;
    used[start]=true;
    subSetGen(start+1,curr_len+1,max_len,used);
    used[start]=false;
    subSetGen(start+1,curr_len,max_len,used);
}


void rulegen(int k)
{
    next.erase(next.begin(),next.end());
    for(int i=2; i<k; i++)
    {
        for(int j=0; j<stfoo[i].finF.size(); j++)
        {
            // Find (k-1) subsets
            cur_rule_index[0] = i; //the stfoo index i.e. i = cardinality
            cur_rule_index[1] = j; //the current item set being read
            bool used[i]; //used will be of size 3 for F3's subset generation
            for(int m=0; m<i; m++) used[m]=false; //cout<<trans[j]<<" ";}
            subset_len = i-1;
            /*Calculate all the subsets and then find out all the possible rules*/
            while(subset_len)
            {
                //cout<<"Cardinality :"<<i<<" Index "<<j<<" Subset "<<subset_len<<endl;
                //myfile<<"Cardinality :"<<i<<" Index "<<j<<" Subset "<<subset_len<<endl;
                subSetGen(0,0,subset_len,used);
                subset_len--;
            }
        }
    }

}

int main()
{
    int i,j,k=1;
    myfile.open ("output.txt");
    tokenize(0);
    //j=0;
    for(i=0; i<200; i++)
    {

        if (count_1[i]>THRESH) // threshold = 1000
        {
            //F[j].arr[0]=i;
            vecStr var;
            var.arr[0] = i;
            F.push_back(var);
            F1.push_back(i);
            //F1[j]=i;
            //j++;
            stfoo[1].finCnt.push_back(count_1[i]);

        }
    }
    vecStr var4;
    for(i=0; i<F1.size(); i++)
    {
        var4.arr[0] = F1[i];

        stfoo[1].finF.push_back(var4);
    }

    //infinity LOOP
    while(F.size()>0)
    {
        //i=0;
        // cout<<"Size "<<F.size()<<endl;

        generate(k);
        sort(k);

        head=NULL;
        //erase previous hash tree
        hashtree(0,k,NULL);
        k++;
        i=1;
        tokenize(i);
        sort(trans.begin(),trans.end());

        while(trans.size()!=0)
        {
            //  cout<<i<<endl;
            bool used[trans.size()];
            for(j=0; j<trans.size(); j++) used[j]=false; //cout<<trans[j]<<" ";}
            //cout<<"\n";
            supportcount(0,0,k,used);
            trans.erase(trans.begin(),trans.end());
            i++;
            tokenize(i);
            sort(trans.begin(),trans.end());
        }

        F.erase(F.begin(),F.end());
        vecStr var3;
        for(i=0; i<Fk.size(); i++)
        {
            if(cnt[i]>THRESH) //Threshold check
            {
                for(j=0; j<k; j++)
                {
                    var3.arr[j] = Fk[i].arr[j];
                }
                F.push_back(var3);
                stfoo[k].finCnt.push_back(cnt[i]);
            }
        }
        //Wait for it
        vecStr var2;
        for(i=0; i<F.size(); i++)
        {
            for(j=0; j<k; j++)
            {
                var2.arr[j] = F[i].arr[j];
            }
            stfoo[k].finF.push_back(var2);
        }
        Fk.erase(Fk.begin(),Fk.end());
    }
    rulegen(k);
    myfile.close();
    /*
            for(i=0;i<stfoo[2].finF.size();i++)
            {
                printf("%d: {",i);
               // j=0;
               // while(F[i].arr[j]>0)
               for(j=0;j<k;j++)
                {
                    printf("%d ",stfoo[2].finF[i].arr[j]);
                }
                printf("} --%d\n",stfoo[2].finCnt[i]);
               // i++;
            }
    */
}

